#include "ResultDB.h"

int main(int argc, char* argv[])
{
  if(argc != 6)
  {
    printf("Usage: %s <results.csv> <name> <type> <value> <error>\n",argv[0]);
    printf("%d\n",argc);
    return 1;
  }
  ResultDB rdb(argv[1]);
  rdb.Update(argv[2],argv[3],atof(argv[4]),atof(argv[5]));
  rdb.Save();
  return 0;
}
