/*
Name: viewCharacter.cpp
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 10/25/2024
*/

#include "viewCharacter.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

void ViewCharacter::loadCharacter(QString name)
{
    // Load the character's information, notes, and stats
}

ViewCharacter::ViewCharacter(QWidget *parent, QString name) :
    QWidget(parent)
{
    // Create the verticle layout for buttons
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create buttons for settings
    QPushButton *Backbutton = new QPushButton("Return to Character Select");
    QPushButton *importButton = new QPushButton(name);
    QPushButton *exportButton = new QPushButton("Export Character");

    // Add buttons to layout
    layout->addWidget(Backbutton);
    layout->addWidget(importButton);
    layout->addWidget(exportButton);

    // Make back button return to character select page
    connect(Backbutton, &QPushButton::clicked, [this]() {
        QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
        if (stackedWidget) {
            stackedWidget->setCurrentIndex(0); // character select is the first page so index 0
        }
    });
}

ViewCharacter::~ViewCharacter()
{
}
