#include "pictureviewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
//    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	PictureViewer w;
	w.show();
    qDebug()<<"main thread:"<<QThread::currentThreadId();
	return a.exec();
}
