/*
Name: viewSpells.h
Description: Page to view a character's spells.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 11/19/2024
Last Modified: 12/3/2024
*/

#ifndef VIEWSPELLS_H
#define VIEWSPELLS_H

#include <QWidget>
#include <QTableWidget>

class ViewSpells : public QWidget
{
Q_OBJECT
public:
    explicit ViewSpells(QWidget *parent = 0, QString name = "");
    ~ViewSpells();

private:
    QTableWidget * spells;
    QString name;
    void loadSpells();
    void addItem(QString line);
private slots:
    void saveSpells();
    void addSpell();
    void goBack();
};

#endif
