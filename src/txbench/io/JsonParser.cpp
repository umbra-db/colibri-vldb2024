#include "txbench/io/JsonParser.hpp"
#include "picojson/picojson.h"
#include <fstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// The implementation
struct JsonParser::Impl {
   /// The root node
   picojson::object root;
   /// Whether we have encountered an error
   bool error = false;
};
//---------------------------------------------------------------------------
JsonParser::JsonParser()
// Constructor
{
}
//---------------------------------------------------------------------------
JsonParser::~JsonParser()
// Destructor
{
}
//---------------------------------------------------------------------------
Status JsonParser::parse(const string& filename)
// Attempt to parse a JSON file
{
   ifstream in(filename);
   if (!in.is_open())
      return Status::Error("unable to open file");

   picojson::value v;
   string err;
   picojson::parse(v, istreambuf_iterator<char>(in), istreambuf_iterator<char>(), &err);

   if (!err.empty() || !v.is<picojson::object>())
      return Status::Error("unable to parse JSON: " + err);

   impl = make_unique<Impl>();
   impl->root = std::move(v.get<picojson::object>());

   return Status::Success;
}
//---------------------------------------------------------------------------
void JsonParser::markFailed()
// Record an error
{
   impl->error = true;
}
//---------------------------------------------------------------------------
void JsonParser::map(const string& key, bool& target)
// Map a string
{
   auto it = impl->root.find(key);
   if (it == impl->root.end())
      return markFailed();

   const auto& value = it->second;
   if (!value.is<bool>())
      return markFailed();

   target = value.get<bool>();
}
//---------------------------------------------------------------------------
void JsonParser::map(const string& key, string& target)
// Map a string
{
   auto it = impl->root.find(key);
   if (it == impl->root.end())
      return markFailed();

   const auto& value = it->second;
   if (!value.is<string>())
      return markFailed();

   target = value.get<string>();
}
//---------------------------------------------------------------------------
void JsonParser::map(const string& key, optional<string>& target)
// Map a string
{
   auto it = impl->root.find(key);
   if (it == impl->root.end())
      return;

   const auto& value = it->second;
   if (!value.is<string>())
      return markFailed();

   target = value.get<string>();
}
//---------------------------------------------------------------------------
void JsonParser::map(const string& key, unsigned& target)
// Map an integer
{
   auto it = impl->root.find(key);
   if (it == impl->root.end())
      return markFailed();

   const auto& value = it->second;
   if (!value.is<int64_t>())
      return markFailed();

   auto intValue = value.get<int64_t>();
   if ((intValue < numeric_limits<unsigned>::min()) || (intValue > numeric_limits<unsigned>::max()))
      return markFailed();

   target = intValue;
}
//---------------------------------------------------------------------------
void JsonParser::map(const string& key, optional<unsigned>& target)
// Map an integer
{
   auto it = impl->root.find(key);
   if (it == impl->root.end())
      return;

   const auto& value = it->second;
   if (!value.is<int64_t>())
      return markFailed();

   auto intValue = value.get<int64_t>();
   if ((intValue < numeric_limits<unsigned>::min()) || (intValue > numeric_limits<unsigned>::max()))
      return markFailed();

   target = intValue;
}
//---------------------------------------------------------------------------
void JsonParser::map(const string& key, vector<std::string>& target)
// Map an array of strings
{
   target.clear();

   auto it = impl->root.find(key);
   if (it == impl->root.end())
      return markFailed();

   const auto& value = it->second;
   if (!value.is<picojson::array>())
      return markFailed();

   const auto& arrayValue = value.get<picojson::array>();
   for (const auto& elem : arrayValue) {
      if (!elem.is<string>())
         return markFailed();

      target.push_back(elem.get<string>());
   }
}
//---------------------------------------------------------------------------
bool JsonParser::hasError() const
// Check whether we have encountered an error during parsing
{
   return impl->error;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
