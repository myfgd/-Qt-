#pragma once

#include <QWebEngineView>

#include <QDomNode>
class MsgHtmlObj :public QObject {
	Q_OBJECT
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)

public:
	MsgHtmlObj(QObject* parent,QString msgLPicPath="");
signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();
	QString getMsgTmplHtml(const QString& code);
private:
	QString m_msgLPicPath;
	QString m_msgLHtmlTmpl;
	QString m_msgRHtmlTmpl;



};

class MsgWebPage :public QWebEnginePage {

	Q_OBJECT
public:
	MsgWebPage(QObject* parent=nullptr) {}
protected:
	bool acceptNavigationRequest(const QUrl& url,NavigationType type,bool isMainFrame);


	 
};


class MsgWebView  : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString& html,QString strObj="0");
private:
	QList<QStringList> parseHtml(const QString& html);

	QList<QStringList> parseDocNode(const QDomNode& node);
signals:
	void signalSendMsg(QString& strData, int& msgType, QString sFile = "");
private:
	MsgHtmlObj* m_msgHtmlObj;
	QWebChannel* m_channel;
};
