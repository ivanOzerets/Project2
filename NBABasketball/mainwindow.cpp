#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "admindialog.h"
#include "souvenirsdialog.h"
#include "map.h"
#include "dfs.h"
#include "bfs.h"
#include "mst.h"
#include "dijkstras.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Open the database -> you'll need to specify the path it's under on your computer
    //                   -> When you change the location, just comment out the others' location, and uncomment yours:
//    DB_Path = "C:\\Users\\Ivan\\School\\Saddleback\\CS1D\\Project2\\SQLite\\DB\\NBAinfo.db";
//    DB_Path = "D:\\System Files\\My Documents\\git\\Project2\\NBAinfo.db";
    DB_Path = "C:\\Users\\Alecstar\\Documents\\GitHub\\Project2\\NBAinfo.db";
    dbOpen();

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    QSqlQuery qry;

    if(db.isOpen())
    {
    qry.exec("DELETE FROM ShoppingCart");
    }
    else
    {
      db.open();
      qry.exec("DELETE FROM ShoppingCart");
    }

    dbClose();

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

/*
 *  StackedWidget Page 1 -> Administrative Tools
 */

// Go to Add Team window
// Set the addExpansionTreeView, addDistanceTreeView to display information beforehand
void MainWindow::on_addTeamButton_clicked()
{
    // Query for the entire Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT * FROM Information");
    qry->exec();

    // Create the modal to hold the new query and set the addTeamTreeView
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->addExpansionTreeView->setModel(modal);

    // Query for the entire Distances table
    qry->prepare("SELECT * FROM Distances");
    qry->exec();

    // Create the modal to hold the new query and set the addTeamTreeView
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->addDistanceTreeView->setModel(modal2);

    // Go to Add Team window
    ui->stackedWidget->setCurrentIndex(2);
}

// Go to Change Souvenir Prices Window
// Set the teamSelectionComboBox and modifySouvenirInformationTreeView
// to display information beforehand
void MainWindow::on_changeSouvenirPricesButton_clicked()
{
    // Create the query for the TeamName column in the Information table
    // -> because TeamName only appears once in Information table rather than
    //    multiple times in Souvenirs table
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

    // Set input masks on souvenirPriceLineEdit
    ui->souvenirPriceLineEdit->setInputMask("009.99");

    // Go to Change Souvenir Prices window
    ui->stackedWidget->setCurrentIndex(3);
}

// Go to Add Souvenirs window
// Set the asTeamSelectionComboBox and modifyArenaInformationTreeView
// to display information beforehand
void MainWindow::on_addSouvenirsButton_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the arenaSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->asTeamSelectionComboBox->setModel(modal);

    // Query for the entire Information table
    qry->prepare("SELECT * FROM Souvenirs ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the modifyArenaInformationTreeView
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->addSouvenirTreeView->setModel(modal2);

    // Set input masks on asSouvenirPriceLineEdit
    ui->asSouvenirPriceLineEdit->setInputMask("009.99");

    // Go to Add Souvenirs window
    ui->stackedWidget->setCurrentIndex(4);
}

// Go to Delete Souvenirs window
// Set the dsTeamSelectionComboBox and deleteSouvenirTreeView
// to display information beforehand
void MainWindow::on_deleteSouvenirsButton_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the dsTeamSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->dsTeamSelectionComboBox->setModel(modal);

    // Query for the entire Information table
    qry->prepare("SELECT * FROM Souvenirs ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the deleteSouvenirTreeView
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->deleteSouvenirTreeView->setModel(modal2);

    ui->stackedWidget->setCurrentIndex(5);
}

// Go to Modify Arena Information Window
// Set the arenaSelectionComboBox and modifyArenaInformationTreeView
// to display information beforehand
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

// Go from Admin window to Main window
void MainWindow::on_adminBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

/*
 * StackedWidget Page 2 -> Add Team
 */

// Prompt the user for a file, upload the file, add the new team to the database,
// and update the addExpansionTreeView
void MainWindow::on_uploadExpansionPushButton_clicked()
{
    // Create the query pointer for later use
    QSqlQuery *qry;
    qry = new QSqlQuery(db);

    // Prompt for Expansion File, Receive, Parse, and Upload to the Database
    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Input Expansion File"), "",
                                                         "Comma Seperated Value (*.csv)");
    QFile file(inputFileName);

    if(file.open(QIODevice::ReadOnly)) {
        QString data;

        while(!file.atEnd()) {
            data = file.readLine();

            QStringList list = data.split(',', QString::SkipEmptyParts);

            qry->prepare("INSERT INTO Information (Conference, Division, TeamName, Location, ArenaName, StadiumCapacity, JoinedLeague, Coach) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
            qry->addBindValue(list[0]);
            qry->addBindValue(list[1]);
            qry->addBindValue(list[2]);
            qry->addBindValue(list[3]+", "+list[4]);
            qry->addBindValue(list[5]);
            qry->addBindValue(list[6]);
            qry->addBindValue(list[7]);
            qry->addBindValue(list[8]);
            qry->exec();

            // Insert the traditional souvenirs for this new team into the Souvenirs table
            QString temporary = list[2] + " Autographed Basketball";
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', ?, '$49.89')");
            qry->addBindValue(temporary);
            qry->exec();

            temporary = list[2] + " Team Pennant";
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', ?, '$17.99')");
            qry->addBindValue(temporary);
            qry->exec();

            temporary = list[2] + " Team Picture";
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', ?, '$29.99')");
            qry->addBindValue(temporary);
            qry->exec();

            temporary = list[2] + " Team Jersey";
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', ?, '$179.99')");
            qry->addBindValue(temporary);
            qry->exec();
        }
    }

    file.close();

    // Update the addExpansionTreeView to reflect the changes
    qry->prepare("SELECT * FROM Information");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->addExpansionTreeView->setModel(modal);
}

