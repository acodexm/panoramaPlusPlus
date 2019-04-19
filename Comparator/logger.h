#pragma once
#ifndef LOG_H
#define LOG_H
#define NOMINMAX
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "windows.h"

class LOG
{
public:
	typedef std::ostream& (*ManipFn)(std::ostream&);
	typedef std::ios_base& (*FlagsFn)(std::ios_base&);

	enum LogLevel
	{
		DEBUG,
		INFO,
		WARN,
		ERR
	};

	LOG() : m_logLevel(INFO) {}

	template<class T>  // int, double, strings, etc
	LOG& operator<<(const T& output)
	{
		m_stream << output;
		return *this;
	}

	LOG& operator<<(ManipFn manip) /// endl, flush, setw, setfill, etc.
	{
		manip(m_stream);

		if (manip == static_cast<ManipFn>(std::flush)
			|| manip == static_cast<ManipFn>(std::endl))
			this->flush();

		return *this;
	}

	LOG& operator<<(FlagsFn manip) /// setiosflags, resetiosflags
	{

		manip(m_stream);
		return *this;
	}

	LOG& operator()(LogLevel e)
	{

		m_logLevel = e;
		return *this;
	}

	void flush()
	{
		// m_stream.str() has your full message here.
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
		DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

		std::ofstream logfile;
		logfile.open("log.txt", std::ofstream::out | std::ofstream::app);
		logfile << "VirMem: " << totalVirtualMem << " RAM: " << physMemUsed << " in: " << m_stream.str();
		logfile.close();

		std::cout << "VirMem: " << totalVirtualMem << " RAM: " << physMemUsed << " in: " << m_stream.str();
		m_logLevel = INFO;

		m_stream.str(std::string());
		m_stream.clear();
	}

private:
	std::stringstream  m_stream;
	int                m_logLevel;
};
#endif