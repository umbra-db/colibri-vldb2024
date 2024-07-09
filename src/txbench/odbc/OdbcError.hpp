#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <array>
#include <string_view>
#include <sql.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
/// Wrapper for an ODBC error
class OdbcError {
   public:
   /// Check if a return value indicates an error
   static bool hasError(SQLRETURN retval);
   /// Check if a return value indicates that diagnostics are available
   static bool hasDiagnostics(SQLRETURN retval);
   /// Convert an ODBC error to an error message
   static std::string mapErrorMessage(SQLSMALLINT handleType, SQLHANDLE handle);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
