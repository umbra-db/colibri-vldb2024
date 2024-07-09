#include "txbench/ConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
Status ConnectionConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("host", host);
   parser.map("port", port);
   parser.map("username", username);
   parser.map("password", password);
   parser.map("pipeline_depth", pipelineDepth);

   return Status::Success;
}
//---------------------------------------------------------------------------
Status ConnectionConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   if (auto res = readValues(parser); !res)
      return res;

   if (parser.hasError())
      return Status::Error("unable to parse connection config");

   return Status::Success;
}
//---------------------------------------------------------------------------
void ConnectionConfig::patch(const std::string& host, unsigned port)
// Patch the connection
{
   this->host = host;
   this->port = port;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
