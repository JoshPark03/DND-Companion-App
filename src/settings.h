#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class QComboBox;

class Settings : public QWidget {
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void changeTheme(const QString &theme);

private:
    QComboBox *themeSelector;
    QString loadSavedTheme() const;
};

#endif // SETTINGS_H
