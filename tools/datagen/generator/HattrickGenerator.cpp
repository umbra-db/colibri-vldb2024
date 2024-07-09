#include "generator/HattrickGenerator.hpp"
#include "txbench/io/CsvWriter.hpp"
#include "txbench/io/ProgressPrinter.hpp"
#include <algorithm>
#include <iomanip>
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
HattrickGenerator::HattrickGenerator(unsigned scaleFactor, const string& basedir)
   : scaleFactor(scaleFactor), basedir(std::move(basedir)), generator(42)
// Constructor
{
   customerCount = 30000 * scaleFactor;
   supplierCount = 2000 * scaleFactor;
   partCount = 200000 * static_cast<int>(floor(1 + log2(scaleFactor)));
   lineorderCount = 1500000 * scaleFactor;

   paymentCounts.resize(customerCount, 0);
   ytds.resize(supplierCount, 0);
}
//---------------------------------------------------------------------------
void HattrickGenerator::generatePart()
// Generate the part table
{
   ProgressPrinter progress("generating parts", partCount);
   CsvWriter writer(basedir + "/part.csv", '!');
   for (unsigned part = 0; part < partCount; ++part) {
      auto c1 = generator.generatePartName();
      auto c2 = generator.generatePartName();
      auto color = generator.generateRand(0, 1) == 0 ? c1 : c2;
      auto mfcr = "MFGR#" + to_string(generator.generateRand(1, 5));
      auto category = mfcr + to_string(generator.generateRand(1, 5));
      auto brand1 = category + to_string(generator.generateRand(1, 40));

      writer << (part + 1); // P_PARTKEY
      writer << c1 + " " + c2; // P_NAME
      writer << mfcr; // P_MFGR
      writer << category; // P_CATEGORY
      writer << brand1; // P_BRAND1
      writer << color; // P_COLOR
      writer << generator.generatePartType(); // P_TYPE
      writer << generator.generateRand(1, 50); // P_SIZE
      writer << generator.generatePartContainer(); // P_CONTAINER
      writer << CsvWriter::setprecision(2) << generator.generatePartPrice(part + 1); // P_PRICE
      writer << CsvWriter::endl;

      progress.update(part + 1);
   }
}
//---------------------------------------------------------------------------
void HattrickGenerator::generateDate()
// Generate the warehouse table
{
   ProgressPrinter progress("generating dates", 1);
   CsvWriter writer(basedir + "/date.csv", '!');

   struct Month {
      string name;
      unsigned days;
   };
   struct Holiday {
      string name;
      vector<string> date;
   };

   static const vector<string> year = {"1992", "1993", "1994", "1995", "1996", "1997", "1998"};
   static const vector<Month> months = {{"January", 31}, {"February", 28}, {"March", 31}, {"April", 30}, {"May", 31}, {"June", 30}, {"July", 31}, {"August", 31}, {"September", 30}, {"October", 31}, {"November", 30}, {"December", 31}};
   static const vector<string> day = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
   static const vector<Holiday> holidays = {{"Christmas", {"2512"}}, {"Thanksgiving", {"26101992", "25101993", "24101994", "23101995", "28101996", "27101997", "26101998"}}, {"Easter", {"19041992", "11041993", "03041994", "16041995", "07041996", "30031997", "12041998"}}, {"Valentine's Day", {"1402"}}, {"Fourth of July", {"0407"}}};

   int weekDay = 2;
   int weekcnt = 1;
   int daycnt = 1;
   bool holidayF = 0;
   int cnt = 0;
   for (int i = 0; i <= 6; i++) {
      for (int j = 0; j < 12; j++) {
         int md = 0;
         if ((year[i].compare("1992") == 0 || year[i].compare("1996") == 0) && months[j].name.compare("February") == 0)
            md = 29;
         else
            md = months[j].days;

         for (int d = 1; d <= md; d++) {
            ostringstream dos;
            dos << setw(2) << setfill('0') << d;
            ostringstream month;
            month << setw(2) << setfill('0') << (j + 1);

            writer << year[i] + month.str() + dos.str(); // D_DATEKEY
            generator.addDate(year[i] + month.str() + dos.str());
            writer << months[j].name + " " + to_string(d) + ", " + year[i]; // D_DATE
            writer << day[weekDay]; // D_DAYOFWEEK
            writer << months[j].name; // D_MONTH
            writer << year[i]; // D_YEAR
            writer << year[i] + month.str(); // D_YEARMONTHNUM
            writer << months[j].name.substr(0, 3) + year[i]; // D_YEARMONTH
            writer << to_string(weekDay + 1); // D_DAYNUMINWEEK
            writer << to_string(d); // D_DAYNUMINMONTH
            writer << to_string(daycnt); // D_DAYNUMINYEAR
            writer << to_string(j + 1); // D_MONTHNUMINYEAR
            writer << to_string(weekcnt); // D_WEEKNUMINYEAR

            for (int k = 0; k < holidays.size(); k++) {
               if ((holidays[k].name == "Thanksgiving") || (holidays[k].name == "Easter")) {
                  for (int l = 0; l < static_cast<int>(holidays[k].date.size()); l++) {
                     if (holidays[k].date[l] == (dos.str() + month.str() + year[i])) {
                        holidayF = 1;
                        writer << holidays[k].name; // D_SELLINGSEASON
                     }
                  }
               } else if (holidays[k].date[0].c_str() == (dos.str() + month.str())) {
                  holidayF = 1;
                  writer << holidays[k].name; // D_SELLINGSEASON
               }
            }

            if (holidayF == 0) writer << string("None"); // D_SELLINGSEASON

            if (weekDay == 6) {
               weekDay = 0;
               weekcnt++;
               writer << to_string(1); // D_LASTDAYINWEEKFL
            } else {
               weekDay++;
               writer << to_string(0); // D_LASTDAYINWEEKFL
            }
            writer << (daycnt == months[j].days ? 1 : 0); // D_LASTDAYINMONTHFL
            writer << (holidayF == 1 ? 1 : 0); // D_HOLIDAYFL
            writer << (weekDay != 5 && weekDay != 6 ? 1 : 0); // D_WEEKDAYFL
            writer << CsvWriter::endl;

            daycnt++;
            holidayF = 0;
            cnt++;
         }
      }
      weekcnt = 0;
   }

   progress.update(1);
}
//---------------------------------------------------------------------------
void HattrickGenerator::generateLineorder()
// Generate the lineorder table
{
   ProgressPrinter progress("generating lineorders and history", lineorderCount);
   CsvWriter lineorder(basedir + "/lineorder.csv", '!');
   CsvWriter history(basedir + "/history.csv", '!');

   for (unsigned lo = 0; lo < lineorderCount; ++lo) {
      int lines = generator.generateRand(1, 7); // number of suborders within the order

      auto custkey = generator.generateRand<uint64_t>(1, customerCount); // choose customer randomly
      paymentCounts[custkey - 1] += 1;

      double amount = 0.0;
      for (unsigned line = 0; line < lines; ++line) {
         lineorder << lo; // LO_ORDERKEY
         lineorder << line; // LO_LINENUMBER
         lineorder << custkey; // LO_CUSTKEY

         auto partkey = generator.generateRand<uint64_t>(1, partCount); // choose part & supplier randomly
         int suppkey = generator.generateRand<uint64_t>(1, supplierCount);
         lineorder << partkey; // LO_PARTKEY
         lineorder << suppkey; // LO_SUPPKEY

         auto dateId = generator.generateRand(0, 2000);
         lineorder << generator.getDate(dateId); // LO_ORDERDATE
         lineorder << generator.generateOrderPriority(); // LO_ORDPRIORITY
         lineorder << generator.generateRand(0, 1); // LO_SHIPPRIORITY

         int quantity = generator.generateRand(1, 50);
         double extendedprice = quantity * generator.getPrice(partkey);
         int discount = generator.generateRand(0, 10);
         double revenue = (extendedprice * (100 - discount)) / 100;
         amount += revenue;
         lineorder << quantity; // LO_QUANTITY
         lineorder << CsvWriter::setprecision(2) << extendedprice; // LO_EXTENDEDPRICE
         lineorder << discount; // LO_DISCOUNT
         lineorder << CsvWriter::setprecision(2) << revenue; // LO_REVENUE
         lineorder << generator.generateReal(1.00, 1000.00); // LO_SUPPLYCOST
         lineorder << generator.generateRand(0, 8); // LO_TAX
         lineorder << generator.getDate(dateId + generator.generateRand(30, 90)); // LO_COMMITDATE
         lineorder << generator.generateShipMode(); // LO_SHIPMODE
         lineorder << CsvWriter::endl;
         ytds[suppkey - 1] = ytds[suppkey - 1] + revenue;
      }

      history << lo; // H_ORDERKEY
      history << custkey; //  H_CUSTKEY
      history << CsvWriter::setprecision(2) << amount; // H_AMOUNT
      history << CsvWriter::endl;

      progress.update(lo + 1);
   }
}
//---------------------------------------------------------------------------
void HattrickGenerator::generateCustomer()
// Generate the customer table
{
   ProgressPrinter progress("generating customers", customerCount);
   CsvWriter writer(basedir + "/customer.csv", '!');
   for (unsigned customer = 0; customer < customerCount; ++customer) {
      writer << (customer + 1); // C_CUSTKEY
      writer << "Customer#" + generator.generateDigitString(customer + 1, 9); // C_NAME
      writer << generator.generateAddress(); // C_ADDRESS

      auto nationId = generator.generateRand(0, 24);
      writer << generator.generateCity(nationId); // C_CITY
      writer << generator.generateNation(nationId); // C_NATION
      writer << generator.generateRegion(nationId); // C_REGION
      writer << generator.generatePhone(nationId); // C_PHONE
      writer << generator.generateCustomerMktSegment(); // C_MKTSEGMENT
      writer << paymentCounts[customer]; // C_PAYMENTCNT
      writer << CsvWriter::endl;

      progress.update(customer + 1);
   }
}
//---------------------------------------------------------------------------
void HattrickGenerator::generateSupplier()
// Generate the customer table
{
   ProgressPrinter progress("generating suppliers", supplierCount);
   CsvWriter writer(basedir + "/supplier.csv", '!');
   for (unsigned supplier = 0; supplier < supplierCount; ++supplier) {
      writer << (supplier + 1); // S_SUPPKEY
      writer << "Supplier#" + generator.generateDigitString(supplier + 1, 9); // S_NAME
      writer << generator.generateAddress(); // S_ADDRESS

      auto nationId = generator.generateRand(0, 24);
      writer << generator.generateCity(nationId); // S_CITY
      writer << generator.generateNation(nationId); // S_NATION
      writer << generator.generateRegion(nationId); // S_REGION
      writer << generator.generatePhone(nationId); // S_PHONE
      writer << CsvWriter::setprecision(2) << ytds[supplier]; // S_YTD
      writer << CsvWriter::endl;

      progress.update(supplier + 1);
   }
}
//---------------------------------------------------------------------------
void HattrickGenerator::generate()
// Generate data
{
   generatePart();
   generateDate();
   generateLineorder();
   generateCustomer();
   generateSupplier();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
