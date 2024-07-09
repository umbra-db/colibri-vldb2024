#include "umbra/tpcch/TpcchQueryClient.hpp"
#include "umbra/Umbra.hpp"
#include <fstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TpcchQueryClient::TpcchQueryClient(const Database& database, unsigned clientId)
   : UmbraClient(database), generator(clientId)
// Constructor
{
   queryRuntimes.resize(22, {0, 0});
}
//---------------------------------------------------------------------------
void TpcchQueryClient::performWork()
// Perform work
{
   while (running()) {
      submittedQueries.fetch_add(1, memory_order_acq_rel);

      // Send a batch of transactions
      auto query = generator.pickQuery();
      {
         RecordQueryRuntime guard(this, query);
         connection.send(statements[query]);

         connection.receive();
      }
      processedQueries.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
void TpcchQueryClient::initialize()
// Initialize the client
{
   UmbraClient::initialize();

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
