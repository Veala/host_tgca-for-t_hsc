#ifndef VARCOMMTEST_H
#define VARCOMMTEST_H

#include "commontest.h"

class VarCommandTest : public CommonTest
{
    Q_OBJECT
public:
    explicit VarCommandTest(QWidget *parent = 0) : CommonTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    void startTest();
    void setEnabledSpecial(bool b);

private:
    QComboBox *comboBoxCheckRW;         // проверка ответного пакета                           не и
    QComboBox *comboBoxRec;             // проверка, что сообщение от КШ принято ОУ                 не и
    QComboBox *comboBoxMemBC;           // сравнение буфера приема КШ с исходными данными    не и ???
    QComboBox *comboBoxMemRT;           // сравнение командного пакета с содержимым буфера приёма ОУ      не и
    QComboBox *comboBoxWrongCfgReg;     // проверка правильности конфигурационного регистра  не и
    QComboBox *comboBoxCheckRTA;        // проверка адреса ОУ  не и

    QCheckBox *checkBoxUseInt;          // определение завершения обмена по прерыванию КШ, если прерывание разрешено в cfg   не б
    QLabel    *labelUseInt;             // метка блокируется одновременно с checkBoxUseInt, если прерывание не разрешено конфигурационным регистром        не б
    QCheckBox *checkBoxParamView;       // вывод параметров теста в окно тестов или проекта    не и

    // параметры тестирования
    QComboBox *comboBoxBCIntErr;        // реакция на отсутствие признака завершения обмена КШ
    QComboBox *comboBoxRTIntErr;        // реакция на отсутствие признака завершения обмена ОУ
    QCheckBox *checkBoxOut;             // включение вывода в окно тестов
    QCheckBox *checkBoxInit;            // загрузка начальной конфигурации
    QLineEdit  *lineEditCycle;          // число итераций

    // параметры конфигурационного регистра
    QComboBox *comboBoxManType;         // КШ и ОУ
    QCheckBox *checkBoxCodec;           // КШ и ОУ
    QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ
    QCheckBox *checkBoxEnaAddr;         // ОУ

    // адрес ОУ
    QCheckBox *checkBoxRTALoad;         // разрешение загрузки rta во вспомогательный регистр в случае выключенного checkBoxEnaAddr (бит rtavsk_ena=0)
    QComboBox *comboBoxRTA;             // адрес ОУ

    // паузы
    QLineEdit *lineEditTime;            // максимальное время ожидания прерывания (завершения цикла обмена, в т.ч. обмена по SPI)
    QLineEdit *lineEditPause;           // пауза между итерациями
    QLineEdit *lineEditWinModePause;    // задержка для оконного режима

    // устройства
    QLineEdit *lineEditDevBC;           // имя устройства, назначенного КШ
    QLineEdit *lineEditDevRT;           // имя устройства, назначенного ОУ

    // включение проверок и сравнений
    QComboBox *comboBoxErrStatusBC;     // проверка флагов ошибок в регистре статуса КШ
    QComboBox *comboBoxErrStatusRT;     // проверка флагов ошибок в регистре статуса ОУ

    // периодичность вывода регистра состояния
    QComboBox *comboBoxBCOutPref;       // выбор периодичности вывода статуса КШ
    QComboBox *comboBoxRTOutPref;       // выбор периодичности вывода статуса ОУ
    QLineEdit *lineEditBCOut;           // частота вывода статуса КШ: если 0, то нет вывода
    QLineEdit *lineEditRTOut;           // частота вывода статуса ОУ: если 0, то нет вывода

    unsigned int mnb;
    void setStatSettings();
    void load(QString fName);
    void defineFields();
    void setFieldConnections();
    void disableUnused();

private slots:
};

class varCommandObjToThread : public commonObjToThread
{
    Q_OBJECT

    bool checkStatusRegBC(int status, int interr, int iter, bool *error);
    bool checkStatusRegRT(int status, int iter, bool *error);
    void setErrorsBeforeCycle(int numerr);
    void setErrorsWithinCycle(bool fatal);
    void switchWindow(int n);

public:
    varCommandObjToThread();

    quint32  waitTime, pauseTime, delayTime;
    int iterCycle;
//    int nwrd;
//    bool trm_mode;
    int rtaddr;
    bool useInt, loadRTA, initEnable, checkLoadCfg;
    bool checkStatusErrBC, checkStatusErrRT, noIntFatalBC;
    int noIntFatalRT;
    bool compEnableMemBC, compEnableMemRT;
    int perOutBC, modeOutBC, perOutRT, modeOutRT;
    void *testData;
    long dataSize;
    QString manipulation;
    bool codec;
    bool checkRTA;
    void destroyData();

protected:
    void perform();
    void terminate(int);
};

#endif // VARCOMMTEST_H
