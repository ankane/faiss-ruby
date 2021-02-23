#pragma once

#include <rice/Object.hpp>
#include <rice/String.hpp>

float* float_array(Rice::Object o);
uint8_t* uint8_array(Rice::Object o);

// TODO return Numo::SFloat
Rice::String result(float* ptr, int64_t length);
// TODO return Numo::UInt8
Rice::String result(uint8_t* ptr, int64_t length);
// TODO return Numo::Int32
Rice::String result(int32_t* ptr, int64_t length);
// TODO return Numo::Int64
Rice::String result(int64_t* ptr, int64_t length);
