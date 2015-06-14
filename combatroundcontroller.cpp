#include "combatroundcontroller.h"
#include "hitrolldialog.h"
#include "enginioclient.h"
#include "playereditdialog.h"
#include <QFile>

CombatRoundController::CombatRoundController(QObject *parent) : QObject(parent)
{
_model = new CombatRoundModel(this);
_playerModel = new PlayerListModel();
}

CombatRoundController::~CombatRoundController()
{

}

void CombatRoundController::generateWeaponList()
{
    QMap<QString, Weapon*>::iterator i;
    for (i = _model->weapons().begin(); i != _model->weapons().end(); ++i)
    {
        //qDebug() << i.key() << ": " << i.value()->toString() << endl;
        _combatRoundWidget->addWeapon(i.key());
    }
}

void CombatRoundController::setCombatRoundWidget(CombatRoundWidget *widget)
{
    _combatRoundWidget = widget;
    _combatRoundWidget->setPlayerListModel(_playerModel);
    _currentAttacker = NULL;
    _currentDefender = NULL;


    connect(_combatRoundWidget,SIGNAL(attackerHPUpdated(int)),this,SLOT(attackerHPUpdated(int)));
    connect(_combatRoundWidget,SIGNAL(defenderHPUpdated(int)),this,SLOT(defenderHPUpdated(int)));
    connect(_combatRoundWidget,SIGNAL(defenderShieldUpdated(bool)),this,SLOT(defenderShieldUpdated(bool)));
    connect(_combatRoundWidget,SIGNAL(bonusUpdated(int)),this,SLOT(bonusUpdated(int)));

    connect(_combatRoundWidget,SIGNAL(playRoundClicked()),this,SLOT(playRound()));
    connect(_combatRoundWidget,SIGNAL(editPlayer(QModelIndex)),this,SLOT(editPlayer(QModelIndex)));
    connect(_combatRoundWidget,SIGNAL(newAttackerAdded(QString)),this,SLOT(newAttackerAdded(QString)));

    connect(_combatRoundWidget,SIGNAL(weaponSelected(QString)),this,SLOT(weaponSelected(QString)));
    connect(_combatRoundWidget,SIGNAL(attackUpdated(int)),this,SLOT(attackUpdated(int)));
    connect(_combatRoundWidget,SIGNAL(defenderSelected(QModelIndex)),this,SLOT(defenderSelected(QModelIndex)));
    connect(_combatRoundWidget,SIGNAL(attackerSelected(QModelIndex)),this,SLOT(attackerSelected(QModelIndex)));

    connect(_combatRoundWidget,SIGNAL(defenceUpdated(int)),this,SLOT(defenceUpdated(int)));

    connect(_combatRoundWidget,SIGNAL(clearClicked()),this,SLOT(clearDeadPeople()));

    connect(_model,SIGNAL(finished()),this,SLOT(generateWeaponList()));
    _model->fetchData();

}

void CombatRoundController::editPlayer(QModelIndex index)
{
    PlayerEditDialog dialog(_combatRoundWidget);
    Player *p = _playerModel->player(index);
    dialog.setPlayer(p);
    dialog.exec();
}

void CombatRoundController::defenderShieldUpdated(bool s)
{
    qDebug() << "set shield to " << s;
    //if (_currentDefender)
    this->_currentDefender->setShield(s);
}

void CombatRoundController::clearDeadPeople()
{
    //Memory leak
    _currentAttacker = NULL;
    _currentDefender = NULL;
    _currentAttackerWeapon = NULL;
    _combatRoundWidget->clearPlayers();
    _playerModel->clearDeads();

    return;


}

void CombatRoundController::attackerHPUpdated(int hp)
{
    if (!_currentAttacker)
        return;
    _currentAttacker->setHealthPoints(hp);
}

void CombatRoundController::bonusUpdated(int bonus)
{
    if (!_currentAttacker || !_currentAttackerWeapon)
        return;
    qDebug() << "Bonus updated:" << bonus;
    _currentAttacker->setBonus(_currentAttackerWeapon->toString(),bonus);
}

void CombatRoundController::defenderHPUpdated(int hp)
{
    if (!_currentDefender)
        return;
    _currentDefender->setHealthPoints(hp);
}

