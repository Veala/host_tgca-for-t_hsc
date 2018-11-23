#ifndef TRMSINGLETEST_H
#define TRMSINGLETEST_H

#include "abstracttest.h"
#include <QRadioButton>

class TrmSingleTest : public AbstractTest
{
    Q_OBJECT
public:
    explicit TrmSingleTest(QWidget *parent = 0) : AbstractTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    virtual void save();
protected:
    virtual void startTest();

private:
    QComboBox *comboBoxErrStatusBC;    // проверка флагов ошибок в регистре статуса КШ        не и
    QComboBox *comboBoxErrStatusRT;    // проверка флагов ошибок в регистре статуса ОУ        не и
    QComboBox *comboBoxCheckRW;        // проверка ответного пакета                           не и
    QComboBox *comboBoxRec;            // проверка, что сообщение от КШ принято ОУ                 не и
    QComboBox *comboBoxBCIntErr;       // реакция на отсутствие признака завершения обмена КШ      не и
    QComboBox *comboBoxRTIntErr;       // реакция на отсутствие признака завершения обмена ОУ      не и
    QComboBox *comboBoxMemBC;          // сравнение командного пакета с содержимым буфера передачи КШ    не и ???
    QComboBox *comboBoxMemRT;          // сравнение командного пакета с содержимым буфера приёма ОУ      не и
    QComboBox *comboBoxErrRegComp;     // проверка загрузки регистров
    QComboBox *comboBoxWrongCfgReg;    // проверка правильности конфигурационного регистра:
                                       // это важно в случае выключенной загрузки регистров   не и
    QCheckBox *checkBoxParamView;      // вывод параметров теста в окно тестов или проекта    не и

    QComboBox *comboBoxCompSPI;        // проверка бита завершения обмена в регистре управления обменом по SPI
    QComboBox *comboBoxCompData;       // сравнение полученных после передачи данных с исходными       не и

    QComboBox *comboBoxTestType;    // тип: приём/передача/цирк.возврат    не и-

    QLabel *labelSPIData;              // элемент секции загрузки по SPI      // не блокируется  // не меняет маску (по сигналу от labelSPIdr)
    QLabel *labelSPIdr;                // элемент секции загрузки по SPI      // не блокируется
    QLabel *labelSPIen;                // элемент секции загрузки по SPI      // не блокируется

    QLabel *labelBroad;    // должно блокироваться для теста приёма  не б

    ///// Эти все 6 пока не блокируются
    QLabel *labelCntOut;               // disable при выкл. checkBoxCounters (также comboBoxCountPref, lineEditCountOut, labelCounterSuf)
    QLabel *labelCounterSuf;
    QLabel *labelBCStatOut;            // disable при выкл. checkBoxBCOut (также comboBoxBCOutPref, lineEditBCOut, labelBCOutSuf)
    QLabel *labelBCOutSuf;
    QLabel *labelRTStatOut;            // disable при выкл. checkBoxRTOut (также comboBoxRTOutPref, lineEditRTOut, labelRTOutSuf)
    QLabel *labelRTOutSuf;





    QLineEdit *lineEditLen;             // длина передаваемых данных в байтах
    QComboBox *comboBoxRTA;             // адрес ОУ
    QComboBox *comboBoxCode;            // код команды
    QCheckBox *checkBoxBroad;           // групповая команда    не б

    // параметры конфигурационного регистра
    QComboBox *comboBoxManType;         // КШ и ОУ
    QCheckBox *checkBoxCodec;           // КШ и ОУ
    QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ          не и-
    QCheckBox *checkBoxEnaAddr;         // ОУ

    QComboBox *comboBoxAmplBC;          // коэффициент усиления КШ
    QComboBox *comboBoxAmplRT;          // коэффициент усиления ОУ

    // параметры генерации данных
    QRadioButton *radioButtonLin;       // выбор способа генерации: линейное со сбросом ("пила")
    QLineEdit *lineEditBegin;           // начальнон значение для генерации данных                           // не меняет маску (по сигналу от comboBoxUnit)
    QLineEdit *lineEditStep;            // приращение для линейных данных или минимум для случайных  // не меняет маску (по сигналу от comboBoxUnit)
    QLineEdit *lineEditNumStep;         // число шагов до сброса или максимум для случайных чисел  // не меняет маску (по сигналу от comboBoxUnit)
    QComboBox *comboBoxUnit;            // длина единицы данных в байтах: 1, 2 или 4

    // паузы
    QLineEdit *lineEditTime;            // максимальное время ожидания прерывания прерывания (завершения цикла обмена, в т.ч. обмена по SPI)
    QLineEdit *lineEditPause;           // пауза между итерациями
    QLineEdit *lineEditWinModePause;    // задержка для оконного режима

