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
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QPushButton>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    ClickableLabel(QWidget *parent = nullptr) : QLabel(parent) {}

signals:
    void clicked();  // Signal to notify when the label is clicked

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            emit clicked();  // Emit the clicked signal when the label is clicked
        }
        QLabel::mousePressEvent(event);  // Call the base class handler
    }
};


class ViewCharacter : public QWidget
{
    Q_OBJECT
public:
    explicit ViewCharacter(QWidget *parent = 0, QString name = "");
    ~ViewCharacter();
    void printCharacterToConsole();

private:
    void loadCharacter(QString name);
    void evaluateCharacterModifiers();
    void changeProfilePicture();
    void loadPicture(const QString &imagePath);
    ClickableLabel *pictureLabel = new ClickableLabel();
    QString name;
    QString characterName;
    QString characterClass;
    QString characterSubclass;
    QString characterRace;
    QString characterBackground;
    QString characterAlignment;
    QStringList characterSkillProficiencies;
    QStringList characterPreppedSpells;
    QStringList characterKnownSpells;
    QStringList characterFeats;
    QStringList characterLanguages;
    QStringList characterEquipmentProficiencies;
    QStringList characterAttunedItems;
    QStringList imageExtentions = {"png", "jpg", "bmp", "jpeg"};
    QList<int> characterAbilities;
    QList<int> characterAbilityBonuses;
    QList<int> characterSavingThrows;
    QList<int> characterSkillBonuses;
    QList<int> characterCoins;
    QList<int> deathThrows;
    int characterHitPoints;
    int characterMaxHitPoints;
    int characterArmorClass;
    int characterInitiative;
    int characterLevel;
    int characterExperience;
    int characterProficiencyBonus;
    const int *proficiencyBonusTable = new int[20]{2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
    const int *experienceTable = new int[21]{0, 300, 900, 2700, 6500, 14000, 23000, 34000, 48000, 64000, 85000, 100000, 120000, 140000, 165000, 195000, 225000, 265000, 305000, 355000, 405000};

private slots:
    void goBack();
    void goToInventory();
    void goToSpells();
    void goToNotes();
    // void importChar();
};

#endif // VIEWCHARACTER_H
