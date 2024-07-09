#pragma once
//---------------------------------------------------------------------------
#include "duckdb/DuckDBInterface.hpp"
#include "txbench/DatabaseClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// A client
class DuckDBClient : public DatabaseClient {
   protected:
   /// The connection
   std::unique_ptr<DuckDBConnection> connection;

   /// Cancel the running queries
   void cancelQueries() override;

   public:
   /// Constructor
   explicit DuckDBClient(const Database& database);
   /// Destructor
   virtual ~DuckDBClient();

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
