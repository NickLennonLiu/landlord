#include "game.h"
#include "ui_game.h"

bool cmp(const Poker& card1,const Poker& card2){
    if(card1.getRank()<=card2.getRank() ) return true;
    if(card1.getRank()>card2.getRank()) return false;
    return card1.getSuit()<= card2.getSuit();
}
game::game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::game),
    lastcomb(new NotACombo())
{
    memset(selection,0,sizeof(selection));
    ui->setupUi(this);
    cardselect = new QSignalMapper;
    ui->wantLord->hide();
    ui->noLord->hide();
    ui->replay->hide();
    ui->exit->hide();
    connect(cardselect,SIGNAL(mapped(int)),this,SLOT(cardSelectSlot(int)));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(playCard()));
    connect(ui->pass,SIGNAL(clicked()),this,SLOT(pass()));
    connect(ui->exit,SIGNAL(clicked()),this,SIGNAL(closegame()));
    ui->sNum->display(17);
    ui->lNum->display(17);
    ui->uNum->display(17);
}

void game::setLastPlayed(int lastplayed){
    last_played = lastplayed;
}

void game::playerPassed(int passid){
    if(passid == ((play_id+1)%3)) ui->uLord->setText("不出");
    else ui->lLord->setText("不出");
}

void game::pass(){
    ui->log->clear();
    ui->confirm->setEnabled(false);
    ui->pass->setEnabled(false);
    emit passSignal(play_id,last_played);
}

void game::showLordQuestion(){
    ui->wantLord->show();
    ui->noLord->show();
    ui->log->setPlainText("是否叫地主？");
    connect(ui->wantLord,SIGNAL(clicked()),this,SIGNAL(wantLord()));
    connect(ui->noLord,SIGNAL(clicked()),this,SIGNAL(noLord()));
    connect(ui->wantLord,SIGNAL(clicked()),this,SLOT(hideLordQuestion()));
    connect(ui->noLord,SIGNAL(clicked()),this,SLOT(hideLordQuestion()));
}

void game::hideLordQuestion(){
    ui->wantLord->hide();
    ui->noLord->hide();
    ui->log->clear();
}

// 由confirm按钮触发
void game::playCard(){
    Poker chu[20];
    int len = 0;
    //for(int i = 0;i<hand.size();i++) qDebug() << "输出hand内容" << hand[i].string();
    for(int i = 0;i<20;i++){
        if(selection[i]){
            chu[len++] = hand[i];
        }
    }
    Combo &comb  = whatCombo(chu,len);
    if((comb.getCat() != notacombo) && canBeNext(comb,*lastcomb)){
        for(int i =19;i>=0;i--){
            if(selection[i]){
                hand.removeAt(i);
            }
        }
        memset(selection,0,sizeof(selection));
        showCard();
        QList<Poker> chucard;
        for(int i = 0;i<len;i++) chucard.append(chu[i]);
        emit sendCombo(chucard,len,hand.size());
        receiveCombo(chucard,len);
        ui->log->clear();
        ui->confirm->setEnabled(false);
        ui->pass->setEnabled(false);
        updateHandNum(play_id,hand.size());
        if(!hand.size()) emit gameEnd(play_id);
    } else {
        ui->log->setPlainText("你出的牌不合法！ ");
    }
}

void game::End(bool winner){
    if(winner) ui->log->setPlainText("你赢了！要再玩一次吗？");
    else ui->log->setPlainText("你输了！要再玩一次吗？");
    ui->replay->show();
    ui->exit->show();
    connect(ui->replay,SIGNAL(clicked()),this,SIGNAL(replaySignal()));
    connect(ui->replay,SIGNAL(clicked()),this,SLOT(hideReplay()));
}

void game::hideReplay(){
    ui->replay->hide();
}

void game::receiveCombo(QList<Poker> cs,int len){
    int pos = 0;
    for(auto &i:ui->central->children()) delete i;
    for(auto i: cs){
        card* c = new card(i,ui->central);
        c->move(QPoint(10,10)+QPoint((pos%10) * 40,pos/10 * 80));
        c->show();
        pos ++;
    }
    Poker co[20];
    for(int i = 0;i<len;i++) {
        co[i] = cs[i];
    }
    lastcomb = &whatCombo(co,len);
    QString info;
    switch(lastcomb->getCat()){
    case single:info += "单张！";break;
    case pair:info += "对子！";break;
    case triple:info+= "三带！";break;
    case straight:info+="顺子！";break;
    case dstraight:info+="连对！";break;
    case quartwdoub:info+="四带二！";break;
    case plane:info+="飞机！！";break;
    case bomb:info+="炸弹！！！";break;
    case jokers:info+="王炸！！！！";break;
    default:break;
    }
    ui->log->setPlainText(info);
    ui->uLord->clear();
    ui->lLord->clear();
}

