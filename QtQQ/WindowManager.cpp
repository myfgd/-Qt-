#include "WindowManager.h"
#include "talkwindow.h"
#include "TalkWindowItem.h"
#include <QSqlQueryModel>
Q_GLOBAL_STATIC(WindowManager,theInstance);

WindowManager::WindowManager()
	:QObject(nullptr),m_talkwindowshell(nullptr)
{}

WindowManager::~WindowManager()
{}

QWidget* WindowManager::findWindowName(const QString & qsWindowName)
{
	if (m_windowMap.contains(qsWindowName)) {
	
		return m_windowMap.value(qsWindowName);
	}

	return nullptr;
}

void WindowManager::deleteWindowName(const QString& qsWindowName)
{
	m_windowMap.remove(qsWindowName);
}

void WindowManager::addWindowName(const QString& qsWindowName, QWidget* qWidget)
{
	if (!m_windowMap.contains(qsWindowName)) {
	
		m_windowMap.insert(qsWindowName,qWidget);
	}
}

TalkWindowShell* WindowManager::getTalkWindowShell()
{
	return m_talkwindowshell;
}

WindowManager* WindowManager::getInstance()
{
	return theInstance;
}

void WindowManager::addNewTalkWindow(const QString& uid/*, GroupType groupType, const QString& strPeople*/)
{
	if (m_talkwindowshell == nullptr) {
	
		m_talkwindowshell = new TalkWindowShell;
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {
			m_talkwindowshell = nullptr;
			});
	}
	QWidget* widget = findWindowName(uid);
	if (!widget) {

		m_strCreatingTalkID = uid;
		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell,uid/*, groupType*/);
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);
		m_strCreatingTalkID = "";

		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID=%1").arg(uid);
		sqlDepModel.setQuery(strSql);
		int rows = sqlDepModel.rowCount();
		QString strWindowName, strMsgLabel;
		 
		if (rows == 0) {
		
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID=%1").arg(uid);
			sqlDepModel.setQuery(sql);
		}

		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0,0);
		signIndex = sqlDepModel.index(0,1);

		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();
		
		talkwindow->setWindowName(strWindowName);
		talkwindowItem->setMsgLabelContent(strMsgLabel);
		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, uid); 
		/*
		switch (groupType)
		{
		case COMPANY:{
			talkwindow->setWindowName(QStringLiteral("��ţ�Ƽ�-Խ����Խӵ��"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
			break;}
		case PERSONELGROUP:{
			talkwindow->setWindowName(QStringLiteral("���ġ����ġ����ġ���������"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("����Ⱥ"));
			break;}
		case DEVELOPMENTGROUP: {
			talkwindow->setWindowName(QStringLiteral("ֻ�����ֱ�����ԣ�һ�������찤��ģ�һ����û�����õ�"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("�з�Ⱥ"));
			break;}
		case MARKERGROUP: {
			talkwindow->setWindowName(QStringLiteral("���칤����Ŭ��������Ŭ���ҹ���"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("�г�Ⱥ"));
			break;}
		case PTOP: {
			talkwindow->setWindowName(QStringLiteral());
			talkwindowItem->setMsgLabelContent(strPeople);
			break;}
		default: {
			break;}
		}

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, groupType);*/
	
	}
	else {
	
		QListWidgetItem* item = m_talkwindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);

		m_talkwindowshell->setCurrentWidget(widget);
	}
	m_talkwindowshell->show();
	m_talkwindowshell->activateWindow();

}

QString WindowManager::getCreatingTalkID()
{
	return m_strCreatingTalkID;
}
