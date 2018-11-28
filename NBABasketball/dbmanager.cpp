#include "dbmanager.h"
#include <QDir>
#include <QSqlError>

DbManager::DbManager()
{
    //qDebug() << "DB constructor called";
    database = QSqlDatabase::addDatabase("QSQLITE");

    //QString databasePath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    QDir databaseDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    databaseDir.mkdir(DB_SUBFOLDER);
    databaseDir.cd(DB_SUBFOLDER);

    qDebug() << "Looking here for the database: " << databaseDir.absoluteFilePath(DB_FILENAME);


    if(QFile::exists(databaseDir.absoluteFilePath(DB_FILENAME)))
    {
        database.setDatabaseName(databaseDir.absoluteFilePath(DB_FILENAME));
        database.setConnectOptions(databaseDir.absoluteFilePath(DB_FILENAME));

        if(!database.open())
        {
            qDebug() << "Error: Connection failed";
        }
        else
        {
            qDebug() << "Datbase connection successful";
        }
    }
    else{
        qDebug() << "Database file not found";
    }
}

DbManager::~DbManager()
{
    if(database.isOpen())
    {
        database.close();
    }

}

DbManager& DbManager::instance()
{
    static DbManager dmInstance;
    return dmInstance;
}



/**********************
 * DATABASE ACCESSORS *
 **********************/

bool DbManager::isOpen() const
{
    return database.isOpen();
}

DbManager::LoginResult DbManager::LoginCheck(QString userName, QString password) const
{
    DbManager::LoginResult status = LoginResult::fail;
    QSqlQuery query;

    query.prepare(
        "SELECT isAdmin "
        "FROM credentials "
        "WHERE username = (:name) AND password = (:pass)"
    );
    query.bindValue(":name", userName);
    query.bindValue(":pass", password);

    if(query.exec())
    {
        int isAdminIndex = query.record().indexOf("isAdmin");

        if(query.next())
        {
            if(query.value(isAdminIndex).toBool())
            {
                status = LoginResult::admin;
            }
            else
            {
                status = LoginResult::student;
            }
        }
    }

    return status;
}

QStringList DbManager::teamNames() const
{
    QStringList nameList;

    QSqlQuery query("SELECT TeamName FROM Information");

    while(query.next()) {
        int nameIndex = query.record().indexOf("TeamName");
        nameList.append(query.value(nameIndex).toString());
    }
    return nameList;
}
QStringList DbManager::arenaNames() const
{
    QStringList nameList;

    QSqlQuery query("SELECT BeginningArena FROM Distances");

    while(query.next()) {
        int nameIndex = query.record().indexOf("BeginningArena");
        nameList.append(query.value(nameIndex).toString());
    }
    return nameList;
}

//Obtains adjacent edge lengths for a specific team
QVector<float> DbManager::getAdjDistance(QString teamName) const
{
    QMap<QString, double> souvenirs;
    QVector<float> distances;
    float dist;

    QStringList test;

    QSqlQuery query;

    query.prepare(
         "SELECT Distance"
         "FROM  Distances"
         "WHERE BeginningTeamName = (SELECT TeamName FROM Information WHERE TeamName = (:name)"

    );
    query.bindValue(":name", teamName);
    //query.bindValue(":name", dist);

    if(query.exec())
    {
        while(query.next())
        {
            int distanceIndex  = query.record().indexOf("Distance");
            //int priceIndex = query.record().indexOf("price");

            //qDebug() << nameIndex;

            souvenirs.insert(
                query.value(distanceIndex).toString(),
            //    query.value(priceIndex).toDouble());

            //qDebug() << souvenirs.firstKey() << endl
            //         << souvenirs.first()    << endl << endl;
            dist = query.value(distanceIndex).toFloat());
            distances.push_back(dist);
        }
    }
    return distances;
}

QMap<QString, double> DbManager::getSouvenirs(QString campusName) const
{
    QMap<QString, double> souvenirs;

    QSqlQuery query;

    query.prepare(
        "SELECT souvenirName, price "
        "FROM souvenir_info "
        "WHERE id = (SELECT id FROM college_info WHERE name = (:name))"
    );
    query.bindValue(":name", campusName);

    if(query.exec())
    {
        while(query.next())
        {
            int nameIndex  = query.record().indexOf("souvenirName");
            int priceIndex = query.record().indexOf("price");

            //qDebug() << nameIndex;

            souvenirs.insert(
                query.value(nameIndex).toString(),
                query.value(priceIndex).toDouble());

            //qDebug() << souvenirs.firstKey() << endl
            //         << souvenirs.first()    << endl << endl;

        }
    }
    return souvenirs;
}

