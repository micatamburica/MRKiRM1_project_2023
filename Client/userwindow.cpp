#include "userwindow.h"
#include "mainwindow.h"
#include "ui_userwindow.h"
#include "connection.h"
#include <QTimer>

extern Connection classForConnection;

//prep for the user window to open
UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    ui->SendEmailLabel->setVisible(false);
    ui->SendEmailInput->setVisible(false);
    ui->SendNameLabel->setVisible(false);
    ui->SendNameInput->setVisible(false);
    ui->SendMessageInput->setVisible(false);
    ui->TheRealSend->setVisible(false);
    ui->TheExitFromSend->setVisible(false);
    ui->TheRealDelete->setVisible(false);
}

UserWindow::~UserWindow()
{
    delete ui;
}

//function that sets the buttons unusable
void UserWindow::setButtonsUnusable() {
    ui->checkButton->setEnabled(false);
    ui->cleanButton->setEnabled(false);
    ui->statButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->receiveButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
}
//function that sets the buttons usable
void UserWindow::setButtonsUsable() {
    ui->checkButton->setEnabled(true);
    ui->cleanButton->setEnabled(true);
    ui->statButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->receiveButton->setEnabled(true);
    ui->sendButton->setEnabled(true);
}

//2. LOGOUT
void UserWindow::on_logoutButton_clicked()
{
    QString receivedMessage = classForConnection.Logout();
    if(receivedMessage == "LOGOUT_CONFORMATION")
    {
        hide();
        classForConnection.DisconnectFromServer();
        exit(0);
    }
}

//4. CLEAN
void UserWindow::on_cleanButton_clicked()
{
    QString receivedMessage = classForConnection.Clean();
    if(receivedMessage == "CLEAN_CONFORMATION")
    {
        ui->statusLabel->setText("All messages deleted successfully");
        QTimer::singleShot(4000, this, [this] () { ui->statusLabel->setText(""); });
    }

}

//5. STAT
void UserWindow::on_statButton_clicked()
{
    QString receivedMessage = classForConnection.Stat();

    if(receivedMessage.contains("STAT_FEEDACK"))
    {
        QStringList loginPieces = receivedMessage.split("~~~");
        QString showMessage = "STATISTICS\nNumber Of Messages: " + loginPieces[1] +
                              "   Number of OPENED Messages: " + loginPieces[2] +
                              "   Number of UNOPENED Messages: " + loginPieces[3];
        for(int i = 4; i < loginPieces.size(); i++)
        {
            showMessage += "\n";
            showMessage += QString::number(i-3);
            showMessage += ". message ";
            showMessage += loginPieces[i];
        }
        this->setButtonsUnusable();
        ui->SendMessageInput->setDisabled(true);
        ui->SendMessageInput->setVisible(true);
        ui->SendMessageInput->setPlainText(showMessage);
        ui->TheExitFromSend->setVisible(true);
    }
}

//7. CHECK
void UserWindow::on_checkButton_clicked()
{
    QString receivedMessage = classForConnection.Check();
    if(receivedMessage.contains("CHECK_FEEDBACK"))
    {
        QStringList loginPieces = receivedMessage.split("~~~");

        if(loginPieces[1] == "0") {
            ui->statusLabel->setText("Sorry, you don't have any new messages");
        } else {
            ui->statusLabel->setText("Number of new messages: " + loginPieces[1]);
        }
        QTimer::singleShot(4000, this, [this] () { ui->statusLabel->setText(""); });
    }
}

//8. RECEIVE
void UserWindow::on_receiveButton_clicked()
{
    QString receivedMessage = classForConnection.Receive();
    if(receivedMessage.contains("RECEIVE_FEEDBACK"))
    {
        QStringList loginPieces = receivedMessage.split("~~~");

        if(loginPieces[1].isEmpty()) {
           ui->statusLabel->setText("Sorry, you don't have any new messages");
           QTimer::singleShot(4000, this, [this] () { ui->statusLabel->setText(""); });
        } else {
            this->setButtonsUnusable();
            ui->statusLabel->setText("From: " + loginPieces[2] + "\nName: " + loginPieces[1]);
            ui->SendMessageInput->setDisabled(true);
            ui->SendMessageInput->setVisible(true);
            ui->SendMessageInput->setPlainText(loginPieces[3]);
            ui->TheExitFromSend->setVisible(true);
        }

    }
}

