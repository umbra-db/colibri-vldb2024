#pragma once
//---------------------------------------------------------------------------
#include "txbench/Network.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
struct pg_conn;
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// A prepared statement
class PqPreparedStatement {
   /// The name of the prepared statement
   std::string name;
   /// The number of parameters of the prepared statement
   int numParams = 0;

   /// The parameter value pointers
   std::vector<const char*> paramValues;
   /// The parameter value buffer
   std::vector<char> paramValuesBuffer;
   /// The parameter lengths
   std::vector<int> paramLengths;
   /// The parameter formats
   std::vector<int> paramFormats;

   /// Constructor
   PqPreparedStatement(std::string name, int numParams);

   /// Serialize parameters
   template <typename... Tail>
   void serialize(unsigned index, int32_t head, Tail&&... tail);
   /// Serialize parameters
   template <typename... Tail>
   void serialize(unsigned index, uint32_t head, Tail&&... tail);
   /// Serialize parameters
   template <typename... Tail>
   void serialize(unsigned index, int64_t head, Tail&&... tail);
   /// Serialize parameters
   template <typename... Tail>
   void serialize(unsigned index, uint64_t head, Tail&&... tail);
   /// Serialize parameters
   template <typename... Tail>
   void serialize(unsigned index, const std::string& head, Tail&&... tail);

   friend class PqConnection;

   public:
   /// Constructor
   PqPreparedStatement();
   /// Constructor
   PqPreparedStatement(PqPreparedStatement&) = delete;
   /// Constructor
   PqPreparedStatement(PqPreparedStatement&&) = default;
   /// Destructor
   ~PqPreparedStatement();

   /// Assignment
   void operator=(PqPreparedStatement&) = delete;
   /// Assignment
   PqPreparedStatement& operator=(PqPreparedStatement&&) = default;

   /// Bind parameter values and prepare an asynchronous call
   template <typename... Args>
   void bind(Args&&... args);
};
//---------------------------------------------------------------------------
template <typename... Tail>
void PqPreparedStatement::serialize(unsigned index, int32_t head, Tail&&... tail)
// Serialize parameters
{
   // Copy the parameter
   auto arg = Network::host2be(head);
   auto offset = paramValuesBuffer.size();
   paramValuesBuffer.resize(paramValuesBuffer.size() + sizeof(int32_t));
   memcpy(&paramValuesBuffer[offset], &arg, sizeof(int32_t));

   paramLengths[index] = sizeof(int32_t);
   paramFormats[index] = 1; // binary format

   if constexpr (sizeof...(tail) > 0)
      serialize(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void PqPreparedStatement::serialize(unsigned index, uint32_t head, Tail&&... tail)
// Serialize parameters
{
   // Copy the parameter
   auto arg = Network::host2be(head);
   auto offset = paramValuesBuffer.size();
   paramValuesBuffer.resize(paramValuesBuffer.size() + sizeof(uint32_t));
   memcpy(&paramValuesBuffer[offset], &arg, sizeof(uint32_t));

   paramLengths[index] = sizeof(uint32_t);
   paramFormats[index] = 1; // binary format

   if constexpr (sizeof...(tail) > 0)
      serialize(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void PqPreparedStatement::serialize(unsigned index, int64_t head, Tail&&... tail)
// Serialize parameters
{
   // Copy the parameter
   auto arg = Network::host2be(head);
   auto offset = paramValuesBuffer.size();
   paramValuesBuffer.resize(paramValuesBuffer.size() + sizeof(int64_t));
   memcpy(&paramValuesBuffer[offset], &arg, sizeof(int64_t));

   paramLengths[index] = sizeof(int64_t);
   paramFormats[index] = 1; // binary format

   if constexpr (sizeof...(tail) > 0)
      serialize(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void PqPreparedStatement::serialize(unsigned index, uint64_t head, Tail&&... tail)
// Serialize parameters
{
   // Copy the parameter
   auto arg = Network::host2be(head);
   auto offset = paramValuesBuffer.size();
   paramValuesBuffer.resize(paramValuesBuffer.size() + sizeof(uint64_t));
   memcpy(&paramValuesBuffer[offset], &arg, sizeof(uint64_t));

   paramLengths[index] = sizeof(uint64_t);
   paramFormats[index] = 1; // binary format

   if constexpr (sizeof...(tail) > 0)
      serialize(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Tail>
void PqPreparedStatement::serialize(unsigned index, const std::string& head, Tail&&... tail)
// Serialize parameters
{
   // Copy the parameter
   auto offset = paramValuesBuffer.size();
   paramValuesBuffer.resize(paramValuesBuffer.size() + head.size() + 1);
   memcpy(&paramValuesBuffer[offset], head.c_str(), head.size());

   // Terminate the string with a zero byte
   paramValuesBuffer[offset + head.size()] = '\0';

   paramLengths[index] = head.size() + 1;
   paramFormats[index] = 0; // string format

   if constexpr (sizeof...(tail) > 0)
      serialize(index + 1, tail...);
}
//---------------------------------------------------------------------------
template <typename... Args>
void PqPreparedStatement::bind(Args&&... args)
// Call the procedure
{
   paramValuesBuffer.clear();

   if constexpr (sizeof...(args) > 0)
      serialize(0, args...);

   auto offset = 0;
   for (int i = 0; i < numParams; ++i) {
      paramValues[i] = &paramValuesBuffer[offset];
      offset += paramLengths[i];
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
