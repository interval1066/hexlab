#ifndef GCONFCLIENT_H
#define GCONFCLIENT_H

#include <gconfmm.h>
#include <string>

namespace config
{
	class ConfClient
	{
		std::string m_path;
		Glib::RefPtr<Gnome::Conf::Client> m_client;

	public:
		ConfClient(std::string);
		virtual ~ConfClient() {}
		inline const std::string getPath() { return m_path; }
		inline void setPath(const std::string path) { m_path = path; }

		std::string format_string(std::string);
		bool getbValue(std::string, bool);
		void setValue(bool, std::string);

		int getiValue(std::string, int);
		void setValue(int, std::string);
		std::string getsValue(std::string, std::string);

		void setValue(std::string, std::string);
		void setValue(float, std::string);
		float getfValue(std::string, float);
	};
}

#endif
