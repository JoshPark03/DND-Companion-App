#ifndef VIEWCHARACTER_H
#define VIEWCHARACTER_H

#include <QWidget>

class ViewCharacter : public QWidget
{
public:
    explicit ViewCharacter(QWidget *parent = 0, QString name = "");
    ~ViewCharacter();

private:
    void loadCharacter(QString name);
};

#endif // SETTINGS_H
