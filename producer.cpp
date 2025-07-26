#include "producer.h"

Producer::Producer(int maxSize, QObject *parent)
    : QObject(parent)
    , m_maxSize(maxSize)
    , m_dataID(0)
{
    connect(&m_timer, &QTimer::timeout, this, &Producer::produce);
}

void Producer::startProducing(int intervalMs)
{
    m_dataID = 0;
    m_timer.start(intervalMs); // 启动定时器
    qInfo() << "Start produce";
}

void Producer::stopProducing()
{
    m_timer.stop(); // 停止定时器
    qInfo() << "Stop produce";
}

void Producer::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;
}

void Producer::produce()
{
    ++m_dataID;
    QMutexLocker lock(&m_lck);
    // 如果队列已满，移除队首元素
    if (m_queue.size() >= m_maxSize) {
        m_queue.dequeue();
    }

    // 添加新元素
    qInfo() << "append data:" << m_dataID;
    m_queue.enqueue(m_dataID);
    lock.unlock();
}

bool Producer::getData(qint32& data)
{
    bool ret = true;
    QMutexLocker lock(&m_lck);
    if(m_queue.isEmpty()){
        ret = false;
    }
    else
    {
        data = m_queue.head();
        m_queue.dequeue();
    }
    lock.unlock();
    return ret;
}
