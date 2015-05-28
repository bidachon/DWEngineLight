#ifndef HITROLLDIALOG_H
#define HITROLLDIALOG_H

#include <QDialog>
#include "player.h"

namespace Ui {
class HitRollDialog;
}

class HitRollDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HitRollDialog(QWidget *parent = 0);
    ~HitRollDialog();
    void setAttack(int attack);
    void setDefence(int defence);
    void setParticipants(Player* attacker, Player* defender);


private:
    void updateTarget();
    Ui::HitRollDialog *ui;
    int _attack;
    int _defence;
    Player* _attacker;
    Player* _defender;
};

#endif // HITROLLDIALOG_H
