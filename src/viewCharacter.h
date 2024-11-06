/*
Name: viewCharacter.h
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 11/6/2024
*/

#ifndef VIEWCHARACTER_H
#define VIEWCHARACTER_H

#include <QWidget>

class ViewCharacter : public QWidget
{
    Q_OBJECT
public:
    explicit ViewCharacter(QWidget *parent = 0, QString name = "");
    ~ViewCharacter();
    void printCharacterToConsole();

private:
    void loadCharacter(QString name);
    QString name;
    QString characterName;
    QString characterClass;
    QString characterSubclass;
    QString characterRace;
    QString characterBackground;
    QString characterAlignment;
    QList<int> characterStats;
    int characterLevel;
    QStringList characterStatProficiencies;
    QStringList characterPreppedSpells;
    QStringList characterKnownSpells;
    QStringList characterFeats;
    QStringList characterLanguages;
    QStringList characterEquipmentProficiencies;
    QStringList characterAttunedItems;
    QList<int> characterCoins;

private slots:
    void goBack();
    // void importChar();
};

#endif // SETTINGS_H
