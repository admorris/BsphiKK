#include "ResultDB.h"

int main()
{
  ResultDB TestResults("temp.csv");
  TestResults.Update("yield","decimal",2345.545555,53.5555555);
  TestResults.Update("unity","decimal",1,0);
  TestResults.Update("phi_s","decimal",0.1,123.4);
  TestResults.Update("slope","decimal",0.14213,0.032134);
  TestResults.Update("width","decimal",2.435,0.123);
  TestResults.Update("mean" ,"decimal",45.3453,32.123);
  TestResults.Export("test.tex");
  TestResults.Save();
  return 0;
}
