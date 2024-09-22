#include "TalkWindowShell.h"
#include <QListWidget>
#include "CommonUtils.h"
#include "EmotionWindow.h"
#include "talkwindow.h"
#include "TalkWindowItem.h"
#include <QMap>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>
#include <QSqlQuery>
#include "WindowManager.h"
#include "ReceiveFile.h"
QString gfileName; //�ļ�����
QString gfileData; //�ļ�����
const int gtcpPort = 8888;
const int UdpPort = 6666;
extern  QString gLoginEmployeeID;
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSoceket();
	initUdpSocket();
	QStringList employeesIDList = getEmployeesID();
	QFile file("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) {
	
	if (!createJSFile(employeesIDList)) {
	
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("js �ļ�д��ʧ��"));
	
	}
	
	
	}
	
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow =nullptr;
}

void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem,const QString& uid/*, GroupType grouptype*/)
{

	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow,SIGNAL(signalEmotionWindowHide()),
		talkWindow,SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem,talkWindow);
	aItem->setSelected(true);

	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture FROM tab_department WHERE departmentID=%1").arg(uid);;
	sqlDepModel.setQuery(strQuery);
	int rows = sqlDepModel.rowCount();
	if (rows == 0) {
		
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);

	QImage img;
	img.load(sqlDepModel.data(index).toString());


	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem,talkWindowItem);
	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem,&TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]() {
		
			m_talkwindowItemMap.remove(aItem);
			talkWindow->close();
			ui.listWidget->takeItem(ui.listWidget->row(aItem));
			delete talkWindowItem;
			ui.rightStackedWidget->removeWidget(talkWindow);

			if (ui.rightStackedWidget->count() < 1) {
			
				close();
			}
		
		});

	
}

void TalkWindowShell::setCurrentWidget(QWidget* widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

const QMap<QListWidgetItem*, QWidget*> TalkWindowShell::getTalkWindowItemMap() const
{
	return m_talkwindowItemMap;
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("����-���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);

	ui.listWidget->setStyle(new CustomProxyStyle(this));
	
	connect(ui.listWidget,&QListWidget::itemClicked,this,&TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow,SIGNAL(signalEmotionItemClicked(int)),this,SLOT(onEmotionItemClicked(int)));
}

void TalkWindowShell::initTcpSoceket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	m_tcpClientSocket->connectToHost("127.0.0.1",gtcpPort);
}

void TalkWindowShell::initUdpSocket()
{
	m_udpReceiver = new QUdpSocket(this);
	for (quint16 port = UdpPort; port < UdpPort + 200; ++port) {
	
		if (m_udpReceiver->bind(port, QUdpSocket::ShareAddress)) {
			break;
		
		}
	
	}
	connect(m_udpReceiver, &QUdpSocket::readyRead, this, &TalkWindowShell::processPendingData);
}

QStringList TalkWindowShell::getEmployeesID()
{
	QStringList employeesIDList;
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT employeeID FROM  tab_employees WHERE status=1");
	int employeesNum = queryModel.rowCount();
	QModelIndex index;
	for (int i = 0; i < employeesNum;++i) {
		index = queryModel.index(i, 0);
		employeesIDList << queryModel.data(index).toString();
	}
	
	return employeesIDList;
}

bool TalkWindowShell::createJSFile(QStringList& employeesList)
{
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile;
	if (fileRead.open(QIODevice::ReadOnly)) {
	
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else {
	
		QMessageBox::information(this,
			QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��ȡ  msgtmpl.txt ʧ��"));
		return false;
	}
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		QString strSourceInitNull = "var external = null;";
		QString strSourceInit = "external = channel.objects.external;";
		QString strSourceNew =
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		);\
			";

		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly)) {
		
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		
		}
		else {
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("��ȡ  recvHtml.txt ʧ��"));
			return false;
		
		}

		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;

		for (int i = 0; i < employeesList.length(); i++) {
		
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external",QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
			strReplaceRecvHtml +="\n";
		
		}

		strFile.replace(strSourceInitNull,strReplaceInitNull);
		strFile.replace(strSourceInit,strReplaceInit);
		//strFile.replace(strSourceNew,strReplaceNew);
		strFile.replace(strSourceRecvHtml,strReplaceRecvHtml);

		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();
		return true;
	
	}

	
}

//�ı����ݰ����͸�ʽ�� Ⱥ�ı�־+����ϢԱ��QQ��+ ����ϢԱ��QQ��(Ⱥ��)+��Ϣ����+���ݳ���+����
//�������ݰ����͸�ʽ�� Ⱥ�ı�־+����ϢԱ��QQ��+ ����ϢԱ��QQ��(Ⱥ��)+��Ϣ����+�������+images+��������

