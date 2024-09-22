#include "titlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#define BUTTON_HEIGHT 27
#define BUTTON_WIDTH 27
#define TITLE_HEIGHT  27

titleBar::titleBar(QWidget *parent)
	: QWidget(parent),m_isPressed(false),m_buttonType(MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");
}

titleBar::~titleBar()
{}

void titleBar::initControl() {

	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonClose = new QPushButton(this);
	m_pButtonMin = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);

	m_pButtonClose->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
	m_pButtonMax->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pButtonMin->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_pButtonRestore->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);

	m_pButtonClose->setObjectName("ButtonClose");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pTitleContent->setObjectName("TitleContent");

	QHBoxLayout* myLayout = new QHBoxLayout(this);
	myLayout->addWidget(m_pIcon);
	myLayout->addWidget(m_pTitleContent);

	myLayout->addWidget(m_pButtonMin);
	myLayout->addWidget(m_pButtonRestore);
	myLayout->addWidget(m_pButtonMax);
	myLayout->addWidget(m_pButtonClose);

	myLayout->setContentsMargins(5,0,0,0);
	myLayout->setSpacing(0);

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

	setFixedHeight(TITLE_HEIGHT);
	setWindowFlags(Qt::FramelessWindowHint); //нч╠ъ©Р

}


void titleBar::initConnections() {


	connect(m_pButtonClose,SIGNAL(clicked()),this,SLOT(onButtonCloseClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));

}

void titleBar::setTitleIcon(const QString& filePath) {

	QPixmap  titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());
	m_pIcon->setPixmap(titleIcon);

}

void titleBar::setTitleContent(const QString& titleContent) {
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;

}

void titleBar::setTitleWidth(int width) {


	setFixedWidth(width);
}

void titleBar::setButtonType(ButtonType buttontype) {


	switch (buttontype)
	{
	case MIN_BUTTON:
	{
		m_pButtonMax->setVisible(false);
		m_pButtonRestore->setVisible(false);
	}
		break;
	case MIN_MAX_BUTTON:
	{
		m_pButtonRestore->setVisible(false);
	}
		break;
	case ONLY_CLOSE_BUTTON:
	{
		m_pButtonMax->setVisible(false);
		m_pButtonRestore->setVisible(false);
		m_pButtonRestore->setVisible(false);
	}
		break;
	default:
		break;
	}

}

void titleBar::saveRestoreInfo(const QPoint& point,const QSize& size) {


	m_restorePos = point;
	m_restoreSize = size;
}


void titleBar::getRestoreInfo(QPoint& point,QSize& size) {


	point = m_restorePos;
	size = m_restoreSize;
}

void titleBar::paintEvent(QPaintEvent* event) {


	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

	if (width() != parentWidget()->width()) {
	
	
		setFixedWidth(parentWidget()->width());
	}
	QWidget::paintEvent(event);


}


void titleBar::mouseDoubleClickEvent(QMouseEvent* event) {


	if (m_buttonType == MIN_MAX_BUTTON) {
	
	
		if (m_pButtonMax->isVisible()) {
		
			onButtonMaxClicked();
		
		}
		else {
		
		
			onButtonRestoreClicked();
		}
	
	}
	return QWidget::mouseDoubleClickEvent(event);

}


void titleBar::mousePressEvent(QMouseEvent* event) {


	if (m_buttonType = MIN_MAX_BUTTON) {
	
		if (m_pButtonMax->isVisible()) {
		
		
			m_isPressed = true;
			m_startMovePos = event->globalPos();
		
		}
		
	
	
	}

	else {
	
	
	
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);


}


void titleBar::mouseMoveEvent(QMouseEvent* event) {


	if (m_isPressed) {
	
		QPoint movePoint = event->globalPos() - m_startMovePos;

		QPoint widgetPos = parentWidget()->pos();

		m_startMovePos = event->globalPos();

		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());

	
	}
	return QWidget::mouseMoveEvent(event);

}


void titleBar::mouseReleaseEvent(QMouseEvent* event) {

	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);

}


void titleBar::loadStyleSheet(const QString& sheetName) {

	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
	
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		setStyleSheet(styleSheet);
	
	
	
	}

}


void titleBar::onButtonCloseClicked() {


	signalButtonCloseClicked();


}

void titleBar::onButtonMinClicked() {


	signalButtonMinClicked();


}


void titleBar::onButtonMaxClicked() {


	m_pButtonRestore->setVisible(true);
	m_pButtonMax->setVisible(false);
	emit signalButtonMaxClicked();

}

void titleBar::onButtonRestoreClicked() {


	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();

}



