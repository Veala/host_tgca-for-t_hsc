#ifndef CONNECTIONFORM_H
#define CONNECTIONFORM_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class Connection;
}

class Connection : public QWidget
{
    Q_OBJECT

signals:
    void connectTry(bool);
    void disconnectTry(bool);

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();
    QString getServerIP() const;
    QString getServerPORT() const;
    QString getHostIP() const;
    void setName(QString);
    void doConnected();
    void doError(QString);
    void doMessage(QString);

private:
    Ui::Connection *ui;

};

#endif // CONNECTIONFORM_H
