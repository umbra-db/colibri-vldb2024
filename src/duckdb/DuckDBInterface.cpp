#include "duckdb/DuckDBInterface.hpp"
#include "txbench/Exception.hpp"
#include "txbench/util/sql_helper.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
DuckDBDatabase::DuckDBDatabase(const string& path)
   : database(nullptr)
// Constructor
{
   if (duckdb_open(path.c_str(), &database) == DuckDBError) {
      throw Exception("Failed to open database");
   }
}
//---------------------------------------------------------------------------
DuckDBDatabase::~DuckDBDatabase()
// Destructor
{
   duckdb_close(&database);
}
//---------------------------------------------------------------------------
DuckDBConnection::DuckDBConnection(DuckDBDatabase& database)
   : connection(nullptr)
// Constructor
{
   if (duckdb_connect(database.database, &connection) == DuckDBError) {
      throw Exception("Failed to connect to database");
   }
}
//---------------------------------------------------------------------------
DuckDBConnection::DuckDBConnection(DuckDBDatabase& database, const std::string& schema)
   : DuckDBConnection(database)
// Constructor
{
   exec("use " + schema + ";");
}
//---------------------------------------------------------------------------
DuckDBConnection::~DuckDBConnection()
// Destructor
{
   duckdb_disconnect(&connection);
}
//---------------------------------------------------------------------------
void DuckDBConnection::exec(const std::string& command)
// Run a sql command
{
   std::vector<string> queries;
   splitSQL(command, queries);

   for (const auto& q : queries) {
      if (duckdb_query(connection, q.c_str(), nullptr) == DuckDBError) {
         throw Exception("Failed to execute query: " + q);
      }
   }
}
//---------------------------------------------------------------------------
void DuckDBConnection::interrupt()
// Interrupt the connection
{
   duckdb_interrupt(connection);
}
//---------------------------------------------------------------------------
DuckDBStatement::DuckDBStatement()
   : stmt(nullptr)
// Constructor
{
}
//---------------------------------------------------------------------------
DuckDBStatement::~DuckDBStatement()
// Destructor
{
   duckdb_destroy_prepare(&stmt);
}
//---------------------------------------------------------------------------
void DuckDBStatement::prepare(DuckDBConnection& connection, const std::string& query)
// Prepare the statement
{
   if (duckdb_prepare(connection.connection, query.c_str(), &stmt) == DuckDBError) {
      throw Exception("Failed to prepare statement");
   }
}
//---------------------------------------------------------------------------
void DuckDBStatement::bindInteger(unsigned index, int value)
// Bind a parameter
{
   duckdb_bind_int32(stmt, index + 1, value);
}
//---------------------------------------------------------------------------
void DuckDBStatement::bindString(unsigned index, const std::string& value)
// Bind a parameter
{
   duckdb_bind_varchar(stmt, index + 1, value.c_str());
}
//---------------------------------------------------------------------------
void DuckDBStatement::bindDouble(unsigned index, double value)
// Bind a parameter
{
   duckdb_bind_double(stmt, index + 1, value);
}
//---------------------------------------------------------------------------
void DuckDBStatement::exec()
// Execute the statement
{
   if (duckdb_execute_prepared(stmt, nullptr) == DuckDBError) {
      throw Exception("Failed to execute statement");
   }
}
//---------------------------------------------------------------------------
DuckDBStatement& DuckDBStatement::execWithResult()
// Execute the statement
{
   if (duckdb_execute_prepared(stmt, &result) == DuckDBError) {
      throw Exception("Failed to execute statement: "s + duckdb_result_error(&result));
   }

   return *this;
}
//---------------------------------------------------------------------------
DuckDBStatement& DuckDBStatement::fetchInteger(size_t row, size_t col, int& value)
// Fetch an integer
{
   value = duckdb_value_int32(&result, col, row);
   return *this;
}
//---------------------------------------------------------------------------
DuckDBStatement& DuckDBStatement::fetchDouble(size_t row, size_t col, int& value)
// Fetch a double
{
   value = duckdb_value_double(&result, col, row);
   return *this;
}
//---------------------------------------------------------------------------
void DuckDBStatement::resetResult()
// Reset the result
{
   duckdb_destroy_result(&result);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
