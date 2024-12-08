/*
Name: viewSpells.h
Description: Page to view a character's spells.
Authors: Zachary Craig, Josh Park
Other Sources: ...
Date Created: 11/19/2024
Last Modified: 12/3/2024
*/

#ifndef VIEWSPELLS_H
#define VIEWSPELLS_H

#include <QWidget>
#include <QTableWidget>

class ViewSpells : public QWidget
{
Q_OBJECT
public:
    explicit ViewSpells(QWidget *parent = 0, QString name = "");
    ~ViewSpells();

private:
    QTableWidget * spells;
    QString name;
    QString charPath;
    int level;
    int totalLevel1Slots;
    int totalLevel2Slots;
    int totalLevel3Slots;
    int totalLevel4Slots;
    int totalLevel5Slots;
    int totalLevel6Slots;
    int totalLevel7Slots;
    int totalLevel8Slots;
    int totalLevel9Slots;
    int level1SlotsUsed;
    int level2SlotsUsed;
    int level3SlotsUsed;
    int level4SlotsUsed;
    int level5SlotsUsed;
    int level6SlotsUsed;
    int level7SlotsUsed;
    int level8SlotsUsed;
    int level9SlotsUsed;
    int maxPrepared;
    void loadSpells();
    void addItem(QString line);

public slots:
    void castSpell(int level);
private slots:
    void saveSpells();
    void saveSlots();
    void addSpell();
    void goBack();
};

#endif
