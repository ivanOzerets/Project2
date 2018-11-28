#include "teamselection.h"
#include "ui_teamselection.h"

teamSelection::teamSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::teamSelection)
{
    ui->setupUi(this);
    ui-> teamSelection_View -> addItems(DbManager::instance().teamNames());
}

teamSelection::~teamSelection()
{
    delete ui;
}

void teamSelection::on_teamSelection_View_activated(const QModelIndex &index)
{

}

void teamSelection::on_addVisit_clicked()
{
    //VARIABLES
    QListWidgetItem *item = ui -> teamSelection_View -> currentItem();
    bool duplicate        = false;


    //Checks if team is already on visitation list
    for(int i = 0; i < ui -> teamVisitation_View -> count(); i++) {
        if(ui -> teamVisitation_View -> item(i) -> data(Qt::DisplayRole).toString() == item -> text()) {
             duplicate = true;
        }
    }
    //Will only add a team when the location has NOT been visited
    if(!duplicate) {
        ui-> teamVisitation_View -> addItem(item -> text());
    }
    QVector<float> test;
    QString other;
    test = DbManager::instance().getAdjDistance(item -> text());
    other = "Distance: " + other.setNum(test[1]);
    ui -> textBrowser -> setText(other);
}

void teamSelection::on_removeVisit_clicked()
{
    QListWidgetItem *item = ui -> teamSelection_View -> currentItem();
    //Will remove any selected team from the visitation list
    qDeleteAll(ui -> teamVisitation_View -> selectedItems());
}
