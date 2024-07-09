#pragma once
//---------------------------------------------------------------------------
#include "QueryRuntime.hpp"
#include "txbench/DriverConfig.hpp"
#include <memory>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
class Client;
class Database;
class DatabaseClient;
//---------------------------------------------------------------------------
/// A generic benchmark driver implementation
class DatabaseDriver {
   protected:
   /// The database
   Database& database;
   /// The driver configuration
   std::unique_ptr<DriverConfig> driverConfig;

   /// The logfile
   std::string logfile;

   /// The clients
   std::vector<std::unique_ptr<DatabaseClient>> clients;

   /// Count the number of submitted transactions
   uint64_t countSubmittedTransactions() const;
   /// Count the number of processed transactions
   uint64_t countProcessedTransactions() const;

   /// Count the number of submitted queries
   uint64_t countSubmittedQueries() const;
   /// Count the number of processed queries
   uint64_t countProcessedQueries() const;

   /// Get the query runtimes (sum, count, min, max)
   std::vector<QueryRuntime> getQueryRuntimes() const;

   /// Create a client
   virtual std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) = 0;

   /// Create the schema
   virtual void createSchema();
   /// Load the data
   virtual void loadData();
   /// Create the functions
   virtual void createFunctions();

   /// Monitor the transactions
   void monitorTransactions();
   /// Monitor the transactions and queries
   void monitorTransactionsAndQueries();
   /// Monitor the driver
   virtual void monitor();

   /// Collect the query runtimes
   void collectQueryRuntimes();
   /// Collect statistics
   virtual void collectStatistics();

   public:
   /// Constructor
   DatabaseDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
   /// Destructor
   virtual ~DatabaseDriver();

   /// Prepare the database
   void prepareDatabase();
   /// Run the driver
   [[nodiscard]] virtual bool run(bool wait = false);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
