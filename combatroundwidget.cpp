#include "combatroundwidget.h"
#include "ui_combatroundwidget.h"
#include <QListWidgetItem>
#include <QLineEdit>
#include <QString>
#include <QDebug>
#include <assert.h>
#include <QSignalMapper>

CombatRoundWidget::CombatRoundWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::CombatRoundWidget)
{
    _selectedTarget = 0;

    _ui->setupUi(this);
    for (int i = 10 ; i < 30; i++)
        QListWidgetItem *item = new QListWidgetItem(QString("%1").arg(i),_ui->_attack);

    for (int i = 1 ; i < 20; i++)
        QListWidgetItem *item = new QListWidgetItem(QString("%1").arg(i),_ui->_defence);

connectAll();

    //_ui->_healthPointsAttacker->toggle();

}

void CombatRoundWidget::connectAll()
{
    connect(_ui->_newAttacker,SIGNAL(returnPressed()),this,SLOT(addNewAttacker()));
    connect(_ui->_increase,SIGNAL(clicked()),this,SLOT(increaseValue()));
    connect(_ui->_decrease,SIGNAL(clicked()),this,SLOT(decreaseValue()));
    connect(_ui->_hasShield,SIGNAL(toggled(bool)),this,SIGNAL(defenderShieldUpdated(bool)));
    connect(_ui->_hitRoll,SIGNAL(clicked()),this,SIGNAL(playRoundClicked()));
    connect(_ui->_attackers,SIGNAL(currentTextChanged(QString)),this,SIGNAL(attackerSelected(QString)));
    connect(_ui->_defenders,SIGNAL(currentTextChanged(QString)),this,SIGNAL(defenderSelected(QString)));
    connect(_ui->_weapons,SIGNAL(currentTextChanged(QString)),this,SIGNAL(weaponSelected(QString)));
    connect(_ui->_attack,SIGNAL(currentTextChanged(QString)),this,SLOT(attackStrUpdated(QString)));
    connect(_ui->_defence,SIGNAL(currentTextChanged(QString)),this,SLOT(defenceStrUpdated(QString)));
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
    connect(_ui->_clear,SIGNAL(clicked()),this,SIGNAL(clearClicked()));
}

void CombatRoundWidget::disconnectAll()
{
    disconnect(_ui->_newAttacker,SIGNAL(returnPressed()),this,SLOT(addNewAttacker()));
    disconnect(_ui->_increase,SIGNAL(clicked()),this,SLOT(increaseValue()));
    disconnect(_ui->_decrease,SIGNAL(clicked()),this,SLOT(decreaseValue()));
    disconnect(_ui->_hasShield,SIGNAL(toggled(bool)),this,SIGNAL(defenderShieldUpdated(bool)));
    disconnect(_ui->_hitRoll,SIGNAL(clicked()),this,SIGNAL(playRoundClicked()));
    disconnect(_ui->_attackers,SIGNAL(currentTextChanged(QString)),this,SIGNAL(attackerSelected(QString)));
    disconnect(_ui->_defenders,SIGNAL(currentTextChanged(QString)),this,SIGNAL(defenderSelected(QString)));
    disconnect(_ui->_weapons,SIGNAL(currentTextChanged(QString)),this,SIGNAL(weaponSelected(QString)));
    disconnect(_ui->_attack,SIGNAL(currentTextChanged(QString)),this,SLOT(attackStrUpdated(QString)));
    disconnect(_ui->_defence,SIGNAL(currentTextChanged(QString)),this,SLOT(defenceStrUpdated(QString)));
    disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
    disconnect(_ui->_clear,SIGNAL(clicked()),this,SIGNAL(clearClicked()));
}

void CombatRoundWidget::setCurrentBonus(int bonus)
{
    _ui->_bonus->display(bonus);
}

void CombatRoundWidget::focusChanged(QWidget*oldwidget,QWidget *newwidget)
{
    if (!newwidget)
        return;
    qDebug() << "Focus changed to " << newwidget->objectName();
    if (newwidget->objectName() == "_attackers")
        handleSelection(0);
    else if (newwidget->objectName() == "_defenders")
        handleSelection(1);
    else if(newwidget->objectName() == "_weapons")
        handleSelection(2);
    else
    {
        _ui->_increase->setEnabled(false);
        _ui->_decrease->setEnabled(false);
        return;
    }
            _ui->_increase->setEnabled(true);
             _ui->_decrease->setEnabled(true);
    return;
}

void CombatRoundWidget::handleSelection(int target)
{
    qDebug() << "handleSelection " << target;
    _selectedTarget = target;
    return;
}

void CombatRoundWidget::increaseValue()
{
    int value = 0;
    switch(_selectedTarget)
    {
    case 0://attacker HP
        value = _ui->_healthPointsAttacker->value();
        value++;
        _ui->_healthPointsAttacker->display(value);
        emit this->attackerHPUpdated(value);
        break;
    case 1://defender HP
        value = _ui->_healthPointsDefender->value();
        value++;
        _ui->_healthPointsDefender->display(value);
        emit this->defenderHPUpdated(value);
        break;
    case 2://bonus
        value = _ui->_bonus->value();
        value++;
        _ui->_bonus->display(value);
        emit this->bonusUpdated(value);
        break;
    default:
        break;
    }
    return;

}


