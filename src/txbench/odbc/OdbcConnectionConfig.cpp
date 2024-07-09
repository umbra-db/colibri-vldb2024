#include "txbench/odbc/OdbcConnectionConfig.hpp"
#include "txbench/io/JsonParser.hpp"
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
Status OdbcConnectionConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("driver", driver);
   parser.map("isolation_level", isolationLevel);

   return ConnectionConfig::readValues(parser);
}
//---------------------------------------------------------------------------
string OdbcConnectionConfig::getConnectionString(const std::string& database) const
// Get the connection string
{
   // "DRIVER={ODBC Driver 18 for SQL Server};SERVER=localhost,21433;UID=SA;TrustServerCertificate=yes;PWD=" + password;

   ostringstream result;
   result << "DRIVER={" << driver << "};";
   result << "Server=" << host << ";";
   result << "Port=" << port << ";";
   result << "TrustServerCertificate=yes;";
   if (!database.empty()) result << "Database=" << database << ";";
   result << "UID=" << username << ";";
   result << "PWD=" << password;

   return result.str();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
