#include <QCoreApplication>
#include "SingleApplication.h"
int main(int argc,char ** argv){
	
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true); // ���ø�DPI
	SingleApplication a(argc, argv);


	
	return 0;
}