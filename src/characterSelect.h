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

class CharacterSelect : public QWidget {
public:
	explicit CharacterSelect(QWidget * parent = 0);

	void addCharacter(QListWidget & characters);
	void deleteCharacter(QListWidget & characters);
	void loadCharacterList(QListWidget & characters);

};