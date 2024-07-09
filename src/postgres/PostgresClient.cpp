#include "postgres/PostgresClient.hpp"
#include "postgres/Postgres.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
PostgresClient::PostgresClient(const Database& database)
   : DatabaseClient(database)
// Constructor
{
}
//---------------------------------------------------------------------------
PostgresClient::~PostgresClient()
// Destructor
{
}
//---------------------------------------------------------------------------
void PostgresClient::cancelQueries()
// Cancel the running queries
{
   connection.cancel();
}
//---------------------------------------------------------------------------
void PostgresClient::initialize()
// Initialize the client
{
   connection.connect(database.getConnectionString());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
