/*
Name: viewCharacter.cpp
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 11/18/2024
*/

#include <QComboBox>
#include <QLineEdit>
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
#include <QSpinBox>
#include <QDialog>
#include <QObject>
#include <QApplication>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <fstream>
#include <iostream>
#include <sstream>

#include "viewCharacter.h"
#include "viewInventory.h"
#include "viewNotes.h"
#include "themeUtils.h" // Include the utility header
#include "utils.h"

#include <QProgressBar>
#include <QString>
#include <QWidget>

// Custom progress bar that can display percentages above 100
class OverProgressBar : public QProgressBar
{
    // Q_OBJECT

public:
    explicit OverProgressBar(QWidget *parent = nullptr) : QProgressBar(parent)
    {
        setTextVisible(true); // Ensure text is visible
    }

    void setValue(int value)
    {
        int maxVal = maximum();
        int minVal = minimum();
        if (value > maxVal)
        {
            // Fill the progress bar and show the percentage exceeding 100%
            QProgressBar::setValue(maxVal);
            int overPercentage = static_cast<double>(value - minVal) / static_cast<double>(maxVal - minVal) * 100.0;
            setFormat(QString::number(overPercentage) + "%");
        }
        else
        {
            // Normal behavior
            setFormat("%p%");
            QProgressBar::setValue(value);
        }
    }
};

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
        {"Survival", 4}};

// When we come back to this screen from the inventory or spells screen, we need to reload the character
void ViewCharacter::loadAll()
{
    loadCharacter(name);                                                              // Load the character's information
    evaluateCharacterModifiers();                                                     // Evaluate the character's modifiers
    loadPicture(QDir::currentPath() + "/data/characters/" + name + "/character.png"); // Load the character's picture
    loadEquippedItems();                                                              // Load the character's equipped items
    loadPreppedSpells();                                                              // Load the character's prepped spells
}

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
            1|    Name,Str,Dex,Con,Int,Wis,Cha,Level:Experience,MaxHealth:CurrentHealth:TempHealth,Class,Sub,Race
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
        for (int i = 1; i < 7; i++)
        {
            characterAbilities.append(line1[i].toInt());
        }
        QStringList levelExperienceList = line1[7].split(":");
        characterLevel = levelExperienceList[0].toInt();
        characterExperience = levelExperienceList[1].toInt();

        //  if character experience is -1, then the character is using milestone leveling
        qDebug() << "Character Experience: " << characterExperience;

        if (characterExperience == -1)
            isMilestone = true;
        else
            isMilestone = false;

        QStringList hitPointsList = line1[8].split(":");
        characterMaxHitPoints = hitPointsList[0].toInt();
        characterHitPoints = hitPointsList[1].toInt();
        characterTempHitPoints = hitPointsList[2].toInt();
        characterClass = line1[9];
        characterSubclass = line1[10];
        characterRace = line1[11];
        characterSubrace = line1[12];

        // Get the second line of character information
        line = in.readLine();
        QStringList line2 = line.split(",");
        for (int i = 0; i < line2.size(); i++)
        {
            if (line2[i] != "" && line2[i] != " ")
                characterSkillProficiencies.append(line2[i]); // Add stat proficiencies to the list
        }

        // Get the third line of character information
        line = in.readLine();
        QStringList line3 = line.split(",");
        for (int i = 0; i < line3.size(); i++)
        {
            if (line3[i] != "" && line3[i] != " ")
                characterFeats.append(line3[i]); // Add feats to the list
        }

        // Get the fourth line of character information
        line = in.readLine();
        QStringList line4 = line.split(",");
        for (int i = 0; i < line4.size(); i++)
        {
            if (line4[i] != "" && line4[i] != " ")
                characterLanguages.append(line4[i]); // Add languages to the list
        }

        // Get the fifth line of character information
        line = in.readLine();
        QStringList line5 = line.split(",");
        for (int i = 0; i < line5.size(); i++)
        {
            if (line5[i] != "" && line5[i] != " ")
                characterEquipmentProficiencies.append(line5[i]); // Add equipment proficiencies to the list
        }

        // Get the sixth line of character information
        line = in.readLine();
        QStringList line6 = line.split(",");
        for (int i = 0; i < line6.size(); i++)
        {
            if (line6[i] != "" && line6[i] != " ")
                characterCoins.append(line6[i].toInt()); // Add coins to the list
        }

        // Close the file
        characterFile.close();

        // Print the character to the console for testing
        // printCharacterToConsole();
    }
}

void ViewCharacter::printCharacterToConsole()
{
    // Make sure that the values are getting set correctly
    qDebug() << "Character Name: " << characterName;
    qDebug() << "Character Abilities: " << characterAbilities;
    qDebug() << "Character Level: " << characterLevel;
    qDebug() << "Character Experience: " << characterExperience;
    qDebug() << "Lower Bound Experience: " << experienceTable[characterLevel - 1];
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
    for (int i = 0; i < 6; i++)
        characterAbilityBonuses.append((int)floor((float)(characterAbilities[i] - 10) / 2));

    // Calculate the character's saving throws
    for (int i = 0; i < 6; i++)
        characterSavingThrows.append(characterAbilityBonuses[i]);

    // Evaluate the character's skill bonuses
    for (QMap<QString, int>::const_iterator it = skillMap.constBegin(); it != skillMap.constEnd(); it++)
    {
        int abilityIndex = it.value();

        // Check if the character is proficient in the skill
        if (characterSkillProficiencies.contains(it.key()))
            characterSkillBonuses.append(characterAbilityBonuses[abilityIndex] + characterProficiencyBonus);
        else
            characterSkillBonuses.append(characterAbilityBonuses[abilityIndex]);
    }

    // Evaluate initiative
    characterInitiative = characterAbilityBonuses[1];

    // Evaluate armor class
    if (/*Has armor equipped       leave false until implemented*/ false)
    {
        // Do armor calculations
    }
    else
        characterArmorClass = 10 + characterAbilityBonuses[1];

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
        for (const QString &ext : imageExtentions)
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
        const int maxWidth = 200;  // Max width
        const int maxHeight = 300; // Max height

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
                int xOffset = (characterPicture.width() - newWidth) / 2; // Center crop
                cropRect = QRect(xOffset, 0, newWidth, characterPicture.height());
            }
            else
            {
                // If the image is taller than the target aspect ratio, crop the top and bottom
                int newHeight = static_cast<int>(characterPicture.width() / targetAspectRatio);
                int yOffset = (characterPicture.height() - newHeight) / 2; // Center crop
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
    equippedItemsList->setSelectionMode(QAbstractItemView::NoSelection);   // Disable selection
    equippedItemsList->setFocusPolicy(Qt::NoFocus);                        // Disable focus
    equippedItemsList->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable editing
    equippedItemsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
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
            QListWidgetItem *item = new QListWidgetItem("(" + QString::number(quantity) + ") " + itemName);
            item->setData(Qt::UserRole, line); // Store full data string in UserRole
            if (attunement)
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
    // Load the character's inventory
    QString characterName = name;
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/spells.csv";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open spells file for loading:" << filePath;
        return;
    }

    QTextStream in(&file);
    preppedSpellsList->clear();

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        QStringList fields = line.split(",");
        if (fields.size() < 13)
        {
            qWarning() << "Invalid spell line:" << line;
            continue; // Skip invalid lines
        }

        QString spellName = fields[0];
        int isPrepared = fields[11].toInt();

        if (isPrepared)
        {
            QListWidgetItem *item = new QListWidgetItem(spellName);
            item->setData(Qt::UserRole, line); // Store full data string in UserRole
            preppedSpellsList->addItem(item);
        }
    }

    file.close();
}

