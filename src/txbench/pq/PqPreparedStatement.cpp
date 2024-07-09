#include "txbench/pq/PqPreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
PqPreparedStatement::PqPreparedStatement()
// Constructor
{
}
//---------------------------------------------------------------------------
PqPreparedStatement::PqPreparedStatement(string name, int numParams)
   : name(std::move(name)), numParams(numParams)
// Constructor
{
   paramValues.resize(numParams);
   paramLengths.resize(numParams);
   paramFormats.resize(numParams);
}
//---------------------------------------------------------------------------
PqPreparedStatement::~PqPreparedStatement()
// Destructor
{
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
