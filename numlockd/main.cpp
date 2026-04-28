#include <QCoreApplication>
#include <QTimer>
#include <QCommandLineParser>
#include <csignal>

#include <DLog>

#include "numlock_simulator.h"
#include "ipc_server.h"

DCORE_USE_NAMESPACE

static QCoreApplication *g_app = nullptr;

void signalHandler(int sig)
{
    if (g_app) {
        dInfo() << "Received signal" << sig << ", shutting down gracefully...";
        g_app->quit();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    g_app = &app;

    app.setApplicationName("numlockd");
    app.setApplicationVersion("1.0.0");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    QCommandLineParser parser;
    parser.setApplicationDescription("Numlock daemon - periodically simulates NumLock key presses");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption intervalOption(QStringList() << "i" << "interval",
                                      "Simulation interval in minutes (default: 15)",
                                      "minutes",
                                      "15");
    parser.addOption(intervalOption);

    parser.process(app);

    int interval = parser.value(intervalOption).toInt();
    if (interval < 2) {
        dWarning() << "Interval too small, setting to minimum 2 minutes";
        interval = 2;
    }
    dInfo() << "Starting numlockd with interval:" << interval << "minutes";

    // 先启动 IPC 服务器，这样客户端可以连接
    IpcServer ipcServer;
    if (!ipcServer.start()) {
        dWarning() << "Failed to start IPC server, continuing without IPC functionality";
    } else {
        dInfo() << "IPC server started successfully";
    }

    NumlockSimulator simulator;
    if (!simulator.init()) {
        dError() << "Failed to initialize NumlockSimulator, continuing without simulation functionality";
        // 不返回错误，让 IPC 服务器继续运行以便客户端可以连接
    } else {
        dInfo() << "NumlockSimulator initialized successfully";
    }

    QTimer timer;
    int actualInterval = (interval - 1) * 60 * 1000;
    if (actualInterval < 60 * 1000) {
        actualInterval = 60 * 1000;
    }
    timer.setInterval(actualInterval);
    dInfo() << "Timer interval set to" << (actualInterval / 60000) << "minutes";

    QObject::connect(&ipcServer, &IpcServer::configReceived, [&timer, &simulator](int newInterval) {
        dInfo() << "Received new interval from IPC:" << newInterval << "minutes";
        if (newInterval < 2) {
            dWarning() << "Received interval too small, setting to minimum 2 minutes";
            newInterval = 2;
        }
        int newActualInterval = (newInterval - 1) * 60 * 1000;
        if (newActualInterval < 60 * 1000) {
            newActualInterval = 60 * 1000;
        }
        timer.setInterval(newActualInterval);
        dInfo() << "Timer interval updated to" << (newActualInterval / 60000) << "minutes";
    });

    QObject::connect(&timer, &QTimer::timeout, [&simulator]() {
        dInfo() << "Timer triggered, simulating NumLock press";
        simulator.simulateNumLock();
    });

    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);

    timer.start();
    dInfo() << "Numlock daemon started successfully";

    return app.exec();
}
