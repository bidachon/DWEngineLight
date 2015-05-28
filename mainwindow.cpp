#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGridLayout>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _combatRoundWidget = new CombatRoundWidget(this);
    _combatRoundWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QGridLayout *gl = new QGridLayout(this);
    gl->setMargin(5);
    gl->addWidget(_combatRoundWidget);
    ui->centralWidget->setLayout(gl);
    //ui->_combatRoundWidget = _combatRoundWidget;

    _combatRoundController = new CombatRoundController(this);
    _combatRoundController->setCombatRoundWidget(_combatRoundWidget);


}

MainWindow::~MainWindow()
{
    delete ui;
}


