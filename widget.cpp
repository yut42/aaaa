#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , clientSocket(nullptr)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    connect(ui->startButton, &QPushButton::clicked, this, &Widget::startServer);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::startServer()
{
    if (server->listen(QHostAddress::AnyIPv4, 9000)) {
        connect(server, &QTcpServer::newConnection, this, &Widget::newClient);
        ui->startButton->setEnabled(false);
    }
}

void Widget::newClient()
{
    clientSocket = server->nextPendingConnection();
    QString clientAddress = clientSocket->peerAddress().toString();
    quint16 clientPort = clientSocket->peerPort();
    ui->lineEditAddress->setText(clientAddress);
    ui->lineEditPort->setText("9000");
    connect(clientSocket, &QTcpSocket::readyRead, this, &Widget::readMessage);
}

void Widget::readMessage()
{
    QString message = clientSocket->readAll();
    ui->lineEditData->setText(message);
}
