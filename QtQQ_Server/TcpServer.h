#pragma once

#include <QTcpServer>

class TcpServer  : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();
public:
	bool run();
protected:
	//客户端有新的连接时
	void incomingConnection(qintptr socketDescriptor);
signals:
	void signalTcpMsgComes(QByteArray&);
private slots:
	//处理数据
	void SocketDataProcessing(QByteArray& SendData,int descriptor);
	//断开连接
	void SocketDisconnected(int descriptor);
private:
	int m_port; 
	QList<QTcpSocket*> m_tcpSocketConnectList;
};
 