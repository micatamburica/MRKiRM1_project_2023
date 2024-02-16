#include "mainwindow.h"
#include "connection.h"
#include "ui_mainwindow.h"

extern Connection classForConnection;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->loginErrorLabel->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//1. LOGIN
void MainWindow::on_loginButton_clicked()
{
    //actions if login button is pushed
    ui->loginErrorLabel->setVisible(false);

    std::string getEmail = (ui->emailInput->text()).toStdString();
    std::string getPassword = (ui->passwordInput->text()).toStdString();

    //send login_request
    QString receivedMessage = classForConnection.Login(getEmail, getPassword);

    //login_conformaiton or login_reject
    if(receivedMessage == "LOGIN_CONFORMATION") {
        hide();
        TheUserWindow = new UserWindow(this);
        TheUserWindow->setWindowTitle("Email");
        TheUserWindow->show();

    } else if(receivedMessage == "LOGIN_REJECT") {
        ui->loginErrorLabel->setVisible(true);
    }

}
