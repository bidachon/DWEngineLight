#ifndef COMBATROUNDWIDGET_H
#define COMBATROUNDWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class CombatRoundWidget;
}

class CombatRoundWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CombatRoundWidget(QWidget *parent = 0);
    ~CombatRoundWidget();
    Ui::CombatRoundWidget* ui() {return _ui;}
    void addWeapon(QString weapon);

    void setCurrentWeapon(QString);
    void setCurrentBonus(int);
    void setCurrentDefender(QString);
    void setHealthPointsAttacker(int);
    void setHealthPointsDefender(int);
    void setCurrentAttack(int);
    void setCurrentDefence(int);
    void setShield(bool);

    void enableWeapons(bool b);
    void enableHitRoll(bool b);
    void enableHealthPointsAttacker(bool b);

signals:
    void attackerHPUpdated(int);
    void defenderHPUpdated(int);

    void defenderShieldUpdated(bool);
    void playRoundClicked();

    void newAttackerAdded(QString);
    //void newDefenderAdded(QString);

    void attackerSelected(QString);
    void defenderSelected(QString);
    void weaponSelected(QString);

    void defenceUpdated(int);
    void attackUpdated(int);
    void bonusUpdated(int);

private slots:
    void addNewAttacker();
    //void addNewDefender();
    void attackStrUpdated(QString);
    void defenceStrUpdated(QString);
    void handleSelection(int item);
    void focusChanged(QWidget*oldwidget,QWidget *newwidget);
    void increaseValue();
    void decreaseValue();


private:
    Ui::CombatRoundWidget* _ui;
    int _selectedTarget;
};

#endif // COMBATROUNDWIDGET_H
