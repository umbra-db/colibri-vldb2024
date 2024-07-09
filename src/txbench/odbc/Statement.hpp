#pragma once
//---------------------------------------------------------------------------
#include <string>
#include <sql.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
class Connection;
//---------------------------------------------------------------------------
/// An ad-hoc statement
class Statement {
   private:
   /// The statement handle
   SQLHSTMT handle = SQL_NULL_HANDLE;
   /// The connection
   Connection* connection;

   public:
   /// Constructor
   Statement();
   /// Destructor
   ~Statement();

   /// Get the statement handle
   SQLHSTMT getHandle() const { return handle; }

   /// Allocate the statement handle
   void allocate(Connection& con);

   /// Execute a command
   void exec(const std::string& command);
   /// Bind a result column
   void bindColumn(unsigned column, SQLINTEGER& target);
   /// Fetch the next result row
   bool next();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
