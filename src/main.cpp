/*
Name: main.cpp
Description: Main entry point for the application, initializes the GUI and sets up the main window.
Authors: Carson Treece, Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/20/2024
Last Modified: 11/06/2024
*/


#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QScreen>
#include <QStackedWidget>

#include "characterSelect.h"
#include "addCharacter.h"
#include "settings.h"

// QLineEdit for textbox

// QTextEdit for textareas

// QPushButton for buttons

// Function to load saved color theme
void loadTheme(QApplication &app) {
	QString theme;

	// Read from selectedTheme.txt
	QFile themeFile("src/themes/selectedTheme.txt");
	if (themeFile.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream in(&themeFile);
		theme = in.readLine();
		themeFile.close();
	}

	// Set the theme
	QString qssFile;
	if (theme == "Light Mode") {
		qssFile = "src/themes/lightMode.qss";
	} else if (theme == "Dark Mode") {
		qssFile = "src/themes/darkMode.qss";
	}

	// Apply the theme
	QFile file(qssFile);
	if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        file.close();
    } else {
        qDebug() << "Failed to open QSS file:" << qssFile;
    }
}

int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Window Object

	// Load the theme
	loadTheme(app);

	// Creating a stacked widget to hold multiple pages
	QStackedWidget * stackedWidget = new QStackedWidget();
	
	// Create the different pages
	CharacterSelect * characterSelect = new CharacterSelect();
	AddCharacter * addCharacter = new AddCharacter();
	Settings * settings = new Settings();
	

	// Add pages to the stacked widget
	stackedWidget->addWidget(characterSelect);
	stackedWidget->addWidget(addCharacter);
	stackedWidget->addWidget(settings);

	// Set the screen to start with the character select page
	stackedWidget->setCurrentWidget(characterSelect);

	// Show the stacked widget
	stackedWidget->resize(app.primaryScreen()->availableGeometry().size()*.7);
	stackedWidget->show();


	// characterSelect.chi
	// AddCharacter * addCharacter = new AddCharacter();
	// addCharacter->resize(app.primaryScreen()->availableGeometry().size()*.7);
	// addCharacter->show();


	// Displays window in maximized state
	// characterSelect->showMaximized();
	// characterSelect->show();
	// Runs the app
	return app.exec();
}

