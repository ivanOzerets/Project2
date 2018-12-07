#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "admindialog.h"
#include "map.h"
#include "dfs.h"
#include "bfs.h"
#include "mst.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Open the database -> you'll need to specify the path it's under on your computer
    DB_Path = "C:\\Users\\Ivan\\School\\Saddleback\\CS1D\\Project2\\SQLite\\DB\\NBAinfo.db";
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
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', 'Autographed Basketball', '$49.89')");
            qry->exec();
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', 'Team Pennant', '$17.99')");
            qry->exec();
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', 'Team Picture', '$29.99')");
            qry->exec();
            qry->prepare("INSERT INTO Souvenirs (TeamName, Souvenir, Price) VALUES ('"+list[2]+"', 'Team Jersey', '$179.99')");
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

// Go from Modify Arena Information window to Admin window
void MainWindow::on_maiBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_startBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_DFS_clicked()
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
    qry->prepare("SELECT Distance * 10 FROM Distances");
    qry->exec();

    // populating dicatance list
    QList<double> dist;
    while(qry->next())
    {
        dist << qry->value(0).toInt() / 10.0;
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

    ui->DFS_output->setText("DFS: \n");

    DFS(graph, names.indexOf("Orlando Magic"), discovered, names);

    // formatting output
    QString str;
    for (int i = 0; i < graph.DFS.size(); i++)
    {
        str = graph.DFS.at(i);
        if (i == 0)
        {
            ui->DFS_output->append(str);
        }
        else
        {
            ui->DFS_output->append(" -> " + str);
        }
    }

    ui->DFS_output->append("\nTotal distance traveled: " + QString::number(graph.sum));

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
    qry->prepare("SELECT Distance * 10 FROM Distances");
    qry->exec();

    // populating distance list
    QList<double> dist;
    while(qry->next())
    {
        dist << qry->value(0).toInt() / 10.0;
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

    ui->BFS_output->setText("BFS: \n");

    BFS(temp, adjMatrix, names.indexOf("Los Angeles Lakers"), names);

    // formatting output
    QString str;
    for (int i = 0; i < temp.BFS.size(); i++)
    {
        str = temp.BFS.at(i);
        if (i == 0)
        {
            ui->BFS_output->append(str);
        }
        else
        {
            ui->BFS_output->append(" -> " + str);
        }
    }

    ui->BFS_output->append("\nTotal distance traveled: " + QString::number(temp.sum));

    ui->stackedWidget->setCurrentIndex(9);
}

void MainWindow::on_BFSBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
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
    qry->prepare("SELECT Distance * 10 FROM Distances");
    qry->exec();

    QList<double> dist;

    while(qry->next())
    {
        dist << qry->value(0).toInt() / 10.0;
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

    ui->MST_output->clear();
    ui->MST_output->append("Edges of MST are:\n");

    // finding edes
    double mst_wt = g.kruskalMST(arenas);

    // printing out edges
    for (int i = 0; i < g.MSTedge1.size(); i++)
    {
        ui->MST_output->append(g.MSTedge1.at(i) + " - " + g.MSTedge2.at(i));
    }

    //printing out total weight
    ui->MST_output->append("\nWeight of MST is " + QString::number(mst_wt));

    ui->stackedWidget->setCurrentIndex(10);
}

void MainWindow::on_MSTBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}
