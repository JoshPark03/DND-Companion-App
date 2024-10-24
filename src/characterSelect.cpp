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
	QPushButton * createChar = new QPushButton("Create Character");
	layout->addWidget(createChar, 5, 8, 5, 10);
	
	// List of all of the characters
	QListWidget * characters = new QListWidget();
	layout->addWidget(characters, 10, 20, 80, 60);

	// button to delete selected character
	QPushButton * deleteChar = new QPushButton("Delete Character");
	layout->addWidget(deleteChar, 87, 20, 10, 60);
	
	// temp for loop to add test items to the list
	for(int i = 0; i < 100; i++) {
		QListWidgetItem * item = new QListWidgetItem("Character " + QString::number(i+1));
		characters->addItem(item);
	}

	
	// TODO for loop that adds all characters to the list
	

	if (characters->count() == 0) {
		QListWidgetItem * nochars = new QListWidgetItem("No Characters Have been created");
		characters->addItem(nochars);
	}

	// delete character button click event
	connect(deleteChar, &QPushButton::clicked, [=](){
		// TODO SQL or function call to delete character
		QListWidgetItem * item = characters->currentItem();
		if (item != nullptr) {
			// removes the character from the list
			characters->removeItemWidget(item);

			// gets the name of the character
			QString name = item->text();

			// TODO delete character from database

			// deletes the pointer from memory
			delete item;
		}
	});


	// std::cout << layout->rowCount() << ", " << layout->columnCount() << std::endl;

}

#endif