// Prompt the user for a file, upload the file, add the new team to the database,
// and update the addDistanceTreeView
void MainWindow::on_uploadDistancePushButton_clicked()
{
    // Create the query pointer for later use
    QSqlQuery *qry;
    qry = new QSqlQuery(db);

    // Prompt for Distance File, Receive, Parse, and Upload to the Database
    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Input Distance File"), "",
                                                          "Comma Seperated Value (*.csv)");
    QFile file(inputFileName);

    if(file.open(QIODevice::ReadOnly)) {
        QString data;

        while(!file.atEnd()) {
            data = file.readLine();

            QStringList list = data.split(',', QString::SkipEmptyParts);

            qry->prepare("INSERT INTO Distances (BeginningTeamName, BeginningArena, EndingTeamName, Distance) VALUES (?, ?, ?, ?)");
            qry->addBindValue(list[0]);
            qry->addBindValue(list[1]);
            qry->addBindValue(list[2]);
            qry->addBindValue(list[3]);
            qry->exec();
        }
    }

    file.close();

    // Update the addDistanceTreeView to reflect the changes
    qry->prepare("SELECT * FROM Distances");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->addDistanceTreeView->setModel(modal);
}

// Go from Add Team window to Admin window
void MainWindow::on_atBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

/*
 * StackedWidget Page 3 -> Change Souvenir Prices
 */

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

// Receive the selection from the souvenirSelectionComboBox and update the lineEdits
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
    ui->souvenirPriceLineEdit->setText(qry->value(1).toString()); // Price
}

// Use the lineEdits to modify the selected souvenir in the database,
// and update the table to reflect the changes
void MainWindow::on_updatePushButton_CSP_clicked()
{
    // Create a placeholder for the current index of the teamSelectionComboBox  (Team Name)
    QString placeholder = ui->teamSelectionComboBox->currentText();

    // Create a placeholder for the current index of the souvenirSelectionComboBox  (Souvenir Name)
    QString placeholder2 = ui->souvenirSelectionComboBox->currentText();

    // Delete from the database the row where TeamName == placeholder && Souvenir == placeholder2
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Souvenirs WHERE TeamName = '"+placeholder+"' AND Souvenir = '"+placeholder2+"'");
    qry->exec();

    // Insert the newly modified arena
    qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES (?, ?, ?)");
    qry->addBindValue(placeholder);                             // Team Name
    qry->addBindValue(ui->souvenirNameLineEdit->text());        // Souvenir
    qry->addBindValue('$' + ui->souvenirPriceLineEdit->text()); // Price
    qry->exec();

    // Update the teamSelectionComboBox
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->teamSelectionComboBox->setModel(modal);

    // Update the modifySouvenirInformationTreeView
    qry->prepare("SELECT * FROM Souvenirs ORDER BY TeamName ASC");
    qry->exec();
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->modifySouvenirInformationTreeView->setModel(modal2);
}

// Go from Change Souvenir Prices window to Admin window
void MainWindow::on_cspBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

/*
 * StackedWidget Page 4 -> Add Souvenirs
 */

// Receive the selection from the asTeamSelectionComboBox and use the lineEdits
// to add a souvenir. Update the table to reflect the changes
void MainWindow::on_updatePushButton_AS_clicked()
{
    // Create a placeholder for the current index of the asTeamSelectionComboBox
    QString placeholder = ui->asTeamSelectionComboBox->currentText();

    // Add the souvenir
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES (?, ?, ?)");
    qry->addBindValue(placeholder);                               // TeamName
    qry->addBindValue(ui->addSouvenirNameLineEdit->text());       // Souvenir
    qry->addBindValue('$' + ui->asSouvenirPriceLineEdit->text()); // Price
    qry->exec();

    // Update the modifyArenaInformationTreeView
    qry->prepare("SELECT * FROM Souvenirs ORDER BY TeamName ASC");
    qry->exec();
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->addSouvenirTreeView->setModel(modal2);

    // Clear the lineEdits
    ui->addSouvenirNameLineEdit->clear();
    ui->asSouvenirPriceLineEdit->clear();
}

// Go from Add Souvenirs window to Admin window
void MainWindow::on_asBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

/*
 * StackedWidget Page 5 -> Delete Souvenirs
 */

// Update the dsSouvenirSelectionComboBox with the souvenirs of the team selected in
// the dsTeamSelectionComboBox
void MainWindow::on_dsTeamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    // Create the query for the row in the Souvenirs table where TeamName == arg1
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT Souvenir FROM Souvenirs WHERE TeamName = '"+arg1+"'");
    qry->exec();

    // Create the modal to hold the query and set the souvenirSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->dsSouvenirSelectionComboBox->setModel(modal);
}

