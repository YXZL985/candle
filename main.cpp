#include <DApplication>
#include <DMainWindow>
#include <DWidgetUtil>
#include <DTitlebar>

#include <QVBoxLayout>
#include <QWidget>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("candle");
    a.setApplicationVersion("1.0");
    a.setProductIcon(QIcon(":/images/logo.svg"));
    a.setProductName("Candle");
    a.setApplicationDescription("A simple DTK6 application.");

    a.loadTranslator();
    a.setApplicationDisplayName(QCoreApplication::translate("Main", "Candle"));

    DMainWindow w;
    w.titlebar()->setIcon(QIcon(":/images/logo.svg"));
    w.titlebar()->setTitle("Candle");
    w.setMinimumSize(QSize(600, 400));

    // 创建空窗体
    QWidget *cw = new QWidget(&w);
    QVBoxLayout *layout = new QVBoxLayout(cw);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    w.setCentralWidget(cw);
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return a.exec();
}
