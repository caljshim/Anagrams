#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include "gamecreated.h"
#include "gamejoined.h"
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientSocket = new QTcpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_createGameButton_clicked() {
    GameCreated* gameCreatedForm = new GameCreated(this);
    connect(gameCreatedForm, &GameCreated::closeAndReturnToMain, this, &MainWindow::onReturnToMain);
    gameCreatedForm->show();

    this->hide();
}

void MainWindow::on_joinGameButton_clicked(){
    ui->joinGameButton->setEnabled(false);
    ui->createGameButton->setEnabled(false);
    GameJoined* gameJoinedForm = new GameJoined(this);
    connect(gameJoinedForm, &GameJoined::closeAndReturnToMain, this, &MainWindow::onReturnToMain);
    connect(gameJoinedForm, &GameJoined::successfullyConnected, this, &MainWindow::onJoinGameConnected);
    gameJoinedForm->setIpAddress(ui->inputIPAddress->toPlainText());
}

void MainWindow::onReturnToMain() {
    ui->joinGameButton->setEnabled(true);
    ui->createGameButton->setEnabled(true);
    this->show();
}

void MainWindow::onJoinGameConnected() {
    this->hide();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qApp->quit();
}
