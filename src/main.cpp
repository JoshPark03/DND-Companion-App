#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QScreen>

#include "characterSelect.h"
#include "addCharacter.h"

// QLineEdit for textbox

// QTextEdit for textareas

// QPushButton for buttons

int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Window Object
	
	CharacterSelect * characterSelect = new CharacterSelect();
	characterSelect->resize(app.primaryScreen()->availableGeometry().size()*.7);


	// characterSelect.chi
	// AddCharacter * addCharacter = new AddCharacter();
	// addCharacter->resize(app.primaryScreen()->availableGeometry().size()*.7);
	// addCharacter->show();


	// Displays window in maximized state
	// characterSelect->showMaximized();
	characterSelect->show();
	// Runs the app
	return app.exec();
}

