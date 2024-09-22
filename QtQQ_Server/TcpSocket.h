#pragma once

#include <QTcpSocket>

class TcpSocket  : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket();
	~TcpSocket();
	void run();
signals:
	void signalGetDataFromClient(QByteArray&,int);
	void signalClientDisconnect(int);
private slots:
	void onReceiveData();
	void onClientDisconnect();
private:
	int m_descriptor;
};
