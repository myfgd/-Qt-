#include "CCMainWindow.h"
#include "basicwindow.h"
#include <QProxyStyle>
#include "SkinWindow.h"
#include <QPainter>
#include <QTimer>
#include "SysTray.h"
#include <QEvent>
#include "notifymanager.h"
#include <QTreeWidgetItem>
#include "RootContactItem.h"
#include "ContactItem.h"
#include "WindowManager.h"
#include "TalkWindowShell.h"
#include <QApplication>
#include <QSqlQuery>
#include <QSqlQueryModel>

extern  QString gLoginEmployeeID;
QString gstrLoginHeadPath;
class CustomProxyStyle : public QProxyStyle {

public:
    virtual void drawPrimitive(PrimitiveElement element,const QStyleOption* option
    ,QPainter* painter,const QWidget* widget=nullptr) {


        if (element == PE_FrameFocusRect) {
        
            return;
        }
        else {
         
            QProxyStyle::drawPrimitive(element,option,painter,widget);
        }
    
    }

};
CCMainWindow::CCMainWindow(QString account, bool isAccountLogin, QWidget *parent)
    : BasicWindow(parent)
    ,m_isAccountLogin(isAccountLogin),
    m_account(account)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags()|Qt::Tool);
    loadStyleSheet("CCMainWindow");
    setHeadPixmap(getHeadPicturePath());
    initControl();
    initTimer();
    initContactTree();
}

CCMainWindow::~CCMainWindow()
{}

void CCMainWindow::initControl()
{
    ui.treeWidget->setStyle(new CustomProxyStyle);
    setLevelPixmap(0);
   // setHeadPixmap(":/Resources/MainWindow/girl.png");
    setStatusMenIcon(":/Resources/MainWindow/StatusSucceeded.png");

    QHBoxLayout* appTopLayout = new QHBoxLayout;
    appTopLayout->setContentsMargins(0,0,0,0);

    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png","app_7"));
    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
    appTopLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
    appTopLayout->addStretch();
    appTopLayout->setSpacing(2);
    ui.appWidget->setLayout(appTopLayout);


    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
    ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
    ui.bottomLayout_up->addStretch();

    //安装事件过滤器
    ui.searchLineEdit->installEventFilter(this);
    ui.lineEdit->installEventFilter(this);
    connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
    connect(ui.sysclose, SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));
    connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
        
        updateSearchStyle();
        });

    SysTray* systray = new SysTray(this);

}

void CCMainWindow::setUserName(const QString& username)
{
    
    //ui.nameLabel->adjustSize();
    QString name = ui.nameLabel->fontMetrics().elidedText(username,Qt::ElideRight,ui.nameLabel->width());
    ui.nameLabel->setText(name);

}

void CCMainWindow::setLevelPixmap(int level)
{
    QPixmap levelPixmap(ui.levelBtn->size());
    levelPixmap.fill(Qt::transparent);

    QPainter painter(&levelPixmap);
    painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));


    int unitNum = level % 10;
    int tenNum = level / 10;

    painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), tenNum * 6, 0, 6, 7);

    painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

    ui.levelBtn->setIcon(levelPixmap);
    ui.levelBtn->setIconSize(ui.levelBtn->size()); 
    



}

void CCMainWindow::setHeadPixmap(const QString& headPath)
{
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath),pix,ui.headLabel->size()));
}

void CCMainWindow::setStatusMenIcon(const QString& statusPath)
{
    QPixmap statusBtnPixmap(ui.stausBtn->size());
    statusBtnPixmap.fill(Qt::transparent);

    QPainter painter(&statusBtnPixmap);
    painter.drawPixmap(4,4,QPixmap(statusPath));

    ui.stausBtn->setIcon(statusBtnPixmap);
    ui.stausBtn->setIconSize(ui.stausBtn->size());

}

QWidget* CCMainWindow::addOtherAppExtension(const QString& appPath, const QString& appName)
{

    QPushButton* btn = new QPushButton(this);
    btn->setFixedSize(20,20);

    QPixmap pixmap(btn->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QPixmap appPixmap(appPath);
    painter.drawPixmap((btn->width()-appPixmap.width())/2,
        (btn->height()-appPixmap.height())/2,appPixmap
        );

    btn->setIcon(pixmap);
    btn->setIconSize(btn->size());
    btn->setObjectName(appName);
    btn->setProperty("hasborder",true);
    connect(btn,&QPushButton::clicked,this,&CCMainWindow::onAppIconClicked);
    return btn;
   
}

void CCMainWindow::initTimer()
{
    QTimer* timer = new QTimer;
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, [this]() {

        static int level = 0;
        level++;
        if (level == 99) {

            level = 0;
        }
        setLevelPixmap(level);

        });

    timer->start();

}

void CCMainWindow::initContactTree()
{
    connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(onItemClicked(QTreeWidgetItem*,int)));
    connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpand(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem* )));
    connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));  
    QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
    pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    pRootGroupItem->setData(0,Qt::UserRole,0);
    RootContactItem* pItemName = new RootContactItem;

    QSqlQuery queryComDepID(QString("SELECT departmentID FROM  tab_department WHERE department_name='%1'").arg(QString::fromLocal8Bit("公司群")));
    queryComDepID.exec();
    queryComDepID.first();
    int ComDepID = queryComDepID.value(0).toInt();

    QSqlQuery querySelfDepID(QString("SELECT departmentID FROM  tab_employees WHERE employeeID=%1").arg(gLoginEmployeeID));
    querySelfDepID.exec();
    querySelfDepID.first();
    int SelfDepID = querySelfDepID.value(0).toInt();

    


    addCompanyDeps(pRootGroupItem, ComDepID);
    addCompanyDeps(pRootGroupItem, SelfDepID);

    QString strGroupName = QString::fromLocal8Bit("奇牛科技");
    pItemName->setText(strGroupName);

    ui.treeWidget->addTopLevelItem(pRootGroupItem);
    ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName);
 /*   QStringList sCompDeps;
    sCompDeps << QString::fromLocal8Bit("公司群");
    sCompDeps << QString::fromLocal8Bit("人事部");
    sCompDeps << QString::fromLocal8Bit("研发部");
    sCompDeps << QString::fromLocal8Bit("市场部");
    for (int nIndex = 0;nIndex< sCompDeps.length();nIndex++) {
        addCompanyDeps(pRootGroupItem,sCompDeps.at(nIndex));
    }*/

}

