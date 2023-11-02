#include "gamemanager.h"
#include <QFile>
#include <QTextStream>
#include <random>
#include <QMutex>
#include <QMutexLocker>

gamemanager::gamemanager() {
}

QString gamemanager::getCurrentWordForRound() {
    QFile file(":/resources/words.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in (&file);
        while (!in.atEnd()){
            wordDictionary.insert(in.readLine());
        }
    }

    for (const QString &word : wordDictionary) {
        if (word.length() == 6) {
            sixLetterWords.append(word);
        }
    }
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, sixLetterWords.size() - 1);

    int randomIndex = distribution(generator);
    currentWordForRound = sixLetterWords[randomIndex];
    return currentWordForRound;
}
