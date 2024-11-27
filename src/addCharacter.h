/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park, Chanu Lee, Carson Treece
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 11/27/2024
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
#include <QList>
#include <QLayout>
#include <QPushButton>

class UpComboBox;
class Portrait;
class StartWidget;
class BaseStatsWidget;
class ClassWidget;
class SpellsWidget;
class RaceWidget;
class BackgroundWidget;
class InventoryWidget;

class UpComboBox : public QComboBox
{
	Q_OBJECT
public:
	virtual void showPopup() override;
};

class Portrait : public QLabel
{
	Q_OBJECT
public:
	explicit Portrait(const QString &type, const QString &selection, QWidget *parent = 0);

private:
	QString *typeWidget;
public slots:
	void getImage(const QString &type);
};

class AddCharacter : public QStackedWidget
{
	Q_OBJECT
private:
	// Declaration of all custom widget class variables
	StartWidget *startWidget;
	BaseStatsWidget *baseStatsWidget;
	ClassWidget *classWidget;
	SpellsWidget * spellsWidget;
	RaceWidget *raceWidget;
	BackgroundWidget *backgroundWidget;
	InventoryWidget *inventoryWidget;

public:
	explicit AddCharacter(QWidget *parent = 0);

	// get functions for each widget variable
	StartWidget *getStartWidget() { return this->startWidget; }
	BaseStatsWidget *getBaseStatsWidget() { return this->baseStatsWidget; }
	ClassWidget *getClassWidget() { return this->classWidget; }
	SpellsWidget * getSpellsWidget() { return this->spellsWidget; }
	RaceWidget *getRaceWidget() { return this->raceWidget; }
	BackgroundWidget *getBackgroundWidget() { return this->backgroundWidget; }
	InventoryWidget *getInventoryWidget() { return this->inventoryWidget; }
signals:
	void createdCharacter();
public slots:
	void createCharacter();
};

class StartWidget : public QWidget
{
	Q_OBJECT
public:
	explicit StartWidget(QWidget *parent = 0);

	// get function for name widget
	QString getName() { return this->name->text(); }

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

	// get funcions for each stat val widget
	int getStrength() { return this->strengthVal->value(); }
	int getDexterity() { return this->dexterityVal->value(); }
	int getConstitution() { return this->constitutionVal->value(); }
	int getIntelligence() { return this->intelligenceVal->value(); }
	int getWisdom() { return this->wisdomVal->value(); }
	int getCharisma() { return this->charismaVal->value(); }

private:
	// Declarations for each stat val widget
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

// This is a very hastily put together solution for replacing values
// replace this with a tsv file later
inline QList<QString> simpleMelee = {"Club", "Dagger", "Greatclub", "Handaxe", "Javelin", "Light Hammer", "Mace", "Quarterstaff", "Sickle", "Spear"};
inline QList<QString> simpleRanged = {"Light Crossbow", "Dart", "Shortbow", "Sling"};
inline QList<QString> martialMelee = {"Battleaxe", "Flail", "Glaive", "Greataxe", "Greatsword", "Halberd"
																								"Lance",
									  "Longsword", "Maul", "Moerningstar", "Pike", "Rapier", "Scimitar", "Shortsword", "Trident", "War pick", "Warhammer", "Whip"};
inline QList<QString> martialRanged = {"Blowgun", "Hand Crossbow", "Heavy Crossbow", "Longbow", "Net"};
inline QList<QString> allSkills = {"Acrobatics", "Animal Handling", "Arcana", "Athletics", "Deception", "History", "Intimidation", "Investigation", "Medicine", "Nature", "Perception", "Performance", "Persuasion", "Religion", "Sleight of Hand", "Stealth", "Survival"};

struct ClassInfo
{
	QString book;
	QString page;
	QList<QString> *armorProficiencies;
	QList<QString> *weaponProficiencies;
	QList<QString> *toolProficiencies;
	QList<QString> *savingThrows;
	int numSkills;
	QList<QString> *skillProficiencies;
	QList<QList<QString> *> *equipmentChoices;
	QList<QString> *givenEquipment;
	QString summary;
};

class ClassWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ClassWidget(QWidget * parent = 0);\
	bool isSpellcaster() { return this->spellcasters.contains(this->getClass()); }
	// function for getting which class is selected
	QString getClass();
	QList<QString> * getArmorProficincies();
	QList<QString> * getWeaponProficincies();
	QList<QString> * getToolProficincies();
	QList<QString> * getSavingThrows();
	QList<QString> * getSkillProficincies();
	QList<QString> * getItems();

private:
	QMap<QString, ClassInfo *> classes;
	QLabel * header;
	Portrait * portrait;
	QLabel * summary;
	QLabel * armor;
	QLabel * weapons;
	QLabel * tools;
	QLabel * savingThrows;
	QVBoxLayout * skillsLayout;
	QList<UpComboBox *> * skillsList;
	QVBoxLayout * choicesLayout;
	QList<UpComboBox *> * choicesList;
	UpComboBox * multipleChoiceBox;
	QList<QWidget *> * multipleChoice;
	QLabel * givenEquipment;
	UpComboBox * classComboBox;
	QList<QString> spellcasters = {"Bard", "Cleric", "Druid", "Paladin", "Ranger", "Sorcerer", "Warlock", "Wizard"};
	void loadClasses();
private slots:
	void backPage();
	void nextPage();
	void updateClassInfo(const QString &name);
	void updateChoice();
	void proficiencyDisableSkills();
};

