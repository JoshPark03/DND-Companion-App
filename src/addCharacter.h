/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 11/5/2024
*/

#ifndef ADD_CHARACTER_H
#define ADD_CHARACTER_H

#include <QStackedWidget>
#include <QListWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>

class StartWidget;
class BaseStatsWidget;
class ClassWidget;
class RaceWidget;
class BackgroundWidget;
class InventoryWidget;

class AddCharacter : public QStackedWidget {
	Q_OBJECT
private:
	// Declaration of all custom widget class variables
	StartWidget * startWidget;
	BaseStatsWidget * baseStatsWidget;
	ClassWidget * classWidget;
	RaceWidget * raceWidget;
	BackgroundWidget * backgroundWidget;
	InventoryWidget * inventoryWidget;
public:
	explicit AddCharacter(QWidget * parent = 0);

	// get functions for each widget variable
	StartWidget * getStartWidget() { return this->startWidget; }
	BaseStatsWidget * getBaseStatsWidget() { return this->baseStatsWidget; }
	ClassWidget * getClassWidget() { return this->classWidget; }
	RaceWidget * getRaceWidget() { return this->raceWidget; }
	BackgroundWidget * getBackgroundWidget() { return this->backgroundWidget; }
	InventoryWidget * getInventoryWidget() { return this->inventoryWidget; }
};

class StartWidget : public QWidget {
	Q_OBJECT
public:
	explicit StartWidget(QWidget * parent = 0);

	// get function for name widget
	QLineEdit * getName() { return this->name; }
private:
	QLineEdit * name;
private slots:
	void nextPage();
};

class BaseStatsWidget : public QWidget {
	Q_OBJECT
public:
	explicit BaseStatsWidget(QWidget * parent = 0);

	// get funcions for each stat val widget
	int getStrength() { return this->strengthVal->value(); }
	int getDexterity() { return this->dexterityVal->value(); }
	int getConstitution() { return this->constitutionVal->value(); }
	int getIntelligence() { return this->intelligenceVal->value(); }
	int getWisdom() { return this->wisdomVal->value(); }
	int getCharisma() { return this->charismaVal->value(); }
private:
	// Declarations for each stat val widget 
	QSpinBox * strengthVal;
	QSpinBox * dexterityVal;
	QSpinBox * constitutionVal;
	QSpinBox * intelligenceVal;
	QSpinBox * wisdomVal;
	QSpinBox * charismaVal;
private slots:
	void nextPage();
};

class ClassWidget : public QWidget {
	Q_OBJECT
public:
	explicit ClassWidget(QWidget * parent = 0);

	// function for getting which radio button is selected
	QRadioButton * getSelected();
private:
	// Declarations for each class radio button
	QRadioButton * barbarian;
	QRadioButton * bard;
	QRadioButton * cleric;
	QRadioButton * druid;
	QRadioButton * fighter;
	QRadioButton * monk;
	QRadioButton * paladin;
	QRadioButton * ranger;
	QRadioButton * rogue;
	QRadioButton * sorcerer;
	QRadioButton * warlock;
	QRadioButton * wizard;
private slots:
	void nextPage();
};

class RaceWidget : public QWidget {
	Q_OBJECT
public:
	explicit RaceWidget(QWidget * parent = 0);

	// function for getting which radio button is selected
	QRadioButton * getSelected();
private:
	// Declarations for each race radio button
	QRadioButton * dwarf;
	QRadioButton * elf;
	QRadioButton * halfling;
	QRadioButton * human;
	QRadioButton * dragonborn;
	QRadioButton * gnome;
	QRadioButton * half_elf;
	QRadioButton * half_orc;
	QRadioButton * tiefling;
private slots:
	void nextPage();
};

class BackgroundWidget : public QWidget {
	Q_OBJECT
public:
	explicit BackgroundWidget(QWidget * parent = 0);

	// function for getting which radio button is selected
	QRadioButton * getSelected();
private:
	// Declarations for each background radio button
	QRadioButton * acolyte;
	QRadioButton * charlatan;
	QRadioButton * criminal;
	QRadioButton * entertainer;
	QRadioButton * folk_hero;
	QRadioButton * guild_artisan;
	QRadioButton * hermit;
	QRadioButton * noble;
	QRadioButton * outlander;
	QRadioButton * sage;
	QRadioButton * sailor;
	QRadioButton * soldier;
	QRadioButton * urchin;
private slots:
	void nextPage();
};

class InventoryWidget : public QWidget {
	Q_OBJECT
public:
	explicit InventoryWidget(QWidget * parent = 0);
private:
	// Declaration for the items list widget
	QListWidget * items;
private slots:
	void nextPage();
};

#endif // ADD_CHARACTER_H