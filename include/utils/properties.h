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

enum MODE
{ PRP_WRITEONLY, PRP_READONLY, PRP_READWRITE };

typedef std::unordered_map<std::string, std::string>::key_type props_key;
typedef std::unordered_map <std::string, std::string> MAP;

typedef struct PROPDATA
{
	MAP _props;
#ifdef _REENTRANT
	volatile int _mode;
	std::mutex _proplock;
#else
	int _mode;
#endif
	std::string _file;
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
		std::mutex _propmutex;
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

	public:
		Properties(int);
		Properties(const char*, int);
		virtual ~Properties(void);

		inline void SetMode(int m) { _propdata._mode = m; }
		void dump(void);
		void Write(const char*);

		void OpenPropFile(const char*);
		std::string Get(const std::string&, const std::string&);
		void Set(const std::string&, const std::string&);

		int Get(const std::string&, int);
		void Set(const std::string&, int);
		unsigned int Get(const std::string&, unsigned int);

		void Set(const std::string&, unsigned int);
		long Get(const std::string&, long);
		void Set(const std::string&, long);

		double Get(const std::string&, double);
		void Set(const std::string&, double);
		float Get(const std::string&, float);

		void Set(const std::string&, float);
	};
}

#endif

