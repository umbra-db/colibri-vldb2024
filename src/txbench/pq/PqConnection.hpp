#pragma once
//---------------------------------------------------------------------------
#include "txbench/pq/PqPreparedStatement.hpp"
#include "txbench/pq/PqResult.hpp"
#include <mutex>
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
struct pg_conn;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// A database connection
class PqConnection {
   /// The database connection object
   pg_conn* con;
   /// Whether we are in pipeline mode
   bool pipelineMode = false;
   /// The mutex to synchronise deconstruction of the connection
   std::mutex mutex;

   public:
   /// Constructor
   PqConnection();
   /// Destructor
   ~PqConnection();

   /// Attempt to connect
   void connect(const std::string& connectionString, unsigned timeout = 120);
   /// Close the connection
   void close();

   /// Execute an ad-hoc statement
   PqResult execute(const std::string& statement);

   /// Cancel the running queries
   void cancel();

   /// Prepare a statement
   PqPreparedStatement prepare(const std::string& name, const std::string& command);
   /// Enter pipeline mode
   void enterPipelineMode();
   /// Send a prepared statement
   void send(const PqPreparedStatement& statement);
   /// Receive a statement result
   PqResult receive();
   /// Synchronize the pipeline
   void sync();
   /// Exit pipeline mode
   void exitPipelineMode();

   /// Flush the pipeline
   void flush();
   /// Enqueue a request that the server flushes its pipeline
   void sendFlushRequest();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
