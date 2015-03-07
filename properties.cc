#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <mutex>
#include <utils/properties.h>

using namespace std;
using namespace utils;

Properties::Properties(int mode)
{
	this->_propdata.mode = mode;
	if(this->_propdata.mode == MODE::PRP_READONLY)
		throw new BADMODE_EXCEPTION;
	this->_propdata.file.clear();
}

Properties::Properties(const char* file, int mode)
{
	_propdata.mode = PRP_READWRITE;
	try {
		OpenPropFile(file);
	}
	catch (PropException* e) {
		throw e;
	}
}

Properties::~Properties()
{
	//Write(_propdata.file.c_str());
}

void
Properties::process(ifstream& f)
{
	string content;

	while(getline(f, content)) {
		_propdata.text.push_back(content);
		try {
			parse(content);
			if(content.length() > 0) {
				if(content.find("=") == string::npos)
					throw new BADKEY_EXCEPTION;
				else {
					string key = content.substr(0, content.find_first_of("="));
					string value = content.substr(content.find_first_of("=") + 1, content.length());
					value.erase(remove(value.begin(), value.end(), '\"'), value.end());

					if((!key.empty()) && str_alnum(key)) {
						pair<string, string> prop_entry(key, value);
						_propdata.props.insert(prop_entry);
					}
				}
			}
		}
		catch (PropException* e) {
			// We don't care that a property was badly formatted.
			// We just discard it, and is not re-rewritten if the
			// user writes new properties to it.
			continue;
		}
	}
}

void
Properties::dump()
{
	for(auto& x: _propdata.props)
		cout << x.first << " - " << x.second << endl;
}

void
Properties::parse(string& str)
{
	string buf;
	str = trim(str);

	size_t f = str.find_first_of(";#[");
	if(f != string::npos)
		buf = str.erase(f, string::npos);

	if(buf.find("\"") == string::npos)
		buf = buf.substr(0, buf.find_first_of(' '));

	if(buf.length() > 0) str = buf;
}

string
Properties::trim(const string& str)
{
	int nsize = str.size();
	int nspos = 0, nepos = 1, i;

	for(i = 0; i < nsize; ++i) {
		if(!isspace(str[i])) {
			nspos = i;
			break;
		}
	}

	for(i = nsize - 1; i >= 0; --i) {
		if(!isspace(str[i])) {
			nepos = i;
			break;
		}
	}
	return string(str, nspos, nepos - nspos + 1);
}

void
Properties::Write(const char* file)
{
	if(_propdata.mode == MODE::PRP_READONLY)
		throw new READONLY_EXCEPTION;

	ofstream out(file);
	for(auto& x: _propdata.text) {
		if(x.find("=", 0) != string::npos) {
			string key = x.substr(0, x.find("=", 0));
			string val = Get(key, "");
			if(val.length() > 1)
				out << key.c_str() << "=" << Get(key, "") << "\n";
			else
				out << x << "\n";
		}
		else
			out << x.c_str() << "\n";
	}
	out.close();
}

void
Properties::OpenPropFile(const char* file)
{
	// mode is read/write unless explicately set
	string fName = string(file);
	if(!FileExist(fName))
		throw new FILE_EXCEPTION;

	_propdata.file = fName;
	
	if(!_propdata.file.empty()) {
		_propdata.file = string(file);
		ifstream filestream(_propdata.file);

		if(filestream.is_open()) {
			process(filestream);
			filestream.close();
		}
	}
	else throw new FILE_EXCEPTION;
}

const string
Properties::Get(const string& key, const string& def)
{
	if(_propdata.mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return string("");
	}

	if(_propdata.file.empty()) {
		throw new NOKEY_EXCEPTION;
		return string("");
	}
	string strRet;

	MAP::const_iterator FOUND = _propdata.props.find(key);
	if(FOUND != _propdata.props.end())
		strRet = FOUND->second;
	else {
		if(def.length() > 0) {
			pair<string, string> prop_entry(key, def);
			_propdata.props.insert(prop_entry);
			strRet = def;
		}
	}
	return strRet;
}

void
Properties::Set(const string& key, const string& value)
{
	if(_propdata.mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	MAP::const_iterator FOUND = _propdata.props.find(key);
 	if(FOUND != _propdata.props.end())
		_propdata.props.erase(key);
	pair<string, string> prop_entry(key, value);
	_propdata.props.insert(prop_entry);
}

double
Properties::Get(const string& key, double def)
{
	if(_propdata.mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return -1.0;
	}
	if(_propdata.file.empty()) {
		throw new NOKEY_EXCEPTION;
		return -1.0;
	}
	double nRet;

	MAP::const_iterator FOUND = _propdata.props.find(key);
	if(FOUND != _propdata.props.end()) {
		istringstream ss(FOUND->second);
		ss >> nRet;
	}
	else {
		pair<string, string> prop_entry(key, to_string(def));
		_propdata.props.insert(prop_entry);
		nRet = def;
	}
	return nRet;
}

void
Properties::Set(const string& key, double value)
{
	if(_propdata.mode == PRP_READONLY)
		throw new READONLY_EXCEPTION;

	MAP::const_iterator FOUND = _propdata.props.find(key);
 	if(FOUND != _propdata.props.end())
		_propdata.props.erase(key);
	pair<string, string> prop_entry(key, to_string(value));
	_propdata.props.insert(prop_entry);
}

