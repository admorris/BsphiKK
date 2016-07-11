#include "ResultDB.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
result::result()
  : name("temp")
  , type("decimal")
  , value(0)
  , error(0)
{

}
result::result(string n, string t, double v, double e)
  : name(n)
  , type(t)
  , value(v)
  , error(e)
{

}
void result::set(string t, double v, double e)
{
  type = t;
  value = v;
  error = e;
}
ResultDB::ResultDB()
  : _isopen(false)
  , _ismod(false)
{

}
ResultDB::ResultDB(string filename)
  : _isopen(false)
  , _ismod(false)
{
  Open(filename);
}
ResultDB::~ResultDB()
{
  Close();
}
bool ResultDB::Open(string filename)
{
  if(_isopen) 
  {
    cerr << "Cannot open " << filename << " because " << _filename << " is already open. Close it first." << endl;
    return false;
  }
  _filename = filename;
  ifstream input(filename);
  if(!input.is_open())
  {
    cerr << _filename << " not found. The file will be created if Save() is called without argument." << endl;
    _isopen = true;
    return true;
  }
  do
  {
    result row;
    input >> row.name >> row.type >> row.value >> row.error;
    if(row.name != "temp") // why is this here?
      _table.push_back(row);
  } while(!input.eof());
  input.close();
  _isopen = true;
  return true;
}
bool ResultDB::Save(string filename)
{
  if(!_isopen)
  {
    cerr << "Nothing to save." << endl;
    return false;
  }
  ofstream output;
  if(filename != "")
    output.open(filename);
  else if(_filename != "")
    output.open(_filename);
  else
  {
    cerr << "No filename specified." << endl;
    return false;
  }
  for(auto row : _table)
    output << row.name << "\t" << row.type << "\t" << row.value << "\t" << row.error << endl;
  output.close();
  _ismod = false;
  return true;
}
bool ResultDB::Close()
{
  if(_isopen)
    _table.clear();
  _isopen = false;
  if(_ismod)
    cerr << "Closed without saving. Changes lost." << endl;
  _ismod = false;
  return true;
}
void ResultDB::Update(string name, string type, double value, double error)
{
  if(name == "") name = "temp";
  if(type == "") type = "default";
  find(name)->set(type, value, error);
  _ismod = true;
}
result* ResultDB::find(string name)
{
  for(auto& row : _table)
    if(row.name == name)
      return &row;
  _table.push_back(result(name,"decimal",0,0));
  return &_table.back();
}
void ResultDB::Export(string filename)
{
  if(filename == "")
  {
    cerr << "Filename cannot be empty." << endl;
    return;
  }
  cout << "Exporting LaTeX commands to " << filename << endl;
  if(filename.find(".tex") == string::npos)
    cerr << "Warning: this should really be a .tex file." << endl;
  ofstream output(filename);
  for(auto row : _table)
  {
    format_result frow(row);
    output << "%-----------------------------------------------" << endl;
    output << "% Ndp: " << frow.ndp << "\t Val s.f. :" << frow.nvsf << "\t Err s.f. :" << frow.nesf << endl;
    output << "\\def\\" << frow.macroname <<    "val{\\ensuremath{" << frow.value << "}\\xspace}" << endl;
    output << "\\def\\" << frow.macroname <<    "err{\\ensuremath{" << frow.error << "}\\xspace}" << endl;
    output << "\\def\\" << frow.macroname <<       "{\\ensuremath{" << frow.both  << "}\\xspace}" << endl;
    output << "\\def\\" << frow.macroname << "scival{\\ensuremath{" << frow.scval << "}\\xspace}" << endl;
    output << "\\def\\" << frow.macroname << "scierr{\\ensuremath{" << frow.scerr << "}\\xspace}" << endl;
    output << "\\def\\" << frow.macroname <<    "sci{\\ensuremath{" << frow.scbo  << "}\\xspace}" << endl;
  }
}
void ResultDB::Print(string name)
{
  for(auto row: _table)
  {
    if(name == row.name)
    {
      cout << "rawvalue  :\t" << row.value << endl;
      cout << "rawerror  :\t" << row.error << endl;
      format_result frow(row);
      cout << "value     :\t" << frow.value << endl;
      cout << "error     :\t" << frow.error << endl;
      cout << "result    :\t" << frow.both  << endl;
      cout << "sci value :\t" << frow.scval << endl;
      cout << "sci error :\t" << frow.scerr << endl;
      cout << "sci result:\t" << frow.scbo  << endl;
    }
  }
}
format_result::format_result(const result& row)
{
  bool perc = row.type == "percent";
  double factor = perc ? 100 : 1;
  double val = row.value*factor, err = row.error*factor;
  macroname = regex_replace(row.name,regex("[^A-Za-z]"),"");
  int ov = order(val);
  int oe = order(err);
  int e3sf;
  if(abs(err)<1e-100)
  {
    if(ov<2) val = roundSF(val,3); // Round to 3sf if no error and order < 2
    else val = roundDP(val,0); // If no error and order > 2, round to nearest int
    err = 0;
    error = "0";
    scerr = "0";
    ndp = 2-ov;
    nvsf = 3;
    nesf = 0;
  }
  else
  {
    e3sf = floor(err*pow(10,3-ceil(log10(abs(err))))); // first 3 significant digits of the error as a 3-digit int
    nesf = 1;
    if(e3sf < 100)
    {
      cerr << "Something is wrong with the rounding" << endl;
    }
    else if(e3sf < 355)
    {
      nesf = 2;
    }
    else if(e3sf < 950)
    {
      nesf = 1;
    }
    else
    {
      err = roundSF(err,1);
      nesf = 2;
      oe++;
    }
    ndp = oe<0 ? nesf-1-oe : 0;
    nvsf = nesf + ov - oe;
    val = roundSF(val,nvsf);
    err = roundSF(err,nesf);
    error = tostring(err,ndp);
    scerr = scinot(err,nesf-1);
  }
  value = tostring(val,ndp);
  scval = scinot(val,nvsf-1);
  both = value + " \\pm " + error;
  scbo = scinot(val,err,nvsf-1);
  if(perc)
  {
    value += "\\,\\%";
    error += "\\,\\%";
    both = "("+both+")\\,\\%";
    scval += "\\,\\%";
    scerr += "\\,\\%";
    scbo = "("+scbo+")\\,\\%";
  }
}