    QCheckBox *checkBoxUseInt;          // определение завершения обмена по прерыванию КШ, если прерывание разрешено в cfg   не б
    QLabel    *labelUseInt;             // метка блокируется одновременно с checkBoxUseInt, если прерывание не разрешено конфигурационным регистром        не б
    QCheckBox *checkBoxOut;             // включение вывода в окно тестов

    QLineEdit *lineEditDevBC;           // имя устройства, назначенного КШ
    QLineEdit *lineEditDevRT;           // имя устройства, назначенного ОУ
    QCheckBox *checkBoxDevBC;           // использовать КШ
    QCheckBox *checkBoxDevRT;           // использовать ОУ

    QLabel *labelLen;                   // меняется при изменении настроек cfg
    QLabel *labelBegin;                 // меняется при переключении radioButtonRand-radioButtonLin
    QLabel *labelStep;                  // меняется при переключении radioButtonRand-radioButtonLin
    QLabel *labelNumStep;               // меняется при переключении radioButtonRand-radioButtonLin
    QRadioButton *radioButtonRand;      // выбор генерации данных: случайные

    QCheckBox *checkBoxInit;            // загрузка начальной конфигурации
    QSpinBox  *spinBoxCycle;            // число итераций

    QRadioButton *radioButtonSingle;    // длина цикла 1
    QRadioButton *radioButtonUnlimited; // длина цикла не ограничена
    QRadioButton *radioButtonEnter;     // длина цикла задаётся параметром spinBoxCycle

    QLineEdit *lineEditSPIData;         // данные SPI
    QCheckBox *checkBoxConfRegLoad;     // загрузка в конфигурационный регистр настроек теста
    QCheckBox *checkBoxRTALoad;         // запись rta во вспомогательный регистр в случае выключенного checkBoxEnaAddr (бит rtavsk_ena=0)
    QComboBox *comboBoxSPILoad;         // устройство, для которого надо задать загрузку по SPI: пусто, КШ или ОУ
    QComboBox *comboBoxSPIdr;           // глубина данных по SPI
    QComboBox *comboBoxSPIen;           // номер устройства на шине SPI

    QCheckBox *checkBoxCounters;        // запрос регистров счётчиков  // не используется, не блокирует зависимые поля
    QCheckBox *checkBoxBCOut;           // запрос регистра статуса КШ  // не используется, не блокирует зависимые поля
    QCheckBox *checkBoxRTOut;           // запрос регистра статуса ОУ  // не используется, не блокирует зависимые поля
    QComboBox *comboBoxCountPref;       // выбор периодичности чтения и вывода счётчиков принятых и переданных пакетов //  не используется, не блокируется
    QComboBox *comboBoxBCOutPref;       // выбор периодичности вывода статуса КШ  //  не используется, не блокируется
    QComboBox *comboBoxRTOutPref;       // выбор периодичности вывода статуса ОУ  //  не используется, не блокируется
    QLineEdit *lineEditCountOut;        // частота чтения и вывода счётчиков принятых и переданных пакетов  //  не используется, не блокируется
    QLineEdit *lineEditBCOut;           // частота вывода статуса КШ        //  не используется, не блокируется
    QLineEdit *lineEditRTOut;           // частота вывода статуса ОУ        //  не используется, не блокируется

    QCheckBox *checkBoxWinMode;         // включение оконного режима

    unsigned int mnb;
    unsigned int maxNumByte();
    void recalc();
    void updateDeviceList();
    void setStatSettings();
    void load(QString fName);
    void defineFields();

private slots:
    void onRadioData();
    void onRadioCycle();
    void onCheckInit();
    void recalcMax();
    void updateSettings();

signals:
    void settingsClosed(int);
};

class trmSingleObjToThread : public absObjToThread
{
    Q_OBJECT
    //void checkStatusReg(Device *dev, int *status);
    int checkStatusRegBC();
    int checkStatusRegRT();
    void checkCounters(Device *dev);
    void stdOutput(QString message_rus, QString message_eng);
//    int readRegVal(Device* dev, int addr);
    void setErrorsBeforeCycle(int numerr);
    void setErrorsWithinCycle(bool fatal);
    bool checkSPI(Device* dev);

public slots:
    virtual void doWork();
    void terminate(int);
public:
    trmSingleObjToThread();

    quint32 /*cfgBC, cfgRT,*/ amplBC, amplRT, waitTime, pauseTime, delayTime, trm_size;
    int iterCycle;
    bool BCtoRT, RTtoBC;
    //bool trm_mode;
    addr_t rtaddr;
    bool broad, useInt, outEnable, initEnable, writeCfg, checkCountersEnable;
    bool statusBCOut, statusRTOut, windowMode;
    int checkFinSPI;
    int compEnableReg, compEnableMemBC, compEnableMemRT;
    int dataSPI;
    QString loadSPI;
    void *trmData;
    void *recData;
    void *testData;
    void destroyData();
    QTcpSocket tcpSocketBC, tcpSocketRT;
    Device *devBC, *devRT;
};

#endif // TRMSINGLETEST_H
