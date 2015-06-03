#ifndef COMBATROUNDCONTROLLER_H
#define COMBATROUNDCONTROLLER_H

#include <QObject>
#include "combatroundwidget.h"
#include "player.h"
#include <QDebug>
#include <QList>
#include <QListWidgetItem>
#include <assert.h>
#include "hitrolldialog.h"
#include "armourbypassdialog.h"
#include "combatroundmodel.h"

class CombatRoundController : public QObject
{
    Q_OBJECT
public:
    explicit CombatRoundController(QObject *parent = 0);
    ~CombatRoundController();
    void setCombatRoundWidget(CombatRoundWidget *);

signals:

public slots:

    void attackerSelected(const QString &attacker);

    void newAttackerAdded(QString);

    void newDefenderAdded(QString);

    void weaponSelected(const QString &weapon);

    void weaponBonusUpdated(const int &bonus);

    void attackUpdated(int attack);

    void defenderSelected(const QString &defender);

    void defenceUpdated(int defence);

    void attackerHPUpdated(int);

    void defenderHPUpdated(int);

    void bonusUpdated(int);

    void defenderShieldUpdated(bool);

    void clearDeadPeople();

    void playRound();

private slots:
    void generateWeaponList();

private:

    CombatRoundWidget *_combatRoundWidget;
    QMap<QString,Player*> _players;
    Player* _currentAttacker;
    Weapon* _currentAttackerWeapon;
    int _currentAttack;
    Player* _currentDefender;
    int _currentDefence;
    CombatRoundModel *_model;
};

#endif // COMBATROUNDCONTROLLER_H
