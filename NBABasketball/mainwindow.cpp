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

// Set the teamSelectionComboBox and ___TreeView to display information
// before going to the Change Souvenir Prices window
void MainWindow::on_changeSouvenirPricesButton_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the teamSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->teamSelectionComboBox->setModel(modal);

    // Query for the entirety of the Souvenirs table
    qry->prepare("SELECT * FROM Souvenirs ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the ___ComboBox
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->modifySouvenirInformationTreeView->setModel(modal2);

    // Go to Change Souvenir Prices window
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

// Set the arenaSelectionComboBox and modifyArenaInformationTreeView
// to display information before going to the Modify Arena Information window
void MainWindow::on_modifyArenaInformationButton_clicked()
{
    // Create the query for the ArenaName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT ArenaName FROM Information ORDER BY ArenaName ASC");
    qry->exec();

    // Create the modal to hold the query and set the arenaSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->arenaSelectionComboBox->setModel(modal);

    // Query for the entire Information table
    qry->prepare("SELECT * FROM Information ORDER BY ArenaName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the modifyArenaInformationTreeView
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->modifyArenaInformationTreeView->setModel(modal2);

    // Set input masks on stadiumCapacityLineEdit, joinedLeagueLineEdit
    ui->stadiumCapacityLineEdit->setInputMask("0000000000");
    ui->joinedLeagueLineEdit->setInputMask("9999");

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

// Receive the selection from the arenaSelectionComboBox and update the lineEdits
// on this window to contain the information already stored in the database
void MainWindow::on_arenaSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    // Create the query for the row in the Information table where ArenaName == arg1
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT * FROM Information WHERE ArenaName = '"+arg1+"'");
    qry->exec();

    // Fill the lineEdits on this window with the queried data
    qry->first();
    ui->conferenceLineEdit->setText(qry->value(0).toString());      // Conference
    ui->divisionLineEdit->setText(qry->value(1).toString());        // Division
    ui->teamNameLineEdit->setText(qry->value(2).toString());        // Team Name
    ui->locationLineEdit->setText(qry->value(3).toString());        // Location
    ui->arenaNameLineEdit->setText(qry->value(4).toString());       // Arena Name
    ui->stadiumCapacityLineEdit->setText(qry->value(5).toString()); // Stadium Capacity
    ui->joinedLeagueLineEdit->setText(qry->value(6).toString());    // Joined League
    ui->coachLineEdit->setText(qry->value(7).toString());           // Coach
}

// Use the lineEdits to modify the selected arena in the database,
// and update the table to reflect the changes
void MainWindow::on_updatePushButton_clicked()
{
    // Create a placeholder for the current index of the arenaSelectionComboBox
    QString placeholder = ui->arenaSelectionComboBox->currentText();

    // Delete from the database the row where ArenaName == placeholder
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Information WHERE ArenaName = '"+placeholder+"'");
    qry->exec();

    // Insert the newly modified arena
    qry->prepare("INSERT INTO Information (Conference, Division, TeamName, Location, ArenaName, StadiumCapacity, JoinedLeague, Coach) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    qry->addBindValue(ui->conferenceLineEdit->text());          // Conference
    qry->addBindValue(ui->divisionLineEdit->text());            // Division
    qry->addBindValue(ui->teamNameLineEdit->text());            // TeamName
    qry->addBindValue(ui->locationLineEdit->text());            // Location
    qry->addBindValue(ui->arenaNameLineEdit->text());           // ArenaName
    qry->addBindValue(ui->stadiumCapacityLineEdit->text());     // StadiumCapacity
    qry->addBindValue(ui->joinedLeagueLineEdit->text());        // JoinedLeague
    qry->addBindValue(ui->coachLineEdit->text());               // Coach
    qry->exec();

    // Update the arenaSelectionComboBox (comboBox) -> Reused code from on_modifyArenaInformationButton_clicked()
    qry->prepare("SELECT ArenaName FROM Information ORDER BY ArenaName ASC");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->arenaSelectionComboBox->setModel(modal);

    // Update the modifyArenaInformationTreeView (treeView) -> Reused code from on_modifyArenaInformationButton_clicked()
    qry->prepare("SELECT * FROM Information ORDER BY ArenaName ASC");
    qry->exec();
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->modifyArenaInformationTreeView->setModel(modal2);
}

// Go from Add Team window to Admin window
void MainWindow::on_atBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Go from Change Souvenir Prices window to Admin window
void MainWindow::on_cspBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Go from Add Souvenirs window to Admin window
void MainWindow::on_asBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Go from Delete Souvenirs window to Admin window
void MainWindow::on_dsBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Set the souvenirSelectionComboBox to display the souvenirs of the team found in
// the teamSelectionComboBox whenever the teamSelectionComboBox changes
void MainWindow::on_teamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    // Create the query for the row in the Souvenirs table where TeamName == arg1
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT Souvenir FROM Souvenirs WHERE TeamName = '"+arg1+"'");
    qry->exec();

    // Create the modal to hold the query and set the souvenirSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->souvenirSelectionComboBox->setModel(modal);
}

// Receive the selection from the ___ComboBox and update the lineEdits
// on this window to contain the information already stored in the database
void MainWindow::on_souvenirSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    // Create a temporary placeholder for the selection in the teamSelectionComboBox
    QString placeholder = ui->teamSelectionComboBox->currentText();

    // Create the query for the row in the Souvenir table where TeamName == placeholder
    // and Souvenir == arg1
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT Souvenir, Price FROM Souvenirs WHERE TeamName = '"+placeholder+"' AND Souvenir = '"+arg1+"'");
    qry->exec();

    // Fill the lineEdits on this window with the queried data
    qry->first();
    ui->souvenirNameLineEdit->setText(qry->value(0).toString());    // Souvenir
    ui->souvenirPriceLineEdit_2->setText(qry->value(1).toString()); // Price
}
