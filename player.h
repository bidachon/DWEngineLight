#ifndef PLAYER
#define PLAYER

#include <QMap>
#include <QDebug>

class Weapon
{
public:
    Weapon():_name("Sword"),_armourBypass("D8"),_dammage(4){}
    Weapon(QString name, QString armourBypass, int dammage):_name(name),_armourBypass(armourBypass),_dammage(dammage){}
    QString _name;
    QString _armourBypass;
    int _dammage;

    QString toString(int bonus = 0) {
        QString str;
        if (bonus == 0)
            str = QString("%1 (%2) %3").arg(_name).arg(_armourBypass).arg(_dammage);
        else
            str = QString("%1 (%2+%3) %4").arg(_name).arg(_armourBypass).arg(bonus).arg(_dammage+bonus);
        return str;
    }
};


class Player {
public:
    Player(QString name){
        _name = name;
        _defaultAttack = 12;
        _defence = 5;
        _defaultBonus = 0;
        _healthpoints = 10;
        _armour = 4;
        _hasShield = false;
        Weapon w;
        _lastWeaponUsed = w.toString();
        _lastDefenderName = _name;//set last defender to self
    }

    QString name() const {return _name;}
    QString lastWeaponUsed() {return _lastWeaponUsed;}
    QString lastDefenderName() {return _lastDefenderName;}
    void setDefenderName(QString def) {_lastDefenderName = def;}
    int healthPoints() const {return _healthpoints;}
    void takeHit(int pdv) {_healthpoints-=pdv;}
    void setHealthPoints(int hp) {_healthpoints = hp;}
    bool hasShield() const { return _hasShield;}
    void setShield(bool hs) {_hasShield = hs;}

    int attack(QString weapon)
    {
        if (_attacks.count(weapon) == 0 )
            _attacks[weapon] = _defaultAttack;
        _lastWeaponUsed = weapon;
        return _attacks[weapon];
    }
    int bonus(QString weapon)
    {
        if (_bonuses.count(weapon) == 0 )
            _bonuses[weapon] = _defaultBonus;
        return _bonuses[weapon];
    }

    void setAttack(QString weapon, int attack)
    {
        _attacks[weapon] = attack;
    }

    void setBonus(QString weapon, int bonus)
    {
        _bonuses[weapon] = bonus;
    }

    int defence() const
    {
        return _defence;
    }

    int armour() const
    {
        return _armour;
    }

    void setArmour(int armour)
    {
        _armour = armour;
    }

    void setDefence(int defence){_defence = defence;}

    int _healthpoints;
    int _armour;
    bool _hasShield;
    QString _name;

private:
    int _defaultAttack;
    int _defaultBonus;
    int _defence;
    QString _lastWeaponUsed;
    QString _lastDefenderName;
    QMap<QString, int> _attacks;
    QMap<QString, int> _bonuses;
};

#endif // PLAYER

