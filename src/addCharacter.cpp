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
#include <QLineEdit>
#include <QLayout>
#include <QLabel>

AddCharacter::AddCharacter(QWidget * parent) : QWidget(parent) {
	QGridLayout * layout = new QGridLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setGeometry(this->geometry());

	QWidget * startWidget = new QWidget();
	startWidget->setGeometry(this->geometry());
	layout->addWidget(startWidget, 0, 0, Qt::AlignCenter);

	QGridLayout * startLayout = new QGridLayout(startWidget);
	startLayout->setGeometry(startWidget->geometry());
	// layout->addLayout(startLayout, 0, 0);

	QPushButton * start = new QPushButton("Start Creation");
	startLayout->addWidget(start, 0, 0, Qt::AlignCenter);

	startWidget->hide();

	QWidget * baseStatsWidget = new QWidget();
	baseStatsWidget->setGeometry(this->geometry());
	layout->addWidget(baseStatsWidget, 0, 0);

	QGridLayout * baseStatsLayout = new QGridLayout(baseStatsWidget);
	baseStatsLayout->setContentsMargins(0, 0, 0, 0);
	baseStatsLayout->setGeometry(this->geometry());

	QSpacerItem * spacer = new QSpacerItem(this->width()/5, 0);
	baseStatsLayout->addItem(spacer, 0, 5);

	QLabel * strengthLabel = new QLabel("Stregnth");
	QLabel * dexterityLabel = new QLabel("Dexterity");
	QLabel * constitutionLabel = new QLabel("Constitution");
	QLabel * intelligenceLabel = new QLabel("Intelligence");
	QLabel * wisdomLabel = new QLabel("Wisdom");
	QLabel * charismaLabel = new QLabel("Charisma");
	baseStatsLayout->addWidget(strengthLabel, 0, 0, Qt::AlignLeft);
	baseStatsLayout->addWidget(dexterityLabel, 1, 0, Qt::AlignLeft);
	baseStatsLayout->addWidget(constitutionLabel, 2, 0, Qt::AlignLeft);
	baseStatsLayout->addWidget(intelligenceLabel, 3, 0, Qt::AlignLeft);
	baseStatsLayout->addWidget(wisdomLabel, 4, 0, Qt::AlignLeft);
	baseStatsLayout->addWidget(charismaLabel, 5, 0, Qt::AlignLeft);

	QLineEdit * strengthVal = new QLineEdit;
	QLineEdit * dexterityVal = new QLineEdit;
	QLineEdit * constitutionVal = new QLineEdit;
	QLineEdit * intelligenceVal = new QLineEdit;
	QLineEdit * wisdomVal = new QLineEdit;
	QLineEdit * charismaVal = new QLineEdit;
	baseStatsLayout->addWidget(strengthVal, 0, 1, Qt::AlignLeft);
	baseStatsLayout->addWidget(dexterityVal, 1, 1, Qt::AlignLeft);
	baseStatsLayout->addWidget(constitutionVal, 2, 1, Qt::AlignLeft);
	baseStatsLayout->addWidget(intelligenceVal, 3, 1, Qt::AlignLeft);
	baseStatsLayout->addWidget(wisdomVal, 4, 1, Qt::AlignLeft);
	baseStatsLayout->addWidget(charismaVal, 5, 1, Qt::AlignLeft);

}



#endif