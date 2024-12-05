/*
Name: viewInventory.h
Description: Page to view a character's inventory.
Authors: Zachary Craig, Carson Treece
Other Sources: ...
Date Created: 11/5/2024
Last Modified: 12/4/2024
*/

#ifndef VIEWINVENTORY_H
#define VIEWINVENTORY_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>

class ViewInventory : public QWidget
{
    Q_OBJECT
public:
    explicit ViewInventory(QWidget *parent = 0, QString name = "");
    ~ViewInventory();

private:
    QLabel *inventoryLabel; // Label for the character's inventory name
    QListWidget *inventoryList; // List widget for displaying inventory items
    void loadInventory(); // Load inventory from file
    void saveInventory(); // Save inventory to file

private slots:
    void goBack(); // Navigate back to the previous screen
    void deleteSelectedItem(); // Delete the selected item from the inventory
    void increaseItemQuantity(); // Increase the quantity of the selected item
    void decreaseItemQuantity(); // Decrease the quantity of the selected item
};

#endif
