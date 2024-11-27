/*
Name: spellsWidget.cpp
body: Implementation of the SpellsWidget class, which allows users to select spells for their character.
Authors: Josh Park
Other Sources: ...
Date Created: 11/27/2024
Last Modified: 11/27/2024
*/

#include "addCharacter.h"

#include <QDialog>
#include <QCheckBox>
#include <QTextEdit>

SpellsWidget::SpellsWidget(QWidget * parent) 
: QWidget(parent) {
	this->spells = new QMap<QString, SpellInfo>;

	// Create the main layout
	QGridLayout * mainLayout = new QGridLayout(this);

	// Create and configure layout for the navbar
	QWidget * navbar = new QWidget();
	QHBoxLayout * navbarLayout = new QHBoxLayout(navbar);
	navbar->setFixedHeight(40);

	// Create navigation buttons
	QPushButton * backButton = new QPushButton("Back");
	QPushButton * nextButton = new QPushButton("Next");

	// Add the combo box and navigation buttons to the navbar
	navbarLayout->addWidget(backButton);
	navbarLayout->addWidget(nextButton);

	// Create header
	header = new QLabel("<h1>Spells</h1>");
	header->setFixedHeight(50);

	// Create the body layout
	QGridLayout * body = new QGridLayout();
	QSpacerItem *spacer = new QSpacerItem(this->width(), this->height());
	this->addSpellButton = new QPushButton("Add Spell");
	this->removeSpellButton = new QPushButton("Remove Spell");
	this->removeSpellButton->setEnabled(false);
	this->spellsList = new QListWidget();

	// Setting the body layout
	body->addItem(spacer, 0, 0, 100, 100);
	body->addWidget(this->addSpellButton, 5, 8, 5, 10);
	body->addWidget(this->removeSpellButton, 15, 8, 5, 10);
	body->addWidget(this->spellsList, 5, 30, 80, 60);

	// Add the all pieces to the main layout
	mainLayout->addWidget(header, 0, 0, 1, -1, {Qt::AlignCenter});
	mainLayout->addLayout(body, 1, 0, 1, 4);
	mainLayout->addWidget(navbar, 2, 0, 1, -1, {Qt::AlignBottom});

	// When backButton is clicked it calls the private SLOT function backPage()
	connect(backButton, SIGNAL(clicked()), SLOT(backPage()));
	// When nextButton is clicked it calls the private SLOT function nextPage()
	connect(nextButton, SIGNAL(clicked()), SLOT(nextPage()));
	// When addSpellButton is clicked it calls the privat SLOT function addSpell()
	connect(addSpellButton, SIGNAL(clicked()), SLOT(addSpell()));
	// When addSpellButton is clicked it calls the privat SLOT function addSpell()
	connect(removeSpellButton, SIGNAL(clicked()), SLOT(removeSpell()));
}

/**
 * This function changes AddCharacter's StackedWidget to ClassWidget
 */
void SpellsWidget::backPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(2);
	}
}

/**
 * This function changes AddCharacter's StackedWidget to RaceWidget
 */
void SpellsWidget::nextPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
	}
}

