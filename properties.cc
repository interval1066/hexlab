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
	this->_propdata._mode = mode;
	if(this->_propdata._mode == MODE::PRP_READONLY)
		throw new BADMODE_EXCEPTION;
	this->_propdata._file.clear();
}

Properties::Properties(const char* file, int mode)
{
	_propdata._mode = PRP_READWRITE;
	OpenPropFile(file);
}

Properties::~Properties()
{
}

void
Properties::process(ifstream& f)
{
	string content;

	while(getline(f, content)) {
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
						_propdata._props.insert(prop_entry);
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
#ifdef _REENTRANT
	lock_guard<mutex> guard(_propdata._proplock);
#endif
	for(auto& x: _propdata._props)
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
	if(_propdata._mode == MODE::PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	size_t found;
	string content;
#ifdef _REENTRANT
	lock_guard<mutex> guard(_propdata._proplock);
#endif
	MAP temp(_propdata._props);

	_propdata._file = string(file);
	string path = getdir(_propdata._file);
	ifstream filestream(_propdata._file.c_str());

	ofstream propout("temp", ios::trunc | ios::out);
	if(filestream.is_open()) {
		while(getline(filestream, content)) {
			found = content.find_first_of("=");
			if(found != string::npos) {
				string key = content.substr(0, found);
				if(str_alnum(key)) {
					MAP::const_iterator FOUND = _propdata._props.find(key);
					if(FOUND != _propdata._props.end()) {
						if(!should_quote(FOUND->second))
							propout << key << '=' << '\"' << FOUND->second << '\"' << '\n';
						else
							propout << key << '=' << FOUND->second << '\n';
						_propdata._props.erase(key);
					}
				}
				else
					propout << '\n';
			}
			else
				propout << content << '\n';
		}
		for(auto& x: _propdata._props) {
			if(!should_quote(x.second)) 
				propout << x.first << "=" << '\"' << x.second << '\"' << '\n';
			else
				propout << x.first << "=" << x.second << '\n';
			pair<string, string> prop_entry(x.first, x.second);
			_propdata._props.swap(temp);
		}
		filestream.close();
		remove(_propdata._file.c_str());
	}
	else {
		for(auto& x: _propdata._props)
			propout << x.first << "=" << x.second << '\n';
	}
	if(rename("./temp", _propdata._file.c_str()) != 0)
		throw new WRITE_EXCEPTION;

	propout.close();
}

void
Properties::OpenPropFile(const char* file)
{
	// mode is read/write unless explicately set
#ifdef _REENTRANT
	lock_guard<mutex> guard(_propdata._proplock);
#endif
	_propdata._file = string(file);
	if(!_propdata._file.empty()) {
		_propdata._file = string(file);
		ifstream filestream(_propdata._file.c_str());

		if(filestream.is_open()) {
			process(filestream);
			filestream.close();
		}
	}
}

string
Properties::Get(const string& key, const string& def)
{
#ifdef _REENTRANT
	lock_guard<mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return string("");
	}
	if(_propdata._file.empty()) {
		throw new NOKEY_EXCEPTION;
		return string("");
	}
	string strRet;

	MAP::const_iterator FOUND = _propdata._props.find(key);
	if(FOUND != _propdata._props.end())
		strRet = FOUND->second;
	else {
		if(def.length() > 0) {
			pair<string, string> prop_entry(key, def);
			_propdata._props.insert(prop_entry);
			strRet = def;
		}
	}
	return strRet;
}

void
Properties::Set(const string& key, const string& value)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	MAP::const_iterator FOUND = _propdata._props.find(key);
 	if(FOUND != _propdata._props.end())
		_propdata._props.erase(key);
	pair<string, string> prop_entry(key, value);
	_propdata._props.insert(prop_entry);
}

int
Properties::Get(const string& key, int def)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return -1;
	}
	if(_propdata._file.empty()) {
		throw new NOKEY_EXCEPTION;
		return -1;
	}
	int nRet;

	MAP::const_iterator FOUND = _propdata._props.find(key);
	if(FOUND != _propdata._props.end()) {
		istringstream ss(FOUND->second);
		ss >> nRet;
	}
	else {
		pair<string, string> prop_entry(key, to_string(def));
		_propdata._props.insert(prop_entry);
		nRet = def;
	}
	return nRet;
}

