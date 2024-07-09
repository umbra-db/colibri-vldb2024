#include "generator/TatpGenerator.hpp"
#include "txbench/io/CsvWriter.hpp"
#include "txbench/io/ProgressPrinter.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
TatpGenerator::TatpGenerator(unsigned scaleFactor, const string& basedir)
   : scaleFactor(scaleFactor), basedir(basedir), generator(42)
// Constructor
{
}
//---------------------------------------------------------------------------
void TatpGenerator::generateSubscriber()
// Generate subscriber data
{
   ProgressPrinter progress("generating subscribers", scaleFactor);
   CsvWriter writer(basedir + "/subscriber.csv");

   for (unsigned subscriber = 0; subscriber < scaleFactor; ++subscriber) {
      writer << (subscriber + 1); // s_id
      writer << generator.generateDigitString(subscriber + 1, 15); // s_nbr

      for (unsigned bit = 0; bit < 10; ++bit)
         writer << generator.generateRand(0, 1); // bit_X
      for (unsigned hex = 0; hex < 10; ++hex)
         writer << generator.generateRand(0, 15); // hex_X
      for (unsigned byte = 0; byte < 10; ++byte)
         writer << generator.generateRand(0, 255); // byte2_X

      writer << generator.generateRand(1, numeric_limits<int>::max()); // msc_location
      writer << generator.generateRand(1, numeric_limits<int>::max()); // vlr_location
      writer << CsvWriter::endl;

      progress.update(subscriber + 1);
   }
}
//---------------------------------------------------------------------------
void TatpGenerator::generateAccessInfo()
// Generate access info data
{
   ProgressPrinter progress("generating access info", scaleFactor);
   CsvWriter writer(basedir + "/access_info.csv");

   auto accessInfoTypeGenerator = generator.getSampleGenerator({1, 2, 3, 4});

   for (unsigned subscriber = 0; subscriber < scaleFactor; ++subscriber) {
      unsigned accessInfoCount = generator.generateRand(1, 4);
      auto accessInfoTypes = accessInfoTypeGenerator.sample(accessInfoCount);
      for (unsigned accessInfo = 0; accessInfo < accessInfoCount; ++accessInfo) {
         writer << (subscriber + 1); // s_id
         writer << accessInfoTypes[accessInfo]; // ai_type
         writer << generator.generateRand(0, 255); // data1
         writer << generator.generateRand(0, 255); // data2
         writer << generator.generateAlphaString(3); // data3
         writer << generator.generateAlphaString(5); // data4
         writer << CsvWriter::endl;
      }

      progress.update(subscriber + 1);
   }
}
//---------------------------------------------------------------------------
void TatpGenerator::generateSpecialFacility()
// Generate special facility data
{
   ProgressPrinter progress("generating special facilities", scaleFactor);

   CsvWriter specialFacilityWriter(basedir + "/special_facility.csv");
   CsvWriter callForwardingWriter(basedir + "/call_forwarding.csv");

   auto specialFacilityTypeGenerator = generator.getSampleGenerator({1, 2, 3, 4});
   auto callForwardingStartTimeGenerator = generator.getSampleGenerator({0, 8, 16});

   for (unsigned subscriber = 0; subscriber < scaleFactor; ++subscriber) {
      unsigned specialFacilityCount = generator.generateRand(1, 4);
      auto specialFacilityTypes = specialFacilityTypeGenerator.sample(specialFacilityCount);
      for (unsigned specialFacility = 0; specialFacility < specialFacilityCount; ++specialFacility) {
         specialFacilityWriter << (subscriber + 1); // s_id
         specialFacilityWriter << specialFacilityTypes[specialFacility]; // sf_type

         if (generator.generateRand(1, 100) <= 85)
            specialFacilityWriter << 1; // is_active
         else
            specialFacilityWriter << 0; // is_active

         specialFacilityWriter << generator.generateRand(0, 255); // error_cntrl
         specialFacilityWriter << generator.generateRand(0, 255); // data_a
         specialFacilityWriter << generator.generateAlphaString(5); // data_b
         specialFacilityWriter << CsvWriter::endl;

         unsigned callForwardingCount = generator.generateRand(0, 3);
         auto callForwardingStartTimes = callForwardingStartTimeGenerator.sample(callForwardingCount);
         for (unsigned callForwarding = 0; callForwarding < callForwardingCount; ++callForwarding) {
            callForwardingWriter << (subscriber + 1); // s_id
            callForwardingWriter << specialFacilityTypes[specialFacility]; // sf_type
            callForwardingWriter << callForwardingStartTimes[callForwarding]; // start_time
            callForwardingWriter << callForwardingStartTimes[callForwarding] + generator.generateRand(1, 8); // end_time
            callForwardingWriter << generator.generateDigitString(generator.generateRand<uint64_t>(0, 999'999'999'999'999), 15); // numberx
            callForwardingWriter << CsvWriter::endl;
         }
      }

      progress.update(subscriber + 1);
   }
}
//---------------------------------------------------------------------------
void TatpGenerator::generate()
// Generate data
{
   generateSubscriber();
   generateAccessInfo();
   generateSpecialFacility();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
