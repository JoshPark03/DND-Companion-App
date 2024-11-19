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
#include "themeUtils.h"


int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Window Object


	// Creating a stacked widget to hold multiple pages
	QStackedWidget * stackedWidget = new QStackedWidget();
	
	// Create the different pages
	CharacterSelect * characterSelect = new CharacterSelect();
	AddCharacter * addCharacter = new AddCharacter();
	QStackedWidget * characterInformation = new QStackedWidget();
	Settings * settings = new Settings();
	

	// Add pages to the stacked widget
	stackedWidget->addWidget(characterSelect);
	stackedWidget->addWidget(addCharacter);
	stackedWidget->addWidget(characterInformation);
	stackedWidget->addWidget(settings);

	qDebug() << "Widgets in QStackedWidget:";
    for (int i = 0; i < stackedWidget->count(); ++i) {
        QWidget *widget = stackedWidget->widget(i);
        if (widget) {
            qDebug() << "Index:" << i << ", Widget:" << widget->metaObject()->className();
        } else {
            qDebug() << "Index:" << i << ", Widget: nullptr";
        }
    }

	// Set the screen to start with the character select page
	stackedWidget->setCurrentWidget(characterSelect);

	// Show the stacked widget
	stackedWidget->resize(app.primaryScreen()->availableGeometry().size()*.7);
	stackedWidget->show();

	// Load the theme
	reloadTheme();

	// Runs the app
	return app.exec();
}

