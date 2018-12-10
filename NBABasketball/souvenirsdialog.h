#ifndef SOUVENIRSDIALOG_H
#define SOUVENIRSDIALOG_H

#include <QDialog>

namespace Ui {
class souvenirsDialog;
}

class souvenirsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit souvenirsDialog(QWidget *parent = nullptr);
    ~souvenirsDialog();

    void setLabel(QString labelName);   // Set the souvenirs label
    int  getNum();  // Get the num of souvenirs

private slots:
    void on_enterButton_sd_clicked();

private:
    Ui::souvenirsDialog *ui;
};

#endif // SOUVENIRSDIALOG_H
