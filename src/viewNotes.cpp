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

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>


// Function to animate delete button
void ViewNotes::animateDeleteButton(QPushButton *button, int startHeight, int endHeight, int duration)
{
    // Get the current height of the widget if maximumHeight is unset
    if (startHeight >= 50000)
    {
        startHeight = button->height(); // Use the actual height of the widget
    }
    button->setMaximumHeight(startHeight);
    QPropertyAnimation *animation = new QPropertyAnimation(button, "maximumHeight");
    animation->setDuration(duration); // Set the duration of the animation
    animation->setStartValue(startHeight); // Set the start value of the animation
    animation->setEndValue(startHeight + endHeight); // Set the end value of the animation
    animation->setEasingCurve(QEasingCurve::InOutQuad); // Smooth easing
    animation->start(QAbstractAnimation::DeleteWhenStopped); // Auto-delete after completion

    reloadTheme(); // Reload the theme after everything is placed
}

void ViewNotes::addDeleteButton()
{
    if(deleteButton->parentWidget() == notesListContainer) // If the delete button is already in the layout
    {
        return; // Skip adding the delete button
    }

    if (notesListContainerLayout->indexOf(deleteButton) == -1) // If the delete button is not in the layout
    {
        notesListContainerLayout->addWidget(deleteButton); // Add the delete button to the layout
        deleteButton->show(); // Show the button
    }

    deleteButton->setStyleSheet(
        "QPushButton {"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   text-align: center;"
        "}"
    );

    // Add the delete button to the layout
    deleteButton->setMaximumHeight(0);
    notesListContainerLayout->addWidget(deleteButton);
    animateDeleteButton(deleteButton, 0, 40);
}

void ViewNotes::removeDeleteButton()
{
    if (deleteButton->parentWidget() == nullptr)
    {
        return;
    }

    // Shrink the delete button
    animateDeleteButton(deleteButton, 40, -40);

    QTimer::singleShot(1000, this, [=]()
    {
        if (notesListContainerLayout->indexOf(deleteButton) != -1)
        {
            notesListContainerLayout->removeWidget(deleteButton); // Remove the button from the layout
        }
        deleteButton->hide(); // Hide the button
        deleteButton->setParent(nullptr); // Remove the parent
    });
}

void ViewNotes::loadNotes()
{
    noteEdit->setEnabled(false); // Disable the text edit by default

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

        // Connect for loss of focus of new note widget
        connect(noteNameEdit, &QLineEdit::editingFinished, this, [=]()
        {
            checkButton->click(); // Automatically confirm the new note on loss of focus
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
            noteEdit->setEnabled(true); // Enable the text edit for editing
            currentSection = newSectionName; // Update the current section
            addDeleteButton(); // Add the delete button to the layout
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

    removeDeleteButton(); // Remove the delete button from the layout
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
    notesListContainer->setFixedWidth(400); // Set the width of the notes list container to 400px

    // Create a label for the notes list
    QLabel *notesListLabel = new QLabel("Notes List");
    notesListLabel->setAlignment(Qt::AlignCenter);
    notesListLabel->setFixedHeight(50);

    // Get rid of padding in the items in the list and set the height to 30px
    notesList->setStyleSheet("QListWidget::item { padding: 0px; height: 30px; }");


    // Add the label and list to the notes list container
    notesListContainerLayout->addWidget(notesListLabel, 0, Qt::AlignTop);
    notesListContainerLayout->addWidget(notesList);
    notesListContainerLayout->setStretch(1, 1); // Allow the list to take up the rest of the space
    notesListContainerLayout->addStretch(); // Add a stretch to push the list to the top

    notesListContainerLayout->setContentsMargins(5, 0, 5, 0);

    // Load the notes
    loadNotes();
    
    // Add the widgets to the body layout
    bodyLayout->addWidget(notesListContainer);
    bodyLayout->addWidget(noteEdit);

    // Add the widgets to the main layout
    layout->addWidget(navbar);
    layout->addWidget(body);

    // Connect functions
    connect(backToCharacter, SIGNAL(clicked()), SLOT(goBack()));

    connect(notesList, &QListWidget::itemClicked, this, &ViewNotes::onNoteSelected);

    connect(deleteButton, &QPushButton::clicked, this, [=]()
    {
        deleteNoteSection(currentSection);
    });

    reloadTheme(); // Reload the theme after everything is placed
}

void ViewNotes::goBack()
{
    // Save the current note before going back
    saveCurrentNote();
    loadNotes();
    notesList->clearSelection();
    notesList->clearFocus();
    currentSection = "";
    removeDeleteButton();
    noteEdit->setEnabled(false);

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