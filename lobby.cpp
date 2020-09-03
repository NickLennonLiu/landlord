#include "lobby.h"
#include "ui_lobby.h"


lobby::lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lobby)
{
    ui->setupUi(this);
    ui->start->setHidden(true);
    ui->replay->setHidden(true);
    ui->connect->setDisabled(false);
    setWindowTitle("Game Lobby");
    gamewindow = new game(this);
    gamewindow->setWindowFlag(Qt::Window);
    connect(gamewindow,SIGNAL(sendCombo(QList<Poker>,int,int)),this,SLOT(sendComboSlot(QList<Poker>,int,int)));
    connect(gamewindow,SIGNAL(passSignal(int,int)),this,SLOT(passSlot(int,int)));
    connect(gamewindow,SIGNAL(gameEnd(int)),this,SLOT(gameEnd(int)));
}

void lobby::sendComboSlot(QList<Poker> cb,int len,int left){
    QString info;
    info += "11 " + QString::number(play_id) + " "
            + QString::number(left) + " "
            + QString::number(len) + " ";
    for(auto i : cb){
        info += i.string() + " ";
    }
    for(int i = 0;i<3;i++){
        if(i!=play_id) sendMessageById(i,info);
    }
}

void lobby::passSlot(int playid,int last_played)
{
    QString info;
    info += "12 " + QString::number(play_id)
            + " " + QString::number(last_played);
    for(int i = 0;i<3;i++){
        if(i!=play_id) sendMessageById(i,info);
    }
}

lobby::~lobby()
{
    delete ui;
}

void lobby::acceptConnectionHost() {
    if(connection==0){
        readWriteSocket_b = listenSocket->nextPendingConnection();
        ui->logshow->appendPlainText("玩家B 加入了房间");
        QObject::connect(readWriteSocket_b,SIGNAL(readyRead()),mapper,SLOT(map()));
        mapper->setMapping(readWriteSocket_b,2);
        connectABC[1] = readWriteSocket_b;

        connection++;
    }   else {
        readWriteSocket_c = listenSocket->nextPendingConnection();
        ui->logshow->appendPlainText("玩家C 加入了房间");
        QObject::connect(readWriteSocket_c,SIGNAL(readyRead()),mapper,SLOT(map()));
        mapper->setMapping(readWriteSocket_c,3);
        connectABC[2] = readWriteSocket_c;
        connection++;

        // 发射玩家就绪信号
        playersReady();
        // 启用 开始游戏 按钮
        ui->start->setHidden(false);
        connect(ui->start,SIGNAL(clicked()),this,SLOT(gameStart()));
    }
}

void lobby::gameStart(){
    if(client_id==0){
        //通知BC设置gamewindow
        sendMessageByABC(1,"2");
        sendMessageByABC(2,"2");
        //决定顺序
        qsrand(time(0));
        int idfora = qrand() % 3;
        int idforb = qrand() % 3;
        while(idforb == idfora) idforb = qrand() % 3 ;
        int idforc = 0;
        while(idforc == idfora || idforc == idforb) idforc++;
        decideArrange(idfora,idforb,idforc);
        //发牌
        serve();

        thread = new LordThread;
        connect(thread,SIGNAL(askSignal(int)),this,SLOT(askForLord(int)));
        connect(this,SIGNAL(lordProgressSignal()),thread,SLOT(lordSlot()));
        connect(thread,SIGNAL(lordDecideSignal(int)),this,SLOT(whosLord(int)));
        connect(thread,SIGNAL(playSignal()),this,SLOT(playStart()));
        thread->start();
    }
    gamewindow->show();

}

void LordThread::run()
{
    emit askSignal(0);
    while(!lorddecided){}
    qDebug() << "player1 decided!";
    emit askSignal(1);
    while(lorddecided==1){}
    qDebug() << "player2 decided!";
    emit askSignal(2);
    while(lorddecided==2){}
    qDebug() << "player3 decided!";
    emit lordDecideSignal(0);
    emit playSignal();
}

void LordThread::lordSlot(){
    lorddecided += 1;
    qDebug() << "lord decided:" << lorddecided ;
}

LordThread::LordThread(QObject *parent)
    :QThread(parent)
{
    lorddecided = 0;
}

LordThread::~LordThread(){}

