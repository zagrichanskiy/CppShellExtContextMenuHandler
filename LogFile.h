#pragma once

#include <string>
#include <map>
#include <cctype>
#include <Shlwapi.h>
#include <windows.h>

typedef		std::wstring	WSTR;
typedef		const WSTR		WCSTR;

struct FileInfo {
	DWORD size;
	SYSTEMTIME file_time;
};

struct comp : std::binary_function<WSTR, WSTR, bool>
{
	struct nocase_compare : public std::binary_function<wint_t, wint_t, bool>
	{
		bool operator() (const wint_t& c1, const wint_t& c2) const {
			return towlower(c1) < towlower(c2);
		}
	};
	bool operator() (const std::wstring & s1, const std::wstring & s2) const {
		return std::lexicographical_compare
			(s1.begin(), s1.end(),
			s2.begin(), s2.end(),
			nocase_compare());
	}
};

class LogFile {
public:
	void AddItem(LPWSTR _file_path);
	UINT WriteLog(PCWSTR _path);
private:
	std::map<WSTR, FileInfo, comp> files_sum;
};