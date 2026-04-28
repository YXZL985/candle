#include "numlock_simulator.h"

#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <QDebug>
#include <QThread>

NumlockSimulator::NumlockSimulator(QObject *parent)
    : QObject(parent)
    , m_uinputFd(-1)
{
}

NumlockSimulator::~NumlockSimulator()
{
    cleanup();
}

bool NumlockSimulator::init()
{
    if (m_uinputFd >= 0) {
        qWarning() << "NumlockSimulator: already initialized";
        return false;
    }

    m_uinputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (m_uinputFd < 0) {
        qWarning() << "NumlockSimulator: failed to open /dev/uinput:" << strerror(errno);
        return false;
    }

    if (ioctl(m_uinputFd, UI_SET_EVBIT, EV_KEY) < 0) {
        qWarning() << "NumlockSimulator: failed to set EV_KEY:" << strerror(errno);
        cleanup();
        return false;
    }

    if (ioctl(m_uinputFd, UI_SET_KEYBIT, KEY_NUMLOCK) < 0) {
        qWarning() << "NumlockSimulator: failed to set KEY_NUMLOCK:" << strerror(errno);
        cleanup();
        return false;
    }

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234;
    usetup.id.product = 0x5678;
    strcpy(usetup.name, "Numlock Virtual Keyboard");

    if (ioctl(m_uinputFd, UI_DEV_SETUP, &usetup) < 0) {
        qWarning() << "NumlockSimulator: failed to setup device:" << strerror(errno);
        cleanup();
        return false;
    }

    if (ioctl(m_uinputFd, UI_DEV_CREATE) < 0) {
        qWarning() << "NumlockSimulator: failed to create device:" << strerror(errno);
        cleanup();
        return false;
    }

    qDebug() << "NumlockSimulator: virtual keyboard created successfully";
    return true;
}

void NumlockSimulator::cleanup()
{
    if (m_uinputFd >= 0) {
        ioctl(m_uinputFd, UI_DEV_DESTROY);
        close(m_uinputFd);
        qDebug() << "NumlockSimulator: virtual keyboard destroyed";
        m_uinputFd = -1;
    }
}

bool NumlockSimulator::simulateNumLock()
{
    if (m_uinputFd < 0) {
        qWarning() << "NumlockSimulator: device not initialized";
        return false;
    }

    struct input_event ev;
    memset(&ev, 0, sizeof(ev));

    auto sendEvent = [&](int type, int code, int value) -> bool {
        ev.type = type;
        ev.code = code;
        ev.value = value;
        ev.time.tv_sec = 0;
        ev.time.tv_usec = 0;
        if (write(m_uinputFd, &ev, sizeof(ev)) < 0) {
            qWarning() << "NumlockSimulator: failed to write event:" << strerror(errno);
            return false;
        }
        return true;
    };

    for (int i = 0; i < 2; ++i) {
        if (!sendEvent(EV_KEY, KEY_NUMLOCK, 1)) {
            return false;
        }
        if (!sendEvent(EV_SYN, SYN_REPORT, 0)) {
            return false;
        }

        if (!sendEvent(EV_KEY, KEY_NUMLOCK, 0)) {
            return false;
        }
        if (!sendEvent(EV_SYN, SYN_REPORT, 0)) {
            return false;
        }

        qDebug() << "NumlockSimulator: Num Lock click" << (i + 1) << "sent";

        if (i < 1) {
            QThread::msleep(50);
        }
    }

    return true;
}

bool NumlockSimulator::isValid() const
{
    return m_uinputFd >= 0;
}
