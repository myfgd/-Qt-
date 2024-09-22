#pragma once

#include <QWidget>
#include "ui_TalkWindowShell.h"
#include "basicwindow.h"
#include "TalkWindowItem.h"
#include <QMap>
#include <QTcpSocket>
#include <QUdpSocket>
class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

enum GroupType {

	COMPANY=0,
	PERSONELGROUP,
	DEVELOPMENTGROUP,
	MARKERGROUP,
	PTOP


};
class TalkWindowShell : public BasicWindow{
	Q_OBJECT

public :
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();
public:
	void addTalkWindow(TalkWindow* talkWindow,TalkWindowItem* talkWindowItem, const QString& uid/*, GroupType grouptype*/);

	void setCurrentWidget(QWidget* widget);
	const QMap<QListWidgetItem*, QWidget*> getTalkWindowItemMap() const;
private:
	void initControl();
	void initTcpSoceket();
	void initUdpSocket();
	QStringList getEmployeesID();
	bool createJSFile(QStringList& employeesList);
	void handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg);
public slots:
	void onEmotionBtnClicked(bool);
	void updateSendTcpMsg(QString& strData,int& msgType,QString fileName="");
	void processPendingData();//处理广播收到的数据
private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);
	void onEmotionItemClicked(int emotionNum);

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;
	EmotionWindow* m_emotionWindow;
	QTcpSocket* m_tcpClientSocket; //客户端
	QUdpSocket* m_udpReceiver;

};
