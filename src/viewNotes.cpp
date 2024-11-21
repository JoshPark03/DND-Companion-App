/*
Name: viewNotes.cpp
Description: Page to view a character's notes.
Authors: Zachary Craig
Other Sources: ...
Date Created: 11/18/2024
Last Modified: 11/18/2024
*/

#include "viewNotes.h"
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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QTimer>
#include <QLineEdit>

void ViewNotes::loadNotes()
{
    // Get the path to the notes file
    QString notesPath = QDir::currentPath() + "/data/characters/" + name + "/notes.json";

    qDebug() << notesPath;

    // Open the notes file
    QFile notesFile(notesPath);
    if (!notesFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open notes file";
        return;
    }

    // Parse the notes file
    QByteArray notesData = notesFile.readAll();
    notesFile.close();

    QJsonDocument notesDoc = QJsonDocument::fromJson(notesData);
    if(!notesDoc.isObject())
    {
        qDebug() << "Failed to parse notes file";
        return;
    }

    QJsonObject notesObj = notesDoc.object();
    QJsonArray notesArr = notesObj["notes"].toArray();

    notesList->clear(); // Clear the notes list
    currentSection = ""; // Reset the current section
    noteEdit->clear(); // Clear the current note

    // Add each note to the list
    for(const QJsonValue &note : notesArr)
    {
        QJsonObject noteObj = note.toObject();
        QString noteSection = noteObj["section"].toString();
        if(!noteSection.isEmpty())
        {
            notesList->addItem(noteSection);
        }
    }

    // Add a create a note button to the list
    notesList->addItem("Create a New Note");
}

void ViewNotes::onNoteSelected(QListWidgetItem *item)
{
    // Get the clicked section name
    QString newSectionName = item->text();

    QString sectionName = item->text();

    if (sectionName == "Create a New Note")
    {
        // Replace the create a new note button with an editable field and checkmark
        QWidget *newNoteWidget = new QWidget();
        QHBoxLayout *newNoteLayout = new QHBoxLayout(newNoteWidget);
        newNoteLayout->setContentsMargins(0, 0, 0, 0); // Remove margins for proper alignment
        newNoteLayout->setSpacing(5); // Adjust spacing between components

        // Editable text field for the new note name
        QLineEdit *noteNameEdit = new QLineEdit();
        noteNameEdit->setPlaceholderText("Enter note name...");
        noteNameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Allow expansion horizontally
        noteNameEdit->setFixedHeight(25); // Set a fixed height to avoid oversized text fields
        noteNameEdit->setStyleSheet("padding: 2px; font-size: 12px;"); // Inline adjustments

        // Checkmark button to confirm the new note creation
        QPushButton *checkButton = new QPushButton("✔");
        checkButton->setFixedSize(25, 25); // Ensure a uniform size
        checkButton->setStyleSheet("font-size: 14px; padding: 0; margin: 0;"); // Minimize padding/margin for compactness

        // Add the text field and button to the layout
        newNoteLayout->addWidget(noteNameEdit);
        newNoteLayout->addWidget(checkButton);

        // Replace the list row with the widget
        int row = notesList->row(item);
        notesList->takeItem(row); // Remove the old "Create a New Note" item
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(newNoteWidget->sizeHint()); // Match the item's size to the widget
        notesList->insertItem(row, newItem); // Insert the new item at the same position
        notesList->setItemWidget(newItem, newNoteWidget); // Set the widget for the item

        // Connect the checkmark button to create the note
        connect(checkButton, &QPushButton::clicked, this, [=]()
        {
            QString newNoteName = noteNameEdit->text().trimmed();
            if (!newNoteName.isEmpty())
            {
                createNewNote(newNoteName); // Create the new note
                notesList->removeItemWidget(newItem); // Remove the temporary widget
                delete newItem; // Clean up temporary widget
                loadNotes(); // Reload the notes list to show the new note
            }
        });

        return;
    }

    // Save the current note before switching
    saveCurrentNote();

    // Get the path to the notes file
    QString notesPath = QDir::currentPath() + "/data/characters/" + name + "/notes.json";

    // Open the notes file
    QFile notesFile(notesPath);
    if (!notesFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open notes file";
        return;
    }

    // Parse the notes file
    QByteArray notesData = notesFile.readAll();
    notesFile.close(); // Close the file after reading

    QJsonDocument notesDoc = QJsonDocument::fromJson(notesData); // Parse the JSON data
    if (!notesDoc.isObject())
    {
        qDebug() << "Failed to parse notes file"; // Log an error if parsing fails
        return;
    }

    QJsonObject notesObj = notesDoc.object(); // Get the JSON object
    QJsonArray notesArr = notesObj["notes"].toArray(); // Get the notes array

    // Find the note corresponding to the new section
    for (const QJsonValue &note : notesArr)
    {
        QJsonObject noteObj = note.toObject();
        if (noteObj["section"].toString() == newSectionName)
        {
            // Populate the noteEdit with the corresponding notes content
            noteEdit->setText(noteObj["notes"].toString());
            currentSection = newSectionName; // Update the current section
            return;
        }
    }

    // If no match is found, clear the noteEdit and reset currentSection
    noteEdit->clear();
    currentSection = "";
}

