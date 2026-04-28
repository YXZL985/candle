#include "mainwindow.h"

#include <DApplication>
#include <DWidgetUtil>

#include <QIcon>

DWIDGET_USE_NAMESPACE

/**
 * @brief 应用程序入口函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出码
 */
int main(int argc, char *argv[])
{
    // 创建 DApplication 实例
    DApplication a(argc, argv);
    
    // 设置应用组织名称
    a.setOrganizationName("YXZL985");
    
    // 设置应用名称
    a.setApplicationName("candle");
    
    // 设置应用版本
    a.setApplicationVersion("1.0.0");
    
    // 设置产品图标
    a.setProductIcon(QIcon(":/images/logo.svg"));
    
    // 设置产品名称（英文）
    a.setProductName("Candle");
    
    // 设置应用描述
    a.setApplicationDescription(QCoreApplication::translate("Main", "秉烛 - 基于 DTK6 的应用程序，用于临时阻止屏幕自动熄灭/锁定屏幕/休眠\n作者：燕戏竹林 (YXZL985)"));
    
    // 加载翻译器
    a.loadTranslator();
    
    // 设置应用显示名称（中文）
    a.setApplicationDisplayName(QCoreApplication::translate("Main", "秉烛"));
    
    // 创建主窗口
    MainWindow w;
    
    // 显示主窗口
    w.show();
    
    // 将窗口移动到屏幕中央
    Dtk::Widget::moveToCenter(&w);
    
    // 进入应用程序事件循环
    return a.exec();
}
