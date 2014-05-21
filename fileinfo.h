// fileinfo.h
// classes to get, store and sort information about classes

#pragma once

#include <functional>
#include <string>
#include <windows.h>
#include <map>
#include <iomanip>
#include <algorithm>
#include <Shlwapi.h>
#include <cstring>
#include <future>
#include <numeric>
#include <sstream>
#include "logfile.h"
#include "ThreadPool.h"

typedef		std::wstring	WSTR;
typedef		const WSTR		WCSTR;
typedef		WCSTR &			WCRSTR;

// comp structure to do not case sensitive comparison in map
struct comp : std::binary_function<WSTR, WSTR, bool> {
	struct nocase_compare : public std::binary_function<wint_t, wint_t, bool> {
		bool operator() (const wint_t& c1, const wint_t& c2) const {
			return towlower(c1) < towlower(c2);
		}
	};
	bool operator() (WCSTR & s1, WCSTR & s2) const {
		return std::lexicographical_compare
			(s1.begin(), s1.end(),
			s2.begin(), s2.end(),
			nocase_compare());
	}
};

// FormatedSize gets size in bytes, formats it and stores in result string
class FormatedSize {
private:
	std::wstring result;
public:
	FormatedSize(DWORD size = 0);
	friend std::wostream & operator<<(std::wostream & wos, const FormatedSize & fs) {
		wos << fs.result.c_str();
		return wos;
	}
};

// FormatedTime object is possible to use with FileTimeToSystemTime(&CreationTime, &formated_time) funcion
class FormatedTime {
private:
	SYSTEMTIME time;
	std::wstring GetString() const;
public:
	PSYSTEMTIME operator&() { return &time; }
	friend std::wostream & operator<<(std::wostream & wos, const FormatedTime & ft) {
		wos << ft.GetString().c_str();
		return wos;
	}
};

// FileInfo gets and stores all needed information about file at file_path
class FileInfo {
private:
	std::wstring path;
	FormatedSize formated_size;
	FormatedTime formated_time;
	unsigned long long byte_sum;

	unsigned long long GetByteSum(LPCTSTR file_path) {
		std::ifstream input(file_path, std::ios_base::binary);
		const unsigned int BUFF_SIZE = 4096;
		unsigned char temp[BUFF_SIZE];
		unsigned long long sum = 0;
		while (input) {
			input.read((char *)temp, BUFF_SIZE);
			sum += std::accumulate(temp, temp + input.gcount(), 0);
		}
		return sum;
	}
public:
	FileInfo() {}
	explicit FileInfo(LPCTSTR file_path);
	friend std::wostream & operator<<(std::wostream & wos, const FileInfo & fi) {
		wos << std::setw(30) << std::left << fi.path
			<< std::setw(10) << std::right << fi.formated_size
			<< std::setw(20) << std::right << fi.formated_time
			<< "    " << fi.byte_sum;
		return wos;
	}
	FileInfo operator()() const { return *this; }
};

// filemap to store and sort files
class filemap : public std::map<WSTR, std::shared_future<FileInfo>, comp> {
public:
	typedef WSTR								_Kty;
	typedef std::shared_future<FileInfo>		_Ty;
	typedef comp								key_compare;
	typedef std::map<_Kty, _Ty, key_compare>	_Mybase;
	typedef std::pair<_Kty, _Ty>				_Mypair;
private:
	ThreadPool pool;

	using _Mybase::insert;
	using _Mybase::at;
	using _Mybase::operator[];
public:
	filemap() : pool(4) {}
	// write files information to log file
	template<typename _El>
	bool WriteLog(const _El * path) {
		logfile<wchar_t> output(path);
		std::for_each(begin(), end(), [&](const _Mypair & p){
			p.second.wait();
			output << newdata << std::setw(35) << std::left
				<< p.first << p.second.get() << std::endl; });
			return true;
	}
	// get file information and add it to map
	void insert(LPCTSTR file_path) {
		WCHAR file_name[MAX_PATH];
		wcscpy_s(file_name, file_path);
		PathStripPathW(file_name);
		insert(_Mypair(WSTR(file_name), pool.enqueue(FileInfo(file_path)).share()));
	}
};