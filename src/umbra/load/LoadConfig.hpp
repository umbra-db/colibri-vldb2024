#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <optional>
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// Configuration for load tests
class LoadConfig {
   private:
   /// The relation (paged or columnar)
   std::string relation;
   /// The backend (buffer or filesystem, only columnar)
   std::optional<std::string> backend;
   /// The block shift (only columnar)
   std::optional<unsigned> blockshift;
   /// Whether to use bulk operations
   bool bulkOperations;

   public:
   /// Read a configuration file
   Status read(const std::string& filename);

   /// Check whether we should use bulk operations
   bool useBulkOperations() const { return bulkOperations; }
   /// Get the relation config
   std::string getRelationConfig() const;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
