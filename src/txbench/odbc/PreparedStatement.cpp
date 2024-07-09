#include "txbench/odbc/PreparedStatement.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Connection.hpp"
#include "txbench/odbc/OdbcError.hpp"
#include <iostream>
#include <sqlext.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
/// A scope guard to free resources
class PreparedStatement::FreeGuard {
   private:
   /// The statement
   PreparedStatement* statement;
   /// Whether the guard is enabled
   bool enabled;

   public:
   /// Constructor
   explicit FreeGuard(PreparedStatement& statement);
   /// Destructor
   ~FreeGuard();

   /// Disable the guard
   void disable();
};
//---------------------------------------------------------------------------
PreparedStatement::FreeGuard::FreeGuard(PreparedStatement& statement)
   : statement(&statement), enabled(true)
// Constructor
{
}
//---------------------------------------------------------------------------
PreparedStatement::FreeGuard::~FreeGuard()
// Destructor
{
   if (enabled)
      statement->free();
}
//---------------------------------------------------------------------------
void PreparedStatement::FreeGuard::disable()
// Disable the guard
{
   enabled = false;
}
//---------------------------------------------------------------------------
PreparedStatement::PreparedStatement(unsigned arraySize)
   : arraySize(arraySize)
// Constructor
{
}
//---------------------------------------------------------------------------
PreparedStatement::~PreparedStatement()
// Destructor
{
   free();
}
//---------------------------------------------------------------------------
void PreparedStatement::free()
// Free the handle
{
   if (handle != SQL_NULL_HANDLE)
      SQLFreeHandle(SQL_HANDLE_STMT, handle);
}
//---------------------------------------------------------------------------
void PreparedStatement::bindIntegerArray(unsigned index, SQLINTEGER* value)
// Bind a parameter
{
   if (index >= parameters.size())
      throw Exception("parameter index out of range"sv);
   if (auto ret = SQLBindParameter(handle, index + 1, SQL_PARAM_INPUT, SQL_C_LONG, parameters[index].dataType, parameters[index].parameterSize, parameters[index].decimalDigits, value, 0, nullptr); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
}
//---------------------------------------------------------------------------
void PreparedStatement::bindCharArray(unsigned index, SQLCHAR* value, size_t length)
// Bind a parameter
{
   if (index >= parameters.size())
      throw Exception("parameter index out of range"sv);
   if (auto ret = SQLBindParameter(handle, index + 1, SQL_PARAM_INPUT, SQL_C_CHAR, parameters[index].dataType, parameters[index].parameterSize, parameters[index].decimalDigits, value, length, nullptr); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
}
//---------------------------------------------------------------------------
void PreparedStatement::bindDoubleArray(unsigned index, SQLDOUBLE* value)
// Bind a parameter
{
   if (index >= parameters.size())
      throw Exception("parameter index out of range"sv);
   if (auto ret = SQLBindParameter(handle, index + 1, SQL_PARAM_INPUT, SQL_C_DOUBLE, parameters[index].dataType, parameters[index].parameterSize, parameters[index].decimalDigits, value, 0, nullptr); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
}
//---------------------------------------------------------------------------
void PreparedStatement::prepare(Connection& con, const std::string& query)
// Prepare a statement
{
   // Automatically free the handle when returning
   FreeGuard guard(*this);

   connection = &con;

   if (auto ret = SQLAllocHandle(SQL_HANDLE_STMT, con.handle, &handle); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_DBC, con.handle));

   if (auto ret = SQLPrepare(handle, reinterpret_cast<SQLCHAR*>(const_cast<char*>(query.data())), query.size()); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   SQLSMALLINT numParams;
   if (auto ret = SQLNumParams(handle, &numParams); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   // Get parameter information
   parameters.resize(numParams);
   for (unsigned i = 0; i < numParams; ++i)
      if (auto ret = SQLDescribeParam(handle, i + 1, &parameters[i].dataType, &parameters[i].parameterSize, &parameters[i].decimalDigits, &parameters[i].nullable); OdbcError::hasError(ret))
         throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   // Configure parameter array binding
   parameterStatus.resize(arraySize);
   if (auto ret = SQLSetStmtAttr(handle, SQL_ATTR_PARAM_BIND_TYPE, SQL_PARAM_BIND_BY_COLUMN, 0); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
   if (auto ret = SQLSetStmtAttr(handle, SQL_ATTR_PARAMSET_SIZE, reinterpret_cast<SQLPOINTER>(arraySize), 0); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
   if (auto ret = SQLSetStmtAttr(handle, SQL_ATTR_PARAMS_PROCESSED_PTR, &parametersProcessed, 0); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
   if (auto ret = SQLSetStmtAttr(handle, SQL_ATTR_PARAM_STATUS_PTR, parameterStatus.data(), 0); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   guard.disable();
}
//---------------------------------------------------------------------------
PreparedStatement& PreparedStatement::exec()
// Execute the statement
{
   connection->runningQuery.store(handle);
   auto qret = SQLExecute(handle);
   connection->runningQuery.store(SQL_NULL_HANDLE);

   if (OdbcError::hasDiagnostics(qret)) {
      std::array<char, 5> status;
      std::array<char, 256> message;

      SQLINTEGER nativeError;
      SQLSMALLINT textSize;

      for (unsigned i = 1;; ++i) {
         auto dret = SQLGetDiagRec(SQL_HANDLE_STMT, handle, i, reinterpret_cast<SQLCHAR*>(status.data()), &nativeError, reinterpret_cast<SQLCHAR*>(message.data()), message.size(), &textSize);
         if (dret == SQL_NO_DATA)
            break;

         cerr << string(status.data(), status.size()) << "/" << nativeError << ": " + string(message.data(), textSize) << endl;
      }
   }
   if (OdbcError::hasError(qret))
      throw Exception("exec(): " + OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   // Check to see which sets of parameters were processed successfully.
   for (auto i = 0; i < parametersProcessed; i++) {
      if (parameterStatus[i] != SQL_PARAM_SUCCESS && parameterStatus[i] != SQL_PARAM_SUCCESS_WITH_INFO)
         throw Exception("exec(): processing parameter " + to_string(i) + "failed");
   }

   if (parametersProcessed != arraySize)
      throw Exception("exec(): processed fewer parameters than specified (parametersProcessed: " + to_string(parametersProcessed) + ", arraySize: " + to_string(arraySize) + ")");

   return *this;
}
//---------------------------------------------------------------------------
PreparedStatement& PreparedStatement::fetchInt(int col, SQLINTEGER& result)
// Fetch the result
{
   if (auto ret = SQLFetch(handle); OdbcError::hasError(ret))
      throw Exception("fetch(): " + OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   SQLLEN indicator = 0;
   if (auto ret = SQLGetData(handle, col, SQL_C_DEFAULT, &result, 0, &indicator); OdbcError::hasError(ret))
      throw Exception("getData(): " + OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   return *this;
}
//---------------------------------------------------------------------------
PreparedStatement& PreparedStatement::fetchDouble(int col, SQLDOUBLE& result)
// Fetch the result
{
   if (auto ret = SQLFetch(handle); OdbcError::hasError(ret))
      throw Exception("fetch(): " + OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   SQLLEN indicator = 0;
   if (auto ret = SQLGetData(handle, col, SQL_C_DOUBLE, &result, 0, &indicator); OdbcError::hasError(ret))
      throw Exception("getData(): " + OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));

   return *this;
}
//---------------------------------------------------------------------------
void PreparedStatement::reset()
// Reset the statement
{
   if (auto ret = SQLFreeStmt(handle, SQL_CLOSE); OdbcError::hasError(ret))
      throw Exception("free(): " + OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
