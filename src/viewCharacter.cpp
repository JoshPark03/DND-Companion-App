/*
Name: viewCharacter.cpp
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 11/18/2024
*/

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
#include <QRadioButton>
#include <QButtonGroup>
#include <QListWidget>
#include <QApplication>
#include <fstream>
#include <iostream>
#include <sstream>

#include "viewCharacter.h"
#include "viewInventory.h"
#include "viewNotes.h"
#include "themeUtils.h" // Include the utility header

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
            3|    Prepped Spells (comma separated)(entire line) // removed
                    SpellName:SpellLevel:Book:Page
            4|    Known Spells (comma separated)(entire line) // removed
                    SpellName:SpellLevel:Book:Page
            5|    Feats (comma separated)(entire line)
            6|    Languages (comma separated)(entire line)
            7|    Equipment Proficiencies (comma separated)(entire line)
            8|    Attuned Items (comma separated)(entire line)(max 3) // removed
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

    /* this portion of the csv has been removed
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
    */

    /* this portion of the csv has been removed

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
    */

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


    /* this portion of the csv has been removed
        // Get the eighth line of character information
        line = in.readLine();
        QStringList line8 = line.split(",");
        for(int i = 0; i < line8.size(); i++)
        {
            if(line8[i] != "" && line8[i] != " ") characterAttunedItems.append(line8[i]); // Add attuned items to the list
        }
    */

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

    qDebug() << "before loadCharacter()";

    loadCharacter(nameIn);
    qDebug() << "after loadCharacter()";
    printCharacterToConsole();
    evaluateCharacterModifiers();

    qDebug() << "past modifiers evaluation";

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

    qDebug() << "before class race level labels";

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
    statsWidget->setFixedHeight(120);

    // Define all of the stat widgets
    QStringList abilitiesNames;
    abilitiesNames << "Str" << "Dex" << "Con" << "Int" << "Wis" << "Cha"; // Adds the names of the abilities to the list

    // Add all of the stat widgets to the stats widget
    statsLayout->addWidget(new QLabel("Ability"), 0, 0); // Creates a label for the ability names section header
    statsLayout->addWidget(new QLabel("Score"), 1, 0); // Creates a label for the ability scores section header
    statsLayout->addWidget(new QLabel("Modifier"), 2, 0); // Creates a label for the ability modifiers section header
    statsLayout->addWidget(new QLabel("Saving Throws"), 3, 0); // Creates a label for the saving throws section header
    
    // Add the ability names and values to the stats widget
    for(int i = 0; i < abilitiesNames.length(); i++)
    {
        statsLayout->addWidget(new QLabel(abilitiesNames[i]), 0, i+1); // Adds the ability names to the list
        statsLayout->addWidget(new QLabel(QString::number(characterAbilities[i])), 1, i+1); // Adds the ability scores to the list
    }

    // Add ability modifiers below ability scores
    for(int i = 0; i < characterAbilityBonuses.length(); i++)
    {
        QString prefix = ""; // Creates a prefix for the modifier
        if(characterSavingThrows[i] >= 0) prefix = "+"; // Adds a plus sign to the front of the modifier if it is positive or zero
        statsLayout->addWidget(new QLabel(prefix + QString::number(characterAbilityBonuses[i])), 2, i+1); // Adds the ability modifiers to the list
    }


    // Add the saving throws to the stats widget
    for(int i = 0; i < abilitiesNames.length(); i++)
    {
        QString prefix = ""; // Creates a prefix for the saving throw
        if(characterSavingThrows[i] >= 0) prefix = "+"; // Adds a plus sign to the front of the saving throw if it is positive or zero
        statsLayout->addWidget(new QLabel(prefix + QString::number(characterSavingThrows[i])), 3, i+1); // Adds the saving throws to the list
    }

    // Create a skills widget for the second column
    QWidget *skillsWidget = new QWidget(); // Creates a widget for the skills section
    QGridLayout *skillsLayout = new QGridLayout(skillsWidget); // Creates a grid layout for the skills section

    // Add all of the skill widgets to the skills widget
    QLabel *skillsLabel = new QLabel("Skills\n"); // Creates a label for the skills section header
    skillsLabel->setAlignment(Qt::AlignLeft); // Aligns the skills label to the left
    QLabel *proficiencyBonusLabel = new QLabel("Proficiency Bonus:\n+" + QString::number(characterProficiencyBonus)); // Adds the proficiency bonus to the label
    proficiencyBonusLabel->setAlignment(Qt::AlignRight); // Aligns the proficiency bonus label to the right
    skillsLayout->addWidget(skillsLabel, 0, 0); // Adds the skills label to the list
    skillsLayout->addWidget(proficiencyBonusLabel, 0, 1, 1, 2); // Adds the proficiency bonus label to the list
    for(int i = 0; i < skillMap.count(); i++)
    {
        QString prefix = "";
        if(characterSkillBonuses[i] >= 0) prefix = "+"; // Adds a plus sign to the front of the skill bonus if it is positive or zero
        QLabel *skillLabel = new QLabel(skillMap.keys().at(i) + "\n" + prefix + QString::number(characterSkillBonuses[i])); // Uses the skill map to get the skill name and adds the skill bonus to the label
        skillLabel->setAlignment(Qt::AlignCenter); // Aligns the skill label to the center
        skillsLayout->addWidget(skillLabel, (i/3)+1, i%3); // Adds the skill label to the list
    }

    // Add the stats widget to column 2
    column2Layout->addWidget(statsWidget); // Adds the stats widget to the list
    column2Layout->addWidget(skillsWidget); // Adds the skills widget to the list


    // Create the third column
    QWidget *column3 = new QWidget();
    QVBoxLayout *column3Layout = new QVBoxLayout(column3);

    // Define all of the column 3 widgets
    QWidget *combatStatsWidget = new QWidget(); // Creates a widget for the combat stats section
    QGridLayout *combatStatsLayout = new QGridLayout(combatStatsWidget); // Creates a grid layout for the combat stats section
    // combatStatsWidget->setFixedHeight(100); // Sets the height of the combat stats widget to 100px
    QString initiativePrefix = (characterInitiative < 0) ? "-" : "+"; // Uses a ternary operator to determine if the initiative is negative or positive
    QLabel *initiativeLabel = new QLabel("Initiative:\n" + initiativePrefix + QString::number(characterInitiative)); // Creates a label with the prefix and initiative as the text
    QLabel *armorClassLabel = new QLabel("Armor Class:\n" + QString::number(characterArmorClass)); // Creates a label with the armor class as the text
    QLabel *hitPointsLabel = new QLabel("Hit Points:\n" + QString::number(characterHitPoints) + "/" + QString::number(characterMaxHitPoints)); // Creates a label with the hit points as the text

    // Defining the area for the death saving throws
    QWidget *deathSavingThrows = new QWidget(); // Creates a widget for the death saving throws section
    QWidget *deathSuccesses = new QWidget(); // Creates a widget for the death successes section
    QWidget *deathFails = new QWidget(); // Creates a widget for the death fails section

    // Setting the layouts for each saving throw area
    QVBoxLayout *deathSavingThrowsLayout = new QVBoxLayout(deathSavingThrows); // Creates a vertical layout for the death saving throws section
    deathSavingThrows->setFixedHeight(150);
    QHBoxLayout *deathSuccessesLayout = new QHBoxLayout(deathSuccesses); // Creates a horizontal layout for the death successes section
    QHBoxLayout *deathFailsLayout = new QHBoxLayout(deathFails); // Creates a horizontal layout for the death fails section

    // Creating the death saving throw widgets
    QLabel *deathSavingThrowsLabel = new QLabel("Death Saving Throws"); // Creates a label for the death saving throws section
    QLabel *deathSuccessesLabel = new QLabel("Successes:"); // Creates a label for the death successes section
    QLabel *deathFailsLabel = new QLabel("Fails:"); // Creates a label for the death fails section

    // Creating the death saving throw radio buttons
    QRadioButton *deathSuccess1 = new QRadioButton(); // Creates a radio button for the first death success
    QRadioButton *deathSuccess2 = new QRadioButton(); // Creates a radio button for the second death success
    QRadioButton *deathSuccess3 = new QRadioButton(); // Creates a radio button for the third death success
    QRadioButton *deathFail1 = new QRadioButton(); // Creates a radio button for the first death fail
    QRadioButton *deathFail2 = new QRadioButton(); // Creates a radio button for the second death fail
    QRadioButton *deathFail3 = new QRadioButton(); // Creates a radio button for the third death fail



    

    // Add the death successes and fails to their respective layouts
    deathSuccessesLayout->addWidget(deathSuccessesLabel); // Adds the death successes label to the list
    deathSuccessesLayout->addWidget(deathSuccess1); // Adds the first death success radio button to the list
    deathSuccessesLayout->addWidget(deathSuccess2); // Adds the second death success radio button to the list
    deathSuccessesLayout->addWidget(deathSuccess3); // Adds the third death success radio button to the list
    deathFailsLayout->addWidget(deathFailsLabel); // Adds the death fails label to the list
    deathFailsLayout->addWidget(deathFail1); // Adds the first death fail radio button to the list
    deathFailsLayout->addWidget(deathFail2); // Adds the second death fail radio button to the list
    deathFailsLayout->addWidget(deathFail3); // Adds the third death fail radio button to the list

    // Add the death successes and fails to the death saving throws layout
    deathSavingThrowsLayout->addWidget(deathSavingThrowsLabel); // Adds the death saving throws label to the list
    deathSavingThrowsLayout->addWidget(deathSuccesses); // Adds the death successes section to the list
    deathSavingThrowsLayout->addWidget(deathFails); // Adds the death fails section to the list


    // Allign the labels to the center
    initiativeLabel->setAlignment(Qt::AlignCenter);
    armorClassLabel->setAlignment(Qt::AlignCenter);
    hitPointsLabel->setAlignment(Qt::AlignCenter);

    // Creates the list widget for the equipped items and the prepped spells and a widget for the list widgets
    QWidget *listsWidget = new QWidget();
    QGridLayout *listsLayout = new QGridLayout(listsWidget);
    QListWidget *equippedItemsList = new QListWidget();
    QListWidget *preppedSpellsList = new QListWidget();

    // Create the labels for the list widgets
    QLabel *equippedItemsLabel = new QLabel("Equipped Items");
    QLabel *preppedSpellsLabel = new QLabel("Prepped Spells");

    // Add the attuned items and prepped spells to the list widgets
    for(int i = 0; i < characterAttunedItems.length(); i++)
    {
        equippedItemsList->addItem(characterAttunedItems[i]);
    }
    for(int i = 0; i < characterPreppedSpells.length(); i++)
    {
        preppedSpellsList->addItem(characterPreppedSpells[i]);
    }

    // Add the list widgets to the list widget widget
    listsLayout->addWidget(equippedItemsLabel, 0, 0);
    listsLayout->addWidget(preppedSpellsLabel, 0, 1);
    listsLayout->addWidget(equippedItemsList, 1, 0, 4, 1);
    listsLayout->addWidget(preppedSpellsList, 1, 1, 4, 1);

    // Creates the inventory and notes buttons and a buttons widget for the buttons
    QWidget *buttonsWidget = new QWidget();
    QGridLayout *buttonsLayout = new QGridLayout(buttonsWidget);
    QPushButton *inventoryButton = new QPushButton("Inventory");
    QPushButton *spellsButton = new QPushButton("Spells");
    QPushButton *notesButton = new QPushButton("Notes");

    // Add the buttons to the buttons widget
    buttonsLayout->addWidget(inventoryButton, 0, 0);
    buttonsLayout->addWidget(spellsButton, 0, 1); // Add the new Spells button
    buttonsLayout->addWidget(notesButton, 1, 0, 1, 2);

    // Add all of the combat stats widgets to the combat stats widget
    combatStatsLayout->addWidget(initiativeLabel, 0, 0);
    combatStatsLayout->addWidget(armorClassLabel, 0, 1);
    combatStatsLayout->addWidget(hitPointsLabel, 0, 2);
    combatStatsLayout->addWidget(deathSavingThrows, 1, 1, 1, 2);

    // Add the column 3 widgets to column 3
    column3Layout->addWidget(combatStatsWidget);
    column3Layout->setAlignment(combatStatsWidget, Qt::AlignTop);
    column3Layout->addWidget(listsWidget);
    column3Layout->addWidget(buttonsWidget);

    // Create buttons for navbar
    QPushButton *Backbutton = new QPushButton("Return to Character Select");
    QPushButton *characterSheetButton = new QPushButton("View Character Sheet");
    QPushButton *exportButton = new QPushButton("Export Character");

    // Add buttons to navbar
    navbarLayout->addWidget(Backbutton);
    navbarLayout->addWidget(characterSheetButton);
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

    // Make inventory button go to inventory page
    connect(inventoryButton, SIGNAL (clicked()), SLOT (goToInventory()));
    
    // Make spells button go to inventory page
    connect(spellsButton, SIGNAL (clicked()), SLOT (goToSpells()));

    // Make notes button go to notes page
    connect(notesButton, SIGNAL (clicked()), SLOT (goToNotes()));

    

    reloadTheme(); // Reload the theme after everything is placed
}

