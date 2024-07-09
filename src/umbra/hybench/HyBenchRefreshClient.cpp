#include "umbra/hybench/HyBenchRefreshClient.hpp"
#include "txbench/Database.hpp"
#include "umbra/hybench/HyBenchDriver.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
HyBenchRefreshClient::HyBenchRefreshClient(const Database& database, HyBenchSharedState& state, unsigned clientId)
   : UmbraClient(database), config(hybench::Config::instantiate(database.getScaleFactor())), generator(config, clientId), transactionGenerator(42 + clientId, false), state(state)
// Constructor
{
}
//---------------------------------------------------------------------------
void HyBenchRefreshClient::performWork()
// Perform work
{
   auto pipelineDepth = database.getPipelineDepth();
   connection.enterPipelineMode();

   auto sendBatch = [&]() {
      // Send a batch of transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         auto tx = transactionGenerator.pickTransaction();
         switch (tx) {
            case 0: {
               int sourceId = generator.generateCustomerOrCompanyId();
               int targetId;
               if (generator.generateReal(0.0, 1.0) < config.riskRate) {
                  targetId = state.getBlockedTransferId(generator);
               } else {
                  targetId = generator.generateCustomerOrCompanyId();
               }

               uint64_t amount;
               string type;
               if (sourceId <= config.customerNumber) {
                  amount = generator.generateReal(0, config.customerSavingBalance * 0.0001) * 100;
                  type = generator.generateCustomerTransferType();
               } else {
                  amount = generator.generateReal(0, config.companySavingBalance * 0.0001) * 100;
                  type = generator.generateCompanyTransferType();
               }

               std::stringstream ss;
               ss << generator.generateDateAfterMidPoint();
               string ts = ss.str();
               statements[tx].bind(++state.transferId, sourceId, targetId, amount, type, ts);
            } break;
            case 1: {
               int sourceId;
               int targetId = generator.generateCustomerOrCompanyId();
               if (generator.generateReal(0.0, 1.0) < config.riskRate) {
                  sourceId = state.getBlockedCheckingId(generator);
               } else {
                  sourceId = generator.generateCustomerOrCompanyId();
               }
               uint64_t amount;
               string type;
               if (sourceId <= config.customerNumber) {
                  amount = generator.generateReal(0, config.customerCheckingBalance * 0.0001) * 100;
                  type = generator.generateCustomerTransferType();
               } else {
                  amount = generator.generateReal(0, config.companyCheckingBalance * 0.0001) * 100;
                  type = generator.generateCompanyTransferType();
               }
               std::stringstream ss;
               ss << generator.generateDateAfterMidPoint();
               string ts = ss.str();
               statements[tx].bind(++state.checkingId, sourceId, targetId, amount, type, ts);
            } break;
            case 2: {
               statements[tx].bind(++state.loantransId, config.getEndDate());
            } break;
            case 3: {
               statements[tx].bind(config.getEndDate());
            } break;
            default: break;
         }

         connection.send(statements[tx]);
         submittedTransactions.fetch_add(1, memory_order_acq_rel);
      }

      connection.send(retrieveBlockedAccounts);
      connection.send(dropBlockedAccounts);

      // Request that the server flushes results
      connection.sendFlushRequest();

      // And submit to the server
      connection.flush();
   };
   auto receiveBatch = [&]() {
      // Receive a batch of transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         connection.receive();
         processedTransactions.fetch_add(1, memory_order_acq_rel);
      }

      {
         auto res = connection.receive();
         vector<int> ids;
         ids.reserve(res.ntuples());
         for (int i = 0, limit = res.ntuples(); i < limit; i++) {
            string s(*res.getvalue(i, 0));
            ids.push_back(std::stoi(s));
         }
         state.addBlockedAccounts(ids);
      }
      connection.receive();
   };

   sendBatch();
   while (running()) {
      sendBatch();
      receiveBatch();
   }

   receiveBatch();

   connection.sync();
   connection.exitPipelineMode();
}
//---------------------------------------------------------------------------
void HyBenchRefreshClient::initialize()
// Initialize the client
{
   UmbraClient::initialize();

   connection.execute("create temporary table blockedaccounts (applicantid integer not null)");

   retrieveBlockedAccounts = connection.prepare("ba1", "select * from blockedaccounts;");
   dropBlockedAccounts = connection.prepare("ba2", "delete from blockedaccounts;");

   statements[0] = connection.prepare("at1", "CALL at1($1::BIGINT, $2::INTEGER, $3::INTEGER, $4::BIGINT, $5::TEXT, $6::TEXT)");
   statements[1] = connection.prepare("at2", "CALL at2($1::BIGINT, $2::INTEGER, $3::INTEGER, $4::BIGINT, $5::TEXT, $6::TEXT)");
   statements[2] = connection.prepare("at3", "CALL at3($1::BIGINT, $2::TEXT)");
   statements[3] = connection.prepare("at4", "CALL at4($1::TEXT)");
   statements[4] = connection.prepare("at5", "CALL at5()");
   statements[5] = connection.prepare("at6", "CALL at6()");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
