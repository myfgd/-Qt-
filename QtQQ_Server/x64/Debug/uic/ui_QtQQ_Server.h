/********************************************************************************
** Form generated from reading UI file 'QtQQ_Server.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTQQ_SERVER_H
#define UI_QTQQ_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtQQ_ServerClass
{
public:
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QTableWidget *tableWidget;
    QWidget *widget;
    QLabel *label;
    QGroupBox *groupBox;
    QFrame *line;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QComboBox *employeeDepBox;
    QLineEdit *nameLineEdit;
    QPushButton *selectPictureBtn;
    QPushButton *addBtn;
    QLabel *headLabel;
    QPushButton *queryDepartmentBtn;
    QComboBox *departmentBox;
    QPushButton *queryIDBtn;
    QPushButton *logoutBtn;
    QLineEdit *queryIDlineEdit;
    QLineEdit *logoutIDLineEdit;

    void setupUi(QDialog *QtQQ_ServerClass)
    {
        if (QtQQ_ServerClass->objectName().isEmpty())
            QtQQ_ServerClass->setObjectName(QStringLiteral("QtQQ_ServerClass"));
        QtQQ_ServerClass->resize(999, 502);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtQQ_ServerClass->sizePolicy().hasHeightForWidth());
        QtQQ_ServerClass->setSizePolicy(sizePolicy);
        QtQQ_ServerClass->setMinimumSize(QSize(999, 502));
        QtQQ_ServerClass->setMaximumSize(QSize(999, 502));
        scrollArea = new QScrollArea(QtQQ_ServerClass);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(1, 1, 700, 500));
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(700, 500));
        scrollArea->setMaximumSize(QSize(700, 500));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 698, 498));
        tableWidget = new QTableWidget(scrollAreaWidgetContents);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 0, 700, 500));
        scrollArea->setWidget(scrollAreaWidgetContents);
        widget = new QWidget(QtQQ_ServerClass);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(702, 1, 296, 500));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 0, 250, 200));
        label->setPixmap(QPixmap(QString::fromUtf8("girl.png")));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 210, 299, 291));
        line = new QFrame(groupBox);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 170, 299, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 30, 72, 15));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 54, 72, 15));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 80, 72, 15));
        employeeDepBox = new QComboBox(groupBox);
        employeeDepBox->setObjectName(QStringLiteral("employeeDepBox"));
        employeeDepBox->setGeometry(QRect(170, 30, 113, 21));
        nameLineEdit = new QLineEdit(groupBox);
        nameLineEdit->setObjectName(QStringLiteral("nameLineEdit"));
        nameLineEdit->setGeometry(QRect(170, 56, 113, 20));
        selectPictureBtn = new QPushButton(groupBox);
        selectPictureBtn->setObjectName(QStringLiteral("selectPictureBtn"));
        selectPictureBtn->setGeometry(QRect(230, 80, 51, 24));
        addBtn = new QPushButton(groupBox);
        addBtn->setObjectName(QStringLiteral("addBtn"));
        addBtn->setGeometry(QRect(170, 140, 113, 24));
        headLabel = new QLabel(groupBox);
        headLabel->setObjectName(QStringLiteral("headLabel"));
        headLabel->setGeometry(QRect(140, 84, 81, 51));
        headLabel->setAlignment(Qt::AlignCenter);
        queryDepartmentBtn = new QPushButton(groupBox);
        queryDepartmentBtn->setObjectName(QStringLiteral("queryDepartmentBtn"));
        queryDepartmentBtn->setGeometry(QRect(170, 180, 113, 24));
        departmentBox = new QComboBox(groupBox);
        departmentBox->setObjectName(QStringLiteral("departmentBox"));
        departmentBox->setGeometry(QRect(20, 180, 113, 21));
        queryIDBtn = new QPushButton(groupBox);
        queryIDBtn->setObjectName(QStringLiteral("queryIDBtn"));
        queryIDBtn->setGeometry(QRect(170, 210, 113, 24));
        logoutBtn = new QPushButton(groupBox);
        logoutBtn->setObjectName(QStringLiteral("logoutBtn"));
        logoutBtn->setGeometry(QRect(170, 240, 113, 24));
        queryIDlineEdit = new QLineEdit(groupBox);
        queryIDlineEdit->setObjectName(QStringLiteral("queryIDlineEdit"));
        queryIDlineEdit->setGeometry(QRect(20, 210, 113, 20));
        logoutIDLineEdit = new QLineEdit(groupBox);
        logoutIDLineEdit->setObjectName(QStringLiteral("logoutIDLineEdit"));
        logoutIDLineEdit->setGeometry(QRect(20, 240, 113, 20));

        retranslateUi(QtQQ_ServerClass);

        QMetaObject::connectSlotsByName(QtQQ_ServerClass);
    } // setupUi

    void retranslateUi(QDialog *QtQQ_ServerClass)
    {
        QtQQ_ServerClass->setWindowTitle(QApplication::translate("QtQQ_ServerClass", "QtQQ_Server", Q_NULLPTR));
        label->setText(QString());
        groupBox->setTitle(QString());
        label_2->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\351\203\250\351\227\250:", Q_NULLPTR));
        label_3->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\247\223\345\220\215:", Q_NULLPTR));
        label_4->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247:", Q_NULLPTR));
        employeeDepBox->clear();
        employeeDepBox->insertItems(0, QStringList()
         << QApplication::translate("QtQQ_ServerClass", "\344\272\272\344\272\213\351\203\250", Q_NULLPTR)
         << QApplication::translate("QtQQ_ServerClass", "\347\240\224\345\217\221\351\203\250", Q_NULLPTR)
         << QApplication::translate("QtQQ_ServerClass", "\345\270\202\345\234\272\351\203\250", Q_NULLPTR)
        );
        nameLineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245\345\247\223\345\220\215", Q_NULLPTR));
        selectPictureBtn->setText(QApplication::translate("QtQQ_ServerClass", "...", Q_NULLPTR));
        addBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\226\260\345\242\236\345\221\230\345\267\245", Q_NULLPTR));
        headLabel->setText(QApplication::translate("QtQQ_ServerClass", "\345\221\230\345\267\245\345\257\270\347\205\247", Q_NULLPTR));
        queryDepartmentBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\237\245\350\257\242\345\221\230\345\267\245", Q_NULLPTR));
        departmentBox->clear();
        departmentBox->insertItems(0, QStringList()
         << QApplication::translate("QtQQ_ServerClass", "\345\205\254\345\217\270\347\276\244", Q_NULLPTR)
         << QApplication::translate("QtQQ_ServerClass", "\344\272\272\344\272\213\351\203\250", Q_NULLPTR)
         << QApplication::translate("QtQQ_ServerClass", "\347\240\224\345\217\221\351\203\250", Q_NULLPTR)
         << QApplication::translate("QtQQ_ServerClass", "\345\270\202\345\234\272\351\203\250", Q_NULLPTR)
        );
        queryIDBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\237\245\350\257\242\345\221\230\345\267\245", Q_NULLPTR));
        logoutBtn->setText(QApplication::translate("QtQQ_ServerClass", "\346\263\250\351\224\200\345\221\230\345\267\245", Q_NULLPTR));
        queryIDlineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245ID\345\217\267", Q_NULLPTR));
        logoutIDLineEdit->setPlaceholderText(QApplication::translate("QtQQ_ServerClass", "\350\257\267\350\276\223\345\205\245\345\221\230\345\267\245ID\345\217\267", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QtQQ_ServerClass: public Ui_QtQQ_ServerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTQQ_SERVER_H
