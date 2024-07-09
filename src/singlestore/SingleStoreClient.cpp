#include "singlestore/SingleStore.hpp"
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/odbc/OdbcConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
SingleStoreClient::SingleStoreClient(const Database& database, const odbc::Environment& environment)
   : DatabaseClient(database), environment(environment)
// Constructor
{
}
//---------------------------------------------------------------------------
SingleStoreClient::~SingleStoreClient()
// Destructor
{
}
//---------------------------------------------------------------------------
void SingleStoreClient::cancelQueries()
// Cancel the running queries
{
   connection.cancel();
}
//---------------------------------------------------------------------------
void SingleStoreClient::initialize()
// Initialize the client
{
   connection.connect(environment, database.getConnectionString());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
