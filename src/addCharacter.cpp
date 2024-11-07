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