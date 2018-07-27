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
#include <QFormBuilder>
#include <QtNetwork>
#include "device.h"

class AbstractTest : public QFrame
{
    Q_OBJECT

signals:

public:
    explicit AbstractTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL, QString fName = "default");
    virtual ~AbstractTest();

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
    virtual void save(bool) = 0;
    void createCopy(bool);
    virtual void startTest(bool) = 0;

private:
    QMenu menu;
    QHBoxLayout *layout;
};

class MemTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit MemTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL, QString fName = "default");
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
};

class RegTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit RegTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL, QString fName = "default");
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
};

class EchoTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit EchoTest(QWidget *parent = 0, QDialog *d = NULL, QVBoxLayout *b = NULL, QString fName = "default");
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
};

namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices);
AbstractTest *loadTest(QString file, QVBoxLayout* devices);
}
#endif // ABSTRACTTEST_H
