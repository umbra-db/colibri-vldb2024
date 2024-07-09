#pragma once
//---------------------------------------------------------------------------
#include "txbench/pq/PqConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// Connection configuration
class UmbraConnectionConfig : public pq::PqConnectionConfig {
   /// Whether asynchronous commits are enabled
   std::string asyncCommit;
   /// The call error handling
   std::string callErrorHandling;

   protected:
   /// Read the values
   Status readValues(JsonParser& parser) override;

   public:
   /// Check whether asynchronous commits are enabled
   const std::string& getAsyncCommit() const { return asyncCommit; }
   /// Get the call error handling
   const std::string& getCallErrorHandling() const { return callErrorHandling; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
