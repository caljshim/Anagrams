#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "gamejoined.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    \
signals:
    void sendString(const QString &text);

private slots:
    void on_createGameButton_clicked();
    void onReturnToMain();
    void on_joinGameButton_clicked();
    void onJoinGameConnected();

private:
    Ui::MainWindow *ui;
    QTcpSocket* clientSocket;
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
