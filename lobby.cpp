#include "lobby.h"
#include "ui_lobby.h"


lobby::lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lobby)
{
    ui->setupUi(this);
    ui->start->setHidden(true);
    ui->replay->setHidden(true);
    setWindowTitle("Game Lobby");
    gamewindow = new game(this);
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
        connect(ui->start,SIGNAL(triggered()),this,SLOT(gameStart()));
    }
}

void lobby::gameStart(){
    if(client_id==0){
        sendMessageByABC(1,"2");
        sendMessageByABC(2,"2");

        qsrand(time(0));
        int idfora = qrand() % 3;
        int idforb = qrand() % 3;
        while(idforb == idfora) idforb = qrand() % 3 ;
        int idforc = 0;
        while(idforc == idfora || idforc == idforb) idforc++;
        decideArrange(idfora,idforb,idforc);

        serve();

        askForLord();
        whosLord();
    }
    gamewindow->show();
}

void lobby::serve(){
    QList<Poker> deck;
    for(int i = 0;i<13;i++) deck.append(Poker(i,club));
    for(int i = 0;i<13;i++) deck.append(Poker(i,diamond));
    for(int i = 0;i<13;i++) deck.append(Poker(i,heart));
    for(int i = 0;i<13;i++) deck.append(Poker(i,spade));
    deck.append(Poker(13,joker));
    deck.append(Poker(14,joker));
    std::random_shuffle(deck.rbegin(),deck.rend());
    Poker ahand[17],
          bhand[17],
          chand[17],
          lords[3];
    for(int i = 0;i<17;i++) ahand[i] = deck[i];
    for(int i = 17;i<34;i++) bhand[i-17] = deck[i];
    for(int i = 34;i<51;i++) chand[i-34] = deck[i];
    for(int i = 51;i<54;i++) lords[i-51] = deck[i];
}

void lobby::askForLord(){
    for(int i = 0;i<3;i++){
        if(i==play_id){
            if(whetherLord()) lord_id = i;
        } else {
            QString info = "5 ";
            info += QString::number(i);
            sendMessageById(i,info);
        }
    }
}

void lobby::chooseLord(int id){
    lord_id = (lord_id < id) ? id : lord_id;
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
            info += QString::number(play_id);
            sendMessageByABC(0,info);
            return true;
        }
        return false;
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
    if(client_id == 1) play_id = b;
    if(client_id == 2) play_id = c;
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
        client_id = 0;
    } else if(listenSocket->listen(QHostAddress("127.0.0.1"),6002)) {
        ui->logshow->appendPlainText("欢迎加入房间，玩家B");
        QObject::connect(listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnectionMedia()));


        connectSocket_a->connectToHost(QHostAddress("127.0.0.1"),6001);
        connectABC[0] = connectSocket_a;
        QObject::connect(connectSocket_a,SIGNAL(readyRead()),mapper,SLOT(map()));

        client_id = 1;
    } else {
        ui->logshow->appendPlainText("欢迎加入房间，玩家C");
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

void lobby::sendMessageById(int id,QString m)
{
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(m);
    connectId[id]->write(array->data());
}

void lobby::sendMessageByABC(int id,QString m)
{
    QByteArray *array = new QByteArray;
    array->clear();
    array->append(m);
    connectABC[id]->write(array->data());
}

void lobby::receiveMessage(int from) {
    /*
    QString Info;
    switch (from) {
    case 0:Info += connectSocket_a->readAll();ui->logShow->append("A说: " + Info);break;
    case 1:Info += connectSocket_b->readAll();ui->logShow->append("B说: "+ Info);break;
    case 2:Info += readWriteSocket_b->readAll();ui->logShow->append("B说: "+ Info);break;
    case 3:Info += readWriteSocket_c->readAll();ui->logShow->append("C说: "+ Info);break;
    default:break;
    }
    */
}
