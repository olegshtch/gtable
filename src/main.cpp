#include <gtkmm/main.h>
#include <glibmm/thread.h>
#include <gtkmm/messagedialog.h>
#include "main_window.h"
#include "shared.h"

int main(int argc, char **argv)
{
	Gtk::Main prog(argc, argv);
	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, TABLE_LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
	Glib::thread_init();
	MainWindow *p_main_window = 0;
	try
	{
		Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
		refBuilder->add_from_file(UIDIR "main_window.glade");

		refBuilder->get_widget_derived("MainWindow", p_main_window);
		if(p_main_window)
		{
			prog.run(*p_main_window);
		}
	}
	catch(Gtk::BuilderError &err)
	{
		std::cerr << "Gtk::BuilderError: " << err.what() << std::endl;
		Gtk::MessageDialog dialog(err.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		dialog.run();
	}
	catch(Glib::Error &err)
	{
		std::cerr << "Glib::Error: " << err.what() << std::endl;
		Gtk::MessageDialog dialog(err.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		dialog.run();
	}
	catch(std::exception &err)
	{
		std::cerr << "std::exception: " << err.what() << std::endl;
		Gtk::MessageDialog dialog(err.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		dialog.run();
	}

	if(p_main_window)
	{
		delete p_main_window;
	}

	return 0;
}

