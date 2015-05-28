#include "hitrolldialog.h"
#include "ui_hitrolldialog.h"

HitRollDialog::HitRollDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HitRollDialog)
{
    ui->setupUi(this);
    _attack = 0;
    _defence = 0;
    _attacker = NULL;
    _defender = NULL;
}

HitRollDialog::~HitRollDialog()
{
    delete ui;
}

void HitRollDialog::setParticipants(Player* attacker, Player* defender)
{
    _attacker = attacker;
    _defender = defender;
    ui->_attacker->setText(QString("%1").arg(attacker->name()));
    ui->_defender->setText(QString("%1").arg(defender->name()));
    updateTarget();
}

void HitRollDialog::setAttack(int attack)
{
    _attack = attack;
    ui->_attack->setText(QString("%1").arg(_attack));
    updateTarget();
}

void HitRollDialog::setDefence(int defence)
{
    _defence = defence;
    ui->_defence->setText(QString("%1").arg(_defence));
    updateTarget();
}

void HitRollDialog::updateTarget()
{
    if (!_attacker)
        return;
    if (_defender->hasShield())
        ui->_shield->setText("Shield hit required\nby defender");
    ui->_target->setText(QString("Less or equal\n than \n %1 with D20").arg(_attack-_defence));
}
