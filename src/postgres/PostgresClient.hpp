#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseClient.hpp"
#include "txbench/pq/PqConnection.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// A client
class PostgresClient : public DatabaseClient {
   protected:
   /// The connection
   pq::PqConnection connection;

   /// Cancel the running queries
   void cancelQueries() override;

   public:
   /// Constructor
   explicit PostgresClient(const Database& database);
   /// Destructor
   ~PostgresClient() override;

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
