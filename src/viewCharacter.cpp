/*
Name: viewCharacter.cpp
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 11/6/2024
*/

#include "viewCharacter.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QList>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QProgressBar>

#include <fstream>
#include <iostream>
#include <sstream>

void ViewCharacter::loadCharacter(QString name)
{
    // Load the character's information, notes, and stats
    QString path = QDir::currentPath() + "/data/characters/" + name;

    // Load the character's information
    QFile characterFile(path + "/character.csv");

    if (characterFile.open(QIODevice::ReadOnly))
    {
        /*
            Character File Format:
            1|    Name,Str,Dex,Con,Int,Wis,Cha,Level,Class,Sub,Race
            2|    Stat Proficiencies (comma separated)(entire line)
            3|    Prepped Spells (comma separated)(entire line)
                    SpellName:SpellLevel:Book:Page
            4|    Known Spells (comma separated)(entire line)
                    SpellName:SpellLevel:Book:Page
            5|    Feats (comma separated)(entire line)
            6|    Languages (comma separated)(entire line)
            7|    Equipment Proficiencies (comma separated)(entire line)
            8|    Attuned Items (comma separated)(entire line)(max 3)
            9|    Coins (platinum,gold,silver,copper)
        */


        QTextStream in(&characterFile);
        // Can not loop through each line because lines are not consistent in context
        // Get the first line of character information
        QString line = in.readLine();


        // Get first line of character information
        QStringList line1 = line.split(",");

        characterName = line1[0];
        for(int i = 1; i < 7; i++)
        {
            characterStats.append(line1[i].toInt());
        }
        characterLevel = line1[7].toInt();
        characterClass = line1[8];
        characterSubclass = line1[9];
        characterRace = line1[10];


        // Get the second line of character information
        line = in.readLine();
        QStringList line2 = line.split(",");
        for(int i = 0; i < line2.size(); i++)
        {
            if(line2[i] != "" && line2[i] != " ") characterStatProficiencies.append(line2[i]); // Add stat proficiencies to the list
        }


        // Get the third line of character information
        line = in.readLine();
        QStringList line3 = line.split(",");
        for(int i = 0; i < line3.size(); i++)
        {
            QStringList spellInfo = line3[i].split(":");
            for(int j = 0; j < spellInfo.size(); j++)
            {
                if(spellInfo[j] != "" && spellInfo[j] != " " && j == 0) characterPreppedSpells.append(spellInfo[j]); // Add prepped spells to the list
            }
            // if(line3[i] != "" && line3[i] != " ") characterPreppedSpells.append(line3[i]); // Add prepped spells to the list
        }


        // Get the fourth line of character information
        line = in.readLine();
        QStringList line4 = line.split(",");
        for(int i = 0; i < line4.size(); i++)
        {
            QStringList spellInfo = line4[i].split(":");
            for(int j = 0; j < spellInfo.size(); j++)
            {
                if(spellInfo[j] != "" && spellInfo[j] != " " && j == 0) characterKnownSpells.append(spellInfo[j]); // Add prepped spells to the list
            }
            // if(line4[i] != "" && line4[i] != " ") characterKnownSpells.append(line4[i]); // Add known spells to the list
        }


        // Get the fifth line of character information
        line = in.readLine();
        QStringList line5 = line.split(",");
        for(int i = 0; i < line5.size(); i++)
        {
            if(line5[i] != "" && line5[i] != " ") characterFeats.append(line5[i]); // Add feats to the list
        }


        // Get the sixth line of character information
        line = in.readLine();
        QStringList line6 = line.split(",");
        for(int i = 0; i < line6.size(); i++)
        {
            if(line6[i] != "" && line6[i] != " ") characterLanguages.append(line6[i]); // Add languages to the list
        }


        // Get the seventh line of character information
        line = in.readLine();
        QStringList line7 = line.split(",");
        for(int i = 0; i < line7.size(); i++)
        {
            if(line7[i] != "" && line7[i] != " ") characterEquipmentProficiencies.append(line7[i]); // Add equipment proficiencies to the list
        }


        // Get the eighth line of character information
        line = in.readLine();
        QStringList line8 = line.split(",");
        for(int i = 0; i < line8.size(); i++)
        {
            if(line8[i] != "" && line8[i] != " ") characterAttunedItems.append(line8[i]); // Add attuned items to the list
        }


        // Get the ninth line of character information
        line = in.readLine();
        QStringList line9 = line.split(",");
        for(int i = 0; i < line9.size(); i++)
        {
            if(line9[i] != "" && line9[i] != " ") characterCoins.append(line9[i].toInt()); // Add coins to the list
        }


        // Close the file
        characterFile.close();

        // Print the character to the console for testing
        printCharacterToConsole();
    }
}

