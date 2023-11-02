#include "gamejoined.h"
#include "ui_GameJoined.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "playgame.h"
#include <QThread>
#include "endingwindow.h"
#include <iostream>
#include <QTimer>

GameJoined::GameJoined(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameJoined)
{
    ui->setupUi(this);
    clientSocket = new QTcpSocket(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(clientSocket, &QTcpSocket::connected, this, &GameJoined::handleSuccessfulConnection, Qt::UniqueConnection);
    connect(clientSocket, &QTcpSocket::errorOccurred, this, &GameJoined::handleConnectionError);
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameJoined::onDataReceived);
    connectionTimeoutTimer = new QTimer(this);
    connectionTimeoutTimer->setSingleShot(true);
    connect(connectionTimeoutTimer, &QTimer::timeout, [=]() {
        handleConnectionError();
    });
    connectionTimeoutTimer->start(5000);
}

GameJoined::~GameJoined()
{
    delete ui;
}

void GameJoined::setIpAddress(const QString &ip) {
    ipAddress = ip;
    quint16 port = 12345;
    clientSocket->connectToHost(ipAddress, port);
    ui->IPLabel->setText(ipAddress);
}

void GameJoined::on_leaveGameButton_clicked(){
    clientSocket->abort();
    this->close();
    emit closeAndReturnToMain();
}

void GameJoined::closeEvent(QCloseEvent *event) {
    on_leaveGameButton_clicked();
}


void GameJoined::handleConnectionError() {
    connectionTimeoutTimer->stop();
    if (this->isVisible()){
        clientSocket->abort();
        QMessageBox::critical(this, "Error", "Host stopped the connection.");
        this->close();
    }
    else{
        QMessageBox::critical(this, "Error", "Could not connect to IP address.");
    }
    clientSocket->abort();
    this->close();
}

void GameJoined::handleSuccessfulConnection(){
    connectionTimeoutTimer->stop();
    emit successfullyConnected();
    this->show();
}

void GameJoined::on_readyButton_clicked(){
    ui->readyButton->setEnabled(false);
    ui->readyLabel->setText("Ready");
    QByteArray data;

    data = "READY|";

    clientSocket->write(data);
}

void GameJoined::onDataReceived() {
    QByteArray data = clientSocket->readAll();
    QList<QByteArray> parts = data.split('|');
    QString dataType = parts[0];

    if (dataType == "START" && parts.size() > 1){
        QString wordForThisRound = QString::fromUtf8(parts[1]);
        PlayGame* newGame = new PlayGame(wordForThisRound);
        connect(newGame, &PlayGame::gameEnded, this, &GameJoined::onGameEnded);
        connect(newGame, &PlayGame::playgameclosed, this, &GameJoined::reveal);
        connect(newGame, &PlayGame::playgameclosed, this, &GameJoined::resetReadyButton);
        newGame->show();
        this->hide();
    }
    else if (dataType == "SCORE"){
        oppScore = parts[1].toInt();
        oppWordCount = parts[2].toInt();
        QStringList wordList1 = QString(parts[3]).split(',');
        oppWords = QSet<QString>(wordList1.begin(), wordList1.end());
    }
}

void GameJoined::onGameEnded(int score, int wordCount, QSet<QString> wordlist){
    myScore = score;
    myWordCount = wordCount;
    myWords = wordlist;

    QByteArray dataToSend;
    dataToSend = "SCORE|";
    dataToSend += QString::number(score).toUtf8();
    dataToSend += "|";
    dataToSend += QString::number(wordCount).toUtf8();
    dataToSend += "|";

    QString allWords;
    const auto &items = wordlist.values();
    for (auto it = items.begin(); it != items.end(); it++){
        if (it != items.begin()){
            allWords += ",";
        }
        allWords += *it;
    }
    dataToSend.append(allWords.toUtf8());

    clientSocket->write(dataToSend);

    EndingWindow* endWindow = new EndingWindow(myScore, myWordCount, myWords, oppScore, oppWordCount, oppWords);
    connect(endWindow, &EndingWindow::endingwindowclosed, this, &GameJoined::reveal);
    endWindow->show();

    resetReadyButton();
}

void GameJoined::resetReadyButton(){
    ui->readyButton->setEnabled(true);
    ui->readyLabel->setText("Not ready");
}

void GameJoined::reveal(){
    this->show();
}
