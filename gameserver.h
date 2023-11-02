#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "gamemanager.h"

class GameServer : public QObject {
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    ~GameServer();
    QTcpServer* internalServer;

    void startServer(quint16 port);
    void stopServer();
    void hostSignalsReady();
    void checkStartGame();
    void onClientDisconnected();
    void sendDataToClient(const QByteArray &data);
    QTcpSocket *clientSocket;
signals:
    void startGame();
    void clientConnected();
    void clientDisconnected();
    void sendWord(QString word);
    void gameEnded(int hostScore, int hostWordCount, QSet<QString> hostWords);
    void clientDataReceived(int clientScore, int clientWordCount, QSet<QString> clientFoundWords);

private slots:
    void newConnection();
    void onClientDataReceived();
private:
    QTcpServer *server;
    int activeConnections = 0;
    bool serverReady = false;
    bool clientReady = false;
    bool isClientConnected;
    gamemanager *gameMang = new gamemanager;
};

#endif // GAMESERVER_H
