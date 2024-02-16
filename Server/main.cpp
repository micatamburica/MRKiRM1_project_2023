#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <string.h>
#include <data.h>
#include <iostream>
#include <QDebug>

QTcpServer server;
QTcpSocket *incomingSocket;

QString LoggedUser;
extern QList<MessageStructure> MessagesFromUsers;
extern MessageStructure DummyMessage;

void communicationWithClient()
{
    QString receivedMessage = incomingSocket->readAll().data();

    //1. LOGIN
    if(receivedMessage.contains("LOGIN_REQUEST"))
    {
        bool loginFlag = false;
        QStringList loginPieces = receivedMessage.split(" ");

        //check if in data base email and password exist
        for(int i = 0; i < MAXUSERS; i++) {
            if(!(loginPieces[1].compare(Users[i][0])) && !(loginPieces[2].compare(Users[i][1]))) {
                LoggedUser = loginPieces[1];
                loginFlag = true;
                break;
            }
        }
        if(loginFlag) {
            incomingSocket->write("LOGIN_CONFORMATION");
        } else {
            incomingSocket->write("LOGIN_REJECT");
        }
    }

    //2. LOGOUT
    if(receivedMessage == "LOGOUT_REQUEST")
    {
        incomingSocket->write("LOGOUT_CONFORMATION");
    }

    //3. SEND
    if(receivedMessage.contains("SEND_MESSAGE"))
    {
        bool sendFlag = false;
        QStringList loginPieces = receivedMessage.split("~~~");

        for(int i = 0; i < MAXUSERS; i++)
        {
            if(!(loginPieces[1].compare(Users[i][0]))) {
                DummyMessage.Email = loginPieces[1];
                DummyMessage.NameOfMessage = loginPieces[2];
                DummyMessage.Message = loginPieces[3];
                DummyMessage.SenderOfMessage = LoggedUser;
                DummyMessage.ReadFlag = false;
                MessagesFromUsers.append(DummyMessage);
                sendFlag = true;
                break;
            }
        }
        if(sendFlag) {
            incomingSocket->write("SEND_CONFORMATION");
        } else {
            incomingSocket->write("SEND_ERROR");
        }
    }

    //4. CLEAN
    if(receivedMessage == "CLEAN_EMAIL")
    {
        for(int i = 0; i < MessagesFromUsers.size(); )
        {
            if(MessagesFromUsers.at(i).Email == LoggedUser) {
                MessagesFromUsers.removeAt(i);
            } else {
                i++;
            }
        }
        incomingSocket->write("CLEAN_CONFORMATION");
    }

    //5. STAT
    if(receivedMessage == "STAT_EMAIL")
    {
        uint32_t numberOfMessages = 0;
        uint32_t numberOfOpened = 0;
        uint32_t numberOfNotOpened = 0;
        std::string theNamesOfMessages;
        std::string sendMessage = "STAT_FEEDACK~~~";
        for(int i = 0; i < MessagesFromUsers.size(); i++)
        {
            if(MessagesFromUsers.at(i).Email == LoggedUser)
            {
                if(MessagesFromUsers.at(i).ReadFlag) {
                    numberOfOpened ++;
                } else {
                    numberOfNotOpened++;
                }
                numberOfMessages++;
                theNamesOfMessages += "~~~";
                theNamesOfMessages += MessagesFromUsers.at(i).NameOfMessage.toStdString();
            }
        }
        sendMessage += std::to_string(numberOfMessages);
        sendMessage += "~~~";
        sendMessage += std::to_string(numberOfOpened);
        sendMessage += "~~~";
        sendMessage += std::to_string(numberOfNotOpened);
        sendMessage += theNamesOfMessages;

        incomingSocket->write(sendMessage.c_str());
    }

    //6. DELETE
    if(receivedMessage.contains("DELETE_MESSAGE"))
    {
         bool deleteFlag = false;
        QStringList loginPieces = receivedMessage.split("~~~");

        for(int i = 0; i < MessagesFromUsers.size(); i++)
        {
            if((loginPieces[1] == (MessagesFromUsers.at(i).NameOfMessage)) && (LoggedUser == (MessagesFromUsers.at(i).Email))) {
                MessagesFromUsers.removeAt(i);
                deleteFlag = true;
                break;
            }
        }

        if(deleteFlag) {
            incomingSocket->write("DELETE_CONFORMATION");
        } else {
            incomingSocket->write("DELETE_ERROR");
        }
    }

    //7. CHECK
    if(receivedMessage == "CHECK_EMAIL")
    {
        uint32_t numberOfNotOpened = 0;
        for(int i = 0; i < MessagesFromUsers.size(); i++)
        {
            if((LoggedUser == (MessagesFromUsers.at(i).Email)) && !(MessagesFromUsers.at(i).ReadFlag)) {
                numberOfNotOpened++;
            }
        }
        incomingSocket->write(("CHECK_FEEDBACK~~~" + std::to_string(numberOfNotOpened)).c_str());
    }

    //8. RECEIVE
    if(receivedMessage == "RECEIVE_MESSAGE")
    {
        std::string sendMessage = "RECEIVE_FEEDBACK~~~";
        for(int i = 0; i < MessagesFromUsers.size(); i++)
        {
            if((LoggedUser == (MessagesFromUsers.at(i).Email)) && !(MessagesFromUsers.at(i).ReadFlag)) {
                sendMessage += MessagesFromUsers.at(i).NameOfMessage.toStdString();
                sendMessage += "~~~";
                sendMessage += MessagesFromUsers.at(i).SenderOfMessage.toStdString();
                sendMessage += "~~~";
                sendMessage += MessagesFromUsers.at(i).Message.toStdString();
                DummyMessage = MessagesFromUsers.at(i);
                DummyMessage.ReadFlag = true;
                MessagesFromUsers.removeAt(i);
                MessagesFromUsers.append(DummyMessage);
                break;
            }
        }
        incomingSocket->write(sendMessage.c_str());
    }
}

