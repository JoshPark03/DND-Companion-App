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
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>

class MyComboBox;
class Portrait;
class StartWidget;
class BaseStatsWidget;
class ClassWidget;
class RaceWidget;
class BackgroundWidget;
class InventoryWidget;

class MyComboBox : public QComboBox
{
	Q_OBJECT
public:
	virtual void showPopup() override;
};

class Portrait : public QLabel
{
	Q_OBJECT
public:
	explicit Portrait(const QString& type, const QString& selection, QWidget *parent = 0);
private:
	QString *typeWidget;
public slots:
	void getImage(const QString& type);
};

class AddCharacter : public QStackedWidget
{
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

class StartWidget : public QWidget
{
	Q_OBJECT
public:
	explicit StartWidget(QWidget * parent = 0);

	// get function for name widget
	QLineEdit * getName() { return this->name; }
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
	void backPage();
	void nextPage();
};

class ClassWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ClassWidget(QWidget *parent = 0);
	// function for getting which radio button is selected
	QComboBox * getSelected();

private:
	QComboBox * classComboBox;
	QLabel * header;
private slots:
	void backPage();
	void nextPage();
	void headerUpdate(const QString &text);
};

class RaceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit RaceWidget(QWidget *parent = 0);
	// function for getting which radio button is selected
	QComboBox *getSelected();

private:
	QComboBox *raceComboBox;
private slots:
	void backPage();
	void nextPage();
};

struct BackgroundInfo {
    QString page;
    QString description;
    QString skillProficiency;
    QString toolProficiency;
    QString languages;
    QString equipment;
    QString feature;
    QString featureDescription;
};


class BackgroundWidget : public QWidget
{
    Q_OBJECT
public:
	explicit BackgroundWidget(QWidget *parent = 0);
	// function for getting which radio button is selected
	QComboBox *getSelected();

private:
    QComboBox *backgroundComboBox;

    // Add new private members for displaying background info
    QLabel *pageLabel;
    QLabel *descriptionLabel;
    QLabel *proficienciesLabel;
    QLabel *equipmentLabel;
    QLabel *featureLabel;
	QLabel *nameAndPageLabel;

    // Add a map to store background data
    QMap<QString, BackgroundInfo> backgrounds;

    // Add declarations for new methods
    void loadBackgrounds();
    void updateBackgroundInfo(const QString &backgroundName);

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
	// Declaration for the items list widget
	QListWidget * items;
private slots:
	void backPage();
	void nextPage();
};

#endif // ADD_CHARACTER_H