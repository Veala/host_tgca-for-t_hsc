#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QComboBox>
#include <QSpinBox>
#include <QtUiTools>
#include <QFormBuilder>
#include <QtNetwork>
#include "../device.h"

class AbstractTest : public QFrame
{
    Q_OBJECT

#define top_1       QFile settingsFile(fName); \
                    if (!settingsFile.open(QFile::ReadOnly)) \
                        return; \
                    QTextStream out(&settingsFile); \
                    out.readLine(); out.readLine();

#define top_2(str)          if (str.isEmpty()) \
                                return; \
                            QFile settingsFile(str); \
                            if(!settingsFile.open(QFile::WriteOnly)) \
                                return; \
                            QTextStream in(&settingsFile); \
                            in << name_enabled->text() << endl; \
                            in << name_enabled->isChecked() << endl;

signals:

public:
    explicit AbstractTest(QWidget *parent = 0);
    virtual ~AbstractTest();
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);

protected:
    void mousePressEvent(QMouseEvent *);
    bool pause, stop;
    QList<QLineEdit*> deviceLineEditList;
    QList<Device*> deviceList;
    QString saveFileNameStr;
    QCheckBox *name_enabled;
    QLabel *fileName;
    QLabel *status;
    QDialog *settings;
    QVBoxLayout *devices;
    QTextBrowser *textBrowser;
    void message(QString);
    void setConnections(Device*);
    void setDisconnections(Device*);

protected slots:
    void showSettings(bool);
    void deleteProc(bool);
    virtual void save(bool);
    virtual void startTest(bool) = 0;
    void newDev(QString);
    void checkDeviceAvailability(int);
    void deletingDevice(QString);
    void deletingDevice_part();
    void connectingSockDevice();
    void disconnectingSockDevice();
    void errorDevice(QAbstractSocket::SocketError);

private:
    QMenu menu;
    QHBoxLayout *layout;
};

class MemTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit MemTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
private:
    QComboBox *mode, *inversion, *oput;
    QLineEdit *startAddr, *endAddr, *startData, *deviceEdit;
    QSpinBox *incAddr, *incData, *iteration;
};

class RegTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit RegTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
};

class EchoTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit EchoTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QTextBrowser *tB);
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
private:
    QLineEdit *echo, *deviceEdit;
};

namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices, QTextBrowser *tB);
AbstractTest *loadTest(QString file, QVBoxLayout* devices, QTextBrowser *tB);
}
#endif // ABSTRACTTEST_H
