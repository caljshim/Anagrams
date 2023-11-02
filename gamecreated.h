#ifndef GAMECREATED_H
#define GAMECREATED_H

#include <QDialog>
#include <QNetworkInterface>
#include "gameserver.h"

namespace Ui {
class GameCreated;
}

class GameCreated : public QDialog
{
    Q_OBJECT

public:
    explicit GameCreated(QWidget *parent = nullptr);
    GameServer* server;
    ~GameCreated();

signals:
    void closeAndReturnToMain();
    void hostIsReady();
    void gameFinished(int score, int wordCount, QSet<QString> wordlist);

public slots:
    void on_closeGameButton_clicked();
    void on_readyButton_clicked();
    void startTheGame(const QString& word);
    void onGameEnded(int score, int wordcount, QSet<QString> wordlist);
    void onClientDataReceived(int score, int wordcount, QSet<QString> words);
    void resetReadyButton();
    void reveal();

private:
    Ui::GameCreated *ui;
    void handleClientConnection();
    void handleClientDisconnection();
    void closeEvent(QCloseEvent *event) override;
    void startGame();
    int oppScore;
    int oppWordCount;
    QSet<QString> oppWords;
    int myScore;
    int myWordCount;
    QSet<QString> myWords;
};


#endif // GAMECREATED_H
