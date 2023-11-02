#include "gamecreated.h"
#include "ui_GameCreated.h"
#include "gameserver.h"
#include <QCloseEvent>
#include "playgame.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "endingwindow.h"


GameCreated::GameCreated(QWidget *parent) :

    QDialog(parent),
    ui(new Ui::GameCreated)
{
    ui->setupUi(this);
    ui->readyButton->setEnabled(false);
    setAttribute(Qt::WA_DeleteOnClose);

    server = new GameServer();
    connect(server, &GameServer::clientConnected, this, &GameCreated::handleClientConnection);
    connect(server, &GameServer::clientDisconnected, this, &GameCreated::handleClientDisconnection);
    connect(this, &GameCreated::hostIsReady, server, &GameServer::hostSignalsReady);
    connect(server, &GameServer::sendWord, this, &GameCreated::startTheGame);
    connect(server, &GameServer::clientDataReceived, this, &GameCreated::onClientDataReceived);
    const quint16 port = 12345;
    server->startServer(port);

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++) {
        if(!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol ) {
                ui->IPLabel->setText(list[nIter].toString());
                break;
            }
    }
}

GameCreated::~GameCreated()
{
    delete ui;
}

void GameCreated::handleClientConnection() {
    ui->statusLabel->setText("Player Joined! Waiting for\nboth players to ready up.");
    ui->readyButton->setEnabled(true);
}

void GameCreated::handleClientDisconnection() {
    ui->statusLabel->setText("Waiting for player to Join");
    ui->readyButton->setEnabled(false);
}

void GameCreated::on_closeGameButton_clicked() {
    server->stopServer();
    emit closeAndReturnToMain();
    this->close();
}

void GameCreated::closeEvent(QCloseEvent *event){
    on_closeGameButton_clicked();
}

void GameCreated::on_readyButton_clicked(){
    ui->readyLabel->setText("Ready");
    ui->readyButton->setEnabled(false);
    emit hostIsReady();
}

void GameCreated::startTheGame(const QString& word) {
    PlayGame* newGame = new PlayGame(word);
    connect(newGame, &PlayGame::gameEnded, this, &GameCreated::onGameEnded);
    connect(newGame, &PlayGame::playgameclosed, this, &GameCreated::reveal);
    connect(newGame, &PlayGame::playgameclosed, this, &GameCreated::resetReadyButton);
    newGame->show();
    this->hide();
}

void GameCreated::onClientDataReceived(int score, int wordcount, QSet<QString> words){
    oppScore = score;
    oppWordCount = wordcount;
    oppWords = words;

    EndingWindow* endWindow = new EndingWindow(myScore, myWordCount, myWords, oppScore, oppWordCount, oppWords);
    connect(endWindow, &EndingWindow::endingwindowclosed, this, &GameCreated::reveal);
    endWindow->show();
}

void GameCreated::onGameEnded(int score, int wordCount, QSet<QString> wordlist){
    myScore = score;
    myWordCount = wordCount;
    myWords = wordlist;

    QByteArray dataToSend;
    dataToSend = "SCORE|";
    dataToSend += QString::number(score).toUtf8();
    dataToSend.append("|");
    dataToSend += QString::number(wordCount).toUtf8();
    dataToSend.append("|");

    QString allWords;
    const auto &items = wordlist.values();
    for (auto it = items.begin(); it != items.end(); it++){
        if (it != items.begin()){
            allWords += ",";
        }
        allWords += *it;
    }
    dataToSend.append(allWords.toUtf8());

    server->sendDataToClient(dataToSend);
    resetReadyButton();
}

void GameCreated::resetReadyButton(){
    ui->readyLabel->setText("Not ready");
    ui->readyButton->setEnabled(true);
}
void GameCreated::reveal(){
    this->show();
}