// Take the selected team and souvenir and delete them from the database
void MainWindow::on_updatePushButton_DS_clicked()
{
    // Create a placeholder for the current index of the teamSelectionComboBox  (Team Name)
    QString placeholder = ui->dsTeamSelectionComboBox->currentText();

    // Create a placeholder for the current index of the souvenirSelectionComboBox  (Souvenir Name)
    QString placeholder2 = ui->dsSouvenirSelectionComboBox->currentText();

    // Delete from the database the row where TeamName == placeholder && Souvenir == placeholder2
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Souvenirs WHERE TeamName = '"+placeholder+"' AND Souvenir = '"+placeholder2+"'");
    qry->exec();

    // Update the dsSouvenirSelectionComboBox
    qry->prepare("SELECT Souvenir FROM Souvenirs WHERE TeamName = '"+placeholder+"'");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->dsSouvenirSelectionComboBox->setModel(modal);

    // Update the deleteSouvenirTreeView
    qry->prepare("SELECT * FROM Souvenirs ORDER BY TeamName ASC");
    qry->exec();
    QSqlQueryModel *modal2 = new QSqlQueryModel();
    modal2->setQuery(*qry);
    ui->deleteSouvenirTreeView->setModel(modal2);
}

// Go from Delete Souvenirs window to Admin window
void MainWindow::on_dsBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

/*
 * StackedWidget Page 6 -> Modify Arena Information
 */

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
    QString ArenaNamePlaceholder = ui->arenaSelectionComboBox->currentText();

    // Create a placeholder2 for the current TeamName (before its change)
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT * FROM Information WHERE ArenaName = '"+ArenaNamePlaceholder+"'");
    qry->exec();
    qry->first();
    QString TeamNamePlaceholder = qry->value(2).toString();

    // Update the row in the Information table where ArenaName == placeholder
    qry->prepare("UPDATE Information SET Conference = ?, Division = ?, TeamName = ?, Location = ?, ArenaName = ?, "
                 "StadiumCapacity = ?, JoinedLeague = ?, Coach = ? WHERE ArenaName = '"+ArenaNamePlaceholder+"' AND TeamName = '"+TeamNamePlaceholder+"'");
    qry->addBindValue(ui->conferenceLineEdit->text());          // Conference
    qry->addBindValue(ui->divisionLineEdit->text());            // Division
    qry->addBindValue(ui->teamNameLineEdit->text());            // TeamName
    qry->addBindValue(ui->locationLineEdit->text());            // Location
    qry->addBindValue(ui->arenaNameLineEdit->text());           // ArenaName
    qry->addBindValue(ui->stadiumCapacityLineEdit->text());     // StadiumCapacity
    qry->addBindValue(ui->joinedLeagueLineEdit->text());        // JoinedLeague
    qry->addBindValue(ui->coachLineEdit->text());               // Coach
    qry->exec();

    // Update the BeginningTeamName in the Distances table
    qry->prepare("UPDATE Distances SET BeginningTeamName = ? WHERE BeginningTeamName = '"+TeamNamePlaceholder+"'");
    qry->addBindValue(ui->teamNameLineEdit->text());
    qry->exec();

    // Update EndingTeamName in the Distances table
    qry->prepare("UPDATE Distances SET EndingTeamName = ? WHERE EndingTeamName = '"+TeamNamePlaceholder+"'");
    qry->addBindValue(ui->teamNameLineEdit->text());
    qry->exec();

    // Update the BeginningArena in the Distances table
    qry->prepare("UPDATE Distances SET BeginningArena = ? WHERE BeginningArena = '"+ArenaNamePlaceholder+"'");
    qry->addBindValue(ui->arenaNameLineEdit->text());
    qry->exec();

    // Update TeamName in the Souvenirs table
    qry->prepare("UPDATE Souvenirs SET TeamName = ? WHERE TeamName = '"+TeamNamePlaceholder+"'");
    qry->addBindValue(ui->teamNameLineEdit->text());
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



