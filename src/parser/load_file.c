#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <windows.h>
	#include <tchar.h>
#endif

#include "load_file.h"

void texan_write_file(const char * filename, const char * data, size_t length) {
	printf("writing file %s\n", filename);

#ifdef _WIN32
	HANDLE hFile;
	DWORD dwBytesToWrite = (DWORD)length;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;

	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), filename);
		return;
	}

	_tprintf(TEXT("Writing %d bytes to %s.\n"), dwBytesToWrite, filename);

	bErrorFlag = WriteFile(hFile, data, dwBytesToWrite, &dwBytesWritten, NULL);
	if (FALSE == bErrorFlag)
	{
		printf("Terminal failure: Unable to write to file.\n");
	}
	else
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
			_tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten, filename);
		}
	}

	CloseHandle(hFile);
#endif
}

char * texan_load_file(const char * filename) {
	printf("loading file %s\n", filename);

#ifdef _WIN32
	HANDLE hMapFile;
	LPCTSTR pBuf;
	DWORD hFileSize;
	DWORD hNBytesRead;
	OFSTRUCT hFileInfo;

	hMapFile = OpenFile(
		filename,
		&hFileInfo,
		OF_READ
	);

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return NULL;
	}

	hFileSize = GetFileSize(
		hMapFile,
		NULL
	);

	printf("filesize: %d\n", hFileSize);

	pBuf = (LPCTSTR)malloc(sizeof(LPCSTR) * hFileSize);

	BOOL result = ReadFile(
		hMapFile,
		pBuf,
		hFileSize,
		&hNBytesRead,
		0
	);

	if (!result || pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return NULL;
	}

	result = CloseHandle(
		hMapFile
	);

	if (!result) {
		free(pBuf);

		_tprintf(TEXT("Could not close file (%d).\n"),
			GetLastError());

		return NULL;
	}

	char * data = (char *)(pBuf);
	data[hFileSize + 1] = '\0';

	return data;
#endif
}