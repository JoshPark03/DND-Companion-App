/*
Name: settings.cpp
Description: Enhanced settings page to allow the user to import characters, switch themes, and return to character select with a structured layout.
Authors: Carson Treece, Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 11/06/2024
*/

#include "settings.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QFile>
#include <QApplication>
#include <QGridLayout>
#include <QSpacerItem>

Settings::Settings(QWidget *parent) :
    QWidget(parent)
{
    // Main layout for the settings page
    QGridLayout *mainLayout = new QGridLayout(this);

    // Back button at the top left
    QPushButton *backButton = new QPushButton("Return to Character Select");
    mainLayout->addWidget(backButton, 0, 0, Qt::AlignLeft);

    // Theme selection combo box at the middle left
    QComboBox *themeSelector = new QComboBox();
    themeSelector->addItem("Light Mode");
    themeSelector->addItem("Dark Mode");
    mainLayout->addWidget(themeSelector, 1, 0, Qt::AlignLeft);

    // Import button at the bottom left
    QPushButton *importButton = new QPushButton("Import Character");
    mainLayout->addWidget(importButton, 2, 0, Qt::AlignLeft);

    // Add some vertical spacing around the components
    mainLayout->setRowStretch(0, 1); // Stretch space above back button
    mainLayout->setRowStretch(1, 3); // Stretch space around theme selector
    mainLayout->setRowStretch(2, 2); // Stretch space around import button

    // Connect theme selector to change styles
    connect(themeSelector, &QComboBox::currentTextChanged, this, &Settings::changeTheme);

    // Connect the back button to navigate to the character select page
    connect(backButton, &QPushButton::clicked, [this]() {
        QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
        if (stackedWidget) {
            stackedWidget->setCurrentIndex(0); // character select is at index 0
        }
    });
}

// Slot to handle theme changes based on combo box selection
void Settings::changeTheme(const QString &theme) {
    QString qssFile;
    if (theme == "Light Mode") {
        qssFile = "src/themes/lightMode.qss";  // Light mode QSS file path
    } else if (theme == "Dark Mode") {
        qssFile = "src/themes/darkMode.qss";   // Dark mode QSS file path
    }

    // Apply selected theme
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
        file.close();
    // Error message, needed this
    } else {
        qDebug() << "Failed to open QSS file:" << qssFile;
    }

    // Save the selected theme to a file
    QFile themeFile("src/themes/selectedTheme.txt");
    if (themeFile.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&themeFile);
        out << theme;
        themeFile.close();
    }
}

Settings::~Settings()
{
}
