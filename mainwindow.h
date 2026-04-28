#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DPushButton>
#include <DSpinBox>
#include <DLineEdit>
#include <DLabel>
#include <DTextEdit>
#include <DFrame>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

/**
 * @brief 服务器控制面板主窗口类
 * 
 * 提供启动/停止服务、配置参数调整和日志查看等功能
 */
class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父窗口指针
     */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~MainWindow();

private slots:
    /**
     * @brief 启动服务按钮槽函数
     */
    void onStartServer();
    
    /**
     * @brief 停止服务按钮槽函数
     */
    void onStopServer();
    
    /**
     * @brief 应用配置按钮槽函数
     */
    void onApplyConfig();

private:
    /**
     * @brief 初始化用户界面
     */
    void initUI();
    
    /**
     * @brief 连接信号与槽
     */
    void setupConnections();
    
    /**
     * @brief 添加日志信息
     * @param message 日志消息
     */
    void appendLog(const QString &message);

    // 中央控件
    QWidget *m_centralWidget;           // 中央窗体
    QVBoxLayout *m_mainLayout;          // 主布局
    
    // 服务控制区域
    DFrame *m_controlFrame;             // 控制区域框架
    QHBoxLayout *m_controlLayout;       // 控制区域布局
    DPushButton *m_startServerBtn;      // 启动服务按钮
    DPushButton *m_stopServerBtn;       // 停止服务按钮
    
    // 配置区域
    DFrame *m_configFrame;              // 配置区域框架
    QHBoxLayout *m_configLayout;        // 配置区域布局
    DLabel *m_configLabel;              // 配置标签
    DSpinBox *m_valueSpinBox;           // 数值输入框
    DPushButton *m_applyBtn;            // 应用配置按钮
    
    // 状态区域
    DFrame *m_statusFrame;              // 状态区域框架
    QHBoxLayout *m_statusLayout;        // 状态区域布局
    DLabel *m_statusTitleLabel;         // 状态标题标签
    DLabel *m_statusValueLabel;         // 状态值标签
    
    // 日志区域
    DFrame *m_logFrame;                 // 日志区域框架
    QVBoxLayout *m_logLayout;           // 日志区域布局
    DLabel *m_logTitleLabel;            // 日志标题标签
    DTextEdit *m_logTextEdit;           // 日志文本编辑框
    
    // 服务状态
    bool m_isServerRunning;             // 服务运行状态标志
};

#endif // MAINWINDOW_H
