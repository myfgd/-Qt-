#pragma once

#include <QWidget>
#include "ui_talkwindow.h"
#include "TalkWindowShell.h"
#include <QMap>

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& uid/*, GroupType groupType*/);
	~TalkWindow();
public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);
	QString getTalkID();
private slots:
	void onFileOpenBtnClicked(bool);
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
	void initControl();
	void initGroupTalkStatus();
	int getComDepID();

 	/*void initCompanyTalk();
	void initPersonelTalk();
	void initDevelopTalk();
	void initMarketTalk();*/
	void initTalkWinow(); //³õÊ¼»¯ÈºÁÄ
	void initPtoPTalk();
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);
private:
	
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;
	//GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;
	friend class TalkWindowShell;
};
