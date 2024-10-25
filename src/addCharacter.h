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

#include <QWidget>
#include <QStackedWidget>

class AddCharacter : public QStackedWidget {
public:
	explicit AddCharacter(QWidget * parent = 0);
};

class StartWidget : public QWidget {
public:
	explicit StartWidget(QWidget * parent = 0);
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