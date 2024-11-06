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

// AddCharacter::~AddCharacter() {
// 	delete this->startWidget;
// 	delete this->baseStatsWidget;
// 	delete this->classWidget;
// 	delete this->raceWidget;
// 	delete this->backgroundWidget;
// 	delete this->inventoryWidget;
// }

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

	// connect(start, &QPushButton::clicked, [=]{
	// 	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	// 	stackedWidget->setCurrentIndex(1);
	// });

	connect(start, SIGNAL (clicked()),  SLOT (nextPage()));
}

void StartWidget::nextPage() {
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget) {
		stackedWidget->setCurrentIndex(1);
	}
}

// StartWidget::~StartWidget() {
// 	delete this->name;
// }

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

	strengthVal = new QSpinBox;
	strengthVal->setRange(3, 18);
	dexterityVal = new QSpinBox;
	dexterityVal->setRange(3, 18);
	constitutionVal = new QSpinBox;
	constitutionVal->setRange(3, 18);
	intelligenceVal = new QSpinBox;
	intelligenceVal->setRange(3, 18);
	wisdomVal = new QSpinBox;
	wisdomVal->setRange(3, 18);
	charismaVal = new QSpinBox;
	charismaVal->setRange(3, 18);
	layout->addWidget(strengthVal, 0, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(dexterityVal, 1, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(constitutionVal, 2, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(intelligenceVal, 3, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(wisdomVal, 4, 1, {Qt::AlignLeft, Qt::AlignBottom});
	layout->addWidget(charismaVal, 5, 1, {Qt::AlignLeft, Qt::AlignBottom});
	
	QPushButton * confirm = new QPushButton("Confirm Stats");
	layout->addWidget(confirm, 6, 0, 1, 2, Qt::AlignCenter);

	// connect(confirm, &QPushButton::clicked, [=]{
	// 	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	// 	stackedWidget->setCurrentIndex(2);
	// });

	connect(confirm, SIGNAL (clicked()), SLOT (nextPage()));
}

void BaseStatsWidget::nextPage() {
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget) {
		stackedWidget->setCurrentIndex(2);
	}
}

// BaseStatsWidget::~BaseStatsWidget() {
// 	delete this->strengthVal;
// 	delete this->dexterityVal;
// 	delete this->constitutionVal;
// 	delete this->intelligenceVal;
// 	delete this->wisdomVal;
// 	delete this->charismaVal;
// }

ClassWidget::ClassWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	barbarian = new QRadioButton("Barbarian");
	bard = new QRadioButton("Bard");
	cleric = new QRadioButton("Cleric");
	druid = new QRadioButton("Druid");
	fighter = new QRadioButton("Fighter");
	monk = new QRadioButton("Monk");
	paladin = new QRadioButton("Paladin");
	ranger = new QRadioButton("Ranger");
	rogue = new QRadioButton("Rogue");
	sorcerer = new QRadioButton("Sorcerer");
	warlock = new QRadioButton("Warlock");
	wizard = new QRadioButton("Wizard");

	layout->addWidget(barbarian, 0, 0, Qt::AlignCenter);
	layout->addWidget(bard, 0, 1, Qt::AlignCenter);
	layout->addWidget(cleric, 0, 2, Qt::AlignCenter);
	layout->addWidget(druid, 1, 0, Qt::AlignCenter);
	layout->addWidget(fighter, 1, 1, Qt::AlignCenter);
	layout->addWidget(monk, 1, 2, Qt::AlignCenter);
	layout->addWidget(paladin, 2, 0, Qt::AlignCenter);
	layout->addWidget(ranger, 2, 1, Qt::AlignCenter);
	layout->addWidget(rogue, 2, 2, Qt::AlignCenter);
	layout->addWidget(sorcerer, 3, 0, Qt::AlignCenter);
	layout->addWidget(warlock, 3, 1, Qt::AlignCenter);
	layout->addWidget(wizard, 3, 2, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Class");
	layout->addWidget(confirm, 4, 0, 1, 3, Qt::AlignCenter);

	// connect(confirm, &QPushButton::clicked, [=]{
	// 	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	// 	stackedWidget->setCurrentIndex(3);
	// });

	connect(confirm, SIGNAL (clicked()), SLOT (nextPage()));
}

void ClassWidget::nextPage() {
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget) {
		stackedWidget->setCurrentIndex(3);
	}
}

// ClassWidget::~ClassWidget() {
// 	delete this->barbarian;
// 	delete this->bard;
// 	delete this->cleric;
// 	delete this->druid;
// 	delete this->fighter;
// 	delete this->monk;
// 	delete this->paladin;
// 	delete this->ranger;
// 	delete this->rogue;
// 	delete this->sorcerer;
// 	delete this->warlock;
// 	delete this->wizard;
// }

RaceWidget::RaceWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);
	
	dwarf = new QRadioButton("Dwarf");
	elf = new QRadioButton("Elf");
	halfling = new QRadioButton("Halfling");
	human = new QRadioButton("Human");
	dragonborn = new QRadioButton("Dragonborn");
	gnome = new QRadioButton("Gnome");
	half_elf = new QRadioButton("Half Elf");
	half_orc = new QRadioButton("Half Orc");
	tiefling = new QRadioButton("Tiefling");

	layout->addWidget(dwarf, 0, 0, Qt::AlignCenter);
	layout->addWidget(elf, 0, 1, Qt::AlignCenter);
	layout->addWidget(halfling, 0, 2, Qt::AlignCenter);
	layout->addWidget(human, 1, 0, Qt::AlignCenter);
	layout->addWidget(dragonborn, 1, 1, Qt::AlignCenter);
	layout->addWidget(gnome, 1, 2, Qt::AlignCenter);
	layout->addWidget(half_elf, 2, 0, Qt::AlignCenter);
	layout->addWidget(half_orc, 2, 1, Qt::AlignCenter);
	layout->addWidget(tiefling, 2, 2, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Race");
	layout->addWidget(confirm, 3, 0, 1, 3, Qt::AlignCenter);

	// connect(confirm, &QPushButton::clicked, [=]{
	// 	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	// 	stackedWidget->setCurrentIndex(4);
	// });

	connect(confirm, SIGNAL (clicked()), SLOT (nextPage()));
}

