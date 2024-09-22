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
QString gfileName; //文件名称
QString gfileData; //文件数据
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
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("js 文件写入失败"));
	
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
	setWindowTitle(QString::fromLocal8Bit("奇奇-聊天窗口"));

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
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("读取  msgtmpl.txt 失败"));
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
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("读取  recvHtml.txt 失败"));
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

//文本数据包发送格式： 群聊标志+发信息员工QQ号+ 收信息员工QQ号(群号)+信息类型+数据长度+数据
//表情数据包发送格式： 群聊标志+发信息员工QQ号+ 收信息员工QQ号(群号)+信息类型+表情个数+images+表情数据

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
	//发送文本信息
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
				QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("不合理的数据长度"));
		}
		
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strDataLength + strData;
	
	}

	else if (msgType==0) {
	//表情信息
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;

	
	}
	else if (msgType==2) {
	//文件信息
	//文件数据包发送格式： 群聊标志+发信息员工QQ号+ 收信息员工QQ号(群号)+
	// 信息类型+文件长度+"bytes"+文件名称+data_begin+文件的内容
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());

		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2"
			+ strLength +"bytes"+fileName+"data_begin"+strData;
	}
	QByteArray dataBt;
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8(); //将字符串变成QbyteArray
	m_tcpClientSocket->write(dataBt);

}

