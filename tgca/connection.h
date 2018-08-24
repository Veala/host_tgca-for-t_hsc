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
    void checkDevice(bool);

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();
    QString getServerIP() const;
    QString getServerPORT() const;
    QString getHostIP() const;
    QString getHostPORT() const;
    void setServerIP(QString);
    void setServerPORT(QString);
    void setHostIP(QString);
    void setHostPORT(QString);
    void setName(QString);

private:
    Ui::Connection *ui;

};

#endif // CONNECTIONFORM_H