void ViewCharacter::loadFeatures()
{
    qDebug() << "In loadFeatures";
    QFile file("data/databases/" + characterClass + ".tsv"); // read in the file
    qDebug() << "Current working directory:" << QDir::currentPath();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // error checking for debugging
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    QTextStream in(&file); // read in the file
    bool isHeader = true;  // our first line is a header

    while (!in.atEnd())
    {
        QString line = in.readLine(); // read in the line
        if (isHeader)
        {
            isHeader = false; // we are no longer on the header
            continue;         // skip the header
        }

        QStringList fields = line.split("\t"); // split the line by \t

        // Fill out info
        FeatureInfo *info = new FeatureInfo{
            fields[1],
            fields[2],
            fields[3].toInt(),
            fields[4]};

        this->featureList.append(info);
    }
}

void ViewCharacter::loadFeats()
{
    qDebug() << "In loadFeats";
    QFile file("data/databases/Feats.tsv"); // read in the file
    qDebug() << "Current working directory:" << QDir::currentPath();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // error checking for debugging
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    QTextStream in(&file); // read in the file
    bool isHeader = true;  // our first line is a header

    while (!in.atEnd())
    {
        QString line = in.readLine(); // read in the line
        if (isHeader)
        {
            isHeader = false; // we are no longer on the header
            continue;         // skip the header
        }

        QStringList fields = line.split("\t"); // split the line by \t

        QString featName = fields[0];

        // Fill out info
        FeatInfo *info = new FeatInfo{
            fields[1],
            fields[2].toInt(),
            fields[3],
            fields[4],
            fields[5],
            fields[6],
            fields[7]};

        this->featList[featName] = info;
        // qDebug() << fields[0];
        // qDebug() << fields[1];
        // qDebug() << fields[2];
        // qDebug() << fields[3];
        // qDebug() << fields[4];
        // qDebug() << fields[5];
        // qDebug() << fields[6];
        // qDebug() << fields[7];
    }
}

