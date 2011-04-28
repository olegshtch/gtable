#include <iostream>
#include <cstdlib>
#include "logbuf.h"

LogBuf *LogBuf::log_buffer = new LogBuf();

LogBuf::LogBuf()
{
	backup = std::clog.rdbuf();
	std::clog.rdbuf(this);

	atexit(DestroyLogBuf);
}

LogBuf::~LogBuf()
{
	std::cout.flush();
	std::clog.rdbuf(backup);
}

int LogBuf::overflow(int c)
{
	if(c != -1)
	{
		char ch = c;
		//std::cout << "Overflow ";
#ifndef WIN32
		std::cout.write("\x1b[32m", 5);
		std::cout.write(&ch, 1);
		std::cout.write("\x1b[0m", 4);
#endif
	}
	return traits_type::not_eof(c);
}

int LogBuf::sync()
{
	//std::cout << "Flush" << std::endl;
#ifndef WIN32
	std::cout.flush();
#endif
	return 0;
}

std::streamsize LogBuf::xsputn(const char *s, std::streamsize n)
{
	//std::cout << "Out n=" << n << " s=\"";
#ifndef WIN32
	std::cout.write("\x1b[32m", 5);
	std::cout.write(s, n);
	std::cout.write("\x1b[0m", 4);
#endif
	//std::cout << "\"" << std::endl;
	return n;
}

void LogBuf::DestroyLogBuf() ///< Callback for desctructor.
{
	delete log_buffer;
}


