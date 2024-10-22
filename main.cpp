#include <QApplication>
#include <QMainWindow>
#include <QLayout>
#include <QScreen>

#include "characterSelect.h"

// QLineEdit for textbox

// QTextEdit for textareas

// QPushButton for buttons

int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Window Object
	// QMainWindow mainWindow;
	// setting window size to 70% of the screen
	// mainWindow.resize(app.primaryScreen()->availableGeometry().size()*.7);
	
	CharacterSelect * characterSelect = new CharacterSelect();
	characterSelect->resize(app.primaryScreen()->availableGeometry().size()*.7);

	// mainWindow.setCentralWidget(characterSelect);


	// Displays window in maximized state
	// mainWindow.showMaximized();
	characterSelect->showMaximized();

	// Displays window
	// mainWindow.show();
	// Runs the app
	return app.exec();
}

