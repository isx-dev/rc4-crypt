// rc4_crypt.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "rc4.hpp"

BOOL ReadFileToBuffer(_In_ LPCTSTR lpFileName, _Out_ LPVOID *lppvBuffer, _Out_opt_ LPDWORD lpdwFileSize)
{
	HANDLE hFile;
	DWORD dwFileSize;
	DWORD dwNumberOfBytesRead;

	LPVOID lpBuffer;

	hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateFile GLE=%08X\n"), GetLastError());
		return FALSE;
	}

	dwFileSize = GetFileSize(hFile, NULL);
	lpBuffer = VirtualAlloc(NULL, dwFileSize, MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!lpBuffer)
	{
		_tprintf(_T("VirtualAlloc GLE=%08X\n"), GetLastError());
		return FALSE;
	}

	VirtualAlloc(lpBuffer, dwFileSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!ReadFile(hFile, lpBuffer, dwFileSize, &dwNumberOfBytesRead, NULL))
	{
		_tprintf(_T("ReadFile GLE=%08X\n"), GetLastError());

		VirtualFree(lpBuffer, 0, MEM_RELEASE);
		return FALSE;
	}

	if (lpdwFileSize != NULL)
		*lpdwFileSize = dwFileSize;

	*lppvBuffer = lpBuffer;
	return TRUE;
}

BOOL CreateFileFromBuffer(_In_ LPCTSTR lpFileName, _In_reads_bytes_(nSize) LPCVOID lpBuffer, _In_ SIZE_T nSize)
{
	DWORD	dwNumberOfBytesToWritten;
	DWORD	dwRestOfSize;
	HANDLE	hFile;
	LPBYTE	pBuffer;
	BOOL	bResult;

	hFile = CreateFile(lpFileName,
		GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	bResult = FALSE;

	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwNumberOfBytesToWritten = 0;
		dwRestOfSize = nSize;
		pBuffer = (LPBYTE)lpBuffer;

		while (TRUE)
		{
			WriteFile(hFile, pBuffer, dwRestOfSize, &dwNumberOfBytesToWritten, NULL);
			if (dwNumberOfBytesToWritten <= 0)
				break;

			pBuffer += dwNumberOfBytesToWritten;
			dwRestOfSize -= dwNumberOfBytesToWritten;

			if (!bResult)
				bResult = TRUE;
		}

		CloseHandle(hFile);
	}

	return bResult;
}

