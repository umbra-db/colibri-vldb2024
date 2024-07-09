#pragma once
//---------------------------------------------------------------------------
#include <string>
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
#define RETHROW(command, message)  \
   do {                            \
      try {                        \
         command;                  \
      } catch (Exception & e) {    \
         throw Exception(message); \
      }                            \
   } while (false)
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// A generic exception
class Exception : public std::exception {
   /// The message that caused the exception
   std::string message;

   public:
   /// Constructor
   explicit Exception(std::string message) : message(std::move(message)) {}
   /// Constructor
   explicit Exception(std::string_view message) : message(message) {}
   /// Constructor
   explicit Exception(const char* message) : message(message) {}
   /// Constructor
   Exception(const Exception&) = default;
   /// Constructor
   Exception(Exception&&) noexcept = default;
   /// Destructor
   ~Exception() = default;

   /// Assignment
   Exception& operator=(const Exception&) = default;
   /// Assignment
   Exception& operator=(Exception&&) = default;

   /// Get the message
   const std::string& getMessage() const { return message; }

   /// Get the message using the standard exception interface
   const char* what() const noexcept override { return message.c_str(); };
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