/*****************************************************************************************************************************
	数据包格式
	文本数据包格式：群聊标志+发信息员工QQ号+收信息员工QQ号（群QQ号）+信息类型（1）+数据长度+数据
	表情数据包格式：群聊标志+发信息员工QQ号+收信息员工QQ号（群QQ号）+信息类型（0）+表情个数+images+表情名称
	文件数据包格式：群聊标志+发信息员工QQ号+收信息员工QQ号（群QQ号）+信息类型（2）+文件字节数+bytes+文件名+data_begin+文件数据

	群聊标志占1位，0表示单聊，1表示群聊
	信息类型占1位，0表示表情信息，1表示文本信息，2表示文件信息

	QQ号占5位，群QQ号占4位，数据长度占5位，表情名称占3位
	（注意：当群聊标志为1时，则数据包没有收信息员工QQ号，而是收信息群QQ号
			当群聊标志为0时，则数据包没有收信息群QQ号，而是收信息员工QQ号)

	群聊文本信息如：1100012001100005Hello  表示QQ10001向群2001发送文本信息，长度是5，数据为Hello
	单聊图片信息如：0100011000201images060 表示QQ10001向QQ10002发送1个表情60.png
	群聊文件信息如：1100052000210bytestest.txtdata_beginhelloworld
										   表示QQ10005向群2000发送文件信息，文件是test.txt，文件内容长度10，内容helloworld

	群聊文件信息解析：1 10001 2001 1 00005 Hello
	单聊图片信息解析：0 10001 10002 0 060
	群聊文件信息解析：1 10005 2000 2 10 bytes test.txt data_begin helloword

******************************************************************************************************************************/
void TalkWindowShell::processPendingData()
{
	//端口中有未处理的数据
	while (m_udpReceiver->hasPendingDatagrams())
	{
		const static int groupFlagWidth = 1;	//群聊标志占位
		const static int groupWidth = 4;		//群QQ号宽度
		const static int employeeWidth = 5;		//员工QQ号宽度
		const static int msgTypeWidth = 1;		//信息类型宽度
		const static int msgLengthWidth = 5;	//文本信息长度的宽度
		const static int pictureWidth = 3;		//表情图片的宽度

		//读取udp数据
		QByteArray btData;
		btData.resize(m_udpReceiver->pendingDatagramSize());
		m_udpReceiver->readDatagram(btData.data(), btData.size());

		QString strData = btData.data();
		QString strWindowID;//聊天窗口ID,群聊则是群号，单聊则是员工QQ号
		QString strSendEmployeeID, strRecevieEmployeeID;//发送及接收端的QQ号
		QString strMsg;		//数据

		int msgLen;	//数据长度
		int msgType;//数据类型

		strSendEmployeeID = strData.mid(groupFlagWidth, employeeWidth);

		//自己发的信息不做处理
		if (strSendEmployeeID == gLoginEmployeeID)
		{
			return;
		}

		if (btData[0] == '1')//群聊
		{
			//群QQ号
			strWindowID = strData.mid(groupFlagWidth + employeeWidth, groupWidth);

			
			if (btData[groupFlagWidth + employeeWidth + groupWidth] == '1')//文本信息
			{
				msgType = 1;
				msgLen = strData.mid(groupFlagWidth + employeeWidth
					+ groupWidth + msgTypeWidth, msgLengthWidth).toInt();
				strMsg = strData.mid(groupFlagWidth + employeeWidth
					+ groupWidth + msgType + msgLengthWidth, msgLen);
			}
			else if (btData[groupFlagWidth + employeeWidth + groupWidth] == '0')//表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				strMsg = strData.right(strData.length() - posImages - QString("images").length());
			}
			else if (btData[groupFlagWidth + employeeWidth + groupWidth] == '2')//文件信息
			{
				msgType = 2;
				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int posData_begin = strData.indexOf("data_begin");

				//文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;
				//文件内容
				int dataLengthWidth;
				int posData = posData_begin + QString("data_begin").length();
				strMsg = strData.mid(posData);
				gfileData = strMsg;
				//根据employeeID获取发送者姓名
				QString sender;
				int employeeID = strSendEmployeeID.toInt();
				QSqlQuery querySenderName(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
					.arg(employeeID));
				querySenderName.exec();

				if (querySenderName.first())
				{
					sender = querySenderName.value(0).toString();
				}

				//接收文件的后续操作。。。
				ReceiveFile* recvFile = new ReceiveFile(this);
				connect(recvFile, &ReceiveFile::refuseFile, [this]() {
					return;
					});

				QString msgLabel = QString::fromLocal8Bit("收到来自") + sender
					+ QString::fromLocal8Bit("发来的文件，是否接受？");
				recvFile->setMsg(msgLabel);
				recvFile->show();
			}
		}
		else//单聊
		{
			strRecevieEmployeeID = strData.mid(groupFlagWidth + employeeWidth, employeeWidth);
			strWindowID = strSendEmployeeID;

			//不是发给我的信息不做处理
			if (strRecevieEmployeeID != gLoginEmployeeID) {
			
				return;
			}


			//获取信息的类型

			if (btData[groupFlagWidth + employeeWidth + employeeWidth] == '1')//文本信息
			{
				msgType = 1;

				//文本信息长度
				msgLen = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth, msgLengthWidth).toInt();

				//文本信息
				strMsg = strData.mid(groupFlagWidth + employeeWidth + employeeWidth
					+ msgTypeWidth + msgLengthWidth, msgLen);
			}
			else if (btData[groupFlagWidth + employeeWidth + employeeWidth] == '0')//表情信息
			{
				msgType = 0;
				int posImages = strData.indexOf("images");
				int imagesWidth = QString("images").length();
				strMsg = strData.mid(posImages + imagesWidth);
			}
			else if (btData[groupFlagWidth + employeeWidth + employeeWidth] == '2')//文件信息
			{
				msgType = 2;

				int bytesWidth = QString("bytes").length();
				int posBytes = strData.indexOf("bytes");
				int data_beginWidth = QString("data_begin").length();
				int posData_begin = strData.indexOf("data_begin");

				//文件名称
				QString fileName = strData.mid(posBytes + bytesWidth, posData_begin - posBytes - bytesWidth);
				gfileName = fileName;
				//文件内容
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

				QString msgLabel = QString::fromLocal8Bit("收到来自") + sender
					+ QString::fromLocal8Bit("发来的文件，是否接受？");
				recvFile->setMsg(msgLabel);
				recvFile->show();

			}
		}

		//将聊天窗口设为活动的窗口
		QWidget* widget = WindowManager::getInstance()->findWindowName(strWindowID);
		if (widget)//聊天窗口存在
		{
			this->setCurrentWidget(widget);

			//同步激活左侧聊天窗口
			QListWidgetItem* item = m_talkwindowItemMap.key(widget);
			item->setSelected(true);
		}
		else//聊天窗口未打开
		{
			return;
		}

		//文件信息另作处理
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

	if (msgType == 1)//文本信息
	{
		msgTextEdit.document()->toHtml();
	}
	else if (msgType == 0)//表情信息
	{
		const int emotionWidth = 3;
		int emotionNum = strMsg.length() / emotionWidth;

		for (int i = 0; i < emotionNum; i++)
		{
			msgTextEdit.addEmotionUrl(strMsg.mid(i * emotionWidth, emotionWidth).toInt());
		}
	}

	QString html = msgTextEdit.document()->toHtml();

	//文本html如果没有字体则添加字体
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
			QMessageBox::information(this, QString::fromLocal8Bit("提示")
				, QString::fromLocal8Bit("文件 msgFont.txt 不存在！"));
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