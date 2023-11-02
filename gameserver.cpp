#include "gameserver.h"
#include <QDebug>
#include <QTcpServer>
#include <iostream>
#include "gamemanager.h"
#include <QDataStream>

GameServer::GameServer(QObject *parent) : QObject(parent) {
    server = new QTcpServer(this);
    clientSocket = nullptr;
    serverReady = false;
    clientReady = false;
    isClientConnected = false;
    activeConnections = 0;
    // Connect the newConnection signal to GameServer
    connect(server, &QTcpServer::newConnection, this, &GameServer::newConnection);
}

GameServer::~GameServer() {
    stopServer();
    delete server;
}

void GameServer::startServer(quint16 port) {
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start";
    } else {
        qDebug() << "Server started";
    }
}

void GameServer::stopServer() {
    server->close();
    if (clientSocket){
        clientSocket->disconnectFromHost();
    }
}

void GameServer::newConnection() {
    // Called when a new client connects
    if (isClientConnected) {
        // Reject any new connection if a client is already connected
        QTcpSocket *tempSocket = server->nextPendingConnection();
        tempSocket->close();
        delete tempSocket;
        return;
    }

    clientSocket = server->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &GameServer::onClientDataReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &GameServer::onClientDisconnected);
    isClientConnected = true;

    emit clientConnected();
}

void GameServer::onClientDisconnected(){
    if (clientSocket) {
        clientSocket->abort();
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }
    isClientConnected = false;
    emit clientDisconnected();
}


void GameServer::onClientDataReceived(){
    QByteArray data = clientSocket->readAll();
    QList<QByteArray> parts = data.split('|');
    QString dataType = parts[0];
    if (dataType == "READY" && parts.size() > 1) {
        clientReady = true;
        checkStartGame();
    }
    else if (dataType == "SCORE"){
        int clientScore = parts[1].toInt();
        int clientWordCount = parts[2].toInt();
        QStringList wordList1 = QString(parts[3]).split(',');
        QSet<QString> clientFoundWords = QSet<QString>(wordList1.begin(), wordList1.end());;

        emit clientDataReceived(clientScore, clientWordCount, clientFoundWords);
    }
}

void GameServer::sendDataToClient(const QByteArray &data){
    if (clientSocket) {
        clientSocket->write(data);
    }
}

void GameServer::checkStartGame() {
    if (clientReady && serverReady) {
        QString wordForThisRound = gameMang->getCurrentWordForRound();
        QByteArray data = "START|" + wordForThisRound.toUtf8();
        clientSocket->write(data);
        emit sendWord(wordForThisRound);
        clientReady = false;
        serverReady = false;
    }
}

void GameServer::hostSignalsReady() {
    serverReady = true;
    checkStartGame();
}
