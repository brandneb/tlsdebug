#ifndef CERTINFO_H
#define CERTINFO_H

#include <QDialog>
#include <QSslCertificate>

namespace Ui {
class CertInfo;
}

class CertInfo : public QDialog
{
    Q_OBJECT

public:
    explicit CertInfo(QWidget *parent = 0);
    ~CertInfo();

    void setContent(QList<QSslCertificate> chain);

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::CertInfo *ui;
    QList<QSslCertificate> certs;
};

#endif // CERTINFO_H
