#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    connection(new Connection(this)),
    info(new CertInfo)
{
    ui->setupUi(this);

    connect(connection, &Connection::connected, this, [=]() {
        ui->connectButton->setText("disconnect");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        addInfo("<connection established>");
    });
    connect(connection, &Connection::disconnected, this, [=]() {
        ui->connectButton->setText("connect");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        addError("<connection closed>");
    });
    connect(connection, &Connection::info, this, &MainWindow::addInfo);
    connect(connection, &Connection::error, this, &MainWindow::addError);
    connect(connection, &Connection::data, this, &MainWindow::addDataReceived);
    connect(connection, &Connection::dataSent, this, &MainWindow::addDataSent);

    restoreSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete info;
}

void MainWindow::on_connectButton_clicked()
{
    if(ui->connectButton->text() == "disconnect") {
        connection->reset();
    } else {
        connection->connectToHost(ui->hostEdit->text(), ui->portEdit->text());
    }
}

void MainWindow::addInfo(QString info)
{
    QListWidgetItem *item = new QListWidgetItem(info);
    item->setForeground(Qt::black);
    ui->messagesLog->addItem(item);
    maybeScroll();
}

void MainWindow::addError(QString error)
{
    QListWidgetItem *item = new QListWidgetItem(error);
    item->setForeground(Qt::darkRed);
    ui->messagesLog->addItem(item);
    maybeScroll();
}

void MainWindow::addDataReceived(QByteArray data)
{
    QListWidgetItem *item = new QListWidgetItem(QString::fromUtf8(data));
    item->setForeground(Qt::darkBlue);
    ui->messagesLog->addItem(item);
    maybeScroll();
}

void MainWindow::addDataSent(QByteArray data, long bytes)
{
    QString text = QString("%1 <%2 bytes>").arg(QString::fromUtf8(data), QString::number(bytes));
    QListWidgetItem *item = new QListWidgetItem(text);
    item->setForeground(Qt::darkGreen);
    ui->messagesLog->addItem(item);
    maybeScroll();
}

void MainWindow::send(QString what, bool hex)
{
    connection->send(what, hex);
}

void MainWindow::on_ignoreErrors_stateChanged(int arg1)
{
    bool ignore = arg1 == Qt::Checked;
    connection->ignoreErrors = ignore;
}

void MainWindow::on_pushButton_clicked()
{
    QString what = ui->lineEdit->text();
    bool hex = ui->checkBox->isChecked();
    send(what, hex);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString what = ui->lineEdit_2->text();
    bool hex = ui->checkBox_2->isChecked();
    send(what, hex);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString what = ui->lineEdit_3->text();
    bool hex = ui->checkBox_3->isChecked();
    send(what, hex);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // save settings
    QSettings settings("Benesoft", "Tlstest");
    settings.setValue("width", width());
    settings.setValue("height", height());
    settings.setValue("host", ui->hostEdit->text());
    settings.setValue("port", ui->portEdit->text());
    settings.setValue("ignoreErrors", ui->ignoreErrors->isChecked());
    settings.setValue("scroll", autoScroll);
    settings.setValue("data1", ui->lineEdit->text());
    settings.setValue("data2", ui->lineEdit_2->text());
    settings.setValue("data3", ui->lineEdit_3->text());
    settings.setValue("hex1", ui->checkBox->isChecked());
    settings.setValue("hex2", ui->checkBox_2->isChecked());
    settings.setValue("hex3", ui->checkBox_3->isChecked());

    event->accept();
}

void MainWindow::restoreSettings()
{
    QSettings settings("Benesoft", "Tlstest");
    resize(settings.value("width", width()).toInt(), settings.value("height", height()).toInt());
    ui->hostEdit->setText(settings.value("host", "localhost").toString());
    ui->portEdit->setText(settings.value("port").toString());
    ui->ignoreErrors->setChecked(settings.value("ignoreErrors").toBool());

    bool scroll = settings.value("scroll", true).toBool();
    ui->actionScroll_with_content->setChecked(scroll);
    autoScroll = scroll;

    ui->lineEdit->setText(settings.value("data1").toString());
    ui->checkBox->setChecked(settings.value("hex1").toBool());
    ui->lineEdit_2->setText(settings.value("data2").toString());
    ui->checkBox_2->setChecked(settings.value("hex2").toBool());
    ui->lineEdit_3->setText(settings.value("data3").toString());
    ui->checkBox_3->setChecked(settings.value("hex3").toBool());
}

void MainWindow::on_actionScroll_with_content_triggered(bool checked)
{
    autoScroll = checked;
    ui->messagesLog->scrollToBottom();
}

void MainWindow::on_actionClear_Log_triggered()
{
    ui->messagesLog->clear();
}

void MainWindow::maybeScroll()
{
    if(autoScroll) {
        ui->messagesLog->scrollToBottom();
    }
}

void MainWindow::on_actionPeer_Certificate_Info_triggered()
{
    auto chain = connection->peerInfo();
    info->setContent(chain);
    info->show();
}
