#include <iostream>
#include <windows.h>
/*Дз. Написать приложение, которое принимает на вход 2 пути:
path_from и path_to.
дальше приложение должно сделать копию файла path_from в path_to используя системные API*/

/*Реализовать класс WinException. Данный класс должен внутри получать системный код ошибки через GetLastError и сообщение об ошибке через FormatMessageA
Реализовать функцию, которая будет вызываться из main и создавать файл используя CreateFile API. В случае ошибки, функция должна выбрасывать WinException, который будет перехватываться в main и выводиться пользователю в консоль.*/

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

	fileSourse = CreateFile(path_from, // open One.txt
		GENERIC_READ,             // open for reading
		0,                        // do not share
		NULL,                     // no security
		OPEN_EXISTING,            // existing file only
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

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
		GENERIC_WRITE,         // open for writing
		FILE_SHARE_READ,          // allow multiple readers
		NULL,                     // no security
		CREATE_ALWAYS,              // open or create
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

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