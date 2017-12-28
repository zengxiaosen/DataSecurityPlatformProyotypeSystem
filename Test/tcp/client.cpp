#include "client.h"

void Client::startConnected(QString host, quint16 port){
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()),
            this,SLOT(onConnected()));
    connect(m_socket, SIGNAL(readyRead()),
            this,SLOT(onReadyRead()));
    m_socket->connectToHost(host, port);
}

void Client::onConnected(){
    m_socket->write("GET / HTTP/1.1\r\n\r\n");
}

void Client::onReadyRead(){
    qDebug() << m_socket->readAll();
}
