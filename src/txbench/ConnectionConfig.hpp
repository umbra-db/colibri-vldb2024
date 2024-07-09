#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include "txbench/io/JsonParser.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Client configuration
class ConnectionConfig {
   protected:
   /// The database host
   std::string host;
   /// The database port
   unsigned port = 0;

   /// The username
   std::string username;
   /// The password
   std::string password;

   /// The pipeline depth
   unsigned pipelineDepth = 0;

   /// Read the values
   virtual Status readValues(JsonParser& parser);

   public:
   /// Destructor
   virtual ~ConnectionConfig() = default;

   /// Read a configuration file
   Status read(const std::string& filename);

   /// Patch the connection
   void patch(const std::string& host, unsigned port);

   /// Get the connection string
   virtual std::string getConnectionString(const std::string& database) const = 0;
   /// Get the pipeline depth
   unsigned getPipelineDepth() const { return pipelineDepth; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
