#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QInputDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QComboBox>
#include <QSpinBox>
#include <QtUiTools>
#include <QtNetwork>
#include "device.h"

class AbstractTest : public QFrame
{
    Q_OBJECT

signals:

public:
    explicit AbstractTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL);
    virtual ~AbstractTest();
    virtual void startTest() = 0;
    void setEnabled(bool en);

protected:
    void mousePressEvent(QMouseEvent *);
    bool pause, stop;
    QCheckBox *name_enabled;
    QLabel *fileName;
    QLabel *status;
    QDialog *settings;
    QVBoxLayout *devices;

protected slots:
    void showSettings(bool);
    void deleteProc(bool);
    void save(bool);

private:
    QMenu menu;
    QHBoxLayout *layout;
};

class MemTest : public AbstractTest
{
    Q_OBJECT

public:
    explicit MemTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL);
    virtual void startTest();

private:
    QComboBox modeBox;
    QLineEdit startAddr;
    QLineEdit endAddr;
    QSpinBox addrInc;
    QLineEdit startData;
    QSpinBox dataInc;
    QComboBox inverse;
    QSpinBox iteration;
    QComboBox output;
    QLineEdit deviceName;
};

class RegTest : public AbstractTest
{
    Q_OBJECT

public:
    explicit RegTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL);
    virtual void startTest();
};


namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices);
AbstractTest *loadTest(QString file, QVBoxLayout* devices);
}
#endif // ABSTRACTTEST_H