void CombatRoundController::playRound()
{
    HitRollDialog hitdialog(this->_combatRoundWidget);
    if (!_currentAttacker || !_currentDefender || !_currentAttackerWeapon)
        return;
    hitdialog.setAttack(_currentAttack);
    hitdialog.setDefence(_currentDefence);
    hitdialog.setParticipants(_currentAttacker, _currentDefender);
    if (hitdialog.exec())
    {
        ArmourBypassDialog armourdialog(this->_combatRoundWidget);
        armourdialog.setWeapon(_currentAttackerWeapon);
        armourdialog.setArmour(_currentDefender->armour());
        armourdialog.setParticipants(_currentAttacker,_currentDefender);
        if (armourdialog.exec())
        {
            _currentDefender->takeHit(_currentAttackerWeapon->_dammage+_currentAttacker->bonus(_currentAttackerWeapon->toString()));
        }

    }


}


void CombatRoundController::attackerSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;
    //qDebug() << "Attacker selected: "<< attacker;

    _currentAttacker = _playerModel->player(index);

    if (!_currentAttacker)
        return;

    _combatRoundWidget->setHealthPointsAttacker(_currentAttacker->healthPoints());
    QString lastWeapon = _currentAttacker->lastWeaponUsed();
    qDebug() << "Last weapon used for player " << _currentAttacker->name() << " is " << lastWeapon;

    _combatRoundWidget->setCurrentWeapon(lastWeapon);
    _combatRoundWidget->setCurrentBonus(_currentAttacker->bonus(lastWeapon));

    this->weaponSelected(lastWeapon);//force update if lastWeapon is already the selected weapon but code executed twice then
    _currentAttackerWeapon = _model->weapons()[lastWeapon];
    assert(_currentAttackerWeapon);
    /** @todo this will add a new entry for given lastDefender with an empty Player pointer to change */
    _currentDefender = _playerModel->player(_currentAttacker->lastDefenderName());

    if (_currentDefender)
    {
        QModelIndex index = _playerModel->find(_currentDefender->name());
        _combatRoundWidget->setCurrentDefender(index);
        this->defenderSelected(_playerModel->find(_currentDefender->name()));
    }


    _combatRoundWidget->enableWeapons(true);
    _combatRoundWidget->enableHitRoll(true);
    //_combatRoundWidget->enableHealthPointsAttacker(true);



}

void CombatRoundController::newAttackerAdded(QString newAttacker)
{
    //qDebug() << "newAttacker Added";
    if (!_playerModel->exists(newAttacker))
    {
        Player *p = new Player(newAttacker);
        _playerModel->addPlayer(p);
        qDebug() << "newAttacker added: " << newAttacker;
        _currentAttacker = p;

    }
    else
    {
        qDebug() << "Attacker already exists:" << newAttacker;
    }

}

void CombatRoundController::newDefenderAdded(QString newDefender)
{

    if (!_playerModel->exists(newDefender))
    {
        Player *p = new Player(newDefender);
        _playerModel->addPlayer(p);
        qDebug() << "newDefender added: " << newDefender;
    }
    else
    {
        qDebug() << "Attacker already exists:" << newDefender;
    }
}


void CombatRoundController::weaponSelected(const QString &weapon)
{
    qDebug() << "weapon selected: "<< weapon;
    _currentAttackerWeapon = _model->weapons()[weapon];
    if (!_currentAttacker)
        return;
    assert(_currentAttackerWeapon);
    _currentAttack = _currentAttacker->attack(_currentAttackerWeapon->toString());
    QString str = QString("Attack for player %1 with weapon %2 is %3").arg(_currentAttacker->name()).arg(_currentAttackerWeapon->toString()).arg(_currentAttack);

    _combatRoundWidget->setCurrentAttack(_currentAttack);

}

void CombatRoundController::weaponBonusUpdated(const int &bonus)
{
    qDebug() << "Bonus Updated: "<< bonus;
}

void CombatRoundController::attackUpdated(int attack)
{
    qDebug() << "Attack Updated: "<< attack;
    _currentAttack = attack;
    if (!_currentAttacker || !_currentAttackerWeapon)
        return;

    _currentAttacker->setAttack(_currentAttackerWeapon->toString(),_currentAttack);
}

void CombatRoundController::defenderSelected(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    _currentDefender = _playerModel->player(index);
    if (!_currentDefender || !_currentAttacker)
        return;

    qDebug() << "Defender Selected: "<< _currentDefender->name();

    _currentAttacker->setDefenderName(_currentDefender->name());
    _currentDefence = _currentDefender->defence();
    _combatRoundWidget->setHealthPointsDefender(_currentDefender->healthPoints());
    _combatRoundWidget->setShield(_currentDefender->hasShield());
    _combatRoundWidget->setCurrentDefence(_currentDefence);


}

void CombatRoundController::defenceUpdated(int defence)
{
    qDebug() << "Defence Updated: "<< defence;
    _currentDefence = defence;
    if (!_currentDefender)
        return;
    _currentDefender->setDefence(defence);
}
