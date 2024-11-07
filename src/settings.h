#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

<<<<<<< Updated upstream
class Settings : public QWidget
{
=======
class QComboBox;

class Settings : public QWidget {
>>>>>>> Stashed changes
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
<<<<<<< Updated upstream
=======

private slots:
    void changeTheme(const QString &theme);

private:
    QComboBox *themeSelector;
>>>>>>> Stashed changes
};

#endif // SETTINGS_H
