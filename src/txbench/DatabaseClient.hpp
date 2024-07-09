#pragma once
//---------------------------------------------------------------------------
#include "txbench/QueryRuntime.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <thread>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
class Database;
//---------------------------------------------------------------------------
/// A client
class DatabaseClient {
   /// The state of the client
   enum State { IDLE,
                RUNNING,
                STOPPING,
                STOPPED };

   /// The processing thread
   std::thread thread;
   /// Whether we are done
   std::atomic<State> state;

   /// The mutex to protect the condition variable
   std::mutex mutex;
   /// The condition variable
   std::condition_variable cond;

   protected:
   /// The database
   const Database& database;

   /// The number of submitted transactions
   std::atomic<uint64_t> submittedTransactions = 0;
   /// The number of processed transactions
   std::atomic<uint64_t> processedTransactions = 0;

   /// The number of submitted queries
   std::atomic<uint64_t> submittedQueries = 0;
   /// The number of processed queries
   std::atomic<uint64_t> processedQueries = 0;

   /// The query runtimes
   std::vector<QueryRuntime> queryRuntimes;

   /// Helper to record query runtimes
   class RecordQueryRuntime {
      /// The database client
      DatabaseClient* client;
      /// The query
      unsigned query;
      /// The begin of the query
      std::chrono::steady_clock::time_point begin;

      public:
      /// Constructor
      RecordQueryRuntime(DatabaseClient* client, unsigned query) : client(client), query(query) {
         begin = std::chrono::steady_clock::now();
      }
      /// Destructor
      ~RecordQueryRuntime() {
         auto end = std::chrono::steady_clock::now();
         client->queryRuntimes[query].add(1e-6 * (end - begin).count());
      }
   };

   /// Is the client running?
   bool running() const { return state.load() == RUNNING; }
   /// Perform work
   virtual void performWork() = 0;

   /// Cancel the running queries
   virtual void cancelQueries();

   public:
   /// Constructor
   explicit DatabaseClient(const Database& database);
   /// Destructor
   virtual ~DatabaseClient();

   /// Get the number of submitted transactions
   uint64_t getSubmittedTransactions() const { return submittedTransactions.load(); };
   /// Get the number of processed transactions
   uint64_t getProcessedTransactions() const { return processedTransactions.load(); };

   /// Get the number of submitted queries
   uint64_t getSubmittedQueries() const { return submittedQueries.load(); };
   /// Get the number of processed queries
   uint64_t getProcessedQueries() const { return processedQueries.load(); };

   /// Get the query runtimes
   const std::vector<QueryRuntime>& getQueryRuntimes() const { return queryRuntimes; }

   /// Initialize the client
   virtual void initialize() = 0;

   /// Start processing
   void start();
   /// Stop processing
   void stop();
   /// Wait for completion
   bool join(bool wait);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