ViewCharacter::ViewCharacter(QWidget *parent, QString nameIn) : QWidget(parent), pictureLabel(new ClickableLabel(this))
{
    loadCharacter(nameIn);
    loadFeatures();
    loadFeats();
    // printCharacterToConsole();
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
    for (const QString &ext : imageExtentions)
    {
        if (QFile::exists(imageDir + "character." + ext))
        {
            imagePath = imageDir + "character." + ext;
            break;
        }
    }

    // Load and display the picture
    loadPicture(imagePath);

    // Define all of the column 1 widgets
    QLabel *nameAndLevelLabel = new QLabel(characterName + " | Level " + QString::number(characterLevel));            // Creates a label for the character's name and level
    QLabel *raceClassAndSubclassLabel = new QLabel(characterRace + " " + characterClass + " | " + characterSubclass); // Creates a label for the character
    OverProgressBar *experienceProgressBar = new OverProgressBar();                                                   // Creates a progress bar for the character's experience
    experienceProgressBar->setRange(experienceTable[characterLevel - 1], experienceTable[characterLevel]);            // Sets the range of the progress bar to the lower and upper bounds of the character's current level
    experienceProgressBar->setValue(characterExperience);                                                             // Sets the value of the progress bar to the character's current experience
    QLabel *experienceLow = new QLabel(QString::number(experienceTable[characterLevel - 1]));                         // Creates a label for the lower bound of the character's experience
    QLabel *experienceHigh = new QLabel(QString::number(experienceTable[characterLevel]));                            // Creates a label for the upper bound of the character's experience
    QLabel *experienceCurrent = new QLabel(QString::number(characterExperience));                                     // Creates a label for the character's current experience
    QPushButton *levelUpButton = new QPushButton("Level Up");                                                         // Creates a button to level up the character
    QPushButton *addExperienceButton = new QPushButton("Add XP");                                                     // Creates a button to add experience to the character

    // Set font for experience labels to be smaller
    experienceLow->setStyleSheet("font-size: 10px;");
    experienceHigh->setStyleSheet("font-size: 10px;");
    experienceCurrent->setStyleSheet("font-size: 10px;");

    // Allow the picture to expand to fill the space
    pictureLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Create a widget for the first column to hold the character information
    QWidget *characterInfoWidget = new QWidget();
    QGridLayout *characterInfoLayout = new QGridLayout(characterInfoWidget);

    QWidget *coins = new QWidget();
    QGridLayout *coinsLayout = new QGridLayout(coins);
    QLabel *platinumLabel = new QLabel("PP\n" + QString::number(characterCoins[0]));
    QLabel *goldLabel = new QLabel("GP\n" + QString::number(characterCoins[1]));
    QLabel *silverLabel = new QLabel("SP\n" + QString::number(characterCoins[2]));
    QLabel *copperLabel = new QLabel("CP\n" + QString::number(characterCoins[3]));
    QPushButton *coinsButton = new QPushButton("Edit Coins");
    coinsButton->setFixedHeight(30);

    // Add all of the coin widgets to the coins widget
    coinsLayout->addWidget(platinumLabel, 0, 0, Qt::AlignCenter);
    coinsLayout->addWidget(goldLabel, 0, 1, Qt::AlignCenter);
    coinsLayout->addWidget(silverLabel, 1, 0, Qt::AlignCenter);
    coinsLayout->addWidget(copperLabel, 1, 1, Qt::AlignCenter);
    coinsLayout->addWidget(coinsButton, 2, 0, 1, 2, Qt::AlignCenter);

    // Add all of the character info widgets to the widget
    characterInfoLayout->addWidget(nameAndLevelLabel, 0, 0, 1, 2);            // Adds the label with the name and level to the widget
    characterInfoLayout->addWidget(pictureLabel, 0, 2, 5, 2, Qt::AlignRight); // Adds the picture label to the widget
    characterInfoLayout->addWidget(raceClassAndSubclassLabel, 1, 0, 1, 2);    // Adds the label with the character race class and subclass to the widget
    characterInfoLayout->addWidget(coins, 2, 0, 3, 2);

    // Set fixed width to 150px
    levelUpButton->setFixedHeight(30);
    // Set fixed height for the add experience button to 30px and set fixed width to 150px
    addExperienceButton->setFixedHeight(30);
    addExperienceButton->setFixedWidth(150);

    if (isMilestone)
    {
        characterInfoLayout->addWidget(levelUpButton, 5, 0, 1, 2);
    }
    else
    {
        characterInfoLayout->addWidget(experienceProgressBar, 7, 0, 1, 4);              // Adds the experience progress bar to the widget
        characterInfoLayout->addWidget(experienceLow, 6, 0, Qt::AlignLeft);             // Adds the lower bound of the experience to the widget
        characterInfoLayout->addWidget(experienceCurrent, 6, 1, 1, 2, Qt::AlignCenter); // Adds the current experience to the widget
        characterInfoLayout->addWidget(experienceHigh, 6, 3, Qt::AlignRight);           // Adds the upper bound of the experience to the widget

        characterInfoLayout->addWidget(addExperienceButton, 8, 0, 1, 2, Qt::AlignLeft); // Adds the add experience button to the widget
        characterInfoLayout->addWidget(levelUpButton, 8, 2, 1, 2, Qt::AlignRight);      // Adds the level up button to the widget
        levelUpButton->setFixedWidth(150);                                              // Set fixed height for the level up button to 30px
    }

    // Add a spacer item to manage the remaining space
    QSpacerItem *column1Spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // Add the characterInfoWidget and the spacer to the layout
    column1Layout->addWidget(characterInfoWidget);
    column1Layout->addSpacerItem(column1Spacer);

    // These will eventually be moved to their final location
    // column1Layout->addWidget(levelUpButton);

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
    statsLayout->addWidget(new QLabel("Ability"), 0, 0);       // Creates a label for the ability names section header
    statsLayout->addWidget(new QLabel("Score"), 1, 0);         // Creates a label for the ability scores section header
    statsLayout->addWidget(new QLabel("Modifier"), 2, 0);      // Creates a label for the ability modifiers section header
    statsLayout->addWidget(new QLabel("Saving Throws"), 3, 0); // Creates a label for the saving throws section header

    // Add the ability names and values to the stats widget
    for (int i = 0; i < abilitiesNames.length(); i++)
    {
        statsLayout->addWidget(new QLabel(abilitiesNames[i]), 0, i + 1); // Adds the ability names to the list
    }

    QLabel *strLabel = new QLabel(QString::number(characterAbilities[0]));
    QLabel *dexLabel = new QLabel(QString::number(characterAbilities[1]));
    QLabel *conLabel = new QLabel(QString::number(characterAbilities[2]));
    QLabel *intLabel = new QLabel(QString::number(characterAbilities[3]));
    QLabel *wisLabel = new QLabel(QString::number(characterAbilities[4]));
    QLabel *chaLabel = new QLabel(QString::number(characterAbilities[5]));
    statsLayout->addWidget(strLabel, 1, 1);
    statsLayout->addWidget(dexLabel, 1, 2);
    statsLayout->addWidget(conLabel, 1, 3);
    statsLayout->addWidget(intLabel, 1, 4);
    statsLayout->addWidget(wisLabel, 1, 5);
    statsLayout->addWidget(chaLabel, 1, 6);

    // Add ability modifiers below ability scores
    for (int i = 0; i < characterAbilityBonuses.length(); i++)
    {
        QString prefix = ""; // Creates a prefix for the modifier
        if (characterSavingThrows[i] >= 0)
            prefix = "+";                                                                                   // Adds a plus sign to the front of the modifier if it is positive or zero
        statsLayout->addWidget(new QLabel(prefix + QString::number(characterAbilityBonuses[i])), 2, i + 1); // Adds the ability modifiers to the list
    }

    // Add the saving throws to the stats widget
    for (int i = 0; i < abilitiesNames.length(); i++)
    {
        QString prefix = ""; // Creates a prefix for the saving throw
        if (characterSavingThrows[i] >= 0)
            prefix = "+";                                                                                 // Adds a plus sign to the front of the saving throw if it is positive or zero
        statsLayout->addWidget(new QLabel(prefix + QString::number(characterSavingThrows[i])), 3, i + 1); // Adds the saving throws to the list
    }

    // Create a skills widget for the second column
    QWidget *skillsWidget = new QWidget();                     // Creates a widget for the skills section
    QGridLayout *skillsLayout = new QGridLayout(skillsWidget); // Creates a grid layout for the skills section

    // Add all of the skill widgets to the skills widget
    QLabel *skillsLabel = new QLabel("Skills\n");                                                                     // Creates a label for the skills section header
    skillsLabel->setAlignment(Qt::AlignLeft);                                                                         // Aligns the skills label to the left
    QLabel *proficiencyBonusLabel = new QLabel("Proficiency Bonus:\n+" + QString::number(characterProficiencyBonus)); // Adds the proficiency bonus to the label
    proficiencyBonusLabel->setAlignment(Qt::AlignRight);                                                              // Aligns the proficiency bonus label to the right
    skillsLayout->addWidget(skillsLabel, 0, 0);                                                                       // Adds the skills label to the list
    skillsLayout->addWidget(proficiencyBonusLabel, 0, 1, 1, 2);                                                       // Adds the proficiency bonus label to the list
    for (int i = 0; i < skillMap.count(); i++)
    {
        QString prefix = "";
        if (characterSkillBonuses[i] >= 0)
            prefix = "+";                                                                                                   // Adds a plus sign to the front of the skill bonus if it is positive or zero
        QLabel *skillLabel = new QLabel(skillMap.keys().at(i) + "\n" + prefix + QString::number(characterSkillBonuses[i])); // Uses the skill map to get the skill name and adds the skill bonus to the label
        skillLabel->setAlignment(Qt::AlignCenter);                                                                          // Aligns the skill label to the center
        skillsLayout->addWidget(skillLabel, (i / 3) + 1, i % 3);                                                            // Adds the skill label to the list
    }

    // Add the stats widget to column 2
    column2Layout->addWidget(statsWidget);  // Adds the stats widget to the list
    column2Layout->addWidget(skillsWidget); // Adds the skills widget to the list

    // Create the third column
    QWidget *column3 = new QWidget();
    QVBoxLayout *column3Layout = new QVBoxLayout(column3);

    // Define all of the column 3 widgets
    QWidget *combatStatsWidget = new QWidget();                          // Creates a widget for the combat stats section
    QGridLayout *combatStatsLayout = new QGridLayout(combatStatsWidget); // Creates a grid layout for the combat stats section
    // combatStatsWidget->setFixedHeight(100); // Sets the height of the combat stats widget to 100px
    QString initiativePrefix = (characterInitiative < 0) ? "" : "+";                                                                           // Uses a ternary operator to determine if the initiative is negative or positive
    QLabel *initiativeLabel = new QLabel("Initiative:\n" + initiativePrefix + QString::number(characterInitiative));                           // Creates a label with the prefix and initiative as the text
    QLabel *armorClassLabel = new QLabel("Armor Class:\n" + QString::number(characterArmorClass));                                             // Creates a label with the armor class as the text
    QLabel *hitPointsLabel = new QLabel("Hit Points:\n" + QString::number(characterHitPoints) + "/" + QString::number(characterMaxHitPoints)); // Creates a label with the hit points as the text

    // Allign the labels to the center
    initiativeLabel->setAlignment(Qt::AlignCenter);
    armorClassLabel->setAlignment(Qt::AlignCenter);
    hitPointsLabel->setAlignment(Qt::AlignCenter);

    // Defining the area for the death saving throws
    QWidget *deathSavingThrows = new QWidget(); // Creates a widget for the death saving throws section
    QWidget *deathSuccesses = new QWidget();    // Creates a widget for the death successes section
    QWidget *deathFails = new QWidget();        // Creates a widget for the death fails section

    // Setting the layouts for each saving throw area
    QVBoxLayout *deathSavingThrowsLayout = new QVBoxLayout(deathSavingThrows); // Creates a vertical layout for the death saving throws section
    deathSavingThrows->setFixedHeight(150);
    QHBoxLayout *deathSuccessesLayout = new QHBoxLayout(deathSuccesses); // Creates a horizontal layout for the death successes section
    QHBoxLayout *deathFailsLayout = new QHBoxLayout(deathFails);         // Creates a horizontal layout for the death fails section

    // Align death successes and fails to the right
    deathSavingThrowsLayout->setAlignment(Qt::AlignRight);
    deathSuccessesLayout->setAlignment(Qt::AlignRight);
    deathFailsLayout->setAlignment(Qt::AlignRight);

    deathSuccessesLayout->setSpacing(15); // Sets the spacing between the death saving throws section
    deathFailsLayout->setSpacing(15);     // Sets the spacing between the death saving throws section

    // Creating the death saving throw widgets
    QLabel *deathSavingThrowsLabel = new QLabel("Death Saving Throws"); // Creates a label for the death saving throws section
    deathSavingThrowsLabel->setAlignment(Qt::AlignCenter);              // Aligns the death saving throws label to the center
    QLabel *deathSuccessesLabel = new QLabel("Successes:");             // Creates a label for the death successes section
    QLabel *deathFailsLabel = new QLabel("Fails:");                     // Creates a label for the death fails section

    // Creating the death saving throw checkboxes
    QCheckBox *deathSuccess1 = new QCheckBox(); // Creates a checkboxes for the first death success
    QCheckBox *deathSuccess2 = new QCheckBox(); // Creates a checkboxes for the second death success
    QCheckBox *deathSuccess3 = new QCheckBox(); // Creates a checkboxes for the third death success
    QCheckBox *deathFail1 = new QCheckBox();    // Creates a checkboxes for the first death fail
    QCheckBox *deathFail2 = new QCheckBox();    // Creates a checkboxes for the second death fail
    QCheckBox *deathFail3 = new QCheckBox();    // Creates a checkboxes for the third death fail

    // Sets the style of the checkboxes
    deathSuccess1->setStyleSheet("QCheckBox::indicator:checked{ background-color: green; }");
    deathSuccess2->setStyleSheet("QCheckBox::indicator:checked{ background-color: green; }");
    deathSuccess3->setStyleSheet("QCheckBox::indicator:checked{ background-color: green; }");
    deathFail1->setStyleSheet("QCheckBox::indicator:checked{ background-color: red; }");
    deathFail2->setStyleSheet("QCheckBox::indicator:checked{ background-color: red; }");
    deathFail3->setStyleSheet("QCheckBox::indicator:checked{ background-color: red; }");

    // Add the death successes and fails to their respective layouts
    deathSuccessesLayout->addWidget(deathSuccessesLabel); // Adds the death successes label to the list
    deathSuccessesLayout->addWidget(deathSuccess1);       // Adds the first death success radio button to the list
    deathSuccessesLayout->addWidget(deathSuccess2);       // Adds the second death success radio button to the list
    deathSuccessesLayout->addWidget(deathSuccess3);       // Adds the third death success radio button to the list
    deathFailsLayout->addWidget(deathFailsLabel);         // Adds the death fails label to the list
    deathFailsLayout->addWidget(deathFail1);              // Adds the first death fail radio button to the list
    deathFailsLayout->addWidget(deathFail2);              // Adds the second death fail radio button to the list
    deathFailsLayout->addWidget(deathFail3);              // Adds the third death fail radio button to the list

    // Add the death successes and fails to the death saving throws layout
    deathSavingThrowsLayout->addWidget(deathSavingThrowsLabel); // Adds the death saving throws label to the list
    deathSavingThrowsLayout->addWidget(deathSuccesses);         // Adds the death successes section to the list
    deathSavingThrowsLayout->addWidget(deathFails);             // Adds the death fails section to the list

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
    connect(Backbutton, SIGNAL(clicked()), SLOT(goBack()));

    // Make inventory button go to inventory page
    connect(inventoryButton, SIGNAL(clicked()), SLOT(goToInventory()));

    // Make spells button go to inventory page
    connect(spellsButton, SIGNAL(clicked()), SLOT(goToSpells()));

    // Make notes button go to notes page
    connect(notesButton, SIGNAL(clicked()), SLOT(goToNotes()));

    // Connect checkboxes
    connect(deathSuccess1, &QCheckBox::stateChanged, this, [deathSuccess2, deathSuccess3](int state)
            {
        // If the first death success checkbox is unchecked, uncheck the other checkboxes
        if(state == Qt::Unchecked)
        {
            deathSuccess2->setCheckState(Qt::Unchecked);
            deathSuccess3->setCheckState(Qt::Unchecked);
        } });

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
        } });

    connect(deathSuccess3, &QCheckBox::stateChanged, this, [deathSuccess1, deathSuccess2](int state)
            {
        // If the third death success checkbox is checked, check the other checkboxes
        if(state == Qt::Checked)
        {
            deathSuccess1->setCheckState(Qt::Checked);
            deathSuccess2->setCheckState(Qt::Checked);
        } });

    connect(deathFail1, &QCheckBox::stateChanged, this, [deathFail2, deathFail3](int state)
            {
        // If the first death fail checkbox is unchecked, uncheck the other checkboxes
        if(state == Qt::Unchecked)
        {
            deathFail2->setCheckState(Qt::Unchecked);
            deathFail3->setCheckState(Qt::Unchecked);
        } });

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
        } });

    connect(deathFail3, &QCheckBox::stateChanged, this, [deathFail1, deathFail2](int state)
            {
        // If the third death fail checkbox is checked, check the other checkboxes
        if(state == Qt::Checked)
        {
            deathFail1->setCheckState(Qt::Checked);
            deathFail2->setCheckState(Qt::Checked);
        } });

    // Picture Label Click Event
    connect(pictureLabel, &ClickableLabel::clicked, this, &ViewCharacter::changeProfilePicture);

    // connect level up button to levelUp function
    connect(levelUpButton, &QPushButton::clicked, [this, experienceProgressBar, experienceLow, experienceHigh, nameAndLevelLabel, strLabel, dexLabel, conLabel, intLabel, wisLabel, chaLabel, hitPointsLabel]()
            {
        levelUp();
        experienceProgressBar->setRange(experienceTable[characterLevel - 1], experienceTable[characterLevel]);
        experienceProgressBar->setValue(characterExperience);
        experienceLow->setText(QString::number(experienceTable[characterLevel - 1]));
        experienceHigh->setText(QString::number(experienceTable[characterLevel]));

        nameAndLevelLabel->setText(characterName + " | Level " + QString::number(characterLevel));
        strLabel->setText(QString::number(characterAbilities[0]));
        dexLabel->setText(QString::number(characterAbilities[1]));
        conLabel->setText(QString::number(characterAbilities[2]));
        intLabel->setText(QString::number(characterAbilities[3]));
        wisLabel->setText(QString::number(characterAbilities[4]));
        chaLabel->setText(QString::number(characterAbilities[5])); 
        hitPointsLabel->setText("Hit Points:\n" + QString::number(characterHitPoints) + "/" + QString::number(characterMaxHitPoints)); });

    // connect add experience button to addExperience function
    connect(addExperienceButton, &QPushButton::clicked, [this, experienceProgressBar, experienceCurrent]()
            {
                addExperience();
                experienceProgressBar->setValue(characterExperience);
                experienceCurrent->setText(QString::number(characterExperience)); });

    connect(coinsButton, &QPushButton::clicked, [this, platinumLabel, goldLabel, silverLabel, copperLabel]()
            {
        editCoins();
        platinumLabel->setText("PP\n" + QString::number(characterCoins[0]));
        goldLabel->setText("GP\n" + QString::number(characterCoins[1]));
        silverLabel->setText("SP\n" + QString::number(characterCoins[2]));
        copperLabel->setText("CP\n" + QString::number(characterCoins[3])); });

    reloadTheme(); // Reload the theme after everything is placed
}

