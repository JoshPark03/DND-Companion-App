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
#include <QMap>

#include <fstream>
#include <iostream>
#include <sstream>

QMap<QString, int> skillMap = 
{
    // The key is the skill's name, and the value is the index corresponding to the ability that the skill is based on
    {"Acrobatics", 1},
    {"Animal Handling", 4},
    {"Arcana", 3},
    {"Athletics", 0},
    {"Deception", 5},
    {"History", 3},
    {"Insight", 4},
    {"Intimidation", 5},
    {"Investigation", 3},
    {"Medicine", 4},
    {"Nature", 3},
    {"Perception", 4},
    {"Performance", 5},
    {"Persuasion", 5},
    {"Religion", 3},
    {"Sleight of Hand", 1},
    {"Stealth", 1},
    {"Survival", 4}
};

void ViewCharacter::loadCharacter(QString name)
{
    // Load the character's information, notes, and stats
    QString path = QDir::currentPath() + "/data/characters/" + name;

    qDebug() << "Character Path: " << path;

    // Load the character's information
    QFile characterFile(path + "/character.csv");

    if (characterFile.open(QIODevice::ReadOnly))
    {
        /*
            Character File Format:
            1|    Name,Str,Dex,Con,Int,Wis,Cha,Level:Experience,CurrentHealth:MaxHealth,Class,Sub,Race
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
            characterAbilities.append(line1[i].toInt());
        }
        QStringList levelExperienceList = line1[7].split(":");
        characterLevel = levelExperienceList[0].toInt();
        characterExperience = levelExperienceList[1].toInt();
        QStringList hitPointsList = line1[8].split(":");
        characterHitPoints = hitPointsList[0].toInt();
        characterMaxHitPoints = hitPointsList[1].toInt();
        characterClass = line1[9];
        characterSubclass = line1[10];
        characterRace = line1[11];


        // Get the second line of character information
        line = in.readLine();
        QStringList line2 = line.split(",");
        for(int i = 0; i < line2.size(); i++)
        {
            if(line2[i] != "" && line2[i] != " ") characterSkillProficiencies.append(line2[i]); // Add stat proficiencies to the list
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
        }


        // Get the fourth line of character information
        line = in.readLine();
        QStringList line4 = line.split(",");
        for(int i = 0; i < line4.size(); i++)
        {
            QStringList spellInfo = line4[i].split(":");
            for(int j = 0; j < spellInfo.size(); j++)
            {
                if(spellInfo[j] != "" && spellInfo[j] != " " && j == 0) characterKnownSpells.append(spellInfo[j]); // Add known spells to the list
            }
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
    qDebug() << "Character Abilities: " << characterAbilities;
    qDebug() << "Character Level: " << characterLevel;
    qDebug() << "Character Experience: " << characterExperience;
    qDebug() << "Lower Bound Experience: " << experienceTable[characterLevel-1];
    qDebug() << "Upper Bound Experience: " << experienceTable[characterLevel];
    qDebug() << "Character Class: " << characterClass;
    qDebug() << "Character Subclass: " << characterSubclass;
    qDebug() << "Character Race: " << characterRace;
    qDebug() << "Character Stat Proficiencies: " << characterSkillProficiencies;
    qDebug() << "Character Prepped Spells: " << characterPreppedSpells;
    qDebug() << "Character Known Spells: " << characterKnownSpells;
    qDebug() << "Character Feats: " << characterFeats;
    qDebug() << "Character Languages: " << characterLanguages;
    qDebug() << "Character Equipment Proficiencies: " << characterEquipmentProficiencies;
    qDebug() << "Character Attuned Items: " << characterAttunedItems;
    qDebug() << "Character Coins: " << characterCoins;
}

void ViewCharacter::evaluateCharacterModifiers()
{
    // Evaluate the character's modifiers
    // Modifiers are calculated by taking the stat, subtracting 10, dividing by 2, and rounding down
    // The proficiency bonus is determined by the character's level
    characterProficiencyBonus = proficiencyBonusTable[characterLevel - 1];

    // Calculate the character's ability bonuses
    for(int i = 0; i < 6; i++) characterAbilityBonuses.append((int)floor((float)(characterAbilities[i] - 10) / 2));

    // Calculate the character's saving throws
    for(int i = 0; i < 6; i++) characterSavingThrows.append(characterAbilityBonuses[i]);

    // Evaluate the character's skill bonuses
    for(QMap<QString, int>::const_iterator it = skillMap.constBegin(); it != skillMap.constEnd(); it++)
    {
        int abilityIndex = it.value();

        // Check if the character is proficient in the skill
        if(characterSkillProficiencies.contains(it.key())) characterSkillBonuses.append(characterAbilityBonuses[abilityIndex] + characterProficiencyBonus);
        else characterSkillBonuses.append(characterAbilityBonuses[abilityIndex]);
    }

    // Evaluate initiative
    characterInitiative = characterAbilityBonuses[1];

    // Evaluate armor class
    if(/*Has armor equipped       leave false until implemented*/ false)
    {
        // Do armor calculations
    }
    else characterArmorClass = 10 + characterAbilityBonuses[1];

    // Evaluate hit points

    


}

