#include "txbench/DatabaseDriver.hpp"
#include "txbench/Database.hpp"
#include "txbench/DatabaseClient.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include "txbench/io/CsvWriter.hpp"
#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
namespace {
//---------------------------------------------------------------------------
string formatTime(unsigned seconds)
// Print a time
{
   stringstream out;
   out << setfill('0') << setw(2) << (seconds / 60);
   out << ":";
   out << setfill('0') << setw(2) << (seconds % 60);
   return out.str();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
DatabaseDriver::DatabaseDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, string logfile)
   : database(database), driverConfig(std::move(driverConfig)), logfile(std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
DatabaseDriver::~DatabaseDriver()
// Destructor
{
}
//---------------------------------------------------------------------------
uint64_t DatabaseDriver::countSubmittedTransactions() const
// Count the number of submitted transactions
{
   uint64_t result = 0;
   for (const auto& client : clients)
      result += client->getSubmittedTransactions();
   return result;
}
//---------------------------------------------------------------------------
uint64_t DatabaseDriver::countProcessedTransactions() const
// Count the number of processed transactions
{
   uint64_t result = 0;
   for (const auto& client : clients)
      result += client->getProcessedTransactions();
   return result;
}
//---------------------------------------------------------------------------
uint64_t DatabaseDriver::countSubmittedQueries() const
// Count the number of submitted queries
{
   uint64_t result = 0;
   for (const auto& client : clients)
      result += client->getSubmittedQueries();
   return result;
}
//---------------------------------------------------------------------------
uint64_t DatabaseDriver::countProcessedQueries() const
// Count the number of processed queries
{
   uint64_t result = 0;
   for (const auto& client : clients)
      result += client->getProcessedQueries();
   return result;
}
//---------------------------------------------------------------------------
std::vector<QueryRuntime> DatabaseDriver::getQueryRuntimes() const
// Get the query runtimes
{
   std::vector<QueryRuntime> result;
   for (const auto& client : clients) {
      auto& runtimes = client->getQueryRuntimes();
      if (runtimes.size() > result.size())
         result.resize(runtimes.size());

      for (unsigned i = 0; i < runtimes.size(); i++) {
         result[i].merge(runtimes[i]);
      }
   }
   return result;
}
//---------------------------------------------------------------------------
void DatabaseDriver::createSchema()
// Prepare the database
{
   cerr << "setting up schema..." << endl;
   for (const auto& filename : database.getBenchmarkConfig().getSchemaScripts())
      RETHROW(database.executeScript(filename), "unable to execute schema script \"" + filename + "\": " + e.getMessage());
}
//---------------------------------------------------------------------------
void DatabaseDriver::loadData()
// Load data
{
   cerr << "loading data..." << endl;
   for (const auto& filename : database.getBenchmarkConfig().getLoadScripts()) {
      RETHROW(database.executeScript(filename), "unable to execute load script \"" + filename + "\": " + e.getMessage());
   }
}
//---------------------------------------------------------------------------
void DatabaseDriver::createFunctions()
// Create functions
{
   cerr << "creating functions..." << endl;
   for (const auto& filename : database.getBenchmarkConfig().getFunctionScripts()) {
      string script;
      RETHROW(database.executeScript(filename), "unable to execute function script \"" + filename + "\": " + e.getMessage());
   }
}
//---------------------------------------------------------------------------
void DatabaseDriver::monitorTransactions()
// Monitor the transactions
{
   auto warmupTime = driverConfig->getWarmupTime();
   auto measureTime = driverConfig->getMeasureTime();

   CsvWriter logWriter(logfile + "_client_trace.csv");

   if (warmupTime) {
      auto beginTime = chrono::high_resolution_clock::now();
      auto processed = countProcessedTransactions();
      for (unsigned i = 1; i <= (10 * warmupTime); ++i) {
         this_thread::sleep_until(beginTime + chrono::milliseconds(100 * i));

         // Write to trace file every 100ms
         auto nextProcessed = countProcessedTransactions();
         logWriter << 100 * i;
         logWriter << nextProcessed;
         logWriter << CsvWriter::endl;

         if ((i % 10) == 0) {
            // Write to stdout every 1s
            cerr << "WARMUP  [ " << formatTime(i / 10) << " / " << formatTime(warmupTime) << " ]: ";
            cerr << (nextProcessed - processed) << " tx/s";
            cerr << " (" << countSubmittedTransactions() << " submitted, " << nextProcessed << " processed)";
            cerr << endl;

            processed = nextProcessed;
         }
      }
   }

   if (measureTime) {
      auto beginTime = chrono::steady_clock::now();
      auto beginSubmitted = countSubmittedTransactions();
      auto beginProcessed = countProcessedTransactions();

      {
         auto processed = beginProcessed;
         for (unsigned i = 1; i <= (10 * measureTime); ++i) {
            this_thread::sleep_until(beginTime + chrono::milliseconds(100 * i));

            // Write to trace file every 100ms
            auto nextProcessed = countProcessedTransactions();
            logWriter << 100 * i;
            logWriter << nextProcessed;
            logWriter << CsvWriter::endl;

            if ((i % 10) == 0) {
               // Write to stdout every 1s
               cerr << "MEASURE [ " << formatTime(i / 10) << " / " << formatTime(measureTime) << " ]: ";
               cerr << (nextProcessed - processed) << " tx/s";
               cerr << " (" << countSubmittedTransactions() << " submitted, " << nextProcessed << " processed)";
               cerr << endl;

               processed = nextProcessed;
            }
         }
      }

      auto endSubmitted = countSubmittedTransactions();
      auto endProcessed = countProcessedTransactions();
      auto processed = endProcessed - beginProcessed;

      cerr << endl;
      cerr << "MEASURE THROUGHPUT: " << (static_cast<double>(processed / measureTime)) << " tx/s ";
      cerr << "(" << (endSubmitted - beginSubmitted) << " submitted, " << processed << " processed)";
      cerr << endl;
   }
}
//---------------------------------------------------------------------------
void DatabaseDriver::monitorTransactionsAndQueries()
// Monitor the transactions and queries
{
   auto warmupTime = driverConfig->getWarmupTime();
   auto measureTime = driverConfig->getMeasureTime();

   CsvWriter logWriter(logfile + "_client_trace.csv");

   if (warmupTime) {
      auto beginTime = chrono::high_resolution_clock::now();
      auto processedTransactions = countProcessedTransactions();
      auto processedQueries = countProcessedQueries();

      for (unsigned i = 1; i <= (10 * warmupTime); ++i) {
         this_thread::sleep_until(beginTime + chrono::milliseconds(100 * i));

         // Write to trace file every 100ms
         auto nextProcessedTransactions = countProcessedTransactions();
         auto nextProcessedQueries = countProcessedQueries();
         logWriter << 100 * i;
         logWriter << nextProcessedTransactions;
         logWriter << nextProcessedQueries;
         logWriter << CsvWriter::endl;

         if ((i % 10) == 0) {
            // Write to stdout every 1s
            cerr << "WARMUP  [ " << formatTime(i / 10) << " / " << formatTime(warmupTime) << " ]: ";
            cerr << (nextProcessedTransactions - processedTransactions) << " tx/s, ";
            cerr << (nextProcessedQueries - processedQueries) << " q/s";
            cerr << " (" << countSubmittedTransactions() << " submitted transactions, " << nextProcessedTransactions << " processed transactions, ";
            cerr << countSubmittedQueries() << " submitted queries, " << nextProcessedQueries << " processed queries)";
            cerr << endl;

            processedTransactions = nextProcessedTransactions;
            processedQueries = nextProcessedQueries;
         }
      }
   }

   if (measureTime) {
      auto beginTime = chrono::steady_clock::now();
      auto beginSubmittedTransactions = countSubmittedTransactions();
      auto beginProcessedTransactions = countProcessedTransactions();
      auto beginSubmittedQueries = countSubmittedQueries();
      auto beginProcessedQueries = countProcessedQueries();

      {
         auto processedTransactions = beginProcessedTransactions;
         auto processedQueries = beginProcessedQueries;

         for (unsigned i = 1; i <= (10 * measureTime); ++i) {
            this_thread::sleep_until(beginTime + chrono::milliseconds(100 * i));

            // Write to trace file every 100ms
            auto nextProcessedTransactions = countProcessedTransactions();
            auto nextProcessedQueries = countProcessedQueries();
            logWriter << 100 * i;
            logWriter << nextProcessedTransactions;
            logWriter << nextProcessedQueries;
            logWriter << CsvWriter::endl;

            if ((i % 10) == 0) {
               // Write to stdout every 1s
               cerr << "MEASURE [ " << formatTime(i / 10) << " / " << formatTime(measureTime) << " ]: ";
               cerr << (nextProcessedTransactions - processedTransactions) << " tx/s, ";
               cerr << (nextProcessedQueries - processedQueries) << " q/s";
               cerr << " (" << countSubmittedTransactions() << " submitted transactions, " << nextProcessedTransactions << " processed transactions, ";
               cerr << countSubmittedQueries() << " submitted queries, " << nextProcessedQueries << " processed queries)";
               cerr << endl;

               processedTransactions = nextProcessedTransactions;
               processedQueries = nextProcessedQueries;
            }
         }
      }

      auto endSubmittedTransactions = countSubmittedTransactions();
      auto endProcessedTransactions = countProcessedTransactions();
      auto processedTransactions = endProcessedTransactions - beginProcessedTransactions;

      auto endSubmittedQueries = countSubmittedQueries();
      auto endProcessedQueries = countProcessedQueries();
      auto processedQueries = endProcessedQueries - beginProcessedQueries;

      cerr << endl;
      cerr << "MEASURE THROUGHPUT: ";
      cerr << (static_cast<double>(processedTransactions / measureTime)) << " tx/s ";
      cerr << (static_cast<double>(processedQueries / measureTime)) << " q/s ";
      cerr << "(" << (endSubmittedTransactions - beginSubmittedTransactions) << " submitted transactions, " << processedTransactions << " processed transactions, ";
      cerr << (endSubmittedQueries - beginSubmittedQueries) << " submitted queries, " << processedQueries << " processed queries)";
      cerr << endl;
   }
}
//---------------------------------------------------------------------------
void DatabaseDriver::monitor()
// Monitor the driver
{
   monitorTransactions();
}
//---------------------------------------------------------------------------
void DatabaseDriver::collectQueryRuntimes()
// Collect the query runtimes
{
   auto runtimes = getQueryRuntimes();
   if (runtimes.empty()) return;

   auto doubleToString = [](double value) {
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2) << value;
      return stream.str();
   };

   static constexpr unsigned num_columns = 5;
   std::vector<array<string, num_columns>> text;
   text.push_back({"query", "avg exec. time", "min exec. time", "max exec. time", "count"});
   CsvWriter logWriter(logfile + "_query_runtimes.csv");
   for (unsigned i = 0; i < runtimes.size(); i++) {
      auto& r = runtimes[i];

      text.push_back({to_string(i + 1), doubleToString(r.runtime / r.count) + " ms", doubleToString(r.minimum) + " ms", doubleToString(r.maximum) + " ms", to_string(r.count)});

      logWriter << i + 1;
      logWriter << r.runtime / r.count;
      logWriter << r.minimum;
      logWriter << r.maximum;
      logWriter << r.count;
      logWriter << CsvWriter::endl;
   }

   // Compute column sizes based on the length of values
   std::array<uint64_t, num_columns> columnSizes = {0, 0, 0};
   for (auto& a : text) {
      for (auto i = 0; i < num_columns; i++)
         columnSizes[i] = max<uint64_t>(columnSizes[i], a[i].length());
   }

   // Print table header separator
   auto seperator = [&]() {
      for (auto size : columnSizes) {
         cerr << "+";
         for (unsigned i = 0; i < size + 2; i++)
            cerr << "-";
      }
      cerr << "+" << endl;
   };
   auto fill = [&](unsigned c, const string& t) {
      auto size = columnSizes[c];
      for (unsigned i = 0; i < size - t.length(); i++) {
         cerr << " ";
      }
   };
   auto row = [&](const array<string, num_columns>& r) {
      for (unsigned c = 0; c < r.size(); c++) {
         cerr << "| ";
         auto& t = r[c];
         fill(c, t);
         cerr << t << " ";
      }
      cerr << "|" << endl;
   };

   seperator();
   row(text[0]);
   seperator();
   for (unsigned i = 1; i < text.size(); i++) {
      row(text[i]);
   }
   seperator();
}
//---------------------------------------------------------------------------
void DatabaseDriver::collectStatistics()
// Collect statistics
{
   collectQueryRuntimes();
}
//---------------------------------------------------------------------------
void DatabaseDriver::prepareDatabase()
// Prepare the database
{
   cerr << "preparing database..." << endl;

   auto beginTime = chrono::steady_clock::now();
   createSchema();
   loadData();
   createFunctions();
   auto endTime = chrono::steady_clock::now();

   cerr << "prepared database in " << (1e-9 * (endTime - beginTime).count()) << " s" << endl;
   cerr << endl;
}
//---------------------------------------------------------------------------
bool DatabaseDriver::run(bool wait)
// Run the driver
{
   unsigned transactionalClients = database.getTransactionalClients();
   unsigned analyticalClients = database.getAnalyticalClients();

   // Initialize clients
   {
      const auto clientCount = transactionalClients + analyticalClients;
      cerr << "initializing " << clientCount << " " << ((clientCount > 1) ? "clients" : "client") << endl;

      const auto beginTime = chrono::steady_clock::now();

      // Initialize clients
      clients.reserve(clientCount);
      for (unsigned i = 0; i < clientCount; ++i) {
         if (auto client = createClient(i)) {
            try {
               client->initialize();
            } catch (Exception& e) {
               cerr << "unable to initialize client: " << e.getMessage() << endl;
               return false;
            }

            clients.push_back(std::move(client));
         }
      }

      const auto endTime = chrono::steady_clock::now();

      cerr << "initialized " << clientCount << " " << ((clientCount > 1) ? "clients" : "client") << " in " << (1e-9 * (endTime - beginTime).count()) << " s" << endl;
      cerr << endl;
   }

   // Start the clients
   {
      for (const auto& client : clients)
         client->start();

      thread monitorThread([this]() { monitor(); });

      // Stop processing
      monitorThread.join();
      collectStatistics();

      cerr << endl;
   }

   // Stop the clients
   {
      const auto clientCount = transactionalClients + analyticalClients;
      cerr << "stopping " << clientCount << " " << ((clientCount > 1) ? "clients" : "client") << endl;

      const auto beginTime = chrono::steady_clock::now();

      // Stop clients
      for (const auto& client : clients) {
         client->stop();
      }

      // Try to join the clients
      for (const auto& client : clients) {
         if (!client->join(wait)) {
            cerr << "unable to join client, exiting..." << endl;
            exit(0);
         }
      }

      const auto endTime = chrono::steady_clock::now();
      cerr << "stopped " << clientCount << " " << ((clientCount > 1) ? "clients" : "client") << " in " << (1e-9 * (endTime - beginTime).count()) << " s" << endl;
      cerr << endl;
   }

   return true;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
