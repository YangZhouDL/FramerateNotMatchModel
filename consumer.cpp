// consumer.cpp
#include "consumer.h"
#include <QDebug>

Consumer::Consumer(Producer* scanner, Producer* tracker, QObject *parent)
    : QObject(parent), m_scanner(scanner), m_tracker(tracker), m_standard(0) {
    connect(&m_timer, &QTimer::timeout, this, &Consumer::consume);
}

void Consumer::startConsuming(int intervalMs) {
    m_standard = 0;
    m_group = qMakePair(0, 0);
    m_groupFlag = qMakePair(false, false);
    m_timer.start(intervalMs);
    m_frameCnt = 0;
    m_frameTimer.start();
    qInfo() << "Start consume";
}

void Consumer::stopConsuming()
{
    m_timer.stop();
    qInfo() << "Stop consume";
}

void Consumer::consume() {
    if(!m_groupFlag.first)
    {
        // 从扫描仪拿数据
        qint32 data = 0;
        if(m_scanner->getData(data))
        {
            qInfo() << "scanner get data:" << data;
            groupData(data);
        }
    }

    if(!m_groupFlag.second)
    {
        // 从跟踪仪拿数据
        qint32 data = 0;
        if(m_tracker->getData(data))
        {
            qInfo() << "track get data:" << data;
            groupData(data, false);
        }
    }

    if(m_frameTimer.elapsed() > 1000)
    {
        float frameRate = static_cast<float>(m_frameCnt) / m_frameTimer.elapsed() * 1000;
        qInfo() << "FrameRate:" << frameRate;
        m_frameTimer.restart();
        m_frameCnt = 0;
    }
}

void Consumer::groupData(const qint32 data, const bool isScanner)
{
    auto setGroupFlag = [this, &isScanner]()
    {
        if(isScanner)
            m_groupFlag.first = true;
        else
            m_groupFlag.second = true;
    };

    auto addGroupData = [this, &isScanner, &data]()
    {
        if(isScanner)
            m_group.first = data;
        else
            m_group.second = data;
    };

    if(m_standard == 0)
    {
        m_standard = data;
        setGroupFlag();
        addGroupData();
    }
    else if(qAbs(data - m_standard) <= 3)
    {
        setGroupFlag();
        addGroupData();
    }
    else if(data - m_standard > 3)
    {
        m_standard = data;
        if(isScanner){
            m_groupFlag.first = true;
            m_groupFlag.second = false;
            m_group.first = data;
            m_group.second = 0;
        }
        else{
            m_groupFlag.first = false;
            m_groupFlag.second = true;
            m_group.first = 0;
            m_group.second = data;
        }
    }

    auto resetGroupData = [this]()
    {
        m_standard = 0;
        m_group.first = 0;
        m_group.second = 0;
        m_groupFlag.first = false;
        m_groupFlag.second = false;
    };

    if(m_groupFlag.first && m_groupFlag.second){
        ++m_frameCnt;
        qInfo() << QString("group data ok, standard %1, scanner %2, tracker %3").arg(m_standard).arg(m_group.first).arg(m_group.second);
        resetGroupData();
    }
    else{
        qCritical() << QString("group data failed, standard %1, scanner %2, tracker %3").arg(m_standard).arg(m_group.first).arg(m_group.second);
    }
}
