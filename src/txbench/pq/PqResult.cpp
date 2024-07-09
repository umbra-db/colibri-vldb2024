#include "txbench/pq/PqResult.hpp"
#include <cassert>
#include <string>
#include <libpq-fe.h>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
PqResult::PqResult()
// Constructor
{
}
//---------------------------------------------------------------------------
PqResult::PqResult(pg_result* res)
   : res(res)
// Constructor
{
}
//---------------------------------------------------------------------------
PqResult::PqResult(PqResult&& other) noexcept
   : res(other.res)
// Move constructor
{
   other.res = nullptr;
}
//---------------------------------------------------------------------------
PqResult::~PqResult()
// Destructor
{
   free();
}
//---------------------------------------------------------------------------
PqResult& PqResult::operator=(PqResult&& other) noexcept
// Move assignment
{
   if (this != &other) {
      free();

      res = other.res;
      other.res = nullptr;
   }
   return *this;
}
//---------------------------------------------------------------------------
void PqResult::free() noexcept
// Free the result
{
   if (res) {
      PQclear(res);
      res = nullptr;
   }
}
//---------------------------------------------------------------------------
int PqResult::status() const
// Get the result status
{
   assert(res);
   return PQresultStatus(res);
}
//---------------------------------------------------------------------------
bool PqResult::success() const
// Was the statement successful
{
   return (status() == PGRES_COMMAND_OK) || (status() == PGRES_TUPLES_OK);
}
//---------------------------------------------------------------------------
string PqResult::errorMessage() const
// Get the result error message
{
   assert(res);
   return PQresultErrorMessage(res);
}
//---------------------------------------------------------------------------
int PqResult::ntuples() const
// Get the number of tuples
{
   assert(status() == ExecStatusType::PGRES_TUPLES_OK);
   return PQntuples(res);
}
//---------------------------------------------------------------------------
int PqResult::nfields() const
// Get the number of fields
{
   assert((status() == ExecStatusType::PGRES_TUPLES_OK) || (status() == ExecStatusType::PGRES_COMMAND_OK));
   return PQnfields(res);
}
//---------------------------------------------------------------------------
int PqResult::nparams() const
// Get the number of parameters
{
   assert(status() == ExecStatusType::PGRES_COMMAND_OK);
   return PQnparams(res);
}
//---------------------------------------------------------------------------
int PqResult::paramtype(int column) const
// Get a parameter type
{
   assert(status() == ExecStatusType::PGRES_COMMAND_OK);
   return PQparamtype(res, column);
}
//---------------------------------------------------------------------------
optional<string_view> PqResult::getvalue(int row, int column) const
// Get a field value
{
   assert(status() == ExecStatusType::PGRES_TUPLES_OK);
   if (PQgetisnull(res, row, column))
      return nullopt;

   return string_view{PQgetvalue(res, row, column), static_cast<size_t>(PQgetlength(res, row, column))};
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
