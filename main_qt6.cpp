#include <QApplication>
#include <QMainWindow>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QDate>
#include <QPropertyAnimation>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("candle");
    a.setApplicationVersion("1.0");
    a.setWindowIcon(QIcon(":/images/logo.svg"));

    QMainWindow w;
    w.setWindowTitle("Candle - Qt6 Version");
    w.setMinimumSize(QSize(600, 200));

    QWidget *cw = new QWidget(&w);
    QVBoxLayout *layout = new QVBoxLayout(cw);
    layout->setContentsMargins(20, 20, 20, 20);

    QDate today = QDate::currentDate();
    QProgressBar *yearProgressBar = new QProgressBar();
    yearProgressBar->setMaximum(today.daysInYear());
    yearProgressBar->setMinimum(0);
    yearProgressBar->setValue(0);
    yearProgressBar->setTextVisible(true);
    yearProgressBar->setAlignment(Qt::AlignCenter);

    // 设置样式
    yearProgressBar->setStyleSheet(R"(
        QProgressBar {
            border: 2px solid #2196F3;
            border-radius: 5px;
            text-align: center;
            height: 30px;
            font-size: 14px;
        }
        QProgressBar::chunk {
            background-color: #2196F3;
            border-radius: 3px;
        }
    )");

    QObject::connect(yearProgressBar, &QProgressBar::valueChanged, yearProgressBar, [yearProgressBar](int value){
        yearProgressBar->setFormat(QString("您的 %1 年使用进度：%2%")
                                   .arg(QDate::currentDate().year())
                                   .arg(value * 100 / yearProgressBar->maximum()));
    });

    layout->addWidget(yearProgressBar);
    w.setCentralWidget(cw);
    w.show();

    // 动画效果
    auto animation = new QPropertyAnimation(yearProgressBar, "value");
    animation->setDuration(5000);
    animation->setStartValue(0);
    animation->setEndValue(today.dayOfYear());
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start();

    return a.exec();
}
