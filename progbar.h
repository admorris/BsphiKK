#ifndef __PROGBAR_H__
#define __PROGBAR_H__
using namespace std;
class progbar
{
  private:
    long t0;
    int nevents, barwidth;
  public:
    progbar(int);
    progbar(int, int);
    void print(int);
    void terminate();
    void reset();
};
#endif