void lobby::serve(){
    QList<Poker> deck;
    for(int i = 0;i<13;i++) deck.append(Poker(i,club));
    for(int i = 0;i<13;i++) deck.append(Poker(i,diamond));
    for(int i = 0;i<13;i++) deck.append(Poker(i,heart));
    for(int i = 0;i<13;i++) deck.append(Poker(i,spade));
    deck.append(Poker(13,joker));
    deck.append(Poker(14,joker));
    std::random_shuffle(deck.rbegin(),deck.rend());
    QString b_info = "4 1 ";
    QString c_info = "4 2 ";
    for(int i = 0;i<17;i++) gamewindow->getCard(deck[i]);
    for(int i = 17;i<34;i++) b_info += deck[i].string() + " ";
    for(int i = 34;i<51;i++) c_info += deck[i].string() + " ";
    for(int i = 51;i<54;i++) lordshand.append(deck[i]);
    sendMessageByABC(1,b_info);
    sendMessageByABC(2,c_info);
    gamewindow->showCard();
}

// A发出 询问是否抢地主
void lobby::askForLord(int i){
    if(i==play_id){
        if(whetherLord()) lord_id = i;
        emit lordProgressSignal();
    } else {
        QString info = "5 ";
        info += QString::number(i);
        sendMessageById(i,info);
    }
}

// B和C决定谁是地主
void lobby::chooseLord(int id,bool yes)
{
    lord_id = (lord_id < id && (yes)) ? id : lord_id;
    emit lordProgressSignal();
}

// 地主是谁，卡牌是什么，瞬时完成
void lobby::whosLord(int lordid){
    qDebug() << "The lord has been decided! " << lordid;

    if(client_id==0){
        lordid = lord_id;
        QString info = "7 ";
        info += QString::number(lord_id) + " ";
        for(int i = 0;i<3;i++) info += lordshand[i].string() + " ";
        sendMessageByABC(1,info);
        sendMessageByABC(2,info);
        for(int i = 0;i<3;i++){
            gamewindow->getlordcard(lordshand[i],i);
            if(play_id==lordid) gamewindow->getCard(lordshand[i]);
        }
        gamewindow->showCard();
    }
    lord_id = lordid;
    gamewindow->showlord(lord_id);
}

bool lobby::whetherLord(){
    if(client_id==0){
        if(gamewindow->asklord()){
            return true;
        }
        return false;
    } else {
        if(gamewindow->asklord()){
            QString info = "6 ";
            info += QString::number(play_id) + " 1";
            sendMessageByABC(0,info);
            qDebug() << "I'm deciding: " << info;
            return true;
        }
        else {
            QString info = "6 ";
            info += QString::number(play_id) + " 0";
            sendMessageByABC(0,info);
            qDebug() << "I'm deciding: " << info;
            return false;
        }
    }
    return false;
}

void lobby::decideArrange(int a, int b, int c){
    connectId[a] = connectABC[0];
    connectId[b] = connectABC[1];
    connectId[c] = connectABC[2];
    if(!client_id){
        play_id = a;
        QString info = "3 ";
        info += QString::number(a) + " ";
        info += QString::number(b) + " ";
        info += QString::number(c);
        sendMessageByABC(1,info);
        sendMessageByABC(2,info);
    }
    if(client_id == 1) {
        play_id = b;

    }
    if(client_id == 2) {
        play_id = c;
    }
    gamewindow->setArrange(client_id,play_id,a,b,c);
    gamewindow->setWindowTitle(QString('A'+client_id) + QString(" : ") + QString::number(play_id));
}


void lobby::playersReady(){
    if(client_id==0) {
        QString info = "1";
        sendMessageByABC(1,info);
        sendMessageByABC(2,info);
    }
    ui->logshow->appendPlainText("所有玩家已就位，等待房主A开始游戏");
}


void lobby::acceptConnectionMedia() {
    readWriteSocket_c = listenSocket->nextPendingConnection();
    ui->logshow->appendPlainText("玩家C 加入了房间");
    QObject::connect(readWriteSocket_c,SIGNAL(readyRead()),mapper,SLOT(map()));
    mapper->setMapping(readWriteSocket_c,3);
    connectABC[2] = readWriteSocket_c;
    connection ++;
}

void lobby::on_connect_clicked(){
    connection = 0;
    mapper = new QSignalMapper;
    connectSocket_a = new QTcpSocket();
    connectSocket_b = new QTcpSocket();
    readWriteSocket_b = new QTcpSocket();
    readWriteSocket_c = new QTcpSocket();

    listenSocket = new QTcpServer();
    if(listenSocket->listen(QHostAddress("127.0.0.1"),6001))
    {
        QObject::connect(listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnectionHost()));
        ui->logshow->appendPlainText("欢迎加入房间，玩家A");
        setWindowTitle("A");
        client_id = 0;
    } else if(listenSocket->listen(QHostAddress("127.0.0.1"),6002)) {
        ui->logshow->appendPlainText("欢迎加入房间，玩家B");
        setWindowTitle("B");
        QObject::connect(listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnectionMedia()));


        connectSocket_a->connectToHost(QHostAddress("127.0.0.1"),6001);
        connectABC[0] = connectSocket_a;
        QObject::connect(connectSocket_a,SIGNAL(readyRead()),mapper,SLOT(map()));

        client_id = 1;
    } else {
        ui->logshow->appendPlainText("欢迎加入房间，玩家C");
        setWindowTitle("C");
        connectSocket_a->connectToHost(QHostAddress("127.0.0.1"),6001);
        connectABC[0] = connectSocket_a;
        QObject::connect(connectSocket_a,SIGNAL(readyRead()),mapper,SLOT(map()));

        connectSocket_b->connectToHost(QHostAddress("127.0.0.1"),6002);
        QObject::connect(connectSocket_b,SIGNAL(readyRead()),mapper,SLOT(map()));
        connectABC[1] = connectSocket_b;

        client_id = 2;
    }

    mapper->setMapping(connectSocket_a,0);
    mapper->setMapping(connectSocket_b,1);
    QObject::connect(mapper,SIGNAL(mapped(int)),this,SLOT(receiveMessage(int)));
    ui->connect->setDisabled(true);
}

