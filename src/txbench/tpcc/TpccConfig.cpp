#include "txbench/tpcc/TpccConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
Status TpccConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("generate_remote_accesses", generateRemoteAccesses);
   parser.map("generate_user_rollbacks", generateUserRollbacks);

   return BenchmarkConfig::readValues(parser);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
