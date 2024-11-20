/*
Name: viewSpells.h
Description: Page to view a character's spells.
Authors: Zachary Craig
Other Sources: ...
Date Created: 11/19/2024
Last Modified: 11/19/2024
*/

#ifndef VIEWSPELLS_H
#define VIEWSPELLS_H

#include <QWidget>

class ViewSpells : public QWidget
{
Q_OBJECT
public:
    explicit ViewSpells(QWidget *parent = 0, QString name = "");
    ~ViewSpells();

private:
    void loadSpells();

private slots:
    void goBack();
};

#endif