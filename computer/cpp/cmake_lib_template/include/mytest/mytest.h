#pragma once

#include <iostream>
#include <string>

#include "detail/add.h"

#define STRING_WRAP(x) #x
#define DEFINE_WRAP(x) STRING_WRAP(x)
#define MY_LIB_VERSION DEFINE_WRAP(MY_LIBRARY_VERSION)
#define MY_LIB_VERSION_MAJOR MY_LIBRARY_VERSION
#define MY_LIB_VERSION_MINOR MY_LIBRARY_VERSION_MINOR
#define MY_LIB_VERSION_PATCH MY_LIBRARY_VERSION_MINOR

int TestAdd(int a, int b);
