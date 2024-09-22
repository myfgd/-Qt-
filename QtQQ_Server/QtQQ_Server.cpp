#include "QtQQ_Server.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QFileDialog>
const int tcpPort = 8888;
const int udpPort = 6666;
QtQQ_Server::QtQQ_Server(QWidget *parent)
    : QDialog(parent),m_pixPath("")
{
	ui.setupUi(this);
	if (!connectMySql()) {
	
		QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("数据库连接失败"));
		close();
		return;
	}

	setDepNameMap();
	setStatusMap();
	setOnlineMap();
	initComboxData();
	m_queryInfoModel.setQuery("SELECT * FROM tab_employees");
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	m_timer = new QTimer(this);
	m_timer->setInterval(200);
	m_timer->start();
	connect(m_timer,&QTimer::timeout,this,&QtQQ_Server::onRefresh);
	
	m_employeeID = 0;
	m_depID = getComDepID();
	m_comDepID = m_depID;
	
	updateTableData();


	initTcpSocket();
	initUdpSocket();
}

QtQQ_Server::~QtQQ_Server()
{}

void QtQQ_Server::initComboxData()
{
	QString itemText;
	QSqlQueryModel queryModel;
	queryModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryModel.rowCount() - 1;

	for (int i = 0; i < depCounts; i++) {
	
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(itemText));
		queryDepID.exec();
		queryDepID.first();

		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}

	for (int i = 0; i < depCounts+1; i++) {

		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'").arg(itemText));
		queryDepID.exec();
		queryDepID.first();

		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
	}


}

void QtQQ_Server::initTcpSocket()
{
    m_tcpServer = new TcpServer(tcpPort);
    m_tcpServer->run();

    connect(m_tcpServer, &TcpServer::signalTcpMsgComes,
        this, &QtQQ_Server::onUDPbroadMsg);
}

void QtQQ_Server::initUdpSocket()
{
	m_udpSender = new QUdpSocket(this);
}

void QtQQ_Server::setDepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("人事部"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("研发部"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("市场部"));


}

void QtQQ_Server::setStatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("有效"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("注销"));

}

void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("离线"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("在线"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("隐身"));

}

void  QtQQ_Server::onUDPbroadMsg(QByteArray& btData) {

	for (quint16 port = udpPort; port < udpPort + 200; ++port) {
	
		m_udpSender->writeDatagram(btData, btData.size(), QHostAddress::Broadcast, port);

	
	}


}

bool QtQQ_Server::connectMySql()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setDatabaseName("qtqq");
	db.setHostName("localhost");
	db.setUserName("root");
	db.setPassword("123456");
	db.setPort(3306);

	if (db.open()) {

		return true;
	}
	else {
		return false;
	}
}

int QtQQ_Server::getComDepID()
{
	QSqlQuery queryComDepID(QString("SELECT departmentID FROM tab_department WHERE department_name='%1'")
	.arg(QStringLiteral("公司群")));

	queryComDepID.exec();
	queryComDepID.next();
	int comDepID = queryComDepID.value(0).toInt();
	return comDepID;

}

void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();
	if (depID&&depID!=m_comDepID) {
	
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE departmentID=%1").arg(depID));

	}
	else if (employeeID) {
	
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID));

	}
	else {
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees"));
	
	
	} 

	int rows = m_queryInfoModel.rowCount();
	int columns = m_queryInfoModel.columnCount();

	QModelIndex index;

	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);

	QStringList headers;
	headers << QStringLiteral("部门")
		<< QStringLiteral("工号")
		<< QStringLiteral("员工姓名")
		<< QStringLiteral("员工签名")
		<< QStringLiteral("员工状态")
		<< QStringLiteral("员工头像")
		<< QStringLiteral("在线状态");
	ui.tableWidget->setHorizontalHeaderLabels(headers);

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) {
	
	
		for (int j = 0; j < columns; j++)
		{
			index = m_queryInfoModel.index(i, j);
			QString strData = m_queryInfoModel.data(index).toString();
			
			QSqlRecord record = m_queryInfoModel.record(i);
			QString strRecordName = record.fieldName(j);

			if (strRecordName == QStringLiteral("departmentID")) {
			
			
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));
				continue;
			}				
			else if (strRecordName == QStringLiteral("status")) {

				QString b =m_statusMap.value(strData);
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;
			}

			else if (strRecordName == QStringLiteral("online")) {


				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;
			}
			ui.tableWidget->setItem(i,j,new QTableWidgetItem(strData));




		}
	
	}


		
}

