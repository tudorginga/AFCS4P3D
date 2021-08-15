#include <QtWidgets/QApplication>

#include "AFCS_UI.h"

int main(int argc, char *argv[])
{
	QApplication A(argc, argv);
	AFCS_UI UI;
	UI.show();
	return A.exec();
}