class SpellsWidget : public QWidget {
	Q_OBJECT
public:
	explicit SpellsWidget(QWidget * parent = 0);
private:
	struct SpellInfo {
		QString book;
		int page;
		int level;
		QString school;
		QString time;
		int minRange;
		int maxRange;
		bool verbal;
		bool somatic;
		bool material;
		QString duration;
		bool concentration;
		bool ritual;
		QString description;
	};

	QLabel * header;
	QListWidget * spellsList;
	QPushButton * addSpellButton;
	QPushButton * removeSpellButton;

	QMap<QString, SpellInfo> * spells;
public slots:
	void recordSpells();
private slots:
	void backPage();
	void nextPage();
	void addSpell();
	void removeSpell();
};

struct SubRaceInfo
{
	QString abilityScoreIncrease;
	QString size;
	QString speed;
	QString languages;
	QList<QString> *abilities;
};

struct RaceInfo
{
	QString book;
	QString page;
	QString summary;
	bool subRacesExist;
	QMap<QString, SubRaceInfo *> subRaces;
};

class RaceWidget : public QWidget
{
	Q_OBJECT
public:
	explicit RaceWidget(QWidget *parent = 0);
	// function for getting which race is selected
	QString getRace();
	QString getSubRace();
	QString getAbilityScoreIncrease();
	QString getSize();
	QString getSpeed();
	QString getLanguages();
	QList<QString> *getAbilities();

private:
	// Add a map to store background data
	QMap<QString, RaceInfo *> races;

	// Add new private members for displaying race info
	QLabel *header;
	Portrait *portrait;
	UpComboBox *raceComboBox;
	UpComboBox *subRaceComboBox;
	QVBoxLayout *subRaceLayout;
	QLabel *summary;
	QLabel *subRace;
	QLabel *abilityScoreIncrease;
	QLabel *size;
	QLabel *speed;
	QLabel *languages;
	QLabel *abilities;

	// Add declarations for new methods
	void loadRaces();

private slots:
	void backPage();
	void nextPage();
	void updateRaceInfo(const QString &raceName);
	void updateSubRaceInfo(const QString &subRaceName);
};

struct BackgroundInfo
{
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
	// function for getting which background is selected
	QList<QString> getSkillProficincies() { return this->skillProficiencies; }
	QList<QString> getToolProficincies() { return this->toolProficiencies; }
	QList<QString> getItems() { return this->items; }

private:
	QComboBox *backgroundComboBox;

	// Add new private members for displaying background info
	QLabel *pageLabel;
	QLabel *descriptionLabel;
	QLabel *proficienciesLabel;
	QLabel *equipmentLabel;
	QLabel *featureLabel;
	QLabel *nameAndPageLabel;

	// These should be the private Qlists that will be used to make our character csv file
	// Not sure about how to implement the rest, out of time for tongiht
	QList<QString> skillProficiencies;
	QList<QString> toolProficiencies;
	QList<QString> items;

	// Add a map to store background data
	QMap<QString, BackgroundInfo> backgrounds;

	// Add declarations for new methods
	void loadBackgrounds();
	void updateBackgroundInfo(const QString &backgroundName);
signals:
	void finished();
private slots:
	void backPage();
	void nextPage();
};

class InventoryWidget : public QWidget
{
	Q_OBJECT
public:
	explicit InventoryWidget(QWidget *parent = 0);
	QList<QString> getItems();

private:
	// Declaration for the items list widget
	QListWidget *items;
	QPushButton *removeItemButton;
signals:
	void finished();
public slots:
	void autofillInventory();
private slots:
	void backPage();
	void nextPage();
	void addItem();
	void deleteItem();
	void selectItem();
};

#endif // ADD_CHARACTER_H