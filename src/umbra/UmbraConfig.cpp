#include "umbra/UmbraConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
Status UmbraConfig::readValues(JsonParser& parser)
// Read the values
{
   parser.map("relation", relation);
   parser.map("backend", backend);
   parser.map("blockshift", blockshift);

   parser.map("bucket", bucket);
   parser.map("access_key_id", accessKeyId);
   parser.map("access_key", accessKey);

   if (relation != "paged" && relation != "columnar")
      return Status::Error("Relation '" + relation + "' not supported!");

   if (relation == "columnar") {
      if (!backend.has_value())
         return Status::Error("Backend required for columnar relation!");
      if (backend != "buffer" && backend != "filesystem" && backend != "cloud")
         return Status::Error("Backend '" + backend.value() + "' not supported by columnar relation!");

      if (!blockshift.has_value())
         return Status::Error("Block shift required for columnar relation!");

      if (backend == "cloud") {
         if (!bucket.has_value())
            return Status::Error("Bucket required for columnar relation on cloud backend!");
         if (!accessKeyId.has_value())
            return Status::Error("Access Key Id required for columnar relation on cloud backend!");
         if (!accessKey.has_value())
            return Status::Error("Access Key required for columnar relation on cloud backend!");
      }
   }

   return DriverConfig::readValues(parser);
}
//---------------------------------------------------------------------------
std::string UmbraConfig::getRelationConfig() const
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
