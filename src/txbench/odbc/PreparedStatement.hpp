#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <cstring>
#include <string>
#include <vector>
#include <sql.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// A fixed size sql string
template <size_t size>
struct SQLSTRING {
   /// The length of the string including the null byte
   static constexpr size_t length = size + 1;

   /// The string data
   char data[length] = {0};

   /// Constructor
   SQLSTRING() {}
   /// Constructor
   SQLSTRING(const std::string& text) {
      memcpy(&data[0], text.c_str(), text.size());
      data[text.size()] = '\0';
   }
};
//---------------------------------------------------------------------------
namespace odbc {
//---------------------------------------------------------------------------
class Connection;
//---------------------------------------------------------------------------
/// A prepared statement
class PreparedStatement {
   private:
   /// A scope guard to free resources associated with the prepared statement
   class FreeGuard;

   /// Parameter information
   struct Parameter {
      /// The SQL data type
      SQLSMALLINT dataType;
      /// The parameter size
      SQLULEN parameterSize;
      /// The parameter decimal digits (if any)
      SQLSMALLINT decimalDigits;
      /// Whether the parameter is nullable
      SQLSMALLINT nullable;
   };

   /// The array size
   unsigned arraySize;
   /// The statement handle
   SQLHSTMT handle = SQL_NULL_HANDLE;
   /// The connection
   Connection* connection;

   /// The parameter descriptors
   std::vector<Parameter> parameters;
   /// The number of processed parameters
   SQLULEN parametersProcessed;
   /// The state of the processed parameters
   std::vector<SQLUSMALLINT> parameterStatus;

   /// Free the handle
   void free();
   /// Bind a parameter
   void bindIntegerArray(unsigned index, SQLINTEGER* value);
   /// Bind a parameter
   void bindCharArray(unsigned index, SQLCHAR* value, size_t length);
   /// Bind a parameter
   void bindDoubleArray(unsigned index, SQLDOUBLE* value);
   /// Bind parameters
   template <typename... Tail>
   void bindImpl(unsigned index, SQLINTEGER* head, Tail&&... tail);
   /// Bind parameters
   template <size_t size, typename... Tail>
   void bindImpl(unsigned index, SQLSTRING<size>* head, Tail&&... tail);
   /// Bind parameters
   template <typename... Tail>
   void bindImpl(unsigned index, SQLDOUBLE* head, Tail&&... tail);

   public:
   /// Constructor
   explicit PreparedStatement(unsigned arraySize = 1);
   /// Destructor
   ~PreparedStatement();

   /// Prepare the statement
   void prepare(Connection& con, const std::string& query);
   /// Bind parameters
   template <typename... Args>
   void bind(Args&&... args);
   /// Execute the statement
   PreparedStatement& exec();
   /// Fetch the result
   PreparedStatement& fetchInt(int col, SQLINTEGER& result);
   /// Fetch the result
   PreparedStatement& fetchDouble(int col, SQLDOUBLE& result);
   /// Reset the statement
   void reset();
};
//---------------------------------------------------------------------------
template <typename... Tail>
void PreparedStatement::bindImpl(unsigned index, SQLINTEGER* head, Tail&&... tail)
// Bind parameters
{
   bindIntegerArray(index, head);
   if constexpr (sizeof...(tail) > 0)
      bindImpl(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <size_t size, typename... Tail>
void PreparedStatement::bindImpl(unsigned index, SQLSTRING<size>* head, Tail&&... tail)
// Bind parameters
{
   bindCharArray(index, reinterpret_cast<SQLCHAR*>(head), SQLSTRING<size>::length);
   if constexpr (sizeof...(tail) > 0)
      bindImpl(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void PreparedStatement::bindImpl(unsigned index, SQLDOUBLE* head, Tail&&... tail)
// Bind parameters
{
   bindDoubleArray(index, head);
   if constexpr (sizeof...(tail) > 0)
      bindImpl(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Args>
void PreparedStatement::bind(Args&&... args)
// Bind parameters
{
   if constexpr (sizeof...(args) > 0)
      bindImpl(0, args...);
}
//---------------------------------------------------------------------------
}
}
//---------------------------------------------------------------------------
