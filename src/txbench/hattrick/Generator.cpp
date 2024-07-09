#include "txbench/hattrick/Generator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::hattrick {
//---------------------------------------------------------------------------
Generator::Generator(uint64_t seed)
   : Random(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
std::string Generator::generatePartName()
// Generate a part name
{
   static const vector<string> partName = {
      "almond", "antique", "aquamarine", "azure", "beige", "bisque", "black", "blanched", "blue", "blush",
      "brown", "burlywood", "burnished", "chartreuse", "chiffon", "chocolate", "coral", "cornflower",
      "cornsilk", "cream", "cyan", "dark", "deep", "dim", "dodger", "drab", "firebrick", "floral", "forest",
      "frosted", "gainsboro", "ghost", "goldenrod", "green", "grey", "honeydew", "hot", "indian", "ivory",
      "khaki", "lace", "lavender", "lawn", "lemon", "light", "lime", "linen", "magenta", "maroon", "medium",
      "metallic", "midnight", "mint", "misty", "moccasin", "navajo", "navy", "olive", "orange", "orchid",
      "pale", "papaya", "peach", "peru", "pink", "plum", "powder", "puff", "purple", "red", "rose", "rosy",
      "royal", "saddle", "salmon", "sandy", "seashell", "sienna", "sky", "slate", "smoke", "snow", "spring",
      "steel", "tan", "thistle", "tomato", "turquoise", "violet", "wheat", "white", "yellow"}; // #92
   static uniform_int_distribution<size_t> partNameDist(0, partName.size() - 1);

   return partName[partNameDist(rng)];
}
//---------------------------------------------------------------------------
std::string Generator::generatePartType()
// Generate a part type
{
   static const vector<string> partType = {
      "STANDARD", "SMALL", "MEDIUM", "LARGE", "ECONOMY", "PROMO", "ANODIZED", "BURNISHED",
      "PLATED", "POLISHED", "BRUSHED", "TIN", "NICKEL", "BRASS", "STEEL", "COPPER"}; // S1(0-5), S2(6-10), S3(11-15)
   static uniform_int_distribution<size_t> partTypeDist1(0, 5);
   static uniform_int_distribution<size_t> partTypeDist2(6, 10);
   static uniform_int_distribution<size_t> partTypeDist3(11, 15);

   return partType[partTypeDist1(rng)] + " " + partType[partTypeDist2(rng)] + " " + partType[partTypeDist3(rng)];
}
//---------------------------------------------------------------------------
std::string Generator::generatePartContainer()
// Generate a part container
{
   static const vector<string> partContainer = {
      "SM", "LG", "MED", "JUMBO", "WRAP", "CASE", "BOX", "BAG", "JAR",
      "PKG", "PACK", "CAN", "DRUM"}; // S1(0-4), S2(5-12)
   static uniform_int_distribution<size_t> partContainerDist1(0, 4);
   static uniform_int_distribution<size_t> partContainerDist2(5, 12);

   return partContainer[partContainerDist1(rng)] + " " + partContainer[partContainerDist2(rng)];
}
//---------------------------------------------------------------------------
double Generator::generatePartPrice(unsigned part)
// Generate a part price
{
   double p = (90000 + fmod((double) part / 10, 20001) + 100 * fmod(part, 1000)) / 100;
   prices.push_back(p);
   return p;
}
//---------------------------------------------------------------------------
std::string Generator::generateOrderPriority()
// Generate an order priority
{
   static const vector<string> orderPriorities = {"1-URGENT", "2-HIGH", "3-MEDIUM", "4-NOT SPECIFIED", "5-LOW"};
   static uniform_int_distribution<size_t> dist(0, orderPriorities.size() - 1);

   return orderPriorities[dist(rng)];
}
//---------------------------------------------------------------------------
std::string Generator::generateShipMode()
// Generate a ship mode
{
   static const vector<string> shipModes = {"REG AIR", "AIR", "RAIL", "SHIP", "TRUCK", "MAIL", "FOB"};
   static uniform_int_distribution<size_t> dist(0, shipModes.size() - 1);

   return shipModes[dist(rng)];
}
//---------------------------------------------------------------------------
std::string Generator::generateAddress()
// Generate an address
{
   auto length = generateRand(10, 25);
   string address = "";
   for (int i = 0; i < length; i++) {
      auto c = static_cast<char>(generateRand(48, 122));
      while (c == '\\')
         c = static_cast<char>(generateRand(48, 122));

      address += c;
   }
   
   return address;
}
//---------------------------------------------------------------------------
std::string Generator::generateCity(unsigned int nationId)
// Generate a city
{
   static const vector<string> nations = {
      "ALGERIA", "ARGENTINA", "BRAZIL", "CANADA", "EGYPT", "ETHIOPIA",
      "FRANCE", "GERMANY", "INDIA", "INDONESIA", "IRAN", "IRAQ", "JAPAN",
      "JORDAN", "KENYA", "MOROCCO", "MOZAMBIQUE", "PERU", "CHINA", "ROMANIA",
      "SAUDI ARABIA", "VIETNAM", "RUSSIA", "UNITED KINGDOM", "UNITED STATES"};

   string city = nations[nationId];
   int length = city.length();
   if (length >= 9)
      return city.substr(0, 9);
   else
      return city.append(9 - length, ' ');
}
//---------------------------------------------------------------------------
std::string Generator::generateNation(unsigned int nationId)
// Generate a nation
{
   static const vector<string> nations = {
      "ALGERIA", "ARGENTINA", "BRAZIL", "CANADA", "EGYPT", "ETHIOPIA",
      "FRANCE", "GERMANY", "INDIA", "INDONESIA", "IRAN", "IRAQ", "JAPAN",
      "JORDAN", "KENYA", "MOROCCO", "MOZAMBIQUE", "PERU", "CHINA", "ROMANIA",
      "SAUDI ARABIA", "VIETNAM", "RUSSIA", "UNITED KINGDOM", "UNITED STATES"};

   return nations[nationId];
}
//---------------------------------------------------------------------------
std::string Generator::generateRegion(unsigned int nationId)
// Generate a region
{
   static const vector<string> regions = {
      "AFRICA", "AMERICA", "AMERICA", "AMERICA", "AFRICA", "AFRICA",
      "EUROPE", "EUROPE", "ASIA", "ASIA", "MIDDLE EAST", "MIDDLE EAST", "ASIA",
      "AFRICA", "AFRICA", "AFRICA", "AFRICA", "AMERICA", "ASIA", "EUROPE",
      "MIDDLE EAST", "ASIA", "EUROPE", "EUROPE", "AMERICA"};

   return regions[nationId];
}
//---------------------------------------------------------------------------
std::string Generator::generatePhone(unsigned int nationId)
// Generate a phone
{
   int countryCode = nationId + 10;
   return to_string(countryCode) + '-' + to_string(generateRand(100, 999)) + '-' + to_string(generateRand(100, 999)) + '-' + to_string(generateRand(1000, 9999));
}
//---------------------------------------------------------------------------
std::string Generator::generateCustomerMktSegment()
// Generate a customer market segment
{
   static const vector<string> mktSegment = {"AUTOMOBILE", "BUILDING", "FURNITURE", "MACHINERY", "HOUSEHOLD"};
   static uniform_int_distribution<size_t> dist(0, mktSegment.size() - 1);

   return mktSegment[dist(rng)];
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