void CombatRoundWidget::decreaseValue()
{
    int value = 0;
    switch(_selectedTarget)
    {
    case 0://attacker HP
        value = _ui->_healthPointsAttacker->value();
        value--;
        _ui->_healthPointsAttacker->display(value);
        emit this->attackerHPUpdated(value);
        break;
    case 1://defender HP
        value = _ui->_healthPointsDefender->value();
        value--;
        _ui->_healthPointsDefender->display(value);
        emit this->defenderHPUpdated(value);
        break;
    case 2://bonus
        value = _ui->_bonus->value();
        value--;
        _ui->_bonus->display(value);
        emit this->bonusUpdated(value);
        break;
    default:
        break;
    }
}

CombatRoundWidget::~CombatRoundWidget()
{
    delete _ui;
}

void CombatRoundWidget::addWeapon(QString weapon){
    QListWidgetItem *item = new QListWidgetItem(weapon,_ui->_weapons);
}

void CombatRoundWidget::clearPlayers()
{
disconnectAll();
    _ui->_attackers->clear();
    _ui->_defenders->clear();
    _ui->_weapons->setEnabled(false);
    _ui->_defenders->setEnabled(false);
    _ui->_attack->setEnabled(false);
    _ui->_defence->setEnabled(false);
    _ui->_hasShield->setEnabled(false);
    _ui->_hitRoll->setEnabled(false);

connectAll();

}

void CombatRoundWidget::addPlayer(QString newAttacker)
{
    if (_ui->_attackers->findItems(newAttacker,Qt::MatchFixedString).size() != 0)
        return;
    emit newAttackerAdded(newAttacker);
    QListWidgetItem *item = new QListWidgetItem(newAttacker);
    _ui->_attackers->addItem(item);
    _ui->_defenders->addItem(item->clone());
    _ui->_attackers->setCurrentItem(item);
}

void CombatRoundWidget::addNewAttacker()
{
    QString newAttacker = _ui->_newAttacker->text();
    _ui->_newAttacker->clear();
    if (newAttacker.isEmpty())
        return;
    addPlayer(newAttacker);

}

/*void CombatRoundWidget::addNewDefender()
{
    QString newDefender = _ui->_newDefender->text();
    _ui->_newDefender->clear();
    if (newDefender.isEmpty())
        return;
    //we do have to search in _attacker list anyways.
    if (_ui->_attackers->findItems(newDefender,Qt::MatchFixedString).size() != 0)
        return;
    QListWidgetItem *item = new QListWidgetItem(newDefender);
    _ui->_attackers->addItem(item->clone());
    _ui->_defenders->addItem(item);
    _ui->_defenders->setCurrentItem(item);
    emit newDefenderAdded(newDefender);
}*/

void CombatRoundWidget::attackStrUpdated(QString str)
{
    int attack = str.toInt();
    emit attackUpdated(attack);
}

void CombatRoundWidget::defenceStrUpdated(QString str)
{
    int defence = str.toInt();
    emit defenceUpdated(defence);
}

void CombatRoundWidget::setCurrentWeapon(QString lastWeapon)
{
    QList<QListWidgetItem *> list = _ui->_weapons->findItems(lastWeapon,Qt::MatchExactly);
    assert(list.size() != 0);
    _ui->_weapons->setCurrentItem(list.at(0));
}

void CombatRoundWidget::setCurrentDefender(QString defenderName)
{
    QList<QListWidgetItem *> list = _ui->_defenders->findItems(defenderName,Qt::MatchExactly);
    assert(list.size() != 0);
    _ui->_defenders->setCurrentItem(list.at(0));
    _ui->_defence->setEnabled(true);
    //_ui->_healthPointsDefender->setEnabled(true);
    _ui->_hasShield->setEnabled(true);

}

void CombatRoundWidget::setHealthPointsAttacker(int hp)
{
    _ui->_healthPointsAttacker->display(hp);
}

void CombatRoundWidget::setHealthPointsDefender(int hp)
{
    _ui->_healthPointsDefender->display(hp);
}

void CombatRoundWidget::enableWeapons(bool b)
{
    _ui->_weapons->setEnabled(b);
}

void CombatRoundWidget::enableHitRoll(bool b)
{
    _ui->_hitRoll->setEnabled(b);
}

/*void CombatRoundWidget::enableHealthPointsAttacker(bool b)
{
    _ui->_healthPointsAttacker->setEnabled(b);
}*/

void CombatRoundWidget::setCurrentAttack(int attack)
{
    QList<QListWidgetItem *> list = _ui->_attack->findItems(QString("%1").arg(attack),Qt::MatchExactly);
    assert(list.size() != 0);
    _ui->_attack->setCurrentItem(list.at(0));
    this->attackUpdated(attack);
    _ui->_attack->setEnabled(true);
}

void CombatRoundWidget::setCurrentDefence(int defence)
{
    QList<QListWidgetItem *> list = _ui->_defence->findItems(QString("%1").arg(defence),Qt::MatchExactly);
    assert(list.size() != 0);
    _ui->_defence->setCurrentItem(list.at(0));
    this->defenceUpdated(defence);
    _ui->_defence->setEnabled(true);
}

void CombatRoundWidget::setShield(bool b)
{
    _ui->_hasShield->setChecked(b);
}
