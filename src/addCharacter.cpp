/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/25/2024
*/

#include "addCharacter.h"

#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>
#include <QLabel>

AddCharacter::AddCharacter(QWidget * parent) :
	QStackedWidget(parent)
{
	StartWidget * startWidget = new StartWidget();
	BaseStatsWidget * baseStatsWidget = new BaseStatsWidget();

	this->addWidget(startWidget);
	this->addWidget(baseStatsWidget);

	this->setCurrentWidget(startWidget);
}

StartWidget::StartWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	QPushButton * start = new QPushButton("Start Creation");
	layout->addWidget(start, 0, 0, Qt::AlignCenter);

	connect(start, &QPushButton::clicked, [=]{
		QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
		stackedWidget->setCurrentIndex(1);
	});
}

BaseStatsWidget::BaseStatsWidget(QWidget * parent) :
	QWidget(parent)
{
	QGridLayout * layout = new QGridLayout(this);

	QLabel * strengthLabel = new QLabel("Stregnth");
	QLabel * dexterityLabel = new QLabel("Dexterity");
	QLabel * constitutionLabel = new QLabel("Constitution");
	QLabel * intelligenceLabel = new QLabel("Intelligence");
	QLabel * wisdomLabel = new QLabel("Wisdom");
	QLabel * charismaLabel = new QLabel("Charisma");
	layout->addWidget(strengthLabel, 0, 0, Qt::AlignRight);
	layout->addWidget(dexterityLabel, 1, 0, Qt::AlignRight);
	layout->addWidget(constitutionLabel, 2, 0, Qt::AlignRight);
	layout->addWidget(intelligenceLabel, 3, 0, Qt::AlignRight);
	layout->addWidget(wisdomLabel, 4, 0, Qt::AlignRight);
	layout->addWidget(charismaLabel, 5, 0, Qt::AlignRight);

	QLineEdit * strengthVal = new QLineEdit;
	QLineEdit * dexterityVal = new QLineEdit;
	QLineEdit * constitutionVal = new QLineEdit;
	QLineEdit * intelligenceVal = new QLineEdit;
	QLineEdit * wisdomVal = new QLineEdit;
	QLineEdit * charismaVal = new QLineEdit;
	layout->addWidget(strengthVal, 0, 1, Qt::AlignLeft);
	layout->addWidget(dexterityVal, 1, 1, Qt::AlignLeft);
	layout->addWidget(constitutionVal, 2, 1, Qt::AlignLeft);
	layout->addWidget(intelligenceVal, 3, 1, Qt::AlignLeft);
	layout->addWidget(wisdomVal, 4, 1, Qt::AlignLeft);
	layout->addWidget(charismaVal, 5, 1, Qt::AlignLeft);
}