#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "poker.h"
#include "combo.h"
#include "check.h"


namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    bool asklord();
    void getCard(Poker);
    ~game();

private:
    Ui::game *ui;
    int high_hand_num, low_hand_num, hand_num;
    QList<Poker> hand;
};

#endif // GAME_H
