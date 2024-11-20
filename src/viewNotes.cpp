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
}


ViewNotes::ViewNotes(QWidget *parent, QString name) :
    QWidget(parent)
{
    this->name = name;
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


    reloadTheme(); // Reload the theme after everything is placed

}

void ViewNotes::goBack()
{
    QStackedWidget *mainStackedWidget = qobject_cast<QStackedWidget *>(this->parentWidget());
    
    if (mainStackedWidget)
    {
        mainStackedWidget->setCurrentIndex(0); // Switch to CharacterSelect (index 0)
    }
}

ViewNotes::~ViewNotes()
{
}