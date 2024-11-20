/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 11/20/2024
*/

#include "addCharacter.h"
#include "characterSelect.h"

#include <iostream>

#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDir>

void UpComboBox::showPopup()
{
	QComboBox::showPopup();

	QWidget *popup = this->findChild<QFrame *>();
	if (popup)
	{
		// Get global position of the QComboBox and adjust for the popup's height
		QPoint globalPos = this->mapToGlobal(QPoint(0, 0));
		int popupHeight = popup->height();

		// Move the popup above the combobox
		popup->move(globalPos.x(), globalPos.y() - popupHeight);
	}
}

Portrait::Portrait(const QString &type, const QString &selection, QWidget *parent) : QLabel(parent)
{
	// Determine the widget type and fetch a picture according to the combo box selection
	this->typeWidget = new QString(type);
	this->getImage(selection);
}

void Portrait::getImage(const QString &selection)
{
	// Attempt to retrieve a picture file from the assets folder based on the widget type and combo box selection
	QPixmap image(QDir::currentPath() + "/src/assets/" + *(this->typeWidget) + "/" + selection.toLower() + ".png");
	if (image.isNull())
	{
		this->setText("Image not available");
	}
	else
	{
		image = image.scaled(450, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		this->setPixmap(image);
	}
}

AddCharacter::AddCharacter(QWidget *parent) : QStackedWidget(parent)
{
	startWidget = new StartWidget();
	baseStatsWidget = new BaseStatsWidget();
	classWidget = new ClassWidget();
	raceWidget = new RaceWidget();
	backgroundWidget = new BackgroundWidget();
	inventoryWidget = new InventoryWidget();

	this->addWidget(startWidget);
	this->addWidget(baseStatsWidget);
	this->addWidget(classWidget);
	this->addWidget(raceWidget);
	this->addWidget(backgroundWidget);
	this->addWidget(inventoryWidget);

	this->setCurrentWidget(startWidget);
}

StartWidget::StartWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create the form layout
	QWidget *form = new QWidget();
	QFormLayout *formLayout = new QFormLayout(form);
	formLayout->setFormAlignment(Qt::AlignCenter);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create text box for the character name
	name = new QLineEdit();
	name->setFixedWidth(100);

	QWidget *nameContainer = new QWidget();
	QHBoxLayout *nameLayout = new QHBoxLayout(nameContainer);
	nameLayout->addWidget(new QLabel("Character Name:"));
	nameLayout->addWidget(name);

	// Create error message for invalid character name
	QLabel *errorLabel = new QLabel();
	errorLabel->setStyleSheet("QLabel { color : red; }");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back to Character Select");
	QPushButton *nextButton = new QPushButton("Next");

	// Add character name input to the form and also add the error label
	formLayout->addWidget(nameContainer);
	formLayout->addWidget(errorLabel);

	// Add buttons to navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(nextButton);

	// Disable the next button by default
	nextButton->setEnabled(false);

	// Add the navbar and form to the main layout
	layout->addWidget(form, 0, Qt::AlignCenter);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));

	// When the character name is invalid, display an error message
	connect(name, &QLineEdit::textChanged, this, [this, nextButton, errorLabel](const QString &text)
			{
				QString name = text.trimmed(); // Remove leading and trailing whitespace

				QDir characterDir(QDir::currentPath() + "/data/characters"); // Directory for character files

				if (name.isEmpty()) // Check if the character name is empty
				{
					errorLabel->setText("Character name cannot be empty");
				}
				else // Check if the character name already exists
				{
					bool nameExists = false;
					QStringList existingNames = characterDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot); // Get list of existing character names
					for (QString existingName : existingNames)											   // For each existing character name
					{
						if (QString::compare(existingName, name, Qt::CaseInsensitive) == 0) // Check if the existing name matches the proposed name (case-insensitive)
						{
							nameExists = true; // Set nameExists to true
							break;
						}
					}
					if (nameExists) // If the name already exists
					{
						errorLabel->setText("Character name already exists"); // Display an error message
					}
					else
					{
						errorLabel->clear(); // Clear the error message if there are no errors
					}
				}

				nextButton->setEnabled(errorLabel->text().isEmpty()); // Enable the next button if there are no errors
			});
}

