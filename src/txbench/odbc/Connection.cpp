#include "txbench/odbc/Connection.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Environment.hpp"
#include "txbench/odbc/OdbcError.hpp"
#include "txbench/odbc/Statement.hpp"
#include "txbench/util/sql_helper.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <sqlext.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
Connection::Connection()
// Constructor
{
}
//---------------------------------------------------------------------------
Connection::~Connection()
// Destructor
{
   disconnect();

   if (handle != SQL_NULL_HANDLE)
      SQLFreeHandle(SQL_HANDLE_ENV, handle);
}
//---------------------------------------------------------------------------
void Connection::connect(const Environment& environment, const string& connectionString, unsigned timeout)
// Attempt to connect
{
   // Allocate a handle for the connection
   if (auto ret = SQLAllocHandle(SQL_HANDLE_DBC, environment.handle, &handle); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_ENV, environment.handle));

   // Attempt to connect
   auto str = connectionString;

   string errorMessage;
   auto endTime = chrono::steady_clock::now() + chrono::seconds(timeout);
   auto counter = 0; // Try at least three times
   while (chrono::steady_clock::now() < endTime || counter < 3) {
      SQLSMALLINT outConnectionStringSize;
      if (auto ret = SQLDriverConnect(handle, nullptr, reinterpret_cast<SQLCHAR*>(str.data()), str.size(), nullptr, 0, &outConnectionStringSize, SQL_DRIVER_COMPLETE_REQUIRED); !OdbcError::hasError(ret)) {
         connected = true;
         return;
      }

      // Remember the last error
      errorMessage = OdbcError::mapErrorMessage(SQL_HANDLE_DBC, handle);

      counter++;
      this_thread::sleep_for(100ms);
   }

   throw Exception("connect(): " + errorMessage);
}
//---------------------------------------------------------------------------
void Connection::disconnect()
// Disconnect
{
   unique_lock lock(mutex);

   if (connected)
      SQLDisconnect(handle);
}
//---------------------------------------------------------------------------
void Connection::exec(const string& command)
// Execute an ad-hoc command
{
   std::vector<string> queries;
   splitSQL(command, queries);

   for (auto& q : queries) {
      Statement statement;
      statement.allocate(*this);
      statement.exec(q);
   }
}
//---------------------------------------------------------------------------
void Connection::cancel()
// Cancel the current query
{
   unique_lock lock(mutex);

   auto queryHandle = runningQuery.load();

   if (queryHandle != SQL_NULL_HANDLE) {
      if (auto ret = SQLCancel(queryHandle); OdbcError::hasError(ret)) {
         cerr << "cancel(): " << OdbcError::mapErrorMessage(SQL_HANDLE_STMT, queryHandle) << endl;
      }
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
