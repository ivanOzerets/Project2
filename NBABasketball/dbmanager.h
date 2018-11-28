#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFile>
#include <QComboBox>
#include <QStringList>
#include <QStandardPaths>
#include <QMap>

#define DB_SUBFOLDER "NBABasketball"
#define DB_FILENAME "NBAinfo.db"

#define CP_FILENAME "school-png.png"
#define RD_FILENAME "road-icon.png"

/*
struct CampusTemplate
{
    QString name;
    QMap<QString, double> souvenirs;
};
*/

class DbManager
{
public:
    enum class LoginResult {
        fail,
        student,
        admin
    };

    enum class FileReadMode {
        unknown,
        campuses,
        distances,
        souvenirs
    };

    DbManager();
    ~DbManager();
    DbManager(const DbManager& database);
    static DbManager& instance();

    bool isOpen() const;
    LoginResult LoginCheck(QString userName, QString password) const;

    // Obsolete
    /*
    QSqlQueryModel* infoToTable(int sorting);
    CampusTemplate getInfo();
    */

    // Database accessing

    // Won't be using this; would be too big/complex/slow
    //QMap<QString, QMap<QString, double>> getAllSouvenir() const;

    /**
     * @brief Gets the names of every team name.
     *
     * @return Returns a list of all of the NBA team names, in no particular order
     */
    QStringList teamNames() const;
    /**
     * @brief Gets the names of every team arena.
     *
     * @return Returns a list of all of the arenas, in no particular order
     */
    QStringList arenaNames() const;


    QVector<float> getAdjDistance(QString teamName) const;
    /**
     * @brief Gets the souvenirs which a given campus sells.
     * @param campusName The name of the campus to look at.
     *
     * @return A map of souvenir names and their prices.
     *         Is empty if campusName is invalid.
     */
    QMap<QString, double> getSouvenirs(QString campusName) const;

    /**
     * @brief Gets the distance between two campuses.
     * @param campus1 Name of first campus.
     * @param campus2 Name of second campus.
     *
     * @return The distance in miles (?) between campuses.
     *         Returns -1 if one of the campus names is invalid
     *         or the position distance cannot be found.
     */
    double getDistance(QString campus1, QString campus2) const;

    /**
     * @brief Gets all the distances from a given campus.
     * @param Campus to start from.
     *
     * @return Map with a key for each known distance to another campus.
     */
    QMap<QString, double> getAllDistancesFrom(QString start) const;


    // Database editting

    /**
     * @brief Adds a campus to the database. This campus has all distances undefined
     *        (i.e. set to -1), and has no souvenirs.
     * @param name Name of the new campus.
     */
    bool addCampus(QString name);

    /**
     * @brief Delete the given campus according to its name. Every trace of the
     *        campus is removed from the database.
     * @param name Name of the campus to remove.
     */
    bool deleteCampus(QString name);


    /**
     * @brief Changes the given campus's name to a new name.
     * @param oldName The name of the campus to modify.
     * @param newName The new name of the campus.
     */
    bool changeCampusName(QString oldName, QString newName);

    /**
     * @brief Changes the souvenirs of a given campus.
     * @param name The name of the campus to modify.
     * @param souvenirs The new sets of souvenirs.
     */
    bool changeCampusSouvenirs(QString name, QMap<QString, double> souvenirs);

    /**
     * @brief Changes a single souvenir for the given campus.
     *
     * @param campus
     *   Name of campus which sells the souvenir.
     * @param oldName
     *   Original name of the souvenir.
     * @param newName
     *   New name of the souvenir.
     *   Cannot match the name of another souvenir sold by the campus.
     *
     * @return True if the change was successful. False otherwise.
     */
    bool changeSouvenirName(QString campus, QString oldName, QString newName);

    /**
     * @brief Changes a single souvenir for the given campus.
     *
     * @param campus
     *   Name of campus which sells the souvenir.
     * @param name
     *   Name of the souvenir.
     * @param newPrice
     *   New price of the souvenir.
     *
     * @return True if the change was successful. False otherwise.
     */
    bool changeSouvenirPrice(QString campus, QString name, double newPrice);

    /**
     * @brief
     *   Adds a souvenir to the given campus. Fails if the campus doesn't
     *   exist or if the campus already has a souvenir with the given name.
     *
     * @param campus
     *   Name of the campus which will sell the souvenir.
     * @param name
     *   Name of the souvenir. Must be unique.
     * @param price
     *   Price of the souvenir.
     *
     * @return If successful, returns true.
     */
    bool addSouvenir(QString campus, QString name, double price);

    /**
     * @brief
     *   Removes a souvenir from the campus store.
     *
     * @param campus
     *   Name of the campus which sells the souvenir.
     * @param name
     *   Name of the souvenir.
     *
     * @return If successful, returns true.
     */
    bool deleteSouvenir(QString campus, QString name);


    /**
     * @brief Changes the distance between two campuses. It does not matter
     *        which campus is the first one and which is the second one.
     * @param name1 Name of first campus.
     * @param name2 Name of second campus.
     * @param distance New distance between campuses.
     */
    bool changeCampusDistance(QString name1, QString name2, double distance);


    /**
     * @brief
     *   TODO
     *
     * @param path
     */
    void readFromFile(QString path);

    //QStringList* getList();


   //void AddInfoToPurchase()
private:
    /**
     * @brief Finds the ID of the given campus. This is the ID used
     *        to identify the campus within the database. Only use this
     *        for internal purposes.
     * @param name name of the campus.
     *
     * @return The campus ID. Returns -1 if not found.
     */
    int getCampusID(QString name) const;

    /**
     * @brief Finds the campus name based on an internal database ID.
     * @param id ID used to identify campus.
     *
     * @return The name of campus. Returns empty string if not found.
     */
    QString getCampusName(int id) const;

    QSqlDatabase database;
    QStringList*    list;

};

Q_DECLARE_METATYPE(DbManager::LoginResult)

#endif // DBMANAGER_H
