#ifndef TRMSINGLETEST_H
#define TRMSINGLETEST_H

#include "commontest.h"

class TrmSingleTest : public CommonTest
{
    Q_OBJECT
public:
    explicit TrmSingleTest(QWidget *parent = 0) : CommonTest(parent) { }
    void setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2);
protected slots:
    void save();
protected:
    void startTest();
    void setEnabledSpecial(bool b);

private:
    QComboBox *comboBoxCheckRW;         // проверка ответного пакета                                не и
    QComboBox *comboBoxRec;             // проверка, что сообщение от КШ принято ОУ                 не и
    QComboBox *comboBoxWrongCfgReg;     // проверка правильности конфигурационного регистра:
                                        // это важно в случае выключенной загрузки регистров   не и
    QCheckBox *checkBoxParamView;       // вывод параметров теста в окно тестов или проекта    не и


    QCheckBox *checkBoxCounters;        // запрос регистров счётчиков  // не блокирует зависимые поля
    QComboBox *comboBoxCountPref;       // выбор периодичности вывода счётчиков принятых и переданных пакетов //  не используется
    QLineEdit *lineEditCountOut;        // частота вывода счётчиков принятых и переданных пакетов  //  не используется



    QLabel *labelSPIData;               // элемент секции загрузки по SPI      // не блокируется  // не меняет маску (по сигналу от labelSPIdr)
    QLabel *labelSPIdr;                 // элемент секции загрузки по SPI      // не блокируется
    QLabel *labelSPIen;                 // элемент секции загрузки по SPI      // не блокируется


    QCheckBox *checkBoxUseInt;          // определение завершения обмена по прерыванию КШ, если прерывание разрешено в cfg   не б
    QLabel    *labelUseInt;             // метка блокируется одновременно с checkBoxUseInt, если прерывание не разрешено конфигурационным регистром        не б



    // ПАРАМЕТРЫ ТЕСТИРОВАНИЯ

    QComboBox *comboBoxTestType;        // тип: приём/передача/циркулярный возврат
    QComboBox *comboBoxSpeed;           // включение измерения скорости передачи данных
    QCheckBox *checkBoxWinMode;         // включение оконного режима
    QComboBox *comboBoxBCIntErr;        // реакция на отсутствие признака завершения обмена КШ
    QComboBox *comboBoxRTIntErr;        // реакция на отсутствие признака завершения обмена ОУ
    QCheckBox *checkBoxOut;             // включение вывода в окно тестов
    QLineEdit *lineEditOver;            // значение, на которое надо уменьшить измереное время передачи данных: расходы на ethernet
    // зацикливание
    QSpinBox  *spinBoxCycle;            // число итераций
    QRadioButton *radioButtonSingle;    // длина цикла 1
    QRadioButton *radioButtonUnlimited; // длина цикла не ограничена
    QRadioButton *radioButtonEnter;     // длина цикла задаётся параметром spinBoxCycle
    // паузы
    QLineEdit *lineEditTime;            // максимальное время ожидания прерывания (завершения цикла обмена, в т.ч. обмена по SPI)
    QLineEdit *lineEditPause;           // пауза между итерациями
    QLineEdit *lineEditReservePause;    // задержка для оконного режима

    // настройки командного слова
    QLineEdit *lineEditLen;             // длина передаваемых данных в байтах
    QComboBox *comboBoxRTA;             // адрес ОУ
    QComboBox *comboBoxCode;            // код команды
    QCheckBox *checkBoxBroad;           // групповая команда

    // параметры конфигурационного регистра
    QComboBox *comboBoxManType;         // КШ и ОУ
    QCheckBox *checkBoxCodec;           // КШ и ОУ
    QCheckBox *checkBoxEnaInt;          // КШ и ОУ, проверяется пока только КШ
    QCheckBox *checkBoxEnaAddr;         // ОУ

    // управление загрузкой регистров
    QCheckBox *checkBoxInit;            // загрузка начальной конфигурации
    QComboBox *comboBoxAmplBC;          // коэффициент усиления КШ
    QComboBox *comboBoxAmplRT;          // коэффициент усиления ОУ
    QCheckBox *checkBoxConfRegLoad;     // загрузка в конфигурационный регистр настроек теста
    QCheckBox *checkBoxRTALoad;         // запись rta во вспомогательный регистр в случае выключенного checkBoxEnaAddr (бит rtavsk_ena=0)
    QComboBox *comboBoxSPILoad;         // устройство, для которого надо задать загрузку по SPI: пусто, КШ или ОУ
    QComboBox *comboBoxSPIdr;           // глубина данных по SPI
    QComboBox *comboBoxSPIen;           // номер устройства на шине SPI
    QLineEdit *lineEditSPIData;         // данные SPI

    // включение проверок и сравнений
    QComboBox *comboBoxErrStatusBC;     // проверка флагов ошибок в регистре статуса КШ
    QComboBox *comboBoxErrStatusRT;     // проверка флагов ошибок в регистре статуса ОУ
    QComboBox *comboBoxErrRegComp;      // проверка загрузки регистров
    QComboBox *comboBoxCompSPI;         // проверка бита завершения обмена в регистре управления обменом по SPI
    QComboBox *comboBoxCompData;        // сравнение полученных после передачи данных с исходными
    QComboBox *comboBoxMemBCRT;         // проверка записи исходных данных в буфер передачи КШ или ОУ (в зависимости от направления передачи)
    QComboBox *comboBoxCheckRTA;        // проверка адреса ОУ

    // управление выводом регистров
    QCheckBox *checkBoxBCOut;           // включение,выключение вывода номеров буферов приёма/передачи при выводе регистра статуса КШ
    QCheckBox *checkBoxRTOut;           // включение,выключение вывода номеров буферов приёма/передачи при выводе регистра статуса ОУ
    QComboBox *comboBoxBCOutPref;       // выбор периодичности вывода статуса КШ
    QComboBox *comboBoxRTOutPref;       // выбор периодичности вывода статуса ОУ
    QLineEdit *lineEditBCOut;           // частота вывода статуса КШ
    QLineEdit *lineEditRTOut;           // частота вывода статуса ОУ


    // поля, которые не используются в тесте, но зависят от других полей
    QLabel *labelSpeed;                 // метка измерения скорости передачи данных - не активна при циркулярном возврате
    QLabel *labelBroad;                 // метка групповой команды - только КШ->ОУ
    QLabel *labelLen;                   // меняется при изменении настроек cfg

    unsigned int mnb;
    unsigned int maxNumByte();
    void recalc();
    void setStatSettings();
    void load(QString fName);
    void defineFields();
    void setFieldConnections();
    void disableUnimplemented();