void ViewCharacter::goBack()
{
    // Set the current stacked widget to the parent of viewCharacter
    QStackedWidget *currentStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    // Loop through the parent widgets until the main stacked widget is found
    while (currentStackedWidget != nullptr)
    {
        // Check if the current stacked widget is the main stacked widget
        QWidget *parent = currentStackedWidget->parentWidget();
        // If the parent is the main stacked widget, set the current index to 0
        QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(parent);
        if (mainStackedWidget)
        {
            // Set the current index to 0
            mainStackedWidget->setCurrentIndex(0); // Switch to CharacterSelect (index 0)
            return;
        }
        // If the parent is not the main stacked widget, set the current stacked widget to the parent
        currentStackedWidget = qobject_cast<QStackedWidget *>(parent);
    }
    // In the event that the main stacked widget is not found, print an error message
    qDebug() << "Main stacked widget not found. Cannot return to Character Select.";
}

void ViewCharacter::goToInventory()
{
    QStackedWidget *currentStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (currentStackedWidget)
    {
        currentStackedWidget->setCurrentIndex(1); // Switch to Inventory (index 1)
    }
}

void ViewCharacter::goToSpells()
{
    QStackedWidget *currentStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (currentStackedWidget)
    {
        currentStackedWidget->setCurrentIndex(2); // Switch to Inventory (index 1)
    }
}

void ViewCharacter::goToNotes()
{
    QStackedWidget *currentStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (currentStackedWidget)
    {
        currentStackedWidget->setCurrentIndex(3); // Switch to Notes (index 2)
    }
}

ViewCharacter::~ViewCharacter()
{
    delete this->proficiencyBonusTable;
    delete this->experienceTable;
}
