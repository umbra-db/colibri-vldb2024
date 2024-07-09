#include "umbra/UmbraDriver.hpp"
#include "txbench/Exception.hpp"
#include "umbra/Umbra.hpp"
#include "umbra/UmbraClient.hpp"
#include "umbra/UmbraConfig.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
UmbraDriver::UmbraDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DatabaseDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
UmbraDriver::~UmbraDriver()
// Destructor
{
}
//---------------------------------------------------------------------------
void UmbraDriver::createSchema()
// Create the schema
{
   auto& umbraConfig = static_cast<const UmbraConfig&>(*driverConfig);
   auto relationConfig = umbraConfig.getRelationConfig();

   cerr << "setting up schema... (" << relationConfig << ")" << endl;
   for (const auto& filename : database.getBenchmarkConfig().getSchemaScripts()) {
      string script;
      RETHROW(database.readScript(filename, script), "unable to read schema script \"" + filename + "\": " + e.getMessage());

      auto replace = [&](const string_view from, const std::string& to) {
         for (auto pos = script.find(from); pos != string::npos; pos = script.find(from, pos)) {
            script.replace(pos, from.size(), to);
            pos += from.size();
         }
      };

      // Patch in scale factor-dependent bucket counts
      replace("%%relationConfig%%", relationConfig);

      RETHROW(database.executeQuery(script), "unable to execute schema script \"" + filename + "\": " + e.getMessage());
   }

   if (umbraConfig.cloud()) {
      cerr << "installing remote storage (bucket: '" << umbraConfig.getBucket() << "', accessKeyId: '" << umbraConfig.getAccessKeyId() << "', accessKey: '" << umbraConfig.getAccessKey() << "')" << endl;
      string stmt = "create remote storage s3 using '" + umbraConfig.getBucket() + "' with secret '" + umbraConfig.getAccessKeyId() + "' '" + umbraConfig.getAccessKey() + "';";
      RETHROW(database.executeQuery(stmt), "unable to install remote storage \"" + stmt + "\": " + e.getMessage());
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
