#ifndef PROPERTIES_H
#define PROPERTIES_H

/**
 * Properties
 *
 * Simple properties object for GCC/Linux
 *
 * (c) 2014 MatterFab, Inc.
 * @author <tim@matterfab.com>
 *
 */

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <mutex>
#include <fstream>

enum MODE
{ PRP_WRITEONLY, PRP_READONLY, PRP_READWRITE };

typedef std::unordered_map<std::string, std::string>::key_type props_key;
typedef std::unordered_map <std::string, std::string> MAP;

typedef struct
{
	MAP								props;
	std::vector<std::string>	text;
	int								mode;
	std::string						file;
} PROPDATA;

#define KEY_EXCEPTION PropException("Key not found")
#define FILE_EXCEPTION PropException("Properties file not found")
#define NAME_EXCEPTION PropException("No properties file name")
#define WRITE_EXCEPTION PropException("Couldn\'t write properties file")
#define BADKEY_EXCEPTION PropException("Key malformed")
#define NOKEY_EXCEPTION PropException("Key doesn\'t exist");
#define BADMODE_EXCEPTION PropException("Can\'t open a new file for read-only")
#define READONLY_EXCEPTION PropException("Mode read-only")
#define WRITEONLY_EXCEPTION PropException("Mode write-only")
#define STRINGVALUE_EXCEPTION PropException("Can\'t return a numeric value");

namespace utils
{
	class PropException : public std::runtime_error
	{
		std::string _msg;

	public:
		PropException(const std::string& m) : std::runtime_error(m), _msg(m) {}
		~PropException() throw() {}
		const char* what() const throw() { return _msg.c_str(); }
	};

	class Properties
	{
		PROPDATA _propdata;
		void process(std::ifstream&);
		void parse(std::string&);

		std::string trim(const std::string&);
		inline std::string getdir(const std::string& str)
		{
			size_t found;
			found = str.find_last_of("/\\");
			return str.substr(0, found);
		}

		inline bool should_quote(const std::string& str)
		{
			return str.find_first_of(" ") == std::string::npos;
		}

		inline bool str_alnum(const std::string& str)
		{
			return find_if(str.begin(), str.end(), 
				[](char c) { return !(isalnum(c) || (c == ' ')); }) == str.end();
		}

		inline bool str_alpha(const std::string& str)
		{
			return find_if(str.begin(), str.end(), 
				[](char c) { return !(isalpha(c) || (c == ' ')); }) == str.end();
		}

	protected:
		Properties();

	public:
		Properties(int);
		Properties(const char*, int);
		virtual ~Properties(void);

		inline void SetMode(int m) { _propdata.mode = m; }
		void dump(void);
		void Write(const char*);

		void OpenPropFile(const char*);
		const std::string Get(const std::string&, const std::string&);
		void Set(const std::string&, const std::string&);

		double Get(const std::string&, double);
		void Set(const std::string&, double);
		inline bool FileExist(const std::string& name)
		{
			std::ifstream file(name);
			if(!file)    //if the file was not found, then file is 0, i.e. !file=1 or true
				return false;    //the file was not found
			else         //if the file was found, then file is non-0
				return true;     //the file was found
		}
	};
}

#endif

