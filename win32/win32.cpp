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

Win32FileDialog::Win32FileDialog(const Glib::ustring& title, Gtk::FileChooserAction action)
	:m_lpTitle(NULL)
	,m_lpDefExt(NULL)
	,m_Action(action)
{
	m_lpTitle = g_utf8_to_utf16(title.c_str(), -1, 0, 0, 0);

	m_Filename[0] = L'\0';
	ZeroMemory(&m_ofnw, sizeof(OPENFILENAMEW));
	m_ofnw.lStructSize = sizeof(OPENFILENAMEW);
	m_ofnw.hwndOwner = NULL;
	m_ofnw.hInstance = GetModuleHandle(NULL);
	m_ofnw.lpstrFilter = NULL;
	m_ofnw.lpstrCustomFilter = 0;
	m_ofnw.nMaxCustFilter = 0;
	m_ofnw.nFilterIndex = 0;
	m_ofnw.lpstrFile = m_Filename;
	m_ofnw.nMaxFile = MAX_FILENAME;
	m_ofnw.lpstrFileTitle = NULL;
	m_ofnw.nMaxFileTitle = 0;
	m_ofnw.lpstrInitialDir = NULL;
	m_ofnw.lpstrTitle = reinterpret_cast<LPCWSTR>(m_lpTitle);
	m_ofnw.Flags = (action == Gtk::FILE_CHOOSER_ACTION_OPEN) ? (OFN_FILEMUSTEXIST | OFN_HIDEREADONLY) : 0;
	m_ofnw.nFileOffset = 0;
	m_ofnw.nFileExtension = 0;
	m_ofnw.lpstrDefExt = NULL;
	m_ofnw.lCustData = 0;
	m_ofnw.lpfnHook = 0;
	m_ofnw.lpTemplateName = 0;
}

Win32FileDialog::~Win32FileDialog()
{
	if(m_lpTitle)
	{
		g_free(m_lpTitle);
	}
	if(m_lpDefExt)
	{
		g_free(m_lpDefExt);
	}
}

Glib::ustring Win32FileDialog::get_filename() const
{
	gunichar2 const* utf16 = reinterpret_cast<gunichar2 const*>(m_Filename);
	gchar* utf8 = g_utf16_to_utf8(utf16, -1, 0, 0, 0);
	Glib::ustring u(utf8);
	g_free(utf8);
	return u;
}

void Win32FileDialog::add_filter(const Glib::ustring& name, const Glib::ustring& pattern)
{
	gunichar2 *name_utf16 = g_utf8_to_utf16(name.c_str(), -1, 0, 0, 0);
	unsigned long index = 0;
	for(; name_utf16[index]; ++ index)
	{
		m_Filter.push_back(name_utf16[index]);
	}
	m_Filter.push_back(L'\0');
	g_free(name_utf16);
	gunichar2 *pattern_utf16 = g_utf8_to_utf16(pattern.c_str(), -1, 0, 0, 0);
	for(index = 0; pattern_utf16[index]; ++ index)
	{
		m_Filter.push_back(pattern_utf16[index]);
	}
	m_Filter.push_back(L'\0');
	g_free(pattern_utf16);
}

void Win32FileDialog::set_def_ext(const Glib::ustring& ext)
{
	if(m_lpDefExt)
	{
		g_free(m_lpDefExt);
	}
	m_lpDefExt = g_utf8_to_utf16(ext.c_str(), -1, 0, 0, 0);
}

int Win32FileDialog::run()
{
	m_Filter.push_back(L'\0');
	if(! m_Filter.empty())
	{
		m_ofnw.lpstrFilter = &(m_Filter[0]);
	}
	m_ofnw.lpstrDefExt = reinterpret_cast<LPCWSTR>(m_lpDefExt);

	BOOL res;

	if(m_Action == Gtk::FILE_CHOOSER_ACTION_SAVE)
	{
		res = GetSaveFileNameW(&m_ofnw);
	}
	else
	{
		res = GetOpenFileNameW(&m_ofnw);
	}

	if(res)
	{
		return Gtk::RESPONSE_YES;
	}
	return Gtk::RESPONSE_NONE;
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