void RaceWidget::nextPage() {
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget) {
		stackedWidget->setCurrentIndex(4);
	}
}

// RaceWidget::~RaceWidget() {
// 	delete this->dwarf;
// 	delete this->elf;
// 	delete this->halfling;
// 	delete this->dragonborn;
// 	delete this->gnome;
// 	delete this->half_elf;
// 	delete this->half_orc;
// 	delete this->tiefling;
// }

BackgroundWidget::BackgroundWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	acolyte = new QRadioButton("Acolyte");
	charlatan = new QRadioButton("Charlatan");
	criminal = new QRadioButton("Criminal");
	entertainer = new QRadioButton("Entertainer");
	folk_hero = new QRadioButton("Folk Hero");
	guild_artisan = new QRadioButton("Guild Artisan");
	hermit = new QRadioButton("Hermit");
	noble = new QRadioButton("Noble");
	outlander = new QRadioButton("Outlander");
	sage = new QRadioButton("Sage");
	sailor = new QRadioButton("Sailor");
	soldier = new QRadioButton("Soldier");
	urchin = new QRadioButton("Urchin");

	layout->addWidget(acolyte, 0, 0, Qt::AlignCenter);
	layout->addWidget(charlatan, 0, 1, Qt::AlignCenter);
	layout->addWidget(criminal, 0, 2, Qt::AlignCenter);
	layout->addWidget(entertainer, 1, 0, Qt::AlignCenter);
	layout->addWidget(folk_hero, 1, 1, Qt::AlignCenter);
	layout->addWidget(guild_artisan, 1, 2, Qt::AlignCenter);
	layout->addWidget(hermit, 2, 0, Qt::AlignCenter);
	layout->addWidget(noble, 2, 1, Qt::AlignCenter);
	layout->addWidget(outlander, 2, 2, Qt::AlignCenter);
	layout->addWidget(sage, 3, 0, 1, 2, Qt::AlignCenter);
	layout->addWidget(sailor, 3, 1, 1, 2, Qt::AlignCenter);
	layout->addWidget(soldier, 4, 0, 1, 2, Qt::AlignCenter);
	layout->addWidget(urchin, 4, 1, 1, 2, Qt::AlignCenter);

	QPushButton * confirm = new QPushButton("Confirm Background");
	layout->addWidget(confirm, 5, 0, 1, 3, Qt::AlignCenter);

	// connect(confirm, &QPushButton::clicked, [=]{
	// 	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	// 	stackedWidget->setCurrentIndex(5);
	// });

	connect(confirm, SIGNAL (clicked()), SLOT (nextPage()));
}

void BackgroundWidget::nextPage() {
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget) {
		stackedWidget->setCurrentIndex(5);
	}
}

// BackgroundWidget::~BackgroundWidget() {
// 	delete this->acolyte;
// 	delete this->charlatan;
// 	delete this->criminal;
// 	delete this->entertainer;
// 	delete this->folk_hero;
// 	delete this->guild_artisan;
// 	delete this->hermit;
// 	delete this->noble;
// 	delete this->outlander;
// 	delete this->sage;
// 	delete this->sage;
// 	delete this->urchin;
// }

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

	// connect(confirm, &QPushButton::clicked, [=]{
	// 	AddCharacter * addCharacterWidget = qobject_cast<AddCharacter *>(this->parentWidget());
	// 	addCharacterWidget->setCurrentIndex(0);
	// 	QStackedWidget * mainStackedWidget = qobject_cast<QStackedWidget *>(addCharacterWidget->parentWidget());
	// 	mainStackedWidget->setCurrentIndex(0);
	// 	// Need to somehow get access to the SelectCharacterWidget to call addCharacter(QString name)
	// 	// Alternatively we could move addCharacter to this file and have the QListWidget characters auto-upadte
	// });

	connect(confirm, SIGNAL (clicked()), SLOT (nextPage()));
}

void InventoryWidget::nextPage() {
	AddCharacter * addCharacterWidget = qobject_cast<AddCharacter *>(this->parentWidget());
	if (addCharacterWidget) {
		addCharacterWidget->setCurrentIndex(0);
	}
	QStackedWidget * mainStackedWidget = qobject_cast<QStackedWidget *>(addCharacterWidget->parentWidget());
	if (mainStackedWidget) {
		mainStackedWidget->setCurrentIndex(0);
	}
	// Need to somehow get access to the SelectCharacterWidget to call addCharacter(QString name)
	// Alternatively we could move addCharacter to this file and have the QListWidget characters auto-upadte
}
