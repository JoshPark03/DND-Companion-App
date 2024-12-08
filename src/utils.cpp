/*
Name: utils.cpp
Description: Utility functions.
Authors: Chanu Lee
Other Sources: ...
Date Created: 12/7/2024
Last Modified: 12/7/2024
*/

#include <QString>
#include <QList>

// Convert list of strings to comma delimited string
QString listToCommaString(const QList<QString> strings)
{
    QString result;

    // Iterate through the list, joining strings with commas
    for (qsizetype i = 0; i < strings.size(); ++i)
    {
        result += strings[i];
        if (i != strings.size() - 1)
        {
            result += ",";
        }
    }

    return result;
}
