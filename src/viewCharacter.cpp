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
#include <QFileDialog>
#include <QCheckBox>
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

    // Load the character's information
    QFile characterFile(path + "/character.csv");

    if (characterFile.open(QIODevice::ReadOnly))
    {
        /*
            Character File Format:
            1|    Name,Str,Dex,Con,Int,Wis,Cha,Level:Experience,CurrentHealth:MaxHealth,Class,Sub,Race
            2|    Stat Proficiencies (comma separated)(entire line)
            3|    Feats (comma separated)(entire line)
            4|    Languages (comma separated)(entire line)
            5|    Equipment Proficiencies (comma separated)(entire line)
            6|    Coins (platinum,gold,silver,copper)
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
            if(line3[i] != "" && line3[i] != " ") characterFeats.append(line3[i]); // Add feats to the list
        }

        // Get the fourth line of character information
        line = in.readLine();
        QStringList line4 = line.split(",");
        for(int i = 0; i < line4.size(); i++)
        {
            if(line4[i] != "" && line4[i] != " ") characterLanguages.append(line4[i]); // Add languages to the list
        }

        // Get the fifth line of character information
        line = in.readLine();
        QStringList line5 = line.split(",");
        for(int i = 0; i < line5.size(); i++)
        {
            if(line5[i] != "" && line5[i] != " ") characterEquipmentProficiencies.append(line5[i]); // Add equipment proficiencies to the list
        }

        // Get the sixth line of character information
        line = in.readLine();
        QStringList line6 = line.split(",");
        for(int i = 0; i < line6.size(); i++)
        {
            if(line6[i] != "" && line6[i] != " ") characterCoins.append(line6[i].toInt()); // Add coins to the list
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
    qDebug() << "Character Feats: " << characterFeats;
    qDebug() << "Character Languages: " << characterLanguages;
    qDebug() << "Character Equipment Proficiencies: " << characterEquipmentProficiencies;
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

// Allows the user to change the character's profile picture
void ViewCharacter::changeProfilePicture()
{
    // Open file dialog to select an image
    QString fileName = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.bmp *.jpeg)");
    
    if (!fileName.isEmpty())
    {
        // Get the file extension of the selected file
        QFileInfo fileInfo(fileName);
        QString extension = fileInfo.suffix(); // Get file extension
        
        // Construct the new file name based on character's name and extension
        QString newFileName = QDir::currentPath() + "/data/characters/" + characterName + "/character." + extension;


        // Check if a file already exists and delete it
        for(const QString &ext : imageExtentions)
        {
            QString existingFileName = QDir::currentPath() + "/data/characters/" + characterName + "/character." + ext;
            if (QFile::exists(existingFileName))
            {
                QFile::remove(existingFileName); // Delete the existing picture
                qDebug() << "Deleted existing image: " << existingFileName;
            }
        }

        // Copy the selected file to the new location
        QFile::copy(fileName, newFileName); // Copy the selected image to the new name

        // Load and display the new image
        QPixmap newPixmap(newFileName);
        if (!newPixmap.isNull())
        {
            pictureLabel->setPixmap(newPixmap.scaled(pictureLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            qDebug() << "Failed to load image:" << newFileName;
        }

        // Load and display the new image
        loadPicture(newFileName);
    }
}

// The function to load and display the picture
void ViewCharacter::loadPicture(const QString &imagePath)
{
    if (!imagePath.isEmpty())
    {
        QPixmap characterPicture(imagePath);
        const int maxWidth = 200;   // Max width
        const int maxHeight = 300;  // Max height
        
        if (!characterPicture.isNull())
        {
            // Calculate the target aspect ratio (2:3)
            const float targetAspectRatio = 2.0 / 3.0;
            const float currentAspectRatio = static_cast<float>(characterPicture.width()) / characterPicture.height();
            QRect cropRect;

            if (currentAspectRatio > targetAspectRatio)
            {
                // If the image is wider than the target aspect ratio, crop the sides
                int newWidth = static_cast<int>(targetAspectRatio * characterPicture.height());
                int xOffset = (characterPicture.width() - newWidth) / 2;  // Center crop
                cropRect = QRect(xOffset, 0, newWidth, characterPicture.height());
            }
            else
            {
                // If the image is taller than the target aspect ratio, crop the top and bottom
                int newHeight = static_cast<int>(characterPicture.width() / targetAspectRatio);
                int yOffset = (characterPicture.height() - newHeight) / 2;  // Center crop
                cropRect = QRect(0, yOffset, characterPicture.width(), newHeight);
            }

            // Crop and scale the image
            QPixmap croppedPicture = characterPicture.copy(cropRect);
            QPixmap scaledPicture = croppedPicture.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // Set the pixmap to the label
            pictureLabel->setPixmap(scaledPicture);
        }
    }
    else
    {
        qDebug() << "Failed to load image at path:" << imagePath;
        pictureLabel->setText("Image not available");
    }
}

// Function to load the equipped items into the equipped items list
void ViewCharacter::loadEquippedItems()
{
    equippedItemsList->setEnabled(false); // Disable the list because there is no need to interact with
    // Load the character's inventory
    QString characterName = name;
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/inventory.csv";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open inventory file for loading:" << filePath;
        return;
    }

    QTextStream in(&file);
    equippedItemsList->clear();

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        QStringList fields = line.split(",");
        if (fields.size() < 4)
        {
            qWarning() << "Invalid inventory line:" << line;
            continue; // Skip invalid lines
        }

        QString itemName = fields[0];
        int quantity = fields[1].toInt();
        int equipped = fields[2].toInt();
        int attunement = fields[3].toInt();

        if (equipped)
        {   
            QListWidgetItem *item = new QListWidgetItem("(x" + QString::number(quantity) + ") " + itemName);
            item->setData(Qt::UserRole, line); // Store full data string in UserRole
            if(attunement)
            {
                item->setText(item->text() + " [Attuned]");
            }
            equippedItemsList->addItem(item);
        }
    }

    file.close();
}

// Function to load the prepped spells into the prepped spells list
void ViewCharacter::loadPreppedSpells()
{
    // TODO once the character's spell list is implemented
}


ViewCharacter::ViewCharacter(QWidget *parent, QString nameIn) :
    QWidget(parent), pictureLabel(new ClickableLabel(this))
{
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

    // Create the first column
    QWidget *column1 = new QWidget();
    QVBoxLayout *column1Layout = new QVBoxLayout(column1);
    column1Layout->setSpacing(5);
    column1Layout->setContentsMargins(0, 0, 0, 0);

    // Create the character picture
    QString imageDir = QDir::currentPath() + "/data/characters/" + characterName + "/";
    QString imagePath;
    for(const QString &ext : imageExtentions)
    {
        if(QFile::exists(imageDir + "character." + ext))
        {
            imagePath = imageDir + "character." + ext;
            break;
        }
    }

    // Load and display the picture
    loadPicture(imagePath);

    // Define all of the column 1 widgets
    QLabel *nameAndLevelLabel = new QLabel(characterName + " | Level " + QString::number(characterLevel)); // Creates a label for the character's name and level
    QLabel *raceClassAndSubclassLabel = new QLabel(characterRace + " " + characterClass + " | " + characterSubclass); // Creates a label for the character
    QProgressBar *experienceProgressBar = new QProgressBar(); // Creates a progress bar for the character's experience
    experienceProgressBar->setRange(experienceTable[characterLevel-1],experienceTable[characterLevel]); // Sets the range of the progress bar to the lower and upper bounds of the character's current level
    experienceProgressBar->setValue(characterExperience); // Sets the value of the progress bar to the character's current experience
    QLabel *experienceLow = new QLabel(QString::number(experienceTable[characterLevel-1])); // Creates a label for the lower bound of the character's experience
    QLabel *experienceHigh = new QLabel(QString::number(experienceTable[characterLevel])); // Creates a label for the upper bound of the character's experience
    QLabel *experienceCurrent = new QLabel(QString::number(characterExperience)); // Creates a label for the character's current experience

    // Set font for experience labels to be smaller
    experienceLow->setStyleSheet("font-size: 10px;");
    experienceHigh->setStyleSheet("font-size: 10px;");
    experienceCurrent->setStyleSheet("font-size: 10px;");

    // Allow the picture to expand to fill the space
    pictureLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Create a widget for the first column to hold the character information
    QWidget *characterInfoWidget = new QWidget();
    QGridLayout *characterInfoLayout = new QGridLayout(characterInfoWidget);

    // Add all of the character info widgets to the widget
    characterInfoLayout->addWidget(nameAndLevelLabel, 0, 0, 1, 2); // Adds the label with the name and level to the widget
    characterInfoLayout->addWidget(pictureLabel, 0, 2, 5, 2, Qt::AlignRight); // Adds the picture label to the widget
    characterInfoLayout->addWidget(raceClassAndSubclassLabel, 1, 0, 1, 2); // Adds the label with the character race class and subclass to the widget
    characterInfoLayout->addWidget(experienceProgressBar, 6, 0, 1, 4); // Adds the experience progress bar to the widget
    characterInfoLayout->addWidget(experienceLow, 7, 0, 1, 1, Qt::AlignLeft); // Adds the lower bound of the experience to the widget
    characterInfoLayout->addWidget(experienceCurrent, 7, 1, 1, 2, Qt::AlignCenter); // Adds the current experience to the widget
    characterInfoLayout->addWidget(experienceHigh, 7, 3, 1, 1, Qt::AlignRight); // Adds the upper bound of the experience to the widget

    // Add a spacer item to manage the remaining space
    QSpacerItem *column1Spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding); 

    // Add the characterInfoWidget and the spacer to the layout
    column1Layout->addWidget(characterInfoWidget);
    column1Layout->addSpacerItem(column1Spacer);

    // Set stretch factors for the layout
    column1Layout->setStretch(0, 1); // Stretch factor for characterInfoWidget
    column1Layout->setStretch(1, 1); // Stretch factor for the spacer (remaining space)



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
    QString initiativePrefix = (characterInitiative < 0) ? "" : "+"; // Uses a ternary operator to determine if the initiative is negative or positive
    QLabel *initiativeLabel = new QLabel("Initiative:\n" + initiativePrefix + QString::number(characterInitiative)); // Creates a label with the prefix and initiative as the text
    QLabel *armorClassLabel = new QLabel("Armor Class:\n" + QString::number(characterArmorClass)); // Creates a label with the armor class as the text
    QLabel *hitPointsLabel = new QLabel("Hit Points:\n" + QString::number(characterHitPoints) + "/" + QString::number(characterMaxHitPoints)); // Creates a label with the hit points as the text
    
    // Allign the labels to the center
    initiativeLabel->setAlignment(Qt::AlignCenter);
    armorClassLabel->setAlignment(Qt::AlignCenter);
    hitPointsLabel->setAlignment(Qt::AlignCenter);

    // Defining the area for the death saving throws
    QWidget *deathSavingThrows = new QWidget(); // Creates a widget for the death saving throws section
    QWidget *deathSuccesses = new QWidget(); // Creates a widget for the death successes section
    QWidget *deathFails = new QWidget(); // Creates a widget for the death fails section

    // Setting the layouts for each saving throw area
    QVBoxLayout *deathSavingThrowsLayout = new QVBoxLayout(deathSavingThrows); // Creates a vertical layout for the death saving throws section
    deathSavingThrows->setFixedHeight(150);
    QHBoxLayout *deathSuccessesLayout = new QHBoxLayout(deathSuccesses); // Creates a horizontal layout for the death successes section
    QHBoxLayout *deathFailsLayout = new QHBoxLayout(deathFails); // Creates a horizontal layout for the death fails section

    // Align death successes and fails to the right
    deathSavingThrowsLayout->setAlignment(Qt::AlignRight);
    deathSuccessesLayout->setAlignment(Qt::AlignRight);
    deathFailsLayout->setAlignment(Qt::AlignRight);

    deathSuccessesLayout->setSpacing(15); // Sets the spacing between the death saving throws section
    deathFailsLayout->setSpacing(15); // Sets the spacing between the death saving throws section

    // Creating the death saving throw widgets
    QLabel *deathSavingThrowsLabel = new QLabel("Death Saving Throws"); // Creates a label for the death saving throws section
    deathSavingThrowsLabel->setAlignment(Qt::AlignCenter); // Aligns the death saving throws label to the center
    QLabel *deathSuccessesLabel = new QLabel("Successes:"); // Creates a label for the death successes section
    QLabel *deathFailsLabel = new QLabel("Fails:"); // Creates a label for the death fails section

    // Creating the death saving throw checkboxes
    QCheckBox *deathSuccess1 = new QCheckBox(); // Creates a checkboxes for the first death success
    QCheckBox *deathSuccess2 = new QCheckBox(); // Creates a checkboxes for the second death success
    QCheckBox *deathSuccess3 = new QCheckBox(); // Creates a checkboxes for the third death success
    QCheckBox *deathFail1 = new QCheckBox(); // Creates a checkboxes for the first death fail
    QCheckBox *deathFail2 = new QCheckBox(); // Creates a checkboxes for the second death fail
    QCheckBox *deathFail3 = new QCheckBox(); // Creates a checkboxes for the third death fail

    // Sets the style of the checkboxes
    deathSuccess1->setStyleSheet("QCheckBox::indicator:checked{ background-color: green; }");
    deathSuccess2->setStyleSheet("QCheckBox::indicator:checked{ background-color: green; }");
    deathSuccess3->setStyleSheet("QCheckBox::indicator:checked{ background-color: green; }");
    deathFail1->setStyleSheet("QCheckBox::indicator:checked{ background-color: red; }");
    deathFail2->setStyleSheet("QCheckBox::indicator:checked{ background-color: red; }");
    deathFail3->setStyleSheet("QCheckBox::indicator:checked{ background-color: red; }");

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

    // Creates the list widget for the equipped items and the prepped spells and a widget for the list widgets
    QWidget *listsWidget = new QWidget();
    QGridLayout *listsLayout = new QGridLayout(listsWidget);
    

    // Create the labels for the list widgets
    QLabel *equippedItemsLabel = new QLabel("Equipped Items");
    QLabel *preppedSpellsLabel = new QLabel("Prepped Spells");

    // Add the attuned items and prepped spells to the list widgets
    loadEquippedItems();
    loadPreppedSpells();

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
    combatStatsLayout->addWidget(initiativeLabel, 0, 0, 1, 2);
    combatStatsLayout->addWidget(armorClassLabel, 0, 2, 1, 2);
    combatStatsLayout->addWidget(hitPointsLabel, 0, 4, 1, 2);
    combatStatsLayout->addWidget(deathSavingThrows, 1, 3, 1, 3);

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

    // Connect checkboxes
    connect(deathSuccess1, &QCheckBox::stateChanged, this, [deathSuccess2, deathSuccess3](int state)
    {
        // If the first death success checkbox is unchecked, uncheck the other checkboxes
        if(state == Qt::Unchecked)
        {
            deathSuccess2->setCheckState(Qt::Unchecked);
            deathSuccess3->setCheckState(Qt::Unchecked);
        }
    });

    connect(deathSuccess2, &QCheckBox::stateChanged, this, [deathSuccess1, deathSuccess3](int state)
    {
        // If the second death success checkbox is unchecked, uncheck the 3rd checkbox
        if(state == Qt::Unchecked)
        {
            deathSuccess3->setCheckState(Qt::Unchecked);
        }
        // If the second death success checkbox is checked, check the first checkbox
        else if(state == Qt::Checked)
        {
            deathSuccess1->setCheckState(Qt::Checked);
        }
    });

    connect(deathSuccess3, &QCheckBox::stateChanged, this, [deathSuccess1, deathSuccess2](int state)
    {
        // If the third death success checkbox is checked, check the other checkboxes
        if(state == Qt::Checked)
        {
            deathSuccess1->setCheckState(Qt::Checked);
            deathSuccess2->setCheckState(Qt::Checked);
        }
    });

    
    connect(deathFail1, &QCheckBox::stateChanged, this, [deathFail2, deathFail3](int state)
    {
        // If the first death fail checkbox is unchecked, uncheck the other checkboxes
        if(state == Qt::Unchecked)
        {
            deathFail2->setCheckState(Qt::Unchecked);
            deathFail3->setCheckState(Qt::Unchecked);
        }
    });

    connect(deathFail2, &QCheckBox::stateChanged, this, [deathFail1, deathFail3](int state)
    {
        // If the second death fail checkbox is unchecked, uncheck the 3rd checkbox
        if(state == Qt::Unchecked)
        {
            deathFail3->setCheckState(Qt::Unchecked);
        }
        // If the second death fail checkbox is checked, check the first checkbox
        else if(state == Qt::Checked)
        {
            deathFail1->setCheckState(Qt::Checked);
        }
    });

    connect(deathFail3, &QCheckBox::stateChanged, this, [deathFail1, deathFail2](int state)
    {
        // If the third death fail checkbox is checked, check the other checkboxes
        if(state == Qt::Checked)
        {
            deathFail1->setCheckState(Qt::Checked);
            deathFail2->setCheckState(Qt::Checked);
        }
    });

    // Picture Label Click Event
    connect(pictureLabel, &ClickableLabel::clicked, this, &ViewCharacter::changeProfilePicture);

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
