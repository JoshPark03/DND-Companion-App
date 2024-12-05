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

void ViewInventory::loadInventory()
{
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

        QListWidgetItem *item = new QListWidgetItem(itemName + " (x" + QString::number(quantity) + ")");
        item->setData(Qt::UserRole, line); // Store full data string in UserRole
        inventoryList->addItem(item);
    }

    file.close();
}


ViewInventory::ViewInventory(QWidget *parent, QString name) :
    QWidget(parent)
{
    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesnt take up half the screen

    // Create buttons to go back to the character page
    QPushButton *backToCharacter = new QPushButton("Return to Character");
    // Create header label with character's name
    inventoryLabel = new QLabel(name + "'s Inventory");

    // Add buttons to navbar
    navbarLayout->addWidget(backToCharacter);
    navbarLayout->addWidget(inventoryLabel, 1, Qt::AlignCenter);

    // Create the layout for the inventory page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create body widget
    QWidget *body = new QWidget();
    QVBoxLayout *bodyLayout = new QVBoxLayout(body);

    // Create the inventory list
    inventoryList = new QListWidget();

    // Add buttons for managing inventory
    QHBoxLayout *inventoryButtons = new QHBoxLayout();
    QPushButton *deleteItemButton = new QPushButton("Delete Item");
    QPushButton *increaseQuantityButton = new QPushButton("Increase Count");
    QPushButton *decreaseQuantityButton = new QPushButton("Decrease Count");

    inventoryButtons->addWidget(deleteItemButton);
    inventoryButtons->addWidget(increaseQuantityButton);
    inventoryButtons->addWidget(decreaseQuantityButton);

    // Add widgets to the body layout
    bodyLayout->addWidget(inventoryList);
    bodyLayout->addLayout(inventoryButtons);

    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));
    connect(deleteItemButton, &QPushButton::clicked, this, &ViewInventory::deleteSelectedItem);
    connect(increaseQuantityButton, &QPushButton::clicked, this, &ViewInventory::increaseItemQuantity);
    connect(decreaseQuantityButton, &QPushButton::clicked, this, &ViewInventory::decreaseItemQuantity);

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
    QListWidgetItem *selectedItem = inventoryList->currentItem();
    if (selectedItem) {
        delete inventoryList->takeItem(inventoryList->row(selectedItem)); // Remove the item from the list
        saveInventory(); // Save the updated inventory
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
}



ViewInventory::~ViewInventory() {

}