private slots:
    void onRadioCycle();
    void onCheckInit();
    void recalcMax();
    void onTypeChanged();
};

class trmSingleObjToThread : public commonObjToThread
{
    Q_OBJECT

    bool checkStatusRegBC(int status, int interr, int iter, bool *error);
    bool checkStatusRegRT(int status, int iter, bool *error);
    void checkCounters(Device *dev);
    void setErrorsBeforeCycle(int numerr);
    void setErrorsWithinCycle(bool fatal);
    bool checkSPI(Device* dev);
    //int  waitForInterruption(int *status);
    void averageSpeed();
    long wholePackBits;
    long totalTime;
    int timeCounter;
    QTime beginTime;
    void initTimeMeasure();

public:
    trmSingleObjToThread();

    quint32 amplBC, amplRT, waitTime, pauseTime, trm_size, postponeTime;
    int iterCycle;
    int nwrd;
    bool BCtoRT, RTtoBC;
    //bool trm_mode;
    int rtaddr;
    bool useInt, initEnable, writeCfg, checkCountersEnable;
    bool statusBCOut, statusRTOut, windowMode;
    bool checkStatusErrBC, checkStatusErrRT, noIntFatalBC, checkLoadCfg;
    int noIntFatalRT;
    int checkFinSPI;
    int compEnableReg, compEnableMemBCRT, compEnableRTA, compEnableData;
    int timeMeasure;
    bool timeCompose;
    int dataSPI;
    int timeOverhead;
    int perOutBC, modeOutBC, perOutRT, modeOutRT;
    QString loadSPI;
    void *trmData;
    void *recData;
    QString manipulation;
    int iManipulation;
    bool codec, broadcast;
    void destroyData();
    //QTcpSocket tcpSocketBC, tcpSocketRT;
    //Device *devBC, *devRT;

protected:
    void perform();
    void terminate(int);
};

#endif // TRMSINGLETEST_H