double DbManager::getDistance(QString campus1, QString campus2) const
{
    // TODO This is rather inefficient. It should just use its own
    //      SQL query rather than building an entire map and then
    //      simply pulling one value out.
    return getAllDistancesFrom(campus1).value(campus2, -1);
}

QMap<QString, double> DbManager::getAllDistancesFrom(QString start) const
{
    QMap<QString, double> distances;

    int startCampusID = getCampusID(start);

    if(startCampusID != -1)
    {
        QSqlQuery query;

        query.prepare(
            "SELECT id1, id2, dist "
            "FROM distances "
            "WHERE id1 = (SELECT id FROM college_info WHERE name = (:name))"
            "   OR id2 = (SELECT id FROM college_info WHERE name = (:name))"
        );
        query.bindValue(":name", start);

        if(query.exec())
        {
            while(query.next())
            {
                int id1Index = query.record().indexOf("id1");
                int id2Index = query.record().indexOf("id2");
                int distIndex = query.record().indexOf("dist");

                int id1 = query.value(id1Index).toInt();
                int id2 = query.value(id2Index).toInt();
                double dist = query.value(distIndex).toDouble();

                QString name = "";

                // If both id1 and id2 equal the start ID (i.e. we're going
                // from self to self), name will stay unset.
                if(id1 != startCampusID)
                {
                    name = getCampusName(id1);
                }
                else if(id2 != startCampusID)
                {
                    name = getCampusName(id2);
                }

                // If name is unset because the other ID is invalid or both
                // IDs are identical, then name will never get set.
                if(name != "")
                {
                    distances.insert(name, dist);
                }

            } // End while(query.next())
        } // End if(query.exec())
    } // End if(campusID != -1)

    return distances;
}



/*********************
 * DATABASE MUTATORS *
 *********************/

bool DbManager::addCampus(QString name)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO college_info (name) VALUES (:name);"
    );
    query.bindValue(":name", name);

    return query.exec();
}

bool DbManager::deleteCampus(QString name)
{
    int id = getCampusID(name);
    bool success = false;

    if(id != -1)
    {
        QSqlQuery query;

        query.prepare(QString("DELETE FROM souvenir_info WHERE id = ") + QString::number(id));
        success = query.exec();
        query.finish();

        query.prepare(QString("DELETE FROM college_info WHERE id = ") + QString::number(id));
        success = query.exec();
        query.finish();


        query.prepare(
            QString("DELETE FROM distances WHERE (id1 = ")
                    + QString::number(id)
                    + QString(" OR id2 = ")
                    + QString::number(id)
                    + QString(")")
        );
        success = query.exec();
        query.finish();
    }

    return success;
}

bool DbManager::changeCampusName(QString oldName, QString newName)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE college_info SET name = (:newName) WHERE name = (:oldName);"
    );
    query.bindValue(":oldName", oldName);
    query.bindValue(":newName", newName);

    return query.exec();
}

bool DbManager::changeCampusSouvenirs(QString name, QMap<QString, double> souvenirs)
{
    int id = getCampusID(name);
    bool success = false;

    if(id != -1)
    {
        QSqlQuery query;

        query.prepare(QString("DELETE FROM souvenir_info WHERE id = ") + QString::number(id));
        success = query.exec();
        query.finish();

        for(QMap<QString, double>::const_iterator it = souvenirs.cbegin(); it != souvenirs.cend(); it++) {

            query.prepare(
                QString("INSERT INTO souvenir_info (id, souvenirName, price) VALUES (") +
                QString::number(id)			+ QString(", '") +
                QString(it.key())			+ QString("', ") +
                QString::number(it.value())	+ QString(")")
            );
            success = query.exec() && success;
            query.finish();

        }

    }

    return success;
}

bool DbManager::changeSouvenirName(QString campus, QString oldName, QString newName)
{
    int id = getCampusID(campus);
    bool success = false;

    if(id != -1 && newName != "" && !getSouvenirs(campus).contains(newName))
    {
        QSqlQuery query;

        query.prepare(
            QString("UPDATE souvenir_info ")
            + QString("SET souvenirName = '")			+ newName
            + QString("' WHERE (")
                    + QString("id = ")					+ QString::number(id)
                    + QString(" AND souvenirName = '")	+ oldName
            + QString("')")
        );
        success = query.exec();
    }

    return success;
}

bool DbManager::changeSouvenirPrice(QString campus, QString name, double newPrice)
{
    int id = getCampusID(campus);
    bool success = false;

    if(id != -1)
    {
        QSqlQuery query;

        query.prepare(
            QString("UPDATE souvenir_info ")
            + QString("SET price = '")			+ QString::number(newPrice)
            + QString("' WHERE (")
                    + QString("id = ")					+ QString::number(id)
                    + QString(" AND souvenirName = '")	+ name
            + QString("')")
        );
        success = query.exec();
    }

    return success;
}

