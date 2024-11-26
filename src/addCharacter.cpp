/*
Name: addCharacter.cpp
Description: Implementation of the AddCharacter class, which allows users to create new characters.
Authors: Josh Park
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 11/24/2024
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
	QPixmap image(QDir::currentPath() + "/src/assets/" + *this->typeWidget + "/" + selection.toLower() + ".png", "-d");
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

// Convert list of strings to comma delimited string
QString listToCommaString(const QList<QString> strings)
{
	QString result;

	// Iterate through the list, joining strings with commas
	for (qsizetype i = 0; i < strings.size(); ++i)
	{
		result += strings[i];
		if (i != strings.size() - 1)
		{
			result += ",";
		}
	}

	return result;
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

	// connect function to autofill inventory when background widget is finished
	connect(backgroundWidget, SIGNAL(finished()), inventoryWidget, SLOT(autofillInventory()));
	// connect function to create the character's csv when finished with character creation
	connect(inventoryWidget, SIGNAL(finished()), SLOT(createCharacter()));
}

/**
 * This function is a public slot that when triggered by going past the inventory screen creates the character's csv file
 */
void AddCharacter::createCharacter()
{
	QString characterName = this->startWidget->getName();

	qDebug() << "in createCharacter()";

	// Path to the characters directory
	QString charPath = QDir::currentPath() + "/data/characters/" + characterName;
	qDebug() << charPath;

	// creates the character directory
	QDir dir;
	if (dir.exists(charPath)) {
		QMessageBox::warning(this, "Character Exists",
								"A character with this name already exists!"); // This is a warning message if the character already exists
		return;
	} else {
		if (dir.mkpath(charPath)) {
			// Create the notes and databases files inside the folder
			QFile notesFile(charPath + "/notes.json"); // Create a notes file
			if (notesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&notesFile);
				// Add default content to notes.txt
				out << "{" << Qt::endl;
				out << "\t\"sortPreference\": \"lastUpdated\"," << Qt::endl;
				out << "\t\"notes\": [" << Qt::endl;
				out << "\t\t" << Qt::endl;
				out << "\t]" << Qt::endl;
				out << "}" << Qt::endl;
				notesFile.close();		  // Close the file
			}
		} else {
			QMessageBox::warning(this, "Error", "Failed to create character directory."); // This is a warning message if the directory creation fails
		}
	}

	QFile characterFile(charPath + "/character.csv");

	if (characterFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&characterFile);
		// Stats
		QString strength = QString::number(this->baseStatsWidget->getStrength());
		QString dexterity = QString::number(this->baseStatsWidget->getDexterity());
		QString constitution = QString::number(this->baseStatsWidget->getConstitution());
		QString intelligence = QString::number(this->baseStatsWidget->getIntelligence());
		QString wisdom = QString::number(this->baseStatsWidget->getWisdom());
		QString charisma = QString::number(this->baseStatsWidget->getCharisma());
		QString level = QString::number(1);
		QString experience = QString::number(0);
		QString maxHp;
		QString characterClass = this->classWidget->getClass();
		int conMod = (this->baseStatsWidget->getConstitution() / 2) - 5;
		if (characterClass == "Barbarian") {
			maxHp = QString::number(12 + conMod);
		} else if (characterClass == "Fighter"
					|| characterClass == "Paladin"
					|| characterClass == "Ranger") {
			maxHp = QString::number(10 + conMod);
		} else if (characterClass == "Bard"
					|| characterClass == "Cleric"
					|| characterClass == "Druid"
					|| characterClass == "Monk"
					|| characterClass == "Ranger"
					|| characterClass == "Warlock") {
			maxHp = QString::number(8 + conMod);
		} else if (characterClass == "Sorcerer"
					|| characterClass == "Wizard") {
			maxHp = QString::number(6 + conMod);
		}
		QString currentHp = maxHp;
		QString tempHp = QString::number(0);
		QString characterSubClass = "None";
		QString characterRace = this->raceWidget->getRace();
		QString characterSubRace = this->raceWidget->getSubRace();

		// Proficiencies
		QList<QString> proficiencies = *this->classWidget->getSkillProficincies();
		proficiencies.append(this->backgroundWidget->getSkillProficincies());

		// Feats
		QList<QString> feats = {};

		// Languages
		QList<QString> languages = {this->raceWidget->getLanguages()};

		// Armor/Weapon Proficiencies
		QList<QString> armorWeaponProficiencies = *this->classWidget->getArmorProficincies();
		armorWeaponProficiencies.append(*this->classWidget->getWeaponProficincies());

		// Coins
		QString numPlatCoins = QString::number(0);
		QString numGoldCoins = QString::number(100);
		QString numSilverCoins = QString::number(0);
		QString numCopperCoins = QString::number(0);

		out << characterName + "," + strength + "," + dexterity + "," + constitution + "," + intelligence + "," + wisdom + "," + charisma + "," + level + ":" + experience + "," + maxHp + ":" + currentHp + ":" + tempHp + "," + characterClass + "," + characterSubClass + "," + characterRace + "," + characterSubRace + "\n";
		out << listToCommaString(proficiencies) + "\n";
		out << listToCommaString(feats) + "\n";
		out << listToCommaString(languages) + "\n";
		out << listToCommaString(armorWeaponProficiencies) + "\n";
		out << numPlatCoins + "," + numGoldCoins + "," + numSilverCoins + "," + numCopperCoins + "\n";
		characterFile.close();
		emit this->createdCharacter();
	}
}

