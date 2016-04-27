#include "ResultDB.h"
#include <fstream>
#include <iostream>
#include <iomanip>
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
    if(row.name != "temp")
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
    string value, error, both;
    string scval, scerr, scbo;
    int ov = order(row.value);
    int oe = order(row.error);
    int ndp = oe<0 ? -oe : 0;
    int nesf = oe>0 ? 1 + oe : 1;
    int nvsf = ov>=oe? 1 + ov + ndp : 1;
    if(abs(row.error)<1e-100) // No error
    {
      error = "0";
      scerr = "0";
    }
    else
    {
      error = tostring(roundSF(row.error,nesf),ndp);
      scerr = scinot(row.error,ndp+nesf-1);
    }
    value = tostring(roundSF(row.value,nvsf),ndp);
    scval = scinot(row.value,ndp+nvsf-1);
    both = value + " \\pm " + error;
    scbo = scinot(roundSF(row.value,nvsf),roundSF(row.error,nesf),nvsf-1);
    output << "%-----------------------------------------------" << endl;
    output << "% Ndp: " << ndp << "\t Val s.f. :" << nvsf << "\t Err s.f. :" << nesf << endl;
    output << "\\def\\" << row.name <<    "val{" << value << "}" << endl;
    output << "\\def\\" << row.name <<    "err{" << error << "}" << endl;
    output << "\\def\\" << row.name <<       "{" << both  << "}" << endl;
    output << "\\def\\" << row.name << "scival{" << scval << "}" << endl;
    output << "\\def\\" << row.name << "scierr{" << scerr << "}" << endl;
    output << "\\def\\" << row.name <<    "sci{" << scbo  << "}" << endl;
  }
}
