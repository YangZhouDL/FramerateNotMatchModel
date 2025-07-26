// consumer.h
#ifndef CONSUMER_H
#define CONSUMER_H

#include <QObject>
#include <QTimer>
#include "producer.h"
#include <QElapsedTimer>

class Consumer : public QObject {
    Q_OBJECT
public:
    explicit Consumer(Producer* scanner, Producer* tracker, QObject *parent = nullptr);
    void startConsuming(int intervalMs = 1000);  // 默认1秒消费一次
    void stopConsuming();

protected:
    void groupData(const qint32 data, const bool isScanner = true);

private slots:
    void consume();

private:
    Producer* m_scanner;
    Producer* m_tracker;
    QTimer m_timer;
    QPair<qint32, qint32> m_group;
    QPair<bool, bool> m_groupFlag;
    qint32 m_standard;
    qint32 m_frameCnt;
    QElapsedTimer m_frameTimer;
};

#endif // CONSUMER_H
