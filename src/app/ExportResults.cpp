#include "ResultDB.h"

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("Usage: %s <results.csv> <output.tex>",argv[0]);
    return 1;
  }
  ResultDB rdb(argv[1]);
  rdb.Export(argv[2]);
  return 0;
}
