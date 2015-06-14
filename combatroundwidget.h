#ifndef COMBATROUNDWIDGET_H
#define COMBATROUNDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QAbstractListModel>

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
    void clearPlayers();
    void setPlayerListModel(QAbstractListModel *);

    void setCurrentWeapon(QString);
    void setCurrentBonus(int);
    void setCurrentDefender(QModelIndex);
    void setHealthPointsAttacker(int);
    void setHealthPointsDefender(int);
    void setCurrentAttack(int);
    void setCurrentDefence(int);
    void setShield(bool);

    void enableWeapons(bool b);
    void enableHitRoll(bool b);
    //void enableHealthPointsAttacker(bool b);

signals:
    void attackerHPUpdated(int);
    void defenderHPUpdated(int);

    void defenderShieldUpdated(bool);
    void playRoundClicked();
    void clearClicked();

    void newAttackerAdded(QString);
    //void newDefenderAdded(QString);

    void attackerSelected(QModelIndex);
    void defenderSelected(QModelIndex);
    void weaponSelected(QString);

    void defenceUpdated(int);
    void attackUpdated(int);
    void bonusUpdated(int);

    void editPlayer(QModelIndex);

private slots:

    void addNewAttacker();
    //void addNewDefender();
    void addPlayer(QString p);
    void attackerSelectedSlot(QModelIndex model);
    void defenderSelectedSlot(QModelIndex model);
    void attackStrUpdated(QString);
    void defenceStrUpdated(QString);
    void handleSelection(int item);
    void focusChanged(QWidget*oldwidget,QWidget *newwidget);
    void increaseValue();
    void decreaseValue();
    void connectAll();
    void disconnectAll();


private:
    Ui::CombatRoundWidget* _ui;
    int _selectedTarget;
};

#endif // COMBATROUNDWIDGET_H
