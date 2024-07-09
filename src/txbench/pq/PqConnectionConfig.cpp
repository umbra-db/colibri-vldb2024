#include "txbench/pq/PqConnectionConfig.hpp"
#include "txbench/io/JsonParser.hpp"
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
string PqConnectionConfig::getConnectionString(const std::string& /*database*/) const
// Get the connection string
{
   ostringstream result;
   if (!host.empty())
      result << "host=" << host << " ";
   if (port != 0)
      result << "port=" << port << " ";
   if (!username.empty())
      result << "user=" << username << " ";
   if (!password.empty())
      result << "password=" << password << " ";

   return result.str();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
