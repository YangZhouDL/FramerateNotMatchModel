#ifndef PRODUCER_H
#define PRODUCER_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

class Producer : public QObject
{
    Q_OBJECT
public:
    explicit Producer(int maxSize = 10, QObject *parent = nullptr);
    void startProducing(int intervalMs = 1000); // 开始生产，默认间隔1秒
    void stopProducing();                       // 停止生产
    void setMaxSize(int maxSize);               // 设置队列最大长度
    bool getData(qint32& data);                // 获取队列中的数据

private slots:
    void produce(); // 定时生产数据

private:
    QQueue<int> m_queue;  // 内部队列
    QTimer m_timer;       // 定时器
    int m_maxSize;        // 队列最大长度
    qint32 m_dataID;     // 模拟数据的ID
    QMutex m_lck;
};

#endif // PRODUCER_H
