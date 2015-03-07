/** $Id: examplewindow.h 705 2013-07-19 02:55:32Z toneil $ -*- c++ -*-
 *
 *
 */

#ifndef HEXLAB_PROJECT_H
#define HEXLAB_PROJECT_H

typedef unsigned char BYTE;

namespace project
{
	typedef struct
	{
		bool					changed;
		std::string			filepath;
		std::string			filename;
		long long			byteoffset;
		BYTE					offset_type;
		int					file_type;
		BYTE**				buffer;
	} HEXFILE;
}

#endif

