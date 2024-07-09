#include "txbench/tpcch/TpcchGenerator.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tpcch {
//---------------------------------------------------------------------------
TpcchGenerator::TpcchGenerator(uint64_t seed)
   : Random(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateSentence()
// Generate a random sentence
{
   static uniform_int_distribution sentenceDistribution(0, 4);

   switch (sentenceDistribution(rng)) {
      case 0: return generateNounPhrase() + " " + generateVerbPhrase() + " " + generateTerminator();
      case 1: return generateNounPhrase() + " " + generateVerbPhrase() + " " + generatePreposition() + generateTerminator();
      case 2: return generateNounPhrase() + " " + generateVerbPhrase() + " " + generateNounPhrase() + generateTerminator();
      case 3: return generateNounPhrase() + " " + generatePreposition() + " " + generateVerbPhrase() + " " + generateNounPhrase() + generateTerminator();
      default: return generateNounPhrase() + " " + generatePreposition() + " " + generateVerbPhrase() + " " + generatePreposition() + generateTerminator();
   }
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateNounPhrase()
// Generate a random phrase
{
   static uniform_int_distribution nounDistribution(0, 3);

   switch (nounDistribution(rng)) {
      case 0: return generateNoun();
      case 1: return generateAdjective() + " " + generateNoun();
      case 2: return generateAdjective() + ", " + generateAdjective() + " " + generateNoun();
      default: return generateAdverb() + " " + generateAdjective() + " " + generateNoun();
   }
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateVerbPhrase()
// Generate a verb phrase
{
   static uniform_int_distribution verbDistribution(0, 3);

   switch (verbDistribution(rng)) {
      case 0: return generateVerb();
      case 1: return generateAuxiliary() + " " + generateVerb();
      case 2: return generateVerb() + ", " + generateAdverb();
      default: return generateAuxiliary() + " " + generateVerb() + " " + generateAdverb();
   }
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateNoun()
// Generate a random noun
{
   static vector<string> nouns = {"foxes", "ideas", "theodolites", "pinto beans",
                                  "instructions", "dependencies", "excuses", "platelets",
                                  "asymptotes", "courts", "dolphins", "multipliers",
                                  "sauternes", "warthogs", "frets", "dinos",
                                  "attainments", "somas", "Tiresias'", "patterns",
                                  "forges", "braids", "hockey players", "frays",
                                  "warhorses", "dugouts", "notornis", "epitaphs",
                                  "pearls", "tithes", "waters", "orbits",
                                  "gifts", "sheaves", "depths", "sentiments",
                                  "decoys", "realms", "pains", "grouches",
                                  "escapades"};
   static uniform_int_distribution<size_t> nounDist(0, nouns.size() - 1);

   return nouns[nounDist(rng)];
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateVerb()
// Generate a random verb
{
   static vector<string> verbs = {"sleep", "wake", "are", "cajole",
                                  "haggle", "nag", "use", "boost",
                                  "affix", "detect", "integrate", "maintain",
                                  "nod", "was", "lose", "sublate",
                                  "solve", "thrash", "promise", "engage",
                                  "hinder", "print", "x-ray", "breach",
                                  "eat", "grow", "impress", "mold",
                                  "poach", "serve", "run", "dazzle",
                                  "snooze", "doze", "unwind", "kindle",
                                  "play", "hang", "believe", "doubt"};
   static uniform_int_distribution<size_t> verbDist(0, verbs.size() - 1);

   return verbs[verbDist(rng)];
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateAdjective()
// Generate a random adjective
{
   static vector<string> adjectives = {"furious", "sly", "careful", "blithe",
                                       "quick", "fluffy", "slow", "quiet",
                                       "ruthless", "thin", "close", "dogged",
                                       "daring", "brave", "stealthy", "permanent",
                                       "enticing", "idle", "busy", "regular",
                                       "final", "ironic", "even", "bold",
                                       "silent"};
   static uniform_int_distribution<size_t> adjectiveDist(0, adjectives.size() - 1);

   return adjectives[adjectiveDist(rng)];
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateAdverb()
// Generate a random adverb
{
   static vector<string> adverbs = {"sometimes", "always", "never", "furiously",
                                    "slyly", "carefully", "blithely", "quickly",
                                    "fluffily", "slowly", "quietly", "ruthlessly",
                                    "thinly", "closely", "doggedly", "daringly",
                                    "bravely", "stealthily", "permanently", "enticingly",
                                    "idly", "busily", "regularly", "finally",
                                    "ironically", "evenly", "boldly", "silently"};
   static uniform_int_distribution<size_t> adverbDist(0, adverbs.size() - 1);

   return adverbs[adverbDist(rng)];
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generatePreposition()
// Generate a random preposition
{
   static vector<string> prepositions = {"about", "above", "according to", "across",
                                         "after", "against", "along", "alongside of",
                                         "among", "around", "at", "atop",
                                         "before", "behind", "beneath", "beside",
                                         "besides", "between", "beyond", "by",
                                         "despite", "during", "except", "for",
                                         "from", "in place of", "inside", "instead of",
                                         "into", "near", "of", "on",
                                         "outside", "over", "past", "since",
                                         "through", "throughout", "to", "toward",
                                         "under", "until", "up", "upon",
                                         "without", "with", "within"};
   static uniform_int_distribution<size_t> prepositionDist(0, prepositions.size() - 1);

   return prepositions[prepositionDist(rng)];
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateTerminator()
// Generate a random terminator
{
   static vector<string> terminators = {".", ";", ":", "?", "!", "--"};
   static uniform_int_distribution<size_t> terminatorDist(0, terminators.size() - 1);

   return terminators[terminatorDist(rng)];
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateAuxiliary()
// Generate a random auxiliary
{
   static vector<string> auxiliaries = {"do", "may", "might", "shall",
                                        "will", "would", "can", "could",
                                        "should", "ought to", "must", "will have to",
                                        "shall have to", "could have to", "should have to", "must have to",
                                        "need to", "try to"};
   static uniform_int_distribution<size_t> auxiliaryDist(0, auxiliaries.size() - 1);

   return auxiliaries[auxiliaryDist(rng)];
}
//---------------------------------------------------------------------------
string TpcchGenerator::generateRegionName(unsigned id)
// Generate a region name
{
   static string regions[regionCount] = {"AFRICA", "AMERICA", "ASIA", "EUROPE", "MIDDLE EAST"};
   return regions[id];
}
//---------------------------------------------------------------------------
TpcchGenerator::Nation TpcchGenerator::generateNation(unsigned id)
// Generate a nation
{
   static Nation nations[nationCount] = {{48, "ALGERIA", 0}, {49, "ARGENTINA", 1}, {50, "BRAZIL", 1}, {51, "CANADA", 1}, {52, "EGYPT", 4}, {53, "ETHIOPIA", 0}, {54, "FRANCE", 3}, {55, "GERMANY", 3}, {56, "INDIA", 2}, {57, "INDONESIA", 2}, {65, "IRAN", 4}, {66, "IRAQ", 4}, {67, "JAPAN", 2}, {68, "JORDAN", 4}, {69, "KENYA", 0}, {70, "MOROCCO", 0}, {71, "MOZAMBIQUE", 0}, {72, "PERU", 1}, {73, "CHINA", 2}, {74, "ROMANIA", 3}, {75, "SAUDI ARABIA", 4}, {76, "VIETNAM", 2}, {77, "RUSSIA", 3}, {78, "UNITED KINGDOM", 3}, {79, "UNITED STATES", 1}, {80, "CHINA", 2}, {81, "PAKISTAN", 2}, {82, "BANGLADESH", 2}, {83, "MEXICO", 1}, {84, "PHILIPPINES", 2}, {85, "THAILAND", 2}, {86, "ITALY", 3}, {87, "SOUTH AFRICA", 0}, {88, "SOUTH KOREA", 2}, {89, "COLOMBIA", 1}, {90, "SPAIN", 3}, {97, "UKRAINE", 3}, {98, "POLAND", 3}, {99, "SUDAN", 0}, {100, "UZBEKISTAN", 2}, {101, "MALAYSIA", 2}, {102, "VENEZUELA", 1}, {103, "NEPAL", 2}, {104, "AFGHANISTAN", 2}, {105, "NORTH KOREA", 2}, {106, "TAIWAN", 2}, {107, "GHANA", 0}, {108, "IVORY COAST", 0}, {109, "SYRIA", 4}, {110, "MADAGASCAR", 0}, {111, "CAMEROON", 0}, {112, "SRI LANKA", 2}, {113, "ROMANIA", 3}, {114, "NETHERLANDS", 3}, {115, "CAMBODIA", 2}, {116, "BELGIUM", 3}, {117, "GREECE", 3}, {118, "PORTUGAL", 3}, {119, "ISRAEL", 4}, {120, "FINLAND", 3}, {121, "SINGAPORE", 2}, {122, "NORWAY", 3}};
   return nations[id];
}
//---------------------------------------------------------------------------
int TpcchGenerator::generateNationKey()
// Generate a nation key
{
   uniform_int_distribution nationDist(0u, nationCount - 1);
   return generateNation(nationDist(rng)).id;
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateSupplierPhone(unsigned id)
// Generate a supplier phone number
{
   string result = "";
   result += to_string((id % 90) + 10) + "-"; // ensure length 2
   result += to_string(uniform_int_distribution(100, 999)(rng)) + "-";
   result += to_string(uniform_int_distribution(100, 999)(rng)) + "-";
   result += to_string(uniform_int_distribution(1000, 9999)(rng));
   return result;
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateSupplierComment(int minLength, int maxLength, std::string action)
// Generate a supplier comment
{
   auto length = uniform_int_distribution(minLength, maxLength)(rng);
   auto length1 = uniform_int_distribution(0, length - 10 - 8)(rng);
   auto length2 = uniform_int_distribution(0, length - length1 - 10 - 8)(rng);
   auto length3 = length - length1 - length2 - 10 - 8;

   string result = "";
   result += generateText(length1);
   result += "Customer";
   result += generateText(length2);
   result += action;
   result += generateText(length3);
   return result;
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateDouble(double minValue, double maxValue, unsigned int decimals)
// Generate a random double
{
   int64_t min = int(minValue * pow(10.0, decimals));
   int64_t max = int(maxValue * pow(10.0, decimals));
   return to_string(static_cast<double>(uniform_int_distribution(min, max)(rng)) / pow(10.0, decimals));
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateText(unsigned length)
// Generate a random text
{
   string s = "";
   for (int i = 0; i < 25; i++)
      s += " " + generateSentence();
   int pos = uniform_int_distribution<size_t>(0, s.length() - length)(rng);
   return s.substr(pos, length);
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateText(unsigned minLength, unsigned maxLength)
// Generate a random text
{
   uniform_int_distribution lengthDist(minLength, maxLength);
   return generateText(lengthDist(rng));
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateAlphanumeric64(unsigned length)
// Generate a alpha-numeric text
{
   uniform_int_distribution charDist(0, 63);

   string result(length, ' ');
   for (auto& c : result)
      c = alphanumeric64[charDist(rng)];
   return result;
}
//---------------------------------------------------------------------------
std::string TpcchGenerator::generateAlphanumeric64(unsigned minLength, unsigned maxLength)
// Generate a alpha-numeric text
{
   uniform_int_distribution lengthDist(minLength, maxLength);
   return generateAlphanumeric64(lengthDist(rng));
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
