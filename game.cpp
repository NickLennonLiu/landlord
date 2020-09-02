#include "game.h"
#include "ui_game.h"

game::game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game)
{
    ui->setupUi(this);
}

game::~game()
{
    delete ui;
}

void game::getCard(Poker card){
    hand.append(card);
}

bool game::asklord(){
    if(QMessageBox::question(this,"是否抢地主？","请选择是否抢地主")==QMessageBox::Yes){
        return true;
    } else return false;
}

void game::showlord(int lord){
    QMessageBox::information(this,"地主是谁？",QString::number(lord));
}

void game::showCard(){
    int pos = 0;
    //sort(hand.rbegin(),hand.rend());
    for(Poker i : hand){
        card* c = new card(i,ui->handArea);
        qDebug() << "add new card" << i.string();
        c->move(pos * 40,0);
        c->show();
        pos++;
    }
}

card::card(Poker c,QWidget* parent)
    :QPushButton(parent)
{
    setIcon(QIcon(":/poker/cards/" + c.string() + ".png"));
    setFixedSize(60,80);
    setIconSize(rect().size());
}
