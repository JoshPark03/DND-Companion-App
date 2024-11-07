/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/31/2024
*/

#ifndef ADD_CHARACTER_H
#define ADD_CHARACTER_H

#include <QStackedWidget>
#include <QListWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>

class StartWidget;
class BaseStatsWidget;
class ClassWidget;
class RaceWidget;
class BackgroundWidget;
class InventoryWidget;

class AddCharacter : public QStackedWidget
{
	Q_OBJECT
private:
	StartWidget *startWidget;
	BaseStatsWidget *baseStatsWidget;
	ClassWidget *classWidget;
	RaceWidget *raceWidget;
	BackgroundWidget *backgroundWidget;
	InventoryWidget *inventoryWidget;

public:
	explicit AddCharacter(QWidget *parent = 0);
	// ~AddCharacter();
	StartWidget *getStartWidget() { return this->startWidget; }
	BaseStatsWidget *getBaseStatsWidget() { return this->baseStatsWidget; }
	ClassWidget *getClassWidget() { return this->classWidget; }
	RaceWidget *getRaceWidget() { return this->raceWidget; }
	BackgroundWidget *getBackgroundWidget() { return this->backgroundWidget; }
	InventoryWidget *getInventoryWidget() { return this->inventoryWidget; }
};

class StartWidget : public QWidget
{
	Q_OBJECT
public:
	explicit StartWidget(QWidget *parent = 0);
	QLineEdit *getName() { return this->name; }

private:
	QLineEdit *name;
private slots:
	void backPage();
	void nextPage();
};

class BaseStatsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BaseStatsWidget(QWidget *parent = 0);
	// ~BaseStatsWidget();
	int getStrength() { return this->strengthVal->value(); }
	int getDexterity() { return this->dexterityVal->value(); }
	int getConstitution() { return this->constitutionVal->value(); }
	int getIntelligence() { return this->intelligenceVal->value(); }
	int getWisdom() { return this->wisdomVal->value(); }
	int getCharisma() { return this->charismaVal->value(); }

private:
	QSpinBox *strengthVal;
	QSpinBox *dexterityVal;
	QSpinBox *constitutionVal;
	QSpinBox *intelligenceVal;
	QSpinBox *wisdomVal;
	QSpinBox *charismaVal;
private slots:
	void backPage();
	void nextPage();
};

class ClassWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ClassWidget(QWidget *parent = 0);
	// ~ClassWidget();
	QComboBox *getSelected();

private:
	QComboBox *classComboBox;
private slots:
	void backPage();
	void nextPage();
};

class RaceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit RaceWidget(QWidget *parent = 0);
	// ~RaceWidget();
	QComboBox *getSelected();

private:
	QComboBox *raceComboBox;
private slots:
	void backPage();
	void nextPage();
};

class BackgroundWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BackgroundWidget(QWidget *parent = 0);
	// ~BackgroundWidget();
	QComboBox *getSelected();

private:
	QComboBox *backgroundComboBox;
private slots:
	void backPage();
	void nextPage();
};

class InventoryWidget : public QWidget
{
	Q_OBJECT
public:
	explicit InventoryWidget(QWidget *parent = 0);

private:
	QListWidget *items;
private slots:
	void backPage();
	void nextPage();
};

#endif // ADD_CHARACTER_H