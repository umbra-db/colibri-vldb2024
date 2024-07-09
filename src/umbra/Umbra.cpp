#include "umbra/Umbra.hpp"
#include "txbench/Exception.hpp"
#include "txbench/io/CsvWriter.hpp"
#include "umbra/UmbraConfig.hpp"
#include "umbra/UmbraConnectionConfig.hpp"
#include "umbra/UmbraDriver.hpp"
#include "umbra/hattrick/HattrickDriver.hpp"
#include "umbra/hybench/HyBenchDriver.hpp"
#include "umbra/load/LoadConfig.hpp"
#include "umbra/tatp/TatpDriver.hpp"
#include "umbra/tpcc/TpccDriver.hpp"
#include "umbra/tpcch/TpcchDriver.hpp"
#include "umbra/tpch/TpchDriver.hpp"
#include <cassert>
#include <chrono>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Umbra::Umbra(System database, Benchmark benchmark, filesystem::path scriptsPath, filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
   : Postgres(database, benchmark, move(scriptsPath), move(logPath), scaleFactor, transactionalClients, analyticalClients)
// Constructor
{
}
//---------------------------------------------------------------------------
Umbra::~Umbra()
// Destructor
{
   Umbra::shutdownDatabase();
}
//---------------------------------------------------------------------------
bool Umbra::trace()
// Check whether the umbra should be traced
{
   if (auto val = getenv("TRACE"); val)
      return string(val) == "1";

   return false;
}
//---------------------------------------------------------------------------
unique_ptr<ConnectionConfig> Umbra::prepareConnectionConfig() const
// Prepare the connection config
{
   return make_unique<UmbraConnectionConfig>();
}
//---------------------------------------------------------------------------
std::unique_ptr<DatabaseDriver> Umbra::prepareDriver(const std::string& driverConfigFile, const std::string& logfile)
// Prepare the database driver
{
   auto driverConfig = make_unique<UmbraConfig>();
   if (auto res = driverConfig->read(driverConfigFile); !res)
      throw Exception("unable to read driver config at \"" + driverConfigFile + "\": " + res.getMessage());

   switch (benchmark.get()) {
      case Benchmark::TPCC: return make_unique<TpccDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCCH: return make_unique<TpcchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TATP: return make_unique<TatpDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH: return make_unique<TpchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH1000: return make_unique<TpchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::HATTRICK: return make_unique<HattrickDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::HYBENCH: return make_unique<HyBenchDriver>(*this, std::move(driverConfig), std::move(logfile));
      default: throw Exception("benchmark \"" + benchmark.toString() + "\" not supported");
   }
}
//---------------------------------------------------------------------------
void Umbra::startDatabase()
// Start the database
{
   if (database == System::Umbra)
      return Postgres::startDatabase();

   assert(database == System::UmbraLocal);
   pid_t pid = fork();

   if (pid == -1) // Error handling
      throw Exception("fork failed");

   if (pid == 0) {
      setpgid(0, 0);
      auto startUmbraLocal = string(filesystem::path(scriptsPath).append("startUmbraLocal.sh"));
      auto benchmarkDir = benchmark.toPath();
      auto scaleFactor = to_string(this->scaleFactor);
      auto outputDir = logPath.string();
      execlp(startUmbraLocal.c_str(), startUmbraLocal.c_str(), benchmarkDir.c_str(), scaleFactor.c_str(), logPath.c_str(), nullptr);
   } else {
      umbraPid = pid;
      cerr << "started local umbra process with pid " << pid << endl;
      connectionConfig->patch("localhost", 5432);
   }
}
//---------------------------------------------------------------------------
void Umbra::shutdownDatabase()
// Shutdown the database
{
   if (database == System::Umbra)
      return Postgres::shutdownDatabase();

   assert(database == System::UmbraLocal);
   if (umbraPid == 0)
      return;

   // Send a termination signal (SIGTERM) to the child process and its descendants
   cerr << "shutting down local umbra process with pid " << umbraPid << endl;
   sleep(5);

   if (kill(-getpgid(umbraPid), SIGTERM) == -1) {
      cerr << "error killing process and its descendants. errno: " << errno << endl;
   }

   // Wait for the child process to finish
   int status;
   waitpid(umbraPid, &status, 0);

   umbraPid = 0;
}
//---------------------------------------------------------------------------
void Umbra::execute(const string& benchmarkConfigFile)
// Execute a benchmark
{
   if (benchmark.get() == Benchmark::LOAD) {
      LoadConfig loadConfig{};
      if (auto res = loadConfig.read(benchmarkConfigFile); !res)
         throw Exception("unable to open LOAD config at \"" + benchmarkConfigFile + "\": " + res.getMessage());

      // Setup schema and load benchmark data
      auto relationConfig = loadConfig.getRelationConfig();
      cerr << "setting up schema... (" << relationConfig << ")" << endl;
      for (const auto& filename : getBenchmarkConfig().getSchemaScripts()) {
         string script;
         RETHROW(readScript(filename, script), "unable to read schema script \"" + filename + "\": " + e.getMessage());

         auto replace = [&](string_view from, std::string& to) {
            for (auto pos = script.find(from); pos != string::npos; pos = script.find(from, pos)) {
               script.replace(pos, from.size(), to);
               pos += from.size();
            }
         };

         // Patch in scale factor-dependent bucket counts
         replace("%%relationConfig%%", relationConfig);

         executeQuery(script);
      }

      cerr << "loading data..." << endl;
      auto begin = chrono::steady_clock::now();
      connection.execute(loadConfig.useBulkOperations() ? "BEGIN BULK WRITE" : "BEGIN");

      for (const auto& filename : getBenchmarkConfig().getLoadScripts())
         RETHROW(executeScript(filename), "unable to execute load script \"" + filename + "\": " + e.getMessage());

      connection.execute("COMMIT");

      auto end = chrono::steady_clock::now();

      // Write client-side trace file
      {
         cerr << "LOAD TIME: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / 1000.0 << " s" << endl;

         CsvWriter logWriter(getLogFile().string() + "_load.csv");
         logWriter << (1e-9 * static_cast<double>((end - begin).count()));
         logWriter << CsvWriter::endl;
      }
   } else {
      // Setup the benchmark driver
      auto driver = prepareDriver(benchmarkConfigFile, getLogFile());
      driver->prepareDatabase();

      if (trace()) {
         executeQuery("SET debug.verbosity = 'info'");
         executeQuery("SET debug.perftracer.dump = 'output/umbra_start.trace'");
         executeQuery("SET debug.database.dumplayout = 'output/layout_start.json'");
         executeQuery("SET debug.verbosity = 'fatal'");
      }

      if (!driver->run())
         throw Exception("could not run benchmark \"" + benchmark.toString() + "\"");
   }

   if (trace()) {
      executeQuery("SET debug.verbosity = 'info'");
      executeQuery("SET debug.perftracer.dump = 'output/umbra.trace'");
      executeQuery("SET debug.database.dumplayout = 'output/layout.json'");
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
