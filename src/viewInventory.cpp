/*
Name: viewInventory.cpp
Description: Page to view a character's inventory.
Authors: Zachary Craig, Carson Treece
Other Sources: ...
Date Created: 11/5/2024
Last Modified: 12/4/2024
*/

#include "viewInventory.h"
#include "themeUtils.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QList>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QSpinBox>

void ViewInventory::loadInventory()
{
    int selectedIndex = inventoryList->currentRow();

    QString characterName = inventoryLabel->text().remove("'s Inventory");
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/inventory.csv";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open inventory file for loading:" << filePath;
        return;
    }

    QTextStream in(&file);
    inventoryList->clear();

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList fields = line.split(",");
        if (fields.size() < 4) {
            qWarning() << "Invalid inventory line:" << line;
            continue; // Skip invalid lines
        }

        QString itemName = fields[0];
        int quantity = fields[1].toInt();
        int equipped = fields[2].toInt();
        int attunement = fields[3].toInt();

        if (attunement == 1 && equipped == 1) {
            itemName.append(" [Equipped & Attuned]");
        } else if (attunement == 1) {
            itemName.append(" [Attuned]");
        } else if (equipped == 1) {
            itemName.append(" [Equipped]");
        }


        QListWidgetItem *item = new QListWidgetItem("(" + QString::number(quantity) + ") " + itemName);
        item->setData(Qt::UserRole, line); // Store full data string in UserRole
        inventoryList->addItem(item);
    }

    file.close();

    if (selectedIndex >= 0 && selectedIndex < inventoryList->count()) {
        inventoryList->setCurrentRow(selectedIndex); // Restore the selected index
    }
}


ViewInventory::ViewInventory(QWidget *parent, QString name) :
    QWidget(parent)
{
    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesn't take up half the screen

    // Create buttons to go back to the character page
    QPushButton *backToCharacter = new QPushButton("Return to Character");
    // Create header label with character's name
    inventoryLabel = new QLabel(name + "'s Inventory");

    // Add buttons to navbar
    navbarLayout->addWidget(backToCharacter);
    navbarLayout->addWidget(inventoryLabel, 1, Qt::AlignCenter);

    // Create the layout for the inventory page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create body layout
    QWidget *body = new QWidget();
    QHBoxLayout *bodyLayout = new QHBoxLayout(body); // Horizontal layout for splitting the view

    // Create the inventory list
    inventoryList = new QListWidget();
    inventoryList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Ensure it scales properly

    // Add buttons for managing inventory on the right side
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QPushButton *addItemButton = new QPushButton("Add Item");
    QPushButton *deleteItemButton = new QPushButton("Delete Item");
    QPushButton *increaseQuantityButton = new QPushButton("Increase Count");
    QPushButton *decreaseQuantityButton = new QPushButton("Decrease Count");
    QPushButton *equipItemButton = new QPushButton("Equip Item");
    QPushButton *attuneItemButton = new QPushButton("Attune Item");

    // Add buttons to the button layout
    buttonLayout->addWidget(addItemButton);
    buttonLayout->addWidget(deleteItemButton);
    buttonLayout->addWidget(increaseQuantityButton);
    buttonLayout->addWidget(decreaseQuantityButton);
    buttonLayout->addWidget(equipItemButton);
    buttonLayout->addWidget(attuneItemButton);
    buttonLayout->addStretch(); // Add a stretch at the bottom for spacing

    // Add the inventory list and buttons to the body layout
    bodyLayout->addWidget(inventoryList, 3); // Inventory list takes 75% of the space
    bodyLayout->addLayout(buttonLayout, 1); // Buttons take 25% of the space

    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));
    connect(deleteItemButton, &QPushButton::clicked, this, &ViewInventory::deleteSelectedItem);
    connect(increaseQuantityButton, &QPushButton::clicked, this, &ViewInventory::increaseItemQuantity);
    connect(decreaseQuantityButton, &QPushButton::clicked, this, &ViewInventory::decreaseItemQuantity);
    connect(addItemButton, &QPushButton::clicked, this, &ViewInventory::addItem);
    connect(equipItemButton, &QPushButton::clicked, this, &ViewInventory::equipItem);
    connect(attuneItemButton, &QPushButton::clicked, this, &ViewInventory::attuneItem);

    reloadTheme(); // Reload the theme after everything is placed

    loadInventory(); // Load the inventory when the page is initialized
}


void ViewInventory::goBack() {
    QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (mainStackedWidget) {
        mainStackedWidget->setCurrentIndex(0); // Switch to CharacterSelect (index 0)
    }
}

void ViewInventory::deleteSelectedItem() {

    // Clear selection and focus
    inventoryList->clearSelection();
    inventoryList->clearFocus();

    QListWidgetItem *selectedItem = inventoryList->currentItem();
    if (selectedItem) {
        // Create a confirmation popup
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, 
            "Confirm Delete", 
            "Are you sure you want to delete the selected item?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            // If user confirms, delete the item
            delete inventoryList->takeItem(inventoryList->row(selectedItem)); // Remove the item from the list
            saveInventory(); // Save the updated inventory
            QMessageBox::information(this, "Item Deleted", "The item has been successfully deleted.");
        }
    } else {
        QMessageBox::warning(this, "No Selection", "Please select an item to delete.");
    }
}


void ViewInventory::increaseItemQuantity()
{
    QListWidgetItem *item = inventoryList->currentItem();
    if (!item) return;

    QStringList fields = item->data(Qt::UserRole).toString().split(",");
    if (fields.size() < 4) return;

    int quantity = fields[1].toInt();
    fields[1] = QString::number(quantity + 1);

    item->setText(fields[0] + " (x" + fields[1] + ")");
    item->setData(Qt::UserRole, fields.join(",")); // Update UserRole data
    saveInventory(); // Save changes to CSV immediately
}

