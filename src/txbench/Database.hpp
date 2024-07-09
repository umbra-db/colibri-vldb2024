#pragma once
//---------------------------------------------------------------------------
#include "txbench/BenchmarkConfig.hpp"
#include "txbench/ConnectionConfig.hpp"
#include <filesystem>
#include <memory>
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
class DatabaseDriver;
//---------------------------------------------------------------------------
/// The available benchmarks
class Benchmark {
   public:
   enum Value {
      LOAD,
      TATP,
      TPCC,
      TPCH,
      TPCH1000,
      TPCCH,
      HATTRICK,
      HYBENCH
   };

   private:
   /// The benchmark
   Value value;

   /// Constructor
   Benchmark(Value value) : value(value) {}

   public:
   /// Get the benchmark
   Value get() const { return value; }

   /// To string
   std::string toString() const;
   /// To scripts path
   std::string toScriptsPath() const;
   /// To path
   std::string toPath() const;

   /// Create a benchmark
   static Benchmark create(const std::string& benchmark);
};
//---------------------------------------------------------------------------
/// The database interface for running benchmarks
class Database {
   public:
   /// The available database systems
   enum System {
      AlloyDB,
      DuckDB,
      SingleStore,
      TiDB,
      Postgres,
      Umbra,
      UmbraLocal
   };

   protected:
   /// The database system
   System database;
   /// The benchmark
   Benchmark benchmark;
   /// The path to the scripts
   std::filesystem::path scriptsPath;
   /// The path to the logs
   std::filesystem::path logPath;

   /// The scale factor
   uint64_t scaleFactor;
   /// The number of transactional clients
   unsigned transactionalClients;
   /// The number of analytical clients
   unsigned analyticalClients;

   /// The connection config
   std::unique_ptr<ConnectionConfig> connectionConfig;
   /// The client config
   std::unique_ptr<BenchmarkConfig> benchmarkConfig;

   /// Get the connection config file
   std::filesystem::path getConnectionConfigFile() const;
   /// Get the client config file
   std::filesystem::path getBenchmarkConfigFile() const;
   /// Get the log file path
   std::filesystem::path getLogFile() const;

   /// Prepare the connection config
   virtual std::unique_ptr<ConnectionConfig> prepareConnectionConfig() const = 0;
   /// Prepare the database benchmark config
   virtual std::unique_ptr<BenchmarkConfig> prepareBenchmarkConfig() const;
   /// Prepare the driver
   virtual std::unique_ptr<DatabaseDriver> prepareDriver(const std::string& driverConfigFile, const std::string& logfile) = 0;

   /// Start the database
   virtual void startDatabase();
   /// Shutdown the database
   virtual void shutdownDatabase();

   public:
   /// Constructor
   explicit Database(System database, Benchmark benchmark, std::filesystem::path scriptsPath, std::filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients);
   /// Destructor
   virtual ~Database();

   /// Initialise
   void init();

   /// Get the scale factor
   uint64_t getScaleFactor() const { return scaleFactor; }
   /// Get the number of transactional clients
   unsigned getTransactionalClients() const { return transactionalClients; }
   /// Get the number of analytical clients
   unsigned getAnalyticalClients() const { return analyticalClients; }
   /// Get the database name
   std::string getDatabaseName() const { return benchmarkConfig->getDatabase(); }

   /// Get the connection config
   const ConnectionConfig& getConnectionConfig() const { return *connectionConfig; }
   /// Get the connection string
   std::string getConnectionString(const std::string& database = "") const { return connectionConfig->getConnectionString(database.empty() ? getDatabaseName() : database); }
   /// Get the pipeline depth
   unsigned getPipelineDepth() const { return connectionConfig->getPipelineDepth(); }

   /// Get the benchmark config
   const BenchmarkConfig& getBenchmarkConfig() const { return *benchmarkConfig; }

   /// Read a query script from a file
   void readScript(const std::string& filename, std::string& target) const;
   /// Execute a script
   void executeScript(const std::string& filename);
   /// Execute a query
   virtual void executeQuery(const std::string& query) = 0;

   /// Connect to the database
   virtual void connect() = 0;
   /// Execute a benchmark
   virtual void execute(const std::string& driverConfigFile);

   /// Create a database
   static std::unique_ptr<Database> create(const std::string& database, const std::string& instanceName, Benchmark benchmark, const std::filesystem::path& txbenchDir, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