void ViewNotes::saveCurrentNote()
{
    // Skip saving if there's no current section
    if (currentSection.isEmpty())
    {
        return;
    }

    // Get the path to the notes file
    QString notesPath = QDir::currentPath() + "/data/characters/" + name + "/notes.json";

    // Open the notes file
    QFile notesFile(notesPath);
    if (!notesFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open notes file for writing";
        return;
    }

    // Parse the JSON file
    QByteArray notesData = notesFile.readAll();
    notesFile.close();

    QJsonDocument notesDoc = QJsonDocument::fromJson(notesData);
    if (!notesDoc.isObject())
    {
        qDebug() << "Invalid JSON format";
        return;
    }

    QJsonObject notesObj = notesDoc.object();
    QJsonArray notesArr = notesObj["notes"].toArray();

    // Update the note for the current section
    for (int i = 0; i < notesArr.size(); ++i)
    {
        QJsonObject noteObj = notesArr[i].toObject();
        if (noteObj["section"].toString() == currentSection)
        {
            noteObj["notes"] = noteEdit->toPlainText();
            notesArr[i] = noteObj; // Save the modified object back to the array
            break;
        }
    }

    // Save the updated JSON back to the file
    notesObj["notes"] = notesArr;
    notesDoc.setObject(notesObj);

    if (!notesFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to reopen notes file for writing";
        return;
    }

    notesFile.write(notesDoc.toJson(QJsonDocument::Indented));
    notesFile.close();
    qDebug() << "Notes saved for section:" << currentSection;
}

void ViewNotes::createNewNote(const QString &newNoteName)
{
    QString notesPath = QDir::currentPath() + "/data/characters/" + name + "/notes.json";

    QFile notesFile(notesPath);
    if (!notesFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open notes file for writing";
        return;
    }

    // Parse the JSON file
    QByteArray notesData = notesFile.readAll();
    notesFile.close();

    QJsonDocument notesDoc = QJsonDocument::fromJson(notesData);
    if (!notesDoc.isObject())
    {
        qDebug() << "Invalid JSON format";
        return;
    }

    QJsonObject notesObj = notesDoc.object();
    QJsonArray notesArr = notesObj["notes"].toArray();

    // Add the new note
    QJsonObject newNote;
    newNote["section"] = newNoteName;
    newNote["notes"] = "";
    newNote["lastUpdated"] = QDateTime::currentDateTime().toString("dd-MM-yyyy|hh:mm:ss");

    notesArr.append(newNote);
    notesObj["notes"] = notesArr;

    // Write back to the file
    notesDoc.setObject(notesObj);
    if (!notesFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to reopen notes file for writing";
        return;
    }

    notesFile.write(notesDoc.toJson(QJsonDocument::Indented));
    notesFile.close();
    qDebug() << "New note created:" << newNoteName;
}

