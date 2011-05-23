#ifndef _WIN32_H_
#define _WIN32_H_

#include <glibmm/ustring.h>
#include <gtkmm/filechooserdialog.h>
#include <windows.h>

const DWORD MAX_FILENAME = 512;

class Win32FileDialog : public Gtk::FileChooser
{
public:
	Win32FileDialog(const Glib::ustring& title, Gtk::FileChooserAction action);
	~Win32FileDialog();

	Glib::ustring get_filename() const;

	int run();

	void add_filter(const Glib::ustring& name, const Glib::ustring& pattern);

	void set_def_ext(const Glib::ustring& ext);

private:
	wchar_t m_Filename[MAX_FILENAME];
	OPENFILENAMEW m_ofnw;
	gunichar2 *m_lpTitle;
	gunichar2 *m_lpDefExt;
	std::vector<wchar_t> m_Filter;
	Gtk::FileChooserAction m_Action;
};

BOOL Win32_OpenFileDialog(LPTSTR filename, DWORD maxname);
BOOL Win32_SaveFileDialog(LPTSTR filename, DWORD maxname);

#endif