// Go from Modify Arena Information window to Admin window
void MainWindow::on_maiBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_startBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_DFS_clicked()
{
    // Create the query for the BeginningTeamName column in the Distances table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT BeginningTeamName FROM Distances ORDER BY Distance ASC");
    qry->exec();

    // the data in lists
    QStringList names, begTeam, endTeam;

    // So this is the way the team data getting sorted into
    // a list by unique team names
    bool exists;
    while(qry->next())
    {
        exists = false;
        if (names.indexOf(qry->value(0).toString()) >= 0)
        {
            exists = true;
        }
        if (names.size() == 0 || !exists)
        {
            names << qry->value(0).toString();
        }
    }

    // preparing beginning teams
    qry->prepare("SELECT BeginningTeamName FROM Distances ORDER BY Distance ASC");
    qry->exec();

    // populating beginning team list
    while(qry->next())
    {
        begTeam << qry->value(0).toString();
    }

    // preparing ending teams
    qry->prepare("SELECT EndingTeamName FROM Distances ORDER BY Distance ASC");
    qry->exec();

    // populating ending team list
    while(qry->next())
    {
        endTeam << qry->value(0).toString();
    }

    // preparing distance
    qry->prepare("SELECT Distance FROM Distances ORDER BY Distance ASC");
    qry->exec();

    // populating dicatance list
    QList<double> dist;
    while(qry->next())
    {
        dist << qry->value(0).toDouble();
    }

    // getting number of total rows
    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows = qry->at() + 1;
        qry->first();
        qry->previous();
    }

    // creating edges
    vector<DFSedge> DFSedges;
    for (int i = 0; i < numberOfRows; i++)
    {
        DFSedge edge = {names.indexOf(begTeam.at(i)), names.indexOf(endTeam.at(i)), dist.at(i)};
        DFSedges.push_back(edge);
    }

    // create a graph from given edges
    Graph graph(DFSedges, numberOfRows);

    // stores vertex is discovered or not
    vector<bool> discovered(numberOfRows);

    ui->searchOutput->setText("DFS: \n");

    DFS(graph, names.indexOf("Orlando Magic"), discovered, names);

    // formatting output
    QString str;
    for (int i = 0; i < graph.DFS.size(); i++)
    {
        str = graph.DFS.at(i);
        if (i == 0)
        {
            ui->searchOutput->append(str);
        }
        else
        {
            ui->searchOutput->append(" -> " + str);
        }
    }

    ui->searchOutput->append("\nTotal distance traveled: " + QString::number(graph.sum));

    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_BFS_clicked()
{
    // Create the query for the BeginningTeamName column in the Distances table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    // the data in lists
    QStringList names, begTeam, endTeam;

    // So this is the way the team data getting sorted into
    // a list by unique team names
    bool exists;
    while(qry->next())
    {
        exists = false;
        if (names.indexOf(qry->value(0).toString()) >= 0)
        {
            exists = true;
        }
        if (names.size() == 0 || !exists)
        {
            names << qry->value(0).toString();
        }
    }

    // preparing beginning teams
    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    // populating beginning team list
    while(qry->next())
    {
        begTeam << qry->value(0).toString();
    }

    // preparing ending teams
    qry->prepare("SELECT EndingTeamName FROM Distances");
    qry->exec();

    // populating ending team list
    while(qry->next())
    {
        endTeam << qry->value(0).toString();
    }

    // preparing distance
    qry->prepare("SELECT Distance FROM Distances");
    qry->exec();

    // populating distance list
    QList<double> dist;
    while(qry->next())
    {
        if (qry->value(0).toDouble() == 0.0)
            dist << qry->value(0).toDouble() + 10.0;
        else dist << qry->value(0).toDouble();
    }

    // getting number of total rows
    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows = qry->at() + 1;
        qry->first();
        qry->previous();
    }

    // creating edges
    vector<BFSedge> BFSedges;
    for (int i = 0; i < numberOfRows; i++)
    {
        BFSedge edge = {names.indexOf(begTeam.at(i)), names.indexOf(endTeam.at(i)), dist.at(i)};
        BFSedges.push_back(edge);
    }

    // creating adjacent matrix
    vector< vector<double> > adjMatrix = FormAdjMatrix(BFSedges, names.size());

    // create a graph from given edges
    Temp temp;

    // stores vertex is discovered or not
    //vector<bool> discovered(numberOfRows);

    ui->searchOutput->setText("BFS: \n");

    BFS(temp, adjMatrix, names.indexOf("Los Angeles Lakers"), names);

    // formatting output
    QString str;
    for (int i = 0; i < temp.BFS.size(); i++)
    {
        str = temp.BFS.at(i);
        if (i == 0)
        {
            ui->searchOutput->append(str);
        }
        else
        {
            ui->searchOutput->append(" -> " + str);
        }
    }

    ui->searchOutput->append("\nTotal distance traveled: " + QString::number(temp.sum - 10));

    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_DFSBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_MST_clicked()
{
    // Create the query for the BeginningTeamName column in the Distances table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    // the data in lists
    QStringList arenas, names, begTeam, endTeam;

    // So this is the way the team data getting sorted into
    // a list by unique team names
    bool exists;
    while(qry->next())
    {
        exists = false;
        if (names.indexOf(qry->value(0).toString()) >= 0)
        {
            exists = true;
        }
        if (names.size() == 0 || !exists)
        {
            names << qry->value(0).toString();
        }
    }

    // preparing beginning teams
    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    while(qry->next())
    {
        begTeam << qry->value(0).toString();
    }

    // preparing ending teams
    qry->prepare("SELECT EndingTeamName FROM Distances");
    qry->exec();

    while(qry->next())
    {
        endTeam << qry->value(0).toString();
    }

    // preparing distance
    qry->prepare("SELECT Distance FROM Distances");
    qry->exec();

    QList<double> dist;

    while(qry->next())
    {
        dist << qry->value(0).toDouble();
    }

    // getting number of total rows
    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows = qry->at() + 1;
        qry->first();
        qry->previous();
    }

    // preparing beginning arenas
    qry->prepare("SELECT BeginningArena FROM Distances");
    qry->exec();

    // So this is the way the arena data getting sorted into
    // a list by unique arena names
    int count = 0;
    bool exists2;
    while(qry->next())
    {
        exists2 = false;
        if (arenas.indexOf(qry->value(0).toString()) >= 0 && (begTeam.indexOf(begTeam.at(count)) < count))
        {
            exists2 = true;
        }
        if (arenas.size() == 0 || !exists2)
        {
            arenas << qry->value(0).toString();
        }
        count++;
    }

    // creating edges
    vector<MSTedge> MSTedges;
    for (int i = 0; i < numberOfRows; i++)
    {
        MSTedge edge = {names.indexOf(begTeam.at(i)), names.indexOf(endTeam.at(i)), dist.at(i)};
        MSTedges.push_back(edge);
    }

    // initializing MSTGraph struct
    MSTGraph g(numberOfRows);

    // adding edges
    for (auto &edge: MSTedges)
    {
        g.addEdge(edge.src, edge.dest, edge.dist);
    }

    ui->searchOutput->clear();
    ui->searchOutput->append("Edges of MST are:\n");

    // finding edes
    double mst_wt = g.kruskalMST(arenas);

    // printing out edges
    for (int i = 0; i < g.MSTedge1.size(); i++)
    {
        ui->searchOutput->append(g.MSTedge1.at(i) + " - " + g.MSTedge2.at(i));
    }

    //printing out total weight
    ui->searchOutput->append("\nWeight of MST is " + QString::number(mst_wt));

    ui->stackedWidget->setCurrentIndex(8);
}

