/*
Name: centeredCheckBox.h
Description: Centered QCheckBox class to make other code look simpler
Authors: Josh Park
Other Sources: ...
Date Created: 12/3/2024
Last Modified: 12/3/2024
*/

#ifndef CENTERED_CHECK_BOX_H
#define CENTERED_CHECK_BOX_H

#include <QCheckBox>

class CenteredCheckBox : public QCheckBox {
	Q_OBJECT
public:
	CenteredCheckBox(QWidget * parent = 0);
	QWidget * getLayoutWidget() { return this->pWidget; }
private:
	QWidget * pWidget;
};

#endif