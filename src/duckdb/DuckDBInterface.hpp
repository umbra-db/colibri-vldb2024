#pragma once
//---------------------------------------------------------------------------
#include "duckdb/duckdb.h"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// A duckdb database
class DuckDBDatabase {
   /// The database
   duckdb_database database;

   friend class DuckDBConnection;

   public:
   /// Constructor
   explicit DuckDBDatabase(const std::string& path);
   /// Destructor
   ~DuckDBDatabase();
};
//---------------------------------------------------------------------------
/// A duckdb connection
class DuckDBConnection {
   /// The connection
   duckdb_connection connection;

   friend class DuckDBStatement;

   public:
   /// Constructor
   explicit DuckDBConnection(DuckDBDatabase& database);
   /// Constructor
   DuckDBConnection(DuckDBDatabase& database, const std::string& schema);
   /// Destructor
   ~DuckDBConnection();

   /// Run a sql command
   void exec(const std::string& command);
   /// Interrupt the connection
   void interrupt();
};
//---------------------------------------------------------------------------
/// A duckdb statement
class DuckDBStatement {
   /// The prepared statement
   duckdb_prepared_statement stmt;
   /// The result
   duckdb_result result;

   /// Bind a parameter
   void bindInteger(unsigned index, int value);
   /// Bind a parameter
   void bindString(unsigned index, const std::string& value);
   /// Bind a parameter
   void bindDouble(unsigned index, double value);
   /// Bind parameters
   template <typename... Tail>
   void bindImpl(unsigned index, int head, Tail&&... tail);
   /// Bind parameters
   template <typename... Tail>
   void bindImpl(unsigned index, const std::string& head, Tail&&... tail);
   /// Bind parameters
   template <typename... Tail>
   void bindImpl(unsigned index, double head, Tail&&... tail);

   public:
   /// Constructor
   DuckDBStatement();
   /// Destructor
   ~DuckDBStatement();

   /// Prepare the statement
   void prepare(DuckDBConnection& connection, const std::string& query);
   /// Bind parameters
   template <typename... Args>
   DuckDBStatement& bind(Args&&... args);
   /// Execute the statement
   void exec();
   /// Execute the statement
   DuckDBStatement& execWithResult();
   /// Fetch an integer
   DuckDBStatement& fetchInteger(size_t row, size_t col, int& value);
   /// Fetch a double
   DuckDBStatement& fetchDouble(size_t row, size_t col, int& value);
   /// Reset the result
   void resetResult();
};
//---------------------------------------------------------------------------
template <typename... Tail>
void DuckDBStatement::bindImpl(unsigned index, int head, Tail&&... tail)
// Bind parameters
{
   bindInteger(index, head);
   if constexpr (sizeof...(tail) > 0)
      bindImpl(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void DuckDBStatement::bindImpl(unsigned index, const std::string& head, Tail&&... tail)
// Bind parameters
{
   bindString(index, head);
   if constexpr (sizeof...(tail) > 0)
      bindImpl(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void DuckDBStatement::bindImpl(unsigned index, double head, Tail&&... tail)
// Bind parameters
{
   bindDouble(index, head);
   if constexpr (sizeof...(tail) > 0)
      bindImpl(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Args>
DuckDBStatement& DuckDBStatement::bind(Args&&... args)
// Bind parameters
{
   if constexpr (sizeof...(args) > 0)
      bindImpl(0, args...);

   return *this;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
