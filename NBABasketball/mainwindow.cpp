#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "admindialog.h"
#include "map.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Administrator Dialog Box
 */
void MainWindow::on_adminButton_clicked()
{
    // Creates the admin dialog window
    admindialog adminLogin;

    // Sets the modal for the admin dialog window, then executes
    adminLogin.setModal(true);
    adminLogin.exec();

    // Login Validation
    if(adminLogin.getLogin()) {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

// Go to Add Team window
void MainWindow::on_addTeamButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Go to Change Souvenir Prices window
void MainWindow::on_changeSouvenirPricesButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

// Go to Add Souvenirs window
void MainWindow::on_addSouvenirsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

// Go to Delete Souvenirs window
void MainWindow::on_deleteSouvenirsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

// Go to Modify Arena Information window
void MainWindow::on_modifyArenaInformationButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}
