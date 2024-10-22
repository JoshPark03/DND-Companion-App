#ifndef CHARACTER_SELECT_H
#define CHARACTER_SELECT_H

#include "characterSelect.h"

#include <iostream>

#include <QListWidget>
#include <QLayout>
#include <QLabel>

CharacterSelect::CharacterSelect(QWidget * parent)
: QWidget(parent) {
	// TODO SQL or function call to get list of characters
	// Creates a layout object and we use it to center the list of characters
	QGridLayout * layout = new QGridLayout(this);
	QListWidget * characters = new QListWidget(this);
	layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 0, 0);
	layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 2, 2);
	layout->addWidget(characters, 0, 1);

	// TODO for loop that adds all characters to the list
}

#endif