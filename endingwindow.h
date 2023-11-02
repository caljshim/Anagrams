#ifndef ENDINGWINDOW_H
#define ENDINGWINDOW_H


#include <QDialog>
#include <QNetworkInterface>

namespace Ui {
class EndingWindow;
}

class EndingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EndingWindow(int yourScore, int yourWordCount, QSet<QString>& yourWords, int oppScore, int oppWordCount, QSet<QString>& oppWords, QWidget *parent = nullptr);
    ~EndingWindow();
    void setClientData(int score, int wordCount, QSet<QString>& words);
    void setHostData(int score, int wordCount, QSet<QString>& words);
    void assertWinner();

signals:
    void endingwindowclosed();

public slots:
    void closeEvent(QCloseEvent *event);


private:
    Ui::EndingWindow *ui;
    int yourScore;
    int opponentScore;
    int yourWordCount;
    int opponentWordCount;
    QSet<QString> yourWords;
    QSet<QString> opponentWords;
};


#endif // ENDINGWINDOW_H
