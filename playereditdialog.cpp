#include "playereditdialog.h"
#include "ui_playereditdialog.h"

PlayerEditDialog::PlayerEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerEditDialog)
{
    ui->setupUi(this);
    connect(ui->_attack,SIGNAL(valueChanged(int)),this,SLOT(attackUpdated(int)));
    connect(ui->_defense,SIGNAL(valueChanged(int)),this,SLOT(defenseUpdated(int)));
    connect(ui->_healthPoints,SIGNAL(valueChanged(int)),this,SLOT(healthPointsUpdated(int)));

}

void PlayerEditDialog::setPlayer(Player* p)
{
    if (!p)
        return;
    _player = p;
    ui->_name->setText(p->name());
    ui->_attack->setValue(p->defaultAttack());
    ui->_defense->setValue(p->defence());
    ui->_healthPoints->setValue(p->healthPoints());
}

PlayerEditDialog::~PlayerEditDialog()
{
    delete ui;
}

void PlayerEditDialog::attackUpdated(int i)
{
    _player->setDefaultAttack(i);
}

void PlayerEditDialog::defenseUpdated(int i)
{
_player->setDefence(i);
}

void PlayerEditDialog::healthPointsUpdated(int i)
{
_player->setHealthPoints(i);
}
