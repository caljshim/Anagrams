#ifndef PLAYGAME_H
#define PLAYGAME_H


#include <QObject>
#include <QDialog>
#include <QNetworkInterface>

namespace Ui {
class PlayGame;
}

class PlayGame : public QDialog
{
    Q_OBJECT

public:
    explicit PlayGame(const QString& word, QWidget *parent = nullptr);
    ~PlayGame();
signals:
    void gameEnded(int score, int wordCount, QSet<QString> words);
    void playgameclosed();

public slots:
    void updateLabel();
    void on_enterButton_clicked();
    void on_letter1_clicked();
    void on_letter2_clicked();
    void on_letter3_clicked();
    void on_letter4_clicked();
    void on_letter5_clicked();
    void on_letter6_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::PlayGame *ui;
    QTimer *timer;
    QString string = "";
    QSet<QString> wordDictionary;
    QSet<QString> alreadyFound;
    int score;
    int wordCount;
    void loadDictionary();
    void enableButtons();
    bool wordExists(const QString &word);
};

#endif // PLAYGAME_H
