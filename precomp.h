// add your external includes to this file instead of to individual .cpp files

#define SCRWIDTH		800
#define SCRHEIGHT		512
// #define FULLSCREEN
// #define ADVANCEDGL	// faster if your system supports it

#include <cinttypes>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <cstdio>

extern "C"
{
#include "glew.h" 
}
#include "gl.h"
#include "wglext.h"

#include "SDL.h"

#include "FreeImage.h"
#include "template.h"
#include "surface.h"

//SIMD Intrinsics headers.
//#include "emmintrin.h"
//#include "immintrin.h"
