#ifndef IPC_SERVER_H
#define IPC_SERVER_H

#include <QObject>
#include <QString>
#include <QList>

QT_BEGIN_NAMESPACE
class QLocalServer;
class QLocalSocket;
QT_END_NAMESPACE

class IpcServer : public QObject
{
    Q_OBJECT

public:
    explicit IpcServer(const QString& socketPath = QStringLiteral("numlockd"), QObject* parent = nullptr);
    ~IpcServer() override;

    bool start();
    void stop();
    void sendLog(const QString& message);

signals:
    void configReceived(int intervalMinutes);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();

private:
    QLocalServer* m_server;
    QList<QLocalSocket*> m_clients;
    QString m_socketPath;
};

#endif // IPC_SERVER_H
