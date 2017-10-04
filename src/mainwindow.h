#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include "certinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void addInfo(QString info);
    void addError(QString error);
    void addDataReceived(QByteArray data);
    void addDataSent(QByteArray data, long bytes);
    void send(QString what, bool hex);

    void on_ignoreErrors_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionScroll_with_content_triggered(bool checked);

    void on_actionClear_Log_triggered();

    void maybeScroll();
    void on_actionPeer_Certificate_Info_triggered();

private:
    Ui::MainWindow *ui;
    Connection *connection;
    CertInfo *info;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void restoreSettings();

    bool autoScroll = true;
};

#endif // MAINWINDOW_H