void ViewNotes::deleteNoteSection(const QString &sectionName)
{
    QString notesPath = QDir::currentPath() + "/data/characters/" + name + "/notes.json";

    QFile notesFile(notesPath);
    if (!notesFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open notes file for deletion";
        return;
    }

    QByteArray notesData = notesFile.readAll();
    notesFile.close();

    QJsonDocument notesDoc = QJsonDocument::fromJson(notesData);
    if (!notesDoc.isObject())
    {
        qDebug() << "Invalid JSON format";
        return;
    }

    QJsonObject notesObj = notesDoc.object();
    QJsonArray notesArr = notesObj["notes"].toArray();

    // Remove the note from the JSON array
    for (int i = 0; i < notesArr.size(); ++i)
    {
        QJsonObject noteObj = notesArr[i].toObject();
        if (noteObj["section"].toString() == sectionName) {
            notesArr.removeAt(i);  // Remove the note
            break;
        }
    }

    // Save the updated JSON back to the file
    notesObj["notes"] = notesArr;
    notesDoc.setObject(notesObj);

    if (!notesFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to reopen notes file for writing";
        return;
    }

    notesFile.write(notesDoc.toJson(QJsonDocument::Indented));
    notesFile.close();
    qDebug() << "Note deleted:" << sectionName;

    loadNotes(); // Reload the notes list after deletion
}


ViewNotes::ViewNotes(QWidget *parent, QString name) :
    QWidget(parent)
{
    this->name = name;

    // Initialize the timer
    saveTimer = new QTimer(this);
    connect(saveTimer, &QTimer::timeout, this, &ViewNotes::saveCurrentNote);

    // Start the timer to trigger every 10 seconds (10,000 milliseconds)
    saveTimer->start(10000);

    // Create a row for the navbar
    QWidget *navbar = new QWidget();
    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbar->setFixedHeight(40); // sets the height of the navbar to 40px to make sure it doesnt take up half the screen

    // Create buttons to go back to the character page
    QPushButton *backToCharacter = new QPushButton("Return to Character");
    // Create header label with character's name
    QLabel *notesLabel = new QLabel(name + "'s Notes");

    // Add buttons to navbar
    navbarLayout->addWidget(backToCharacter);
    navbarLayout->addWidget(notesLabel, 1, Qt::AlignCenter);
    
    // Create the layout for the inventory page
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create body widget
    QWidget *body = new QWidget();
    QHBoxLayout *bodyLayout = new QHBoxLayout(body);

    // Create a container for the notes list
    QWidget *notesListContainer = new QWidget();
    QVBoxLayout *notesListContainerLayout = new QVBoxLayout(notesListContainer);
    notesListContainer->setFixedWidth(400); // Set the width of the notes list container to 400px

    // Create a label for the notes list
    QLabel *notesListLabel = new QLabel("Notes List");
    notesListLabel->setAlignment(Qt::AlignCenter);
    notesListLabel->setFixedHeight(50);

    // Get rid of padding in the items in the list and set the height to 30px
    notesList->setStyleSheet("QListWidget::item { padding: 0px; height: 30px; }");


    // Add the label and list to the notes list container
    notesListContainerLayout->addWidget(notesListLabel);
    notesListContainerLayout->addWidget(notesList);

    // Load the notes
    loadNotes();

    // Create a label where the title of the notes will be displayed
    QLabel *notesTitle = new QLabel("Notes");
    notesTitle->setAlignment(Qt::AlignCenter);
    notesTitle->setFixedHeight(50);

    // Create notes edit container
    QWidget *notesEditContainer = new QWidget();
    QVBoxLayout *notesEditContainerLayout = new QVBoxLayout(notesEditContainer);

    // Add the title and text edit to the notes edit container
    notesEditContainerLayout->addWidget(notesTitle);
    notesEditContainerLayout->addWidget(noteEdit);

    // Allow the text edit to take up the rest of the space
    notesEditContainerLayout->setStretch(1, 1);

    
    // Add the widgets to the body layout
    bodyLayout->addWidget(notesListContainer);
    bodyLayout->addWidget(notesEditContainer);

    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));

    connect(notesList, &QListWidget::itemClicked, this, &ViewNotes::onNoteSelected);

    // Just a test to see if the delete function works
    // connect(notesList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item)
    // {
    //     QString sectionName = item->text();
    //     if (sectionName != "Create a New Note")
    //     {
    //         deleteNoteSection(sectionName);
    //     }
    // });

    reloadTheme(); // Reload the theme after everything is placed

}

void ViewNotes::goBack()
{
    // Save the current note before going back
    saveCurrentNote();
    loadNotes();

    QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    
    if (mainStackedWidget)
    {
        mainStackedWidget->setCurrentIndex(0); // Switch to CharacterSelect (index 0)
    }
}

ViewNotes::~ViewNotes()
{
    if (saveTimer->isActive())
    {
        saveTimer->stop();
    }
    delete saveTimer;
}