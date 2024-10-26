/*
Name: characterSelect.h
Description: Main landing page for the application, allowing users to create, delete, and manage characters.
			 Also provides navigation to settings.
Authors: Carson Treece, Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/22/2024
Last Modified: 10/25/2024
*/

#ifndef CHARACTER_SELECT
#define CHARACTER_SELECT

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLayout>
#include <QLabel>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>

class CharacterSelect : public QWidget {
	Q_OBJECT
public:
	explicit CharacterSelect(QWidget * parent = 0);
	~CharacterSelect();

	QListWidget * getCharacters() { return this->characters; }

	void addCharacter(QString charName = 0);
	void deleteCharacter();
	void loadCharacterList();

private:
	QGridLayout * layout;
	QPushButton * createChar;
	QPushButton * settings;
	QListWidget * characters;
	QPushButton * deleteChar;
};

#endif // CHARACTER_SELECT_H