void lobby::playStart(){
    if(client_id==0){
        QString info("8");
        sendMessageByABC(1,info);
        sendMessageByABC(2,info);
    }
    gamewindow->playStart(lord_id);
}

void lobby::sendMessageById(int id,QString m)
{
    QString info;
    info.fill('/',100);
    info.push_front(m);
    info = info.left(100);
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(info);
    connectId[id]->write(array->data());
}

void lobby::sendMessageByABC(int id,QString m)
{
    QString info;
    info.fill('/',100);
    info.push_front(m);
    info = info.left(100);
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(info);
    connectABC[id]->write(array->data());
}

void lobby::receiveMessage(int from) {  // 0A 1B 2B 3C
    QTcpSocket* message;
    switch (from) {
    case 0:{
        message = connectSocket_a;break;
    }
    case 1:{
        message = connectSocket_b;break;
    }
    case 2:{
        message = readWriteSocket_b;break;
    }
    case 3:{
        message = readWriteSocket_c;break;
    }
    }
    while(message->bytesAvailable()){
        QString info = message->read(100);
        qDebug() << "Receive Message: "<< info;
        QTextStream in(&info);
        int header;
        in >> header;
        qDebug() << header;
        switch(header){
        case WAITFORSTART:playersReady();break;
        case STARTUP:gameStart();break;
        case ARRANGE:{
            int a,b,c;
            in >> a >> b >> c;
            qDebug() << a << b << c;
            decideArrange(a,b,c);
            break;
        }
        case SERVE:{
            int id,ca;
            char c,space;
            in >> id;
            if(id == client_id){
                for(int i = 0;i<17;i++){
                    in >> space >> c >> ca;
                    Poker newcard = Poker(QString(c) + QString::number(ca));
                    gamewindow->getCard(newcard);
                }
                gamewindow->showCard();
            }
            break;
        }
        case ASKFORLORD:{
            int ask_id;
            in >> ask_id;
            if(ask_id == play_id){
                whetherLord();
            }
            break;
        }
        case WANTLORD:{
            int want,id;
            in >> id >> want;
            chooseLord(id,want);
            break;
        }
        case LORDCARDS:{
            int lordid;
            in >> lordid;
            whosLord(lordid);
            char space,c;
            int ca;
            for(int i = 0;i<3;i++){
                in >> space >> c >> ca;
                Poker newcard = Poker(QString(c) + QString::number(ca));
                gamewindow->getlordcard(newcard,i);
                if(play_id==lordid) gamewindow->getCard(newcard);
            }
            if(play_id==lordid) gamewindow->showCard();
            break;
        }
        case GAMESTART: playStart();break;
        case GAMEEND:break;
        case REPLAY:break;
        case COMBO:{
            int playedid, left, len;
            in >> playedid >> left >> len;
            QList<Poker> co;
            for(int i = 0;i<len;i++){
                char space,c;
                int ca;
                in >> space >> c >> ca;
                Poker newcard = Poker(QString(c) + QString::number(ca));
                co.append(newcard);

            }
            gamewindow->receiveCombo(co,len);
            gamewindow->updateHandNum(playedid,left);
            gamewindow->setLastPlayed(playedid);
            if(left==0) gameEnd(playedid);
            else if((playedid+1)%3==play_id) gamewindow->myTurn(true);
            break;
        }
        case PASS:{
            int passid,lastplayed;
            in >> passid >> lastplayed;
            gamewindow->setLastPlayed(lastplayed);
            if((passid+1)%3==play_id) gamewindow->myTurn((lastplayed!=play_id));
            break;
        }
        default:break;
        }
    }
}

void lobby::gameEnd(int winner){
    QMessageBox::information(this,"游戏结束！",((winner==lord_id)? "胜利方：地主":"胜利方：农民"));
}
