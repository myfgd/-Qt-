#include "basicwindow.h"
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>
#include "CommonUtils.h"
#include <QPalette>
#include "notifymanager.h"
#include  <qDebug>
#include <QSqlQuery>
extern QString gLoginEmployeeID;
BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);
	//QPalette palette;
	//palette.setColor(QPalette::Background,Qt::white);
	//setPalette(palette);
	setAttribute(Qt::WA_TranslucentBackground,true);
	connect(NotifyManager::getInstance(),SIGNAL(signalSkinChanged(const QColor&)),this,SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{}


void BasicWindow::onSignalSkinChanged(const QColor& color) {

	m_colorBackGround = color;
	loadStyleSheet(m_styleName);

}

void BasicWindow::setTitleBarTitle(const QString& title,const QString& icon) {


	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::initTitleBar(ButtonType buttontype) {

	_titleBar = new titleBar(this);
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0,0);


	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));

}
void BasicWindow::loadStyleSheet(const QString& sheetName) {
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/"+sheetName+".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		/*qsstyleSheet += QString( "QWidget[titleskin=true]\
                                 {background-color:rgb(%1,%2,%3);\
                                  border-top-left-radius:4px;}\
                                  QWidget[bottomskin=true]\
								  {border-top:1px solid rgba(%1,%2,%3,100);\
								  background-color:rgba(%1,%2,%3,50);\
								  border-bottom-left-radius:4px;\
                                  border-bottom-right-radius:4px;}")
			                      .arg(r).arg(g).arg(b);*/
		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);\
								background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
			.arg(r).arg(g).arg(b);
		setStyleSheet(qsstyleSheet);
	
	}
	file.close();

}


void BasicWindow::initBackGroundColor() {
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);

}
void BasicWindow::paintEvent(QPaintEvent* event) {

	initBackGroundColor();
	QDialog::paintEvent(event);

}

QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize) {


	if (masksize == QSize(0, 0)) {
		masksize = mask.size();
	
	}

	else {
	
		mask=mask.scaled(masksize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
	}

	QImage resultImage(masksize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);

	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(),Qt::transparent);

	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0,0,mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0,0,src.scaled(masksize,Qt::KeepAspectRatio,Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);

}


void BasicWindow::onShowClose(bool) {


	close();
}

void BasicWindow::onShowMin(bool) {

	showMinimized();


}

void BasicWindow::onShowHide(bool) {

	hide();
}

void BasicWindow::onShowNormal(bool) {

	show();
	activateWindow(); 

}

void BasicWindow::onShowQuit(bool) {
	//更新登录状态为离线
	QString strSqlStatus = QString("UPDATE tab_employees SET online=1 WHERE employeeID=%1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();
	QApplication::quit();


}

void BasicWindow::mouseMoveEvent(QMouseEvent* e) {


	if (m_mosuePressed && (e->buttons() && Qt::LeftButton)) {
	
		move(e->globalPos() - m_mousePoint);
		e->accept();
	}
}


void BasicWindow::mousePressEvent(QMouseEvent* e) {

	if (e->button() == Qt::LeftButton) {
	
		m_mosuePressed = true;
		m_mousePoint = e->globalPos() - pos();
		e->accept();
	}


}


void BasicWindow::mouseReleaseEvent(QMouseEvent*) {

	m_mosuePressed = false;
}


void BasicWindow::onButtonMinClicked() {

	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
	
		hide();
	}
	else {
	
		showMinimized();
	}



}

void BasicWindow::onButtonRestoreClicked() {

	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos,windowSize);
	setGeometry(QRect(windowPos,windowSize));


}


void BasicWindow::onButtonCloseClicked() {


	close();

}

void BasicWindow::onButtonMaxClicked() {

	_titleBar->saveRestoreInfo(pos(),QSize(width(),height()));

	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
	


}