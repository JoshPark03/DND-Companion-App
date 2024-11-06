/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/31/2024
*/

#include "addCharacter.h"
#include "characterSelect.h"

#include <iostream>

#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>

AddCharacter::AddCharacter(QWidget *parent) : QStackedWidget(parent)
{
	startWidget = new StartWidget();
	baseStatsWidget = new BaseStatsWidget();
	classWidget = new ClassWidget();
	raceWidget = new RaceWidget();
	backgroundWidget = new BackgroundWidget();
	inventoryWidget = new InventoryWidget();

	this->addWidget(startWidget);
	this->addWidget(baseStatsWidget);
	this->addWidget(classWidget);
	this->addWidget(raceWidget);
	this->addWidget(backgroundWidget);
	this->addWidget(inventoryWidget);

	this->setCurrentWidget(startWidget);
}

StartWidget::StartWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create the form layout
	QWidget *form = new QWidget();
	QFormLayout *formLayout = new QFormLayout(form);
	formLayout->setFormAlignment(Qt::AlignCenter);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create text box for the character name
	name = new QLineEdit();
	name->setFixedWidth(100);

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back to Character Select");
	QPushButton *nextButton = new QPushButton("Next");

	// Add character name input to the form
	formLayout->addRow("Character Name:", name);

	// Add buttons to navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(nextButton);

	// Add the navbar and form to the main layout
	layout->addWidget(form);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void StartWidget::backPage() {
	QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget()->parentWidget());
	if (mainStackedWidget) {
		mainStackedWidget->setCurrentIndex(0);
	}
}

void StartWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(1);
	}
}

BaseStatsWidget::BaseStatsWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create the form layout
	QWidget *form = new QWidget();
	QFormLayout *formLayout = new QFormLayout(form);
	formLayout->setFormAlignment(Qt::AlignCenter);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create the stats spin boxes
	strengthVal = new QSpinBox;
	strengthVal->setRange(3, 18);
	strengthVal->setFixedWidth(50);

	dexterityVal = new QSpinBox;
	dexterityVal->setRange(3, 18);
	dexterityVal->setFixedWidth(50);

	constitutionVal = new QSpinBox;
	constitutionVal->setRange(3, 18);
	constitutionVal->setFixedWidth(50);

	intelligenceVal = new QSpinBox;
	intelligenceVal->setRange(3, 18);
	intelligenceVal->setFixedWidth(50);

	wisdomVal = new QSpinBox;
	wisdomVal->setRange(3, 18);
	wisdomVal->setFixedWidth(50);

	charismaVal = new QSpinBox;
	charismaVal->setRange(3, 18);
	charismaVal->setFixedWidth(50);

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add spinboxes to the form
	formLayout->addRow("Strength:", strengthVal);
	formLayout->addRow("Dexterity:", dexterityVal);
	formLayout->addRow("Constitution:", constitutionVal);
	formLayout->addRow("Intelligence:", intelligenceVal);
	formLayout->addRow("Wisdom:", wisdomVal);
	formLayout->addRow("Charisma:", charismaVal);

	// Add buttons to navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(nextButton);

	// Add the navbar and form to the main layout
	layout->addWidget(form);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void BaseStatsWidget::backPage() {
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(0);
	}
}

void BaseStatsWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}
ClassWidget::ClassWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create horizontal layer for the columns
	QWidget *body = new QWidget();
	QHBoxLayout *bodyLayout = new QHBoxLayout(body);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create the class combo box
	QComboBox *classComboBox = new QComboBox;
	classComboBox->addItem("Barbarian");
	classComboBox->addItem("Bard");
	classComboBox->addItem("Cleric");
	classComboBox->addItem("Druid");
	classComboBox->addItem("Fighter");
	classComboBox->addItem("Monk");
	classComboBox->addItem("Paladin");
	classComboBox->addItem("Ranger");
	classComboBox->addItem("Rogue");
	classComboBox->addItem("Sorcerer");
	classComboBox->addItem("Warlock");
	classComboBox->addItem("Wizard");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(classComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the navbar to the main layout
	layout->addWidget(body);
	layout->addWidget(navbar);

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void ClassWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3);
	}
}

RaceWidget::RaceWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create horizontal layer for the columns
	QWidget *body = new QWidget();
	QHBoxLayout *bodyLayout = new QHBoxLayout(body);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create the race combo box
	QComboBox *raceComboBox = new QComboBox;
	raceComboBox->addItem("Dwarf");
	raceComboBox->addItem("Elf");
	raceComboBox->addItem("Halfling");
	raceComboBox->addItem("Human");
	raceComboBox->addItem("Dragonborn");
	raceComboBox->addItem("Gnome");
	raceComboBox->addItem("Half Elf");
	raceComboBox->addItem("Half Orc");
	raceComboBox->addItem("Tiefling");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(raceComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the navbar to the main layout
	layout->addWidget(body);
	layout->addWidget(navbar);

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void RaceWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create horizontal layer for the columns
	QWidget *body = new QWidget();
	QHBoxLayout *bodyLayout = new QHBoxLayout(body);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create combo box for the background
	QComboBox *backgroundComboBox = new QComboBox;
	backgroundComboBox->addItem("Acolyte");
	backgroundComboBox->addItem("Charlatan");
	backgroundComboBox->addItem("Criminal");
	backgroundComboBox->addItem("Entertainer");
	backgroundComboBox->addItem("Folk Hero");
	backgroundComboBox->addItem("Guild Artisan");
	backgroundComboBox->addItem("Hermit");
	backgroundComboBox->addItem("Noble");
	backgroundComboBox->addItem("Outlander");
	backgroundComboBox->addItem("Sage");
	backgroundComboBox->addItem("Sailor");
	backgroundComboBox->addItem("Soldier");
	backgroundComboBox->addItem("Urchin");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(backgroundComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the body and navbar to the main layout
	layout->addWidget(body);
	layout->addWidget(navbar);

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void BackgroundWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(5);
	}
}

InventoryWidget::InventoryWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create the layout of the inventory
	QWidget *inventory = new QWidget();
	QGridLayout *inventoryLayout = new QGridLayout(inventory);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create inventory elements
	QSpacerItem *spacer = new QSpacerItem(100, 100);
	QListWidget *items = new QListWidget();
	QPushButton *addItem = new QPushButton("Add Item");
	QPushButton *removeItem = new QPushButton("Remove Selected Item");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *finishButton = new QPushButton("Finish");

	// Add inventory elements to the body
	inventoryLayout->addItem(spacer, 0, 3);
	inventoryLayout->addItem(spacer, 3, 3);
	inventoryLayout->addWidget(items, 2, 2, 1, 3);
	inventoryLayout->addWidget(addItem, 1, 1, 1, 3, Qt::AlignCenter);
	inventoryLayout->addWidget(removeItem, 1, 3, 1, 3, Qt::AlignCenter);

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(finishButton);

	// Add the body and navbar to the main layout
	layout->addWidget(inventory);
	layout->addWidget(navbar);

	connect(finishButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void InventoryWidget::nextPage()
{
	AddCharacter *addCharacterWidget = qobject_cast<AddCharacter *>(this->parentWidget());
	if (addCharacterWidget)
	{
		addCharacterWidget->setCurrentIndex(0);
	}
	QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(addCharacterWidget->parentWidget());
	if (mainStackedWidget)
	{
		mainStackedWidget->setCurrentIndex(0);
	}
	// Need to somehow get access to the SelectCharacterWidget to call addCharacter(QString name)
	// Alternatively we could move addCharacter to this file and have the QListWidget characters auto-upadte
}
