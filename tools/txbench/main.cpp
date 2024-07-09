#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include <charconv>
#include <filesystem>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
static void main(int argc, const char* argv[])
// Entry point
{
   if (argc != 9) {
      throw Exception("USAGE: " + string(argv[0]) + " (SingleStore|Postgres|Umbra|DuckDB|AlloyDB) INSTANCE_NAME (TPCC|TATP|TPCH|TPCH1000|TPCCH|HATTRICK|HYBENCH|LOAD) TXBENCH_DIR BENCHMARK_CONFIG TRANSACTIONAL_CLIENTS ANALYTICAL_CLIENTS SCALE_FACTOR");
   }

   string databaseString = argv[1];
   string instanceName = argv[2];
   string benchmarkString = argv[3];
   string txbenchDir = argv[4];
   string benchmarkConfig = argv[5];
   string transactionalClientString = argv[6];
   string analyticalClientString = argv[7];
   string scaleFactorString = argv[8];

   unsigned transactionalClients, analyticalClients;
   {
      auto [ptr, ec] = from_chars(transactionalClientString.data(), transactionalClientString.data() + transactionalClientString.size(), transactionalClients);
      if ((ec != std::errc()) || (ptr != transactionalClientString.data() + transactionalClientString.size()))
         throw Exception("unable to parse transactional client count \"" + transactionalClientString + "\"");
   }
   {
      auto [ptr, ec] = from_chars(analyticalClientString.data(), analyticalClientString.data() + analyticalClientString.size(), analyticalClients);
      if ((ec != std::errc()) || (ptr != analyticalClientString.data() + analyticalClientString.size()))
         throw Exception("unable to parse analytical client count \"" + analyticalClientString + "\"");
   }

   uint64_t scaleFactor;
   {
      auto [ptr, ec] = from_chars(scaleFactorString.data(), scaleFactorString.data() + scaleFactorString.size(), scaleFactor);
      if ((ec != std::errc()) || (ptr != scaleFactorString.data() + scaleFactorString.size()))
         throw Exception("unable to parse scale factor \"" + scaleFactorString + "\"");
   }

   cerr << endl;
   cerr << "running " << databaseString << " on " << benchmarkString << " at scale factor " << scaleFactor << " using " << transactionalClients << " transaction clients and " << analyticalClients << " analytical clients" << endl;

   auto benchmark = Benchmark::create(benchmarkString);
   auto database = Database::create(databaseString, instanceName, benchmark, txbenchDir, scaleFactor, transactionalClients, analyticalClients);

   cerr << endl;

   // Connect to the database
   database->connect();

   // Run the benchmark
   database->execute(benchmarkConfig);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
int main(int argc, const char* argv[])
// Entry point
{
   try {
      txbench::main(argc, argv);
   } catch (txbench::Exception& e) {
      cerr << e.getMessage() << endl;
      return 1;
   }
   return 0;
}
//---------------------------------------------------------------------------
