/*
Name: viewSpells.cpp
Description: Page to view a character's spells.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 11/5/2024
Last Modified: 12/3/2024
*/

#include "viewSpells.h"
#include "viewCharacter.h"
#include "themeUtils.h"
#include "centeredCheckBox.h"

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
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QTextEdit>

ViewSpells::ViewSpells(QWidget *parent, QString nameIn) :
    QWidget(parent), name(nameIn)
{
    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesnt take up half the screen

    // Create buttons to go back to the character page
    QPushButton *backToCharacter = new QPushButton("Return to Character");
    // Create header label with character's name
    QLabel *spellsLabel = new QLabel(name + "'s Spells");

    // Add buttons to navbar
    navbarLayout->addWidget(backToCharacter);
    navbarLayout->addWidget(spellsLabel, 1, Qt::AlignCenter);

    
    // Create the layout for the spells page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create body widget
    QWidget *body = new QWidget();
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);

    this->spells = new QTableWidget();
    this->spells->setColumnCount(15);

    QWidget * column = new QWidget();
    QVBoxLayout * columnLayout = new QVBoxLayout(column);

    QPushButton * addSpellButton = new QPushButton("Learn Spell");

    QLabel * spellSlotsLabel = new QLabel("Spell Slots");
    QFont font = spellSlotsLabel->font();
    font.setPointSize(font.pointSize() + 2);
    spellSlotsLabel->setFont(font);
    QLabel * level1Slots = new QLabel("Level 1: ");
    QLabel * level2Slots = new QLabel("Level 2: ");
    QLabel * level3Slots = new QLabel("Level 3: ");
    QLabel * level4Slots = new QLabel("Level 4: ");
    QLabel * level5Slots = new QLabel("Level 5: ");
    QLabel * level6Slots = new QLabel("Level 6: ");
    QLabel * level7Slots = new QLabel("Level 7: ");
    QLabel * level8Slots = new QLabel("Level 8: ");
    QLabel * level9Slots = new QLabel("Level 9: ");

    columnLayout->addWidget(addSpellButton, Qt::AlignHCenter);
    columnLayout->addWidget(spellSlotsLabel, Qt::AlignHCenter);
    columnLayout->addWidget(level1Slots);
    columnLayout->addWidget(level2Slots);
    columnLayout->addWidget(level3Slots);
    columnLayout->addWidget(level4Slots);
    columnLayout->addWidget(level5Slots);
    columnLayout->addWidget(level6Slots);
    columnLayout->addWidget(level7Slots);
    columnLayout->addWidget(level8Slots);
    columnLayout->addWidget(level9Slots);

    bodyLayout->addWidget(this->spells);
    bodyLayout->addWidget(column);

    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(saveSpells()));

    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));

    connect(addSpellButton, SIGNAL(clicked()), SLOT(addSpell()));

    this->loadSpells();

    reloadTheme(); // Reload the theme after everything is placed
}

void ViewSpells::addSpell() {
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
        QString newItem;
        newItem += spellName->text() + ",";
        newItem += book->text() + ",";
        newItem += std::to_string(page->value()).c_str() + QString(",");
        newItem += std::to_string(level->value()).c_str() + QString(",");
        newItem += school->text() + ",";
        newItem += time->text() + ",";
        newItem += std::to_string(maxRange->value()).c_str() + QString(",");
        if (verbal->isChecked()) {
            newItem += "v";
        }
        if (somatic->isChecked()) {
            newItem += "s";
        }
        if (material->isChecked()) {
            newItem += "m";
        }
        newItem += ",";
        newItem += duration->text() + ",";
        if (concentration->isChecked()) {
            newItem += "1";
        }
        newItem += ",";
        if (ritual->isChecked()) {
            newItem += "1";
        }
        newItem += ",0,";
		QString desc = description->toPlainText();
		QRegularExpression re("\n");
		desc.replace(re,"<br>");
        newItem += desc;
        this->addItem(newItem);
    
        this->spells->resizeColumnsToContents();
        this->spells->sortByColumn(0, Qt::AscendingOrder);
        this->spells->sortByColumn(3, Qt::AscendingOrder);
	}
}

