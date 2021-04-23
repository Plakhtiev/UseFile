#include <iostream>
#include <windows.h>

void ErrorMessage() {
	setlocale(LC_ALL, "ru");
	DWORD errorCode = GetLastError();
	CHAR      buff[80];

	FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buff,
		(sizeof(buff) / sizeof(CHAR)),
		NULL);
	throw std::exception(buff);
}

void CopyFile(LPCWSTR path_from, LPCWSTR  path_to) {
	HANDLE fileSourse;
	HANDLE fileCopy;
	DWORD  dwBytesRead, dwBytesWritten, dwPos;
	BYTE   buff[4096];

	fileSourse = CreateFile(path_from,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (fileSourse == INVALID_HANDLE_VALUE)
	{
		try {
			ErrorMessage();
		}
		catch (std::exception ex) {
			std::cout << ex.what();
		}
	}

	fileCopy = CreateFile(path_to,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (fileCopy == INVALID_HANDLE_VALUE)
	{
		try {
			ErrorMessage();
		}
		catch (std::exception ex) {
			std::cout << ex.what();
		}
	}

	while (ReadFile(fileSourse, buff, sizeof(buff), &dwBytesRead, NULL)
		&& dwBytesRead > 0)
	{
		dwPos = SetFilePointer(fileCopy, 0, NULL, FILE_CURRENT);
		LockFile(fileCopy, dwPos, 0, dwBytesRead, 0);
		WriteFile(fileCopy, buff, dwBytesRead, &dwBytesWritten, NULL);
		UnlockFile(fileCopy, dwPos, 0, dwBytesRead, 0);
	}

	CloseHandle(fileSourse);
	CloseHandle(fileCopy);
}

int main()
{
	CopyFile(L"F:/D/cpp/one.txt", L"F:/D/cpp/oneC.txt");
}