#include "duckdb/tpch/TpchQueryClient.hpp"
#include "txbench/Database.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
TpchQueryClient::TpchQueryClient(const Database& database, unsigned clientIndex)
   : DuckDBClient(database), generator(clientIndex)
// Constructor
{
   queryRuntimes.resize(22, {0, 0});
}
//---------------------------------------------------------------------------
void TpchQueryClient::performWork()
// Perform work
{
   while (running()) {
      submittedQueries.fetch_add(1, memory_order_acq_rel);

      auto query = generator.pickQuery();
      {
         RecordQueryRuntime guard(this, query);
         statements[query].exec();
      }
      processedQueries.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
void TpchQueryClient::initialize()
// Initialize the client
{
   DuckDBClient::initialize();

   statements.resize(22);
   for (unsigned i = 0; i < 22; ++i) {
      string query;
      database.readScript("sql/queries/" + to_string(i + 1) + ".sql", query);

      statements[i].prepare(*connection, query);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
