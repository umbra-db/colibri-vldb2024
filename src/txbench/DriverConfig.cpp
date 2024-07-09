#include "txbench/DriverConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
Status DriverConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("warmup_time", warmupTime);
   parser.map("measure_time", measureTime);

   return Status::Success;
}
//---------------------------------------------------------------------------
Status DriverConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   if (auto res = readValues(parser); !res)
      return res;

   if (parser.hasError())
      return Status::Error("unable to parse driver config");

   return Status::Success;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