void QtQQ_Server::onRefresh()
{
	updateTableData(m_depID,m_employeeID);

}

void QtQQ_Server::on_queryDepartmentBtn_clicked()
{
	ui.queryIDlineEdit->clear();
	ui.logoutIDLineEdit->clear();
	m_employeeID = 0;
	m_depID = ui.departmentBox->currentData().toInt();
	updateTableData(m_depID);
	
}

void QtQQ_Server::on_queryIDBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);
	ui.logoutIDLineEdit->clear();
	m_depID = m_comDepID;
	if (!ui.queryIDlineEdit->text().length()) {
	
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("请输入员工QQ号"));
		ui.queryIDlineEdit->setFocus();
		return;
		
	}
	int employeeID = ui.queryIDlineEdit->text().toInt();
	QSqlQuery queryInfo(QStringLiteral("SELECT * FROM tab_employees WHERE employeeID=%1").arg(employeeID));
	queryInfo.exec();
	if (!queryInfo.next()) {
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("请输入正确的员工QQ号"));
		return;
	
	}
	else {
	
		m_employeeID = employeeID;
	
	}


	
}

void QtQQ_Server::on_logoutBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);
	ui.queryIDlineEdit->clear();

	if (!ui.logoutIDLineEdit->text().length()) {

		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("请输入员工QQ号"));
		ui.logoutIDLineEdit->setFocus();
		return;

	}

	int canCelEmployeeID = ui.logoutIDLineEdit->text().toInt();
	QSqlQuery queryInfo(QStringLiteral("SELECT employee_name FROM tab_employees WHERE employeeID=%1").arg(canCelEmployeeID));
	queryInfo.exec();
	if (!queryInfo.next()) {
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("请输入正确的员工QQ号"));
		ui.logoutIDLineEdit->setFocus();
		return;
	}


	QSqlQuery updateStatus(QString("UPDATE tab_employees SET status=0 WHERE employeeID=%1").arg(canCelEmployeeID));
	updateStatus.exec();
	QString employeeName = queryInfo.value(0).toString();
	QMessageBox::information(this,
		QStringLiteral("提示"),
		QString::fromLocal8Bit("员工%1 工号%2已注销").arg(employeeName).arg(canCelEmployeeID));



}

void QtQQ_Server::on_selectPictureBtn_clicked()
{
	m_pixPath = QFileDialog::getOpenFileName(
	this,
	QStringLiteral("选择头像"),
	".",
	"*.png;;*.jpg"
	);

	if (!m_pixPath.size()) {
		return;
	}

	QPixmap pixmap;
	pixmap.load(m_pixPath);

	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height() * heightRatio);
	ui.headLabel->setPixmap(pixmap.scaled(size));
}

void QtQQ_Server::on_addBtn_clicked()
{
	QString strName = ui.nameLineEdit->text();
	if (!strName.size()) {
	
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("请输入员工姓名！"));
		ui.nameLineEdit->setFocus();
		return;
	}
	if (!m_pixPath.size()) {
	
		QMessageBox::information(this,
			QStringLiteral("提示"),
			QStringLiteral("请输入员工头像路径！"));
		return;
	
	}
	QSqlQuery maxEmployeeID("SELECT MAX(employeeID) FROM tab_employees");
	maxEmployeeID.exec();
	maxEmployeeID.next();

	int employeeID = maxEmployeeID.value(0).toInt() + 1;

	int depID = ui.employeeDepBox->currentData().toInt();

	m_pixPath.replace("/", "\\\\");
	QSqlQuery insertSql(QString("INSERT INTO tab_employees(departmentID,employeeID,employee_name,picture)\
		VALUES(%1,%2,'%3','%4')")
	.arg(depID)
	.arg(employeeID)
	.arg(strName)
	.arg(m_pixPath));

	insertSql.exec();
	QMessageBox::information(this,
		QStringLiteral("提示"),
		QStringLiteral("新增员工成功"));
	m_pixPath = "";
	ui.headLabel->setText(QStringLiteral("员工寸照"));
	ui.nameLineEdit->clear();





}

 