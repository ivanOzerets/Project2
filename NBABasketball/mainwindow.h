#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// For SQL
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

// For File Input
#include <QFileDialog>

// For Error Checking
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

    void on_arenaSelectionComboBox_currentIndexChanged(const QString &arg1);

    void on_updatePushButton_clicked();

    void on_atBackPushButton_clicked();

    void on_cspBackPushButton_clicked();

    void on_asBackPushButton_clicked();

    void on_dsBackPushButton_clicked();

    void on_teamSelectionComboBox_currentIndexChanged(const QString &arg1);

    void on_souvenirSelectionComboBox_currentIndexChanged(const QString &arg1);

    void on_updatePushButton_CSP_clicked();

    void on_updatePushButton_AS_clicked();

    void on_dsTeamSelectionComboBox_currentIndexChanged(const QString &arg1);

    void on_updatePushButton_DS_clicked();

    void on_uploadExpansionPushButton_clicked();

    void on_uploadDistancePushButton_clicked();

    void on_pushButton_clicked();

    void on_startBack_clicked();

    void on_DFS_clicked();

    void on_BFS_clicked();

    void on_BFSBack_clicked();

    void on_DFSBack_clicked();

    void on_MST_clicked();

    void on_MSTBack_clicked();

private:
    Ui::MainWindow *ui;

    QString DB_Path;    // The string that holds the database's path on your computer
};

#endif // MAINWINDOW_H
