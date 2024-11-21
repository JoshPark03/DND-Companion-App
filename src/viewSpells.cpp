/*
Name: viewSpells.cpp
Description: Page to view a character's spells.
Authors: Zachary Craig
Other Sources: ...
Date Created: 11/5/2024
Last Modified: 11/5/2024
*/

#include "viewSpells.h"
#include "themeUtils.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QList>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QLabel>

void ViewSpells::loadSpells()
{

}


ViewSpells::ViewSpells(QWidget *parent, QString name) :
    QWidget(parent)
{
    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesnt take up half the screen

    // Create buttons to go back to the character page
    QPushButton *backToCharacter = new QPushButton("Return to Character");
    // Create header label with character's name
    QLabel *spellsLabel = new QLabel(name + "'s Spells");

    // Add buttons to navbar
    navbarLayout->addWidget(backToCharacter);
    navbarLayout->addWidget(spellsLabel, 1, Qt::AlignCenter);

    
    // Create the layout for the spells page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create body widget
    QWidget *body = new QWidget();
    QVBoxLayout *bodyLayout = new QVBoxLayout(body);


    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));


    reloadTheme(); // Reload the theme after everything is placed
}

void ViewSpells::goBack()
{
    QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    
    if (mainStackedWidget)
    {
        mainStackedWidget->setCurrentIndex(0); // Switch to CharacterSelect (index 0)
    }
}

ViewSpells::~ViewSpells()
{
}