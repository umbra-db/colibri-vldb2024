#include "txbench/pq/PqConnection.hpp"
#include "txbench/Exception.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
#include "txbench/pq/PqResult.hpp"
#include <cassert>
#include <iostream>
#include <thread>
#include <libpq-fe.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
PqConnection::PqConnection()
   : con(nullptr)
// Constructor
{
}
//---------------------------------------------------------------------------
PqConnection::~PqConnection()
// Destructor
{
   close();
}
//---------------------------------------------------------------------------
void PqConnection::connect(const string& connectionString, unsigned timeout)
// Attempt to connect
{
   close();

   string errorMessage;
   auto endTime = chrono::steady_clock::now() + chrono::seconds(timeout);
   while (chrono::steady_clock::now() < endTime) {
      con = PQconnectdb(connectionString.c_str());
      if (auto res = PQstatus(con); res == CONNECTION_OK) {
         return;
      }

      // Remember the last error
      errorMessage = PQerrorMessage(con);

      // And retry
      close();
      this_thread::sleep_for(100ms);
   }

   throw Exception("connect(): " + errorMessage);
}
//---------------------------------------------------------------------------
void PqConnection::close()
// Close the PqConnection
{
   unique_lock lock(mutex);

   if (con) {
      PQfinish(con);
      con = nullptr;
   }
}
//---------------------------------------------------------------------------
PqResult PqConnection::execute(const string& statement)
// Execute an ad-hoc query
{
   PqResult result{PQexec(con, statement.c_str())};
   if (!result.success())
      throw Exception("exec(" + statement + "): " + result.errorMessage());

   return result;
}
//---------------------------------------------------------------------------
void PqConnection::cancel()
// Cancel the running queries
{
   unique_lock lock(mutex);

   PGcancel* handle = PQgetCancel(con);
   char errbuf[257];
   if (!PQcancel(handle, errbuf, 256)) {
      cerr << "cancel() " << errbuf << endl;
   }
   PQfreeCancel(handle);
}
//---------------------------------------------------------------------------
PqPreparedStatement PqConnection::prepare(const string& name, const std::string& query)
// Prepare the statement
{
   assert(con);

   {
      PqResult result(PQprepare(con, name.c_str(), query.c_str(), 0, nullptr));
      if (!result.success())
         throw Exception("prepare(" + name + ", " + query + "): " + result.errorMessage());
   }

   int numParams;
   {
      PqResult result(PQdescribePrepared(con, name.c_str()));
      if (!result.success())
         throw Exception("prepare(" + name + ", " + query + "): " + result.errorMessage());

      numParams = result.nparams();
   }

   return PqPreparedStatement(name, numParams);
}
//---------------------------------------------------------------------------
void PqConnection::enterPipelineMode()
// Enter pipeline mode
{
   assert(!pipelineMode);

   if (PQenterPipelineMode(con) != 1)
      throw Exception("enterPipelineMode(): " + string(PQerrorMessage(con)));

   pipelineMode = true;
}
//---------------------------------------------------------------------------
void PqConnection::send(const PqPreparedStatement& statement)
// Send a prepared statement
{
   [[maybe_unused]] auto validate = [&]() {
      for (auto& length : statement.paramLengths) {
         if (length == 0) return false;
      }
      for (auto& val : statement.paramValues) {
         if (val < statement.paramValuesBuffer.data() || val >= statement.paramValuesBuffer.data() + statement.paramValuesBuffer.size()) return false;
      }
      return true;
   };
   assert(validate());

   if (!PQsendQueryPrepared(con, statement.name.c_str(), statement.numParams, statement.paramValues.data(), statement.paramLengths.data(), statement.paramFormats.data(), 0))
      throw Exception("send(" + statement.name + "): " + string(PQerrorMessage(con)));
}
//---------------------------------------------------------------------------
PqResult PqConnection::receive()
// Receive a statement result
{
   // Get the actual result
   PqResult result{PQgetResult(con)};
   if (!result.success())
      throw Exception("receive(): " + result.errorMessage());
   if (PQgetResult(con) != nullptr)
      throw Exception("receive(): unexpected result"sv);

   return result;
}
//---------------------------------------------------------------------------
void PqConnection::sync()
// Synchronize the pipeline
{
   assert(pipelineMode);

   if (PQpipelineSync(con) != 1)
      throw Exception("sync(): " + string(PQerrorMessage(con)));

   PqResult result(PQgetResult(con));
   if (result.status() != PGRES_PIPELINE_SYNC)
      throw Exception("sync(): " + result.errorMessage());
   if (PQgetResult(con) != nullptr)
      throw Exception("sync(): unexpected result"sv);
}
//---------------------------------------------------------------------------
void PqConnection::exitPipelineMode()
// Exit pipeline mode
{
   assert(pipelineMode);

   if (PQexitPipelineMode(con) != 1)
      throw Exception("exitPipelineMode(): " + string(PQerrorMessage(con)));

   pipelineMode = false;
}
//---------------------------------------------------------------------------
void PqConnection::flush()
// Flush the pipeline
{
   assert(pipelineMode);

   if (PQflush(con) != 0)
      throw Exception("flush(): " + string(PQerrorMessage(con)));
}
//---------------------------------------------------------------------------
void PqConnection::sendFlushRequest()
// Enqueue a flush request
{
   assert(pipelineMode);

   if (PQsendFlushRequest(con) != 1)
      throw Exception("sendFlushRequest(): " + string(PQerrorMessage(con)));
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
