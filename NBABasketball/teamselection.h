#ifndef TEAMSELECTION_H
#define TEAMSELECTION_H

#include <QDialog>
#include "DbManager.h"

namespace Ui {
class teamSelection;
}

class teamSelection : public QDialog
{
    Q_OBJECT

public:
    explicit teamSelection(QWidget *parent = 0);
    ~teamSelection();

    //utilize DB functions to retrieve data from database

private slots:
    void on_teamsToSelect_activated(const QModelIndex &index);

    void on_teamSelection_View_activated(const QModelIndex &index);

    void on_addVisit_clicked();

    void on_removeVisit_clicked();

private:
    Ui::teamSelection *ui;

};

#endif // TEAMSELECTION_H