void newConnection()
{
    //Making of a socket
    incomingSocket = server.nextPendingConnection();
    qDebug() << "New client has connected at Port: " << incomingSocket->peerPort();
    QObject::connect(incomingSocket, &QTcpSocket::readyRead, communicationWithClient);
}

//MAIN FUNCTION
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Making of dummy messages for the user admin@gmail.com for testing purposes
    DummyMessage.Email = "admin@gmail.com";
    DummyMessage.NameOfMessage = "The First Message";
    DummyMessage.Message = "This message is used for testing";
    DummyMessage.SenderOfMessage = "baltazar@yahoo.com";
    DummyMessage.ReadFlag = true;
    MessagesFromUsers.append(DummyMessage);
    DummyMessage.NameOfMessage = "A Joke";
    DummyMessage.Message = "A joke is a display of humour in which words are used within a specific and well-defined "
                           "narrative structure to make people laugh and is usually not meant to be interpreted literally.";
    DummyMessage.ReadFlag = false;
    MessagesFromUsers.append(DummyMessage);
    DummyMessage.NameOfMessage = "How to win a car if you want to win a car";
    DummyMessage.Message = "Try entering a few contests each week, if not daily. Your odds will get better as you enter the void";
    DummyMessage.ReadFlag = true;
    MessagesFromUsers.append(DummyMessage);
    DummyMessage.NameOfMessage = "copy";
    DummyMessage.Message = "This is a copy of out passport";
    DummyMessage.ReadFlag = false;
    DummyMessage.SenderOfMessage = "a@yahho.com";
    MessagesFromUsers.append(DummyMessage);

    //ADDRESS AND PORT
    QHostAddress serverAddress;
    serverAddress.setAddress("127.0.0.1");
    quint16 serverPort = 12345;

    //Open server to wait for clients
    server.listen(serverAddress, serverPort);

    //If opening successful
    if(server.isListening()) {
        qDebug() << "Server is running";
        qDebug() << "Adress: " << server.serverAddress().toString();
        qDebug() << "Port:    " << server.serverPort();

        //Make a socket for new connection
        QObject::connect(&server, &QTcpServer::newConnection, newConnection);

    } else {
        qDebug() << "Server is NOT running";
    }

    return a.exec();
}
