#include "souvenirsdialog.h"
#include "ui_souvenirsdialog.h"

souvenirsDialog::souvenirsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::souvenirsDialog)
{
    ui->setupUi(this);
    ui->amountLineEdit->setText("0");
    ui->amountLineEdit->setInputMask("000");
}

souvenirsDialog::~souvenirsDialog()
{
    delete ui;
}

void souvenirsDialog::setLabel(QString labelName)
{
    ui->souvenirsLabel->setText(labelName);
}

int souvenirsDialog::getNum()
{
    int num = ui->amountLineEdit->text().toInt();
    return num;
}

void souvenirsDialog::on_enterButton_sd_clicked()
{
    close();
}
