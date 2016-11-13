#include "main_window.hpp"

#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QtWidgets>

#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	main_window win;

	win.show();
	return a.exec();
}
