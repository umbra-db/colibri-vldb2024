#pragma once
//---------------------------------------------------------------------------
#include <atomic>
#include <mutex>
#include <string>
#include <sql.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
class Environment;
//---------------------------------------------------------------------------
/// A database connection
class Connection {
   /// The connection handle
   SQLHDBC handle = SQL_NULL_HANDLE;
   /// Whether the connection is connected
   bool connected = false;
   /// The mutex to synchronise deconstruction of handles
   std::mutex mutex;

   /// The currently running query
   std::atomic<SQLHSTMT> runningQuery = SQL_NULL_HANDLE;

   friend class PreparedStatement;
   friend class Statement;

   public:
   /// Constructor
   explicit Connection();
   /// Destructor
   ~Connection();

   /// Attempt to connect
   void connect(const Environment& environment, const std::string& connectionString, unsigned timeout = 120);
   /// Disconnect
   void disconnect();

   /// Execute an ad-hoc command
   void exec(const std::string& command);

   /// Cancel the current query
   void cancel();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
