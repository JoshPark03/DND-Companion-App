/*
Name: spellsWidget.cpp
body: Implementation of the SpellsWidget class, which allows users to select spells for their character.
Authors: Josh Park
Other Sources: ...
Date Created: 11/27/2024
Last Modified: 12/1/2024
*/

#include "addCharacter.h"

#include <QFile>
#include <QDialog>
#include <QCheckBox>
#include <QTextEdit>

SpellsWidget::SpellsWidget(QWidget * parent) 
: QWidget(parent) {
	this->spells = new QMap<QString, SpellInfo *>;

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
	std::string headerstr = "<h1>Spells</h1><br><h3>Choose " + std::to_string(this->numSpells()) + " Spells</h3>";
	header = new QLabel(headerstr.c_str());
	// header->setFixedHeight(50);

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
	// When addSpellButton is clicked it calls the private SLOT function addSpell()
	connect(addSpellButton, SIGNAL(clicked()), SLOT(addSpell()));
	// When addSpellButton is clicked it calls the private SLOT function addSpell()
	connect(removeSpellButton, SIGNAL(clicked()), SLOT(removeSpell()));
	// When a spell is selected it calls the private SLOT function selectSpell()
	connect(spellsList, SIGNAL(itemClicked(QListWidgetItem *)), SLOT(selectSpell()));
	// When spellsList has a spell added or removed call private SLOT function checkNumSpells()
	auto spellsModel = this->spellsList->model();
	connect(spellsModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), SLOT(checkNumSpells()));
	connect(spellsModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), SLOT(checkNumSpells()));
}

/**
 * This function changes AddCharacter's StackedWidget to ClassWidget
 */
void SpellsWidget::backPage()
{
	QStackedWidget * stackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
	if (stackedWidget)
	{
		stackedWidget->setCurrentIndex(4);
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
		stackedWidget->setCurrentIndex(6);
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
	page->setMaximum(999);

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
	maxRange->setMaximum(360);

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
		SpellInfo * info = new SpellInfo;
		info->book = book->text();
		info->page = page->value();
		info->level = level->value();
		info->school = school->text();
		info->time = time->text();
		info->minRange = minRange->value();
		info->maxRange = maxRange->value();
		info->verbal = verbal->isChecked();
		info->somatic = somatic->isChecked();
		info->material = material->isChecked();
		info->duration = duration->text();
		info->concentration = concentration->isChecked();
		info->ritual = ritual->isChecked();
		QString desc = description->toPlainText();
		QRegularExpression re("\n");
		desc.replace(re,"<br>");
		info->description = desc;
		(*this->spells)[spellName->text()] = info;
	}
	
	// this is so that no spell is selected after creation
	this->spellsList->selectionModel()->clear();
	this->removeSpellButton->setEnabled(false);
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
		this->spells->remove(item->text());
		delete item;
	}

	// clear the selection
	// this is so that no spell is selected after deletion
	this->spellsList->selectionModel()->clear();
	this->removeSpellButton->setEnabled(false);
}

void SpellsWidget::checkNumSpells() {
	if (this->spellsList->count() < this->numSpells()) { // cantrips + spells known
		this->addSpellButton->setEnabled(true);
	} else {
		this->addSpellButton->setEnabled(false);
	}
}

int SpellsWidget::numSpells() {
	AddCharacter * stackedWidget = qobject_cast<AddCharacter *>(this->parentWidget());
	if (!stackedWidget) {
		qDebug() << "stackedWidget of AddCharcter not initialized";
		return 0;
	}
	BaseStatsWidget * baseStatsWidget = qobject_cast<BaseStatsWidget *>(stackedWidget->widget(1));
	if (!baseStatsWidget) {
		qDebug() << "baseStatsWidget not initialized";
		return 0;
	}
    ClassWidget * classWidget = qobject_cast<ClassWidget *>(stackedWidget->widget(2));
	if (!classWidget) {
		qDebug() << "classWidget not initialized";
		return 0;
	}
	QString className = classWidget->getClass();

	if (className == "Bard") { // cantrips + spells known
		return 2 + 4;
	}

	if (className == "Cleric") { // cantrips + prepared spells (level + modifier)
		return 3 + std::max(1, 1 + ((baseStatsWidget->getWisdom() / 2) - 5));
	}

	if (className == "Druid") { // cantrips + prepared spells (level + modifier)
		return 2 + std::max(1, 1 + ((baseStatsWidget->getWisdom() / 2) - 5));
	}

	if (className == "Paladin") { // prepared spells (floor(level/2) + modifier)
		return std::max(1, 1 + ((baseStatsWidget->getCharisma() / 2) - 5));
	}

	if (className == "Ranger") { // spells known
		return 2;
	}

	if (className == "Sorcerer") { // cantrips + spells known
		return 4 + 2;
	}

	if (className == "Warlock") { // cantrips + spells known
		return 2 + 2;
	}

	if (className == "Wizard") { // cantrips + spells known
		return 3 + 6;
	}
	return 0;
}

void SpellsWidget::selectSpell() {
	this->removeSpellButton->setEnabled(true);
}

void SpellsWidget::updateNumSpells() {
	std::string headerstr = "<h1>Spells</h1><br><h3>Choose " + std::to_string(this->numSpells()) + " Spells</h3>";
	header->setText(headerstr.c_str());
}

void SpellsWidget::recordSpells(QString charPath) {
	QFile spellsFile(charPath + "/spells.csv");

	if (spellsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		while (auto spell = this->spellsList->takeItem(0)) {
			QString spellName = spell->text();
			SpellInfo * info = (*this->spells)[spellName];

			QTextStream out(&spellsFile);

			out << spellName << "," << info->book << "," << info->page << "," << info->level << "," << info->school << "," << info->time << "," << info->maxRange << ",";
			if (info->verbal) {
				out << "v";
			}
			if (info->somatic) {
				out << "s";
			}
			if (info->material) {
				out << "m";
			}
			out << "," << info->duration << "," << std::to_string(info->concentration).c_str() << "," << std::to_string(info->ritual).c_str() << ",false," << info->description << Qt::endl;
		}
		spellsFile.close();
	}
}