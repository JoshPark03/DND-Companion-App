/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/25/2024
*/

#ifndef ADD_CHARACTER_H
#define ADD_CHARACTER_H

#include <QStackedWidget>
#include <QLineEdit>
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
	StartWidget * startWidget;
	BaseStatsWidget * baseStatsWidget;
	ClassWidget * classWidget;
	RaceWidget * raceWidget;
	BackgroundWidget * backgroundWidget;
	InventoryWidget * inventoryWidget;
public:
	explicit AddCharacter(QWidget * parent = 0);
	~AddCharacter();
	StartWidget * getStartWidget() { return this->startWidget; }
	BaseStatsWidget * getBaseStatsWidget() { return this->baseStatsWidget; }
	ClassWidget * getClassWidget() { return this->classWidget; }
	RaceWidget * getRaceWidget() { return this->raceWidget; }
	BackgroundWidget * getBackgroundWidget() { return this->backgroundWidget; }
	InventoryWidget * getInventoryWidget() { return this->inventoryWidget; }
};

class StartWidget : public QWidget {
public:
	explicit StartWidget(QWidget * parent = 0);
	~StartWidget();
	QLineEdit * getName() { return this->name; }
private:
	QLineEdit * name;
};

class BaseStatsWidget : public QWidget {
public:
	explicit BaseStatsWidget(QWidget * parent = 0);
};

class ClassWidget : public QWidget {
public:
	explicit ClassWidget(QWidget * parent = 0);
};

class RaceWidget : public QWidget {
public:
	explicit RaceWidget(QWidget * parent = 0);
};

class BackgroundWidget : public QWidget {
public:
	explicit BackgroundWidget(QWidget * parent = 0);
};

class InventoryWidget : public QWidget {
public:
	explicit InventoryWidget(QWidget * parent = 0);
};

#endif // ADD_CHARACTER_H