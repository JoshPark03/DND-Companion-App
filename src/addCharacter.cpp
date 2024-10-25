/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/24/2024
*/


#ifndef ADD_CHARACTER_H
#define ADD_CHARACTER_H

#include "addCharacter.h"

#include <QPushButton>
#include <QLayout>

AddCharacter::AddCharacter(QWidget * parent) : QWidget(parent) {
	QGridLayout * layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setGeometry(this->geometry());

	QPushButton * start = new QPushButton("Start Creation");
	layout->addWidget(start, 0, 0, Qt::AlignCenter);
}



#endif