int _tmain(int argc, _TCHAR* argv[])
{	
	unsigned char pbKey[] = {
		0x10, 0xD4, 0x05, 0x4F, 0x07, 0xB1, 0x11, 0x94, 0x06, 0xEB, 0x30, 0xA1, 0x82, 0x28, 0x08, 0x48,
		0x83, 0x1F, 0x0A, 0xE9, 0x03, 0x82, 0x1F, 0xA2, 0x92, 0xF3, 0xDB, 0xAC, 0x54, 0x21, 0xF5, 0xB4,
		0x8E, 0xE3, 0xF3, 0x48, 0xF3, 0x63, 0x13, 0x55, 0x70, 0xB7, 0x1F, 0x98, 0xDA, 0xE2, 0x3D, 0x24,
		0x8F, 0xE1, 0x61, 0xF3, 0xF8, 0x98, 0xDB, 0x87, 0x7E, 0x6E, 0xFA, 0xB9, 0x45, 0xC2, 0xDF, 0xD3,
		0xBD, 0xC5, 0xFE, 0x9C, 0xE7, 0x5A, 0x3C, 0x78, 0x57, 0x3E, 0x9F, 0x47, 0xB2, 0x23, 0x9E, 0x6D,
		0x9B, 0x0D, 0x4B, 0x66, 0x85, 0xFD, 0x74, 0x4F, 0x4A, 0x2A, 0xE8, 0x3C, 0x98, 0x12, 0x16, 0x50,
		0x36, 0xAA, 0xFE, 0x8F, 0xE1, 0x15, 0x10, 0x37, 0x56, 0x1A, 0x16, 0x5C, 0x35, 0x9C, 0x89, 0x18,
		0xD1, 0xC2, 0xE7, 0x64, 0x33, 0x8C, 0xE5, 0xE4, 0xAA, 0xB8, 0x7F, 0x78, 0xC4, 0x4E, 0xCE, 0x08,
		0x50, 0x8E, 0xB5, 0xD4, 0xAF, 0x6E, 0x74, 0x37, 0xE5, 0xEA, 0x74, 0x85, 0xDE, 0x9A, 0x97, 0xC2,
		0xF7, 0xE9, 0x90, 0x3F, 0xC1, 0xD8, 0x34, 0x9D, 0xE5, 0x13, 0xB8, 0xEA, 0x86, 0x03, 0x9B, 0xD4,
		0xFA, 0xE0, 0x37, 0x00, 0x20, 0xE1, 0x66, 0x39, 0x37, 0xA6, 0xE0, 0xAC, 0x28, 0xE4, 0x80, 0xC4,
		0x68, 0x41, 0xC1, 0x7B, 0x2A, 0x08, 0xA7, 0x47, 0xBE, 0x62, 0x8C, 0x13, 0xA5, 0x8E, 0x42, 0x53,
		0xBC, 0x08, 0xFE, 0x05, 0x15, 0x86, 0xAE, 0x28, 0xC9, 0x48, 0xA3, 0xE6, 0x60, 0xAB, 0x94, 0x16,
		0xE1, 0x19, 0x2D, 0x35, 0xE3, 0x43, 0x61, 0x5C, 0xA7, 0xF3, 0x30, 0xE6, 0x4F, 0x6F, 0x8E, 0x31,
		0x01, 0x81, 0x5A, 0x06, 0xB2, 0xF5, 0xD5, 0x20, 0x19, 0x7C, 0xDD, 0xA0, 0x8F, 0x37, 0x6D, 0xF8,
		0xD7, 0x5D, 0xD2, 0xBE, 0xA9, 0x10, 0x9D, 0x72, 0x09, 0x97, 0x6D, 0x79, 0xCC, 0xAF, 0xB4, 0x4F,
	};

	LPVOID lpBuffer;
	DWORD  dwFileSize;

	if (argc <= 3)
	{
		_tprintf(_T("[utage]\n")
				 _T("\trc4_crypt.exe -d [filename] [output_filename]\t\t<= decrypt file\n")
				 _T("\trc4_crypt.exe -e [filename] [output_filename]\t\t<= encrypt file\n"));
	}
	else
	{
		if (ReadFileToBuffer(argv[2], &lpBuffer, &dwFileSize))
		{
			if (!_tcscmp(argv[1], _T("-d")))
			{
				rc4_decrypt_ALL(pbKey, sizeof(pbKey), (unsigned char*)lpBuffer, (unsigned char*)lpBuffer, dwFileSize);

				if (PIMAGE_DOS_HEADER(lpBuffer)->e_magic != IMAGE_DOS_SIGNATURE)
				{
					_tprintf(_T("Failed to decrypted...\n"));
					VirtualFree(lpBuffer, 0, MEM_RELEASE);
					return -1;
				}

				if (CreateFileFromBuffer(argv[3], lpBuffer, dwFileSize))
				{
					_tprintf(_T("decrypted file!\n"));
					VirtualFree(lpBuffer, 0, MEM_RELEASE);
					return 0;
				}
			}
			else if (!_tcscmp(argv[1], _T("-e")))
			{
				rc4_encrypt_ALL(pbKey, sizeof(pbKey), (unsigned char*)lpBuffer, (unsigned char*)lpBuffer, dwFileSize);

				if (CreateFileFromBuffer(argv[3], lpBuffer, dwFileSize))
				{
					_tprintf(_T("encrypted file!\n"));
					VirtualFree(lpBuffer, 0, MEM_RELEASE);
					return 0;
				}
			}

			VirtualFree(lpBuffer, 0, MEM_RELEASE);
		}
	}

	return -1;
}