void ViewSpells::loadSpells()
{
    this->spells->clear();
    QStringList colNames = {"Name", "Book", "Page", "Level", "School", "Casting Time", "Range", "Verbal", "Somatic", "Material", "Duration", "Concentration", "Ritual", "Prepared", "Description"};
    this->spells->setHorizontalHeaderLabels(colNames);
    // hide the description column because it would take too much space
    this->spells->setColumnHidden(14, true);

	// Path to the characters directory
	QString charPath = QDir::currentPath() + "/data/characters/" + this->name;

    QFile charSpells(charPath + "/spells.csv");

    if (charSpells.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&charSpells);

        while (!in.atEnd()) {
            QString line = in.readLine();
            this->addItem(line);
        }
    }
    this->spells->resizeColumnsToContents();
    this->spells->sortByColumn(0, Qt::AscendingOrder);
    this->spells->sortByColumn(3, Qt::AscendingOrder);
}

void ViewSpells::addItem(QString line) {
    QStringList values = line.split(",");
    QTableWidgetItem * name = new QTableWidgetItem(values[0]);
    name->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * book = new QTableWidgetItem(values[1]);
    book->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * page = new QTableWidgetItem(values[2]);
    page->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * level = new QTableWidgetItem(values[3]);
    level->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * school = new QTableWidgetItem(values[4]);
    school->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * time = new QTableWidgetItem(values[5]);
    time->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem * range = new QTableWidgetItem(values[6]);
    range->setTextAlignment(Qt::AlignCenter);
    CenteredCheckBox * verbal = new CenteredCheckBox();
    CenteredCheckBox * somatic = new CenteredCheckBox();
    CenteredCheckBox * material = new CenteredCheckBox();
    if (values[7].contains("v")) {
        verbal->setChecked(true);
    }
    if (values[7].contains("s")) {
        somatic->setChecked(true);
    }
    if (values[7].contains("m")) {
        material->setChecked(true);
    }
    verbal->setEnabled(false);
    somatic->setEnabled(false);
    material->setEnabled(false);
    QTableWidgetItem * duration = new QTableWidgetItem(values[8]);
    duration->setTextAlignment(Qt::AlignCenter);
    CenteredCheckBox * concentration = new CenteredCheckBox();
    if (values[9] == "1") {
        concentration->setChecked(true);
    }
    concentration->setEnabled(false);
    CenteredCheckBox * ritual = new CenteredCheckBox();
    if (values[10] == "1") {
        ritual->setChecked(true);
    }
    ritual->setEnabled(false);
    CenteredCheckBox * prepared = new CenteredCheckBox();
    if (values[11] == "1") {
        prepared->setChecked(true);
    }
    QTableWidgetItem * description = new QTableWidgetItem(values[11]);

    int row = this->spells->rowCount();
    this->spells->insertRow(row);
    this->spells->setItem(row, 0, name);
    this->spells->setItem(row, 1, book);
    this->spells->setItem(row, 2, page);
    this->spells->setItem(row, 3, level);
    this->spells->setItem(row, 4, school);
    this->spells->setItem(row, 5, time);
    this->spells->setItem(row, 6, range);
    this->spells->setCellWidget(row, 7, verbal->getLayoutWidget());
    this->spells->setCellWidget(row, 8, somatic->getLayoutWidget());
    this->spells->setCellWidget(row, 9, material->getLayoutWidget());
    this->spells->setItem(row, 10, duration);
    this->spells->setCellWidget(row, 11, concentration->getLayoutWidget());
    this->spells->setCellWidget(row, 12, ritual->getLayoutWidget());
    this->spells->setCellWidget(row, 13, prepared->getLayoutWidget());
    this->spells->setItem(row, 14, description);
}

