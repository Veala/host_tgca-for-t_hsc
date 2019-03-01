#ifndef VARBROADTEST_H
#define VARBROADTEST_H

#include "commontest.h"

class VarBroadTest : public CommonTest
{
    Q_OBJECT
public:
    explicit VarBroadTest(QWidget *parent = 0) : CommonTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
private slots:
    void onPushCodes();
protected:
    void startTest();
    void setEnabledSpecial(bool b);

private:
    QComboBox *comboBoxRec;             // проверка, что сообщение от КШ принято ОУ                 не и
    QComboBox *comboBoxMemRT;           // сравнение командного пакета с содержимым буфера приёма ОУ      не и

    // QCheckBox *checkBoxUseInt;          // определение завершения обмена по прерыванию КШ, если прерывание разрешено в cfg   не б
    // QLabel    *labelUseInt;             // метка должна блокироваться одновременно с checkBoxUseInt, если прерывание не разрешено
                                           // конфигурационным регистром         не б   поле определено в CommonTest
    QCheckBox *checkBoxParamView;       // вывод параметров теста в окно тестов или проекта    не и

    // параметры тестирования
    QComboBox *comboBoxBCIntErr;        // реакция на отсутствие признака завершения обмена КШ
    // QCheckBox *checkBoxOut;             // включение вывода в окно тестов    поле определено в CommonTest
    QCheckBox *checkBoxInit;            // загрузка начальной конфигурации
    QLineEdit  *lineEditCycle;          // число итераций

    // параметры конфигурационного регистра
    QComboBox *comboBoxManType;         // КШ и ОУ
    QCheckBox *checkBoxCodec;           // КШ и ОУ
    //QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ  поле определено в CommonTest

    // паузы  определены в классе CommonTest
    //QLineEdit *lineEditTime;            // максимальное время ожидания завершения цикла обмена
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

    QPushButton *pushButtonCodes;

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

class varBroadObjToThread : public commonObjToThread
{
    Q_OBJECT

    bool checkStatusRegBC(int status, int interr, int iter, bool *error);
    bool checkStatusRegRT(int status, int iter, bool *error);
    void setErrorsBeforeCycle(int numerr);
    void setErrorsWithinCycle(bool fatal);

    int buf_rx_bc;
    int buf_tx_rt;

public:
    varBroadObjToThread();

    quint32  waitTime, pauseTime, postponeTime;
    int iterCycle;
    bool useInt, initEnable;
    bool statusBCOut, statusRTOut;
    bool checkStatusErrBC, checkStatusErrRT, noIntFatalBC;
    bool compEnableMemRT;
    int perOutBC, modeOutBC, perOutRT, modeOutRT;
    void *testData;
    long dataSize;
    bool codec;
    void destroyData();

protected:
    void perform();
};

#endif // VARBROADTEST_H
