// logfile.h
// logfile class for easy and automatic store information in log file

#pragma once
#ifndef LOG_FILE_H_
#define LOG_FILE_H_

#include <fstream>
#include <string>
#include <ctime>
#include <chrono>

template<class _Elem>
class logfile : private std::basic_ofstream<_Elem> {
private:
	static const char * START_LOG;
	static const char * END_LOG;
	unsigned int counter;
public:
	// constructors, operator=()
	logfile() : std::basic_ofstream<_Elem>() {}
	template<class _El>
	explicit logfile(const _El * filename);
	template<class _El>
	explicit logfile(const std::basic_string<_El> & filename);
	logfile(logfile && lf) : std::basic_ofstream<_Elem>(std::move(lf)), counter(lf.counter) {}
	logfile(const logfile &) = delete;
	logfile & operator=(const logfile &) = delete;
	logfile & operator=(logfile && lf);
	~logfile();

	// open()
	template<class _El>
	void open(const _El * filename);
	template<class _El>
	void open(const std::basic_string<_El> & filename);
	using std::basic_ofstream<_Elem>::is_open;
	using std::basic_ofstream<_Elem>::close;

	// operator<<()
	template<typename _Val>
	logfile & operator<<(const _Val & val) {
		static_cast<std::basic_ostream<_Elem> &>(*this) << val;
		return *this;
	}
	logfile & operator<<(std::basic_ostream<_Elem> & (*manip)(std::basic_ostream<_Elem> &)) {
		static_cast<std::basic_ostream<_Elem> &>(*this) << manip;
		return *this;
	}
	logfile & operator<<(logfile & (*manip)(logfile &)) { return manip(*this); }

	// manipulator
	friend logfile & newdata(logfile & lf) {
		lf.counter++;
		lf << std::right << std::setw(2) << lf.counter << ") ";
		return lf;
	}
};

// strings
template<class _Elem>
const char * logfile<_Elem>::START_LOG = 
	">----------------------------------------------------------- START LOG ----------------------------------------------------------->";
template<class _Elem>
const char * logfile<_Elem>::END_LOG =
	">------------------------------------------------------------ END LOG ------------------------------------------------------------>";

// constructors, operator=
template<class _Elem>
template<class _El>
logfile<_Elem>::logfile(const _El * filename) : std::basic_ofstream<_Elem>(filename), counter(0) {
	using std::chrono::system_clock;
	system_clock::time_point time_now = system_clock::now();
	std::time_t time_now_t = system_clock::to_time_t(time_now);
	
	*this << "Name of logfile: " << filename << std::endl
		  << "Creation time: " << ctime(&time_now_t) << std::endl
		  << "Srart of log:\n" << START_LOG << std::endl;
}

template<class _Elem>
template<class _El>
logfile<_Elem>::logfile(const std::basic_string<_El> & filename) : logfile(filename.c_str()) {}

template<class _Elem>
logfile<_Elem> & logfile<_Elem>::operator=(logfile && lf) {
	std::basic_ofstream<_Elem>::operator=(std::move(lf));
	counter = lf.counter;
	return *this;
}

// open
template<class _Elem>
template<class _El>
void logfile<_Elem>::open(const _El * filename) {
	counter = 0;
	using std::chrono::system_clock;
	system_clock::time_point time_now = system_clock::now();
	std::time_t time_now_t = system_clock::to_time_t(time_now);
	
	*this << "Name of logfile: " << filename << std::endl
		  << "Creation time: " << ctime(&time_now_t) << std::endl
		  << "Srart of log:\n" << START_LOG << std::endl;
}

template<class _Elem>
template<class _El>
void logfile<_Elem>::open(const std::basic_string<_El> & filename) { open(filename.c_str()); }

// destructor
template<class _Elem>
logfile<_Elem>::~logfile() {
	*this << END_LOG << std::endl
		  << "Number of logged files: " << counter << std::endl;
}

#endif