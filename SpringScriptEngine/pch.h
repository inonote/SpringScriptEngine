// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _SSE_WINDOWS
#ifdef _SSE_WINDOWS
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include "types.h"
#endif // _SSE_WINDOWS

#include <stdlib.h>
#include <mbstring.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>