void ViewCharacter::printCharacterToConsole()
{
    // Make sure that the values are getting set correctly
    qDebug() << "Character Name: " << characterName;
    qDebug() << "Character Stats: " << characterStats;
    qDebug() << "Character Level: " << characterLevel;
    qDebug() << "Character Class: " << characterClass;
    qDebug() << "Character Subclass: " << characterSubclass;
    qDebug() << "Character Race: " << characterRace;
    qDebug() << "Character Stat Proficiencies: " << characterStatProficiencies;
    qDebug() << "Character Prepped Spells: " << characterPreppedSpells;
    qDebug() << "Character Known Spells: " << characterKnownSpells;
    qDebug() << "Character Feats: " << characterFeats;
    qDebug() << "Character Languages: " << characterLanguages;
    qDebug() << "Character Equipment Proficiencies: " << characterEquipmentProficiencies;
    qDebug() << "Character Attuned Items: " << characterAttunedItems;
    qDebug() << "Character Coins: " << characterCoins;
}

ViewCharacter::ViewCharacter(QWidget *parent, QString nameIn) :
    QWidget(parent)
{
    // QString characterName;
    // QString characterClass;
    // QString characterSubclass;
    // QString characterRace;
    // QString characterBackground;
    // QString characterAlignment;
    // QList<int> characterStats;
    // int characterLevel;
    // QStringList characterStatProficiencies;
    // QStringList characterPreppedSpells;
    // QStringList characterKnownSpells;
    // QStringList characterFeats;
    // QStringList characterLanguages;
    // QStringList characterEquipmentProficiencies;
    // QStringList characterAttunedItems;
    // QList<int> characterCoins;

    loadCharacter(nameIn);

    this->name = nameIn;
    // Create the verticle layout for buttons
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesnt take up half the screen

    // Create the horizontal layout for the columns
    QWidget *body = new QWidget();
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);

    // Create the buttons for the navbar


    // Create the first column
    QWidget *column1 = new QWidget();
    QVBoxLayout *column1Layout = new QVBoxLayout(column1);
    column1Layout->setSpacing(5);
    column1Layout->setContentsMargins(0, 0, 0, 0);

    // Define all of the column 1 widgets
    QLabel *nameLabel = new QLabel(characterName);
    QLabel *pictureLabel = new QLabel();
    pictureLabel->setFixedSize(200,250);
    QPixmap characterPicture("data/characters/" + characterName + "/character.png");
    QPixmap scaledCharacterPicture = characterPicture.scaled(200,250,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    pictureLabel->setPixmap(scaledCharacterPicture);
    QLabel *classAndSubclassLabel = new QLabel(characterClass + " | " + characterSubclass);
    QLabel *raceLabel = new QLabel(characterRace);
    QLabel *levelLabel = new QLabel(QString::number(characterLevel));
    QProgressBar *experienceProgressBar = new QProgressBar();
    experienceProgressBar->setRange(0,300);
    experienceProgressBar->setValue(190);

    // Add all of the column 1 widgets to column 1
    column1Layout->addWidget(nameLabel);
    column1Layout->addWidget(pictureLabel);
    column1Layout->addWidget(classAndSubclassLabel);
    column1Layout->addWidget(raceLabel);
    column1Layout->addWidget(levelLabel);
    column1Layout->addWidget(experienceProgressBar);




    QWidget *column2 = new QWidget();
    QVBoxLayout *column2Layout = new QVBoxLayout(column2);

    QWidget *column3 = new QWidget();
    QVBoxLayout *column3Layout = new QVBoxLayout(column3);


    // Create buttons for settings
    QPushButton *Backbutton = new QPushButton("Return to Character Select");
    QPushButton *importButton = new QPushButton("Import Character");
    QPushButton *exportButton = new QPushButton("Export Character");

    // Add buttons to navbar
    navbarLayout->addWidget(Backbutton);
    navbarLayout->addWidget(importButton);
    navbarLayout->addWidget(exportButton);

    // Add columns to body
    bodyLayout->addWidget(column1);
    bodyLayout->addWidget(column2);
    bodyLayout->addWidget(column3);

    // Add navbar and body to main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // // Make back button return to character select page
    // connect(Backbutton, &QPushButton::clicked, [this]() {
    //     QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    //     if (stackedWidget) {
    //         stackedWidget->setCurrentIndex(0); // character select is the first page so index 0
    //     }
    // });

    // Make back button return to character select page
    connect(Backbutton, SIGNAL (clicked()), SLOT (goBack()));

    // // Prints information to the console for testing
    // connect(importButton, &QPushButton::clicked, [this, name]() {
    //     this->loadCharacter(name);
    // });

    // connect(importButton, SIGNAL (clicked()), SLOT (importChar()));
}

void ViewCharacter::goBack() {
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (stackedWidget) {
        stackedWidget->setCurrentIndex(0); // character select is the first page so index 0
    }
}

// void ViewCharacter::importChar() {
//     this->loadCharacter();
// }

ViewCharacter::~ViewCharacter()
{
}
