#include "armourbypassdialog.h"
#include "ui_armourbypassdialog.h"

ArmourBypassDialog::ArmourBypassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArmourBypassDialog)
{
    ui->setupUi(this);
    _armour = 0;
    _weapon = NULL;
}

ArmourBypassDialog::~ArmourBypassDialog()
{
    delete ui;
}

void ArmourBypassDialog::update()
{
    ui->_attacker->setText(_attacker->name());
    ui->_defender->setText(_defender->name());
    ui->_protection->setText(QString("%1").arg(_armour));
    int bonus = _attacker->bonus(_weapon->toString());
    ui->_weapon->setText(_weapon->toString(bonus));
    QString bonusStr;
    if (bonus != 0)
        bonusStr = "+" + QString::number(bonus);
    if (_weapon)
        ui->_target->setText(QString("greater than\n %1 \n with %2%3").arg(_armour).arg(_weapon->_armourBypass).arg(bonusStr));
}

void ArmourBypassDialog::setParticipants(Player* attacker, Player* defender)
{
    _defender = defender;
    _attacker = attacker;
    update();
}

void ArmourBypassDialog::setWeapon(Weapon *w)
{
    _weapon = w;

}

void ArmourBypassDialog::setArmour(int armour)
{
    _armour = armour;

}
