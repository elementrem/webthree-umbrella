#include "AleleZero.h"
#include <QtWidgets/QApplication>

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
	dev::alele::zero::AleleZero w;
	w.show();
	
	return a.exec();
}
