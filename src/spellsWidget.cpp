/*
Name: spellsWidget.cpp
body: Implementation of the SpellsWidget class, which allows users to select spells for their character.
Authors: Josh Park
Other Sources: ...
Date Created: 11/27/2024
Last Modified: 11/27/2024
*/

#include "addCharacter.h"

SpellsWidget::SpellsWidget(QWidget * parent) :
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

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(nextButton);

	// Create header
	header = new QLabel("<h1>Spells</h1>");
	header->setFixedHeight(50);

	// Create the body layout
	QGridLayout * body = new QGridLayout();

	// Setting the body layout

	// Add the all pieces to the main layout
	mainLayout->addWidget(header, 0, 0, 1, -1, {Qt::AlignCenter});
	mainLayout->addLayout(body, 1, 1, 1, 4);
	mainLayout->addWidget(navbar, 2, 0, 1, -1, {Qt::AlignBottom});

	// When backButton is clicked it calls the private SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	// When nextButton is clicked it calls the private SLOT function nextPage()
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

/**
 * This function changes AddCharacter's StackedWidget to ClassWidget
 */
void SpellsWidget::backPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to RaceWidget
 */
void SpellsWidget::nextPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

void SpellsWidget::recordSpells() {
	
}