ViewCharacter::ViewCharacter(QWidget *parent, QString nameIn) :
    QWidget(parent)
{
    if(nameIn == "")
    {
        qDebug() << "No character name provided";
        return;
    }

    loadCharacter(nameIn);
    printCharacterToConsole();
    evaluateCharacterModifiers();


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

    QString imagePath = QDir::currentPath() + "/data/characters/" + characterName + "/character.png";

    QPixmap characterPicture(imagePath);

    // Check if the pixmap loaded successfully
    if (characterPicture.isNull())
    {
        qDebug() << "Failed to load image at path:" << imagePath;
        pictureLabel->setText("Image not available");  // Display placeholder text if image fails to load
    }
    else
    {
        QPixmap scaledCharacterPicture = characterPicture.scaled(200, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        pictureLabel->setPixmap(scaledCharacterPicture);
    }


    QLabel *classAndSubclassLabel = new QLabel(characterClass + " | " + characterSubclass);
    QLabel *raceLabel = new QLabel(characterRace);
    QLabel *levelLabel = new QLabel(QString::number(characterLevel));
    QProgressBar *experienceProgressBar = new QProgressBar();
    experienceProgressBar->setRange(experienceTable[characterLevel-1],experienceTable[characterLevel]);
    experienceProgressBar->setValue(characterExperience);

    // Add all of the column 1 widgets to column 1
    column1Layout->addWidget(nameLabel);
    column1Layout->addWidget(pictureLabel);
    column1Layout->addWidget(classAndSubclassLabel);
    column1Layout->addWidget(raceLabel);
    column1Layout->addWidget(levelLabel);
    column1Layout->addWidget(experienceProgressBar);



    // Create the second column
    QWidget *column2 = new QWidget();
    QVBoxLayout *column2Layout = new QVBoxLayout(column2);

    // Create a stats widget for the second column
    QWidget *statsWidget = new QWidget();
    QGridLayout *statsLayout = new QGridLayout(statsWidget);
    statsWidget->setFixedHeight(100);

    // Define all of the stat widgets
    QStringList abilitiesNames;
    abilitiesNames << "Str" << "Dex" << "Con" << "Int" << "Wis" << "Cha";
    QLabel *statsLabel = new QLabel("Stats");
    
    QLabel *savingThrowsLabel = new QLabel("Saving Throws");

    // Add all of the stat widgets to the stats widget
    statsLayout->addWidget(statsLabel, 1, 0);
    for(int i = 0; i < abilitiesNames.length(); i++)
    {
        statsLayout->addWidget(new QLabel(abilitiesNames[i]), 0, i+1);
        statsLayout->addWidget(new QLabel(QString::number(characterAbilities[i])), 1, i+1);
    }
    statsLayout->addWidget(savingThrowsLabel, 2, 0);
    for(int i = 0; i < abilitiesNames.length(); i++)
    {
        QString prefix = "";
        if(characterSavingThrows[i] >= 0) prefix = "+";
        statsLayout->addWidget(new QLabel(prefix + QString::number(characterSavingThrows[i])), 2, i+1);
    }

    // Create a skills widget for the second column
    QWidget *skillsWidget = new QWidget();
    QGridLayout *skillsLayout = new QGridLayout(skillsWidget);

    // Add all of the skill widgets to the skills widget
    QLabel *skillsLabel = new QLabel("Skills");
    skillsLabel->setAlignment(Qt::AlignCenter);
    QLabel *proficiencyBonusLabel = new QLabel("Proficiency Bonus:\n" + QString::number(characterProficiencyBonus));
    proficiencyBonusLabel->setAlignment(Qt::AlignCenter);
    skillsLayout->addWidget(skillsLabel, 0, 0);
    skillsLayout->addWidget(proficiencyBonusLabel, 0, 1, 1, 2);
    for(int i = 0; i < skillMap.count(); i++)
    {
        QString prefix = "";
        if(characterSkillBonuses[i] >= 0) prefix = "+";

        QLabel *skillLabel = new QLabel(skillMap.keys().at(i) + "\n" + prefix + QString::number(characterSkillBonuses[i]));
        skillLabel->setAlignment(Qt::AlignCenter);

        skillsLayout->addWidget(skillLabel, (i/3)+1, i%3);
    }

    // Add the stats widget to column 2
    column2Layout->addWidget(statsWidget);
    column2Layout->addWidget(skillsWidget);


    // Create the third column
    QWidget *column3 = new QWidget();
    QVBoxLayout *column3Layout = new QVBoxLayout(column3);

    // Define all of the column 3 widgets
    QWidget *combatStatsWidget = new QWidget();
    QGridLayout *combatStatsLayout = new QGridLayout(combatStatsWidget);
    combatStatsWidget->setFixedHeight(100);
    QString initiativePrefix = (characterInitiative < 0) ? "-" : "+";
    QLabel *initiativeLabel = new QLabel("Initiative:\n" + initiativePrefix + QString::number(characterInitiative));
    QLabel *armorClassLabel = new QLabel("Armor Class:\n" + QString::number(characterArmorClass));
    QLabel *hitPointsLabel = new QLabel("Hit Points:\n" + QString::number(characterHitPoints) + "/" + QString::number(characterMaxHitPoints));

    // Allign the labels to the center
    initiativeLabel->setAlignment(Qt::AlignCenter);
    armorClassLabel->setAlignment(Qt::AlignCenter);
    hitPointsLabel->setAlignment(Qt::AlignCenter);

    // Add all of the combat stats widgets to the combat stats widget
    combatStatsLayout->addWidget(initiativeLabel, 0, 0);
    combatStatsLayout->addWidget(armorClassLabel, 0, 1);
    combatStatsLayout->addWidget(hitPointsLabel, 0, 2);

    // Add the column 3 widgets to column 3
    column3Layout->addWidget(combatStatsWidget);



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

    // Set stretch factors for columns
    bodyLayout->setStretch(0, 1);
    bodyLayout->setStretch(1, 1);
    bodyLayout->setStretch(2, 1);

    // Add navbar and body to main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Make back button return to character select page
    connect(Backbutton, SIGNAL (clicked()), SLOT (goBack()));
}

void ViewCharacter::goBack()
{
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (stackedWidget)
    {
        stackedWidget->setCurrentIndex(0); // character select is the first page so index 0
    }
}

ViewCharacter::~ViewCharacter()
{
    delete this->proficiencyBonusTable;
    delete this->experienceTable;
}