/**
 * Constructor for the class
 */
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
BackgroundWidget::BackgroundWidget(QWidget *parent) : QWidget(parent)
{
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

	backgroundComboBox = new UpComboBox;

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
	if (!backgroundComboBox->currentText().isEmpty())
	{
		updateBackgroundInfo(backgroundComboBox->currentText());
	}
}

/**
 * This function changes AddCharacter's StackedWidget to RaceWidget
 */
void BackgroundWidget::loadBackgrounds()
{
	QFile file("data/databases/Backgrounds.tsv"); // read in the file
	qDebug() << "Current working directory:" << QDir::currentPath();
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{ // error checking for debugging
		std::cerr << "Failed to open file" << std::endl;
		return;
	}

	QTextStream in(&file); // read in the file
	bool isHeader = true;  // our first line is a header

	while (!in.atEnd())
	{
		QString line = in.readLine(); // read in the line
		if (isHeader)
		{
			isHeader = false; // we are no longer on the header
			continue;		  // skip the header
		}

		QStringList fields = line.split("\t"); // split the line by \t
		if (fields.size() < 8)
			continue; // ensure we get all the fields

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

		QString name = fields[0];		   // name of the background
		backgrounds[name] = info;		   // add the background to the map
		backgroundComboBox->addItem(name); // add the background to the combo box
	}
}

void BackgroundWidget::updateBackgroundInfo(const QString &backgroundName)
{
	if (!backgrounds.contains(backgroundName))
	{
		qWarning() << "Background not found:" << backgroundName;
		return;
	}

	// get info
	BackgroundInfo info = backgrounds[backgroundName];

	// make labels
	nameAndPageLabel->setText(backgroundName + " (Page " + info.page + ")");
	descriptionLabel->setText(info.description);
	proficienciesLabel->setText(info.skillProficiency + "\n" + info.toolProficiency);
	equipmentLabel->setText(info.equipment);
	featureLabel->setText(info.feature + "\n" + info.featureDescription);

	// make lists to put into the character
	QList<QString> skillProficiencies = info.skillProficiency.split(":", Qt::SkipEmptyParts);
	QList<QString> toolProficiencies = info.toolProficiency.split(":", Qt::SkipEmptyParts);
	this->items = info.equipment.split(":", Qt::SkipEmptyParts);
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
	emit this->finished();
	QStackedWidget *stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(5);
	}
}

