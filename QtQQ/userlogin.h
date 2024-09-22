#pragma once

#include <QWidget>
#include "ui_userlogin.h"
#include "basicwindow.h"
class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = nullptr);
	~UserLogin();

private slots:
	void onLoginBtnClicked();
private:
	void initControl();
	bool connectMySql();
	bool verifyAccountCode(bool& isAccountLogin,QString& strAccount);

private:
	Ui::UserLogin ui;
};
