#ifndef PLAYEREDITDIALOG_H
#define PLAYEREDITDIALOG_H

#include <QDialog>
#include "player.h"
namespace Ui {
class PlayerEditDialog;
}

class PlayerEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerEditDialog(QWidget *parent = 0);
    ~PlayerEditDialog();
    void setPlayer(Player*);

public slots:
    void attackUpdated(int);
    void defenseUpdated(int);
    void healthPointsUpdated(int);

private:
    Ui::PlayerEditDialog *ui;
    Player *_player;
};

#endif // PLAYEREDITDIALOG_H
