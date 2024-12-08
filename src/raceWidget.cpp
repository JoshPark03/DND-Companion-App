/*
Name: raceWidget.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Chanu Lee
Other Sources: ...
Date Created: 11/23/2024
Last Modified: 11/23/2024
*/

#include "addCharacter.h"

#include <iostream>

#include <QDir>
#include <QFile>
#include <QLayout>
#include <QPushButton>
/**
 * Constructor for the class
 */

RaceWidget::RaceWidget(QWidget *parent) : QWidget(parent)
{
    // Create the main layout
    QGridLayout *mainLayout = new QGridLayout(this);

    // Create and configure layout for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40);

    // Create navigation buttons
    QPushButton *backButton = new QPushButton("Back");
    QPushButton *nextButton = new QPushButton("Next");

    // Create the race combo box
    raceComboBox = new UpComboBox;

    // Add the combo box and navigation buttons to the navbar
    navbarLayout->addWidget(backButton);
    navbarLayout->addWidget(this->raceComboBox);
    navbarLayout->addWidget(nextButton);

    // Create header
    header = new QLabel("<h1>" + this->raceComboBox->currentText() + "</h1>");
    header->setFixedHeight(50);

    // Create portrait
    this->portrait = new Portrait("races", this->raceComboBox->currentText());

    // Create the description layout
    QGridLayout *description = new QGridLayout();

    // Create the summary textbox
    this->summary = new QLabel("summary");
    summary->setWordWrap(true);

    // Create the subrace layout, combobox, and label
    this->subRaceLayout = new QVBoxLayout();
    this->subRaceComboBox = new UpComboBox;
    this->subRace = new QLabel("<h3>SubRaces:</h3>");

    subRaceLayout->addWidget(subRace, {Qt::AlignTop});
    subRaceLayout->addWidget(subRaceComboBox, {Qt::AlignTop});

    // Create the proficiencies layout
    QVBoxLayout *proficienciesLayout = new QVBoxLayout();
    QLabel *proficiencies = new QLabel("<h3>Proficiencies:</h3>");
    this->abilityScoreIncrease = new QLabel("ability score increase");
    this->size = new QLabel("size");
    this->speed = new QLabel("speed");
    this->languages = new QLabel("languages");
    this->abilities = new QLabel("abilities");

    // Setting the proficiencies layout
    proficienciesLayout->addWidget(proficiencies, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
    proficienciesLayout->addWidget(abilityScoreIncrease, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
    proficienciesLayout->addWidget(size, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
    proficienciesLayout->addWidget(speed, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
    proficienciesLayout->addWidget(languages, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
    proficienciesLayout->addWidget(abilities, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});

    // Setting the description layout
    description->addWidget(summary, 0, 0, -1, 1, {Qt::AlignTop});
    description->addLayout(subRaceLayout, 0, 1, 1, 1, {Qt::AlignTop});
    description->addLayout(proficienciesLayout, 1, 1, 2, 1, {Qt::AlignTop});

    // Add the all pieces to the main layout
    mainLayout->addWidget(header, 0, 0, 1, -1, {Qt::AlignCenter});
    mainLayout->addWidget(portrait, 1, 0, 1, 1, {Qt::AlignCenter});
    mainLayout->addLayout(description, 1, 1, 1, 4);
    mainLayout->addWidget(navbar, 2, 0, 1, -1, {Qt::AlignBottom});

    // loading the races from the tsv file
    this->loadRaces();

    // Initialize with the first race's details
    if (!raceComboBox->currentText().isEmpty())
    {
        this->updateRaceInfo(raceComboBox->currentText());
        this->updateSubRaceInfo(subRaceComboBox->currentText());
    }

    // When backButton is clicked it calls the private SLOT function backPage()
    connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
    // When nextButton is clicked it calls the private SLOT function nextPage()
    connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
    // When raceComboBox is changed it calls the private SLOT function updateRaceInfo()
    connect(this->raceComboBox, SIGNAL(currentTextChanged(const QString &)), SLOT(updateRaceInfo(const QString &)));
    // When subRaceComboBox is changed it calls the private SLOT function updateRaceInfo()
    connect(this->subRaceComboBox, SIGNAL(currentTextChanged(const QString &)), SLOT(updateSubRaceInfo(const QString &)));
}

/**
 * This function changes AddCharacter's StackedWidget to ClassWidget
 */
void RaceWidget::backPage()
{
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (!stackedWidget)
    {
        qDebug() << "Failed to change page because stackedWidget was not correctly casted";
        return;
    }
    stackedWidget->setCurrentIndex(2);
}

/**
 * This function changes AddCharacter's StackedWidget to BackgroundWidget
 */
void RaceWidget::nextPage()
{
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (stackedWidget)
    {
        stackedWidget->setCurrentIndex(4);
    }
    qDebug() << "Race: " << this->getRace();
    qDebug() << "SubRace: " << this->getSubRace();
    qDebug() << "Ability Score Increase: " << this->getAbilityScoreIncrease();
    qDebug() << "Size: " << this->getSize();
    qDebug() << "Speed: " << this->getSpeed();
    qDebug() << "Languages: " << this->getLanguages();
    qDebug() << "Abilities: " << *this->getAbilities() << Qt::endl;
}

void RaceWidget::loadRaces()
{
    QFile file("data/databases/Races.tsv"); // read in the file
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
        if (fields.size() < 10)
        {
            continue; // ensure we get all the fields
        }

        QList<QString> *abilities = new QList<QString>(fields[9].split(", "));

        QString name = fields[0];
        QString subRaceName = fields[4];

        // See whether the race has subraces or not
        bool subRacesExist = subRaceName.isEmpty() ? false : true;

        if (!this->races.contains(name))
        {
            // Create new entry in the list of races if it does not yet exist
            QMap<QString, SubRaceInfo *> subRaces;
            // Fill out info
            RaceInfo *info = new RaceInfo{
                fields[1],
                fields[2],
                fields[3],
                subRacesExist,
                subRaces};

            // Add the race to the combobox
            this->raceComboBox->addItem(name);
            this->races[name] = info;
        }

        // List of subraces associated with a race
        QMap<QString, SubRaceInfo *> subRaces = this->races[name]->subRaces;
        // Fill out info
        SubRaceInfo *subRaceInfo = new SubRaceInfo{
            fields[5],
            fields[6],
            fields[7],
            fields[8],
            abilities};

        if (subRacesExist)
        {
            // If the race has subraces, create a new entry for the subrace in the list of subraces
            this->races[name]->subRaces[subRaceName] = subRaceInfo;
        }
        else
        {
            // If not, then create entry in the list of subraces for info associated with the race
            // under the race's name
            this->races[name]->subRaces[name] = subRaceInfo;
        }
    }
}

/**
 * This function updates all information in RaceWidget to match what race is selected
 */
void RaceWidget::updateRaceInfo(const QString &name)
{
    if (!this->races.contains(name))
    {
        qWarning() << "Race not found:" << name;
        return;
    }

    RaceInfo *info = this->races[name];

    this->header->setText("<h1>" + name + " " + info->book + " pg " + info->page + "</h1>");

    this->portrait->getImage(name);

    this->summary->setText("<h3>Summary:</h3><br>" + info->summary);

    this->subRaceComboBox->clear();

    // Update subrace combobox if subraces exist
    if (info->subRacesExist)
    {
        this->subRaceComboBox->setEnabled(true);
        QMap<QString, SubRaceInfo *> subRaces = info->subRaces;
        for (auto item = subRaces.cbegin(); item != subRaces.cend(); ++item)
        {
            this->subRaceComboBox->addItem(item.key());
        }
    }
    else
    {
        this->subRaceComboBox->addItem("None");
        this->subRaceComboBox->setEnabled(false);
    }
}

// Update info on the page associated with subraces
void RaceWidget::updateSubRaceInfo(const QString &subRaceName)
{
    if (subRaceName.isEmpty())
    {
        return;
    }

    QString raceName = this->raceComboBox->currentText();
    SubRaceInfo *subRaceInfo;
    if (!this->races[raceName]->subRacesExist)
    {
        subRaceInfo = this->races[raceName]->subRaces[raceName];
    }
    else
    {
        subRaceInfo = this->races[raceName]->subRaces[subRaceName];
    }

    this->abilityScoreIncrease->setText("<h4>Ability Score Increase (Str, Dex, Con, Int, Wis, Cha):</h4><br>" + subRaceInfo->abilityScoreIncrease);

    this->size->setText("<h4>Size:</h4><br>" + subRaceInfo->size);

    this->speed->setText("<h4>Speed:</h4><br>" + subRaceInfo->speed);

    this->languages->setText("<h4>Languages:</h4><br>" + subRaceInfo->languages);

    this->abilities->setText("<h4>Abilities:</h4>");
    for (QString ability : *subRaceInfo->abilities)
    {
        this->abilities->setText(this->abilities->text() + "<br>" + ability);
    }
}

/**
 * This function returns the race chosen
 */
QString RaceWidget::getRace()
{
    return this->raceComboBox->currentText();
}

/**
 * This function returns the subrace chosen
 */
QString RaceWidget::getSubRace()
{
    return this->subRaceComboBox->currentText();
}

/**
 * This function returns the ability score increase from chosen race/subrace
 */
QString RaceWidget::getAbilityScoreIncrease()
{
    QString raceName = this->raceComboBox->currentText();
    if (this->races[raceName]->subRacesExist)
    {
        QString subRaceName = this->subRaceComboBox->currentText();
        return this->races[raceName]->subRaces[subRaceName]->abilityScoreIncrease;
    }
    else
    {
        return this->races[raceName]->subRaces[raceName]->abilityScoreIncrease;
    }
}

/**
 * This function returns the size from chosen race/subrace
 */
QString RaceWidget::getSize()
{
    QString raceName = this->raceComboBox->currentText();
    if (this->races[raceName]->subRacesExist)
    {
        QString subRaceName = this->subRaceComboBox->currentText();
        return this->races[raceName]->subRaces[subRaceName]->size;
    }
    else
    {
        return this->races[raceName]->subRaces[raceName]->size;
    }
}

/**
 * This function returns the speed from chosen race/subrace
 */
QString RaceWidget::getSpeed()
{
    QString raceName = this->raceComboBox->currentText();
    if (this->races[raceName]->subRacesExist)
    {
        QString subRaceName = this->subRaceComboBox->currentText();
        return this->races[raceName]->subRaces[subRaceName]->speed;
    }
    else
    {
        return this->races[raceName]->subRaces[raceName]->speed;
    }
}

/**
 * This function returns the languages from chosen race/subrace
 */
QString RaceWidget::getLanguages()
{
    QString raceName = this->raceComboBox->currentText();
    if (this->races[raceName]->subRacesExist)
    {
        QString subRaceName = this->subRaceComboBox->currentText();
        return this->races[raceName]->subRaces[subRaceName]->languages;
    }
    else
    {
        return this->races[raceName]->subRaces[raceName]->languages;
    }
}

/**
 * This function returns the abilities from chosen race/subrace
 */
QList<QString> *RaceWidget::getAbilities()
{
    QString raceName = this->raceComboBox->currentText();
    if (this->races[raceName]->subRacesExist)
    {
        QString subRaceName = this->subRaceComboBox->currentText();
        return this->races[raceName]->subRaces[subRaceName]->abilities;
    }
    else
    {
        return this->races[raceName]->subRaces[raceName]->abilities;
    }
}
