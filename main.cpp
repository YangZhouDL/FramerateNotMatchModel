#include <QCoreApplication>
#include "producer.h"
#include "consumer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const int queueSize = 100;
    Producer scanner(queueSize);
    Producer tracker(queueSize);
    Consumer consumer(&scanner, &tracker);

    const int scannerRate = 40;
    const int trackerRate = 50;
    const int captureRate = 100;
    scanner.startProducing(1000 / scannerRate);
    tracker.startProducing(1000 / trackerRate);
    consumer.startConsuming(1000 / captureRate);

    return a.exec();
}
