#include "generator/TpccGenerator.hpp"
#include "txbench/io/CsvWriter.hpp"
#include "txbench/io/ProgressPrinter.hpp"
#include <algorithm>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
TpccGenerator::TpccGenerator(unsigned scaleFactor, const string& basedir)
   : scaleFactor(scaleFactor), basedir(std::move(basedir)), generator(42)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpccGenerator::generateItem()
// Generate the item table
{
   ProgressPrinter progress("generating items", tpcc::TpccGenerator::itemCount);
   CsvWriter writer(basedir + "/item.csv");
   for (unsigned item = 0; item < tpcc::TpccGenerator::itemCount; ++item) {
      writer << (item + 1); // I_ID
      writer << generator.generateRand(1, 10000); // I_IM_ID
      writer << generator.generateAlphanumString(14, 24); // I_NAME
      writer << CsvWriter::setprecision(2) << (generator.generateRand(100, 10000) / 100.0); // I_PRICE
      writer << generator.generateData(); // I_DATA
      writer << CsvWriter::endl;

      progress.update(item + 1);
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generateWarehouse()
// Generate the warehouse table
{
   ProgressPrinter progress("generating warehouses", scaleFactor);
   CsvWriter writer(basedir + "/warehouse.csv");
   for (unsigned warehouse = 0; warehouse < scaleFactor; ++warehouse) {
      writer << (warehouse + 1); // W_ID
      writer << generator.generateAlphanumString(6, 10); // W_NAME
      writer << generator.generateAlphanumString(10, 20); // W_STREET_1
      writer << generator.generateAlphanumString(10, 20); // W_STREET_2
      writer << generator.generateAlphanumString(10, 20); // W_CITY
      writer << generator.generateAlphanumString(2); // W_STATE
      writer << generator.generateZipCode(); // W_ZIP
      writer << CsvWriter::setprecision(4) << (generator.generateRand(0, 2000) / 10000.0); // W_TAX
      writer << CsvWriter::setprecision(2) << 300000.0; // W_YTD
      writer << CsvWriter::endl;

      progress.update(warehouse + 1);
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generateStock()
// Generate the stock table
{
   ProgressPrinter progress("generating stock", scaleFactor * tpcc::TpccGenerator::itemCount);
   CsvWriter writer(basedir + "/stock.csv");
   for (unsigned warehouse = 0; warehouse < scaleFactor; ++warehouse) {
      for (unsigned item = 0; item < tpcc::TpccGenerator::itemCount; ++item) {
         writer << (item + 1); // S_I_ID
         writer << (warehouse + 1); // S_W_ID
         writer << generator.generateRand(10, 100); // S_QUANTITY
         for (unsigned i = 0; i < 10; ++i)
            writer << generator.generateAlphanumString(24); // S_DIST_*
         writer << 0; // S_YTD
         writer << 0; // S_ORDER_CNT
         writer << 0; // S_REMOTE_CNT
         writer << generator.generateData(); // S_DATA
         writer << CsvWriter::endl;

         progress.update((warehouse * tpcc::TpccGenerator::itemCount) + item + 1);
      }
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generateDistrict()
// Generate the district table
{
   ProgressPrinter progress("generating districts", scaleFactor * tpcc::TpccGenerator::districtsPerWarehouse);
   CsvWriter writer(basedir + "/district.csv");
   for (unsigned warehouse = 0; warehouse < scaleFactor; ++warehouse) {
      for (unsigned district = 0; district < tpcc::TpccGenerator::districtsPerWarehouse; ++district) {
         writer << (district + 1); // D_ID
         writer << (warehouse + 1); // D_W_ID
         writer << generator.generateAlphanumString(6, 10); // D_NAME
         writer << generator.generateAlphanumString(10, 20); // D_STREET_1
         writer << generator.generateAlphanumString(10, 20); // D_STREET_2
         writer << generator.generateAlphanumString(10, 20); // D_CITY
         writer << generator.generateAlphanumString(2); // D_STATE
         writer << generator.generateZipCode(); // D_ZIP
         writer << CsvWriter::setprecision(4) << (generator.generateRand(0, 2000) / 10000.0); // D_TAX
         writer << CsvWriter::setprecision(2) << 30000.0; // D_YTD
         writer << 3001; // D_NEXT_O_ID
         writer << CsvWriter::endl;

         progress.update((warehouse * tpcc::TpccGenerator::districtsPerWarehouse) + district + 1);
      }
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generateCustomer()
// Generate the customer table
{
   ProgressPrinter progress("generating customers", scaleFactor * tpcc::TpccGenerator::districtsPerWarehouse * tpcc::TpccGenerator::customersPerDistrict);
   CsvWriter writer(basedir + "/customer.csv");
   for (unsigned warehouse = 0; warehouse < scaleFactor; ++warehouse) {
      for (unsigned district = 0; district < tpcc::TpccGenerator::districtsPerWarehouse; ++district) {
         for (unsigned customer = 0; customer < tpcc::TpccGenerator::customersPerDistrict; ++customer) {
            writer << (customer + 1); // C_ID
            writer << (district + 1); // C_D_ID
            writer << (warehouse + 1); // C_W_ID

            writer << generator.generateAlphanumString(8, 16); // C_FIRST
            writer << "OE"; // C_MIDDLE

            if (customer < 1000) {
               writer << generator.generateCustomerLastname(customer); // C_LAST
            } else {
               writer << generator.generateCustomerLastname(generator.generateNURand<unsigned>(255, 0, 999)); // C_LAST
            }

            writer << generator.generateAlphanumString(10, 20); // C_STREET_1
            writer << generator.generateAlphanumString(10, 20); // C_STREET_2
            writer << generator.generateAlphanumString(10, 20); // C_CITY
            writer << generator.generateAlphanumString(2); // C_STATE
            writer << generator.generateZipCode(); // C_ZIP
            writer << generator.generateNumString(16); // C_PHONE
            writer << generator.generateDatetime(); // C_SINCE

            if (generator.generateRand(1, 100) <= 10)
               writer << "BC"; // C_CREDIT
            else
               writer << "GC"; // C_CREDIT

            writer << CsvWriter::setprecision(2) << 50000.0; // C_CREDIT_LIM
            writer << CsvWriter::setprecision(4) << (generator.generateRand(0, 5000) / 10000.0); // C_DISCOUNT
            writer << CsvWriter::setprecision(2) << -10.0; // C_BALANCE
            writer << CsvWriter::setprecision(2) << 10.0; // C_YTD_PAYMENT
            writer << 1; // C_PAYMENT_CNT
            writer << 0; // C_DELIVERY_CNT
            writer << generator.generateAlphanumString(300, 500); // C_DATA
            writer << CsvWriter::endl;

            progress.update((((warehouse * tpcc::TpccGenerator::districtsPerWarehouse) + district) * tpcc::TpccGenerator::customersPerDistrict) + customer + 1);
         }
      }
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generateHistory()
// Generate the history table
{
   ProgressPrinter progress("generating history", scaleFactor * tpcc::TpccGenerator::districtsPerWarehouse * tpcc::TpccGenerator::customersPerDistrict);
   CsvWriter writer(basedir + "/history.csv");
   for (unsigned warehouse = 0; warehouse < scaleFactor; ++warehouse) {
      for (unsigned district = 0; district < tpcc::TpccGenerator::districtsPerWarehouse; ++district) {
         for (unsigned customer = 0; customer < tpcc::TpccGenerator::customersPerDistrict; ++customer) {
            writer << (customer + 1); // H_C_ID
            writer << (district + 1); // H_C_D_ID
            writer << (warehouse + 1); // H_C_W_ID
            writer << (district + 1); // H_D_ID
            writer << (warehouse + 1); // H_W_ID
            writer << generator.generateDatetime(); // H_DATE
            writer << CsvWriter::setprecision(2) << 10.0; // H_AMOUNT
            writer << generator.generateAlphanumString(12, 24); // H_DATA

            writer << CsvWriter::endl;

            progress.update((((warehouse * tpcc::TpccGenerator::districtsPerWarehouse) + district) * tpcc::TpccGenerator::customersPerDistrict) + customer + 1);
         }
      }
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generateOrders()
// Generate the orders table
{
   ProgressPrinter progress("generating orders", scaleFactor * tpcc::TpccGenerator::districtsPerWarehouse * tpcc::TpccGenerator::customersPerDistrict);
   CsvWriter ordersWriter(basedir + "/orders.csv");
   CsvWriter newOrderWriter(basedir + "/new_orders.csv");
   CsvWriter orderLineWriter(basedir + "/order_line.csv");
   for (unsigned warehouse = 0; warehouse < scaleFactor; ++warehouse) {
      for (unsigned district = 0; district < tpcc::TpccGenerator::districtsPerWarehouse; ++district) {
         vector<unsigned> customerIds(tpcc::TpccGenerator::customersPerDistrict);
         iota(customerIds.begin(), customerIds.end(), 1);
         shuffle(customerIds.begin(), customerIds.end(), generator.getEngine());

         for (unsigned order = 0; order < tpcc::TpccGenerator::customersPerDistrict; ++order) {
            ordersWriter << (order + 1); // O_ID
            ordersWriter << customerIds[order]; // O_C_ID
            ordersWriter << (district + 1); // O_D_ID
            ordersWriter << (warehouse + 1); // O_W_ID
            auto entryD = generator.generateDatetime();
            ordersWriter << entryD; // O_ENTRY_D

            if (order < 2100)
               ordersWriter << generator.generateRand(1, 10); // O_CARRIER_ID
            else
               ordersWriter << ""; // O_CARRIER_ID

            unsigned olCnt = generator.generateRand(5, 15);
            ordersWriter << olCnt; // O_OL_CNT
            ordersWriter << 1; // O_ALL_LOCAL
            ordersWriter << CsvWriter::endl;

            for (unsigned ol = 0; ol < olCnt; ++ol) {
               orderLineWriter << (order + 1); // OL_O_ID
               orderLineWriter << (district + 1); // OL_D_ID
               orderLineWriter << (warehouse + 1); // OL_W_ID
               orderLineWriter << (ol + 1); // OL_NUMBER
               orderLineWriter << generator.generateRand(1, 100000); // OL_I_ID
               orderLineWriter << (warehouse + 1); // OL_SUPPLY_W_ID

               if (order < 2100)
                  orderLineWriter << entryD; // OL_DELIVERY_D
               else
                  orderLineWriter << ""; // OL_DELIVERY_D

               orderLineWriter << 5; // OL_QUANTITY

               if (order < 2100)
                  orderLineWriter << CsvWriter::setprecision(2) << 0.0; // OL_AMOUNT
               else
                  orderLineWriter << CsvWriter::setprecision(2) << (generator.generateRand(1, 999999) / 100.0); // OL_AMOUNT

               orderLineWriter << generator.generateAlphanumString(24); // OL_DIST_INFO
               orderLineWriter << CsvWriter::endl;
            }

            if (order >= 2100) {
               newOrderWriter << (order + 1); // NO_O_ID
               newOrderWriter << (district + 1); // NO_D_ID
               newOrderWriter << (warehouse + 1); // NO_W_ID
               newOrderWriter << CsvWriter::endl;
            }

            progress.update((((warehouse * tpcc::TpccGenerator::districtsPerWarehouse) + district) * tpcc::TpccGenerator::customersPerDistrict) + order + 1);
         }
      }
   }
}
//---------------------------------------------------------------------------
void TpccGenerator::generate()
// Generate data
{
   generateItem();
   generateWarehouse();
   generateStock();
   generateDistrict();
   generateCustomer();
   generateHistory();
   generateOrders();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
