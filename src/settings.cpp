/*
Name: settings.cpp
Description: Basic settings page to allow the user to import/export characters and return to character select.
Authors: Carson Treece, Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/25/2024
*/

#include "settings.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

Settings::Settings(QWidget *parent) :
    QWidget(parent)
{
    // Create the verticle layout for buttons
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create buttons for settings
    QPushButton *Backbutton = new QPushButton("Return to Character Select");
    QPushButton *importButton = new QPushButton("Import Character");
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

    connect(Backbutton, SIGNAL (clicked()), SLOT (nextPage()));
}

void Settings::nextPage() {
    QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (stackedWidget) {
        stackedWidget->setCurrentIndex(0); // character select is the first page so index 0
    }

}

Settings::~Settings()
{
}