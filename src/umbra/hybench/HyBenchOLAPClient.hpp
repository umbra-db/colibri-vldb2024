#pragma once
//---------------------------------------------------------------------------
#include "txbench/hybench/Generator.hpp"
#include "txbench/hybench/QueryGenerator.hpp"
#include "umbra/UmbraClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The olap client for TPCH
class HyBenchOLAPClient : public UmbraClient {
   /// The config
   hybench::Config config;
   /// The generator
   hybench::Generator generator;
   /// The query generator
   hybench::QueryGenerator queryGenerator;
   /// The query statements
   std::vector<pq::PqPreparedStatement> statements;

   protected:
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HyBenchOLAPClient(const Database& database, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
