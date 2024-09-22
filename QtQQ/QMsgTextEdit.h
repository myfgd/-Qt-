#pragma once

#include <QTextEdit>
#include <QMovie>
#include <QUrl>
class QMsgTextEdit  : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent=nullptr);
	~QMsgTextEdit();
private:
	void insertFromMimeData(const QMimeData* source) override;
private slots:
	void onEmotionImageFrameChange(int frame);
public:
	void addEmotionUrl(int emotionNum);
	void deleteAllEmotionImage();
private:
	QList<QString> m_listEmotionUrl;
	QMap<QMovie*, QString> m_emotionMap;
	
};
