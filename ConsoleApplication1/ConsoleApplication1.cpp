// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Windows.h>
#include <string>
#include <iostream>

using namespace std;
char * UnicodeToUTF8(const wchar_t* str)
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	result = (char *)malloc((textlen + 1) * sizeof(char));
	memset(result, 0, sizeof(char) * (textlen + 1));
	WideCharToMultiByte(CP_UTF8, 0, str, -1, result, textlen, NULL, NULL);
	return result;
}

bool SaveToFile(char * pPathFile, wstring strFile)
{
	FILE *filepath = fopen(pPathFile, "w+");

	if (filepath == NULL)
	{
		return false;
	}
	const wchar_t* strfilew = strFile.c_str();

	string strfileUTF;
	strfileUTF = UnicodeToUTF8(strfilew);

	fwrite(strfileUTF.c_str(), strfileUTF.length(), 1, filepath);

	fclose(filepath);

	return true;
}

int main()
{
	HMODULE hModule = NULL;

	hModule = LoadLibrary("ChromePlugin.dll");

	typedef void* (__cdecl*pendd)(DWORD a);

	pendd pencrypt = pendd(GetProcAddress(hModule, "ReadChromeLoginData"));


	DWORD a = 473416;
	 pencrypt(a);

	system("pause");

	return 0;
}