void MainWindow::on_planATrip_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_denverNuggetsTrip_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Trip");
    qry->exec();

    qry->prepare("DELETE FROM shortestDist");
    qry->exec();

    ui->teamsVisited->clear();
    ui->teamsChosen->clear();
    ui->finalTotalDist->clear();

    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the arenaSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->tripSelection->setModel(modal);

    qry->prepare("INSERT INTO Trip (TeamName) VALUES ('Denver Nuggets')");
    qry->exec();

    ui->teamsChosen->append("Denver Nuggets");
    ui->orderPreservedCheck->setChecked(false);

    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_tripSelection_currentIndexChanged(const QString &arg1)
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows = qry->at() + 1;
        qry->first();
        qry->previous();
    }

    qry->next();
    if (qry->value(0).toString() == "Easter Egg")
    {
        qry->prepare("DELETE FROM Trip");
        qry->exec();

        ui->teamsChosen->append(arg1);

        qry->prepare("INSERT INTO Trip (TeamName) VALUES ('"+arg1+"')");
        qry->exec();
    }
    else if (numberOfRows != 0)
    {
        ui->teamsChosen->append(arg1);

        qry->prepare("INSERT INTO Trip (TeamName) VALUES ('"+arg1+"')");
        qry->exec();
    }
}

void MainWindow::on_startTrip_clicked()
{
    // Create the query for the TeamName column in the Trip table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    qry->next();
    QString str = qry->value(0).toString();

    ui->teamNameDisplay->setText(str);
    ui->teamsVisited->append(str);
    ui->distFromPrevTeam->setText("0");
    ui->totalDistance->setText("0");



    // Set the souvenirs tree to display the first team's souvenirs
    qry->prepare("SELECT Souvenir, Price FROM Souvenirs WHERE TeamName = '"+str+"'");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->souvenirTreeView->setModel(modal);



    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_tripBack_clicked()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Trip");
    qry->exec();

    ui->teamsChosen->clear();

    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_undo_clicked()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    // getting number of total rows
    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows = qry->at() + 1;
        qry->first();
        qry->previous();
    }

    qry->prepare("DELETE FROM Trip WHERE id = '" + QString::number(numberOfRows) + "'");
    qry->exec();

    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    ui->teamsChosen->clear();
    while(qry->next())
    {
        ui->teamsChosen->append(qry->value(0).toString());
    }
}

