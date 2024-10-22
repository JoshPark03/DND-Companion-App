#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QScreen>

int main(int argc, char ** argv) {
	QApplication app (argc, argv);
	// Main Window Object
	QMainWindow mainWindow;
	// Sets to fullscreen (no minimize/close)
	// mainWindow.setWindowState(Qt::WindowFullScreen);
	// Sets to match window size
	mainWindow.setWindowState(Qt::WindowMaximized);
	// Displays window
	mainWindow.show();
	// Runs the app
	return app.exec();
}