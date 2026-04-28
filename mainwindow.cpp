#include "mainwindow.h"

#include <DSuggestButton>
#include <DWarningButton>
#include <DWidgetUtil>
#include <DTitlebar>

#include <QDateTime>
#include <QIcon>

DWIDGET_USE_NAMESPACE

/**
 * @brief 构造函数
 */
MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent)
    , m_isServerRunning(false)
{
    // 设置窗口属性
    setMinimumSize(QSize(900, 500));
    resize(900, 500);
    
    // 设置标题栏
    titlebar()->setIcon(QIcon(":/images/logo.svg"));
    titlebar()->setTitle(tr("秉烛"));
    
    // 初始化 UI
    initUI();
    
    // 连接信号与槽
    setupConnections();
    
    // 初始化状态
    m_statusValueLabel->setText(tr("已停止"));
    m_statusValueLabel->setStyleSheet("color: #999;");
    
    // 添加启动日志
    appendLog(tr("应用程序已启动"));
}

/**
 * @brief 析构函数
 */
MainWindow::~MainWindow()
{
}

/**
 * @brief 初始化用户界面
 */
void MainWindow::initUI()
{
    // 创建中央窗体
    m_centralWidget = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    m_mainLayout->setSpacing(15);
    
    // ==================== 服务控制区域 ====================
    m_controlFrame = new DFrame(this);
    m_controlLayout = new QHBoxLayout(m_controlFrame);
    m_controlLayout->setContentsMargins(15, 15, 15, 15);
    m_controlLayout->setSpacing(15);
    
    // 启动服务按钮 - 使用 DSuggestButton 样式（蓝色强调）
    m_startServerBtn = new DPushButton(tr("启动服务"), this);
    m_startServerBtn->setIcon(QIcon::fromTheme("media-playback-start"));
    m_startServerBtn->setFixedSize(140, 40);
    m_startServerBtn->setProperty("type", "suggest");  // 设置建议按钮样式
    
    // 停止服务按钮 - 使用 DWarningButton 样式（红色警示）
    m_stopServerBtn = new DPushButton(tr("停止服务"), this);
    m_stopServerBtn->setIcon(QIcon::fromTheme("media-playback-stop"));
    m_stopServerBtn->setFixedSize(140, 40);
    m_stopServerBtn->setProperty("type", "warning");  // 设置警告按钮样式
    m_stopServerBtn->setEnabled(false);  // 初始状态下禁用停止按钮
    
    m_controlLayout->addWidget(m_startServerBtn);
    m_controlLayout->addWidget(m_stopServerBtn);
    m_controlLayout->addStretch();
    
    // ==================== 配置区域 ====================
    m_configFrame = new DFrame(this);
    m_configLayout = new QHBoxLayout(m_configFrame);
    m_configLayout->setContentsMargins(15, 15, 15, 15);
    m_configLayout->setSpacing(15);
    
    // 配置标签
    m_configLabel = new DLabel(tr("时间:"), this);
    
    // 数值输入框
    m_valueSpinBox = new DSpinBox(this);
    m_valueSpinBox->setRange(1, 9999);
    m_valueSpinBox->setValue(15);
    m_valueSpinBox->setSuffix(tr(" 分钟"));
    m_valueSpinBox->setFixedWidth(180);
    
    // 应用配置按钮
    m_applyBtn = new DPushButton(tr("应用配置"), this);
    m_applyBtn->setIcon(QIcon::fromTheme("document-save"));
    m_applyBtn->setFixedSize(120, 36);
    
    m_configLayout->addWidget(m_configLabel);
    m_configLayout->addWidget(m_valueSpinBox);
    m_configLayout->addWidget(m_applyBtn);
    m_configLayout->addStretch();
    
    // ==================== 底部区域（状态栏 + 日志）====================
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(15);
    
    // 状态区域
    m_statusFrame = new DFrame(this);
    m_statusLayout = new QHBoxLayout(m_statusFrame);
    m_statusLayout->setContentsMargins(15, 15, 15, 15);
    m_statusLayout->setSpacing(10);
    
    m_statusTitleLabel = new DLabel(tr("服务状态:"), this);
    m_statusValueLabel = new DLabel(this);
    m_statusValueLabel->setWordWrap(true);
    
    m_statusLayout->addWidget(m_statusTitleLabel);
    m_statusLayout->addWidget(m_statusValueLabel);
    m_statusLayout->addStretch();
    
    // 日志区域
    m_logFrame = new DFrame(this);
    m_logLayout = new QVBoxLayout(m_logFrame);
    m_logLayout->setContentsMargins(15, 15, 15, 15);
    m_logLayout->setSpacing(10);
    
    m_logTitleLabel = new DLabel(tr("运行日志"), this);
    m_logTitleLabel->setStyleSheet("font-weight: bold;");
    
    m_logTextEdit = new DTextEdit(this);
    m_logTextEdit->setReadOnly(true);
    m_logTextEdit->setPlaceholderText(tr("日志将显示在这里..."));
    
    m_logLayout->addWidget(m_logTitleLabel);
    m_logLayout->addWidget(m_logTextEdit);
    
    // 将状态栏和日志窗格添加到底部布局
    bottomLayout->addWidget(m_statusFrame, 1);  // 状态栏占 1 份
    bottomLayout->addWidget(m_logFrame, 2);     // 日志窗格占 2 份
    
    // ==================== 组装主布局 ====================
    m_mainLayout->addWidget(m_controlFrame);
    m_mainLayout->addWidget(m_configFrame);
    m_mainLayout->addLayout(bottomLayout, 1);  // 底部区域可伸展
    
    // 设置中央窗体
    setCentralWidget(m_centralWidget);
}

