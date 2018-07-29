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

#define top_1       QFile settingsFile(fName); \
                    if (!settingsFile.open(QFile::ReadOnly)) \
                        return; \
                    QTextStream out(&settingsFile); \
                    out.readLine(); out.readLine();

#define top_2(str)          if (str.isEmpty()) { \
    qDebug() << "empty file 2"; \
                                return; \
} \
    qDebug() << " no empty file"; \
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
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName);

protected:
    void mousePressEvent(QMouseEvent *);
    bool pause, stop;
    QString saveFileNameStr;
    QCheckBox *name_enabled;
    QLabel *fileName;
    QLabel *status;
    QDialog *settings;
    QVBoxLayout *devices;

protected slots:
    void showSettings(bool);
    void deleteProc(bool);
    virtual void save(bool);
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
    explicit MemTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName);
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
private:
    QComboBox *mode, *inversion, *oput, *device;
    QLineEdit *startAddr, *endAddr, *startData;
    QSpinBox *incAddr, *incData, *iteration;
};

class RegTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit RegTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName);
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
};

class EchoTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit EchoTest(QWidget *parent = 0) : AbstractTest(parent) { }
    virtual void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName);
protected slots:
    virtual void save(bool);
    virtual void startTest(bool);
private:
    QLineEdit *echo;
    QComboBox *device;
};

namespace testLib {
AbstractTest *createTest(QVBoxLayout* devices);
AbstractTest *loadTest(QString file, QVBoxLayout* devices);
}
#endif // ABSTRACTTEST_H
