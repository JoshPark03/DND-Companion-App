/*
Name: themeUtils.h
Description: Utility functions for loading and reloading the application theme.
Authors: Zachary Craig
Other Sources: ...
Date Created: 11/18/2024
Last Modified: 11/18/2024
*/

#ifndef THEMEUTILS_H
#define THEMEUTILS_H

#include <QApplication>
#include <QFile>
#include <QDebug>

inline void loadTheme(QApplication &app) {
    QString theme;

    // Read the selected theme from the file
    QFile themeFile("src/themes/selectedTheme.txt");
    if (themeFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&themeFile);
        theme = in.readLine();
        themeFile.close();
    }

    // Determine the QSS file based on the selected theme
    QString qssFile;
    if (theme == "Light Mode") {
        qssFile = "src/themes/lightMode.qss";
    } else if (theme == "Dark Mode") {
        qssFile = "src/themes/darkMode.qss";
    }

    // Apply the theme
    QFile file(qssFile);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        file.close();
    } else {
        qDebug() << "Failed to open QSS file:" << qssFile;
    }
}

// Reload the application theme
inline void reloadTheme() {
    QApplication *app = qobject_cast<QApplication *>(QCoreApplication::instance());
    if (app) {
        loadTheme(*app);
    } else {
        qDebug() << "Failed to reload theme: QApplication instance not found.";
    }
}

#endif
