#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include "txbench/io/JsonParser.hpp"
#include <string>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Client configuration
class BenchmarkConfig {
   /// The database
   std::string database;

   /// The init scripts (run on the master database)
   std::vector<std::string> initScripts;
   /// The schema scripts (run on the benchmark database)
   std::vector<std::string> schemaScripts;
   /// The load scripts (run on the benchmark database)
   std::vector<std::string> loadScripts;
   /// The function scripts (run on the benchmark database)
   std::vector<std::string> functionScripts;

   protected:
   /// Read the values
   virtual Status readValues(JsonParser& parser);

   public:
   /// Destructor
   virtual ~BenchmarkConfig() = default;

   /// Read a configuration file
   Status read(const std::string& filename);

   /// Get the database
   const std::string& getDatabase() const { return database; }

   /// Get the init scripts
   const std::vector<std::string>& getInitScripts() const { return initScripts; }
   /// Get the schema scripts
   const std::vector<std::string>& getSchemaScripts() const { return schemaScripts; }
   /// Get the load scripts
   const std::vector<std::string>& getLoadScripts() const { return loadScripts; }
   /// Get the load scripts
   const std::vector<std::string>& getFunctionScripts() const { return functionScripts; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