void ViewCharacter::editCoins()
{
    // Create a popup to edit the coins
    QDialog popup;
    popup.setWindowTitle("Edit Coins");
    popup.setFixedSize(400, 200);

    // Create a layout for the popup
    QGridLayout layout(&popup);

    // Create the widgets for the popup
    QLabel *platinumLabel = new QLabel("Platnum:\n" + QString::number(characterCoins[0])); // Creates a label for the platinum coins
    QLabel *goldLabel = new QLabel("Gold:\n" + QString::number(characterCoins[1]));        // Creates a label for the gold coins
    QLabel *silverLabel = new QLabel("Silver:\n" + QString::number(characterCoins[2]));    // Creates a label for the silver coins
    QLabel *copperLabel = new QLabel("Copper:\n" + QString::number(characterCoins[3]));    // Creates a label for the copper coins
    QSpinBox *platinumEdit = new QSpinBox();                                               // Creates a text edit for the platinum coins
    QSpinBox *goldEdit = new QSpinBox();                                                   // Creates a text edit for the gold coins
    QSpinBox *silverEdit = new QSpinBox();                                                 // Creates a text edit for the silver coins
    QSpinBox *copperEdit = new QSpinBox();                                                 // Creates a text edit for the copper coins
    QPushButton *addCoins = new QPushButton("Add Coins");                                  // Creates a button to add coins
    QPushButton *removeCoins = new QPushButton("Remove Coins");                            // Creates a button to remove coins

    // Set widget properties
    addCoins->setFixedHeight(30);    // Set the height of the add coins button to 30px
    addCoins->setFixedWidth(150);    // Set the width of the add coins button to 150px
    removeCoins->setFixedHeight(30); // Set the height of the remove coins button to 30px
    removeCoins->setFixedWidth(150); // Set the width of the remove coins button to 150px
    platinumEdit->setRange(0, 1000); // Sets the range of the platinum coins to 0-1000
    goldEdit->setRange(0, 1000);     // Sets the range of the gold coins to 0-1000
    silverEdit->setRange(0, 1000);   // Sets the range of the silver coins to 0-1000
    copperEdit->setRange(0, 1000);   // Sets the range of the copper coins to 0-1000

    // Add the widgets to the layout
    layout.addWidget(platinumLabel, 0, 0, Qt::AlignCenter);                                            // Adds the platinum label to the layout
    layout.addWidget(platinumEdit, 1, 0);                                                              // Adds the platinum edit to the layout
    layout.addWidget(goldLabel, 0, 1, Qt::AlignCenter);                                                // Adds the gold label to the layout
    layout.addWidget(goldEdit, 1, 1);                                                                  // Adds the gold edit to the layout
    layout.addWidget(silverLabel, 0, 2, Qt::AlignCenter);                                              // Adds the silver label to the layout
    layout.addWidget(silverEdit, 1, 2);                                                                // Adds the silver edit to the layout
    layout.addWidget(copperLabel, 0, 3, Qt::AlignCenter);                                              // Adds the copper label to the layout
    layout.addWidget(copperEdit, 1, 3);                                                                // Adds the copper edit to the layout
    layout.addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 0, 1, 4); // Adds a spacer to the layout
    layout.addWidget(addCoins, 3, 0, 1, 2, Qt::AlignLeft);                                             // Adds the add coins button to the layout
    layout.addWidget(removeCoins, 3, 2, 1, 2, Qt::AlignRight);                                         // Adds the remove coins button to the layout
    layout.setContentsMargins(10, 30, 10, 30);                                                         // Sets the margins of the layout to 10px

    // Connect the add coins button to the addCoins function
    QObject::connect(addCoins, &QPushButton::clicked, &popup, &QDialog::accept);
    QObject::connect(removeCoins, &QPushButton::clicked, [&popup, this, platinumEdit, platinumLabel, goldEdit, goldLabel, silverEdit, silverLabel, copperEdit, copperLabel]()
                     {
        bool anyExceeded = false; // Creates a boolean to check if any coins exceeded the amount the character has

        if(platinumEdit->value() > characterCoins[0]) // If the platinum edit value is greater than the character's platinum coins
        {
            animateLabelBackground(platinumLabel); // Animate the platinum label background
            anyExceeded = true; // Set anyExceeded to true
        }

        if(goldEdit->value() > characterCoins[1]) // If the gold edit value is greater than the character's gold coins
        {
            animateLabelBackground(goldLabel); // Animate the gold label background
            anyExceeded = true; // Set anyExceeded to true
        }

        if(silverEdit->value() > characterCoins[2]) // If the silver edit value is greater than the character's silver coins
        {
            animateLabelBackground(silverLabel); // Animate the silver label background
            anyExceeded = true; // Set anyExceeded to true
        }

        if(copperEdit->value() > characterCoins[3]) // If the copper edit value is greater than the character's copper coins
        {
            animateLabelBackground(copperLabel); // Animate the copper label background
            anyExceeded = true; // Set anyExceeded to true
        }

        if(!anyExceeded) // If no coins exceeded the amount the character has, accept the dialog
        {
            popup.done(2); // Accept the dialog
        } });

    popup.exec(); // Execute the popup

    // Add coins if the add coins button was clicked
    if (popup.result() == QDialog::Accepted)
    {
        characterCoins[0] += platinumEdit->value(); // Adds the platinum coins to the character's coins
        characterCoins[1] += goldEdit->value();     // Adds the gold coins to the character's coins
        characterCoins[2] += silverEdit->value();   // Adds the silver coins to the character's coins
        characterCoins[3] += copperEdit->value();   // Adds the copper coins to the character's coins
    }

    // Remove coins if the remove coins button was clicked
    if (popup.result() == 2)
    {
        // if(platinumEdit->value() > characterCoins[0] || goldEdit->value() > characterCoins[1] || silverEdit->value() > characterCoins[2] || copperEdit->value() > characterCoins[3])
        // {

        // }
        // else
        // {
        characterCoins[0] -= platinumEdit->value(); // Removes the platinum coins from the character's coins
        characterCoins[1] -= goldEdit->value();     // Removes the gold coins from the character's coins
        characterCoins[2] -= silverEdit->value();   // Removes the silver coins from the character's coins
        characterCoins[3] -= copperEdit->value();   // Removes the copper coins from the character's coins
        // }
    }
    saveCoins();
}

