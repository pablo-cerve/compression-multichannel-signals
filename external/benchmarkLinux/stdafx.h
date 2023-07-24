// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef _STDAFX_
#define _STDAFX_

#define DOUBLE_SIZE 4
#define INT_SIZE 4

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#if defined(__unix__)
//#include <dirent.h>
//TODO: uncomment next line
//#include "redef.h"
#elif defined(__linux__)
#include "redef.h"
#elif defined(_WIN32)
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include "Utils/dirent.h"
#endif

#endif
// TODO: reference additional headers your program requires here

