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
	//�ͻ������µ�����ʱ
	void incomingConnection(qintptr socketDescriptor);
signals:
	void signalTcpMsgComes(QByteArray&);
private slots:
	//��������
	void SocketDataProcessing(QByteArray& SendData,int descriptor);
	//�Ͽ�����
	void SocketDisconnected(int descriptor);
private:
	int m_port; 
	QList<QTcpSocket*> m_tcpSocketConnectList;
};
 