#include "login.h"
#include "ui_login.h"
#include "teamselection.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
        QString username = ui->lineedit_username->text();
        QString password = ui->lineedit_password->text();

        if(username == "test") {
            if(password == "1234") {
                QMessageBox::information(this, "Login", "Login Successful. Welcome. ");
                tsPage = new teamSelection;
                tsPage -> show();
                this -> close();
            }
            else {
                QMessageBox::information(this, "Login", "Invalid Login. Try again.  ");
            }
        }
        else {
            QMessageBox::information(this, "Login", "Invalid Login. Try again.  ");
        }
}
