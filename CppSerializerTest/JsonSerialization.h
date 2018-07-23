#pragma once
#include <utility>

// this macro creates a pair<const char*, ValType> first : name of a value second : value
#define KEY_VAL(NAME)  std::make_pair(#NAME, NAME) 
#define KEY_VALPTR(NAME) std::make_pair(#NAME, &NAME) 

