/*
Name: characterSelect.cpp
Description: Main landing page for the application, allowing users to create, delete, and manage characters.
			 Also provides navigation to settings.
Authors: Carson Treece, Zachary Craig, Josh Park
Other Sources: ...
Date Created: 10/22/2024
Last Modified: 11/6/2024
*/

#include "characterSelect.h"
#include "viewCharacter.h"
#include "viewInventory.h"
#include "viewSpells.h"
#include "viewNotes.h"
#include "themeUtils.h"

#include <iostream>
#include <string>

#include <QStackedWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QListWidget>
#include <QWidget>
#include <QLayout>
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QDir>

// Add a new character to the list
void CharacterSelect::addCharacter(QString charName)
{
	// Get character name from the user
	bool ok = true;
	if (charName.isEmpty())
	{
		charName = QInputDialog::getText(this, tr("Create New Character"),
										 tr("Character Name:"), QLineEdit::Normal,
										 "", &ok);
	}

	// Check if ok is clicked and the name is not empty
	if (ok && !charName.isEmpty())
	{
		// Path to the characters directory
		QString charPath = QDir::currentPath() + "/data/characters/" + charName;

		// Check if the directory already exists
		QDir dir;
		if (dir.exists(charPath))
		{
			QMessageBox::warning(this, "Character Exists",
								 "A character with this name already exists!"); // This is a warning message if the character already exists
		}
		else
		{
			// Create the character directory
			if (dir.mkpath(charPath))
			{
				// Create the notes and databases files inside the folder
				QFile notesFile(charPath + "/notes.json"); // Create a notes file
				if (notesFile.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QTextStream out(&notesFile);
					out << "Character Notes"; // Add default content to notes.txt
					notesFile.close();		  // Close the file
				}

				// Add character to the list
				std::cout << "before addItem(charName);, charName = " << charName.toStdString() << std::endl;
				this->characters->addItem(charName);
				std::cout << "after addItem(charName);" << std::endl;
			}
			else
			{
				QMessageBox::warning(this, "Error", "Failed to create character directory."); // This is a warning message if the directory creation fails
			}
		}
	}
	std::cout << "End of addCharacter" << std::endl;
}

// Delete the selected character
void CharacterSelect::deleteCharacter()
{
	QListWidgetItem *item = this->characters->currentItem();
	if (item != nullptr)
	{
		// gets the name of the character
		QString name = item->text();

		// creates popup for the user to confirm the deletion of their character
		QDialog popup;

		popup.setWindowModality(Qt::ApplicationModal); // stios the user from interacting with the main window while the popup is open
		popup.setWindowTitle("Delete Character?");	   // set the title of the popup

		popup.setFixedSize(200, 150); // set the size of the popup

		QLabel *popupText = new QLabel("Are you sure you want to delete " + name + "?", &popup); // creates the text for the popup
		popupText->setWordWrap(true);															 // allows the text to wrap around if it is too long

		// create the buttons for the popup
		QPushButton *popupConfirm = new QPushButton("Confirm", &popup); // creates the confirm button
		QPushButton *popupCancel = new QPushButton("Cancel", &popup);	// creates the cancel button

		// Gets rid of the padding around the text of the buttons
		popupConfirm->setStyleSheet("padding: 5px 0px; margin: 0px;");
		popupCancel->setStyleSheet("padding: 5px 0px; margin: 0px;");

		// Connect the confirm button to accept the dialog (e.g., confirming the deletion)
		QObject::connect(popupConfirm, &QPushButton::clicked, &popup, &QDialog::accept);

		// Connect the cancel button to reject the dialog (e.g., canceling the deletion)
		QObject::connect(popupCancel, &QPushButton::clicked, &popup, &QDialog::reject);

		// create the layout for the popup
		QVBoxLayout popupLayout(&popup);

		// Create button widget
		QWidget *popupButtonWidget = new QWidget(&popup);
		QHBoxLayout popupButtonLayout(popupButtonWidget); // Makes the buttons horizontal

		// adds the buttons to the button widget
		popupButtonLayout.addWidget(popupConfirm);
		popupButtonLayout.addWidget(popupCancel);

		// adds the components to the popup
		popupLayout.addWidget(popupText);
		popupLayout.addWidget(popupButtonWidget);

		// makes the popup text take up more horizontal space
		popupLayout.setStretch(0, 1);

		// show the popup
		popup.exec();

		// if the user confirms the deletion
		if (popup.result() == QDialog::Accepted)
		{
			// removes the character from the list
			// this->characters->removeItemWidget(item);

			QString charName = item->text(); // Get the name of the character
			QString charPath = QDir::currentPath() + "/data/characters/" + charName;

			QDir charDir(charPath);
			if (charDir.removeRecursively())
			{ // Remove the folder and its contents
				// Remove the character from the UI list
				// delete this->characters->takeItem(this->characters->row(item));
				delete item;

				QMessageBox::information(this, "Character Deleted", "Character " + charName + " was deleted successfully."); // This is a message box that appears when the character is deleted
			}
			else
			{
				QMessageBox::warning(this, "Deletion Failed", "Failed to delete character " + charName + "."); // This is a warning message if the character deletion fails
			}
		}
	}

	// clear the selection
	// this is so that no character is selected after deletion
	this->characters->selectionModel()->clear();
}

