#ifndef DATA_H
#define DATA_H

#include <QList>
#define MAXUSERS 20

//  Dummy data base with emails and passwords
//  [n][0] email@gmail.com    [n][1] password
const QString Users[MAXUSERS][2] = {

    {"jovan@gmail.com", "jovanovaSifra"},
    {"admin@gmail.com", "admin"},
    {"com@com.com", "monkey"},
    {"nikoola123@gmail.com", "123456789"},
    {"mVasiljevic@gmail.com", "kobasica"},
    {"user57@yahoo.com", "password57"},
    {"radNaSkela@gmail.com", "brmbrm"},
    {"jana@gmail.com", "ana"},
    {"ana@gmail.com", "lana"},
    {"lana@gmail.com", "ana"},
    {"dijamantMargarin@gmail.com", "uSveSeMesa"},
    {"manojlo666@gmail.com", "okfD2a(s"},
    {"brica@gmail.com", "Ivan456"},
    {"a@yahho.com", "aaa"},
    {"main@gmail.com", "int_main"},
    {"veceras?@gmail.com", "padasneg"},
    {"skockoskocko@yahoo.com", "lIkA"},
    {"ftn@gmail.com", "studentskaSluzba"},
    {"javniSudRepublikeSrbije@gmail.com", "VukKaradzic"},
    {"pavle3@gmail.com", "SigurnaSifra.23"}

};

struct MessageStructure{
    QString Email;
    QString NameOfMessage;
    QString Message;
    QString SenderOfMessage;
    bool ReadFlag;
};

QList<MessageStructure> MessagesFromUsers;
MessageStructure DummyMessage;

#endif // DATA_H
