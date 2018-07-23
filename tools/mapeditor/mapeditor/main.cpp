#include "mapeditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mapeditor w;
	w.show();
	return a.exec();
}
