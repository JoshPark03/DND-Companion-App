/*
Name: classWidget.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 11/20/2024
Last Modified: 11/20/2024
*/

#include "addCharacter.h"

#include <QLayout>
#include <QPushButton>

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

	// Create the class combo box
	MyComboBox * classComboBox = new MyComboBox;
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

	// Create header
	header = new QLabel("<h1>" + classComboBox->currentText() + "</h1>");
	header->setFixedHeight(50);
	header->setAlignment(Qt::AlignCenter);

	// Create portrait
	Portrait * portrait = new Portrait("classes", classComboBox->currentText());

	// Create the description layout
	QGridLayout * description = new QGridLayout();

	// Create the summary textbox
	QLabel * summary = new QLabel("summary");

	// Create the proficiencies layout
	QGridLayout * proficienciesLayout = new QGridLayout();
	QLabel * proficiencies = new QLabel("Proficiencies:");
	QLabel * armor = new QLabel("armor");
	QLabel * weapons = new QLabel("weapons");
	QLabel * tools = new QLabel("tools");
	QLabel * savingThrows = new QLabel("saving throws");
	QLabel * skills = new QLabel("Skills:");
	QComboBox * skill1 = new QComboBox();
	QComboBox * skill2 = new QComboBox();

	// Setting the proficiencies layout
	proficienciesLayout->addWidget(proficiencies, 0, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(armor, 1, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(weapons, 2, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(tools, 3, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(savingThrows, 4, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(skills, 5, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(skill1, 6, 0, {Qt::AlignCenter});
	proficienciesLayout->addWidget(skill2, 7, 0, {Qt::AlignCenter});

	// Create the equipment layout
	QGridLayout * equipmentLayout = new QGridLayout();
	QLabel * equipment = new QLabel("Equipment:");
	QComboBox * choice1 = new QComboBox();
	QComboBox * choice2 = new QComboBox();
	QLabel * otherEquipment = new QLabel("other equipment");

	// Setting the equipment layout
	equipmentLayout->addWidget(equipment, 0, 0, {Qt::AlignCenter});
	equipmentLayout->addWidget(choice1, 1, 0, {Qt::AlignCenter});
	equipmentLayout->addWidget(choice2, 2, 0, {Qt::AlignCenter});
	equipmentLayout->addWidget(otherEquipment, 3, 0, {Qt::AlignCenter});

	// Setting the description layout
	description->addWidget(summary, 0, 0, 1, -1, {Qt::AlignCenter});
	description->addLayout(proficienciesLayout, 1, 0, 2, 1);
	description->addLayout(equipmentLayout, 1, 1, 2, 1);

	// Create navigation buttons
	QPushButton * backButton = new QPushButton("Back");
	QPushButton * nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(classComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the navbar to the main layout
	mainLayout->addWidget(header, 0, 0, 1, -1, {Qt::AlignCenter});
	mainLayout->addWidget(portrait, 1, 0, 1, 1, {Qt::AlignCenter});
	mainLayout->addLayout(description, 1, 1, 1, 1);
	mainLayout->addWidget(navbar, 2, 0, 1, -1, {Qt::AlignBottom});

	// When backButton is clicked it calls the private SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	// When nextButton is clicked it calls the private SLOT function nextPage()
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
	// When classComboBox is changed it calls the private SLOT function backPage() and the portrait's public SLOT function getImage()
	connect(classComboBox, SIGNAL(currentTextChanged(const QString &)), SLOT(headerUpdate(const QString &)));
	connect(classComboBox, SIGNAL(currentTextChanged(const QString &)), portrait, SLOT(getImage(const QString &)));
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

/**
 * This function changes ClassWidget's header widget
 */
void ClassWidget::headerUpdate(const QString &text) {
	this->header->setText("<h1>" + text + "</h1>");
}