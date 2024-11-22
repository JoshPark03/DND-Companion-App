/*
Name: classWidget.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 11/20/2024
Last Modified: 11/20/2024
*/

#include "addCharacter.h"

#include <iostream>

#include <QDir>
#include <QFile>
#include <QLayout>
#include <QPushButton>
#include <QRegularExpression>

/**
 * Constructor for the class
 */
ClassWidget::ClassWidget(QWidget * parent) :
	QWidget(parent) {
	// Create the main layout
	QGridLayout * mainLayout = new QGridLayout(this);

	// Create and configure layout for the navbar
	QWidget * navbar = new QWidget();
	QHBoxLayout * navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create navigation buttons
	QPushButton * backButton = new QPushButton("Back");
	QPushButton * nextButton = new QPushButton("Next");

	// Create the class combo box
	this->classComboBox = new UpComboBox;
	// this->classComboBox->addItem("Barbarian");
	// this->classComboBox->addItem("Bard");
	// this->classComboBox->addItem("Cleric");
	// this->classComboBox->addItem("Druid");
	// this->classComboBox->addItem("Fighter");
	// this->classComboBox->addItem("Monk");
	// this->classComboBox->addItem("Paladin");
	// this->classComboBox->addItem("Ranger");
	// this->classComboBox->addItem("Rogue");
	// this->classComboBox->addItem("Sorcerer");
	// this->classComboBox->addItem("Warlock");
	// this->classComboBox->addItem("Wizard");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(this->classComboBox);
	navbarLayout->addWidget(nextButton);

	// Create header
	header = new QLabel("<h1>" + this->classComboBox->currentText() + "</h1>");
	header->setFixedHeight(50);

	// Create portrait
	this->portrait = new Portrait("classes", this->classComboBox->currentText());

	// Create the description layout
	QGridLayout * description = new QGridLayout();

	// Create the summary textbox
	this->summary = new QLabel("summary");
	summary->setWordWrap(true);

	// Create the proficiencies layout
	QVBoxLayout * proficienciesLayout = new QVBoxLayout();
	QLabel * proficiencies = new QLabel("<h3>Proficiencies:</h3>");
	this->armor = new QLabel("armor");
	this->weapons = new QLabel("weapons");
	this->tools = new QLabel("tools");
	this->savingThrows = new QLabel("saving throws");

	// Create the skills layout
	this->skillsLayout = new QVBoxLayout();
	QLabel * skills = new QLabel("<h4>Skills:</h4>");
	this->skillsList = nullptr;

	skillsLayout->addWidget(skills, {Qt::AlignTop});

	// Setting the proficiencies layout
	proficienciesLayout->addWidget(proficiencies, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
	proficienciesLayout->addWidget(armor, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
	proficienciesLayout->addWidget(weapons, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
	proficienciesLayout->addWidget(tools, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
	proficienciesLayout->addWidget(savingThrows, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});
	proficienciesLayout->addLayout(skillsLayout, Qt::Alignment{Qt::AlignLeft, Qt::AlignTop});

	// Create the equipment layout
	QVBoxLayout * equipmentLayout = new QVBoxLayout();
	QLabel * equipment = new QLabel("<h3>Equipment:</h3>");
	this->choicesList = nullptr;
	this->givenEquipment = new QLabel("given equipment");

	// Create the equipment choices layout
	this->choicesLayout = new QVBoxLayout();

	// Setting the equipment layout
	equipmentLayout->addWidget(equipment,  {Qt::AlignTop});
	equipmentLayout->addLayout(this->choicesLayout,  {Qt::AlignTop});
	equipmentLayout->addWidget(givenEquipment,  {Qt::AlignTop});

	// Setting the description layout
	description->addWidget(summary, 0, 0, -1, 1, {Qt::AlignTop});
	description->addLayout(proficienciesLayout, 0, 1, 2, 1, {Qt::AlignTop});
	description->addLayout(equipmentLayout, 0, 2, 1, 1, {Qt::AlignTop});

	// Add the all pieces to the main layout
	mainLayout->addWidget(header, 0, 0, 1, -1, {Qt::AlignCenter});
	mainLayout->addWidget(portrait, 1, 0, 1, 1, {Qt::AlignCenter});
	mainLayout->addLayout(description, 1, 1, 1, 4);
	mainLayout->addWidget(navbar, 2, 0, 1, -1, {Qt::AlignBottom});

	//loading the classes from the tsv file
	this->loadClasses();

	// Initialize with the first class's details
	if (!classComboBox->currentText().isEmpty())
	{
		this->updateClassInfo(classComboBox->currentText());
	}

	// When backButton is clicked it calls the private SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	// When nextButton is clicked it calls the private SLOT function nextPage()
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
	// When classComboBox is changed it calls the private SLOT function updateClassInfo()
	connect(this->classComboBox, SIGNAL(currentTextChanged(const QString &)), SLOT(updateClassInfo(const QString &)));
}

/**
 * This function changes AddCharacter's StackedWidget to BaseStatsWidget
 */
void ClassWidget::backPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(1);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to RaceWidget
 */
void ClassWidget::nextPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3);
	}
}

