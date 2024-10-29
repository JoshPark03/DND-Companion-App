/*
Name: settings.h
Description: Basic settings page to allow the user to import/export characters and return to character select.
Authors: Carson Treece
Other Sources: ...
Date Created: 10/24/2024
Last Modified: 10/25/2024
*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
};

#endif // SETTINGS_H