void
Properties::Set(const string& key, int value)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	MAP::const_iterator FOUND = _propdata._props.find(key);
 	if(FOUND != _propdata._props.end())
		_propdata._props.erase(key);
	pair<string, string> prop_entry(key, to_string(value));
	_propdata._props.insert(prop_entry);
}

unsigned int
Properties::Get(const string& key, unsigned int def)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return -1;
	}
	if(_propdata._file.empty()) {
		throw new NOKEY_EXCEPTION;
		return -1;
	}
	unsigned int nRet;

	MAP::const_iterator FOUND = _propdata._props.find(key);
	if(FOUND != _propdata._props.end()) {
		istringstream ss(FOUND->second);
		ss >> nRet;
	}
	else {
		pair<string, string> prop_entry(key, to_string(def));
		_propdata._props.insert(prop_entry);
		nRet = def;
	}
	return nRet;
}

void
Properties::Set(const string& key, unsigned int value)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	MAP::const_iterator FOUND = _propdata._props.find(key);
 	if(FOUND != _propdata._props.end())
		_propdata._props.erase(key);
	pair<string, string> prop_entry(key, to_string(value));
	_propdata._props.insert(prop_entry);
}

long
Properties::Get(const string& key, long def)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return -1L;
	}
	if(_propdata._file.empty()) {
		throw new NOKEY_EXCEPTION;
		return -1L;
	}
	long nRet;

	MAP::const_iterator FOUND = _propdata._props.find(key);
	if(FOUND != _propdata._props.end()) {
		istringstream ss(FOUND->second);
		ss >> nRet;
	}
	else {
		pair<string, string> prop_entry(key, to_string(def));
		_propdata._props.insert(prop_entry);
		nRet = def;
	}
	return nRet;
}

void
Properties::Set(const string& key, long value)
{
#ifdef _REENTRANT
	_propmutex.lock();
#endif
	if(_propdata._mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	MAP::const_iterator FOUND = _propdata._props.find(key);
 	if(FOUND != _propdata._props.end())
		_propdata._props.erase(key);
	pair<string, string> prop_entry(key, to_string(value));
	_propdata._props.insert(prop_entry);
#ifdef _REENTRANT
	_propmutex.unlock();
#endif
}

double
Properties::Get(const string& key, double def)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return -1.0;
	}
	if(_propdata._file.empty()) {
		throw new NOKEY_EXCEPTION;
		return -1.0;
	}
	double nRet;

	MAP::const_iterator FOUND = _propdata._props.find(key);
	if(FOUND != _propdata._props.end()) {
		istringstream ss(FOUND->second);
		ss >> nRet;
	}
	else {
		pair<string, string> prop_entry(key, to_string(def));
		_propdata._props.insert(prop_entry);
		nRet = def;
	}
	return nRet;
}

void
Properties::Set(const string& key, double value)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	MAP::const_iterator FOUND = _propdata._props.find(key);
 	if(FOUND != _propdata._props.end())
		_propdata._props.erase(key);
	pair<string, string> prop_entry(key, to_string(value));
	_propdata._props.insert(prop_entry);
}

float
Properties::Get(const string& key, float def)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_WRITEONLY) {
		throw new WRITEONLY_EXCEPTION;
		return -1.0;
	}
	if(_propdata._file.empty()) {
		throw new NOKEY_EXCEPTION;
		return -1.0;
	}
	float nRet;

	unordered_map<string, string>::const_iterator FOUND = _propdata._props.find(key);
	if(FOUND != _propdata._props.end()) {
		istringstream ss(FOUND->second);
		ss >> nRet;
	}
	else {
		pair<string, string> prop_entry(key, to_string(def));
		_propdata._props.insert(prop_entry);
		nRet = def;
	}
	return nRet;
}

void
Properties::Set(const string& key, float value)
{
#ifdef _REENTRANT
	lock_guard<std::mutex> guard(_propdata._proplock);
#endif
	if(_propdata._mode == PRP_READONLY) {
		throw new READONLY_EXCEPTION;
		return;
	}
	unordered_map<string, string>::const_iterator FOUND = _propdata._props.find(key);
 	if(FOUND != _propdata._props.end())
		_propdata._props.erase(key);
	pair<string, string> prop_entry(key, to_string(value));
	_propdata._props.insert(prop_entry);

}

