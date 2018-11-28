#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QSqlQueryModel>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase db;    // The database variable, accessible anywhere
    bool dbOpen();      // Function to open the database
    void dbClose();     // Function to close the database

private slots:
    void on_adminButton_clicked();

    void on_addTeamButton_clicked();

    void on_changeSouvenirPricesButton_clicked();

    void on_addSouvenirsButton_clicked();

    void on_deleteSouvenirsButton_clicked();

    void on_modifyArenaInformationButton_clicked();

    void on_maiBackPushButton_clicked();

    void on_adminBackPushButton_clicked();

private:
    Ui::MainWindow *ui;

    QString DB_Path;    // The string that holds the database's path on your computer
};

#endif // MAINWINDOW_H
