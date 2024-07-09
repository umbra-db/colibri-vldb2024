#include "umbra/load/LoadConfig.hpp"
#include "txbench/io/JsonParser.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Status LoadConfig::read(const string& filename)
// Read a configuration file
{
   JsonParser parser;
   if (auto res = parser.parse(filename); !res)
      return res;

   parser.map("relation", relation);
   parser.map("backend", backend);
   parser.map("blockshift", blockshift);
   parser.map("bulk_operations", bulkOperations);

   if (relation != "paged" && relation != "columnar")
      return Status::Error("Relation '" + relation + "' not supported!");

   if (relation == "columnar") {
      if (!backend.has_value())
         return Status::Error("Backend required for columnar relation!");
      if (backend != "buffer" && backend != "filesystem")
         return Status::Error("Backend '" + backend.value() + "' not supported by columnar relation!");

      if (!blockshift.has_value())
         return Status::Error("Block shift required for columnar relation!");
   }

   if (parser.hasError())
      return Status::Error("unable to parse loadtest config");

   return Status::Success;
}
//---------------------------------------------------------------------------
std::string LoadConfig::getRelationConfig() const
// Get the relation config
{
   if (relation == "paged") {
      return "storage=paged";
   } else {
      return "storage=columnar,backend=" + backend.value() + ",blocksize=" + to_string(1ull << blockshift.value());
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
