#include "LogFile.h"
#include <strsafe.h>
#include <fstream>

void LogFile::AddItem(LPWSTR file_path) {
	FileInfo new_file;
	wchar_t name[MAX_PATH];
	wcscpy_s(name, file_path);
	PathStripPathW(name);
	if (files_sum.count(name) != 0) return;

	HANDLE hFile = CreateFile(file_path,
							  GENERIC_READ,
							  FILE_SHARE_READ,
							  NULL,
							  OPEN_EXISTING,
							  FILE_ATTRIBUTE_NORMAL,
							  NULL);
	new_file.size = GetFileSize(hFile, NULL);
	FILETIME CreationTime;
	GetFileTime(hFile, &CreationTime, NULL, NULL);
	FileTimeToSystemTime(&CreationTime, &new_file.file_time);

	files_sum.insert(std::make_pair(name, new_file));

	CloseHandle(hFile);
}

UINT LogFile::WriteLog(PCWSTR path) {
	std::wfstream output;
	output.open(path, std::ios::out | std::ios::trunc);
	if (!output.is_open()) return 1;

	wchar_t szMessage[300];
	std::map<WSTR, FileInfo>::iterator pos;
	for (pos = files_sum.begin(); pos != files_sum.end(); ++pos) {
		StringCchPrintfW(szMessage, ARRAYSIZE(szMessage),
			L"%-40s\t%02d.%02d.%4d %02d:%02d\t%ul bytes", pos->first.c_str(), 
			pos->second.file_time.wDay, pos->second.file_time.wMonth, pos->second.file_time.wYear,
			pos->second.file_time.wHour, pos->second.file_time.wMinute,
			pos->second.size);

		output << szMessage << std::endl;
	}
	output << L"Sum is " << files_sum.size() << std::endl;
	output.close();

	return 0;
}