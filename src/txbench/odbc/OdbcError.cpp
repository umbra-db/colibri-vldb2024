#include "txbench/odbc/OdbcError.hpp"
#include <cassert>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
bool OdbcError::hasError(SQLRETURN retval)
// Check if a return value indicates an error
{
   return (retval != SQL_SUCCESS) && (retval != SQL_SUCCESS_WITH_INFO);
}
//---------------------------------------------------------------------------
bool OdbcError::hasDiagnostics(SQLRETURN retval)
// Check if a return value indicates that diagnostics are available
{
   return (retval == SQL_ERROR) || (retval == SQL_SUCCESS_WITH_INFO);
}
//---------------------------------------------------------------------------
string OdbcError::mapErrorMessage(SQLSMALLINT handleType, SQLHANDLE handle)
// Map an error
{
   // Buffers for the error code and message
   std::array<char, 5> status;
   std::array<char, 256> message;

   SQLINTEGER nativeError;
   SQLSMALLINT textSize;
   auto ret = SQLGetDiagRec(handleType, handle, 1, reinterpret_cast<SQLCHAR*>(status.data()), &nativeError, reinterpret_cast<SQLCHAR*>(message.data()), message.size(), &textSize);
   if (ret == SQL_NO_DATA || ret == SQL_INVALID_HANDLE || ret == SQL_ERROR)
      return "could not map error message";

   assert(textSize <= message.size());
   return string(status.data(), status.size()) + ": " + string(message.data(), textSize);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
