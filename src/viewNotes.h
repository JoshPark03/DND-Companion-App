/*
Name: viewNotes.h
Description: Header file for the viewNotes class, which displays a character's notes.
Authors: Zachary Craig
Other Sources: ...
Date Created: 11/18/2024
Last Modified: 11/18/2024
*/

#ifndef VIEWNOTES_H
#define VIEWNOTES_H

#include <QWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QPushButton>

class ViewNotes : public QWidget
{
Q_OBJECT
public:
    explicit ViewNotes(QWidget *parent = 0, QString name = "");
    ~ViewNotes();

private:
    QString name;
    QString currentSection;
    QTimer *saveTimer;
    void loadNotes();
    void onNoteSelected(QListWidgetItem *item);
    void saveCurrentNote();
    void createNewNote(const QString &newNoteName);
    void deleteNoteSection(const QString &sectionName);
    void animateDeleteButton(QPushButton *button, int startHeight, int endHeight, int duration = 300);
    void addDeleteButton();
    void removeDeleteButton();
    QListWidget *notesList = new QListWidget();
    QWidget *notesListContainer = new QWidget();
    QVBoxLayout *notesListContainerLayout = new QVBoxLayout(notesListContainer);
    QTextEdit *noteEdit = new QTextEdit();
    QPushButton *deleteButton = new QPushButton("Delete Note");

private slots:
    void goBack();
};

#endif
