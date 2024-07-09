#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseClient.hpp"
#include "txbench/odbc/Connection.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// A client
class SingleStoreClient : public DatabaseClient {
   protected:
   /// The environment
   const odbc::Environment& environment;
   /// The connection
   odbc::Connection connection;

   /// Cancel the running queries
   void cancelQueries() override;

   public:
   /// Constructor
   explicit SingleStoreClient(const Database& database, const odbc::Environment& environment);
   /// Destructor
   virtual ~SingleStoreClient();

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
