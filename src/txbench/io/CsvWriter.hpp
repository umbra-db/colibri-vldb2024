#pragma once
//---------------------------------------------------------------------------
#include <fstream>
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
class CsvWriter {
   /// The output stream
   std::ofstream stream;
   /// The seperator
   char seperator;
   /// Whether we are writing the first column in a line
   bool firstInLine;

   public:
   /// Helper for setting the precision
   struct Setprecision {
      /// The precision
      int precision;
   };

   /// Helper for line ends
   static constexpr struct Endline {
   } endl = {};

   /// Return a Setprecision struct
   static constexpr Setprecision setprecision(int precision) { return {precision}; }

   /// Constructor
   explicit CsvWriter(const std::string& filename, char seperator = ',');
   /// Destructor
   ~CsvWriter();

   /// Output
   template <typename T>
   CsvWriter& operator<<(const T& val);
   /// Output
   CsvWriter& operator<<(const Endline&);
   /// Output
   CsvWriter& operator<<(const Setprecision& setprecision);
};
//---------------------------------------------------------------------------
template <typename T>
CsvWriter& CsvWriter::operator<<(const T& val)
// Output
{
   if (firstInLine)
      firstInLine = false;
   else
      stream << seperator;

   stream << val;

   return *this;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
