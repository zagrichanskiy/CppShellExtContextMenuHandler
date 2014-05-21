#include "fileinfo.h"

FormatedSize::FormatedSize(DWORD size) {
	const DWORD dwKB = 1024;          // Kilobyte
	const DWORD dwMB = 1024 * dwKB;   // Megabyte
	const DWORD dwGB = 1024 * dwMB;   // Gigabyte
	std::wostringstream temp;
	temp.precision(2);
	temp << std::fixed << std::setw(6);

	if (size < dwKB)
		temp << (double)size << L" " << std::setw(2) << std::right << L"B" << std::ends;
	else if (size < dwMB)
		temp << (double)size / dwKB << L" " << std::setw(2) << "KB" << std::ends;
	else if (size < dwGB)
		temp << (double)size / dwMB << L" " << std::setw(2) << "MB" << std::ends;
	else
		temp << (double)size / dwGB << L" " << std::setw(2) << "GB" << std::ends;

	result = temp.str();
}

std::wstring FormatedTime::GetString() const {
	std::wstringstream result;
	result << std::setw(2) << std::setfill(L'0') << time.wDay << '.'
		<< std::setw(2) << std::setfill(L'0') << time.wMonth << '.'
		<< time.wYear << ' '
		<< std::setw(2) << time.wHour << ':'
		<< std::setw(2) << std::setfill(L'0') << time.wMinute;
	return result.str();
}

FileInfo::FileInfo(LPCTSTR file_path) : path(file_path) {
	path.resize(path.rfind(L'\\') + 1);

	HANDLE hFile = CreateFile(file_path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	formated_size = GetFileSize(hFile, NULL);

	FILETIME CreationTime;
	GetFileTime(hFile, &CreationTime, NULL, NULL);
	FileTimeToSystemTime(&CreationTime, &formated_time);
	byte_sum = GetByteSum(file_path);
	CloseHandle(hFile);
}