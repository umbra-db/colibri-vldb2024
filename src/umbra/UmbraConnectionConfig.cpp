#include "umbra/UmbraConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Status UmbraConnectionConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("async_commit", asyncCommit);
   parser.map("call_error_handling", callErrorHandling);

   return PqConnectionConfig::readValues(parser);
   auto validate = [&]() {
      if (parser.hasError())
         return false;
      if ((asyncCommit != "on") && (asyncCommit != "off"))
         return false;
      if ((callErrorHandling != "raise") && (callErrorHandling != "report") && (callErrorHandling != "swallow"))
         return false;

      return true;
   };

   return validate() ? Status::Success : Status::Error("unable to parse client config");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