void ViewCharacter::animateLabelBackground(QLabel *label)
{
    if (!label)
        return; // Return if the label is null

    label->setStyleSheet("color: red;"); // Set the label's stylesheet to red

    QTimer::singleShot(3000, [label]()
                       {
                           label->setStyleSheet(""); // Reset the label's stylesheet after 3 seconds
                       });

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this); // Create an opacity effect for the label
    label->setGraphicsEffect(effect);                                  // Set the opacity effect for the label

    // Create the animation for the opacity effect
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity", this);

    animation->setDuration(500);                        // Set the duration of the animation to 500ms
    animation->setStartValue(0.0);                      // Set the start value of the animation to 0.0
    animation->setEndValue(1.0);                        // Set the end value of the animation to 1.0
    animation->setEasingCurve(QEasingCurve::InOutQuad); // Set the easing curve of the animation
    animation->setLoopCount(6);                         // Set the loop count of the animation to 6

    animation->start(QAbstractAnimation::DeleteWhenStopped); // Start the animation
}

void ViewCharacter::saveCoins()
{
    // Saves coins to the character's character.csv file
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/character.csv"; // Creates a file path for the character's character.csv file
    QFile file(filePath);                                                                            // Creates a file object for the character's character.csv file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open character file for saving coins:" << filePath; // Outputs a warning if the file fails to open
        return;
    }

    QTextStream in(&file); // Create a text stream to read the file
    QStringList lines;     // Create a list of strings to store the lines of the file
    while (!in.atEnd())
    {
        lines.append(in.readLine()); // Read each line of the file
    }

    QString coins = QString::number(characterCoins[0]) + "," +
                    QString::number(characterCoins[1]) + "," +
                    QString::number(characterCoins[2]) + "," +
                    QString::number(characterCoins[3]); // Creates a string for the coins

    lines[5] = coins; // Update the coins line

    file.close(); // Close the file

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open character file for saving coins:" << filePath;
        return;
    }

    QTextStream out(&file); // Create a text stream to write to the file

    for (const QString &line : lines)
    {
        out << line << "\n"; // Write each line to the file
    }

    file.close(); // Close the file
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