//6. DELETE
void UserWindow::on_deleteButton_clicked()
{
    ui->SendNameLabel->setVisible(true);
    ui->SendNameInput->setVisible(true);
    this->setButtonsUnusable();
    ui->TheRealDelete->setVisible(true);
    ui->TheExitFromSend->setVisible(true);
    ui->statusLabel->setText("Click the Delete Message button when ready to send");
}
void UserWindow::on_TheRealDelete_clicked()
{
    QString receivedMessage = classForConnection.Delete(ui->SendNameInput->text());
    if(receivedMessage == "DELETE_CONFORMATION")
    {
        ui->SendNameLabel->setVisible(false);
        ui->SendNameInput->setVisible(false);
        ui->SendNameInput->setText("");
        this->setButtonsUsable();
        ui->TheRealDelete->setVisible(false);
        ui->TheExitFromSend->setVisible(false);
        ui->statusLabel->setText("The message has been deleted");
        QTimer::singleShot(5000, this, [this] () { ui->statusLabel->setText(""); });
    } else
        if(receivedMessage == "DELETE_ERROR") {
            ui->statusLabel->setText("Error deleting the message (try another name)");
            QTimer::singleShot(4000, this, [this] () { ui->statusLabel->setText(""); });
        }
}

//3. SEND
void UserWindow::on_sendButton_clicked()
{
    ui->SendEmailLabel->setVisible(true);
    ui->SendEmailInput->setVisible(true);
    ui->SendNameLabel->setVisible(true);
    ui->SendNameInput->setVisible(true);
    ui->SendMessageInput->setVisible(true);
    this->setButtonsUnusable();
    ui->TheRealSend->setVisible(true);
    ui->TheExitFromSend->setVisible(true);
    ui->statusLabel->setText("Click the Send Message button when ready to send");
}
void UserWindow::on_TheRealSend_clicked()
{
    QString receivedMessage = classForConnection.Send(ui->SendEmailInput->text(), ui->SendNameInput->text(), ui->SendMessageInput->toPlainText());
    if(receivedMessage == "SEND_CONFORMATION")
    {
        ui->SendEmailLabel->setVisible(false);
        ui->SendEmailInput->setVisible(false);
        ui->SendEmailInput->setText("");
        ui->SendNameLabel->setVisible(false);
        ui->SendNameInput->setVisible(false);
        ui->SendNameInput->setText("");
        ui->SendMessageInput->setVisible(false);
        ui->SendMessageInput->setPlainText("");
        this->setButtonsUsable();
        ui->TheRealSend->setVisible(false);
        ui->TheExitFromSend->setVisible(false);
        ui->statusLabel->setText("The message has been send");
        QTimer::singleShot(4000, this, [this] () { ui->statusLabel->setText(""); });
    } else
        if(receivedMessage == "SEND_ERROR") {
        ui->statusLabel->setText("Error sending the message (try another user)");
        QTimer::singleShot(4000, this, [this] () { ui->statusLabel->setText(""); });
    }
}
void UserWindow::on_TheExitFromSend_clicked()
{
    ui->SendEmailLabel->setVisible(false);
    ui->SendEmailInput->setVisible(false);
    ui->SendEmailInput->setText("");
    ui->SendNameLabel->setVisible(false);
    ui->SendNameInput->setVisible(false);
    ui->SendNameInput->setText("");
    ui->SendMessageInput->setVisible(false);
    ui->SendMessageInput->setPlainText("");
    this->setButtonsUsable();
    ui->SendMessageInput->setDisabled(false);
    ui->TheRealSend->setVisible(false);
    ui->TheExitFromSend->setVisible(false);
    ui->statusLabel->setText("");
    ui->TheRealDelete->setVisible(false);
}
