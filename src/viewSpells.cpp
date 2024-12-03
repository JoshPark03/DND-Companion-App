/*
Name: viewSpells.cpp
Description: Page to view a character's spells.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 11/5/2024
Last Modified: 12/3/2024
*/

#include "viewSpells.h"
#include "themeUtils.h"
#include "centeredCheckBox.h"

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
#include <QCheckBox>

void ViewSpells::loadSpells()
{
    this->spells->clear();
    QStringList colNames = {"Name", "Book", "Page", "Level", "School", "Casting Time", "Range", "Verbal", "Somatic", "Material", "Duration", "Concentration", "Ritual", "Prepared"};
    this->spells->setHorizontalHeaderLabels(colNames);

	// Path to the characters directory
	QString charPath = QDir::currentPath() + "/data/characters/" + this->name;

    QFile charSpells(charPath + "/spells.csv");

    if (charSpells.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&charSpells);

        while (!in.atEnd()) {
            QString line = in.readLine();
            this->addItem(line);
        }
        this->spells->resizeColumnsToContents();
        this->spells->sortByColumn(3, Qt::AscendingOrder);
    }
}

void ViewSpells::addItem(QString line) {
    QStringList values = line.split(",");
    QTableWidgetItem * name = new QTableWidgetItem(values[0]);
    name->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * book = new QTableWidgetItem(values[1]);
    book->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * page = new QTableWidgetItem(values[2]);
    page->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * level = new QTableWidgetItem(values[3]);
    level->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * school = new QTableWidgetItem(values[4]);
    school->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * time = new QTableWidgetItem(values[5]);
    time->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * range = new QTableWidgetItem(values[6]);
    range->setTextAlignment(Qt::AlignCenter);
    CenteredCheckBox * verbal = new CenteredCheckBox();
    CenteredCheckBox * somatic = new CenteredCheckBox();
    CenteredCheckBox * material = new CenteredCheckBox();
    if (values[7].contains("v")) {
        verbal->setChecked(true);
    }
    if (values[7].contains("s")) {
        somatic->setChecked(true);
    }
    if (values[7].contains("m")) {
        material->setChecked(true);
    }
    verbal->setEnabled(false);
    somatic->setEnabled(false);
    material->setEnabled(false);
    QTableWidgetItem * duration = new QTableWidgetItem(values[8]);
    duration->setTextAlignment(Qt::AlignCenter);
    CenteredCheckBox * concentration = new CenteredCheckBox();
    if (values[9] == "true") {
        concentration->setChecked(true);
    }
    concentration->setEnabled(false);
    CenteredCheckBox * ritual = new CenteredCheckBox();
    if (values[10] == "true") {
        ritual->setChecked(true);
    }
    ritual->setEnabled(false);
    CenteredCheckBox * prepared = new CenteredCheckBox();
    if (values[11] == "true") {
        prepared->setChecked(true);
    }
    // QTableWidgetItem * description = new QTableWidgetItem(values[11]);
    // description as part of the QTableWidget would take too much space

    int row = this->spells->rowCount();
    this->spells->insertRow(row);
    this->spells->setItem(row, 0, name);
    this->spells->setItem(row, 1, book);
    this->spells->setItem(row, 2, page);
    this->spells->setItem(row, 3, level);
    this->spells->setItem(row, 4, school);
    this->spells->setItem(row, 5, time);
    this->spells->setItem(row, 6, range);
    this->spells->setCellWidget(row, 7, verbal->getLayoutWidget());
    this->spells->setCellWidget(row, 8, somatic->getLayoutWidget());
    this->spells->setCellWidget(row, 9, material->getLayoutWidget());
    this->spells->setItem(row, 10, duration);
    this->spells->setCellWidget(row, 11, concentration->getLayoutWidget());
    this->spells->setCellWidget(row, 12, ritual->getLayoutWidget());
    this->spells->setCellWidget(row, 13, prepared->getLayoutWidget());
}


ViewSpells::ViewSpells(QWidget *parent, QString nameIn) :
    QWidget(parent), name(nameIn)
{
    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesnt take up half the screen

    // Create buttons to go back to the character page
    QPushButton *backToCharacter = new QPushButton("Return to Character");
    // Create header label with character's name
    QLabel *spellsLabel = new QLabel(name + "'s Spells");

    // Add buttons to navbar
    navbarLayout->addWidget(backToCharacter);
    navbarLayout->addWidget(spellsLabel, 1, Qt::AlignCenter);

    
    // Create the layout for the spells page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create body widget
    QWidget *body = new QWidget();
    QVBoxLayout *bodyLayout = new QVBoxLayout(body);

    this->spells = new QTableWidget();
    this->spells->setColumnCount(14);
    bodyLayout->addWidget(this->spells);

    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));

    this->loadSpells();

    reloadTheme(); // Reload the theme after everything is placed
}

void ViewSpells::goBack()
{
    QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    
    if (mainStackedWidget)
    {
        mainStackedWidget->setCurrentIndex(0); // Switch to CharacterSelect (index 0)
    }
}

ViewSpells::~ViewSpells()
{
}