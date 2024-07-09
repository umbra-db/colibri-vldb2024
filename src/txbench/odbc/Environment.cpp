#include "txbench/odbc/Environment.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/OdbcError.hpp"
#include <sqlext.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
Environment::Environment()
// Constructor
{
}
//---------------------------------------------------------------------------
Environment::~Environment()
// Destructor
{
   if (handle != SQL_NULL_HANDLE)
      SQLFreeHandle(SQL_HANDLE_ENV, handle);
}
//---------------------------------------------------------------------------
void Environment::allocate()
// Allocate an environment handle
{
   if (auto ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &handle); OdbcError::hasError(ret))
      throw Exception("unable to allocate environment: " + OdbcError::mapErrorMessage(SQL_HANDLE_ENV, SQL_NULL_HANDLE));
   if (auto ret = SQLSetEnvAttr(handle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 0); OdbcError::hasError(ret))
      throw Exception("unable to allocate environment: " + OdbcError::mapErrorMessage(SQL_HANDLE_ENV, handle));
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------