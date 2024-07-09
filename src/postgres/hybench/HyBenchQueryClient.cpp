#include "postgres/hybench/HyBenchQueryClient.hpp"
#include "postgres/Postgres.hpp"
#include "postgres/hybench/HyBenchDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
HyBenchQueryClient::HyBenchQueryClient(const Database& database, HyBenchSharedState& state, unsigned clientId)
   : PostgresClient(database), config(hybench::Config::instantiate(database.getScaleFactor())), generator(config, clientId), queryGenerator(clientId, 6), state(state)
// Constructor
{
   queryRuntimes.resize(6, {0, 0});
}
//---------------------------------------------------------------------------
void HyBenchQueryClient::performWork()
// Perform work
{
   while (running()) {
      submittedQueries.fetch_add(1, memory_order_acq_rel);

      // Send a batch of transactions
      auto query = queryGenerator.pickQuery();
      {
         RecordQueryRuntime guard(this, query);
         switch (query) {
            case 0: {
               int customerId;
               double rate = generator.generateReal(0.0, 1.0);
               if (rate < config.riskRate / 2) {
                  customerId = state.getBlockedTransferId(generator);
               } else if (rate < config.riskRate) {
                  customerId = state.getBlockedAccountId(generator);
               } else {
                  customerId = generator.generateCustomerId();
               }
               statements[query].bind(customerId);
            } break;
            case 1: {
               int companyid;
               double rate = generator.generateReal(0.0, 1.0);
               if (rate < config.riskRate) {
                  companyid = state.getBlockedAccountId(generator);
               } else {
                  companyid = generator.generateCompanyId();
               }
               statements[query].bind(companyid);
            } break;
            case 2:
            case 3:
            case 4: {
               int companyid = generator.generateCompanyId();
               statements[query].bind(companyid);

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
void HyBenchQueryClient::initialize()
// Initialize the client
{
   PostgresClient::initialize();

   statements.resize(6);
   for (unsigned i = 0; i < 6; ++i) {
      string query;
      database.readScript("sql/olxp/" + to_string(i + 1) + ".sql", query);

      statements[i] = connection.prepare("iq" + to_string(i), query);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
