#ifndef CONNECTION_H
#define CONNECTION_H

#define MAXSIZEOFMESSAGE 100
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <iostream>
#include <string.h>
#include <iostream>

class Connection {

    QTcpSocket clientSocket;

public:

    void ConnectToServer();
    void DisconnectFromServer();

    QString Login(const std::string, const std::string);
    QString Logout();
    QString Send(const QString, const QString, const QString);
    QString Clean();
    QString Stat();
    QString Delete(const QString);
    QString Check();
    QString Receive();

};

#endif // CONNECTION_H