bool DbManager::addSouvenir(QString campus, QString name, double price)
{
    int id = getCampusID(campus);
    bool success = false;

    if(id != -1 && name != "" && !getSouvenirs(campus).contains(name))
    {
        QSqlQuery query;

        query.prepare(
            QString("INSERT INTO souvenir_info ('id', 'souvenirName', 'price') ")
            + QString("VALUES (")
                + QString("'") + QString::number(id)	+ QString("', ")
                + QString("'") + name					+ QString("', ")
                + QString("'") + QString::number(price)	+ QString("'")
            + QString(")")
        );
        success = query.exec();
    }

    return success;
}

bool DbManager::deleteSouvenir(QString campus, QString name)
{
    int id = getCampusID(campus);
    bool success = false;

    if(id != -1 && name != "")
    {
        QSqlQuery query;

        query.prepare(
            QString("DELETE FROM souvenir_info ")
            + QString("WHERE (")
                + QString("id = '") + QString::number(id)	+ QString("'")
                + QString(" AND ")
                + QString("souvenirName = '") + name		+ QString("'")
            + QString(")")
        );
        success = query.exec();
    }

    return success;
}

bool DbManager::changeCampusDistance(QString name1, QString name2, double distance)
{
    bool success = false;

    if(name1 != name2)
    {
        int id1 = getCampusID(name1);
        int id2 = getCampusID(name2);

        if(id1 != -1 && id2 != -1)
        {
            QSqlQuery query;

            if(getDistance(name1, name2) > 0)
            {
                query.prepare(
                    QString("UPDATE distances SET dist = ")	+ QString::number(distance) +
                    QString(" WHERE (id1 = ")				+ QString::number(id1) +
                    QString(" AND id2 = ")					+ QString::number(id2) +
                    QString(") OR (id1 = ")					+ QString::number(id2) +
                    QString(" AND id2 = ")					+ QString::number(id1) +
                    QString(")")
                );
            }
            else
            {
                if(id1 > id2)
                {
                    std::swap(id1, id2);
                }

                query.prepare(
                    QString("INSERT INTO distances (id1, id2, dist) VALUES (") +
                    QString::number(id1)		+ QString(", ") +
                    QString::number(id2)		+ QString(", ") +
                    QString::number(distance)	+ QString(")")
                );
            }

            success = query.exec();
            query.finish();
        }
    }

    return success;
}


void DbManager::readFromFile(QString path)
{
    QFile file(path);
    FileReadMode mode = FileReadMode::unknown;
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        //QString line = in.readLine();
        QStringList fields = in.readLine().split("\t");

        //qDebug() << fields;

        if(fields.length() == 1) {
            if(fields[0] == "CAMPUSES") {
                mode = FileReadMode::campuses;
            }
            else if(fields[0] == "DISTANCES") {
                mode = FileReadMode::distances;
            }
            else if(fields[0] == "SOUVENIRS") {
                mode = FileReadMode::souvenirs;
            }
            else if(mode == FileReadMode::campuses && fields[0] != "") {
                //qDebug() << "Adding" << fields[0];
                addCampus(fields[0]);
            }
        }
        else if(mode == FileReadMode::distances && fields.length() == 3) {
            //qDebug() << fields[0] << fields[1] << fields[2].toDouble();
            changeCampusDistance(fields[0], fields[1], fields[2].toDouble());
        }
        else if(mode == FileReadMode::souvenirs && fields.length() == 3) {
            double price = fields[2].remove(QChar('$')).toDouble();

            //qDebug() << fields[0] << fields[1] << price;
            addSouvenir(fields[0], fields[1], price);
        }
    }

    file.close();
}




/*********************
 * PRIVATE FUNCTIONS *
 *********************/

int DbManager::getCampusID(QString name) const
{
    int id = -1;

    QSqlQuery query;

    query.prepare("SELECT id FROM college_info WHERE name = (:name)");
    query.bindValue(":name", name);

    if(query.exec() && query.next())
    {
        int idIndex = query.record().indexOf("id");

        id = query.value(idIndex).toInt();
    }

    return id;
}

QString DbManager::getCampusName(int id) const
{
    QString name = "";

    QSqlQuery query;

    query.prepare("SELECT name FROM college_info WHERE id = (:id)");
    query.bindValue(":id", id);

    if(query.exec() && query.next())
    {
        int nameIndex = query.record().indexOf("name");

        name = query.value(nameIndex).toString();
    }

    return name;
}
