#include "postgres/tpch/TpchQueryClient.hpp"
#include "postgres/Postgres.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpchQueryClient::TpchQueryClient(const Database& database, unsigned clientId)
   : PostgresClient(database), generator(clientId)
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

      // Send a batch of transactions
      const auto query = generator.pickQuery();
      {
         RecordQueryRuntime guard(this, query);
         connection.send(statements[query]);

         connection.receive();
      }
      processedQueries.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
void TpchQueryClient::initialize()
// Initialize the client
{
   PostgresClient::initialize();

   statements.resize(22);
   for (unsigned i = 0; i < 22; ++i) {
      string query;
      database.readScript("sql/queries/" + to_string(i + 1) + ".sql", query);

      statements[i] = connection.prepare("q" + to_string(i), query);
      statements[i].bind();
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
