#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QString>
#include <QSet>

class gamemanager {
public:
    QString getCurrentWordForRound();
    QString currentWordForRound;

public:
    gamemanager();
    ~gamemanager() = default;


    QSet<QString> wordDictionary;
    QList<QString> sixLetterWords;
};

#endif // GAMEMANAGER_H
