#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>

enum ButtonType {
	MIN_BUTTON=0,
	MIN_MAX_BUTTON,
	ONLY_CLOSE_BUTTON

};
class titleBar  : public QWidget
{
		Q_OBJECT

public:
	titleBar(QWidget *parent);
	~titleBar();

	void setTitleIcon(const QString& filePath);
	void setTitleContent(const QString& titleContent);
	void setTitleWidth(int width);
	void setButtonType(ButtonType buttonType);

	void saveRestoreInfo(const QPoint& point,const QSize& size);
	void getRestoreInfo(QPoint& point,QSize& size);

private:
	void paintEvent(QPaintEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

	void initControl();
	void  initConnections();
	void loadStyleSheet(const QString& sheetName);
signals:
	void signalButtonMinClicked();
	void signalButtonMaxClicked();
	void signalButtonCloseClicked();
	void signalButtonRestoreClicked();
private slots:
	void onButtonMinClicked();
	void onButtonMaxClicked();
	void onButtonRestoreClicked();
	void onButtonCloseClicked();

private:
	QLabel* m_pIcon;
	QLabel* m_pTitleContent;
	QPushButton* m_pButtonMin;
	QPushButton* m_pButtonRestore;
	QPushButton* m_pButtonClose;
	QPushButton* m_pButtonMax;

	QPoint m_restorePos;
	QSize m_restoreSize;

	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;
	ButtonType m_buttonType;



};
