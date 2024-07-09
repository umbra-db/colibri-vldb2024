#include "generator/HattrickGenerator.hpp"
#include "generator/HyBenchGenerator.hpp"
#include "generator/TatpGenerator.hpp"
#include "generator/TpccGenerator.hpp"
#include "generator/TpcchGenerator.hpp"
#include <charconv>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
static int main(int argc, const char* argv[])
// Entry point
{
   if (argc != 4) {
      cerr << "USAGE: " << argv[0] << " (TPCC|TPCCH|TATP|HATTRICK|HYBENCH) SCALE_FACTOR DIRECTORY" << endl;
      return 1;
   }

   string_view scaleFactorString = argv[2];
   unsigned scaleFactor;
   auto [ptr, ec] = from_chars(scaleFactorString.data(), scaleFactorString.data() + scaleFactorString.size(), scaleFactor);
   if ((ec != std::errc()) || (ptr != scaleFactorString.data() + scaleFactorString.size())) {
      cerr << "unable to parse scale factor \"" << scaleFactorString << "\"" << endl;
      return 1;
   }

   string benchmark = argv[1];
   string basedir = argv[3];
   if (benchmark == "TPCC") {
      TpccGenerator(scaleFactor, basedir).generate();
   } else if (benchmark == "TPCCH") {
      TpcchGenerator(scaleFactor, basedir).generate();
   } else if (benchmark == "TATP") {
      TatpGenerator(scaleFactor, basedir).generate();
   } else if (benchmark == "HATTRICK") {
      HattrickGenerator(scaleFactor, basedir).generate();
   } else if (benchmark == "HYBENCH") {
      HyBenchGenerator(scaleFactor, basedir).generate();
   } else {
      cerr << "unknown benchmark \"" << benchmark << "\"";
      return 1;
   }

   return 0;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
int main(int argc, const char* argv[])
// Entry point
{
   return txbench::datagen::main(argc, argv);
}
//---------------------------------------------------------------------------
