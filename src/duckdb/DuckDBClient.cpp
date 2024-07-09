#include "duckdb/DuckDBClient.hpp"
#include "duckdb/DuckDB.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
DuckDBClient::DuckDBClient(const Database& database)
   : DatabaseClient(database)
// Constructor
{
}
//---------------------------------------------------------------------------
DuckDBClient::~DuckDBClient()
// Destructor
{
}
//---------------------------------------------------------------------------
void DuckDBClient::cancelQueries()
// Cancel the running queries
{
   connection->interrupt();
}
//---------------------------------------------------------------------------
void DuckDBClient::initialize()
// Initialize the client
{
   connection = reinterpret_cast<const DuckDB&>(database).openConnection();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
