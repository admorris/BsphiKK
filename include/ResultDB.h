#ifndef __ResultDB_h__
#define __ResultDB_h__
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;
int order(double value)
{
  if(abs(value)<1e-100) return 0;
  return floor(log10(abs(value)));
}
double roundSF(double value, int nsf)
{
  if(abs(value)<1e-100) return 0;
  if(nsf<1) return 0;
  double factor = pow(10.0, nsf - ceil(log10(abs(value))));
  return round(value * factor) / factor;
}
double roundDP(double value, int ndp)
{
  double factor = pow(10.0, ndp);
  return round(value * factor) / factor;
}
template<typename T>
string tostring(T value, int prec)
{
  stringstream s;
  s << setprecision(prec) << fixed << value;
  return s.str();
}
string scinot(double value, int ndp)
{
  int ov = order(value);
  value /= pow(10.,ov);
  return tostring(roundDP(value,ndp), ndp) + "\\times 10^{" + tostring(ov,0) + "}";
}
string scinot(double value, double error, int ndp)
{
  int o = order(value);
  value /= pow(10.,o);
  error /= pow(10.,o);
  return "(" + tostring(roundDP(value,ndp), ndp) + " \\pm " + tostring(roundDP(error,ndp), ndp) + ")"+ "\\times 10^{" + tostring(o, 0) + "}";
}
struct result
{
  result();
  result(string, string, double, double);
  void set(string, double, double);
  string name;
  string type;
  double value;
  double error;
};
class ResultDB
{
  public:
    ResultDB();
    ResultDB(string);
    ~ResultDB();
    bool Open(string);
    bool Save(string filename = "");
    bool Close();
    void Update(string, string, double, double);
    void Export(string);
  private:
    result* find(string);
    string _filename;
    vector<result> _table;
    bool _isopen;
    bool _ismod;
};
#endif
