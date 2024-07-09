#pragma once
//---------------------------------------------------------------------------
#include <string>
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Generic result status type for operations that can fail with an error message
class Status {
   public:
   /// Tag type for constructing success results
   static constexpr struct SuccessTag {
   } Success{};

   private:
   /// Whether the operation was successful
   bool success;
   /// The error message (if any)
   std::string message;

   /// Constructor
   explicit Status(std::string message) : success(false), message(std::move(message)) {}

   public:
   /// Constructor
   Status(SuccessTag) : success(true) {} // NOLINT: Intentional implicit conversion
   /// Constructor for unsuccessful results
   static Status Error(const char* message) { return Status(message); }
   /// Constructor for unsuccessful results
   static Status Error(std::string message) { return Status(std::move(message)); }
   /// Constructor for unsuccessful results
   static Status Error(std::string_view message) { return Status(std::string(message)); }

   /// Conversion to bool
   constexpr operator bool() const { return success; } // NOLINT: Intentional implicit conversion
   /// Conversion to bool
   constexpr bool operator!() const { return !success; }

   /// Get the error message (if any)
   [[nodiscard]] const std::string& getMessage() const { return message; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
