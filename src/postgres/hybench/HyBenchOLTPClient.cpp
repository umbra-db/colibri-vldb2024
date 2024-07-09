#include "postgres/hybench/HyBenchOLTPClient.hpp"
#include "postgres/hybench/HyBenchDriver.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include <cassert>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
HyBenchOLTPClient::HyBenchOLTPClient(const Database& database, HyBenchSharedState& state, unsigned clientId)
   : PostgresClient(database), config(hybench::Config::instantiate(database.getScaleFactor())), generator(config, clientId), transactionGenerator(42 + clientId, true), state(state)
// Constructor
{
}
//---------------------------------------------------------------------------
void HyBenchOLTPClient::performWork()
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
               int customerId = generator.generateCustomerId();
               statements[tx].bind(customerId);
            } break;
            case 1: {
               int companyId = generator.generateCompanyId();
               statements[tx].bind(companyId);
            } break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7: {
               int accountId = generator.generateCustomerOrCompanyId();
               statements[tx].bind(accountId);
            } break;
            case 8: {
               int sourceId = generator.generateCustomerOrCompanyId();
               int targetId = generator.generateCustomerOrCompanyId();
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
            case 9: {
               int sourceId = generator.generateCompanyId();
               uint64_t amount = generator.generateReal(0, config.companySavingBalance * 0.01) * 100;
               string type = "salary";
               std::stringstream ss;
               ss << generator.generateDateAfterMidPoint();
               string ts = ss.str();
               uint64_t tid = state.transferId.fetch_add(maxCompanySize);
               statements[tx].bind(tid, sourceId, amount, type, ts);
            } break;
            case 10: {
               int sourceId = generator.generateCustomerOrCompanyId();
               int targetId = generator.generateCustomerOrCompanyId();
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
            case 11: {
               int applicantId = generator.generateCustomerOrCompanyId();
               string status = "under_review";
               int duration = generator.generateLoanDuration();
               uint64_t amount;
               if (applicantId <= config.customerNumber) {
                  amount = generator.generateReal(0, config.customerLoanBalance) * 100;
               } else {
                  amount = generator.generateReal(0, config.companyLoanBalance) * 100;
               }
               statements[tx].bind(++state.loanappId, applicantId, amount, duration, status, config.getEndDate(), config.customerNumber);
            } break;
            case 12: {
               string status = generator.generateString({"accept", "reject"});
               std::stringstream ss;
               ss << generator.generateLoanDate();
               string ts = ss.str();
               statements[tx].bind(++state.loantransId, status, config.getEndDate(), config.customerNumber);
            } break;
            case 13:
            case 14:
            case 15: {
               statements[tx].bind(config.getEndDate());
            } break;
            case 16:
            case 17: {
               int accountId = generator.generateCustomerOrCompanyId();
               uint64_t amount;
               if (accountId <= config.customerNumber) {
                  amount = generator.generateReal(0, config.customerSavingBalance * 0.0001) * 100;
               } else {
                  amount = generator.generateReal(0, config.companySavingBalance * 0.0001) * 100;
               }
               statements[tx].bind(accountId, amount);
            } break;
            default: {
               assert(false);
               __builtin_unreachable();
            }
         }

         connection.send(statements[tx]);
         submittedTransactions.fetch_add(1, memory_order_acq_rel);
      }

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
void HyBenchOLTPClient::initialize()
// Initialize the client
{
   PostgresClient::initialize();

   maxCompanySize = [this]() {
      auto result = connection.execute("SELECT max(c) FROM (select count(*) as c from customer group by companyid) as t");

      uint64_t count;
      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), count);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse max company size '" + string(field) + "'");

      return count;
   }();

   statements[0] = connection.prepare("tp1", "CALL tp1($1::INTEGER)");
   statements[1] = connection.prepare("tp2", "CALL tp2($1::INTEGER)");
   statements[2] = connection.prepare("tp3", "CALL tp3($1::INTEGER)");
   statements[3] = connection.prepare("tp4", "CALL tp4($1::INTEGER)");
   statements[4] = connection.prepare("tp5", "CALL tp5($1::INTEGER)");
   statements[5] = connection.prepare("tp6", "CALL tp6($1::INTEGER)");
   statements[6] = connection.prepare("tp7", "CALL tp7($1::INTEGER)");
   statements[7] = connection.prepare("tp8", "CALL tp8($1::INTEGER)");
   statements[8] = connection.prepare("tp9", "CALL tp9($1::BIGINT, $2::INTEGER, $3::INTEGER, $4::BIGINT, $5::TEXT, $6::TEXT)");
   statements[9] = connection.prepare("tp10", "CALL tp10($1::BIGINT, $2::INTEGER, $3::BIGINT, $4::TEXT, $5::TEXT)");
   statements[10] = connection.prepare("tp11", "CALL tp11($1::BIGINT, $2::INTEGER, $3::INTEGER, $4::BIGINT, $5::TEXT, $6::TEXT)");
   statements[11] = connection.prepare("tp12", "CALL tp12($1::BIGINT, $2::INTEGER, $3::BIGINT, $4::INTEGER, $5::TEXT, $6::TEXT, $7::BIGINT)");
   statements[12] = connection.prepare("tp13", "CALL tp13($1::BIGINT, $2::TEXT, $3::TEXT, $4::BIGINT)");
   statements[13] = connection.prepare("tp14", "CALL tp14($1::TEXT)");
   statements[14] = connection.prepare("tp15", "CALL tp15($1::TEXT)");
   statements[15] = connection.prepare("tp16", "CALL tp16($1::TEXT)");
   statements[16] = connection.prepare("tp17", "CALL tp17($1::INTEGER, $2::BIGINT)");
   statements[17] = connection.prepare("tp18", "CALL tp18($1::INTEGER, $2::BIGINT)");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
