#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseClient.hpp"
#include "txbench/pq/PqConnection.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
class Umbra;
//---------------------------------------------------------------------------
/// A client
class UmbraClient : public DatabaseClient {
   protected:
   /// The connection
   pq::PqConnection connection;

   public:
   /// Constructor
   explicit UmbraClient(const Database& database);
   /// Destructor
   ~UmbraClient() override;

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
