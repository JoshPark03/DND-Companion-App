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

class ViewNotes : public QWidget
{
Q_OBJECT
public:
    explicit ViewNotes(QWidget *parent = 0, QString name = "");
    ~ViewNotes();

private:
    QString name;
    void loadNotes();
    QListWidget *notesList = new QListWidget();
    QTextEdit *noteEdit = new QTextEdit();

private slots:
    void goBack();
};

#endif
