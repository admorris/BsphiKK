#ifndef __annotation_h__
#define __annotation_h__
#include <string>
#include "TLine.h"
#include "TLatex.h"
using namespace std;
typedef struct
{
  TLine line;
  TLatex label;
} annotation;
annotation* resonance(double mass, string name, bool drawline = true);
#endif
