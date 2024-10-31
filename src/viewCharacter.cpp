/*
Name: viewCharacter.cpp
Description: Page to view a character's information, notes, and stats.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/25/2024
Last Modified: 10/31/2024
*/

#include "viewCharacter.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

void ViewCharacter::loadCharacter(QString name)
{
    // Load the character's information, notes, and stats
    this->printCharacterToConsole("./data/characters/" + name.toStdString() + "/stats.csv");
}

void ViewCharacter::printCharacterToConsole(std::string path) {
    std::cout << path << std::endl;
    std::fstream file(path);
    std::stringstream buffer;
    std::string bufferstr, str;
    bool first;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);
    getline(buffer, str, ',');
    std::cout << "Name: " << str << std::endl;
    getline(buffer, str, ',');
    std::cout << "Strength: " << str << std::endl;
    getline(buffer, str, ',');
    std::cout << "Dexterity: " << str << std::endl;
    getline(buffer, str, ',');
    std::cout << "Constitution: " << str << std::endl;
    getline(buffer, str, ',');
    std::cout << "Intelligence: " << str << std::endl;
    getline(buffer, str, ',');
    std::cout << "Wisdom: " << str << std::endl;
    getline(buffer, str, ',');
    std::cout << "Charisma: " << str << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Proficiencies: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Prepped Spells: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Spells: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Feats: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Languages: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Armor/Weapon Proficiencies: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Attuned Items: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    std::cout << "Money: ";
    getline(buffer, str, ',');
    std::cout << "P: " << str << " ";
    getline(buffer, str, ',');
    std::cout << "G: " << str << " ";
    getline(buffer, str, ',');
    std::cout << "S: " << str << " ";
    getline(buffer, str, ',');
    std::cout << "C: " << str << std::endl;

    getline(file, bufferstr);
    buffer = std::stringstream(bufferstr);

    first = true;
    std::cout << "Inventory: ";
    while (getline(buffer, str, ',')) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << str;
        first = false;
    }
    std::cout << std::endl;

    file.close();
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

    // Prints information to the console for testing
    connect(importButton, &QPushButton::clicked, [this, name]() {
        this->loadCharacter(name);
    });
}

ViewCharacter::~ViewCharacter()
{
}
