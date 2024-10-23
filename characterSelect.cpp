#ifndef CHARACTER_SELECT_H
#define CHARACTER_SELECT_H

#include "characterSelect.h"

#include <iostream>

#include <QPushButton>
#include <QListWidget>
#include <QLayout>
#include <QLabel>

CharacterSelect::CharacterSelect(QWidget * parent)
: QWidget(parent) {
	// TODO SQL or function call to get list of characters

	// Layout object for automatically centering and placing widgets
	QGridLayout * layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setGeometry(this->geometry());

	//spcer item for spacing
	QSpacerItem * spacer = new QSpacerItem(this->width(), this->height());
	layout->addItem(spacer, 0, 0, 100, 100);

	// button for character creation
	QPushButton * createChar = new QPushButton("Create Character", this);
	// std::cout << gridBoxHeight << ", " << gridBoxWidth << std::endl;
	// std::cout << createChar->height() << ", " << createChar->width() << std::endl;
	layout->addWidget(createChar, 5, 8, 5, 10);
	
	// List of all of the characters
	QListWidget * characters = new QListWidget();
	layout->addWidget(characters, 10, 20, 80, 60);
	

	// TODO for loop that adds all characters to the list

	if (characters->count() == 0) {
		QListWidgetItem * nochars = new QListWidgetItem("No Chracters Have been created");
		characters->addItem(nochars);
	}


	std::cout << layout->rowCount() << ", " << layout->columnCount() << std::endl;

}

#endif