void ViewSpells::goBack()
{
    QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    if (!mainStackedWidget) {
        qDebug() << "mainStackedWidget not retrieved";
        return;
    }
    // cast the QWidget from index 0
    QWidget *viewCharacterWidget = mainStackedWidget->widget(0);

    // call the loadAll() function from viewCharacter which is the QWidget at index 0
    ViewCharacter *character = qobject_cast<ViewCharacter *>(viewCharacterWidget);
    if (!character) {
        qDebug() << "character not retrieved";
        return;
    }
    character->loadAll();

    mainStackedWidget->setCurrentIndex(0); // Switch to ViewCharacter (index 0)
}

void ViewSpells::saveSpells() {
	QFile spellsFile("data/characters/" + this->name + "/spells.csv");

	if (spellsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        for (int i = 0; i < this->spells->rowCount(); i++) {
			QTextStream out(&spellsFile);

            QString spellName = this->spells->item(i,0)->text();
            QString book = this->spells->item(i,1)->text();
            QString page = this->spells->item(i,2)->text();
            QString level = this->spells->item(i,3)->text();
            QString school = this->spells->item(i,4)->text();
            QString time = this->spells->item(i,5)->text();
            QString range = this->spells->item(i,6)->text();
            QString components;
            auto temp = this->spells->cellWidget(i,13);
            auto tempChild = temp->children();
            qDebug() << temp;
            qDebug() << tempChild;
            qDebug() << tempChild[1];
            CenteredCheckBox * verbal = qobject_cast<CenteredCheckBox *>(this->spells->cellWidget(i,7)->children()[1]);
            CenteredCheckBox * somatic = qobject_cast<CenteredCheckBox *>(this->spells->cellWidget(i,8)->children()[1]);
            CenteredCheckBox * material = qobject_cast<CenteredCheckBox *>(this->spells->cellWidget(i,9)->children()[1]);
            if (!verbal) {
                qDebug() << "verbal CenteredCheckBox not retrieved";
                continue;
            }
            if (!somatic) {
                qDebug() << "somatic CenteredCheckBox not retrieved";
                continue;
            }
            if (!material) {
                qDebug() << "material CenteredCheckBox not retrieved";
                continue;
            }
            qDebug() << "verbal somatic and material exist";
			if (verbal->isChecked()) {
				components += "v";
			}
            qDebug() << "past verbal";
			if (somatic->isChecked()) {
				components += "s";
			}
            qDebug() << "past somatic";
			if (material->isChecked()) {
				components += "m";
			}
            qDebug() << "past material";
            QString duration = this->spells->item(i,10)->text();
            CenteredCheckBox * concentration = qobject_cast<CenteredCheckBox *>(this->spells->cellWidget(i,11)->children()[1]);
            CenteredCheckBox * ritual = qobject_cast<CenteredCheckBox *>(this->spells->cellWidget(i,12)->children()[1]);
            CenteredCheckBox * prepared = qobject_cast<CenteredCheckBox *>(this->spells->cellWidget(i,13)->children()[1]);
            qDebug() << "before description";
            QString description = this->spells->item(i,14)->text();
            if (!concentration) {
                qDebug() << "concentration CenteredCheckBox not retrieved";
                continue;
            }
            if (!ritual) {
                qDebug() << "ritual CenteredCheckBox not retrieved";
                continue;
            }
            if (!prepared) {
                qDebug() << "prepared CenteredCheckBox not retrieved";
                continue;
            }

            out << spellName << "," << book << "," << page << "," << level << "," << school << "," << time << "," << range << "," << components << "," <<
                    duration << "," << concentration->isChecked() << "," << ritual->isChecked() << "," << prepared->isChecked() << "," << description << Qt::endl;
        }
        spellsFile.close();
    }
}

ViewSpells::~ViewSpells()
{
}