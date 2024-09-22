#include "TcpSocket.h"

TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

void TcpSocket::run()
{
	m_descriptor = this->socketDescriptor();
	connect(this,SIGNAL(readyRead()),this,SLOT(onReceiveData()));
	connect(this,SIGNAL(disconnected()),this,SLOT(onClientDisconnect()));
}
void TcpSocket::onClientDisconnect()
{
	emit signalClientDisconnect(m_descriptor);
}
void TcpSocket::onReceiveData() {

	QByteArray buffer = this->readAll();
	if (!buffer.isEmpty()) {
	
		QString strData = QString::fromLocal8Bit(buffer);
		emit signalGetDataFromClient(buffer,m_descriptor);
	}

}