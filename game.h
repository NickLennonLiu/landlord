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
#include <QSignalMapper>

namespace Ui {
class game;
}

class card : public QPushButton
{
    Q_OBJECT
public:
    card(Poker c,QWidget *parent = nullptr);
public slots:
    void selfSelected();
signals:
    void cardSelected();
private:
    bool selected;
    Poker c;
};


class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    bool asklord();

    void getCard(Poker);
    void getlordcard(Poker,int);

    void showlord(int);
    void showCard();

    void playStart(int id);

    void setArrange(int selfname,int selfid,int a,int b,int c);
    void setLastPlayed(int lastplayed);
    void updateHandNum(int id,int leftnum);
    void myTurn(bool canpass);
    ~game();

public slots:
    void cardSelectSlot(int);
    void receiveCombo(QList<Poker>,int);
    void playCard();
    void pass();

signals:
    void sendCombo(QList<Poker>,int,int);
    void passSignal(int,int);
    void gameEnd(int);

private:
    //void paintEvent(QPaintEvent *event) override;




    Ui::game *ui;
    int high_hand_num, low_hand_num, hand_num;
    QList<Poker> hand;
    QList<card*> cards;
    int last_played;
    int selection[20];
    QSignalMapper *cardselect;
    int name, play_id;
    Combo *lastcomb  ;
};



#endif // GAME_H
