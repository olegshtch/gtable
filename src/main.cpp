#include <gtkmm/main.h>
#include <glibmm/thread.h>
#include <gtkmm/messagedialog.h>
#include "main_window.h"
#include "shared.h"

// ToDo:
// 	редактирование условий
// 	* Связать сущности и модели
// 	* ScrolledWindow для OrderDialog
// 	* Запрет на отрицательные и нулевые индексы в зависимых полях
// 	* Объединение виджетов для отображения моделей в шаблонный класс
// 	решение
// 	* построение списков соответствия идентификаторов и индексов в математической модели
// 	* создание входных массивов данных
// 	* построение оптимизированной нейросети
// 	* алгоритм
// 	редактирование расписания
// 	* таблица хранения расписания
// 	* виджет ячейки с элементов расписания
//

int main(int argc, char **argv)
{
	Gtk::Main prog(argc, argv);
	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, TABLE_LOCALEDIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
	Glib::thread_init();
	try
	{
		MainWindow main_window;
		prog.run(main_window);
	}
	catch(Glib::Error &err)
	{
		Gtk::MessageDialog(err.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
	}
	catch(std::exception &err)
	{
		Gtk::MessageDialog(err.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
	}

	return 0;
}

