#include "mainwindow.h"
#include "connection.h"
#include <QApplication>

Connection classForConnection;

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);

    classForConnection.ConnectToServer();

    MainWindow w;
    w.setWindowTitle("Email");
    w.show();

    return a.exec();
}
