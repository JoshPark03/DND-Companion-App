/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/25/2024
*/

#include "addCharacter.h"
#include "characterSelect.h"

#include <iostream>

#include <QStackedWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>

AddCharacter::AddCharacter(QWidget * parent) :
	QStackedWidget(parent)
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

StartWidget::StartWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	name = new QLineEdit();
	layout->addWidget(name, 0, 0, {Qt::AlignCenter, Qt::AlignBottom});

	QSpacerItem * spacer = new QSpacerItem(50, 50);
	layout->addItem(spacer, 1, 0);

	QPushButton * start = new QPushButton("Start Creation");
	layout->addWidget(start, 2, 0, {Qt::AlignCenter, Qt::AlignTop});

	connect(start, &QPushButton::clicked, [=]{
		QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
		stackedWidget->setCurrentIndex(1);
	});
}

StartWidget::~StartWidget() {
	delete this->name;
}

BaseStatsWidget::BaseStatsWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	QLabel * strengthLabel = new QLabel("Strength:\n");
	QLabel * dexterityLabel = new QLabel("Dexterity:\n");
	QLabel * constitutionLabel = new QLabel("Constitution:\n");
	QLabel * intelligenceLabel = new QLabel("Intelligence:\n");
	QLabel * wisdomLabel = new QLabel("Wisdom:\n");
	QLabel * charismaLabel = new QLabel("Charisma:\n");
	layout->addWidget(strengthLabel, 0, 0, {Qt::AlignRight, Qt::AlignBottom});
	layout->addWidget(dexterityLabel, 1, 0, {Qt::AlignRight, Qt::AlignBottom});
	layout->addWidget(constitutionLabel, 2, 0, {Qt::AlignRight, Qt::AlignBottom});
	layout->addWidget(intelligenceLabel, 3, 0, {Qt::AlignRight, Qt::AlignBottom});
	layout->addWidget(wisdomLabel, 4, 0, {Qt::AlignRight, Qt::AlignBottom});
	layout->addWidget(charismaLabel, 5, 0, {Qt::AlignRight, Qt::AlignBottom});

	QLineEdit * strengthVal = new QLineEdit;
	QLineEdit * dexterityVal = new QLineEdit;
	QLineEdit * constitutionVal = new QLineEdit;
	QLineEdit * intelligenceVal = new QLineEdit;
	QLineEdit * wisdomVal = new QLineEdit;
	QLineEdit * charismaVal = new QLineEdit;
	layout->addWidget(strengthVal, 0, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(dexterityVal, 1, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(constitutionVal, 2, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(intelligenceVal, 3, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(wisdomVal, 4, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(charismaVal, 5, 1, {Qt::AlignLeft, Qt::AlignBottom});
	
	QPushButton * confirm = new QPushButton("Confirm Stats");
	layout->addWidget(confirm, 6, 0, 1, 2, Qt::AlignCenter);

	connect(confirm, &QPushButton::clicked, [=]{
		QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
		stackedWidget->setCurrentIndex(2);
	});
}

ClassWidget::ClassWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	QRadioButton * barbarian = new QRadioButton();
	QRadioButton * bard = new QRadioButton();
	QRadioButton * cleric = new QRadioButton();
	QRadioButton * druid = new QRadioButton();
	QRadioButton * fighter = new QRadioButton();
	QRadioButton * monk = new QRadioButton();
	QRadioButton * paladin = new QRadioButton();
	QRadioButton * ranger = new QRadioButton();
	QRadioButton * rogue = new QRadioButton();
	QRadioButton * sorcerer = new QRadioButton();
	QRadioButton * warlock = new QRadioButton();
	QRadioButton * wizard = new QRadioButton();

	layout->addWidget(barbarian, 0, 0, Qt::AlignCenter);
	layout->addWidget(bard, 0, 1, Qt::AlignCenter);
	layout->addWidget(cleric, 0, 2, Qt::AlignCenter);
	layout->addWidget(druid, 1, 0, Qt::AlignCenter);
	layout->addWidget(fighter, 1, 1, Qt::AlignCenter);
	layout->addWidget(monk, 1, 2, Qt::AlignCenter);
	layout->addWidget(paladin, 2, 0, Qt::AlignCenter);
	layout->addWidget(ranger, 2, 1, Qt::AlignCenter);
	layout->addWidget(rogue, 2, 1, Qt::AlignCenter);
	layout->addWidget(sorcerer, 2, 2, Qt::AlignCenter);
	layout->addWidget(warlock, 3, 0, Qt::AlignCenter);
	layout->addWidget(wizard, 3, 1, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Class");
	layout->addWidget(confirm, 4, 0, 1, 3, Qt::AlignCenter);

	connect(confirm, &QPushButton::clicked, [=]{
		QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
		stackedWidget->setCurrentIndex(3);
	});
}

RaceWidget::RaceWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);
	
	QRadioButton * dwarf = new QRadioButton();
	QRadioButton * elf = new QRadioButton();
	QRadioButton * halfling = new QRadioButton();
	QRadioButton * human = new QRadioButton();
	QRadioButton * dragonborn = new QRadioButton();
	QRadioButton * gnome = new QRadioButton();
	QRadioButton * half_elf = new QRadioButton();
	QRadioButton * half_orc = new QRadioButton();
	QRadioButton * tiefling = new QRadioButton();

	layout->addWidget(dwarf, 0, 0, Qt::AlignCenter);
	layout->addWidget(elf, 0, 1, Qt::AlignCenter);
	layout->addWidget(halfling, 0, 2, Qt::AlignCenter);
	layout->addWidget(human, 1, 0, Qt::AlignCenter);
	layout->addWidget(dragonborn, 1, 1, Qt::AlignCenter);
	layout->addWidget(gnome, 1, 2, Qt::AlignCenter);
	layout->addWidget(half_elf, 2, 0, Qt::AlignCenter);
	layout->addWidget(half_orc, 2, 1, Qt::AlignCenter);
	layout->addWidget(tiefling, 2, 1, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Race");
	layout->addWidget(confirm, 3, 0, 1, 3, Qt::AlignCenter);

	connect(confirm, &QPushButton::clicked, [=]{
		QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
		stackedWidget->setCurrentIndex(4);
	});
}

BackgroundWidget::BackgroundWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	QRadioButton * acolyte = new QRadioButton();
	QRadioButton * charlatan = new QRadioButton();
	QRadioButton * criminal = new QRadioButton();
	QRadioButton * entertainer = new QRadioButton();
	QRadioButton * folk_hero = new QRadioButton();
	QRadioButton * guild_artisan = new QRadioButton();
	QRadioButton * hermit = new QRadioButton();
	QRadioButton * noble = new QRadioButton();
	QRadioButton * outlander = new QRadioButton();
	QRadioButton * sage = new QRadioButton();
	QRadioButton * sailor = new QRadioButton();
	QRadioButton * soldier = new QRadioButton();
	QRadioButton * urchin = new QRadioButton();

	layout->addWidget(acolyte, 0, 0, Qt::AlignCenter);
	layout->addWidget(charlatan, 0, 1, Qt::AlignCenter);
	layout->addWidget(criminal, 0, 2, Qt::AlignCenter);
	layout->addWidget(entertainer, 1, 0, Qt::AlignCenter);
	layout->addWidget(folk_hero, 1, 1, Qt::AlignCenter);
	layout->addWidget(guild_artisan, 1, 2, Qt::AlignCenter);
	layout->addWidget(hermit, 2, 0, Qt::AlignCenter);
	layout->addWidget(noble, 2, 1, Qt::AlignCenter);
	layout->addWidget(outlander, 2, 2, Qt::AlignCenter);
	layout->addWidget(sage, 3, 0, Qt::AlignCenter);
	layout->addWidget(sailor, 3, 1, Qt::AlignCenter);
	layout->addWidget(soldier, 4, 0, Qt::AlignCenter);
	layout->addWidget(urchin, 4, 1, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Background");
	layout->addWidget(confirm, 5, 0, 1, 3, Qt::AlignCenter);

	connect(confirm, &QPushButton::clicked, [=]{
		QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
		stackedWidget->setCurrentIndex(5);
	});
}

InventoryWidget::InventoryWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	QSpacerItem * spacer = new QSpacerItem(100, 100);
	layout->addItem(spacer, 0, 3);
	layout->addItem(spacer, 3, 3);

	QListWidget * items = new QListWidget();
	layout->addWidget(items, 2, 2, 1, 3);

	QPushButton * addItem = new QPushButton("Add Item");
	layout->addWidget(addItem, 1, 1, 1, 3, Qt::AlignCenter);

	QPushButton * removeItem = new QPushButton("Remove Selected Item");
	layout->addWidget(removeItem, 1, 3, 1, 3, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Inventory");
	layout->addWidget(confirm, 3, 3, Qt::AlignCenter);

	connect(confirm, &QPushButton::clicked, [=]{
		AddCharacter * addCharacterWidget = qobject_cast<AddCharacter *>(this->parentWidget());
		addCharacterWidget->setCurrentIndex(0);
		QStackedWidget * mainStackedWidget = qobject_cast<QStackedWidget *>(addCharacterWidget->parentWidget());
		mainStackedWidget->setCurrentIndex(0);
		// Need to somehow get access to the SelectCharacterWidget to call addCharacter(QString name)
		// Alternatively we could move addCharacter to this file and have the QListWidget characters auto-upadte
	});
}

AddCharacter::~AddCharacter() {
	delete this->startWidget;
	delete this->baseStatsWidget;
	delete this->classWidget;
	delete this->raceWidget;
	delete this->backgroundWidget;
	delete this->inventoryWidget;
}