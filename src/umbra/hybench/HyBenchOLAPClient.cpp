#include "umbra/hybench/HyBenchOLAPClient.hpp"
#include "umbra/Umbra.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
HyBenchOLAPClient::HyBenchOLAPClient(const Database& database, unsigned clientId)
   : UmbraClient(database), config(hybench::Config::instantiate(database.getScaleFactor())), generator(config, clientId), queryGenerator(clientId, 13)
// Constructor
{
   queryRuntimes.resize(13, {0, 0});
}
//---------------------------------------------------------------------------
void HyBenchOLAPClient::performWork()
// Perform work
{
   while (running()) {
      submittedQueries.fetch_add(1, memory_order_acq_rel);

      // Send a batch of transactions
      auto query = queryGenerator.pickQuery();
      {
         RecordQueryRuntime guard(this, query);
         switch (query) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 12: {
               int customerId = generator.generateCustomerId();
               statements[query].bind(customerId);
            } break;
            case 4:
            case 6: {
               int companyId = generator.generateCompanyId();
               statements[query].bind(companyId);
            } break;
            case 11: {
               string category = generator.generateCompanyCategory();
               statements[query].bind(category);
            } break;
            default: break;
         }
         connection.send(statements[query]);

         connection.receive();
      }
      processedQueries.fetch_add(1, memory_order_acq_rel);
   }
}
//---------------------------------------------------------------------------
void HyBenchOLAPClient::initialize()
// Initialize the client
{
   UmbraClient::initialize();

   statements.resize(13);
   for (unsigned i = 0; i < 13; ++i) {
      string query;
      database.readScript("sql/queries/" + to_string(i + 1) + ".sql", query);

      statements[i] = connection.prepare("q" + to_string(i), query);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