void ViewInventory::decreaseItemQuantity()
{
    QListWidgetItem *item = inventoryList->currentItem();
    if (!item) return;

    QStringList fields = item->data(Qt::UserRole).toString().split(",");
    if (fields.size() < 4) return;

    int quantity = fields[1].toInt();
    if (quantity > 1) {
        fields[1] = QString::number(quantity - 1);

        item->setText(fields[0] + " (x" + fields[1] + ")");
        item->setData(Qt::UserRole, fields.join(",")); // Update UserRole data
        saveInventory(); // Save changes to CSV immediately
    }
}



void ViewInventory::saveInventory()
{
    QString characterName = inventoryLabel->text().remove("'s Inventory");
    QString filePath = QDir::currentPath() + "/data/characters/" + characterName + "/inventory.csv";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open inventory file for saving:" << filePath;
        return;
    }

    QTextStream out(&file);

    for (int i = 0; i < inventoryList->count(); ++i) {
        QListWidgetItem *item = inventoryList->item(i);
        QString itemData = item->data(Qt::UserRole).toString(); // Retrieve full data
        out << itemData << "\n"; // Write each item's data to the CSV
    }
    
    file.close();
    
    loadInventory(); // Reload the inventory after saving
}

void ViewInventory::addItem() {

    // Clear selection and focus
    inventoryList->clearSelection();
    inventoryList->clearFocus();

    qDebug() << "Add Item";

    QDialog popup;
    popup.setWindowModality(Qt::ApplicationModal);
    popup.setWindowTitle("Add Item");
    
    QVBoxLayout layout(&popup);


    // make item label and textbox
    QLabel *itemNameLabel = new QLabel("Item Name:");
    QLineEdit *itemNameEdit = new QLineEdit();
    layout.addWidget(itemNameLabel);
    layout.addWidget(itemNameEdit);

    // make quantity label and spinbox
    QLabel *quantityLabel = new QLabel("Quantity:");
    QSpinBox *quantityEdit = new QSpinBox();
    quantityEdit->setMinimum(1);
    layout.addWidget(quantityLabel);
    layout.addWidget(quantityEdit);

    // add and cancel buttons
    QPushButton *addButton = new QPushButton("Add Item");
    QPushButton *cancelButton = new QPushButton("Cancel");

    // make widget for add and cancel buttons
    QWidget buttonWidget;
    buttonWidget.setFixedHeight(50);

    // make layout for add and cancel buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout(&buttonWidget);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(cancelButton);
    layout.addWidget(&buttonWidget, Qt::AlignBottom);

    // connect the buttons
    QObject::connect(addButton, &QPushButton::clicked, &popup, &QDialog::accept);
    QObject::connect(cancelButton, &QPushButton::clicked, &popup, &QDialog::reject);

    // show the popup
    popup.exec();

    // if the user clicks the add button
    if (popup.result() == QDialog::Accepted) {

        qDebug() << "Add Item";

        // get the item name and quantity
        QString itemName = itemNameEdit->text();
        int quantity = quantityEdit->value();

        // create the item string
        QListWidgetItem *item = new QListWidgetItem(itemName + " (x" + QString::number(quantity) + ")");
        item->setData(Qt::UserRole, itemName + "," + QString::number(quantity) + ",0,0");
        inventoryList->addItem(item);
    
        saveInventory();
    }

}

void ViewInventory::equipItem() {
    QListWidgetItem *item = inventoryList->currentItem();
    if (!item) return;

    QStringList fields = item->data(Qt::UserRole).toString().split(",");
    if (fields.size() < 4) return;

    int equipped = fields[2].toInt();
    int attunement = fields[3].toInt();

    if (equipped == 0) {
        fields[2] = "1";
        // QMessageBox::information(this, "Item Equipped", "Item has been equipped.");
    } else {
        fields[2] = "0";

        if (attunement == 1) {
            fields[3] = "0";
            // QMessageBox::information(this, "Item Unequipped and Unattuned", "Item was unequipped and is no longer attuned.");
        } else {
            // QMessageBox::information(this, "Item Unequipped", "Item has been unequipped.");
        }
    }

    item->setData(Qt::UserRole, fields.join(",")); // Update UserRole data
    saveInventory(); // Save changes to CSV immediately
}

void ViewInventory::attuneItem() {
    QListWidgetItem *item = inventoryList->currentItem();
    if (!item) return;

    QStringList fields = item->data(Qt::UserRole).toString().split(",");
    if (fields.size() < 4) return;

    int attunement = fields[3].toInt();
    int equipped = fields[2].toInt();
    int attunedItems = 0;

    if (equipped == 0) {
        QMessageBox::warning(this, "Item Not Equipped", "You must equip the item before attuning to it.");
        return;
    }

    if (attunement == 0) {
        for (int i = 0; i < inventoryList->count(); ++i) {
            QStringList itemFields = inventoryList->item(i)->data(Qt::UserRole).toString().split(",");
            if (itemFields.size() >= 4 && itemFields[3].toInt() == 1) {
                attunedItems++;
            }
        }

        if (attunedItems >= 3) {
            QMessageBox::warning(this, "Too Many Attuned Items", "You can only attune to 3 items at a time.");
            return;
        }

        fields[3] = QString::number(attunement == 0 ? 1 : 0);
        // QMessageBox::information(this, "Item Attuned", "Item has been attuned.");

    } else {
        fields[3] = "0";
        // QMessageBox::information(this, "Item Unattuned", "Item has been unattuned.");
    }

    item->setData(Qt::UserRole, fields.join(",")); // Update UserRole data
    saveInventory(); // Save changes to CSV immediately
}


ViewInventory::~ViewInventory() {

}
