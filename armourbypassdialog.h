#ifndef ARMOURBYPASSDIALOG_H
#define ARMOURBYPASSDIALOG_H

#include <QDialog>
#include <player.h>

namespace Ui {
class ArmourBypassDialog;
}

class ArmourBypassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArmourBypassDialog(QWidget *parent = 0);
    ~ArmourBypassDialog();
    void setParticipants(Player* attacker, Player* defender);
    void setWeapon(Weapon *w);
    void setArmour(int armour);
    void update();

private:
    Ui::ArmourBypassDialog *ui;
    int _armour;
    Weapon *_weapon;
    Player* _attacker;
    Player* _defender;
};

#endif // ARMOURBYPASSDIALOG_H