void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName)
{
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	QString talkId = curTalkWindow->getTalkID();

	QString strGroupFlag;
	QString strSend;
	if (talkId.length() == 4) {
	
		strGroupFlag = "1";
	}
	else {
	
		strGroupFlag = "0";
	}
	int nstrDataLength = strData.length();
	int dataLength = QString::number(nstrDataLength).length();
	const int sourceDataLength = dataLength;
	QString strDataLength;

	if (msgType == 1) {
	//�����ı���Ϣ
		if (dataLength == 1) {
		strDataLength = "0000" + QString::number(nstrDataLength);
		}
		else if (dataLength == 2) {
		
			strDataLength = "000" + QString::number(nstrDataLength);
		
		}
		else if (dataLength == 3) {

			strDataLength = "00" + QString::number(nstrDataLength);

		}
		else if (dataLength == 4) {

			strDataLength = "0" + QString::number(nstrDataLength);

		}
		else if (dataLength == 5) {

			strDataLength = QString::number(nstrDataLength);

		}
		else {
		
			QMessageBox::information(this,
				QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("����������ݳ���"));
		}
		
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strDataLength + strData;
	
	}

	else if (msgType==0) {
	//������Ϣ
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;

	
	}
	else if (msgType==2) {
	//�ļ���Ϣ
	//�ļ����ݰ����͸�ʽ�� Ⱥ�ı�־+����ϢԱ��QQ��+ ����ϢԱ��QQ��(Ⱥ��)+
	// ��Ϣ����+�ļ�����+"bytes"+�ļ�����+data_begin+�ļ�������
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());

		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2"
			+ strLength +"bytes"+fileName+"data_begin"+strData;
	}
	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8(); //���ַ������QbyteArray
	m_tcpClientSocket->write(dataBt);

}

