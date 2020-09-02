#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "poker.h"
#include "combo.h"
#include "check.h"
#include <QWaitCondition>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

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
    void showlord(int);
    void showCard();
    ~game();

public slots:


private:
    //void paintEvent(QPaintEvent *event) override;
    Ui::game *ui;
    int high_hand_num, low_hand_num, hand_num;
    QList<Poker> hand;
};

class card : public QPushButton
{
    Q_OBJECT
public:
    card(Poker c,QWidget *parent = nullptr);
};

#endif // GAME_H
