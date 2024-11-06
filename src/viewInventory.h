/*
Name: viewInventory.h
Description: Page to view a character's inventory.
Authors: Zachary Craig
Other Sources: ...
Date Created: 11/5/2024
Last Modified: 11/5/2024
*/

#ifndef VIEWINVENTORY_H
#define VIEWINVENTORY_H

#include <QWidget>

class ViewInventory : public QWidget
{
public:
    explicit ViewInventory(QWidget *parent = 0);
    ~ViewInventory();

private:
    void loadInventory();
};

#endif
