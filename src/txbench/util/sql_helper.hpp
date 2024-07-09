#pragma once
//---------------------------------------------------------------------------
#include <regex>
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Split a SQL commands
static void splitSQL(const std::string& sql, std::vector<std::string>& queries) {
   if (sql.find("$$") != std::string::npos) {
      queries.push_back(sql);
      return;
   }

   std::regex pattern(";(?=(?:[^'\"]*[\"'][^'\"]*[\"'])*[^'\"]*$)(?![^\\(]*\\))");

   std::vector<std::string> commands;

   // Using std::regex_iterator to iterate over matches
   auto it = std::sregex_iterator(sql.begin(), sql.end(), pattern);
   auto end = std::sregex_iterator();
   size_t pos = 0;

   for (; it != end; ++it) {
      auto match = *it;
      size_t match_pos = match.position();
      commands.push_back(sql.substr(pos, match_pos - pos + 1));
      pos = match_pos + 1; // Skip the ';'
   }

   // Add the last part of the string if it exists
   if (pos < sql.size())
      commands.push_back(sql.substr(pos));

   for (auto& c : commands) {
      size_t first_not_space = c.find_first_not_of(" \t\r\n");
      size_t last_not_space = c.find_last_not_of(" \t\r\n");
      if (first_not_space == std::string::npos || last_not_space == std::string::npos)
         continue;

      queries.push_back(c);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
