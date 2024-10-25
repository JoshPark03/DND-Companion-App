#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QScreen>
#include <QStackedWidget>

#include "characterSelect.h"
#include "addCharacter.h"
#include "viewCharacter.h"
#include "settings.h"

// QLineEdit for textbox

// QTextEdit for textareas

// QPushButton for buttons

int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Window Object

	// Creating a stacked widget to hold multiple pages
	QStackedWidget * stackedWidget = new QStackedWidget();
	
	// Create the different pages
	CharacterSelect * characterSelect = new CharacterSelect();
	AddCharacter * addCharacter = new AddCharacter();
	ViewCharacter * viewCharacter = new ViewCharacter();
	Settings * settings = new Settings();
	

	// Add pages to the stacked widget
	stackedWidget->addWidget(characterSelect);
	stackedWidget->addWidget(addCharacter);
	stackedWidget->addWidget(viewCharacter);
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

