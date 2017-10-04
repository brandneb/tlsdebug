#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QSSLSocket>

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);

signals:
    void data(QByteArray data);
    void dataSent(QByteArray data, long bytes);
    void connected();
    void disconnected();
    void error(QString what);
    void info(QString what);

public slots:
    void reset();
    void disconnectFromHost();
    void connectToHost(QString address, QString port);
    void send(QString what, bool isHex);

    QList<QSslCertificate> peerInfo();

public:
    bool ignoreErrors = false;
private:
    QSslSocket socket;
};

#endif // CONNECTION_H
