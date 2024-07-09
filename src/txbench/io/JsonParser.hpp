#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Helper class for JSON parsing
class JsonParser {
   private:
   /// The implementation
   struct Impl;
   /// The implementation
   std::unique_ptr<Impl> impl;

   /// Record an error
   void markFailed();

   public:
   /// Constructor
   JsonParser();
   /// Destructor
   ~JsonParser();

   /// Parse a JSON file
   Status parse(const std::string& filename);

   /// Map a boolean
   void map(const std::string& key, bool& target);
   /// Map a string
   void map(const std::string& key, std::string& target);
   /// Map a string
   void map(const std::string& key, std::optional<std::string>& target);
   /// Map an integer
   void map(const std::string& key, unsigned& target);
   /// Map an integer
   void map(const std::string& key, std::optional<unsigned>& target);
   /// Map an array of strings
   void map(const std::string& key, std::vector<std::string>& target);

   /// Check whether any errors occurred during mapping
   bool hasError() const;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
