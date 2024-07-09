#include "duckdb/hattrick/HattrickQueryClient.hpp"
#include "txbench/Database.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
HattrickQueryClient::HattrickQueryClient(const Database& database, unsigned clientIndex)
   : DuckDBClient(database), generator(clientIndex)
// Constructor
{
   queryRuntimes.resize(13);
}
//---------------------------------------------------------------------------
void HattrickQueryClient::performWork()
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
void HattrickQueryClient::initialize()
// Initialize the client
{
   DuckDBClient::initialize();

   statements.resize(13);
   for (unsigned i = 0; i < 13; ++i) {
      string query;
      database.readScript("sql/queries/" + to_string(i + 1) + ".sql", query);

      statements[i].prepare(*connection, query);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
