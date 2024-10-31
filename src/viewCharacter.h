#ifndef VIEWCHARACTER_H
#define VIEWCHARACTER_H

#include <QWidget>

class ViewCharacter : public QWidget
{
public:
    explicit ViewCharacter(QWidget *parent = 0, QString name = "");
    ~ViewCharacter();
    void printCharacterToConsole(std::string path);

private:
    void loadCharacter(QString name);
};

#endif // SETTINGS_H
