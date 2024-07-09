#include "generator/TpcchGenerator.hpp"
#include "txbench/io/CsvWriter.hpp"
#include "txbench/io/ProgressPrinter.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
TpcchGenerator::TpcchGenerator(unsigned scaleFactor, const string& basedir)
   : TpccGenerator(scaleFactor, basedir), generator(42)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpcchGenerator::generateRegion()
// Generate the region table
{
   ProgressPrinter progress("generating regions", tpcch::TpcchGenerator::regionCount);
   CsvWriter writer(basedir + "/region.csv", '|');
   for (unsigned region = 0; region < tpcch::TpcchGenerator::regionCount; ++region) {
      writer << region; // R_ID
      writer << generator.generateRegionName(region); // R_Name
      writer << generator.generateText(31, 115); // R_Comment
      writer << CsvWriter::endl;

      progress.update(region + 1);
   }
}
//---------------------------------------------------------------------------
void TpcchGenerator::generateNation()
// Generate the nation table
{
   ProgressPrinter progress("generating nations", tpcch::TpcchGenerator::nationCount);
   CsvWriter writer(basedir + "/nation.csv", '|');
   for (unsigned nation = 0; nation < tpcch::TpcchGenerator::nationCount; ++nation) {
      auto n = generator.generateNation(nation);

      writer << n.id; // N_ID
      writer << n.name; // N_Name
      writer << n.regionId; // N_RegionID
      writer << generator.generateText(31, 114); // R_Comment
      writer << CsvWriter::endl;

      progress.update(nation + 1);
   }
}
//---------------------------------------------------------------------------
void TpcchGenerator::generateSupplier()
// Generate the supplier table
{
   ProgressPrinter progress("generating suppliers", tpcch::TpcchGenerator::supplierCount);
   CsvWriter writer(basedir + "/supplier.csv", '|');
   for (unsigned supplier = 0; supplier < tpcch::TpcchGenerator::supplierCount; ++supplier) {
      writer << supplier; // SU_ID
      writer << "Supplier#" + generator.generateDigitString(supplier, 9); // SU_Name
      writer << generator.generateAlphanumeric64(10, 40); // SU_ADDRESS
      writer << generator.generateNationKey(); // SU_NATIONKEY
      writer << generator.generateSupplierPhone(supplier); // SU_PHONE
      writer << generator.generateDouble(-999.99, 9999.99, 2); // SU_ACCTBAL
      if ((supplier + 7) % 1893 == 0) // SU_COMMENT: 5 rows "Customer%Recommends" + 5 rows "Customer%Complaints"
         writer << generator.generateSupplierComment(25, 100, "Complaints");
      else if ((supplier + 13) % 1983 == 0) //no spec
         writer << generator.generateSupplierComment(25, 100, "Recommends");
      else
         writer << generator.generateText(25, 100);
      writer << CsvWriter::endl;

      progress.update(supplier + 1);
   }
}
//---------------------------------------------------------------------------
void TpcchGenerator::generate()
// Generate data
{
   TpccGenerator::generate();
   generateRegion();
   generateNation();
   generateSupplier();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
