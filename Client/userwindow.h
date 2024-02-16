#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_logoutButton_clicked();
    void on_cleanButton_clicked();
    void on_sendButton_clicked();
    void on_TheRealSend_clicked();
    void on_TheExitFromSend_clicked();
    void on_statButton_clicked();
    void on_deleteButton_clicked();
    void on_TheRealDelete_clicked();
    void on_checkButton_clicked();
    void on_receiveButton_clicked();
    void setButtonsUnusable();
    void setButtonsUsable();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
