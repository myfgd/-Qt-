#include "userlogin.h"
#include "CCMainWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

QString gLoginEmployeeID;
UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	//this->setStyleSheet("background-color: white;");
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("",":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin()
{
}

void UserLogin::initControl() {

	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68,68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),pix,headlabel->size()));
	headlabel->move(width()/2-34,ui.titleWidget->height()-34);
	connect(ui.loginBtn,&QPushButton::clicked,this,&UserLogin::onLoginBtnClicked);
	if (!connectMySql()) 
	{
		QMessageBox::information(NULL,QString::fromLocal8Bit("提示")
		,QString::fromLocal8Bit("数据库连接失败！"));
		close();
	}
}

bool UserLogin::connectMySql()
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

bool UserLogin::verifyAccountCode(bool& isAccountLogin, QString& strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID=%1").arg(strAccountInput);

	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();

	if (queryEmployeeID.first()) {

		QString strCode = queryEmployeeID.value(0).toString();
		if (strCode == strCodeInput) {
			gLoginEmployeeID = strAccountInput;
			strAccount = strAccountInput;
			isAccountLogin = false;
			return true;
		}
		else {
		
			return false;
			}

	}



	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account='%1'").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();

	if (queryAccount.first()) {

		QString strCode = queryAccount.value(0).toString();
		if (strCode == strCodeInput) {
			gLoginEmployeeID = queryAccount.value(1).toString();
			isAccountLogin = true;
			strAccount = strAccountInput;
			return true;
		}
		else {

			return false;
		}

	}

	return false;
}


void UserLogin::onLoginBtnClicked() {
	bool isAccountLogin;
	QString strAccount;
	bool a = !verifyAccountCode(isAccountLogin,strAccount);
	if (a) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示")
			, QString::fromLocal8Bit("你输入的账号或密码有误，请重新输入！"));
		return;
	}
	close();
	//更新状态为在线
	QString strSqlStatus = QString("UPDATE tab_employees SET online=2 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();
	CCMainWindow* mainwindow = new CCMainWindow(strAccount,isAccountLogin);
	mainwindow->show();

}


