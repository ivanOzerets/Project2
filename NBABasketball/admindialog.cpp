#include "admindialog.h"
#include "ui_admindialog.h"

admindialog::admindialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admindialog)
{
    ui->setupUi(this);
    ui->invalidLabel->hide();
    loginValid = false;
}

admindialog::~admindialog()
{
    delete ui;
}

void admindialog::on_enterButton_clicked()
{
    QString username = "admin";
    QString password = "1234";

    if(ui->userLineEdit->text() == username && ui->passLineEdit->text() == password) {
        isTrue();
        close();
    }
    else {
        ui->invalidLabel->show();
    }
}

void admindialog::isTrue()
{
    loginValid = true;
}

bool admindialog::getLogin()
{
    return loginValid;
}