void StartWidget::backPage()
{
	QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget()->parentWidget());
	if (mainStackedWidget)
	{
		mainStackedWidget->setCurrentIndex(0);
	}
}

void StartWidget::nextPage()
{
	QString updatedName = name->text().trimmed(); // Remove leading and trailing whitespace
	updatedName[0] = updatedName[0].toUpper();	  // Capitalize the first letter of the name
	name->setText(updatedName);					  // Update the name in the text box

	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget()); // Get the parent stacked widget
	if (stackedWidget)																	  // If the parent widget is a stacked widget
	{
		stackedWidget->setCurrentIndex(1); // Move to the next page
	}
}

BaseStatsWidget::BaseStatsWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create the form layout
	QWidget *form = new QWidget();
	QFormLayout *formLayout = new QFormLayout(form);
	formLayout->setFormAlignment(Qt::AlignCenter);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create the stats spin boxes
	strengthVal = new QSpinBox;
	strengthVal->setRange(3, 18);
	strengthVal->setFixedWidth(50);

	dexterityVal = new QSpinBox;
	dexterityVal->setRange(3, 18);
	dexterityVal->setFixedWidth(50);

	constitutionVal = new QSpinBox;
	constitutionVal->setRange(3, 18);
	constitutionVal->setFixedWidth(50);

	intelligenceVal = new QSpinBox;
	intelligenceVal->setRange(3, 18);
	intelligenceVal->setFixedWidth(50);

	wisdomVal = new QSpinBox;
	wisdomVal->setRange(3, 18);
	wisdomVal->setFixedWidth(50);

	charismaVal = new QSpinBox;
	charismaVal->setRange(3, 18);
	charismaVal->setFixedWidth(50);

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add spinboxes to the form
	formLayout->addRow("Strength:", strengthVal);
	formLayout->addRow("Dexterity:", dexterityVal);
	formLayout->addRow("Constitution:", constitutionVal);
	formLayout->addRow("Intelligence:", intelligenceVal);
	formLayout->addRow("Wisdom:", wisdomVal);
	formLayout->addRow("Charisma:", charismaVal);

	// Add buttons to navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(nextButton);

	// Add the navbar and form to the main layout
	layout->addWidget(form);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));

	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void BaseStatsWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(0);
	}
}

void BaseStatsWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}

ClassWidget::ClassWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create horizontal layer for the columns
	QWidget *body = new QWidget();
	QHBoxLayout *bodyLayout = new QHBoxLayout(body);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create the class combo box
	UpComboBox *classComboBox = new UpComboBox;
	classComboBox->addItem("Barbarian");
	classComboBox->addItem("Bard");
	classComboBox->addItem("Cleric");
	classComboBox->addItem("Druid");
	classComboBox->addItem("Fighter");
	classComboBox->addItem("Monk");
	classComboBox->addItem("Paladin");
	classComboBox->addItem("Ranger");
	classComboBox->addItem("Rogue");
	classComboBox->addItem("Sorcerer");
	classComboBox->addItem("Warlock");
	classComboBox->addItem("Wizard");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(classComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the navbar to the main layout
	layout->addWidget(body);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void ClassWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(1);
	}
}

void ClassWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3);
	}
}

