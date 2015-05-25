#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>
/*
  Before loop: create a progbar object and tell it how many events are in the loop.
  During loop: call progbar::print(i) where i is the current event number
  After  loop: call progbar::terminate()
  Example:

    int n = 1000;
    progbar bar(n);
    for(int i = 0; i < n; i++)
    {
      if(i%10==0) bar.print(i);
    }
    bar.terminate();

*/
class progbar
{
  private:
    long t0;
    int nevents, barwidth;
  public:
    progbar(int n_events, int bar_width = 80)
    {
      nevents=n_events;
      barwidth=bar_width;
      t0=time(0);
    }
    void print(int);
    void terminate();
    void reset();
};
void progbar::print(int ievent)
{
  int perc = (int)(100*ievent/nevents)+1;
  cout << "\r ┃";
  for(int ibar = 0; ibar < (perc*barwidth)/100; ibar++)
    cout << "█";
  for(int ibar = (perc*barwidth)/100; ibar < barwidth; ibar++)
    cout << " ";
  cout << "┃ " << perc << "% " << flush;
  long t1 = time(0);
  cout << (t1-t0)/60 << ":" << setw(2) << setfill('0') << (t1-t0)%60 << flush;
  return;
}
void progbar::terminate()
{
  this->print(nevents-1);
  cout << "\t " << nevents << " events processed " << endl;
  return;
}
void progbar::reset()
{
  t0=time(0);
  return;
}