void ViewCharacter::levelUp()
{
    qDebug() << "Level Up Button Clicked";
    if (characterExperience < experienceTable[characterLevel] && isMilestone == false)
    {
        return;
    }

    // Increase level
    characterLevel += 1;

    if (isSpellcaster())
    {
        for (int i = 0; i < 2; i++)
        {
            addSpell();
        }
    }

    QDialog popup;
    popup.setWindowModality(Qt::ApplicationModal);
    popup.setWindowTitle("Level Up");

    QVBoxLayout layout(&popup);

    for (int i = 0; i < featureList.size(); ++i)
    {
        // List of new features from level up
        FeatureInfo *info = featureList[i];
        if (info->level == characterLevel && (info->subClass == characterSubclass || info->subClass == "Base"))
        {
            QString feature = info->featureName;
            QString description = info->description;
            QLabel *featureLabel = new QLabel(feature);
            QLabel *descriptionLabel = new QLabel(description);
            descriptionLabel->setWordWrap(true);
            layout.addWidget(featureLabel);
            layout.addWidget(descriptionLabel);
        }
    }

    if (this->abilityScoreImprovementLevels.contains(characterLevel) && characterLevel != 4)
    {
        // For ability score improvement levels above 4
        QString feature = featureList[2]->featureName;
        QString description = featureList[2]->description;
        QLabel *featureLabel = new QLabel(feature);
        QLabel *descriptionLabel = new QLabel(description);
        descriptionLabel->setWordWrap(true);
        layout.addWidget(featureLabel);
        layout.addWidget(descriptionLabel);
    }

    // Choose feat
    QComboBox *featComboBox = new QComboBox();
    if (this->abilityScoreImprovementLevels.contains(characterLevel))
    {

        featComboBox->setStyleSheet("QComboBox { combobox-popup: 0; }");
        featComboBox->setMaxVisibleItems(10);

        for (auto it = featList.begin(); it != featList.end(); ++it)
        {
            featComboBox->addItem(it.key());
        }
        layout.addWidget(featComboBox);
    }

    // make health spinbox
    QLabel *hpLabel = new QLabel("HP Increase:");
    QSpinBox *hpEdit = new QSpinBox();
    hpEdit->setMaximum(hitDie[characterClass]);
    layout.addWidget(hpLabel);
    layout.addWidget(hpEdit);

    // continue button
    QPushButton *continueButton = new QPushButton("Continue");

    // make widget for continue button
    QWidget buttonWidget;
    buttonWidget.setFixedHeight(50);

    // make layout for add and cancel buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout(&buttonWidget);
    buttonLayout->addWidget(continueButton);
    layout.addWidget(&buttonWidget, Qt::AlignBottom);

    // connect the buttons
    QObject::connect(continueButton, &QPushButton::clicked, &popup, &QDialog::accept);
    // QObject::connect(cancelButton, &QPushButton::clicked, &popup, &QDialog::reject);

    // show the popup
    popup.exec();

    // if the user clicks the continue button
    if (popup.result() == QDialog::Accepted)
    {
        if (this->abilityScoreImprovementLevels.contains(characterLevel))
        {
            // Update feats or ability scores
            QString feat = featComboBox->currentText();
            FeatInfo *info = featList[feat];
            QStringList abilityScoreImprovements = info->abilityScoreImprovements.split(":");
            for (int i = 0; i < characterAbilities.size(); ++i)
            {
                characterAbilities[i] += abilityScoreImprovements[i].toInt();
            }
            characterFeats.append(feat);
        }
        // Update max hp
        characterMaxHitPoints += hpEdit->value();
        // Save to csv
        saveCharacterStatsAndFeats();
    }
}