RaceWidget::RaceWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create horizontal layer for the columns
	QWidget *body = new QWidget();
	QHBoxLayout *bodyLayout = new QHBoxLayout(body);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create the race combo box
	UpComboBox *raceComboBox = new UpComboBox;
	raceComboBox->addItem("Dwarf");
	raceComboBox->addItem("Elf");
	raceComboBox->addItem("Halfling");
	raceComboBox->addItem("Human");
	raceComboBox->addItem("Dragonborn");
	raceComboBox->addItem("Gnome");
	raceComboBox->addItem("Half Elf");
	raceComboBox->addItem("Half Orc");
	raceComboBox->addItem("Tiefling");

	// Create the race header
	QLabel *header = new QLabel("<h1>" + raceComboBox->currentText() + "</h1>");
	header->setFixedHeight(50);
	header->setAlignment(Qt::AlignCenter);

	// Create the race portrait
	Portrait *racePortrait = new Portrait("races", raceComboBox->currentText());

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(raceComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the portrait to the horizontal layer
	bodyLayout->addSpacing(100);
	bodyLayout->addWidget(racePortrait);

	// Add the header, body, and navbar to the main layout
	layout->addWidget(header);
	layout->addWidget(body);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));

	// When the current selection in the combo box changes, the header and portrait must also change
	connect(raceComboBox, &QComboBox::currentTextChanged, [header](const QString &text)
			{ header->setText("<h1>" + text + "</h1>"); });
	connect(raceComboBox, &QComboBox::currentTextChanged, racePortrait, &Portrait::getImage);
}

void RaceWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}

void RaceWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create horizontal layer for the columns
	QWidget *body = new QWidget();
	QHBoxLayout *bodyLayout = new QHBoxLayout(body);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create combo box for the background
	UpComboBox *backgroundComboBox = new UpComboBox;
	backgroundComboBox->addItem("Acolyte");
	backgroundComboBox->addItem("Charlatan");
	backgroundComboBox->addItem("Criminal");
	backgroundComboBox->addItem("Entertainer");
	backgroundComboBox->addItem("Folk Hero");
	backgroundComboBox->addItem("Guild Artisan");
	backgroundComboBox->addItem("Hermit");
	backgroundComboBox->addItem("Noble");
	backgroundComboBox->addItem("Outlander");
	backgroundComboBox->addItem("Sage");
	backgroundComboBox->addItem("Sailor");
	backgroundComboBox->addItem("Soldier");
	backgroundComboBox->addItem("Urchin");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(backgroundComboBox);
	navbarLayout->addWidget(nextButton);

	// Add the body and navbar to the main layout
	layout->addWidget(body);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void BackgroundWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3);
	}
}

void BackgroundWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(5);
	}
}

InventoryWidget::InventoryWidget(QWidget *parent) : QWidget(parent)
{
	// Create the main vertical layout
	QVBoxLayout *layout = new QVBoxLayout(this);

	// Create the layout of the inventory
	QWidget *inventory = new QWidget();
	QGridLayout *inventoryLayout = new QGridLayout(inventory);

	// Create and configure layout for the navbar
	QWidget *navbar = new QWidget();
	QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create inventory elements
	QSpacerItem *spacer = new QSpacerItem(100, 100);
	QListWidget *items = new QListWidget();
	QPushButton *addItem = new QPushButton("Add Item");
	QPushButton *removeItem = new QPushButton("Remove Selected Item");

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *finishButton = new QPushButton("Finish");

	// Add inventory elements to the body
	inventoryLayout->addItem(spacer, 0, 3);
	inventoryLayout->addItem(spacer, 3, 3);
	inventoryLayout->addWidget(items, 2, 2, 1, 3);
	inventoryLayout->addWidget(addItem, 1, 1, 1, 3, Qt::AlignCenter);
	inventoryLayout->addWidget(removeItem, 1, 3, 1, 3, Qt::AlignCenter);

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(finishButton);

	// Add the body and navbar to the main layout
	layout->addWidget(inventory);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	connect(finishButton, SIGNAL(clicked()), SLOT(nextPage()));
}

void InventoryWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

void InventoryWidget::nextPage()
{
	AddCharacter *addCharacterWidget = qobject_cast<AddCharacter *>(this->parentWidget());
	if (addCharacterWidget)
	{
		addCharacterWidget->setCurrentIndex(0);
	}
	QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(addCharacterWidget->parentWidget());
	if (mainStackedWidget)
	{
		mainStackedWidget->setCurrentIndex(0);
	}
	// Need to somehow get access to the SelectCharacterWidget to call addCharacter(QString name)
	// Alternatively we could move addCharacter to this file and have the QListWidget characters auto-upadte
}
