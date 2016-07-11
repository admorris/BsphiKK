#include "ResultDB.h"

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("Usage: %s <results.csv> <name>\n",argv[0]);
    printf("%d\n",argc);
    return 1;
  }
  ResultDB rdb(argv[1]);
  rdb.Print(argv[2]);
  rdb.Save();
  return 0;
}
