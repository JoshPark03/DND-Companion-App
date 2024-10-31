/*
Name: viewCharacter.cpp
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 10/31/2024
*/

#ifndef VIEWCHARACTER_H
#define VIEWCHARACTER_H

#include <QWidget>

class ViewCharacter : public QWidget
{
public:
    explicit ViewCharacter(QWidget *parent = 0, QString name = "");
    ~ViewCharacter();
    void printCharacterToConsole(std::string path);

private:
    void loadCharacter(QString name);
};

#endif // SETTINGS_H
