#include "annotation.h"

using namespace std;
annotation* resonance(double mass, string name, bool drawline)
{
  annotation* _a = new annotation;
  _a->line.SetY1(0);
  _a->line.SetY2(1);
  _a->line.SetX1(mass);
  _a->line.SetX2(mass);
  _a->line.SetLineStyle(2);
  if(drawline)
  {
    _a->line.SetLineColor(2);
  }
  else
  {
    _a->line.SetLineColor(0);
  }
  _a->label.SetText(mass-50,1.0,name.c_str());
  return _a;
}
