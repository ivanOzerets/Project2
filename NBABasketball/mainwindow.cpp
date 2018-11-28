#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "admindialog.h"
#include "map.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Open the database -> you'll need to specify the path it's under on your computer
    DB_Path = "D:\\System Files\\My Documents\\git\\Project2\\NBAinfo.db";
    dbOpen();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * To open the database
 */
bool MainWindow::dbOpen()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_Path);
    if(!db.open()) {
        qWarning() << "ERROR: " << db.lastError();
        return false;
    }
    else {
        return true;
    }
}

/*
 * To close the database - is this really necessary?
 */
void MainWindow::dbClose()
{
    db.close();
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

// Set the arenaSelectionComboBox and modifyArenaInformationTableWidget
// to display information before going to the Modify Arena Information window
void MainWindow::on_modifyArenaInformationButton_clicked()
{
    // Create the query for the ArenaName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT ArenaName FROM Information");
    qry->exec();

    // Create the modal to hold the query and set the arenaSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->arenaSelectionComboBox->setModel(modal);

    // Query for the entire Information table
    qry->prepare("SELECT * FROM Information");
    qry->exec();

    // Create the modal to hold the new query and set the modifyArenaInformationTreeView
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->modifyArenaInformationTreeView->resizeColumnToContents(25);
    ui->modifyArenaInformationTreeView->setModel(modal2);

    // Go to Modify Arena Information window
    ui->stackedWidget->setCurrentIndex(6);
}

// Go from Modify Arena Information window to Admin window
void MainWindow::on_maiBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Go from Admin window to Main window
void MainWindow::on_adminBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
