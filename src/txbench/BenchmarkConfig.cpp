#include "txbench/BenchmarkConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
Status BenchmarkConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("database", database);
   parser.map("init_scripts", initScripts);
   parser.map("schema_scripts", schemaScripts);
   parser.map("load_scripts", loadScripts);
   parser.map("function_scripts", functionScripts);

   return Status::Success;
}
//---------------------------------------------------------------------------
Status BenchmarkConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   if (auto res = readValues(parser); !res)
      return res;

   if (parser.hasError())
      return Status::Error("unable to parse client config");

   return Status::Success;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