QString CCMainWindow::getHeadPicturePath()
{
    QString strPicturePath;
    bool a = m_isAccountLogin;
    if (!m_isAccountLogin) {
        QSqlQuery queryPicture(QString("SELECT picture FROM tab_employees WHERE employeeID=%1 ").arg(gLoginEmployeeID));

        queryPicture.exec();
        queryPicture.next();
        strPicturePath = queryPicture.value(0).toString();
    }
    else {
        QSqlQuery queryEmployeeID(QString("SELECT employeeID FROM tab_accounts WHERE account='%1'").arg(m_account));
        queryEmployeeID.exec();
        queryEmployeeID.next();

        int employeeID = queryEmployeeID.value(0).toInt();

        QSqlQuery queryPicture(QString("SELECT picture FROM tab_employees WHERE employeeID=%1").arg(employeeID));
        queryPicture.exec();
        queryPicture.next();
        strPicturePath = queryPicture.value(0).toString();


    }
    gstrLoginHeadPath=strPicturePath;
    return strPicturePath;
}

void CCMainWindow::onItemExpand(QTreeWidgetItem* item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild) {
        ;
        RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
        if (prootItem) {
        
            prootItem->setExpanded(true);
        }
    } 
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem* item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild) {
        ;
        RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
        if (prootItem) {

            prootItem->setExpanded(false);
        }
    }
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (bIsChild) {
        WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());

        
        //QString strGroup = m_groupMap.value(item);
    /*    if (strGroup == QString::fromLocal8Bit("公司群")) {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), COMPANY);
        }
        else if (strGroup == QString::fromLocal8Bit("人事部") ){
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), PERSONELGROUP);
        }
        else if (strGroup == QString::fromLocal8Bit("市场部")) {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), MARKERGROUP);
        }
        else if (strGroup == QString::fromLocal8Bit("研发部")) {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), DEVELOPMENTGROUP);
        }*/
    }
}

void CCMainWindow::onItemClicked(QTreeWidgetItem* item, int column)
{
    bool bIsChild = item->data(0,Qt::UserRole).toBool();
    if (!bIsChild) {
        item->setExpanded(!item->isExpanded());
    
    }
}

void CCMainWindow::updateSearchStyle()
{ 
    ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3,30)}\
                QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}")
        .arg(m_colorBackGround.red())
        .arg(m_colorBackGround.green())
        .arg(m_colorBackGround.blue()));
                
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem* pRootGroupItem,  int DepID)
{
    QTreeWidgetItem* pChild = new QTreeWidgetItem;
    pChild->setData(0, Qt::UserRole, 1);
    pChild->setData(0, Qt::UserRole + 1, DepID);
    /* QPixmap pix(":/Resources/MainWindow/head_mask.png");*/
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");


    QPixmap groupPix;
    QSqlQuery queryPicture(QString("SELECT picture FROM tab_department WHERE  departmentId=%1").arg(DepID));
    queryPicture.exec();
    queryPicture.next();
    QString s = queryPicture.value(0).toString();
    groupPix.load(queryPicture.value(0).toString());
    
    QString strDepName;
    QSqlQuery queryDepName(QString("SELECT department_name FROM tab_department WHERE departmentID=%1").arg(DepID));
    queryDepName.exec();
    queryDepName.first();
    strDepName = queryDepName.value(0).toString();

    ContactItem* pContactItem = new ContactItem;
    pContactItem->setHeapPixmap(getRoundImage(groupPix, pix,pContactItem->getHeadLabelSize()));
 
   

    pContactItem->setUserName(strDepName);
    pRootGroupItem->addChild(pChild);
    ui.treeWidget->setItemWidget(pChild,0,pContactItem);
    //m_groupMap.insert(pChild,sDeps);
}

void CCMainWindow::resizeEvent(QResizeEvent* event)
{
    setUserName(QString::fromLocal8Bit("奇牛科技-越分享越拥有"));
    BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject* obj, QEvent* event)
{

    if (obj==ui.searchLineEdit) {
    

        if (event->type() == QEvent::FocusIn) {
        
        
            ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100)}\
                QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}\
                 QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}\
                QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhigh down.png)}\
              ").arg(m_colorBackGround.red())
                .arg(m_colorBackGround.green())
                .arg(m_colorBackGround.blue()));
        
        }
        else if (event->type()==QEvent::FocusOut) {
            updateSearchStyle();
        }
    
    }
    return false;
}

void CCMainWindow::mousePressEvent(QMouseEvent* event)
{
    if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus()){
        ui.searchLineEdit->clearFocus();
    }
    else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus()) {
        ui.lineEdit->clearFocus();
    }
    BasicWindow::mousePressEvent(event);
}

void CCMainWindow::onAppIconClicked() {

    if (sender()->objectName() == "app_skin") {
    
        SkinWindow* s = new SkinWindow;
        s->show();
    
    }
}

