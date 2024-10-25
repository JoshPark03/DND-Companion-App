#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLayout>
#include <QLabel>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QMessageBox>

class ViewCharacter : public QWidget {
public:
	explicit ViewCharacter(QWidget * parent = 0, QString name = "");
    ~ViewCharacter();

private:
    void loadCharacter(QString name);
};