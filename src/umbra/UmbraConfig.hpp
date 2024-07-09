#pragma once
//---------------------------------------------------------------------------
#include "txbench/DriverConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// Umbra driver configuration
class UmbraConfig : public DriverConfig {
   protected:
   /// The relation (paged or columnar)
   std::string relation;
   /// The backend (buffer, filesystem, or remote, only columnar)
   std::optional<std::string> backend;
   /// The block shift (only columnar)
   std::optional<unsigned> blockshift;

   /// The bucket (only columnar relation with cloud backend)
   std::optional<std::string> bucket;
   /// The access key id
   std::optional<std::string> accessKeyId;
   /// The access key
   std::optional<std::string> accessKey;

   /// Read the values
   Status readValues(JsonParser& parser) override;

   public:
   /// Get the relation config
   std::string getRelationConfig() const;

   /// Is cloud backend?
   bool cloud() const { return relation == "columnar" && backend.value() == "cloud"; }
   /// Get the bucket
   std::string getBucket() const { return bucket.value(); }
   /// Get the access key id
   std::string getAccessKeyId() const { return accessKeyId.value(); }
   /// Get the access key
   std::string getAccessKey() const { return accessKey.value(); }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