/*****************************************************************************************************************************
	���ݰ���ʽ
	�ı����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ�ţ�ȺQQ�ţ�+��Ϣ���ͣ�1��+���ݳ���+����
	�������ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ�ţ�ȺQQ�ţ�+��Ϣ���ͣ�0��+�������+images+��������
	�ļ����ݰ���ʽ��Ⱥ�ı�־+����ϢԱ��QQ��+����ϢԱ��QQ�ţ�ȺQQ�ţ�+��Ϣ���ͣ�2��+�ļ��ֽ���+bytes+�ļ���+data_begin+�ļ�����

	Ⱥ�ı�־ռ1λ��0��ʾ���ģ�1��ʾȺ��
	��Ϣ����ռ1λ��0��ʾ������Ϣ��1��ʾ�ı���Ϣ��2��ʾ�ļ���Ϣ

	QQ��ռ5λ��ȺQQ��ռ4λ�����ݳ���ռ5λ����������ռ3λ
	��ע�⣺��Ⱥ�ı�־Ϊ1ʱ�������ݰ�û������ϢԱ��QQ�ţ���������ϢȺQQ��
			��Ⱥ�ı�־Ϊ0ʱ�������ݰ�û������ϢȺQQ�ţ���������ϢԱ��QQ��)

	Ⱥ���ı���Ϣ�磺1100012001100005Hello  ��ʾQQ10001��Ⱥ2001�����ı���Ϣ��������5������ΪHello
	����ͼƬ��Ϣ�磺0100011000201images060 ��ʾQQ10001��QQ10002����1������60.png
	Ⱥ���ļ���Ϣ�磺1100052000210bytestest.txtdata_beginhelloworld
										   ��ʾQQ10005��Ⱥ2000�����ļ���Ϣ���ļ���test.txt���ļ����ݳ���10������helloworld

	Ⱥ���ļ���Ϣ������1 10001 2001 1 00005 Hello
	����ͼƬ��Ϣ������0 10001 10002 0 060
	Ⱥ���ļ���Ϣ������1 10005 2000 2 10 bytes test.txt data_begin helloword

******************************************************************************************************************************/
void TalkWindowShell::processPendingData()
{
	//�˿�����δ���������
	while (m_udpReceiver->hasPendingDatagrams())
	{
		const static int groupFlagWidth = 1;	//Ⱥ�ı�־ռλ
		const static int groupWidth = 4;		//ȺQQ�ſ��
		const static int employeeWidth = 5;		//Ա��QQ�ſ��
		const static int msgTypeWidth = 1;		//��Ϣ���Ϳ��
		const static int msgLengthWidth = 5;	//�ı���Ϣ���ȵĿ��
		const static int pictureWidth = 3;		//����ͼƬ�Ŀ��

		//��ȡudp����
		QByteArray btData;
		btData.resize(m_udpReceiver->pendingDatagramSize());
		m_udpReceiver->readDatagram(btData.data(), btData.size());

		QString strData = btData.data();
		QString strWindowID;//���촰��ID,Ⱥ������Ⱥ�ţ���������Ա��QQ��
		QString strSendEmployeeID, strRecevieEmployeeID;//���ͼ����ն˵�QQ��
		QString strMsg;		//����

		int msgLen;	//���ݳ���
		int msgType;//��������

		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);

		//�Լ�������Ϣ��������
		if (strSendEmployeeID == gLoginEmployeeID)
		{
			return;
		}

		if (btData[0] == '1')//Ⱥ��
		{
			//ȺQQ��
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);

			
			if (btData[groupFlagWidth + employeeWidth + groupWidth] == '1')//�ı���Ϣ
			{
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth
					+ groupWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth
					+ groupWidth + msgType + msgLengthWidth, msgLen);
			}
			else if (btData[groupFlagWidth + employeeWidth + groupWidth] == '0')//������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());
			}
			else if (btData[groupFlagWidth + employeeWidth + groupWidth] == '2')//�ļ���Ϣ
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int posData_begin = strData.indexOf("data_begin");

				//�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;
				//�ļ�����
				int dataLengthWidth;
				int posData = posData_begin + QString("data_begin").length();
				strMsg = strData.mid(posData);
				gfileData = strMsg;
				//����employeeID��ȡ����������
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
					.arg(employeeID));
				querySenderName.exec();

				if (querySenderName.first())
				{
					sender = querySenderName.value(0).toString();
				}

				//�����ļ��ĺ�������������
				ReceiveFile* recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
					});

				QString msgLabel = QString::fromLocal8Bit("�յ�����") + sender
					+ QString::fromLocal8Bit("�������ļ����Ƿ���ܣ�");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}
		}
		else//����
		{
			strRecevieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;

			//���Ƿ����ҵ���Ϣ��������
			if (strRecevieEmployeeID != gLoginEmployeeID) {
			
				return;
			}


			//��ȡ��Ϣ������

			if (btData[groupFlagWidth + employeeWidth + employeeWidth] == '1')//�ı���Ϣ
			{
				msgType = 1;

				//�ı���Ϣ����
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth, msgLengthWidth).toInt();

				//�ı���Ϣ
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (btData[groupFlagWidth + employeeWidth + employeeWidth] == '0')//������Ϣ
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				strMsg = strData.mid(posImages + imagesWidth);
			}
			else if (btData[groupFlagWidth + employeeWidth + employeeWidth] == '2')//�ļ���Ϣ
			{
				msgType = 2;

				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_beginWidth = QString("data_begin").length();
				int posData_begin = strData.indexOf("data_begin");

				//�ļ�����
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;
				//�ļ�����
				strMsg = strData.mid(posData_begin + data_beginWidth);
				gfileData = strMsg;
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
					.arg(employeeID));
				querySenderName.exec();

				if (querySenderName.first())
				{
					sender = querySenderName.value(0).toString();
				}

				ReceiveFile* recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
					});

				QString msgLabel = QString::fromLocal8Bit("�յ�����") + sender
					+ QString::fromLocal8Bit("�������ļ����Ƿ���ܣ�");
				recvFile->setMsg(msgLabel);
				recvFile->show();

			}
		}

		//�����촰����Ϊ��Ĵ���
		QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget)//���촰�ڴ���
		{
			this->setCurrentWidget(widget);

			//ͬ������������촰��
			QListWidgetItem* item = m_talkwindowItemMap.key(widget);
			item->setSelected(true);
		}
		else//���촰��δ��
		{
			return;
		}

		//�ļ���Ϣ��������
		if (msgType != 2)
		{
			int sendEmployeeID = strSendEmployeeID.toInt();
			handleReceivedMsg(sendEmployeeID, msgType, strMsg);
		}

	}
}

void TalkWindowShell::handleReceivedMsg(int senderEmployeeID, int msgType, QString strMsg)
{
	QMsgTextEdit msgTextEdit;
	msgTextEdit.setText(strMsg);

	if (msgType == 1)//�ı���Ϣ
	{
		msgTextEdit.document()->toHtml();
	}
	else if (msgType == 0)//������Ϣ
	{
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;

		for (int i = 0; i < emotionNum; i++)
		{
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString html = msgTextEdit.document()->toHtml();

	//�ı�html���û���������������
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			fontHtml.replace("%1", strMsg);
			file.close();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ")
				, QString::fromLocal8Bit("�ļ� msgFont.txt �����ڣ�"));
			return;
		}

		if (!html.contains(fontHtml))
		{
			html.replace(strMsg, fontHtml);
		}
	}

	TalkWindow* talkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	talkWindow->ui.msgWidget->appendMsg(html, QString::number(senderEmployeeID));
}

void TalkWindowShell::onEmotionBtnClicked(bool)
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	QPoint emotionPoint = this->mapToGlobal(QPoint(0,0));
	emotionPoint.setX(emotionPoint.x()+170);
	emotionPoint.setY(emotionPoint.y()+220);
	m_emotionWindow->move(emotionPoint);
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item) {
	QWidget* talkwindowWidget = m_talkwindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);


}


void TalkWindowShell::onEmotionItemClicked(int emotionNum) {

	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {

		curTalkWindow->addEmotionImage(emotionNum);

	}
}