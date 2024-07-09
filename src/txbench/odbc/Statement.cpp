#include "txbench/odbc/Statement.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Connection.hpp"
#include "txbench/odbc/OdbcError.hpp"
#include <mutex>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
Statement::Statement()
// Constructor
{
}
//---------------------------------------------------------------------------
Statement::~Statement()
// Destructor
{
   if (handle != SQL_NULL_HANDLE) {
      unique_lock lock(connection->mutex);
      SQLFreeHandle(SQL_HANDLE_STMT, handle);
   }
}
//---------------------------------------------------------------------------
void Statement::allocate(Connection& con)
// Allocate the handle
{
   connection = &con;

   if (auto ret = SQLAllocHandle(SQL_HANDLE_STMT, con.handle, &handle); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_DBC, con.handle));
}
//---------------------------------------------------------------------------
void Statement::exec(const string& command)
// Execute a statement
{
   connection->runningQuery.store(handle);
   if (auto ret = SQLExecDirect(handle, reinterpret_cast<SQLCHAR*>(const_cast<char*>(command.data())), command.size()); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
   connection->runningQuery.store(SQL_NULL_HANDLE);
}
//---------------------------------------------------------------------------
void Statement::bindColumn(unsigned column, SQLINTEGER& target)
// Bind a result column
{
   if (auto ret = SQLBindCol(handle, column, SQL_INTEGER, &target, sizeof(target), nullptr); OdbcError::hasError(ret))
      throw Exception(OdbcError::mapErrorMessage(SQL_HANDLE_STMT, handle));
}
//---------------------------------------------------------------------------
bool Statement::next()
// Fetch the next row
{
   if (auto ret = SQLFetch(handle); OdbcError::hasError(ret))
      return false;

   return true;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
