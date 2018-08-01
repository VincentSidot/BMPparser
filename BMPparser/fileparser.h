#pragma once
#include <Windows.h>


class File
{
public:

	File(LPCSTR lpFileName)
	{
		hFile = CreateFileA(lpFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	~File()
	{
		CloseHandle(hFile);
	}

	template<class T>
	T read(LONG addr) const
	{
		T rep;
		DWORD readed;
		SetFilePointer(hFile, addr, 0, FILE_BEGIN);
		ReadFile(hFile, &rep, sizeof(rep), &readed, NULL);
		return rep;
	}

	template<class T>
	void write(LONG addr, T value)
	{
		DWORD written;
		SetFilePointer(hFile, addr, 0, FILE_BEGIN);
		WriteFile(hFile, &value, sizeof(value), &written, NULL);
	}

	static void copy(LPCSTR in, LPCSTR out)
	{
		CopyFile(in, out, FALSE);
	}

protected:
	HANDLE hFile;
	LPCSTR hFileName;

};