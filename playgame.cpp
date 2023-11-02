#include "playgame.h"
#include "ui_PlayGame.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QSet>
#include <random>
#include <algorithm>

PlayGame::PlayGame(const QString& word, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayGame)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    this->setFocus();
    ui->textBrowser->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    loadDictionary();
    wordCount = 0;
    score = 0;
    setAttribute(Qt::WA_DeleteOnClose);

    std::string stdRandomWord = word.toStdString();
    std::default_random_engine generator(std::random_device{}());
    std::shuffle(stdRandomWord.begin(), stdRandomWord.end(), generator);

    ui->letter1->setText(QChar(stdRandomWord[0]));
    ui->letter2->setText(QChar(stdRandomWord[1]));
    ui->letter3->setText(QChar(stdRandomWord[2]));
    ui->letter4->setText(QChar(stdRandomWord[3]));
    ui->letter5->setText(QChar(stdRandomWord[4]));
    ui->letter6->setText(QChar(stdRandomWord[5]));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PlayGame::updateLabel);
    ui->timerLabel->setText("8");
    timer->start(1000);
}

PlayGame::~PlayGame()
{
    delete ui;
}


void PlayGame::updateLabel() {
    int currentTime = ui->timerLabel->text().toInt();
    if (currentTime > 0) {
        currentTime--;
        ui->timerLabel->setText(QString::number(currentTime));
    }
    else{
        timer->stop();
        this->close();
        emit gameEnded(score, wordCount, alreadyFound);
    }
}

void PlayGame::loadDictionary() {
    QFile file(":/resources/words.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in (&file);
        while (!in.atEnd()){
            wordDictionary.insert(in.readLine());
        }
    }
}

bool PlayGame::wordExists(const QString &word) {
    return wordDictionary.contains(word);
}

void PlayGame::on_enterButton_clicked(){
    QString string = ui->textBrowser->text();
    if (alreadyFound.contains(string)){
        ui->label->setVisible(true);
        ui->label->setText("Word already found!");
        ui->textBrowser->setText("");
        enableButtons();
    }
    else if (wordExists(string)){
        ui->label->setVisible(false);
        wordCount++;
        int charCount = 0;
        for (int i = 0; i<string.size(); i++){
            charCount++;
        }
        score += 100*charCount;
        alreadyFound.insert(string);
        ui->scoreCount->setText(QString::number(score));
        ui->wordCount->setText(QString::number(wordCount));
        ui->textBrowser->setText("");
        enableButtons();
    }
    else{
        ui->label->setVisible(true);
        ui->label->setText("Word not found!");
        ui->textBrowser->setText("");
        enableButtons();
    }
}

void PlayGame::on_letter1_clicked(){
    QString currentText = ui->textBrowser->text();
    currentText += ui->letter1->text();
    ui->textBrowser->setText(currentText);
    ui->letter1->setEnabled(false);
}

void PlayGame::on_letter2_clicked(){
    QString currentText = ui->textBrowser->text();
    currentText += ui->letter2->text();
    ui->textBrowser->setText(currentText);
    ui->letter2->setEnabled(false);
}

void PlayGame::on_letter3_clicked(){
    QString currentText = ui->textBrowser->text();
    currentText += ui->letter3->text();
    ui->textBrowser->setText(currentText);
    ui->letter3->setEnabled(false);
}

void PlayGame::on_letter4_clicked(){
    QString currentText = ui->textBrowser->text();
    currentText += ui->letter4->text();
    ui->textBrowser->setText(currentText);
    ui->letter4->setEnabled(false);
}

void PlayGame::on_letter5_clicked(){
    QString currentText = ui->textBrowser->text();
    currentText += ui->letter5->text();
    ui->textBrowser->setText(currentText);
    ui->letter5->setEnabled(false);
}

void PlayGame::on_letter6_clicked(){
    QString currentText = ui->textBrowser->text();
    currentText += ui->letter6->text();
    ui->textBrowser->setText(currentText);
    ui->letter6->setEnabled(false);
}

void PlayGame::enableButtons(){
    ui->letter1->setEnabled(true);
    ui->letter2->setEnabled(true);
    ui->letter3->setEnabled(true);
    ui->letter4->setEnabled(true);
    ui->letter5->setEnabled(true);
    ui->letter6->setEnabled(true);
}

void PlayGame::closeEvent(QCloseEvent *event){
    if (ui->timerLabel->text() != "0"){
        emit playgameclosed();
    }
}
