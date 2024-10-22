#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>

int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Main Window Object
	QMainWindow mainWindow;
	// Gets main screen, typically going to be the only screen of the device as it will usually be used on laptops
	QScreen * screen  = QGuiApplication::primaryScreen();
	// Gets size of the screen
	QRect screenSize = screen->geometry();
	// Sets the window to that size
	mainWindow.setGeometry(screenSize);
	// Displays window
	mainWindow.show();
	// Runs the app
	return app.exec();
}