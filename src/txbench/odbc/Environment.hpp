#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <sql.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
/// Wrapper for an ODBC environment handle
class Environment {
   private:
   /// The handle
   SQLHENV handle = SQL_NULL_HANDLE;

   friend class Connection;

   public:
   /// Constructor
   Environment();
   /// Destructor
   ~Environment();

   /// Allocate an environment handle
   void allocate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
