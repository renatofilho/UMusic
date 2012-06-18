#include "usubsonic.h"
#include "umodel.h"

#include <QDir>
#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#define BASE_DIR "../"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Ubuntu");
    QCoreApplication::setApplicationName("UbuntuOne");

    QDir::addSearchPath("artwork", BASE_DIR + QString("artwork/"));

    USubSonic subsonic;
    UModel umodel(&subsonic);

    QDeclarativeView *view = new QDeclarativeView();
    view->rootContext()->setContextProperty("umodel", &umodel);
    QUrl source("../src/uview.qml");
    view->setBaseSize(480, 800);
    view->setSource(source);
    view->show();

    umodel.loadAlbumns();

    app.exec();
}
