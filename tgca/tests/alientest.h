#ifndef ALIENTEST_H
#define ALIENTEST_H

#include "commontest.h"

class AlienTest : public CommonTest
{
    Q_OBJECT
public:
    explicit AlienTest(QWidget *parent = 0) : CommonTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    void startTest();
    void setEnabledSpecial(bool b);

private:
    QCheckBox *checkBoxParamView;       // вывод параметров теста в окно тестов или проекта    не и

    // параметры тестирования
    QComboBox *comboBoxBCIntErr;        // реакция на отсутствие признака завершения обмена КШ
    QComboBox *comboBoxTR;              // приём или передача
    QComboBox *comboBoxA1;              // приём или передача
    QComboBox *comboBoxA2;              // приём или передача
    QLineEdit  *lineEditAddLen;         // число итераций

    // параметры конфигурационного регистра
    QComboBox *comboBoxManType;         // КШ и ОУ
    QCheckBox *checkBoxCodec;           // КШ и ОУ
    // QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ  поле определено в CommonTest
    // QCheckBox *checkBoxEnaAddr;         // ОУ  поле определено в CommonTest

    // начальная загрузка и проверка регистров
    QCheckBox *checkBoxInit;            // загрузка начальной конфигурации
    QCheckBox *checkBoxRTALoad;         // разрешение загрузки rta во вспомогательный регистр в случае выключенного checkBoxEnaAddr (бит rtavsk_ena=0)
    QCheckBox *checkBoxLoadCfgReg;      // не и
    QCheckBox *checkBoxCheckCfgReg;     // не и
    QCheckBox *checkBoxCheckRTA;        // проверка адреса ОУ  не и

    // паузы  определены в классе CommonTest
    //QLineEdit *lineEditTime;            // максимальное время ожидания завершения обмена
    //QLineEdit *lineEditPause;           // пауза между итерациями
    //QLineEdit *lineEditReservePause;    // задержка для оконного режима

    // включение проверок и сравнений
    QComboBox *comboBoxErrStatusBC;     // проверка флагов ошибок в регистре статуса КШ
    QComboBox *comboBoxErrStatusRT;     // проверка флагов ошибок в регистре статуса ОУ

    // управление выводом регистров состояния
    QCheckBox *checkBoxBCOut;           // включение,выключение вывода номеров буферов приёма/передачи при выводе регистра статуса КШ
    QCheckBox *checkBoxRTOut;           // включение,выключение вывода номеров буферов приёма/передачи при выводе регистра статуса ОУ
    QComboBox *comboBoxBCOutPref;       // выбор периодичности вывода статуса КШ
    QComboBox *comboBoxRTOutPref;       // выбор периодичности вывода статуса ОУ
    QLineEdit *lineEditBCOut;           // частота вывода статуса КШ: если 0, то нет вывода
    QLineEdit *lineEditRTOut;           // частота вывода статуса ОУ: если 0, то нет вывода

    QLineEdit *lineEditCode;

    unsigned int mnb;

    void setStatSettings();
    void load(QString fName);
    void defineFields();
    void setFieldConnections();
    void disableUnimplemented();

private slots:
    void onCheckUseInt();
    void onCheckEnaInt();
};

class alienObjToThread : public commonObjToThread
{
    Q_OBJECT

    bool checkStatusRegBC(int status, int interr, int iter, bool *error);
    bool checkStatusRegRT(int status, int iter, bool *error);
    void setErrorsBeforeCycle(int numerr);
    void setErrorsWithinCycle(bool fatal);

public:
    alienObjToThread();

    int step;
    int rtaddr;
    int addr1, addr2;
    int code;
    bool loadRTA, initEnable;
    bool statusBCOut, statusRTOut;
    bool checkStatusErrBC, checkChangeStatusRT, noIntFatalBC;
    int perOutBC, modeOutBC, perOutRT, modeOutRT;
    void *testData;
    long dataSize;
    bool codec;
    bool checkRTA;
    int dir; // приём-передача
    void destroyData();

protected:
    void perform();
};

#endif // ALIENTEST_H
