#include "combatroundcontroller.h"
#include "hitrolldialog.h"
#include "armourbypassdialog.h"

CombatRoundController::CombatRoundController(QObject *parent) : QObject(parent)
{

}

CombatRoundController::~CombatRoundController()
{

}

void CombatRoundController::generateWeaponList()
{
  Weapon *p = new Weapon("Arbalest","D12",5);
   _weapons[p->toString()] = p;
   p = new Weapon("Battleaxe","D8",6);
   _weapons[p->toString()] = p;
   p = new Weapon("Bow","D6",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Crossbow","D10",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Cudgel","D3",3);
   _weapons[p->toString()] = p;
   p = new Weapon("Dagger","D4",3);
   _weapons[p->toString()] = p;
   p = new Weapon("Flail","D6",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Footman flail","D6",5);
   _weapons[p->toString()] = p;
   p = new Weapon("Halberd","D10",5);
   _weapons[p->toString()] = p;
   p = new Weapon("Javelin","D8",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Longbow","D8",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Longsword","2D4",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Mace","D6",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Morning star","D6",5);
   _weapons[p->toString()] = p;
   p = new Weapon("Pollaxe","D12",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Rock","D3",2);
   _weapons[p->toString()] = p;
   p = new Weapon("Scourge","D2",3);
   _weapons[p->toString()] = p;
   p = new Weapon("Shortsword","D8",3);
   _weapons[p->toString()] = p;
   p = new Weapon("Sling","D6",3);
   _weapons[p->toString()] = p;
   p = new Weapon("Spear","2D4",4);
   _weapons[p->toString()] = p;
   p = new Weapon("Staff","D6",3);
   _weapons[p->toString()] = p;
   p = new Weapon("Sword","D8",4);
   _weapons[p->toString()] = p;
   p = new Weapon("2H sword","D10",5);
   _weapons[p->toString()] = p;
   p = new Weapon("Thw spike","D2+1",2);
   _weapons[p->toString()] = p;
   p = new Weapon("Unarmed","D3",2);
   _weapons[p->toString()] = p;
   p = new Weapon("Staff","D6",3);
   _weapons[p->toString()] = p;
   p = new Weapon("War axe","D6",5);
   _weapons[p->toString()] = p;
   p = new Weapon("War lance","2D4",6);
   _weapons[p->toString()] = p;
   p = new Weapon("Warhammer","D10",3);
   _weapons[p->toString()] = p;
}

void CombatRoundController::setCombatRoundWidget(CombatRoundWidget *widget)
{
    generateWeaponList();
    _combatRoundWidget = widget;
    _currentAttacker = NULL;
    _currentDefender = NULL;

    QMap<QString, Weapon*>::iterator i;
    for (i = _weapons.begin(); i != _weapons.end(); ++i)
    {
        qDebug() << i.key() << ": " << i.value()->toString() << endl;
        _combatRoundWidget->addWeapon(i.key());
    }

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

}

void CombatRoundController::defenderShieldUpdated(bool s)
{
    qDebug() << "set shield to " << s;
    if (_currentDefender)
        this->_currentDefender->setShield(s);
}

void CombatRoundController::attackerHPUpdated(int hp)
{
    if (!_currentAttacker)
        return;
    _currentAttacker->setHealthPoints(hp);
}

void CombatRoundController::bonusUpdated(int bonus)
{
    if (!_currentAttacker)
        return;
    qDebug() << "Bonus updated:" << bonus;
    assert(_currentAttackerWeapon);
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
    qDebug() << "Attacker selected: "<< attacker;
    _currentAttacker = _players[attacker];


    QString lastWeapon = _currentAttacker->lastWeaponUsed();
    qDebug() << "Last weapon used for player " << attacker << " is " << lastWeapon;

    _combatRoundWidget->setCurrentWeapon(lastWeapon);
    _combatRoundWidget->setCurrentBonus(_currentAttacker->bonus(lastWeapon));

    this->weaponSelected(lastWeapon);//force update if lastWeapon is already the selected weapon but code executed twice then
    _currentAttackerWeapon = _weapons[lastWeapon];
    _currentDefender = _players[_currentAttacker->lastDefenderName()];

    _combatRoundWidget->setCurrentDefender(_currentDefender->name());

    this->defenderSelected(_currentDefender->name());

    _combatRoundWidget->setHealthPointsAttacker(_currentAttacker->healthPoints());

    _combatRoundWidget->enableWeapons(true);
    _combatRoundWidget->enableHitRoll(true);
    _combatRoundWidget->enableHealthPointsAttacker(true);



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
    _currentAttackerWeapon = _weapons[weapon];
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
    assert(_currentAttacker);
    assert(_currentAttackerWeapon);
    _currentAttacker->setAttack(_currentAttackerWeapon->toString(),_currentAttack);
}

void CombatRoundController::defenderSelected(const QString &defender)
{
    if (defender.isEmpty())
    {
        qDebug() << "defender is empty";
        return;
    }
    qDebug() << "Defender Selected: "<< defender;
    _currentDefender = _players[defender];
    assert(_currentAttacker);
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
    assert(_currentDefender);
    _currentDefender->setDefence(defence);
}
