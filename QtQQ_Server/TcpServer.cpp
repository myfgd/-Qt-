#include "TcpServer.h"
#include <QTcpSocket>
#include "TcpSocket.h"
TcpServer::TcpServer(int port):
	m_port(port)
{}

TcpServer::~TcpServer()
{}

bool TcpServer::run()
{
	if (this->listen(QHostAddress::AnyIPv4, m_port)) {
	
		qDebug() << QString::fromLocal8Bit("服务器监听端口%1 成功！").arg(m_port)<<endl;	
		return true;
	}
	else {
	
		qDebug() << QString::fromLocal8Bit("服务器监听端口%1 失败！").arg(m_port)<<endl;
		return false;
	}
	
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << QString::fromLocal8Bit("新的连接: ") << socketDescriptor << endl;
	TcpSocket* tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);
	tcpsocket->run();
	connect(tcpsocket,SIGNAL(signalGetDataFromClient(QByteArray&,int)),
		this,SLOT(SocketDataProcessing(QByteArray&,int)));
	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)),
		this, SLOT(SocketDisconnected(int)));
	m_tcpSocketConnectList.append(tcpsocket);
}

void TcpServer::SocketDisconnected(int descriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i) {

		QTcpSocket* item = m_tcpSocketConnectList.at(i);
		int itemDescriptor = item->socketDescriptor();
		if (itemDescriptor == descriptor||itemDescriptor==-1) {
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();
			qDebug() << QString::fromLocal8Bit("TcpSocket断开连接:") << descriptor << endl;
			return;

		}

	}
}

void TcpServer::SocketDataProcessing(QByteArray& SendData, int descriptor) {


	for (int i = 0; i < m_tcpSocketConnectList.count(); i++) {
	
		QTcpSocket* item = m_tcpSocketConnectList.at(i);
		if (item->socketDescriptor() == descriptor) {
			qDebug() <<QString::fromLocal8Bit( "来自IP:") << item->peerAddress().toString()
				<<QString::fromLocal8Bit( "发来的数据:" )<< QString(SendData)<<endl;

			emit signalTcpMsgComes(SendData);

		
		}
	
	}



}
