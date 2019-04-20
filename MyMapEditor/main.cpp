
#include <QtGui/QApplication>
#include "QtGUI/MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	// …Ë÷√∫∫◊÷±‡¬Î
	QTextCodec *codec = QTextCodec::codecForName("GB2312");  
	QTextCodec::setCodecForLocale(codec);  
	QTextCodec::setCodecForCStrings(codec);  
	QTextCodec::setCodecForTr(codec);

	MapEditor w;
	w.show();
	return a.exec();

}