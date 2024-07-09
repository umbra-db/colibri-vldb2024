#include "singlestore/tatp/TatpClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Statement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
TatpClient::TatpClient(const Database& database, const odbc::Environment& environment, unsigned clientIndex)
   : SingleStoreClient(database, environment), generator(42 + clientIndex), transactionNumbers(database.getPipelineDepth()), subscriberCounts(database.getPipelineDepth()), dispatch(database.getPipelineDepth())
// Constructor
{
}
//---------------------------------------------------------------------------
void TatpClient::performWork()
// Perform work
{
   const auto pipelineDepth = database.getPipelineDepth();

   while (running()) {
      submittedTransactions += pipelineDepth;

      for (unsigned i = 0; i < pipelineDepth; ++i)
         transactionNumbers[i] = static_cast<SQLINTEGER>(generator.pickTransactionType());

      dispatch.exec();

      processedTransactions += pipelineDepth;
   }
}
//---------------------------------------------------------------------------
void TatpClient::initialize()
// Initialize the client
{
   SingleStoreClient::initialize();

   {
      odbc::Statement statement;
      statement.allocate(connection);
      statement.exec("SELECT COUNT(*) FROM subscriber");

      SQLINTEGER subscriberCount = 0;
      statement.bindColumn(1, subscriberCount);
      if (!statement.next())
         throw Exception("no data returned from query"sv);

      for (auto& v : subscriberCounts)
         v = subscriberCount;
   }

   dispatch.prepare(connection, "{CALL dispatch(?, ?)}");
   dispatch.bind(transactionNumbers.data(), subscriberCounts.data());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
