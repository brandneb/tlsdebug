#include "certinfo.h"
#include "ui_certinfo.h"
#include <QDebug>

CertInfo::CertInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CertInfo)
{
    ui->setupUi(this);
}

CertInfo::~CertInfo()
{
    delete ui;
}

void CertInfo::setContent(QList<QSslCertificate> chain)
{
    certs = chain;

    ui->comboBox->clear();
    for(int i = 0; i < certs.length(); i++) {
        QString cns = certs[i].subjectInfo(QSslCertificate::CommonName).join(", ");
        ui->comboBox->addItem(QString("#%1 <%2>").arg(QString::number(i), cns));
    }

    if(certs.length() > 0) {
        ui->comboBox->setCurrentIndex(0);
        on_comboBox_currentIndexChanged(0);
    } else {
        ui->textBrowser->setPlainText("No certificates available");
    }
}

void CertInfo::on_comboBox_currentIndexChanged(int index)
{
    if(index >= 0 && index < certs.length()) {
        ui->textBrowser->setPlainText(certs[index].toText());
    }
}
