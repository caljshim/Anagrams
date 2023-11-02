#ifndef GAMEJOINED_H
#define GAMEJOINED_H

#include <QDialog>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QString>

namespace Ui {
class GameJoined;
}

class GameJoined : public QDialog
{
    Q_OBJECT

public:
    explicit GameJoined(QWidget *parent = nullptr);
    QString ipAddress;
    void setIpAddress(const QString &ip);
    ~GameJoined();

signals:
    void closeAndReturnToMain();
    void readyButtonClicked();
    void hideMain();
    void successfullyConnected();
    void gameEnded(int clientScore, int clientWordCount, QSet<QString> clientWords);

public slots:
    void on_leaveGameButton_clicked();
    void on_readyButton_clicked();
    void handleConnectionError();
    void handleSuccessfulConnection();
    void onDataReceived();
    void onGameEnded(int score, int wordcount, QSet<QString> words);
    void resetReadyButton();
    void reveal();

private:
    Ui::GameJoined *ui;
    void closeEvent(QCloseEvent *event) override;
    QTcpSocket* clientSocket;
    int oppScore;
    int oppWordCount;
    QSet<QString> oppWords;
    int myScore = 0;
    int myWordCount = 0;
    QSet<QString> myWords;
    QTimer* connectionTimeoutTimer;
};

#endif // GAMEJOINED_H
