#pragma once
//---------------------------------------------------------------------------
#include "txbench/tpcc/TpccGenerator.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
class TpccGenerator {
   protected:
   /// The scale factor
   unsigned scaleFactor;
   /// The output directory
   const std::string& basedir;

   private:
   /// The generator
   tpcc::TpccGenerator generator;

   /// Generate the item table
   void generateItem();
   /// Generate the warehouse table
   void generateWarehouse();
   /// Generate the stock table
   void generateStock();
   /// Generate the district table
   void generateDistrict();
   /// Generate the customer table
   void generateCustomer();
   /// Generate the history table
   void generateHistory();
   /// Generate the order tables
   void generateOrders();

   public:
   /// Constructor
   TpccGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   virtual void generate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
