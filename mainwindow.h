#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMap>

#include "combatroundcontroller.h"
#include "combatroundwidget.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    CombatRoundController* _combatRoundController;
    CombatRoundWidget* _combatRoundWidget;

};

#endif // MAINWINDOW_H