/**
 * Constructor for the class
 */
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
	this->items = new QListWidget();
	QPushButton *addItemButton = new QPushButton("Add Item");
	this->removeItemButton = new QPushButton("Remove Selected Item");
	removeItemButton->setEnabled(false);

	// Create navigation buttons
	QPushButton *backButton = new QPushButton("Back");
	QPushButton *finishButton = new QPushButton("Finish");

	// Add inventory elements to the body
	inventoryLayout->addItem(spacer, 0, 3);
	inventoryLayout->addItem(spacer, 3, 3);
	inventoryLayout->addWidget(items, 2, 2, 1, 3);
	inventoryLayout->addWidget(addItemButton, 1, 1, 1, 3, Qt::AlignCenter);
	inventoryLayout->addWidget(removeItemButton, 1, 3, 1, 3, Qt::AlignCenter);

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(finishButton);

	// Add the body and navbar to the main layout
	layout->addWidget(inventory);
	layout->addWidget(navbar);

	// When back button is clicked it calls the public SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	connect(finishButton, SIGNAL(clicked()), SLOT(nextPage()));
	connect(this->removeItemButton, SIGNAL(clicked()), SLOT(deleteItem()));
	connect(addItemButton, SIGNAL(clicked()), SLOT(addItem()));
	connect(this->items, SIGNAL(itemClicked(QListWidgetItem *)), SLOT(selectItem()));
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
	emit this->finished();
	AddCharacter *addCharacterWidget = qobject_cast<AddCharacter *>(this->parentWidget());
	if (addCharacterWidget)
	{
		addCharacterWidget->setCurrentIndex(0);
	}
	QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(addCharacterWidget->parentWidget());
	if (mainStackedWidget)
	{
		CharacterSelect *charSelect = qobject_cast<CharacterSelect *>(mainStackedWidget->widget(0)); // Get the character select widget
		charSelect->loadCharacterList(); // Reload the character list
		mainStackedWidget->setCurrentIndex(0); // Switch to the character select page
	}
}

void InventoryWidget::autofillInventory()
{
	AddCharacter *parent = qobject_cast<AddCharacter *>(this->parent());

	QList<QString> classItems = *parent->getClassWidget()->getItems();
	QList<QString> backgroundItems = parent->getBackgroundWidget()->getItems();

	this->items->clear();

	this->items->addItems(classItems);
	this->items->addItems(backgroundItems);
}

void InventoryWidget::addItem()
{
	QDialog popup;

	popup.setWindowModality(Qt::ApplicationModal); // stios the user from interacting with the main window while the popup is open
	popup.setWindowTitle("Add Item");			   // set the title of the popup

	popup.setFixedSize(200, 150); // set the size of the popup

	QLabel *popupText = new QLabel("Add Item", &popup); // creates the text for the popup
	QLabel *textboxLabel = new QLabel("Name:", &popup); // label for textbox
	QLineEdit *itemName = new QLineEdit(&popup);

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
	popupLayout.addWidget(textboxLabel);
	popupLayout.addWidget(itemName);
	popupLayout.addWidget(popupButtonWidget);

	// makes the popup text take up more horizontal space
	popupLayout.setStretch(0, 1);

	// show the popup
	popup.exec();

	if (popup.result() == QDialog::Accepted)
	{
		this->items->addItem(itemName->text());
	}
}

void InventoryWidget::deleteItem()
{
	QListWidgetItem *item = this->items->currentItem();
	if (item == nullptr)
	{
		return;
	}

	// gets the name of the item
	QString name = item->text();

	// creates popup for the user to confirm the deletion of the item
	QDialog popup;

	popup.setWindowModality(Qt::ApplicationModal); // stops the user from interacting with the main window while the popup is open
	popup.setWindowTitle("Delete Item?");		   // set the title of the popup

	popup.setFixedSize(200, 150); // set the size of the popup

	QLabel *popupText = new QLabel("Delete " + name + "?", &popup); // creates the text for the popup
	popupText->setWordWrap(true);									// allows the text to wrap around if it is too long

	// create the buttons for the popup
	QPushButton *popupConfirm = new QPushButton("Yes", &popup); // creates the confirm button
	QPushButton *popupCancel = new QPushButton("No", &popup);	// creates the cancel button

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

	if (popup.result() == QDialog::Accepted)
	{
		this->items->removeItemWidget(item);
		delete item;
	}
}

void InventoryWidget::selectItem()
{
	this->removeItemButton->setEnabled(true);
}