/**
 * @brief 连接信号与槽
 */
void MainWindow::setupConnections()
{
    // 启动服务按钮
    connect(m_startServerBtn, &DPushButton::clicked, this, &MainWindow::onStartServer);
    
    // 停止服务按钮
    connect(m_stopServerBtn, &DPushButton::clicked, this, &MainWindow::onStopServer);
    
    // 应用配置按钮
    connect(m_applyBtn, &DPushButton::clicked, this, &MainWindow::onApplyConfig);
}

/**
 * @brief 启动服务按钮槽函数
 */
void MainWindow::onStartServer()
{
    if (m_isServerRunning) {
        appendLog(tr("服务已经在运行中"));
        return;
    }
    
    m_isServerRunning = true;
    
    // 更新状态显示
    m_statusValueLabel->setText(tr("运行中 (时间: %1 分钟)").arg(m_valueSpinBox->value()));
    m_statusValueLabel->setStyleSheet("color: #2ecc71; font-weight: bold;");
    
    // 更新按钮状态
    m_startServerBtn->setEnabled(false);
    m_stopServerBtn->setEnabled(true);
    
    // 记录日志
    appendLog(tr("服务已启动，设定时间: %1 分钟").arg(m_valueSpinBox->value()));
}

/**
 * @brief 停止服务按钮槽函数
 */
void MainWindow::onStopServer()
{
    if (!m_isServerRunning) {
        appendLog(tr("服务已经处于停止状态"));
        return;
    }
    
    m_isServerRunning = false;
    
    // 更新状态显示
    m_statusValueLabel->setText(tr("已停止"));
    m_statusValueLabel->setStyleSheet("color: #999;");
    
    // 更新按钮状态
    m_startServerBtn->setEnabled(true);
    m_stopServerBtn->setEnabled(false);
    
    // 记录日志
    appendLog(tr("服务已停止"));
}

/**
 * @brief 应用配置按钮槽函数
 */
void MainWindow::onApplyConfig()
{
    int timeValue = m_valueSpinBox->value();
    
    // 如果服务正在运行，提示需要重启
    if (m_isServerRunning) {
        appendLog(tr("配置已更新为时间 %1 分钟，将在服务重启后生效").arg(timeValue));
        m_statusValueLabel->setText(tr("运行中 (时间: %1 分钟，配置待重启)").arg(timeValue));
    } else {
        appendLog(tr("配置已更新为时间 %1 分钟").arg(timeValue));
    }
}

/**
 * @brief 添加日志信息
 */
void MainWindow::appendLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logEntry = QString("[%1] %2").arg(timestamp, message);
    
    m_logTextEdit->append(logEntry);
}
