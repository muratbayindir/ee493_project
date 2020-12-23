#pragma once
#include <iostream>
#include <string>
#include <stdint.h>
#include <Windows.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#ifdef _DEBUG
#define log std::cout
#else
#define log {}/ ## /
#endif
