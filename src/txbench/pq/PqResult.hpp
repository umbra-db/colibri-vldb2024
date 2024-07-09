#pragma once
//---------------------------------------------------------------------------
#include <optional>
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
struct pg_result;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// A result object
class PqResult {
   /// The result object
   pg_result* res = nullptr;

   /// Constructor
   explicit PqResult(pg_result* res);

   /// Get the PqResult status
   int status() const;
   /// Was the statement successful
   bool success() const;
   /// Get the result error message
   std::string errorMessage() const;

   friend class PqConnection;

   public:
   /// Constructor
   PqResult();
   /// Copy constructor
   PqResult(const PqResult&) = delete;
   /// Move constructor
   PqResult(PqResult&& other) noexcept;
   /// Destructor
   ~PqResult();

   /// Copy assignment
   PqResult& operator=(const PqResult&) = delete;
   /// Move assignment
   PqResult& operator=(PqResult&& other) noexcept;

   /// Free the result object
   void free() noexcept;

   /// Get the number of rows
   int ntuples() const;
   /// Get the number of fields
   int nfields() const;
   /// Get the number of parameters
   int nparams() const;
   /// Get a parameter type
   int paramtype(int column) const;

   /// Get a field value. Returns nullopt if the field is null
   std::optional<std::string_view> getvalue(int row, int column) const;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
