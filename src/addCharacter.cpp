/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 11/5/2024
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

void MyComboBox::showPopup()
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
	QPixmap image(QDir::currentPath() + "/src/assets/" + *(this->typeWidget) + "/" + selection + ".png", "-d");
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

/**
 * Constructor for the class
 */
AddCharacter::AddCharacter(QWidget *parent) : QStackedWidget(parent)
{
	// defining all of the custom widgets
	startWidget = new StartWidget();
	baseStatsWidget = new BaseStatsWidget();
	classWidget = new ClassWidget();
	raceWidget = new RaceWidget();
	backgroundWidget = new BackgroundWidget();
	inventoryWidget = new InventoryWidget();

	// adding all of the custom widgets to the stacked widget
	this->addWidget(startWidget);
	this->addWidget(baseStatsWidget);
	this->addWidget(classWidget);
	this->addWidget(raceWidget);
	this->addWidget(backgroundWidget);
	this->addWidget(inventoryWidget);

	// seting the current widget to the startWidget
	this->setCurrentWidget(startWidget);
}

/**
 * Constructor for the class
 */
StartWidget::StartWidget(QWidget * parent) :
	QWidget(parent)
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

/**
 * This function changes mainStackedWidget to SelectWidget
 */
void StartWidget::backPage()
{
	QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget()->parentWidget());
	if (mainStackedWidget)
	{
		mainStackedWidget->setCurrentIndex(0);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to BaseStatsWidget
 */
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

/**
 * Constructor for the class
 */
BaseStatsWidget::BaseStatsWidget(QWidget * parent) :
	QWidget(parent)
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

/**
 * This function changes AddCharacter's StackedWidget to StartWidget
 */
void BaseStatsWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(0);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to ClassWidget
 */
void BaseStatsWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}

/**
 * Constructor for the class
 */
ClassWidget::ClassWidget(QWidget * parent) :
	QWidget(parent) {
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
	MyComboBox *classComboBox = new MyComboBox;
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

/**
 * This function changes AddCharacter's StackedWidget to BaseStatsWidget
 */
void ClassWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(1);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to RaceWidget
 */
void ClassWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3);
	}
}

/**
 * Constructor for the class
 */
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
	MyComboBox *raceComboBox = new MyComboBox;
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

/**
 * This function changes AddCharacter's StackedWidget to ClassWidget
 */
void RaceWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to BackgroundWidget
 */
void RaceWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

/**
 * Constructor for the class
 */
BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent) {
    // Create the main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);

	// Creating out labels, styles, and alignments
	nameAndPageLabel = new QLabel();
	nameAndPageLabel->setStyleSheet("font-weight: bold; font-size: 18px; margin: 0; padding: 0;");
	nameAndPageLabel->setAlignment(Qt::AlignCenter);

	descriptionLabel = new QLabel();
	descriptionLabel->setStyleSheet("font-size: 14px; margin: 0; padding: 0;");
	descriptionLabel->setAlignment(Qt::AlignCenter);
	descriptionLabel->setWordWrap(true);

	proficienciesLabel = new QLabel();
	proficienciesLabel->setAlignment(Qt::AlignCenter);
	proficienciesLabel->setWordWrap(true);

	equipmentLabel = new QLabel();
	equipmentLabel->setAlignment(Qt::AlignCenter);
	equipmentLabel->setWordWrap(true);

	featureLabel = new QLabel();
	featureLabel->setAlignment(Qt::AlignCenter);
	featureLabel->setWordWrap(true);

	// Create column headers
    QLabel *proficienciesHeader = new QLabel("Proficiencies");
    proficienciesHeader->setAlignment(Qt::AlignCenter);
    proficienciesHeader->setStyleSheet("font-weight: bold; font-size: 14px;");

    QLabel *equipmentHeader = new QLabel("Equipment");
    equipmentHeader->setAlignment(Qt::AlignCenter);
    equipmentHeader->setStyleSheet("font-weight: bold; font-size: 14px;");

    QLabel *featureHeader = new QLabel("Feature");
    featureHeader->setAlignment(Qt::AlignCenter);
    featureHeader->setStyleSheet("font-weight: bold; font-size: 14px;");

    // Add Name (Page #) to the top
    mainLayout->addWidget(nameAndPageLabel, 0, Qt::AlignTop);

    // Add Description
    mainLayout->addWidget(descriptionLabel, 0, Qt::AlignTop);

    // Create a layout for the three columns
    QHBoxLayout *columnsLayout = new QHBoxLayout();
	

    // Proficiencies Column
    QVBoxLayout *proficienciesLayout = new QVBoxLayout();
    proficienciesLayout->addWidget(proficienciesHeader);
    proficienciesLayout->addWidget(proficienciesLabel);

    // Equipment Column
    QVBoxLayout *equipmentLayout = new QVBoxLayout();
    equipmentLayout->addWidget(equipmentHeader);
    equipmentLayout->addWidget(equipmentLabel);

    // Feature Column
    QVBoxLayout *featureLayout = new QVBoxLayout();
    featureLayout->addWidget(featureHeader);
    featureLayout->addWidget(featureLabel);

    // Add the three columns to the horizontal layout
    columnsLayout->addLayout(proficienciesLayout);
    columnsLayout->addLayout(equipmentLayout);
    columnsLayout->addLayout(featureLayout);

    // Add the columns layout to the main layout
    mainLayout->addLayout(columnsLayout);

    // Navigation bar at the bottom
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40);

    backgroundComboBox = new MyComboBox;

    // Create navigation buttons
    QPushButton *backButton = new QPushButton("Back");
    QPushButton *nextButton = new QPushButton("Next");

    // Add the combo box and navigation buttons to the navbar
    navbarLayout->addWidget(backButton);
    navbarLayout->addWidget(backgroundComboBox);
    navbarLayout->addWidget(nextButton);

    // Add the navbar to the main layout
    mainLayout->addWidget(navbar);

    // Load the background data
    loadBackgrounds();

    // Connect the combo box to update the displayed information
    connect(backgroundComboBox, &QComboBox::currentTextChanged, this, &BackgroundWidget::updateBackgroundInfo);

    // When back button is clicked it calls the public SLOT function backPage()
    connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
    connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));

    // Initialize with the first background's details
    if (!backgroundComboBox->currentText().isEmpty()) {
        updateBackgroundInfo(backgroundComboBox->currentText());
    }
}

/**
 * This function changes AddCharacter's StackedWidget to RaceWidget
 */
void BackgroundWidget::loadBackgrounds() {
	QFile file("data/databases/Backgrounds.tsv"); // read in the file
	qDebug() << "Current working directory:" << QDir::currentPath();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { // error checking for debugging
		std::cerr << "Failed to open file" << std::endl;
		return;
	}

	QTextStream in(&file); // read in the file
	bool isHeader = true; // our first line is a header

	while (!in.atEnd()) {
		QString line = in.readLine(); // read in the line
		if (isHeader) {
			isHeader = false; // we are no longer on the header
			continue; // skip the header
		}

		QStringList fields = line.split("\t"); // split the line by \t
		if (fields.size() < 8) continue; // ensure we get all the fields

		BackgroundInfo info = {
			fields[1], // page
			fields[2], // description
			fields[3], // skill proficiencies
			fields[4], // tool proficiencies
			fields[5], // languages
			fields[6], // equipment
			fields[7], // feature
			fields[8]  // feature description
		};

		QString name = fields[0]; // name of the background
		backgrounds[name] = info; // add the background to the map
		backgroundComboBox->addItem(name); // add the background to the combo box
	}
}

void BackgroundWidget::updateBackgroundInfo(const QString &backgroundName) {
    if (!backgrounds.contains(backgroundName)) {
        qWarning() << "Background not found:" << backgroundName;
        return;
    }

    BackgroundInfo info = backgrounds[backgroundName];
    nameAndPageLabel->setText(backgroundName + " (Page " + info.page + ")");
    descriptionLabel->setText(info.description);
    proficienciesLabel->setText(info.skillProficiency + "\n" + info.toolProficiency);
    equipmentLabel->setText(info.equipment);
    featureLabel->setText(info.feature + "\n" + info.featureDescription);
}


void BackgroundWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(3);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to InventoryWidget
 */
void BackgroundWidget::nextPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(5);
	}
}

/**
 * Constructor for the class
 */
InventoryWidget::InventoryWidget(QWidget * parent) :
	QWidget(parent)
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

/**
 * This function changes AddCharacter's StackedWidget to BackgroundWidget
 */
void InventoryWidget::backPage()
{
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

/**
 * This function resets AddCharacter's StackedWidget to StartWidget
 * It also changes mainStackedWidget to SelectCharacter
 */
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