void MainWindow::on_abandonTrip_clicked()
{
    QSqlQuery qry;

    if(db.isOpen())
    {
    qry.exec("DELETE FROM ShoppingCart");
    }
    else
    {
      db.open();
      qry.exec("DELETE FROM ShoppingCart");
    }

    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_nextTeam_clicked()
{
    // Create the query for the BeginningTeamName column in the Distances table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    // the data in lists
    QStringList names, begTeam, endTeam;

    // So this is the way the team data getting sorted into
    // a list by unique team names
    bool exists;
    while(qry->next())
    {
        exists = false;
        if (names.indexOf(qry->value(0).toString()) >= 0)
        {
            exists = true;
        }
        if (names.size() == 0 || !exists)
        {
            names << qry->value(0).toString();
        }
    }

    // preparing beginning teams
    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    // populating beginning team list
    while(qry->next())
    {
        begTeam << qry->value(0).toString();
    }

    // preparing ending teams
    qry->prepare("SELECT EndingTeamName FROM Distances");
    qry->exec();

    // populating ending team list
    while(qry->next())
    {
        endTeam << qry->value(0).toString();
    }

    // preparing distance
    qry->prepare("SELECT Distance FROM Distances");
    qry->exec();

    // populating distance list
    QList<double> dist;
    while(qry->next())
    {
        dist << qry->value(0).toDouble();
    }

    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    qry->next();
    QString str = qry->value(0).toString();

    qry->prepare("DELETE FROM Trip WHERE id = 1");
    qry->exec();

    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    QStringList teams;

    while(qry->next())
    {
        teams << qry->value(0).toString();
    }

    qry->prepare("DELETE FROM Trip");
    qry->exec();

    if (teams.size() >= 1)
    {
        for (int i = 0; i < teams.size(); i++)
        {
            qry->prepare("INSERT INTO Trip (TeamName) VALUES ('"+teams.at(i)+"')");
            qry->exec();
        }

        vector<iPairD > adj[names.size()];
        Shortest smallest;

        for (int i = 0; i < dist.size(); ++i)
            addEdge(adj, names.indexOf(begTeam.at(i)), names.indexOf(endTeam.at(i)), dist.at(i));

        if (ui->orderPreservedCheck->isChecked())
        {
            QString str = teams.at(0);
            teams.clear();
            teams << str;
        }

        double shortDist = shortestPath(smallest, adj, names.size(), names.indexOf(str), names, teams);

        ui->teamNameDisplay->setText(smallest.small);
        ui->teamsVisited->append(smallest.small);
        ui->distFromPrevTeam->setText(QString::number(shortDist));



        // Display the team's souvenirs in the souvenir tree
        qry->prepare("SELECT Souvenir, Price FROM Souvenirs WHERE TeamName = '"+smallest.small+"'");
        qry->exec();
        QSqlQueryModel *modal = new QSqlQueryModel();
        modal->setQuery(*qry);
        ui->souvenirTreeView->setModel(modal);

        // Reset team total to 0
        ui->teamPriceTotal->setText("0");



        if (!ui->orderPreservedCheck->isChecked())
        {
            QStringList teamsRevised;

            teamsRevised << smallest.small;
            for (int i = 0; i < teams.size(); i++)
            {
                if (teams.at(i) != smallest.small)
                    teamsRevised << teams.at(i);
            }

            qry->prepare("DELETE FROM Trip");
            qry->exec();

            for (int i = 0; i < teamsRevised.size(); i++)
            {
                qry->prepare("INSERT INTO Trip (TeamName) VALUES ('"+teamsRevised.at(i)+"')");
                qry->exec();
            }
        }
        else
        {
            teams.clear();
            qry->prepare("SELECT TeamName FROM Trip");
            qry->exec();

            while(qry->next())
            {
                teams << qry->value(0).toString();
            }
        }

        qry->prepare("SELECT shortDist FROM shortestDist");
        qry->exec();

        int numberOfRows = 0;
        if(qry->last())
        {
            numberOfRows = qry->at() + 1;
            qry->first();
            qry->previous();
        }

        double total= 0.0;
        if (numberOfRows > 0)
        {
            qry->next();
            total = qry->value(0).toDouble() + shortDist;
        }
        else total = shortDist;

        ui->totalDistance->setText(QString::number(total));

        qry->prepare("DELETE FROM shortestDist");
        qry->exec();

        qry->prepare("INSERT INTO shortestDist (shortDist) VALUES ('"+QString::number(total)+"')");
        qry->exec();
    }

    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    // getting number of total rows
    int numberOfRows = 0;
    if(qry->last())
    {
        numberOfRows = qry->at() + 1;
        qry->first();
        qry->previous();
    }

    qry->prepare("SELECT shortDist FROM shortestDist");
    qry->exec();
    qry->next();

    if (numberOfRows == 0)
    {
        ui->finalTotalDist->setText(QString::number(qry->value(0).toDouble()));

        // Set the finalTotalPurch
        ui->finalTotalPurch->setText(ui->teamGrandTotal->text());

        // Set purchases tree view
        qry->prepare("SELECT * FROM ShoppingCart");
        qry->exec();
        QSqlQueryModel *model = new QSqlQueryModel;
        model->setQuery(*qry);
        ui->treeView_shoppingCart->setModel(model);

        ui->stackedWidget->setCurrentIndex(11);
    }
}

void MainWindow::on_finishButton_clicked()
{
    QSqlQuery qry;

    if(db.isOpen())
    {
    qry.exec("DELETE FROM ShoppingCart");
    }
    else
    {
      db.open();
      qry.exec("DELETE FROM ShoppingCart");
    }

    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_detroitPistonsTrip_clicked()
{
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Trip");
    qry->exec();

    qry->prepare("DELETE FROM shortestDist");
    qry->exec();

    ui->teamsVisited->clear();
    ui->teamsChosen->clear();
    ui->finalTotalDist->clear();

    qry->prepare("INSERT INTO Trip (TeamName) VALUES ('Detroit Pistons')");
    qry->exec();

    qry->prepare("SELECT TeamName FROM Trip");
    qry->exec();

    qry->next();
    QString str = qry->value(0).toString();

    ui->teamNameDisplay->setText(str);
    ui->teamsVisited->append(str);
    ui->distFromPrevTeam->setText("0");
    ui->totalDistance->setText("0");



    // Set the souvenirs tree to display the first team's souvenirs
    qry->prepare("SELECT Souvenir, Price FROM Souvenirs WHERE TeamName = '"+str+"'");
    qry->exec();
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->souvenirTreeView->setModel(modal);



    qry->prepare("SELECT BeginningTeamName FROM Distances");
    qry->exec();

    // the data in lists
    QStringList names;

    // So this is the way the team data getting sorted into
    // a list by unique team names
    bool exists;
    while(qry->next())
    {
        exists = false;
        if (names.indexOf(qry->value(0).toString()) >= 0)
        {
            exists = true;
        }
        if (names.size() == 0 || !exists)
        {
            names << qry->value(0).toString();
        }
    }

    for (int i = 0; i < names.size(); i++)
    {
        if (names.at(i) != "Detroit Pistons")
        {
            qry->prepare("INSERT INTO Trip (TeamName) VALUES ('"+names.at(i)+"')");
            qry->exec();
        }
    }

    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_anyToAny_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("DELETE FROM Trip");
    qry->exec();

    qry->prepare("DELETE FROM shortestDist");
    qry->exec();

    ui->teamsVisited->clear();
    ui->teamsChosen->clear();
    ui->finalTotalDist->clear();

    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the arenaSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->tripSelection->setModel(modal);

    qry->prepare("INSERT INTO Trip (TeamName) VALUES ('Easter Egg')");
    qry->exec();

    ui->orderPreservedCheck->setChecked(false);

    ui->stackedWidget->setCurrentIndex(9);
}

// This button will pop up a window that lets the user know the total seating capacity
// of all the stadiums in the NBA
void MainWindow::on_pushButton_totalSeatingCapacity_clicked()
{
    // Create a query for the total seating capacity
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT SUM(StadiumCapacity) FROM Information");
    qry->exec();
    qry->first();

    // Display a box to allow the user to see the total seating capacity
    QString total = "The total seating capacity of the NBA teams is: " + qry->value(0).toString();
    QMessageBox::about(this,tr("Total Seating Capacity"),total);
}

// This button will take you from the information page to the main page
void MainWindow::on_infoBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// This button will take you from the single team display page to the info page
void MainWindow::on_singleTeamBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

// This button will take you from the team display page to the info page
void MainWindow::on_nbaTeamsBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

// This button will take you from the souvenirs display page to the info page
void MainWindow::on_souvenirsBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

// This button will prepare the single team display page, and take you
// from the info page to the single team display page
void MainWindow::on_pushButton_singleTeam_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the teamSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->singleTeamSelectionComboBox->setModel(modal);

    // Go to single team display page
    ui->stackedWidget->setCurrentIndex(13);
}

// On the single team display page, adjust the labels according to the team the user
// selects from the singleTeamSelectionComboBox
void MainWindow::on_singleTeamSelectionComboBox_currentIndexChanged(const QString &arg1)
{
    // Create the query for the row in the Information table where TeamName == arg1
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT * FROM Information WHERE TeamName = '"+arg1+"'");
    qry->exec();

    // Fill the lineEdits on this window with the queried data
    qry->first();
    ui->displayTeamNameLabel->setText("Go, " + qry->value(2).toString() + "!");                 // Team Name
    ui->arenaNameLabel->setText("Arena Name: " + qry->value(4).toString());                     // Arena Name
    ui->arenaSeatingCapacityLabel->setText("Stadium Capacity: " + qry->value(5).toString());    // Stadium Capacity
    ui->locationLabel->setText("Location: " + qry->value(3).toString());                        // Location
    ui->conferenceLabel->setText("Conference: " + qry->value(0).toString());                    // Conference
    ui->divisionLabel->setText("Division: " + qry->value(1).toString());                        // Division
    ui->joinedLeagueLabel->setText("Joined League: " + qry->value(6).toString());               // Joined League
    ui->coachLabel->setText("Coach: " + qry->value(7).toString());                              // Coach
}

// This button will prepare the nba teams display page, and take you
// from the info page to the nba teams display page
void MainWindow::on_pushButton_nbaTeams_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("NBA Teams");
    ui->totalSeatingCapacityLabel->hide();

    // Query for the team names from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the nba arenas display page, and take you
// from the info page to the nba arenas display page
void MainWindow::on_pushButton_nbaArenas_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("NBA Arenas & Teams");
    ui->totalSeatingCapacityLabel->hide();

    // Query for the arena and team names from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT ArenaName, TeamName FROM Information ORDER BY ArenaName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the eastern conference teams display page, and take you
// from the info page to the easter conference teams display page
void MainWindow::on_pushButton_easternConferenceTeams_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("Eastern Conference Teams");
    ui->totalSeatingCapacityLabel->hide();

    // Query for the team names from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information WHERE Conference = 'Eastern' ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the southeast division teams display page, and take you
// from the info page to the southeast division teams display page
void MainWindow::on_pushButton_southeastDivisionTeams_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("Southeast Division Teams");
    ui->totalSeatingCapacityLabel->hide();

    // Query for the team names from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information WHERE Division = 'Southeast' ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the coaches and teams display page, and take you
// from the info page to the coaches and teams display page
void MainWindow::on_pushButton_nbaCoaches_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("NBA Coaches and Teams");
    ui->totalSeatingCapacityLabel->hide();

    // Query for the team names from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName, Coach FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the arenas, teams, seating capacities display page, and
// take you to the arenas, teams, seating capacities display page
void MainWindow::on_pushButton_nbaArenasTeamsCapacities_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("NBA Arenas, Teams, and Seating Capacities");
    ui->totalSeatingCapacityLabel->show();

    // Query for the arenas, teams, and seating capacities from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName, ArenaName, StadiumCapacity FROM Information ORDER BY StadiumCapacity ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Create a query for the total seating capacity
    qry->prepare("SELECT SUM(StadiumCapacity) FROM Information");
    qry->exec();
    qry->first();

    // Set the side label to the appropriate text
    QString total = "Total Seating Capacity: " + qry->value(0).toString();
    ui->totalSeatingCapacityLabel->setText(total);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the teams, arenas, years display page, and take you
// to the teams, arenas, years display page
void MainWindow::on_pushButton_teamsArenasYears_clicked()
{
    // Set the header label to the appropriate text
    ui->ambiguousLabel->setText("NBA Teams, Arenas, and Years");
    ui->totalSeatingCapacityLabel->hide();

    // Query for the team names from the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName, ArenaName, JoinedLeague FROM Information ORDER BY JoinedLeague ASC");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_nbaTeams->setModel(modal);

    // Go to nba teams display page
    ui->stackedWidget->setCurrentIndex(14);
}

// This button will prepare the souvenirs display page, and take you
// to the souvenirs display page
void MainWindow::on_pushButton_souvenirs_clicked()
{
    // Create the query for the TeamName column in the Information table
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT TeamName FROM Information ORDER BY TeamName ASC");
    qry->exec();

    // Create the modal to hold the query and set the teamSelectionComboBox
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->teamSelectionComboBox_souvenirs->setModel(modal);

    // Go to single team display page
    ui->stackedWidget->setCurrentIndex(15);
}

// On the souvenirs display page, adjust the label and treeView according to the team the user
// selects from the teamSelectionComboBox_souvenirs
void MainWindow::on_teamSelectionComboBox_souvenirs_currentIndexChanged(const QString &arg1)
{
    // Create the query for the row(s) in the Souvenirs table where TeamName == arg1
    QSqlQuery *qry = new QSqlQuery(db);
    qry->prepare("SELECT Souvenir, Price FROM Souvenirs WHERE TeamName = '"+arg1+"'");
    qry->exec();

    // Create the modal to hold the new query and set the treeView_nbaTeams
    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery(*qry);
    ui->treeView_souvenirs->setModel(modal);

    // Fill the lineEdits on this window with the queried data
    ui->displayTeamNameLabel_2->setText(arg1 + " Souvenirs");
}

// Takes the user from main window to the info page
void MainWindow::on_pushButton_Information_clicked()
{
    ui->stackedWidget->setCurrentIndex(12);
}

// When a souvenir is double clicked from the trip page, open up a pop-up box...
// Display the souvenir name on the box, and take in from the user how many of
// that item they wish to purchase...
// When the enter button is clicked, add that amount of that item to the shoppingCart table
void MainWindow::on_souvenirTreeView_doubleClicked(const QModelIndex &index)
{
    // Creates the admin dialog window
    souvenirsDialog *numOfSouvenirs;
    numOfSouvenirs = new souvenirsDialog();

    // This is necessary because the tree displays 2 columns, and we need to pay attention
    // to which column the user double-clicked on -> basically this ensures that we're working
    // with the columns reading from left to right regardless of where the user clicked
    QModelIndex newIndex;

    int row = index.row();
    int col = index.column();

    if( col > 0 ) {
        newIndex = index.sibling(row, 0);
    }
    else {
        newIndex = index.sibling(row, col);
    }

    QString col1Data = newIndex.data().toString();  // Col1 @ Row# -> the souvenir name
    QString col2Data = newIndex.sibling(row, 1).data().toString();  // Col2 @ Row# -> the souvenir price

    // Set the numOfSouvenir's souvenir label to the name of the souvenir the user selected
    numOfSouvenirs->setLabel(col1Data);

    // Sets the modal for the admin dialog window, then executes
    numOfSouvenirs->setModal(true);
    numOfSouvenirs->exec();

    // Take the # of souvenirs to be purchased from the dialog box, and add that many to the
    // shopping cart table
    QSqlQuery *qry = new QSqlQuery(db);
    int num = numOfSouvenirs->getNum();
    for(int i = 0; i < num; i++) {
        qry->prepare("INSERT INTO ShoppingCart (TeamName, Souvenir, Price) VALUES (?, ?, ?)");
        qry->addBindValue(ui->teamNameDisplay->text());
        qry->addBindValue(col1Data);
        qry->addBindValue(col2Data);
        qry->exec();
    }

    // Update the totalPrice box
    QString priceStr = col2Data;
    priceStr.remove('$');
    double price = num * priceStr.toDouble();
    double price2 = num * priceStr.toDouble();

    // Take the price from text box and add
    price = price + ui->teamPriceTotal->text().toDouble();
    price2 = price2 + ui->teamGrandTotal->text().toDouble();

    priceStr = QString::number(price);
    QString priceStr2 = QString::number(price2);

    // Reset textbox
    ui->teamPriceTotal->setText(priceStr);
    ui->teamGrandTotal->setText(priceStr2);
}
