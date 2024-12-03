/*
Name: centeredCheckBox.cpp
Description: Centered QCheckBox class to make other code look simpler
Authors: Josh Park
Other Sources: ...
Date Created: 12/3/2024
Last Modified: 12/3/2024
*/

#include "centeredCheckBox.h"

#include <QLayout>

CenteredCheckBox::CenteredCheckBox(QWidget * parent)
: QCheckBox(parent) {
	pWidget = new QWidget();
	QHBoxLayout * pLayout = new QHBoxLayout(pWidget);
	pLayout->addWidget(this);
	pLayout->setAlignment(Qt::AlignCenter);
	pLayout->setContentsMargins(0,0,0,0);
	pWidget->setLayout(pLayout);
}