void SpellsWidget::addSpell() {
	QDialog popup;

	popup.setWindowModality(Qt::ApplicationModal); // stios the user from interacting with the main window while the popup is open
	popup.setWindowTitle("Add Spell");			   // set the title of the popup

	QLabel *popupHeader = new QLabel("Add Spell"); // creates the header text for the popup

	QLabel * bookLabel = new QLabel("Book Abbreviation:");
	QLineEdit * book = new QLineEdit();

	QLabel * pageLabel = new QLabel("Page:");
	QSpinBox * page = new QSpinBox();

	// spell attributes and labels
	QLabel *nameLabel = new QLabel("Name"); // label for textbox
	QLineEdit *spellName = new QLineEdit();

	QLabel * levelLabel = new QLabel("Level");
	QSpinBox * level = new QSpinBox();
	level->setRange(0, 9);

	QLabel * schoolLabel = new QLabel("School of Magic");
	QLineEdit * school = new QLineEdit();

	QLabel * timeLabel = new QLabel("Casting Time");
	QLineEdit * time = new QLineEdit();

	QLabel * rangeLabel = new QLabel("Range");
	QLabel * minRangeLabel = new QLabel("Min");
	QLabel * maxRangeLabel = new QLabel("Max");
	QSpinBox * minRange = new QSpinBox();	
	QSpinBox * maxRange = new QSpinBox();

	QLabel * componentsLabel = new QLabel("Components");
	QLabel * verbalLabel = new QLabel("Verbal");
	QLabel * somaticLabel = new QLabel("Somatic");
	QLabel * materialLabel = new QLabel("Material");
	QCheckBox * verbal = new QCheckBox();
	QCheckBox * somatic = new QCheckBox();
	QCheckBox * material = new QCheckBox();

	QLabel * durationLabel = new QLabel("Duration");
	QLineEdit * duration = new QLineEdit();

	QLabel * concentrationLabel = new QLabel("Concentration");
	QCheckBox * concentration = new QCheckBox();
	
	QLabel * ritualLabel = new QLabel("Ritual");
	QCheckBox * ritual = new QCheckBox();

	QLabel * descriptionLabel = new QLabel("Description");
	QTextEdit * description = new QTextEdit();

	// create the buttons for the popup
	QPushButton *popupConfirm = new QPushButton("Confirm"); // creates the confirm button
	QPushButton *popupCancel = new QPushButton("Cancel");	// creates the cancel button

	// Gets rid of the padding around the text of the buttons
	popupConfirm->setStyleSheet("padding: 5px 0px; margin: 0px;");
	popupCancel->setStyleSheet("padding: 5px 0px; margin: 0px;");

	// Connect the confirm button to accept the dialog (e.g., confirming the deletion)
	QObject::connect(popupConfirm, &QPushButton::clicked, &popup, &QDialog::accept);

	// Connect the cancel button to reject the dialog (e.g., canceling the deletion)
	QObject::connect(popupCancel, &QPushButton::clicked, &popup, &QDialog::reject);

	// create the layout for the popup
	QVBoxLayout * popupLayout = new QVBoxLayout(&popup);

	// create the layout for the header of the popup
	QHBoxLayout * popupHeaderLayout = new QHBoxLayout();

	// creates the layout for the body of the popup
	QGridLayout * popupBodyLayout = new QGridLayout();

	// Create button widget
	QHBoxLayout * popupButtonLayout = new QHBoxLayout();

	// adds the header components to the header layout
	popupHeaderLayout->addWidget(popupHeader);
	popupBodyLayout->addWidget(bookLabel, 0, 0, 1, 1, {Qt::AlignLeft});
	popupBodyLayout->addWidget(book, 0, 1, 1, 1, {Qt::AlignLeft});
	popupBodyLayout->addWidget(pageLabel, 0, 2, 1, 1, {Qt::AlignLeft});
	popupBodyLayout->addWidget(page, 0, 3, 1, 1, {Qt::AlignLeft});

	// adds the attributes to the body layout
	popupBodyLayout->addWidget(nameLabel, 1, 0, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(spellName, 2, 0, 2, 1);

	popupBodyLayout->addWidget(levelLabel, 1, 1, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(level, 2, 1, 2, 1);

	popupBodyLayout->addWidget(schoolLabel, 1, 2, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(school, 2, 2, 2, 1);

	popupBodyLayout->addWidget(timeLabel, 1, 3, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(time, 2, 3, 2, 1);

	popupBodyLayout->addWidget(rangeLabel, 1, 4, 1, 2, {Qt::AlignCenter});
	popupBodyLayout->addWidget(minRangeLabel, 2, 4, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(minRange, 3, 4, 1, 1);
	popupBodyLayout->addWidget(maxRangeLabel, 2, 5, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(maxRange, 3, 5, 1, 1);

	popupBodyLayout->addWidget(componentsLabel, 1, 6, 1, 3, {Qt::AlignCenter});
	popupBodyLayout->addWidget(verbalLabel, 2, 6, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(verbal, 3, 6, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(somaticLabel, 2, 7, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(somatic, 3, 7, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(materialLabel, 2, 8, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(material, 3, 8, 1, 1, {Qt::AlignCenter});

	popupBodyLayout->addWidget(durationLabel, 1, 9, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(duration, 2, 9, 2, 1);

	popupBodyLayout->addWidget(concentrationLabel, 1, 10, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(concentration, 2, 10, 2, 1, {Qt::AlignCenter});
	
	popupBodyLayout->addWidget(ritualLabel, 1, 11, 1, 1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(ritual, 2, 11, 2, 1, {Qt::AlignCenter});

	popupBodyLayout->addWidget(descriptionLabel, 4, 0, 1, -1, {Qt::AlignCenter});
	popupBodyLayout->addWidget(description, 5, 0, -1, -1);

	// adds the buttons to the button layout
	popupButtonLayout->addWidget(popupConfirm);
	popupButtonLayout->addWidget(popupCancel);

	// adds the components to the popup
	popupLayout->addLayout(popupHeaderLayout);
	popupLayout->addLayout(popupBodyLayout);
	popupLayout->addLayout(popupButtonLayout);

	// makes the popup text take up more horizontal space
	popupLayout->setStretch(0, 1);

	// show the popup
	popup.exec();

	if (popup.result() == QDialog::Accepted)
	{
		this->spellsList->addItem(spellName->text());
	}
}

void SpellsWidget::removeSpell() {
	QListWidgetItem *item = this->spellsList->currentItem();
	if (item == nullptr)
	{
		qDebug() << "No spell selected";
		return;
	}
	// gets the name of the spell
	QString name = item->text();

	// creates popup for the user to confirm the deletion of the spell
	QDialog popup;

	popup.setWindowModality(Qt::ApplicationModal); // stops the user from interacting with the main window while the popup is open
	popup.setWindowTitle("Delete Spell?");	   // set the title of the popup

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
		this->spellsList->removeItemWidget(item);
		delete item;
	}


	// clear the selection
	// this is so that no spell is selected after deletion
	this->spellsList->selectionModel()->clear();
}

void SpellsWidget::recordSpells() {

}