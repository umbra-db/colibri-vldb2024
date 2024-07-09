#include "umbra/UmbraClient.hpp"
#include "umbra/Umbra.hpp"
#include "umbra/UmbraConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
UmbraClient::UmbraClient(const Database& database)
   : DatabaseClient(database)
// Constructor
{
}
//---------------------------------------------------------------------------
UmbraClient::~UmbraClient()
// Destructor
{
}
//---------------------------------------------------------------------------
void UmbraClient::initialize()
// Initialize the client
{
   connection.connect(database.getConnectionString());

   {
      // Set the commit behavior
      connection.execute("SET async_commit = " + reinterpret_cast<const UmbraConnectionConfig&>(database.getConnectionConfig()).getAsyncCommit());

      // Set the verbosity
      connection.execute("SET debug.verbosity = 'fatal'");
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
