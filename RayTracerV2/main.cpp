#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "RayTracer.h"

using namespace std;

int main(int argc, char** argv) {
   srand((unsigned)time(0));
   int maxReflections = 3;
   int superSamples = 8;
   int depthComplexity = 0;
   RayTracer rayTracer(1000, 500, maxReflections, superSamples, depthComplexity, "out.tga");
   //RayTracer rayTracer(1920, 1080, maxReflections, superSamples, depthComplexity);
}
