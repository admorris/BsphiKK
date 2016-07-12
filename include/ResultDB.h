#ifndef __ResultDB_h__
#define __ResultDB_h__
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "MassFitter.h"
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
struct parameter
{
  parameter(string _n, string _l, Component* _c) : name(_c->GetName()+_n), latex(_l)
  {
    value = _c->GetValue(_n);
    error = _c->GetError(_n);
  }
  string name;
  string latex;
  double value;
  double error;
  string safename()
  {
    string temp = name;
    replace(temp.begin(),temp.end(),'1','A'); // Numbers can't go in LaTeX macros
    replace(temp.begin(),temp.end(),'2','B');
    replace(temp.begin(),temp.end(),'3','C');
    return temp;
  }
};
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
struct format_result
{
  format_result(const result&);
  string value;
  string error;
  string both;
  string scval;
  string scerr;
  string scbo;
  string macroname;
  int ndp, nvsf, nesf;
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
    result Get(string resultname){ return *find(resultname); }
    void Export(string);
    void Print(string);
  private:
    result* find(string);
    string _filename;
    vector<result> _table;
    bool _isopen;
    bool _ismod;
};
#endif