void ViewCharacter::addSpell()
{
    qDebug() << "In addSpell";
    QDialog popup;

    popup.setWindowModality(Qt::ApplicationModal); // stios the user from interacting with the main window while the popup is open
    popup.setWindowTitle("Add Spell");             // set the title of the popup

    QLabel *popupHeader = new QLabel("Add Spell"); // creates the header text for the popup

    QLabel *bookLabel = new QLabel("Book Abbreviation:");
    QLineEdit *book = new QLineEdit();

    QLabel *pageLabel = new QLabel("Page:");
    QSpinBox *page = new QSpinBox();
    page->setMaximum(999);

    // spell attributes and labels
    QLabel *nameLabel = new QLabel("Name"); // label for textbox
    QLineEdit *spellName = new QLineEdit();

    QLabel *levelLabel = new QLabel("Level");
    QSpinBox *level = new QSpinBox();
    level->setRange(0, 9);

    QLabel *schoolLabel = new QLabel("School of Magic");
    QLineEdit *school = new QLineEdit();

    QLabel *timeLabel = new QLabel("Casting Time");
    QLineEdit *time = new QLineEdit();

    QLabel *rangeLabel = new QLabel("Range");
    QLabel *minRangeLabel = new QLabel("Min");
    QLabel *maxRangeLabel = new QLabel("Max");
    QSpinBox *minRange = new QSpinBox();
    QSpinBox *maxRange = new QSpinBox();
    maxRange->setMaximum(360);

    QLabel *componentsLabel = new QLabel("Components");
    QLabel *verbalLabel = new QLabel("Verbal");
    QLabel *somaticLabel = new QLabel("Somatic");
    QLabel *materialLabel = new QLabel("Material");
    QCheckBox *verbal = new QCheckBox();
    QCheckBox *somatic = new QCheckBox();
    QCheckBox *material = new QCheckBox();

    QLabel *durationLabel = new QLabel("Duration");
    QLineEdit *duration = new QLineEdit();

    QLabel *concentrationLabel = new QLabel("Concentration");
    QCheckBox *concentration = new QCheckBox();

    QLabel *ritualLabel = new QLabel("Ritual");
    QCheckBox *ritual = new QCheckBox();

    QLabel *descriptionLabel = new QLabel("Description");
    QTextEdit *description = new QTextEdit();

    // create the buttons for the popup
    QPushButton *popupConfirm = new QPushButton("Confirm"); // creates the confirm button
    QPushButton *popupCancel = new QPushButton("Cancel");   // creates the cancel button

    // Gets rid of the padding around the text of the buttons
    popupConfirm->setStyleSheet("padding: 5px 0px; margin: 0px;");
    popupCancel->setStyleSheet("padding: 5px 0px; margin: 0px;");

    // Connect the confirm button to accept the dialog (e.g., confirming the deletion)
    QObject::connect(popupConfirm, &QPushButton::clicked, &popup, &QDialog::accept);

    // Connect the cancel button to reject the dialog (e.g., canceling the deletion)
    QObject::connect(popupCancel, &QPushButton::clicked, &popup, &QDialog::reject);

    // create the layout for the popup
    QVBoxLayout *popupLayout = new QVBoxLayout(&popup);

    // create the layout for the header of the popup
    QHBoxLayout *popupHeaderLayout = new QHBoxLayout();

    // creates the layout for the body of the popup
    QGridLayout *popupBodyLayout = new QGridLayout();

    // Create button widget
    QHBoxLayout *popupButtonLayout = new QHBoxLayout();

    // adds the header components to the header layout
    popupHeaderLayout->addWidget(popupHeader);
    popupBodyLayout->addWidget(bookLabel, 0, 0, 1, 1, {Qt::AlignLeft});
    popupBodyLayout->addWidget(book, 0, 1, 1, 1, {Qt::AlignLeft});
    popupBodyLayout->addWidget(pageLabel, 0, 2, 1, 1, {Qt::AlignLeft});
    popupBodyLayout->addWidget(page, 0, 3, 1, 1, {Qt::AlignLeft});

    // adds the attributes to the body layout
    popupBodyLayout->addWidget(nameLabel, 1, 0, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(spellName, 2, 0, 2, 1);

    popupBodyLayout->addWidget(levelLabel, 1, 1, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(level, 2, 1, 2, 1);

    popupBodyLayout->addWidget(schoolLabel, 1, 2, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(school, 2, 2, 2, 1);

    popupBodyLayout->addWidget(timeLabel, 1, 3, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(time, 2, 3, 2, 1);

    popupBodyLayout->addWidget(rangeLabel, 1, 4, 1, 2, {Qt::AlignCenter});
    popupBodyLayout->addWidget(minRangeLabel, 2, 4, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(minRange, 3, 4, 1, 1);
    popupBodyLayout->addWidget(maxRangeLabel, 2, 5, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(maxRange, 3, 5, 1, 1);

    popupBodyLayout->addWidget(componentsLabel, 1, 6, 1, 3, {Qt::AlignCenter});
    popupBodyLayout->addWidget(verbalLabel, 2, 6, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(verbal, 3, 6, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(somaticLabel, 2, 7, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(somatic, 3, 7, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(materialLabel, 2, 8, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(material, 3, 8, 1, 1, {Qt::AlignCenter});

    popupBodyLayout->addWidget(durationLabel, 1, 9, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(duration, 2, 9, 2, 1);

    popupBodyLayout->addWidget(concentrationLabel, 1, 10, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(concentration, 2, 10, 2, 1, {Qt::AlignCenter});

    popupBodyLayout->addWidget(ritualLabel, 1, 11, 1, 1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(ritual, 2, 11, 2, 1, {Qt::AlignCenter});

    popupBodyLayout->addWidget(descriptionLabel, 4, 0, 1, -1, {Qt::AlignCenter});
    popupBodyLayout->addWidget(description, 5, 0, -1, -1);

    // adds the buttons to the button layout
    popupButtonLayout->addWidget(popupConfirm);
    popupButtonLayout->addWidget(popupCancel);

    // adds the components to the popup
    popupLayout->addLayout(popupHeaderLayout);
    popupLayout->addLayout(popupBodyLayout);
    popupLayout->addLayout(popupButtonLayout);

    // makes the popup text take up more horizontal space
    popupLayout->setStretch(0, 1);

    // show the popup
    popup.exec();

    if (popup.result() == QDialog::Accepted)
    {
        // this->spellsList->addItem(spellName->text());
        SpellInfo *info = new SpellInfo;
        info->spellName = spellName->text();
        info->book = book->text();
        info->page = page->value();
        info->level = level->value();
        info->school = school->text();
        info->time = time->text();
        info->minRange = minRange->value();
        info->maxRange = maxRange->value();
        info->verbal = verbal->isChecked();
        info->somatic = somatic->isChecked();
        info->material = material->isChecked();
        info->duration = duration->text();
        info->concentration = concentration->isChecked();
        info->ritual = ritual->isChecked();
        QString desc = description->toPlainText();
        QRegularExpression re("\n");
        desc.replace(re, "<br>");
        info->description = desc;
        // (*this->spells)[spellName->text()] = info;
        this->saveSpell(info);
    }

    // this is so that no spell is selected after creation
    // this->spellsList->selectionModel()->clear();
    // this->removeSpellButton->setEnabled(false);
}

void ViewCharacter::saveSpell(SpellInfo *info)
{
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/spells.csv"; // Creates a file path for the character's spell.csv file

    QFile spellsFile(filePath);

    if (spellsFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&spellsFile);

        out << info->spellName << "," << info->book << "," << info->page << "," << info->level << "," << info->school << "," << info->time << "," << info->maxRange << ",";
        if (info->verbal)
        {
            out << "v";
        }
        if (info->somatic)
        {
            out << "s";
        }
        if (info->material)
        {
            out << "m";
        }
        out << "," << info->duration << "," << info->concentration << "," << info->ritual << ",false," << info->description << Qt::endl;

        spellsFile.close();
    }
}

void ViewCharacter::addExperience()
{
    qDebug() << "Add Experience Button Clicked";

    QDialog popup;
    popup.setWindowModality(Qt::ApplicationModal);
    popup.setWindowTitle("Add XP");

    QVBoxLayout layout(&popup);

    // make quantity label and spinbox
    QLabel *xpLabel = new QLabel("XP Amount:");
    QSpinBox *xpEdit = new QSpinBox();
    xpEdit->setMinimum(experienceTable[characterLevel - 1] - characterExperience);
    xpEdit->setMaximum(experienceTable[20] - characterExperience);
    layout.addWidget(xpLabel);
    layout.addWidget(xpEdit);

    // add and cancel buttons
    QPushButton *addButton = new QPushButton("Add XP");
    QPushButton *cancelButton = new QPushButton("Cancel");

    // make widget for add and cancel buttons
    QWidget buttonWidget;
    buttonWidget.setFixedHeight(50);

    // make layout for add and cancel buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout(&buttonWidget);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(cancelButton);
    layout.addWidget(&buttonWidget, Qt::AlignBottom);

    // connect the buttons
    QObject::connect(addButton, &QPushButton::clicked, &popup, &QDialog::accept);
    QObject::connect(cancelButton, &QPushButton::clicked, &popup, &QDialog::reject);

    // show the popup
    popup.exec();

    // if the user clicks the add button
    if (popup.result() == QDialog::Accepted)
    {

        qDebug() << "Add XP";

        // get the xp amount
        int xp = xpEdit->value();
        qDebug() << xp;

        // update experience and save to csv file
        characterExperience += xp;
        qDebug() << characterExperience;
        saveCharacterStatsAndFeats();
    }
}

void ViewCharacter::saveCharacterStatsAndFeats()
{
    // Saves experience to the character's character.csv file
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/character.csv"; // Creates a file path for the character's character.csv file
    QFile file(filePath);                                                                            // Creates a file object for the character's character.csv file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open character file for saving experience:" << filePath; // Outputs a warning if the file fails to open
        return;
    }

    QTextStream in(&file); // Create a text stream to read the file
    QStringList lines;     // Create a list of strings to store the lines of the file
    while (!in.atEnd())
    {
        lines.append(in.readLine()); // Read each line of the file
    }

    // Create string for the character stats
    QString characterStats = characterName + ",";

    for (int i = 0; i < 6; i++)
    {
        characterStats += QString::number(characterAbilities[i]) + ",";
    }

    characterStats += QString::number(characterLevel) + ":" + QString::number(characterExperience) + "," +
                      QString::number(characterMaxHitPoints) + ":" + QString::number(characterHitPoints) + ":" + QString::number(characterTempHitPoints) + "," +
                      characterClass + "," +
                      characterSubclass + "," +
                      characterRace + "," +
                      characterSubrace;

    lines[0] = characterStats; // Update the character stats line

    file.close(); // Close the file

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open character file for saving experience:" << filePath;
        return;
    }

    QTextStream out(&file); // Create a text stream to write to the file

    for (const QString &line : lines)
    {
        out << line << "\n"; // Write each line to the file
    }

    file.close(); // Close the file
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
