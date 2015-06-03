#include "combatroundcontroller.h"
#include "hitrolldialog.h"
#include <enginioclient.h>
#include <QFile>

CombatRoundController::CombatRoundController(QObject *parent) : QObject(parent)
{
_model = new CombatRoundModel(this);
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
    _currentAttacker = NULL;
    _currentDefender = NULL;


    connect(_combatRoundWidget,SIGNAL(attackerHPUpdated(int)),this,SLOT(attackerHPUpdated(int)));
    connect(_combatRoundWidget,SIGNAL(defenderHPUpdated(int)),this,SLOT(defenderHPUpdated(int)));
    connect(_combatRoundWidget,SIGNAL(defenderShieldUpdated(bool)),this,SLOT(defenderShieldUpdated(bool)));
    connect(_combatRoundWidget,SIGNAL(bonusUpdated(int)),this,SLOT(bonusUpdated(int)));

    connect(_combatRoundWidget,SIGNAL(playRoundClicked()),this,SLOT(playRound()));

    connect(_combatRoundWidget,SIGNAL(newAttackerAdded(QString)),this,SLOT(newAttackerAdded(QString)));

    connect(_combatRoundWidget,SIGNAL(weaponSelected(QString)),this,SLOT(weaponSelected(QString)));
    connect(_combatRoundWidget,SIGNAL(attackUpdated(int)),this,SLOT(attackUpdated(int)));
    connect(_combatRoundWidget,SIGNAL(defenderSelected(QString)),this,SLOT(defenderSelected(QString)));
    connect(_combatRoundWidget,SIGNAL(attackerSelected(QString)),this,SLOT(attackerSelected(QString)));

    connect(_combatRoundWidget,SIGNAL(defenceUpdated(int)),this,SLOT(defenceUpdated(int)));

    connect(_combatRoundWidget,SIGNAL(clearClicked()),this,SLOT(clearDeadPeople()));

    connect(_model,SIGNAL(finished()),this,SLOT(generateWeaponList()));
    _model->fetchData();

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
    QMap<QString, Player*>::iterator i = _players.begin();
    while (i != _players.end()) {
        if (i.value()->healthPoints() <= -3)
            i = _players.erase(i);
        else
            ++i;
    }
    i = _players.begin();
    while (i != _players.end()) {
        _combatRoundWidget->addPlayer(i.key());
        ++i;
    }

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


void CombatRoundController::attackerSelected(const QString &attacker)
{
    if (attacker.isEmpty())
        return;
    qDebug() << "Attacker selected: "<< attacker;

    _currentAttacker = _players[attacker];

    if (!_currentAttacker)
        return;

    _combatRoundWidget->setHealthPointsAttacker(_currentAttacker->healthPoints());
    QString lastWeapon = _currentAttacker->lastWeaponUsed();
    qDebug() << "Last weapon used for player " << attacker << " is " << lastWeapon;

    _combatRoundWidget->setCurrentWeapon(lastWeapon);
    _combatRoundWidget->setCurrentBonus(_currentAttacker->bonus(lastWeapon));

    this->weaponSelected(lastWeapon);//force update if lastWeapon is already the selected weapon but code executed twice then
    _currentAttackerWeapon = _model->weapons()[lastWeapon];
    assert(_currentAttackerWeapon);
    _currentDefender = _players[_currentAttacker->lastDefenderName()];

    if (_currentDefender)
    {
        _combatRoundWidget->setCurrentDefender(_currentDefender->name());
        this->defenderSelected(_currentDefender->name());
    }


    _combatRoundWidget->enableWeapons(true);
    _combatRoundWidget->enableHitRoll(true);
    //_combatRoundWidget->enableHealthPointsAttacker(true);



}

void CombatRoundController::newAttackerAdded(QString newAttacker)
{
    //qDebug() << "newAttacker Added";
    if (_players.count(newAttacker) ==0)
    {
        Player *p = new Player(newAttacker);
        _players[newAttacker] = p;
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

    if (_players.count(newDefender) ==0)
    {
        Player *p = new Player(newDefender);
        _players[newDefender] = p;
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

void CombatRoundController::defenderSelected(const QString &defender)
{
    if(defender.isEmpty())
        return;
    _currentDefender = _players[defender];
    if (!_currentDefender || !_currentAttacker)
        return;

    qDebug() << "Defender Selected: "<< defender;

    qDebug() << _players.count(_currentDefender->name());
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