// Load the list of characters from the characters directory
void CharacterSelect::loadCharacterList()
{

	// Path to the characters directory
	QString charDirPath = QDir::currentPath() + "/data/characters";
	QDir charDir(charDirPath);

	// Check if the directory exists
	if (charDir.exists())
	{
		// Get the list of directories in the characters folder
		// Because the name of each folder is the name of the character
		QStringList characterFolders = charDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

		// Add each character to the list
		for (const QString &charFolder : characterFolders)
		{
			this->characters->addItem(charFolder);
		}

		// Shows a message if no characters have been created
		if (this->characters->count() == 0)
		{
			this->characters->addItem("No Characters Have been created");
		}
	}
	else
	{
		// Make sure the characters directory exists
		QMessageBox::warning(this, "Error", "Characters directory does not exist.");
	}
}

CharacterSelect::CharacterSelect(QWidget *parent)
	: QWidget(parent)
{
	// TODO SQL or function call to get list of characters

	// Layout object for automatically centering and placing widgets
	layout = new QGridLayout(this);

	// spcer item for spacing
	QSpacerItem *spacer = new QSpacerItem(this->width(), this->height());
	layout->addItem(spacer, 0, 0, 100, 100);

	// button for character creation

	this->createChar = new QPushButton("Create Character");
	layout->addWidget(createChar, 5, 8, 5, 10);

	// button for settings
	this->settings = new QPushButton("Settings");
	// layout->addWidget(settings, 5, 82, 5, 10);
	layout->addWidget(settings, 15, 8, 5, 10);

	// List of all of the characters
	this->characters = new QListWidget();
	// layout->addWidget(characters, 10, 20, 80, 60);
	layout->addWidget(characters, 5, 30, 80, 60);

	// button to delete selected character
	this->deleteChar = new QPushButton("Delete Character", this);
	// layout->addWidget(this->deleteChar, 87, 20, 10, 60);
	layout->addWidget(this->deleteChar, 10, 8, 5, 10);

	// set the delete button to be disabled by default
	this->deleteChar->setEnabled(false);

	loadCharacterList();

	/*
		THE AREA BELOW SHOULD BE USED FOR THE CONNECT FUNCTIONS IE. EVENT HANDLERS
	*/

	// delete character button click event
	connect(this->deleteChar, SIGNAL(clicked()), SLOT(deleteCharSlot()));

	// enable the delete button when a character is clicked
	connect(this->characters, SIGNAL(itemClicked(QListWidgetItem *)), SLOT(selectChar()));

	// double click event to view character
	connect(this->characters, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(openChar()));

	// create character button click event
	connect(this->createChar, SIGNAL(clicked()), SLOT(gotoAddCharacter()));

	// settings button click event
	connect(this->settings, SIGNAL(clicked()), SLOT(gotoSettings()));
}

void CharacterSelect::deleteCharSlot()
{
	// call the delete character function
	this->deleteCharacter();

	// if there are no characters left, add a message to the list
	if (characters->count() == 0)
	{
		QListWidgetItem *nochars = new QListWidgetItem("No Characters Have been created");
		this->characters->addItem(nochars);
	}

	// disable delete button after character deletion
	this->deleteChar->setEnabled(false);
}

void CharacterSelect::selectChar()
{
	this->deleteChar->setEnabled(true);
}

void CharacterSelect::openChar()
{
	// get the name of the character
	QString name = characters->currentItem()->text();

	// clear the selection and focus after getting character name
	this->characters->selectionModel()->clear();
	this->characters->clearFocus();

	// reload the theme
	reloadTheme();

	// Start the process of creating the viewCharacter page and switching to it
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		if(!stackedWidget->widget(2))
		{
			qDebug() << "No characterInformation stack found";
			return;
		}
		// get the characterInformation stack
		QStackedWidget * characterInformation = qobject_cast<QStackedWidget *>(stackedWidget->widget(2));

		// Loop through all widgets and delete them
    	while (characterInformation->count() > 0) 
		{
			QWidget *widget = characterInformation->widget(0); // Always get the first widget
			characterInformation->removeWidget(widget); // Remove it from the stack
			delete widget; // Delete the widget
    	}
		

		// Create the viewCharacter page, viewInventory page, and viewNotes page
		ViewCharacter *newViewCharacter = new ViewCharacter(nullptr, name);
		ViewInventory *newViewInventory = new ViewInventory(nullptr, name);
		ViewSpells *newViewSpells = new ViewSpells(nullptr, name);
		ViewNotes *newViewNotes = new ViewNotes(nullptr, name);

		// Add the viewCharacter page, viewInventory page, and viewNotes page to the stacked widget
		characterInformation->addWidget(newViewCharacter);
		characterInformation->addWidget(newViewInventory);
		characterInformation->addWidget(newViewSpells);
		characterInformation->addWidget(newViewNotes);


		stackedWidget->setCurrentIndex(2); // viewCharacter in the characterInformation stack which is on index 2
	}

	// disable delete button since itemClicked collides with itemDoubleClicked
	deleteChar->setEnabled(false);
}

void CharacterSelect::gotoAddCharacter()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(1); // character select is the first page so index 0
	}
}

void CharacterSelect::gotoSettings()
{
	// find the parent stacked widget and switch to settings page
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3); // settings is the fourth page so index 3
	}
}