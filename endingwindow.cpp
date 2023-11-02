#include "endingwindow.h"
#include "ui_EndingWindow.h"
#include "QLabel"
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QStandardItemModel>

EndingWindow::EndingWindow(int yourScore, int yourWordCount, QSet<QString>& yourWords, int oppScore, int oppWordCount, QSet<QString>& oppWords, QWidget *parent) :

    QDialog(parent),
    ui(new Ui::EndingWindow)
{
    ui->setupUi(this);
    setClientData(yourScore, yourWordCount, yourWords);
    setHostData(oppScore, oppWordCount, oppWords);
    setAttribute(Qt::WA_DeleteOnClose);
}

EndingWindow::~EndingWindow()
{
    delete ui;
}

void EndingWindow::setClientData(int score, int wordCount, QSet<QString>& words) {
    ui->yourScoreLabel->setText(QString::number(score));
    ui->yourWordCount->setText("Your Words: " + QString::number(wordCount));

    for (const QString &item : words){
        ui->listWidget->addItem(item);
    }
    assertWinner();
    QWidget::update();
}

void EndingWindow::setHostData(int score, int wordCount, QSet<QString>& words) {
    ui->oppScoreLabel->setText(QString::number(score));
    ui->oppWords->setText("Opponent's Words: " + QString::number(wordCount));

    for (const QString &item : words){
        ui->listWidget_2->addItem(item);
    }
    assertWinner();
    QWidget::update();
}

void EndingWindow::assertWinner(){
    if (ui->yourScoreLabel->text() > ui->oppScoreLabel->text()){
        ui->winorloseLabel->setText("You won!");
    }
    else if (ui->yourScoreLabel->text() == ui->oppScoreLabel->text()){
        ui->winorloseLabel->setText("You tied.");
    }
    else{
        ui->winorloseLabel->setText("You lost.");
    }
}

void EndingWindow::closeEvent(QCloseEvent *event){
    emit endingwindowclosed();
}
