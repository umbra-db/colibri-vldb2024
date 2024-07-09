#include "tidb/TiDB.hpp"
#include "tidb/TiDBClient.hpp"
#include "txbench/odbc/OdbcConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
TiDBClient::TiDBClient(const Database& database, const odbc::Environment& environment)
   : DatabaseClient(database), environment(environment)
// Constructor
{
}
//---------------------------------------------------------------------------
TiDBClient::~TiDBClient()
// Destructor
{
}
//---------------------------------------------------------------------------
void TiDBClient::cancelQueries()
// Cancel the running queries
{
   connection.cancel();
}
//---------------------------------------------------------------------------
void TiDBClient::initialize()
// Initialize the client
{
   connection.connect(environment, database.getConnectionString());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
