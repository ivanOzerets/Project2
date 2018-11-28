#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_adminButton_clicked();

    void on_addTeamButton_clicked();

    void on_changeSouvenirPricesButton_clicked();

    void on_addSouvenirsButton_clicked();

    void on_deleteSouvenirsButton_clicked();

    void on_modifyArenaInformationButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
