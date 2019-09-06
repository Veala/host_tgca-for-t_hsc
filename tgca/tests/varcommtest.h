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
private slots:
    void onPushCodes();
protected:
    void startTest();
    void setEnabledSpecial(bool b);

private:
    QComboBox *comboBoxCheckRW;         // проверка ответного пакета                           не и
    QComboBox *comboBoxRec;             // проверка, что сообщение от КШ принято ОУ                 не и
    QComboBox *comboBoxMemBC;           // сравнение буфера приема КШ с исходными данными    не и ???
    QComboBox *comboBoxMemRT;           // сравнение командного пакета с содержимым буфера приёма ОУ      не и
    QComboBox *comboBoxCheckRTA;        // проверка адреса ОУ  не и

    // QCheckBox *checkBoxUseInt;          // определение завершения обмена по прерыванию КШ, если прерывание разрешено в cfg   не б
    // QLabel    *labelUseInt;             // метка должна блокироваться одновременно с checkBoxUseInt, если прерывание не разрешено
                                           // конфигурационным регистром         не б   поле определено в CommonTest
    QCheckBox *checkBoxParamView;       // вывод параметров теста в окно тестов или проекта    не и

    QLineEdit * lineViewCodes;
    QPushButton *pushButtonCodes;

    // параметры тестирования
    QComboBox *comboBoxBCIntErr;        // реакция на отсутствие признака завершения обмена КШ
    QComboBox *comboBoxRTIntErr;        // реакция на отсутствие признака завершения обмена ОУ
    // QCheckBox *checkBoxOut;             // включение вывода в окно тестов   поле определено в CommonTest
    QCheckBox *checkBoxInit;            // загрузка начальной конфигурации
    QLineEdit  *lineEditCycle;          // число итераций

    // параметры конфигурационного регистра
    QComboBox *comboBoxManType;         // КШ и ОУ
    QCheckBox *checkBoxCodec;           // КШ и ОУ
    // QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ  поле определено в CommonTest
    // QCheckBox *checkBoxEnaAddr;         // ОУ  поле определено в CommonTest

    // адрес ОУ
    QCheckBox *checkBoxRTALoad;         // разрешение загрузки rta во вспомогательный регистр в случае выключенного checkBoxEnaAddr (бит rtavsk_ena=0)
    // QComboBox *comboBoxRTA;             // адрес ОУ  определено в CommonTest

    // паузы  определены в классе CommonTest
    //QLineEdit *lineEditTime;            // максимальное время ожидания завершения обмена
    //QLineEdit *lineEditPause;           // пауза между итерациями
    //QLineEdit *lineEditReservePause;    // задержка для оконного режима

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
    void disableUnimplemented();

private slots:
    void onCheckUseInt();
    void onCheckEnaInt();
    void applyCodes(QString);
};

class varCommandObjToThread : public commonObjToThread
{
    Q_OBJECT

    bool checkStatusRegBC(int status, int interr, int iter, bool *error);
    bool checkStatusRegRT(int status, int iter, bool *error);
    void setErrorsBeforeCycle(int numerr);
    void setErrorsWithinCycle(bool fatal);

public:
    varCommandObjToThread();

    int iterCycle;
    int rtaddr;
    bool loadRTA, initEnable;
    bool checkStatusErrBC, checkStatusErrRT, noIntFatalBC;
    int noIntFatalRT;
    bool compEnableMemBC, compEnableMemRT;
    int perOutBC, modeOutBC, perOutRT, modeOutRT;
    void *testData;
    long dataSize;
    bool codec;
    bool checkRTA;
    QString codeMask;
    void destroyData();
    bool checkReceiving;
    int checkResponse;

protected:
    void perform();
};

#endif // VARCOMMTEST_H