void ClassWidget::loadClasses() {
	QFile file("data/databases/ClassInventory.tsv"); // read in the file
	qDebug() << "Current working directory:" << QDir::currentPath();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		// error checking for debugging
		std::cerr << "Failed to open file" << std::endl;
		return;
	}

	QTextStream in(&file); // read in the file
	bool isHeader = true;  // our first line is a header

	while (!in.atEnd()) {
		QString line = in.readLine(); // read in the line
		if (isHeader) {
			isHeader = false; // we are no longer on the header
			continue;		  // skip the header
		}

		QStringList fields = line.split("\t"); // split the line by \t
		if (fields.size() < 10) {
			continue; // ensure we get all the fields
		}

		QList<QString> * armors = new QList<QString>(fields[3].split(", "));

		QList<QString> * weapons = new QList<QString>(fields[4].split(", "));

		QList<QString> * tools = new QList<QString>(fields[5].split(", "));

		QList<QString> * savingThrows = new QList<QString>(fields[6].split(", "));

		QList<QString> * skills = new QList<QString>(fields[8].split(", "));
		// qDebug() << "matching " << fields[9];
		QList<QList<QString> *> * choices = new QList<QList<QString> *>;
		QRegularExpression re("\\([^)]+\\)");
		QRegularExpressionMatchIterator match = re.globalMatch(fields[9]);
		// qDebug() << choiceList;
		while (match.hasNext()) {
			QString itemsStr = match.next().captured();
			itemsStr = itemsStr.mid(1, itemsStr.size()-2);
			// qDebug() << itemsStr;
			QList<QString> items = itemsStr.split(", ");
			QList<QString> * choice = new QList<QString>;
			for (QString item : items) {
				choice->append(item);
			}
			choices->append(choice);
			// qDebug() << *choice << Qt::endl;
		}

		QList<QString> * given = new QList<QString>(fields[10].split(", "));

		ClassInfo * info = new ClassInfo {
			fields[1],
			fields[2],
			armors,
			weapons,
			tools,
			savingThrows,
			fields[7].toInt(),
			skills,
			choices,
			given,
			fields[11]
		};

		QString name = fields[0];
		this->classes[name] = info;
		this->classComboBox->addItem(name);
	}
}

/**
 * This function updates all information in ClassWidget to match what class is selected
 */
void ClassWidget::updateClassInfo(const QString &name) {
	if (!this->classes.contains(name))
	{
		qWarning() << "Background not found:" << name;
		return;
	}

	ClassInfo * info = this->classes[name];

	this->header->setText("<h1>" + name + " " + info->book + " pg " + info->page + "</h1>");

	this->portrait->getImage(name);

	// this->summary->setText("<h3>Summary:</h3><br>");
	this->summary->setText("<h3>Summary:</h3><br>" + info->summary);

	this->armor->setText("<h4>Armor:</h4>");
	for (QString prof : *info->armorProficiencies) {
		this->armor->setText(this->armor->text() + "<br>" + prof);
	}

	this->weapons->setText("<h4>Weapons:</h4>");
	for (QString prof : *info->weaponProficiencies) {
		this->weapons->setText(this->weapons->text() + "<br>" + prof);
	}

	this->tools->setText("<h4>Tools:</h4>");
	for (QString prof : *info->toolProficiencies) {
		this->tools->setText(this->tools->text() + "<br>" + prof);
	}

	this->savingThrows->setText("<h4>Saving Throws:</h4>");
	for (QString prof : *info->savingThrows) {
		this->savingThrows->setText(this->savingThrows->text() + "<br>" + prof);
	}

	if (this->skillsList) {
		for (auto skill : *this->skillsList) {
			this->skillsLayout->removeWidget(skill);
			delete skill;
		}
		delete this->skillsList;
	}
	this->skillsList = new QList<UpComboBox *>();
	for (int i = 0; i < info->numSkills; i++) {
		UpComboBox * skillBox = new UpComboBox();
		for (QString skill : *info->skillProficiencies) {
			if (skill == "All") {
				skillBox->addItems(allSkills);
				continue;
			}
			skillBox->addItem(skill);
		}
		this->skillsLayout->addWidget(skillBox, {Qt::AlignTop});
		this->skillsList->append(skillBox);
	}
	// add connect function for the each skillbox here to prevent them from being the same skill

	if (this->choicesList) {
		for (auto choice : *this->choicesList) {
			this->choicesLayout->removeWidget(choice);
			delete choice;
		}
		delete this->choicesList;
	}
	this->choicesList = new QList<UpComboBox *>();
	for (auto choice : *info->equipmentChoices) {
		UpComboBox * choiceBox = new UpComboBox();
		for (QString item : *choice) {
			if (item == "Simple") {
				choiceBox->addItems(simpleMelee);
				choiceBox->addItems(simpleRanged);
				continue;
			}
			if (item == "Simple Melee") {
				choiceBox->addItems(simpleMelee);
				continue;
			}
			if (item == "Martial") {
				choiceBox->addItems(martialMelee);
				choiceBox->addItems(martialRanged);
				continue;
			}
			if (item == "Martial Melee") {
				choiceBox->addItems(martialMelee);
				continue;
			}
			choiceBox->addItem(item);
		}
		this->choicesLayout->addWidget(choiceBox, {Qt::AlignTop});
		this->choicesList->append(choiceBox);
	}

	this->givenEquipment->clear();
	bool first = true;
	for (QString prof : *info->givenEquipment) {
		if (first) {
			first = false;
			this->givenEquipment->setText(this->givenEquipment->text() + prof);
		} else {
			this->givenEquipment->setText(this->givenEquipment->text() + "<br>" + prof);
		}
	}
}