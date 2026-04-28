#ifndef NUMLOCK_SIMULATOR_H
#define NUMLOCK_SIMULATOR_H

#include <QObject>

class NumlockSimulator : public QObject
{
    Q_OBJECT

public:
    explicit NumlockSimulator(QObject *parent = nullptr);
    ~NumlockSimulator();

    bool init();
    void cleanup();
    bool simulateNumLock();
    bool isValid() const;

private:
    int m_uinputFd;
};

#endif // NUMLOCK_SIMULATOR_H
