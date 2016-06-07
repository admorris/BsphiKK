#include "ResultDB.h"

int main(int argc, char* argv[])
{
  if(argc != 5)
  {
    printf("Usage: %s <results.csv> <numerator> <denominator> <new name>",argv[0]);
    return 1;
  }
  ResultDB rdb(argv[1]);
  result numerator = rdb.Get(argv[2]);
  result denominator = rdb.Get(argv[3]);
  double value = numerator.value/denominator.value;
  double error = sqrt(pow(numerator.error/numerator.value,2)+pow(denominator.error/denominator.value,2));
  rdb.Update(argv[4],"percent",value,error);
  rdb.Save();
  return 0;
}