game::~game()
{
    delete ui;
}

void game::getCard(Poker card){
    hand.append(card);
    sort(hand.rbegin(),hand.rend(),cmp);
}

void game::playStart(int id){
    ui->log->setPlainText("游戏开始!请地主开始出牌");
    ui->sLord->clear();
    ui->uLord->clear();
    ui->lLord->clear();
    if(play_id==id){
        ui->log->setPlainText("轮到你出牌了！");
        ui->confirm->setEnabled(true);
    }
}

void game::getlordcard(Poker ca, int id)
{
    card *lordcard = new card(ca,ui->lord);
    lordcard->setDisabled(true);
    QPalette pa = lordcard->palette();
    pa.setCurrentColorGroup(QPalette::Normal);
    lordcard->setPalette(pa);
    lordcard->setFixedSize(60,80);
    lordcard->setIconSize(lordcard->rect().size());
    lordcard->show();
    lordcard->move(ui->lord->pos()+QPoint(id*70,0));
}

void game::showlord(int lord){
    if(lord == play_id){
        ui->sIdentity->setText("地主");
        ui->lIdentity->setText("农民");
        ui->uIdentity->setText("农民");
        ui->sNum->display(20);
    }
    if(lord == (play_id+1)%3){
        ui->lIdentity->setText("地主");
        ui->sIdentity->setText("农民");
        ui->uIdentity->setText("农民");
        ui->lNum->display(20);
    }
    if(lord == (play_id+2)%3){
        ui->uNum->display(20);
        ui->uIdentity->setText("地主");
        ui->lIdentity->setText("农民");
        ui->sIdentity->setText("农民");
    }
}


void game::showCard(){
    int pos = 0;
    for(auto i : cards){
        delete i;
    }
    cards.clear();
    sort(hand.rbegin(),hand.rend(),cmp);
    for(Poker i : hand){
        card* c = new card(i,ui->handArea);
        connect(c,SIGNAL(cardSelected()),cardselect,SLOT(map()));
        cardselect->setMapping(c,pos);
        cards.append(c);
        c->move(pos * 40,30);
        c->show();
        pos++;
    }
}

void game::setArrange(int selfname, int selfid, int a,int b,int c){
    play_id = selfid;
    name = selfname;
    if(selfname==0){
        ui->sName->setText("玩家A");
        ui->lName->setText(((b==(play_id + 1)%3) ? "玩家B" : "玩家C"));
        ui->uName->setText(((b==(play_id + 1)%3) ? "玩家C" : "玩家B"));
    }
    if(selfname==1){
        ui->sName->setText("玩家B");
        ui->lName->setText(((a==(play_id + 1)%3) ? "玩家A" : "玩家C"));
        ui->uName->setText(((a==(play_id + 1)%3) ? "玩家C" : "玩家A"));
    }
    if(selfname==2){
        ui->sName->setText("玩家C");
        ui->lName->setText(((a==(play_id + 1)%3) ? "玩家A" : "玩家B"));
        ui->uName->setText(((a==(play_id + 1)%3) ? "玩家B" : "玩家A"));
    }
}

card::card(Poker c,QWidget* parent)
    :QPushButton(parent)
    , c(c)
{
    QIcon icon = QIcon();
    icon.addPixmap(QPixmap(":/poker/cards/" + c.string() + ".png"),QIcon::Normal);
    icon.addPixmap(QPixmap(":/poker/cards/" + c.string() + ".png"),QIcon::Disabled);
    setIcon(icon);
    setFixedSize(90,120);
    setIconSize(rect().size());
    selected = 0;
    connect(this,SIGNAL(clicked()),this,SLOT(selfSelected()));
}

void card::selfSelected(){
    if(!selected){
        move(pos() + QPoint(0,-20));
    }
    else {
        move(pos() + QPoint(0,20));
    }
    selected = !selected;
    emit cardSelected();
}

void game::cardSelectSlot(int cd)
{
    selection[cd] = !selection[cd];
}

void game::updateHandNum(int id, int leftnum){
    if(play_id==id){
        ui->sNum->display(leftnum);
    }
    else if((play_id+1)%3==id){
        ui->lNum->display(leftnum);
    } else {
        ui->uNum->display(leftnum);
    }
}

void game::myTurn(bool canpass){
    if(canpass) ui->pass->setEnabled(true);
    if(!canpass) lastcomb = new NotACombo();
    ui->confirm->setEnabled(true);
    ui->log->setPlainText("轮到你出牌了！");
}

void game::showlorddecision(int id, bool want){
    if(id == play_id){
        ui->sLord->setText((want)?"叫地主":"不叫");
    }
    if(id == (play_id+1)%3){
        ui->lLord->setText((want)?"叫地主":"不叫");
    }
    if(id == (play_id+2)%3){
        ui->uLord->setText((want)?"叫地主":"不叫");
    }
}
