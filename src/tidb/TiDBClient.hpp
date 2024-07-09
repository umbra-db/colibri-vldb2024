#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseClient.hpp"
#include "txbench/odbc/Connection.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// A client
class TiDBClient : public DatabaseClient {
   protected:
   /// The environment
   const odbc::Environment& environment;
   /// The connection
   odbc::Connection connection;

   /// Cancel the running queries
   void cancelQueries() override;

   public:
   /// Constructor
   explicit TiDBClient(const Database& database, const odbc::Environment& environment);
   /// Destructor
   virtual ~TiDBClient();

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
