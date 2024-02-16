#include "connection.h"

QString receivedMessage;

void Connection::ConnectToServer()
{
    //ADDRESS AND PORT
    QHostAddress serverAddress;
    serverAddress.setAddress("127.0.0.1");
    quint16 serverPort = 12345;

    clientSocket.connectToHost(serverAddress, serverPort, QIODevice::ReadWrite);
    if(clientSocket.waitForConnected(10000)) {

    } else {
        qDebug("Did not connect to the server");
        exit(0);
    }
}

void Connection::DisconnectFromServer()
{
    clientSocket.disconnectFromHost();
}

//1. LOGIN
QString Connection::Login(const std::string Email, const std::string Password)
{
    std::string gatherMessage = "LOGIN_REQUEST " + Email + " " + Password;
    clientSocket.write(gatherMessage.c_str());

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.readAll();

    return receivedMessage;
}

//2. LOGOUT
QString Connection::Logout()
{
    clientSocket.write("LOGOUT_REQUEST");

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}

//3. SEND
QString Connection::Send(const QString Email, const QString Name, const QString Message)
{
    std::string gatherMessage = "SEND_MESSAGE~~~" + Email.toStdString() + "~~~" + Name.toStdString() + "~~~" + Message.toStdString();
    clientSocket.write(gatherMessage.c_str());

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}

//4. CLEAN
QString Connection::Clean()
{
    clientSocket.write("CLEAN_EMAIL");

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}

//5. STAT
QString Connection::Stat()
{
    clientSocket.write("STAT_EMAIL");

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}

//6. DELETE
QString Connection::Delete(const QString Name)
{
    std::string gatherMessage = "DELETE_MESSAGE~~~" + Name.toStdString();
    clientSocket.write(gatherMessage.c_str());

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}

//7. CHECK
QString Connection::Check()
{
    clientSocket.write("CHECK_EMAIL");

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}

//8. RECEIVE
QString Connection::Receive()
{
    clientSocket.write("RECEIVE_MESSAGE");

    clientSocket.waitForReadyRead();
    receivedMessage = clientSocket.read(MAXSIZEOFMESSAGE);

    return receivedMessage;
}
