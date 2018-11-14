#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QDialog>

namespace Ui {
class admindialog;
}

class admindialog : public QDialog
{
    Q_OBJECT

public:
    explicit admindialog(QWidget *parent = nullptr);
    ~admindialog();
    void isTrue();
    bool getLogin();

private slots:
    void on_enterButton_clicked();

private:
    Ui::admindialog *ui;
    bool loginValid;
};

#endif // ADMINDIALOG_H
