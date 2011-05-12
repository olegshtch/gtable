#include "win32.h"

int main(int argc, char **argv);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
)
{
	char *argv[2] = {"gtable.exe", NULL};
	//wchar_t filename[512];
	//Win32_OpenFileDialog(filename, 512);
	return main(1, argv);
}

BOOL Win32_OpenFileDialog(LPTSTR filename, DWORD maxname)
{
	OPENFILENAMEW ofnw;
	ZeroMemory(&ofnw, sizeof(OPENFILENAMEW));
	ofnw.lStructSize = sizeof(OPENFILENAMEW);
	ofnw.hwndOwner = NULL;
	ofnw.hInstance = GetModuleHandle(0);
	ofnw.lpstrFilter = L"Расписание\0*.tbl\0Всё файлы\0*.*\0\0";
	ofnw.lpstrCustomFilter = 0;
	ofnw.nMaxCustFilter = 0;
	ofnw.nFilterIndex = 0;
	ofnw.lpstrFile = filename;
	ofnw.nMaxFile = maxname;
	ofnw.lpstrFileTitle = NULL;
	ofnw.nMaxFileTitle = 0;
	ofnw.lpstrInitialDir = NULL;
	ofnw.lpstrTitle = L"Выберите файл расписания";
	ofnw.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofnw.nFileOffset = 0;
	ofnw.nFileExtension = 0;
	ofnw.lpstrDefExt = L"tbl";
	ofnw.lCustData = 0;
	ofnw.lpfnHook = 0;
	ofnw.lpTemplateName = 0;
	return GetOpenFileNameW(&ofnw);
}

BOOL Win32_SaveFileDialog(LPTSTR filename, DWORD maxname)
{
	OPENFILENAMEW ofnw;
	ZeroMemory(&ofnw, sizeof(OPENFILENAMEW));
	ofnw.lStructSize = sizeof(OPENFILENAMEW);
	ofnw.hwndOwner = NULL;
	ofnw.hInstance = GetModuleHandle(0);
	ofnw.lpstrFilter = L"Расписание\0*.tbl\0Всё файлы\0*.*\0\0";
	ofnw.lpstrCustomFilter = 0;
	ofnw.nMaxCustFilter = 0;
	ofnw.nFilterIndex = 0;
	ofnw.lpstrFile = filename;
	ofnw.nMaxFile = maxname;
	ofnw.lpstrFileTitle = NULL;
	ofnw.nMaxFileTitle = 0;
	ofnw.lpstrInitialDir = NULL;
	ofnw.lpstrTitle = L"Выберите файл расписания";
	ofnw.Flags = 0;
	ofnw.nFileOffset = 0;
	ofnw.nFileExtension = 0;
	ofnw.lpstrDefExt = L"tbl";
	ofnw.lCustData = 0;
	ofnw.lpfnHook = 0;
	ofnw.lpTemplateName = 0;
	return GetSaveFileNameW(&ofnw);
}