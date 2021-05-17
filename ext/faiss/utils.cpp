#include "utils.h"

float* float_array(Rice::Object o)
{
  Rice::String s = o.call("to_binary");
  return (float*) s.c_str();
}

uint8_t* uint8_array(Rice::Object o)
{
  Rice::String s = o.call("to_binary");
  return (uint8_t*) s.c_str();
}

// TODO return Numo::SFloat
Rice::String result(float* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(float)));
}

// TODO return Numo::UInt8
Rice::String result(uint8_t* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(uint8_t)));
}

// TODO return Numo::Int32
Rice::String result(int32_t* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(int32_t)));
}

// TODO return Numo::Int64
Rice::String result(int64_t* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(int64_t)));
}
