#include "singlestore/SingleStoreConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
string SingleStoreConnectionConfig::getConnectionString(const std::string& database) const
// Get the connection string
{
   return OdbcConnectionConfig::getConnectionString(database) + ";OPTION=" + to_string(67108864 + 1048576);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
