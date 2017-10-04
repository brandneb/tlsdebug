#include "connection.h"

#include <QHostAddress>
#include <QDebug>

Connection::Connection(QObject *parent) : QObject(parent)
{
    connect(&socket, &QSslSocket::connected, this, [=]() {
        info("<connected, performing ssl handshake...>");
    });
    connect(&socket, static_cast<void(QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors),
            [=](const QList<QSslError> &errors){
        QStringList errs;
        for(QSslError e : errors) {
            errs.append(e.errorString());
        }
        QString err = errs.join("; ");
        if(ignoreErrors) {
            info("Ssl errors: " + err);
            socket.ignoreSslErrors();
        } else {
            error("Aborting after ssl errors: " + err);
        }
    });
    connect(&socket, &QSslSocket::encrypted, this, [=]() {
        info("<connected, ssl handshake successful>");
        connected();
    });
    connect(&socket, &QSslSocket::readyRead, this, [=]() {
        QByteArray d = socket.readAll();
        data(d);
    });
    connect(&socket, &QSslSocket::stateChanged, this, [=](QSslSocket::SocketState state) {
        if(state == QAbstractSocket::UnconnectedState) {
            disconnected();
        }
        qDebug() << "state changed to" << state;
    });
}

void Connection::reset()
{
    if(socket.isOpen()) {
        info("RST connection");
    }
    socket.abort();
}

void Connection::disconnectFromHost()
{
    if(socket.isOpen()) {
        info("FIN connection");
    }
    socket.close();
}

void Connection::connectToHost(QString address, QString port)
{
    bool ok;
    int portInt = port.toInt(&ok);
    if(!ok) {
        error("Invalid port number");
        return;
    }

    socket.connectToHostEncrypted(address, portInt);
}

void Connection::send(QString what, bool isHex)
{
    if(!socket.isOpen()) {
        error("No connection");
        return;
    }

    QByteArray bytes;
    if(isHex) {
        bytes = QByteArray::fromHex(what.toLatin1());
    } else {
        bytes = what.toUtf8();
    }

    long bts = socket.write(bytes);
    if(bts == -1) {
        error("write failed: " + socket.errorString());
    } else {
        dataSent(bytes, bts);
    }
}

QList<QSslCertificate> Connection::peerInfo()
{
    return socket.peerCertificateChain();
}
