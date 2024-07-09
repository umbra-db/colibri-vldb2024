#include "txbench/io/CsvWriter.hpp"
#include <filesystem>
#include <iomanip>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
CsvWriter::CsvWriter(const string& filename, char seperator)
   : seperator(seperator), firstInLine(true)
// Constructor
{
   create_directories(filesystem::path(filename).parent_path());
   stream.open(filename);
}
//---------------------------------------------------------------------------
CsvWriter::~CsvWriter()
// Destructor
{
   // Make sure we get a trailing newline
   if (!firstInLine)
      stream << '\n';
}
//---------------------------------------------------------------------------
CsvWriter& CsvWriter::operator<<(const Endline&)
// Output
{
   stream << '\n';
   firstInLine = true;

   return *this;
}
//---------------------------------------------------------------------------
CsvWriter& CsvWriter::operator<<(const Setprecision& setprecision)
// Output
{
   stream << ::setprecision(setprecision.precision) << fixed;

   return *this;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
