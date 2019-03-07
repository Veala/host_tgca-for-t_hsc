#include "trmsingletest.h"
#include "../testutil.h"
#include "../ctestbc.h"
#include "../command.h"
//#include <QtWidgets/QMenu>

#include <QElapsedTimer>

void TrmSingleTest::setStatSettings()
{
    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("totalErr", stats->findChild<QLabel*>("totalErr"));
                 statsMap.insert("errReceive", stats->findChild<QLabel*>("errReceive"));  ///////////
                 statsMap.insert("errRW", stats->findChild<QLabel*>("errRW"));  ///////////
    statsMap.insert("errNoSW", stats->findChild<QLabel*>("errNoSW"));
    statsMap.insert("errYesSW", stats->findChild<QLabel*>("errYesSW"));
    statsMap.insert("errNoFin", stats->findChild<QLabel*>("errNoFin"));
    statsMap.insert("errCodec", stats->findChild<QLabel*>("errCodec"));
    statsMap.insert("errCompare", stats->findChild<QLabel*>("errCompare"));
    statsMap.insert("errNoIntBC", stats->findChild<QLabel*>("errNoIntBC"));
    statsMap.insert("errNoIntRT", stats->findChild<QLabel*>("errNoIntRT"));
    statsMap.insert("errStatusRT", stats->findChild<QLabel*>("errStatusRT"));
    statsMap.insert("errBefore", stats->findChild<QLabel*>("errBefore"));
    statsMap.insert("errFatal", stats->findChild<QLabel*>("errFatal"));
    //stats->findChild<QLabel*>("errFatal")->setVisible(false);
    //stats->findChild<QLabel*>("labelErrFatal")->setVisible(false);
                //statsMap.insert("totalReset", stats->findChild<QLabel*>("totalReset"));    // сброс не реализован
                 stats->findChild<QLabel*>("totalReset")->setVisible(false);
                 stats->findChild<QLabel*>("labelTotalReset")->setVisible(false);
    statsMap.insert("errOther", stats->findChild<QLabel*>("errOther"));
}

void TrmSingleTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    changeConnections();

    defineFields();

    load(fName);

    setFieldConnections();
    setTestConnections();

    recalcMax();
    onRadioCycle();
    onTypeChanged();
    disableUnimplemented();    // отключить нереализованные настройки

    addDevicesTolist();

    objToThread = new trmSingleObjToThread();
    connectThread();

    setStatSettings();
    connectStatisticSlots();

    testThread.start();
}


void TrmSingleTest::defineFields()
{
    dataGen.settings();

    lineEditLen = settings->findChild<QLineEdit*>("lineEditLen");
    lineEditTime = settings->findChild<QLineEdit*>("lineEditTime");
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");

    comboBoxRTA = settings->findChild<QComboBox*>("comboBoxRTA");
    comboBoxCode = settings->findChild<QComboBox*>("comboBoxCode");
    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");
    comboBoxAmplBC = settings->findChild<QComboBox*>("comboBoxAmplBC");
    comboBoxAmplRT = settings->findChild<QComboBox*>("comboBoxAmplRT");

    checkBoxBroad = settings->findChild<QCheckBox*>("checkBoxBroad");
    checkBoxCodec = settings->findChild<QCheckBox*>("checkBoxCodec");
    checkBoxEnaInt = settings->findChild<QCheckBox*>("checkBoxEnaInt");
    checkBoxEnaAddr = settings->findChild<QCheckBox*>("checkBoxEnaAddr");
    checkBoxUseInt = settings->findChild<QCheckBox*>("checkBoxUseInt");
    labelUseInt = settings->findChild<QLabel*>("labelUseInt");
    checkBoxOut = settings->findChild<QCheckBox*>("checkBoxOut");
    checkBoxDevBC = settings->findChild<QCheckBox*>("checkBoxDevBC");
    checkBoxDevRT = settings->findChild<QCheckBox*>("checkBoxDevRT");

    radioButtonSingle = settings->findChild<QRadioButton*>("radioButtonSingle");
    radioButtonUnlimited = settings->findChild<QRadioButton*>("radioButtonUnlimited");
    radioButtonEnter = settings->findChild<QRadioButton*>("radioButtonEnter");

    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");

    labelLen = settings->findChild<QLabel*>("labelLen");

    spinBoxCycle = settings->findChild<QSpinBox*>("spinBoxCycle");
    checkBoxInit = settings->findChild<QCheckBox*>("checkBoxInit");

    lineEditSPIData = settings->findChild<QLineEdit*>("lineEditSPIData");
    checkBoxConfRegLoad = settings->findChild<QCheckBox*>("checkBoxConfRegLoad");
    checkBoxRTALoad = settings->findChild<QCheckBox*>("checkBoxRTALoad");
    comboBoxSPILoad = settings->findChild<QComboBox*>("comboBoxSPILoad");
    checkBoxCounters = settings->findChild<QCheckBox*>("checkBoxCounters");
    comboBoxSPIdr = settings->findChild<QComboBox*>("comboBoxSPIdr");
    comboBoxSPIen = settings->findChild<QComboBox*>("comboBoxSPIen");

    checkBoxBCOut = settings->findChild<QCheckBox*>("checkBoxBCOut");
    checkBoxRTOut = settings->findChild<QCheckBox*>("checkBoxRTOut");

    comboBoxCountPref = settings->findChild<QComboBox*>("comboBoxCountPref");
    comboBoxBCOutPref = settings->findChild<QComboBox*>("comboBoxBCOutPref");
    comboBoxRTOutPref = settings->findChild<QComboBox*>("comboBoxRTOutPref");
    lineEditCountOut = settings->findChild<QLineEdit*>("lineEditCountOut");
    lineEditBCOut = settings->findChild<QLineEdit*>("lineEditBCOut");
    lineEditRTOut = settings->findChild<QLineEdit*>("lineEditRTOut");

    checkBoxWinMode = settings->findChild<QCheckBox*>("checkBoxWinMode");
    lineEditReservePause = settings->findChild<QLineEdit*>("lineEditReservePause");

    comboBoxErrStatusBC = settings->findChild<QComboBox*>("comboBoxErrStatusBC");
    comboBoxErrStatusRT = settings->findChild<QComboBox*>("comboBoxErrStatusRT");
    comboBoxCheckRW = settings->findChild<QComboBox*>("comboBoxCheckRW");
    comboBoxRec = settings->findChild<QComboBox*>("comboBoxRec");
    comboBoxBCIntErr = settings->findChild<QComboBox*>("comboBoxBCIntErr");
    comboBoxRTIntErr = settings->findChild<QComboBox*>("comboBoxRTIntErr");
    comboBoxMemBCRT = settings->findChild<QComboBox*>("comboBoxMemBCRT");
    comboBoxCheckRTA = settings->findChild<QComboBox*>("comboBoxCheckRTA");
    comboBoxErrRegComp = settings->findChild<QComboBox*>("comboBoxErrRegComp");
    comboBoxWrongCfgReg = settings->findChild<QComboBox*>("comboBoxWrongCfgReg");

    comboBoxTestType = settings->findChild<QComboBox*>("comboBoxTestType");
    comboBoxCompSPI = settings->findChild<QComboBox*>("comboBoxCompSPI");
    comboBoxCompData = settings->findChild<QComboBox*>("comboBoxCompData");

    labelSPIData = settings->findChild<QLabel*>("labelSPIData");
    labelSPIdr = settings->findChild<QLabel*>("labelSPIdr");
    labelSPIen = settings->findChild<QLabel*>("labelSPIen");

    labelBroad = settings->findChild<QLabel*>("labelBroad");

    checkBoxParamView = settings->findChild<QCheckBox*>("checkBoxParamView");
    comboBoxSpeed = settings->findChild<QComboBox*>("comboBoxSpeed");
    labelSpeed = settings->findChild<QLabel*>("labelSpeed");

    lineEditOver =  settings->findChild<QLineEdit*>("lineEditOver");
}

void TrmSingleTest::load(QString fName)
{
top_1
    lineEditLen->setText(out.readLine());
    comboBoxRTA->setCurrentText(out.readLine());
    comboBoxCode->setCurrentText(out.readLine());
    checkBoxBroad->setChecked(!out.readLine().isEmpty());

    comboBoxManType->setCurrentText(out.readLine());
    checkBoxCodec->setChecked(!out.readLine().isEmpty());
    checkBoxEnaInt->setChecked(!out.readLine().isEmpty());
    checkBoxEnaAddr->setChecked(!out.readLine().isEmpty());
    comboBoxAmplBC->setCurrentText(out.readLine());
    comboBoxAmplRT->setCurrentText(out.readLine());

    QString genDataType = out.readLine();
    QString genDataBegin = out.readLine();
    QString genDataStep = out.readLine();
    QString genDataNumStep = out.readLine();
    QString genDataUnit = out.readLine();
    dataGen.init(genDataType, genDataBegin, genDataStep, genDataNumStep, genDataUnit, "");

    lineEditTime->setText(out.readLine());
    lineEditPause->setText(out.readLine());
    checkBoxUseInt->setChecked(!out.readLine().isEmpty());
    checkBoxOut->setChecked(!out.readLine().isEmpty());

    lineEditDevBC->setText(out.readLine());
    checkBoxDevBC->setChecked(!out.readLine().isEmpty());
    lineEditDevRT->setText(out.readLine());
    checkBoxDevRT->setChecked(!out.readLine().isEmpty());

    checkBoxParamView->setChecked(!out.readLine().isEmpty());
    checkBoxInit->setChecked(!out.readLine().isEmpty());
    spinBoxCycle->setValue(out.readLine().toInt());
    int cycleMode = out.readLine().toInt();
    if (cycleMode == 0)
        radioButtonEnter->setChecked(true);
    else if (cycleMode == 1)
        radioButtonSingle->setChecked(true);
    else
        radioButtonUnlimited->setChecked(true);

    lineEditSPIData->setText(out.readLine());
    checkBoxConfRegLoad->setChecked(!out.readLine().isEmpty());
    checkBoxRTALoad->setChecked(!out.readLine().isEmpty());
    comboBoxSPILoad->setCurrentText(out.readLine());
    checkBoxCounters->setChecked(!out.readLine().isEmpty());
    comboBoxSPIdr->setCurrentIndex(out.readLine().toInt());
    comboBoxSPIen->setCurrentIndex(out.readLine().toInt());

    checkBoxBCOut->setChecked(!out.readLine().isEmpty());
    checkBoxRTOut->setChecked(!out.readLine().isEmpty());
    checkBoxWinMode->setChecked(!out.readLine().isEmpty());
    comboBoxCountPref->setCurrentText(out.readLine());
    comboBoxBCOutPref->setCurrentText(out.readLine());
    comboBoxRTOutPref->setCurrentText(out.readLine());
    lineEditCountOut->setText(out.readLine());
    lineEditBCOut->setText(out.readLine());
    lineEditRTOut->setText(out.readLine());

    comboBoxErrStatusBC->setCurrentIndex(out.readLine().toInt());
    comboBoxErrStatusRT->setCurrentIndex(out.readLine().toInt());
    comboBoxCheckRW->setCurrentIndex(out.readLine().toInt());
    comboBoxRec->setCurrentIndex(out.readLine().toInt());
    comboBoxBCIntErr->setCurrentIndex(out.readLine().toInt());
    comboBoxRTIntErr->setCurrentIndex(out.readLine().toInt());
    comboBoxMemBCRT->setCurrentIndex(out.readLine().toInt());
    comboBoxCheckRTA->setCurrentIndex(out.readLine().toInt());
    comboBoxErrRegComp->setCurrentIndex(out.readLine().toInt());
    comboBoxWrongCfgReg->setCurrentIndex(out.readLine().toInt());
    comboBoxCompSPI->setCurrentIndex(out.readLine().toInt());
    comboBoxCompData->setCurrentIndex(out.readLine().toInt());
    comboBoxTestType->setCurrentIndex(out.readLine().toInt());
    lineEditReservePause->setText(out.readLine());
    comboBoxSpeed->setCurrentIndex(out.readLine().toInt());
    lineEditOver->setText(out.readLine());

    settingsFile.close();
}

void TrmSingleTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)
    in << lineEditLen->text() << endl;
    in << comboBoxRTA->currentText() << endl;
    in << comboBoxCode->currentText() << endl;
    in << (checkBoxBroad->isChecked() ? "1" : "") << endl;

    in << comboBoxManType->currentText() << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxEnaAddr->isChecked() ? "1" : "") << endl;
    in << comboBoxAmplBC->currentText() << endl;
    in << comboBoxAmplRT->currentText() << endl;

    in << dataGen.genType() << endl;
    in << dataGen.begin() << endl;
    in << dataGen.step() << endl;
    in << dataGen.numStep() << endl;
    in << dataGen.unit() << endl;

    in << lineEditTime->text() << endl;
    in << lineEditPause->text() << endl;
    in << (checkBoxUseInt->isChecked() ? "1" : "") << endl;
    in << (checkBoxOut->isChecked() ? "1" : "") << endl;

    in << lineEditDevBC->text() << endl;
    in << (checkBoxDevBC->isChecked() ? "1" : "") << endl;
    in << lineEditDevRT->text() << endl;
    in << (checkBoxDevRT->isChecked() ? "1" : "") << endl;

    in << (checkBoxParamView->isChecked() ? "1" : "") << endl;
    in << (checkBoxInit->isChecked() ? "1" : "") << endl;
    in << spinBoxCycle->text() << endl;
    int cycleMode = radioButtonEnter->isChecked() ? 0 : (radioButtonSingle->isChecked() ? 1 : 2);
    in << cycleMode << endl;

    in << lineEditSPIData->text() << endl;
    in << (checkBoxConfRegLoad->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTALoad->isChecked() ? "1" : "") << endl;
    in << comboBoxSPILoad->currentText() << endl;
    in << (checkBoxCounters->isChecked() ? "1" : "") << endl;
    in << comboBoxSPIdr->currentIndex() << endl;
    in << comboBoxSPIen->currentIndex() << endl;

    in << (checkBoxBCOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxRTOut->isChecked() ? "1" : "") << endl;
    in << (checkBoxWinMode->isChecked() ? "1" : "") << endl;
    in << comboBoxCountPref->currentText() << endl;
    in << comboBoxBCOutPref->currentText() << endl;
    in << comboBoxRTOutPref->currentText() << endl;
    in << lineEditCountOut->text() << endl;
    in << lineEditBCOut->text() << endl;
    in << lineEditRTOut->text() << endl;

    in << comboBoxErrStatusBC->currentIndex() << endl;
    in << comboBoxErrStatusRT->currentIndex() << endl;
    in << comboBoxCheckRW->currentIndex() << endl;
    in << comboBoxRec->currentIndex() << endl;
    in << comboBoxBCIntErr->currentIndex() << endl;
    in << comboBoxRTIntErr->currentIndex() << endl;
    in << comboBoxMemBCRT->currentIndex() << endl;
    in << comboBoxCheckRTA->currentIndex() << endl;
    in << comboBoxErrRegComp->currentIndex() << endl;
    in << comboBoxWrongCfgReg->currentIndex() << endl;
    in << comboBoxCompSPI->currentIndex() << endl;
    in << comboBoxCompData->currentIndex() << endl;
    in << comboBoxTestType->currentIndex() << endl;
    in << lineEditReservePause->text() << endl;
    in << comboBoxSpeed->currentIndex() << endl;
    in << lineEditOver->text() << endl;

    settingsFile.close();
}

void TrmSingleTest::setFieldConnections()
{
    connect(comboBoxManType, SIGNAL(activated(int)), this, SLOT(recalcMax()));
    connect(checkBoxCodec, SIGNAL(clicked(bool)), this, SLOT(recalcMax()));
    connect(radioButtonSingle, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(radioButtonUnlimited, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(radioButtonEnter, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));
    connect(checkBoxInit, SIGNAL(clicked()), this, SLOT(onCheckInit()));
    connect(comboBoxTestType, SIGNAL(activated(int)), this, SLOT(onTypeChanged()));
}

void TrmSingleTest::onTypeChanged()
{
    labelBroad->setEnabled(comboBoxTestType->currentIndex() == 1);
    checkBoxBroad->setVisible(comboBoxTestType->currentIndex() == 1);
    if (comboBoxTestType->currentIndex() == 2) // циркулярный возврат
    {
        if (comboBoxSpeed->count() == 4)
        {
            if (comboBoxSpeed->currentIndex() == 3)
                comboBoxSpeed->setCurrentIndex(0);
            comboBoxSpeed->removeItem(3);
        }
    }
    else
    {
        if (comboBoxSpeed->count() < 4)
            comboBoxSpeed->addItem(tr("Альтернативный замер"));
    }
}

void TrmSingleTest::onRadioCycle()
{
    if (!userMode())
        spinBoxCycle->setEnabled(radioButtonEnter->isChecked());
}

unsigned int TrmSingleTest::maxNumByte()
{
    return calcNumWordInSymbol(comboBoxManType->currentIndex(), checkBoxCodec->isChecked())*MAXNUMSYM*sizeof(word32_t) - sizeof(word32_t);
}

void TrmSingleTest::recalcMax()
{
    mnb = maxNumByte();
    labelLen->setText(QString(tr("Длина сообщения в байтах (0-%1)").arg(mnb)));
    if (lineEditLen->text() != "max" && lineEditLen->text().toUInt() > mnb)
        lineEditLen->setText("max");
}

void TrmSingleTest::onCheckInit()
{
    spinBoxCycle->setMinimum(checkBoxInit->isChecked() ? 0 : 1);
}

void TrmSingleTest::disableUnimplemented()
{
    // ПОКА НЕ РЕАЛИЗОВАНЫ
    comboBoxCheckRW->setDisabled(true);                             // проверка ответного пакета
    settings->findChild<QLabel*>("labelCheckRW")->setDisabled(true);
    comboBoxRec->setDisabled(true);                                 // проверка, что сообщение от КШ принято ОУ
    settings->findChild<QLabel*>("labelReceived")->setDisabled(true);
    comboBoxCountPref->setDisabled(true);                           // выбор периодичности вывода счётчиков принятых/переданных пакетов
    lineEditCountOut->setDisabled(true);                            // частота вывода счётчиков принятых/переданных пакетов
    settings->findChild<QLabel*>("labelCntOut")->setDisabled(true);
    settings->findChild<QLabel*>("labelCounterSuf")->setDisabled(true);
    checkBoxParamView->setDisabled(true);
    settings->findChild<QLabel*>("labelParamView")->setDisabled(true);
}

void TrmSingleTest::setEnabledSpecial(bool b)
{
    AbstractTest::setEnabledSpecial(b);

    if (b)
    {
        onTypeChanged();
        onRadioCycle();

        disableUnimplemented();    // отключить нереализованные настройки
    }
    else
    {
        checkBoxCodec->setEnabled(true);
        settings->findChild<QLabel*>("labelCodec")->setEnabled(true);
        settings->findChild<QLabel*>("labelHeaderConfig")->setEnabled(true);
        settings->findChild<QLabel*>("labelHeaderCommand")->setEnabled(true);

        checkBoxWinMode->setVisible(false);
        settings->findChild<QLabel*>("labelWinMode")->setVisible(false);

        if (checkBoxInit->isChecked())
        {
            /// Подтип теста - ИНИЦИАЛИЗАЦИЯ
            if (checkBoxDevRT->isChecked())
            {
                comboBoxRTA->setEnabled(true);
                settings->findChild<QLabel*>("labelRTA")->setEnabled(true);
            }
            // устройства
            //checkBoxDevBC->setEnabled(true);
            //checkBoxDevRT->setEnabled(true);
            //settings->findChild<QLabel*>("labelHeaderDevices")->setEnabled(true);
            //settings->findChild<QLabel*>("labelDevBC")->setEnabled(true);
            //settings->findChild<QLabel*>("labelDevRT")->setEnabled(true);
        }
        else
        {
            dataGen.enable(true);

            if (!checkBoxEnaAddr->isChecked())
            {
                checkBoxRTALoad->setEnabled(true);
                settings->findChild<QLabel*>("labelRTARegLoad")->setEnabled(true);
                settings->findChild<QLabel*>("labelHeaderRegisters")->setEnabled(true);
            }
            comboBoxRTA->setEnabled(true);
            settings->findChild<QLabel*>("labelRTA")->setEnabled(true);

            settings->findChild<QLabel*>("labelHeaderTest")->setEnabled(true);
            settings->findChild<QLabel*>("labelTime")->setEnabled(true);
            settings->findChild<QLabel*>("labelPause")->setEnabled(true);
            lineEditTime->setEnabled(true);
            lineEditPause->setEnabled(true);
            spinBoxCycle->setEnabled(true);

            if (comboBoxSpeed->currentIndex() == 0)
            {
                lineEditLen->setEnabled(true);
                labelLen->setEnabled(true);

                // периодичность вывода
                //comboBoxBCOutPref->setEnabled(true);
                //comboBoxRTOutPref->setEnabled(true);
                //lineEditBCOut->setEnabled(true);
                //lineEditRTOut->setEnabled(true);
            }
            else
            {
                /// Подтип теста - СКОРОСТЬ
                lineEditOver->setEnabled(true);
                settings->findChild<QLabel*>("labelOver")->setEnabled(true);
            }
        }

    }


//        QCheckBox *checkBoxParamView;      // вывод параметров теста в окно тестов или проекта    не и
//        QCheckBox *checkBoxOut;             // включение вывода в окно тестов







        // включение проверок и сравнений
//        QComboBox *comboBoxErrStatusBC;     // проверка флагов ошибок в регистре статуса КШ
//        QComboBox *comboBoxErrStatusRT;     // проверка флагов ошибок в регистре статуса ОУ



}

void TrmSingleTest::startTest()
{
    trmSingleObjToThread* curThread = (trmSingleObjToThread*)objToThread;

    int tr_mode = comboBoxTestType->currentIndex();
    curThread->RTtoBC = (tr_mode != 1);
    curThread->BCtoRT = (tr_mode != 0);
    curThread->broadcast = curThread->RTtoBC ? false : checkBoxBroad->isChecked();
    curThread->useInt = checkBoxUseInt->isChecked() && (checkBoxEnaInt->isChecked() || !checkBoxConfRegLoad->isChecked());
        // Если загрузка cfg_reg выключена, то теоретически может быть загружен флаг en_rt_bc_int. Если проверка cfg_reg
        // включена и в реальном регистре флаг en_rt_bc_int равен 0, то режим useInt автоматически выключится.
        // Иначе за противоречия в настройках отвечает пользователь.
    if (checkBoxUseInt->isChecked() && !checkBoxEnaInt->isChecked() && checkBoxConfRegLoad->isChecked())
    {
        message(tr("Использование прерывание КШ не разрешено"));
    }
    curThread->setOutEnabled(checkBoxOut->isChecked());
    curThread->waitTime = lineEditTime->text().toInt(0, 16);
    curThread->rtaddr = comboBoxRTA->currentIndex();
    curThread->checkCountersEnable = checkBoxCounters->isChecked();
    curThread->writeCfg = checkBoxConfRegLoad->isChecked();
    //curThread->modeSPI = val_REG_CR_SPI_dr_UNDEF;
    curThread->loadSPI = comboBoxSPILoad->currentText();
    curThread->trm_size = 0;
    curThread->trmData = 0;
    curThread->recData = 0;
    //curThread->testData = 0;
    curThread->windowMode = checkBoxWinMode->isChecked();
    curThread->compEnableMemBCRT = comboBoxMemBCRT->currentIndex();
    curThread->compEnableRTA = comboBoxCheckRTA->currentIndex();
    curThread->compEnableData = comboBoxCompData->currentIndex();
    curThread->compEnableReg = comboBoxErrRegComp->currentIndex();
    curThread->checkFinSPI = comboBoxCompSPI->currentIndex();
    curThread->timeMeasure = comboBoxSpeed->currentIndex();
    if (curThread->timeMeasure == 3 && tr_mode == 2)
    {
        curThread->timeMeasure = 0;
        message("Альтернативное измерение скорости в режиме циркулярного возврата не разрешено");
    }
    curThread->timeCompose = (curThread->timeMeasure == 1 || curThread->timeMeasure == 2);
    if (radioButtonEnter->isChecked())
        curThread->iterCycle = spinBoxCycle->value();
    else if (radioButtonUnlimited->isChecked())
        curThread->iterCycle = -1;
    else
        curThread->iterCycle = 1;
    curThread->initEnable = checkBoxInit->isChecked();
    curThread->pauseTime = lineEditPause->text().toInt(0, 10);
    curThread->postponeTime = lineEditReservePause->text().toInt(0, 10);
    curThread->checkStatusErrBC = (comboBoxErrStatusBC->currentIndex() > 0);
    curThread->checkStatusErrRT = (comboBoxErrStatusRT->currentIndex() > 0);
    curThread->noIntFatalBC = (comboBoxBCIntErr->currentIndex() != 0);
    curThread->noIntFatalRT = comboBoxRTIntErr->currentIndex();
    curThread->statusBCOut = checkBoxBCOut->isChecked();
    curThread->statusRTOut = checkBoxRTOut->isChecked();
    curThread->perOutBC = lineEditBCOut->text().isEmpty() ? 0 : lineEditBCOut->text().toInt(0, 10);
    curThread->perOutRT = lineEditRTOut->text().isEmpty() ? 0 : lineEditRTOut->text().toInt(0, 10);
    curThread->modeOutBC = comboBoxBCOutPref->currentIndex();
    curThread->modeOutRT = comboBoxRTOutPref->currentIndex();
    curThread->timeOverhead = lineEditOver->text().isEmpty() ? 0 : lineEditOver->text().toInt(0, 10);
    curThread->checkLoadCfg = comboBoxWrongCfgReg->currentIndex() > 0;

    curThread->manipulation = comboBoxManType->currentText();
    curThread->codec = checkBoxCodec->isChecked();

    ushort rta = curThread->broadcast ? BRD_RT_ADDR : curThread->rtaddr;

    int index = 0;
    if (checkBoxDevBC->isChecked())
    {
        curThread->devBC = deviceList.at(0);
        index ++;
        quint32 cfgBC = curThread->devBC->configuration.getConfigReg();
        curThread->devBC->reg_hsc_cfg.setData(cfgBC);
        curThread->devBC->reg_hsc_cfg.rt_bc = CFG_MODE_BC;   // признак КШ
        curThread->devBC->reg_hsc_cfg.type_man = comboBoxManType->currentIndex();
        curThread->devBC->reg_hsc_cfg.en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;
        curThread->devBC->reg_hsc_cfg.ena_codec = checkBoxCodec->isChecked() ? 1 : 0;

        if (comboBoxSPILoad->currentText() == "КШ")
        {
            curThread->dataSPI = lineEditSPIData->text().toInt(0, 16);
            curThread->devBC->reg_hsc_cr_spi.dr8_16_32 = comboBoxSPIdr->currentIndex();
            curThread->devBC->reg_hsc_cr_spi.spi_en = comboBoxSPIen->currentIndex();
        }

        curThread->amplBC= comboBoxAmplBC->currentIndex();
    }
    else
        curThread->devBC = 0;


    if (checkBoxDevRT->isChecked())
    {
        curThread->devRT = deviceList.at(index);

        quint32 cfgRT = curThread->devRT->configuration.getConfigReg();
        curThread->devRT->reg_hsc_cfg.setData(cfgRT);
        curThread->devRT->reg_hsc_cfg.rt_bc = CFG_MODE_RT;     // признак ОУ
        curThread->devRT->reg_hsc_cfg.type_man = comboBoxManType->currentIndex();
//        curThread->devRT->reg_hsc_cfg.en_rt_bc_int = checkBoxEnaInt->isChecked() ? 1 : 0;   // прерывание ОУ не было включено
        curThread->devRT->reg_hsc_cfg.ena_codec = checkBoxCodec->isChecked() ? 1 : 0;
        curThread->devRT->reg_hsc_cfg.rtavsk_ena = checkBoxEnaAddr->isChecked() ? 1 : 0;
        if (checkBoxEnaAddr->isChecked())
        {
            curThread->devRT->reg_hsc_cfg.rtavsk = curThread->rtaddr;
            // если конфигурационный регистр будет загружен и адрес ОУ будет в нём,
            // то во вспомогательный регистр адрес загружать не нужно, но мы этому не препятствуем
            // curThread->rtaddr = MAX_RT_ADDR + 1;
        }
        curThread->amplRT = comboBoxAmplRT->currentIndex();
        if (comboBoxSPILoad->currentText() == "ОУ")
        {
            curThread->dataSPI = lineEditSPIData->text().toInt(0, 16);
            curThread->devRT->reg_hsc_cr_spi.dr8_16_32 = comboBoxSPIdr->currentIndex();
            curThread->devRT->reg_hsc_cr_spi.spi_en = comboBoxSPIen->currentIndex();
        }
    }
    else
        curThread->devRT = 0;

    if (!checkBoxRTALoad->isChecked())
        curThread->rtaddr = MAX_RT_ADDR + 1;

    uint num_b = mnb;
    if (lineEditLen->text() != "max")
        num_b = lineEditLen->text().toInt(0, 10); // или 16
    if (num_b > mnb)
    {
        num_b = mnb;
        lineEditLen->setText("max");
    }
    CTestBC test;
    test.setConfigFlds(comboBoxManType->currentIndex(), checkBoxCodec->isChecked());
    if (test.maxNumByte() != mnb)
    {
        qDebug() << "Error max num byte: " << mnb << " " << test.maxNumByte();
        throw QString("Unexpected configuration parameters");
    }
    curThread->nwrd = test.numWordInSymbol();
    int num_s = test.NumSymOFDM(num_b);

    if (num_b != 0 || (curThread->devBC != 0 && curThread->iterCycle != 0))
    {
        void *rawData = dataGen.createData(num_b, 1);

        curThread->trm_size = (test.NumSymOFDM(num_b) + 1) * NUMBYTEINOFDMSYM;
        //int ns = test.NumSymOFDM(num_b) + 1;

        // Создаём пакет передачи данных КШ-ОУ
        // В случае команды передачи ОУ-КШ массив данных, следующий за командным словом,
        // будет использоваться для передачи и сравнения.

        if (curThread->BCtoRT)
            qDebug() << "BC-RT:  sizes " << num_b << " " << (num_s+1) << " " << curThread->trm_size;
        curThread->trmData = malloc(curThread->trm_size);
        if (curThread->trmData == 0)
        {
            message(tr("not enough memory"));
            destroyData(rawData);
            return;
        }
        if (!test.createCommandPack(curThread->trmData, curThread->trm_size,
                                    rawData, num_b, rta, tgca_tr_REC, comboBoxCode->currentIndex()))
        {
            message(tr("Error in createCommandPack() BC-RT !!!"));
            destroyData(rawData);
            curThread->destroyData();
            return;
        }

        destroyData(rawData);

        if (curThread->RTtoBC)  // приём от ОУ или циркулярный возврат
        {
            // Создаём командный пакет на передачу ОУ-КШ
            qDebug() << "RT-BC:  sizes " << num_b << " " << (num_s+1) << " " << NUMBYTEINOFDMSYM;
            curThread->recData = malloc(NUMBYTEINOFDMSYM);
            if (curThread->recData == 0)
            {
                message(tr("not enough memory"));
                destroyData(rawData);
                return;
            }
            if (!test.createCommandPack(curThread->recData, NUMBYTEINOFDMSYM,
                                        0, num_b, rta, tgca_tr_TRM, comboBoxCode->currentIndex()))
            {
                message(tr("Error in createCommandPack() RT-BC !!!"));
                destroyData(rawData);
                curThread->destroyData();
                return;
            }
        }
    }

    // Начальные сообщения теста
    QString msg = comboBoxTestType->currentText() + tr(" %1 байт данных (%2 символ").arg(num_b).arg(num_s);
    QString suf("");
    int res = num_s%10;
    if (num_s>10 && (num_s-res)%100 == 10)
        suf = "ов";
    else if (res>1 && res<=4)
        suf = "";
    else if (res != 1)
        suf = "ов";
    curThread->privet_msg = msg + suf + tr(").");

    QString eng_1 = curThread->BCtoRT ? (curThread->RTtoBC ? tr("Wrap around") : tr("Receiving")) : tr("Transmission");
    QString eng_2 = tr(" data of %1 byte (%2 OFDM symbol)").arg(num_s).arg(num_s);
    curThread->hello_msg = eng_1 + eng_2;

    //message(curThread->privet_msg);

    emit startTestTh();
}

void writeSPI(Device* dev, quint32 dataSPI)
{
    QVector<BaseReg*> regs;
    regs.append(&dev->reg_hsc_dr_spi_lsw);
    if (dev->reg_hsc_cr_spi.dr8_16_32 == val_REG_CR_SPI_dr_DEP32)
    {
        regs.append(&dev->reg_hsc_dr_spi_msw);
        dev->reg_hsc_dr_spi_msw.data = dataSPI >> 16;
    }
    regs.append(&dev->reg_hsc_cr_spi);
    dev->reg_hsc_dr_spi_lsw.data = dataSPI & 0xFFFF;

    dev->writeRegs(regs);

    setRegWritten(dev, dev->reg_hsc_cr_spi);
    setRegWritten(dev, dev->reg_hsc_dr_spi_lsw);
    if (dev->reg_hsc_cr_spi.dr8_16_32 == val_REG_CR_SPI_dr_DEP32)
        setRegWritten(dev, dev->reg_hsc_dr_spi_msw);
}

/*
void trmSingleObjToThread::checkStatusReg(Device *dev, int *status)
{
    int addr_stat = REG_VSK_status;
    QByteArray answer;
    QByteArray readArray = cmdHead(3, 4);
    readArray.append((char*)&addr_stat, 4);
    dev->read_F1(readArray, answer);

    *status = *(int*)(answer.data());
}
*/

bool inline validOut(int freq, int mode, int iter)
{
//    bool b = (freq>0 && (mode ? iter%freq == 0 : iter < freq));
//    qDebug() << "Valid:  " << freq << " " << mode << " iter=" << iter << "   " << b;
    return (freq>0 && (mode ? iter%freq == 0 : iter < freq));
}

bool trmSingleObjToThread::checkSPI(Device* dev)
{
    int bRet = true;
    int oldval = dev->reg_hsc_cr_spi.getData();
    QTimer timer;
    timer.setInterval(waitTime);
    timer.start();
    do
    {
        dev->readReg(&dev->reg_hsc_cr_spi);
        if (dev->reg_hsc_cr_spi.spif == 1)
            break;
  /*      {
            dev->reg_hsc_cr_spi.spif = 0;
            return
        }*/
    }
    while (timer.remainingTime() > 0);

    if (dev->reg_hsc_cr_spi.spif == 1)
    {
        dev->reg_hsc_cr_spi.spif = 0;
        if (checkFinSPI && (oldval != dev->reg_hsc_cr_spi.getData()))
        {
            emit statsOutputReady("errCompare", 1);
            stdOutput(tr("Ошибка сравнения регистра управления обменом по SPI"), tr("Comparison SPI register wrong"));
            bRet = false;
        }
    }
    else
    {
        emit statsOutputReady("errCompare", 1);
        stdOutput(tr("Ошибка признака завершения обмена по SPI"), tr("Check SPI register wrong: spif"));
        bRet = false;
        if (checkFinSPI && (oldval != dev->reg_hsc_cr_spi.getData()))
            stdOutput(tr("Ошибка сравнения регистра управления обменом по SPI"), tr("Comparison SPI register wrong"));
    }
    return bRet;
}

bool trmSingleObjToThread::checkStatusRegBC(int statusBC, int interruption, int it, bool *error)
{
    bool bNoInt = false;

    if (interruption == 0)
    {
        stdOutput(tr("Итерация = %1   Нет прерывания КШ").arg(it, 6), tr("Iter = %1   No interruption from BC").arg(it, 6));
        emit statsOutputReady("errNoIntBC", 1);
        *error = true;
        bNoInt = true;
    }
    if ((statusBC & fl_REG_STATUS_rt_bc_int) == 0)
    {
        stdOutput(tr("Итерация = %1   Нет признака завершения обмена КШ").arg(it, 6),
                  tr("Iter = %1   No interruption flag in BS status").arg(it, 6));
        emit statsOutputReady("errNoFin", 1);
        *error = true;
        bNoInt = true;
    }
    int trmbuf, recbuf;
    if (statusBCOut)
    {
        trmbuf = statusBC&fl_REG_STATUS_tx_num_buf ? 1 : 0;
        recbuf = statusBC&fl_REG_STATUS_rx_num_buf ? 1 : 0;
    }

    if (checkStatusErrBC && (devRT != 0))
    {
        bool bErr = false;

        if (codec && (statusBC & fl_REG_STATUS_rs_err) != 0)
        {
            emit statsOutputReady("errCodec", 1);
            bErr = true;
        }
        if (statusBC & fl_REG_STATUS_no_aw_err)
        {
            emit statsOutputReady("errNoSW", 1);
            bErr = true;
            if (broadcast)
            {
                emit statsOutputReady("errOther", 1);
                stdOutput(tr("Неправильный флаг ошибки для групповой команды"), tr("Illegal error flag for broadcast message"));
            }
        }
        if (statusBC & fl_REG_STATUS_yes_aw_gr_err)
        {
            emit statsOutputReady("errYesSW", 1);
            bErr = true;
            if (!broadcast)
            {
                emit statsOutputReady("errOther", 1);
                stdOutput(tr("Неправильный флаг ошибки для негрупповой команды"), tr("Illegal error flag for not broadcast message"));
            }
        }
        if (bErr)
        {
            if (statusBCOut)
                stdOutput(QString(tr("Итерация = %1   Ошибка в статусе КШ: %2  буфер передачи %3  буфер приёма %4").
                                  arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)),
                          QString(tr("Iter = %1   Errors in BC status: %2  trm buf %3  rec buf %4").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)));
            else
                stdOutput(QString(tr("Итерация = %1   Ошибка в статусе КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))),
                          QString(tr("Iter = %1   Errors in BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))));
            *error = true;
        }
    }
    else if (bNoInt)
    {
        if (statusBCOut)
            stdOutput(QString(tr("Статус КШ: %1  буфер передачи %2  буфер приёма %3").
                              arg(statusBC, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)),
                      QString(tr("BC status: %1  trm buf %2  rec buf %3").arg(statusBC, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)));
        else
            stdOutput(QString(tr("Статус КШ: %1").arg(statusBC, 4, 16, QLatin1Char('0'))),
                      QString(tr("BC status: %1").arg(statusBC, 4, 16, QLatin1Char('0'))));
    }
    else if (validOut(perOutBC, modeOutBC, it))
    {
        if (statusBCOut)
            stdOutput(QString(tr("Итерация = %1   Статус КШ: %2  буфер передачи %3  буфер приёма %4").
                              arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)),
                      QString(tr("Iter = %1   BC status: %2  trm buf %3  rec buf %4").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)));
        else
            stdOutput(QString(tr("Итерация = %1   Статус КШ: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))),
                      QString(tr("Iter = %1   BC status: %2").arg(it, 6).arg(statusBC, 4, 16, QLatin1Char('0'))));
    }
    return bNoInt;
}

bool trmSingleObjToThread::checkStatusRegRT(int status, int it, bool *error)
{
    bool bNoInt = false;

    /*
    if (useInt && noIntFatalRT)  // Нет! Прерывание ОУ не проверяем. Только бит завершения обмена.
    {
        devRT->readReg(&devRT->reg_aux_interruption);
        if (devRT->reg_aux_interruption.inter == 0)
        {
            stdOutput(tr("Итерация = %1   Нет прерывания ОУ").arg(it, 6), tr("Iter = %1   No interruption from RT").arg(it, 6));
            emit statsOutputReady("errNoIntRT", 1);
            *error = true;
            bNoInt = true;
        }
    }
    */
    if ( ((status & fl_REG_STATUS_rt_bc_int) == 0 && (noIntFatalRT || checkStatusErrRT)) ||
         (checkStatusErrRT && ((codec && ((status & fl_REG_STATUS_rs_err) != 0)) || ((status & (fl_REG_STATUS_no_aw_err | fl_REG_STATUS_yes_aw_gr_err)) != 0))) )
    {
        stdOutput(QString(tr("Итерация = %1   Есть ошибки в статусе ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))),
                  QString(tr("Iter = %1   Errors in RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))));
        emit statsOutputReady("errStatusRT", 1);
        *error = true;

        if ((status & fl_REG_STATUS_rt_bc_int) == 0 && (noIntFatalRT || checkStatusErrRT))
        {
            stdOutput(tr("              Нет признака завершения обмена ОУ"),
                      tr("          No interruption flag in RT status"));
            bNoInt = true;
        }
        if (checkStatusErrRT && codec && ((status & fl_REG_STATUS_rs_err) != 0))
        {
            stdOutput(tr("              Ошибка декодера кодека Рида-Соломона"),
                      tr("          Codec error flag in RT status"));
        }
        if (checkStatusErrRT && ((status & (fl_REG_STATUS_no_aw_err | fl_REG_STATUS_yes_aw_gr_err)) != 0))
        {
            stdOutput(tr("              Непредусмотренный флаг ошибки ОУ"),
                      tr("          Invalid error flag(s) in RT status"));
        }

        if (validOut(perOutRT, modeOutRT, it) && statusRTOut)
            {
                int trmbuf = status&fl_REG_STATUS_tx_num_buf ? 1 : 0;
                int recbuf = status&fl_REG_STATUS_rx_num_buf ? 1 : 0;
                stdOutput(QString(tr("Итерация = %1   Статус ОУ: %2  буфер передачи %3  буфер приёма %4").
                                  arg(it, 6).arg(status, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)),
                          QString(tr("Iter = %1   RT status: %2  trm buf %3  rec buf %4").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)));
            }
    }
    else if (validOut(perOutRT, modeOutRT, it))
    {
        if (statusRTOut)
        {
            int trmbuf = status&fl_REG_STATUS_tx_num_buf ? 1 : 0;
            int recbuf = status&fl_REG_STATUS_rx_num_buf ? 1 : 0;
            stdOutput(QString(tr("Итерация = %1   Статус ОУ: %2  буфер передачи %3  буфер приёма %4").
                              arg(it, 6).arg(status, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)),
                      QString(tr("Iter = %1   RT status: %2  trm buf %3  rec buf %4").arg(it, 6).arg(status, 4, 16, QLatin1Char('0')).arg(trmbuf).arg(recbuf)));
        }
        else
            stdOutput(QString(tr("Итерация = %1   Статус ОУ: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))),
                      QString(tr("Iter = %1   RT status: %2").arg(it, 6).arg(status, 4, 16, QLatin1Char('0'))));
    }
    return bNoInt;
}

void trmSingleObjToThread::checkCounters(Device *dev)
{
    if (checkCountersEnable)
    {
        QVector<BaseReg*> regs;
        regs.append(&dev->reg_hsc_cnt_pct_rx_lsw);
        regs.append(&dev->reg_hsc_cnt_pct_rx_msw);
        regs.append(&dev->reg_hsc_cnt_pct_tx_lsw);
        regs.append(&dev->reg_hsc_cnt_pct_tx_msw);
        dev->readRegs(regs);

        qDebug() << "Counters:  " << dev->reg_hsc_cnt_pct_tx_msw.counter << "  " << dev->reg_hsc_cnt_pct_tx_lsw.counter
                 << "  " << dev->reg_hsc_cnt_pct_rx_msw.counter << "  " << dev->reg_hsc_cnt_pct_rx_lsw.counter;
    }
}

trmSingleObjToThread::trmSingleObjToThread():
    commonObjToThread(),
    //cfgBC(0), cfgRT(0),
    amplBC(0), amplRT(0),
    trmData(0), recData(0)
{
}

void trmSingleObjToThread::perform()
{
    qDebug() << "trmSingleObjToThread::perform() started";
    stdOutput(privet_msg, hello_msg);

    int rec_pk_size = NUMBYTEINOFDMSYM;
    int errorsBefore = 0;
    int errorsWithin = 0;
    CTestBC test;

    if (trmData)
    {
        if (trm_size > 0)
        {
            stdOutput(QString(tr("Командное слово: %1")).arg(*(int*)trmData, 8, 16, QLatin1Char('0')),
                      QString(tr("Command word: %1")).arg(*(int*)trmData, 8, 16, QLatin1Char('0')));

            addr_t rta;
            short num_s, tr, code;
            parseCommandWord(trmData, &rta, &num_s, &tr, &code);
            qDebug() << "Command word: addr num_s tr code " << rta << " " << num_s << " " << tr << " " << code;
        }
        else
            stdOutput(QString(tr("Размер данных для передачи - 0 байт!")), QString(tr("Data size = 0 !")));
    }
    if (devBC)
    {
        if (connectBC() != AbstractTest::Running)
            return;

        checkCounters(devBC);
    }
    else
    {
        stdOutput(tr("КШ не используется"), tr("BC not used"));
    }
    if (devRT)
    {
        if (connectRT() != AbstractTest::Running)
            return;

        checkCounters(devRT);
    }
    else
    {
        stdOutput(tr("ОУ не используется"), tr("RT not used"));
    }

    if (pause_stop() == -1)
        return;

    if (devRT)
    {
        if (initEnable)
        {
            qDebug() << "start RT configuration";
            if (writeCfg)
                devRT->configuration.setChecked(config_NUMREG_cfg, false);
            QByteArray regRT = devRT->configuration.getRegistersToWrite();
            if (regRT.size() > 0)
            {
                devRT->write_F1(regRT.data(), regRT.size());
                devRT->configuration.doneWriteReg(regRT);
            }
            ///   Сравнение конфигурации
            if (compEnableReg > 0 && regRT.size() > 0)
            {
                QByteArray addrArray, valArray;
                if (splitOddEven(regRT, addrArray, valArray))
                {
                    // Чтение регистров ОУ для сравнения
                    QByteArray readArrayC;
                    readArrayC.resize(addrArray.size());
                    devRT->read_F1(addrArray.data(), readArrayC.data(), addrArray.size());

                    if (readArrayC == valArray)
                    {
                        stdOutput(tr("Сравнение регистров ОУ успешно"), tr("Comparison RT registers OK"));
                    }
                    else
                    {
                        stdOutput(tr("Ошибка конфигурации ОУ: регистры не совпали"), tr("Comparison RT registers wrong"));
                        stdOutput(tr("Возвращенный размер = %1").arg(readArrayC.size()), tr("Returned size = %1").arg(readArrayC.size()));
                        stdOutput(tr("Возвращенные значения = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                  tr("Returned values = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));
                        emit statsOutputReady("errCompare", 1);
                        errorsBefore++;
                        if (compEnableReg == 2)
                        {
                            setErrorsBeforeCycle(errorsBefore);
                            return;
                        }
                    }
                }
            }
            checkCounters(devRT);
        }

        bool cfg_err = false;
        if (writeCfg)
        {
            devRT->writeReg(&devRT->reg_hsc_cfg);
            setRegWritten(devRT, devRT->reg_hsc_cfg);
          /*  qDebug() << devRT->reg_hsc_cfg.address << " " << devRT->reg_hsc_cfg.ena_aru << " " << devRT->reg_hsc_cfg.ena_codec << " " <<
                        devRT->reg_hsc_cfg.ena_mem_vsk << " " << devRT->reg_hsc_cfg.en_rt_bc_int << " "
                     << devRT->reg_hsc_cfg.rtavsk << " " << devRT->reg_hsc_cfg.rtavsk_ena << " " << devRT->reg_hsc_cfg.rt_bc << " " << devRT->reg_hsc_cfg.type_man;*/

            if ((checkLoadCfg || (compEnableReg > 0)) && devRT->reg_hsc_cfg.getData() != readRegVal(devRT, &devRT->reg_hsc_cfg))
                cfg_err = true;
        }
        else if (checkLoadCfg)
        {
            REG_HSC_cfg cfg;
            devRT->readReg(&cfg);
//            qDebug() << "RT cfg  " << cfg.ena_aru << " " << cfg.ena_codec << " " << cfg.ena_mem_vsk << " " << cfg.en_rt_bc_int << " "
//                     << cfg.rtavsk << " " << cfg.rtavsk_ena << " " << cfg.rt_bc << " " << cfg.type_man;
            if (cfg.rt_bc != CFG_MODE_RT || cfg.ena_codec != codec ||
                cfg.type_man != devRT->reg_hsc_cfg.type_man || cfg.rtavsk_ena != devRT->reg_hsc_cfg.rtavsk_ena)
                cfg_err = true;
            else
            {
                if (useInt && (cfg.en_rt_bc_int != devRT->reg_hsc_cfg.en_rt_bc_int)) // хотя прерывание ОУ сейчас не используется, но всё равно требуем совпадения признака
                    cfg_err = true;
                else
                {
                    if (cfg.rtavsk_ena) // иначе адрес ОУ должен быть во вспомогательном регистре reg_aux_rtaddr, но это не будем проверять,
                                        // потому что это регулируется записью и проверкой регистра reg_aux_rtaddr
                    {
                        if (cfg.rtavsk != devRT->reg_hsc_cfg.rtavsk)
                            cfg_err = true;
                        if (rtaddr <= MAX_RT_ADDR && cfg.rtavsk != rtaddr)
                            cfg_err = true;
                    }
                }
            }
        }
        if (cfg_err)
        {
            stdOutput(tr("Ошибка сравнения конфигурационного регистра ОУ"), tr("Comparison RT cfg register wrong"));
            emit statsOutputReady("errCompare", 1);
            setErrorsBeforeCycle(++errorsBefore);
            return;
        }

        if (amplRT)
        {
            int ampl = (amplRT == 1) ? 0 : amplRT + 14;
            devRT->reg_hsc_amplificatoin_factor.rx_oe_ampl = (amplRT == 1) ? 0 : 1;
            devRT->reg_hsc_amplificatoin_factor.rx_ampl = ampl & FL_REG_AMPL_FACTOR_rx_ampl;
            devRT->writeReg(&devRT->reg_hsc_amplificatoin_factor);
            setRegWritten(devRT, devRT->reg_hsc_amplificatoin_factor);
            if (compEnableReg > 0 && readRegVal(devRT, &devRT->reg_hsc_amplificatoin_factor) != ampl)
            {
                stdOutput(tr("Ошибка сравнения регистра коэффициента усиления ОУ"), tr("Comparison RT amplification factor wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }
        if (loadSPI == "ОУ")
        {
            writeSPI(devRT, dataSPI);
            if (!checkSPI(devRT))
            {
                errorsBefore++;
                if (checkFinSPI == 2)
                {
                     setErrorsBeforeCycle(errorsBefore);
                     return;
                }
            }
        }

        if (rtaddr <= MAX_RT_ADDR)
        {
            quint32 addr = rtaddr;
            devRT->reg_aux_rtaddr.setData(addr);
            devRT->writeReg(&devRT->reg_aux_rtaddr);
            if ((compEnableReg > 0 || compEnableRTA > 0) && readRegVal(devRT, &devRT->reg_aux_rtaddr) != rtaddr)
            {
                stdOutput(tr("Ошибка сравнения регистра адреса ОУ"), tr("Comparison RTA register wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                if (compEnableReg == 2 || compEnableRTA == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }

        if (compEnableRTA > 0 && !broadcast) // окончательная проверка адреса ОУ
        {
            REG_HSC_cfg cfg;
            int addr_dst = getRtaFromCommand(trmData);  // адрес отправки пакета
            bool check_aux_rtaddr = (devRT->reg_hsc_cfg.rtavsk_ena == 0);
            int addr_real = devRT->reg_hsc_cfg.rtavsk;
            if (!initEnable && !writeCfg && !checkLoadCfg)  // если конфигурационный регистр загружали или проверяли, то считаеи, что текущее значение правильное
            {
                devRT->readReg(&cfg);
                check_aux_rtaddr = (cfg.rtavsk_ena == 0);
                addr_real = cfg.rtavsk;
            }
            if (check_aux_rtaddr) // надо проверить вспомогательный регистр
            {
                REG_AUX_rtaddr reg;
                devRT->readReg(&reg);
                addr_real = reg.addr;
            }
            if (addr_real != addr_dst)
            {
                if ((rtaddr != addr_dst) || (compEnableReg == 0 && compEnableRTA == 0)) // иначе сообщение о несовпадении уже было при загрузке и сравнении регистра адреса
                {
                    stdOutput(tr("Ошибка сравнения адреса ОУ"), tr("Comparison RT address wrong"));
                    errorsBefore++;
                    emit statsOutputReady("errCompare", 1);
                }
                if (compEnableRTA == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }

        test.setConfigFlds(devRT->reg_hsc_cfg.type_man, devRT->reg_hsc_cfg.ena_codec!=0);
    }
    if (devBC)
    {
        if (initEnable)
        {
            qDebug() << "start BC configuration";
            if (writeCfg)
            {
                devBC->configuration.setChecked(config_NUMREG_cfg, false);
            }
            QByteArray regBC = devBC->configuration.getRegistersToWrite();
            if (regBC.size() > 0)
            {
                QByteArray regs;
                regs.append(regBC);
                devBC->write_F1(regs.data(), regs.size());
                devBC->configuration.doneWriteReg(regBC);
            }
            if (compEnableReg > 0 && regBC.size() > 0)
            {
                QByteArray addrArray, valArray;
                if (splitOddEven(regBC, addrArray, valArray))
                {
                    // Чтение регистров КШ для сравнения
                    QByteArray readArrayC;
                    readArrayC.resize(addrArray.size());
                    devBC->read_F1(addrArray.data(), readArrayC.data(), addrArray.size());

                    if (readArrayC == valArray)
                    {
                        stdOutput(tr("Сравнение регистров КШ успешно"), tr("Comparison BC registers OK"));
                    }
                    else
                    {
                        stdOutput(tr("Ошибка конфигурации КШ: регистры не совпали"), tr("Comparison BC registers wrong"));
                        stdOutput(tr("Возвращенный размер = %1").arg(readArrayC.size()), tr("Returned size = %1").arg(readArrayC.size()));
                        stdOutput(tr("Возвращенные данные = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                  tr("Returned data = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));
                        errorsBefore++;
                        emit statsOutputReady("errCompare", 1);
                        if (compEnableReg == 2)
                        {
                            setErrorsBeforeCycle(errorsBefore);
                            return;
                        }
                    }
                }
            }
            checkCounters(devBC);
        }

        bool cfg_err = false;
        if (writeCfg)
        {
            devBC->writeReg(&devBC->reg_hsc_cfg);
            setRegWritten(devBC, devBC->reg_hsc_cfg);
            if ((checkLoadCfg || (compEnableReg > 0)) && devBC->reg_hsc_cfg.getData() != readRegVal(devBC, &devBC->reg_hsc_cfg))
                cfg_err = true;
        }
        else if (checkLoadCfg)
        {
            REG_HSC_cfg cfg;
            devBC->readReg(&cfg);
//            qDebug() << "BC cfg  " << cfg.ena_aru << " " << cfg.ena_codec << " " << cfg.ena_mem_vsk << " " << cfg.en_rt_bc_int << " "
//                     << cfg.rtavsk << " " << cfg.rtavsk_ena << " " << cfg.rt_bc << " " << cfg.type_man;
            if (cfg.rt_bc != CFG_MODE_BC || cfg.ena_codec != codec ||
                cfg.type_man != devBC->reg_hsc_cfg.type_man || (useInt && (cfg.en_rt_bc_int != devBC->reg_hsc_cfg.en_rt_bc_int)))
            {
                stdOutput(tr("Ошибка сравнения конфигурационного регистра КШ"), tr("Comparison BC cfg register wrong"));
                emit statsOutputReady("errCompare", 1);
                setErrorsBeforeCycle(++errorsBefore);
                return;
            }
            if (useInt && (cfg.en_rt_bc_int == 0))
            {
                useInt = false;
                stdOutput(tr("Использование прерывание КШ не разрешено"), tr("Interruption is not enabled in BC configuration"));
            }
        }
        // Еще одна возможность проверки разрешения использования прерывания КШ
        if (useInt && !writeCfg && !checkLoadCfg && initEnable && devBC->configuration.disInt())
        {
            useInt = false;
            stdOutput(tr("Использование прерывание КШ не разрешено"), tr("Interruption is not enabled in BC configuration"));
        }

        if (amplBC)
        {
            int ampl = (amplBC == 1) ? 0 : amplBC + 14;
            devBC->reg_hsc_amplificatoin_factor.rx_oe_ampl = (amplBC == 1) ? 0 : 1;
            devBC->reg_hsc_amplificatoin_factor.rx_ampl = ampl & FL_REG_AMPL_FACTOR_rx_ampl;
            devBC->writeReg(&devBC->reg_hsc_amplificatoin_factor);
            setRegWritten(devBC, devBC->reg_hsc_amplificatoin_factor);
            if (compEnableReg > 0 && readRegVal(devBC, &devBC->reg_hsc_amplificatoin_factor) != ampl)
            {
                stdOutput(tr("Ошибка сравнения регистра коэффициента усиления КШ"), tr("Comparison BC amplification factor wrong"));
                errorsBefore++;
                emit statsOutputReady("errCompare", 1);
                if (compEnableReg == 2)
                {
                    setErrorsBeforeCycle(errorsBefore);
                    return;
                }
            }
        }
        if (loadSPI == "КШ")
        {
            writeSPI(devBC, dataSPI);
            if (!checkSPI(devBC))
            {
                errorsBefore++;
                if (checkFinSPI == 2)
                {
                     setErrorsBeforeCycle(errorsBefore);
                     return;
                }
            }
        }
    }
    if (errorsBefore > 0)
    {
        emit statsOutputReady("errBefore", errorsBefore);
        emit statsOutputReady("totalErr", errorsBefore);
    }

    if (iterCycle != 0)
    {
        int statusBC, statusRT, statusRTBefore;
        if (devRT)
            statusRT = getStatusReg(devRT);

        if (devBC)
        {
            statusBC = getStatusReg(devBC);
            initStartBC();
            initTimeMeasure();

            for (uint it = 0; it!=iterCycle; it++)
            {
                bool errorOccured = false;
                if (pause_stop() == -1)
                {
                    averageSpeed();
                    return;
                }

                statusRTBefore = statusRT;

                if (BCtoRT)
                {
                    emit statsOutputReady("totalIter", 1);
                    // Запись данных в буфер передачи
                    devBC->write_F2(getBufTrm(statusBC), (char*)trmData, trm_size);
                    if (compEnableMemBCRT > 0 && it <= 1)
                    {
                        // Чтение данных из буфера передачи КШ для сравнения
                        QByteArray readArray1;
                        readArray1.resize(trm_size);
                        devBC->read_F2(getBufTrm(statusBC), trm_size, readArray1.data());

                        if (memcmp(readArray1.data(), trmData, trm_size) == 0)
                        {
                            stdOutput(tr("Сравнение буфера передачи КШ успешно"), tr("Comparison BC trm buffer OK"));
                        }
                        else
                        {
                            stdOutput(tr("Ошибка сравнения буфера передачи КШ"), tr("Comparison BC trm buffer wrong"));
                            stdOutput(tr("Длина данных = %1 байт").arg(readArray1.size()), tr("Returned size = %1").arg(readArray1.size()));
                            stdOutput(tr("Командное слово = %1").arg(*(int*)(readArray1.data()), 8, 16, QLatin1Char('0')),
                                      tr("Command word = %1").arg(*(int*)(readArray1.data()), 8, 16, QLatin1Char('0')));
                            emit statsOutputReady("errCompare", 1);
                            if (compEnableMemBCRT == 2)
                            {
                                setErrorsWithinCycle(true);
                                averageSpeed();
                                return;
                            }
                            errorOccured = true;
                        }
                    }

                    // Оконный режим
                    if(windowMode && devRT)
                    {
                        switchWindow(1);
                        thread()->msleep(delayTime);
                    }

                    // Старт обмена
//                    QTime curTime;
//                    if (timeCompose)
//                        curTime = QTime::currentTime();
                    QElapsedTimer curTime;
                    if (timeCompose)
                        curTime.start();

                    devBC->writeReg(&devBC->reg_hsc_creg);
                    int interruption = waitForInterruption(devBC, useInt, waitTime, &statusBC);

                    if (timeMeasure)
                    {
                        timeCounter++;
                        if (timeCompose)
                        {
                            //int t = curTime.msecsTo(QTime::currentTime());
                             int t = curTime.elapsed();
                            if (t > timeOverhead)
                                t -= timeOverhead;
                            totalTime += t;
                            if (timeMeasure == 1)
                                stdOutput(tr("Время передачи %1 мс, полное время %2 мс, данных %3 бит.").arg(t).arg(totalTime).arg(wholePackBits),
                                          tr("Transmission time = %1 ms, total time = %2 ms, data size = %3 bit.").arg(t).arg(totalTime).arg(wholePackBits));
                        }
                    }

                    if(windowMode && devRT)
                        switchWindow(0);

                    if (devRT)
                        statusRT = getStatusReg(devRT);

                    bool bNoIntBC = checkStatusRegBC(statusBC, interruption, it, &errorOccured);
                    bool bNoIntRT = false;

                    if (devRT)
                        bNoIntRT = checkStatusRegRT(statusRT, it, &errorOccured);

                    if ((bNoIntBC && noIntFatalBC) || (bNoIntRT && noIntFatalRT > 1))
                    {
                        setErrorsWithinCycle(true);
                        averageSpeed();
                        return;
                    }

                    checkCounters(devBC);

                    if (devRT)
                    {
                        checkCounters(devRT);

                        if (compEnableData>0 && !RTtoBC)
                        {
                            // Чтение данных из буфера приёма ОУ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(trm_size);
                            if (postponeTime > 0)
                                thread()->msleep(postponeTime);
                            devRT->read_F2(getBufRec(statusRTBefore), trm_size, readArrayC.data());
                            if (test.cmpPack((void*)(readArrayC.data()), trmData, trm_size/NUMBYTEINOFDMSYM -1, true))
                            {
                                if (it <= 1)
                                    stdOutput(tr("Сравнение буфера приёма ОУ успешно"), tr("Comparison RT rec buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера приёма ОУ"), tr("Comparison RT rec buffer wrong"));
                                stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                /*stdOutput(tr("Первое прочитанное слово = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')),
                                          tr("First read word = %1").arg(*(int*)(readArrayC.data()), 4, 16, QLatin1Char('0')));*/
                                emit statsOutputReady("errCompare", 1);
                                errorOccured = true;
                            }
                        }
                    }
                }  // BCtoRT
                else  //  здесь RTtoBC, не циркулярный возврат
                {
                    // Запись данных в буфер передачи ОУ для сравнения их потом с исходными тестовыми данными
                    if (devRT!=0 && (compEnableData!=0 || compEnableMemBCRT!=0))
                    {
                        char* pc = (char*)trmData;
                        int test_size = trm_size-sizeof(word32_t);
                        pc += sizeof(word32_t);
                        devRT->write_F2(getBufTrm(statusRTBefore) + sizeof(word32_t), pc, test_size);
                        if (compEnableMemBCRT && it <= 1)
                        {
                            // Чтение данных из буфера передачи ОУ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(test_size);
                            devRT->read_F2(getBufTrm(statusRTBefore) + sizeof(word32_t), test_size, readArrayC.data());
                            if (memcmp(readArrayC.data(), pc, test_size) == 0)
                            {
                                stdOutput(tr("Сравнение буфера передачи ОУ успешно"), tr("Comparison RT trm buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера передачи ОУ"), tr("Comparison RT trm buffer wrong"));
                                stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                                stdOutput(tr("Первое прочитанное слово = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')),
                                          tr("First read word = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')));
                                emit statsOutputReady("errCompare", 1);
                                if (compEnableMemBCRT == 2)
                                {
                                    setErrorsWithinCycle(true);
                                    return;
                                }
                                errorOccured = true;
                            }
                        }
                    }
                }

                if (RTtoBC)
                {
                    if (BCtoRT && devRT!=0)  // это циркулярный возврат
                    {
                        if (pauseTime > 0)
                            thread()->msleep(pauseTime);

                        // Переписываем данные из буфера приёма в буфер передачи
/*
                        int test_size = trm_size-sizeof(word32_t);
                        QByteArray readArrayW;
                        readArrayW.resize(test_size);
                        devRT->read_F2(getBufRec(statusRTBefore) + sizeof(word32_t), test_size, readArrayW.data());
                        devRT->write_F2(getBufTrm(statusRT) + sizeof(word32_t), readArrayW.data(), test_size);
*/
                        // теперь новая функция
                        devRT->cpyOnHard(getBufRec(statusRTBefore) + sizeof(word32_t), trm_size-sizeof(word32_t), getBufTrm(statusRT) + sizeof(word32_t));

                        statusRTBefore = statusRT;
                    }
                    devBC->write_F2(getBufTrm(statusBC), (char*)recData, rec_pk_size);
                    int addr_rx = getBufRec(statusBC);

                    // Оконный режим
                    if(windowMode && devRT)
                    {
                        switchWindow(1);
                        thread()->msleep(delayTime);
                    }

                    if (!BCtoRT)
                        emit statsOutputReady("totalIter", 1);

//                    QTime curTime;
//                    if (timeCompose)
//                        curTime = QTime::currentTime();
                    QElapsedTimer curTime;
                    if (timeCompose)
                        curTime.start();

                    // Старт обмена
                    devBC->writeReg(&devBC->reg_hsc_creg);

                    int interruption = waitForInterruption(devBC, useInt, waitTime, &statusBC);

                    if (timeMeasure)
                    {
                        timeCounter++;
                        if (timeCompose)
                        {
                            //int t = curTime.msecsTo(QTime::currentTime());
                            int t = curTime.elapsed();
                            if (t > timeOverhead)
                                t -= timeOverhead;
                            totalTime += t;
                            if (timeMeasure == 1)
                                stdOutput(tr("Время передачи %1 мс, полное время %2 мс, данных %3 бит.").arg(t).arg(totalTime).arg(wholePackBits),
                                          tr("Transmission time = %1 ms, total time = %2 ms, data size = %3 bit.").arg(t).arg(totalTime).arg(wholePackBits));
                        }
                    }

                    if(windowMode && devRT)
                        switchWindow(0);

                    if (devRT)
                        statusRT = getStatusReg(devRT);

                    bool bNoIntBC = checkStatusRegBC(statusBC, interruption, it, &errorOccured);
                    bool bNoIntRT = false;

                    if (devRT)
                        bNoIntRT = checkStatusRegRT(statusRT, it, &errorOccured);

                    if ((bNoIntBC && noIntFatalBC) || (bNoIntRT && noIntFatalRT > 1))
                    {
                        setErrorsWithinCycle(true);
                        return;
                    }

                    checkCounters(devBC);

                    if (devRT)
                    {
                        checkCounters(devRT);

                        if (compEnableData > 0 /* || проверка ответного слова */)
                        {
                            // Чтение данных из буфера приёма КШ для сравнения
                            QByteArray readArrayC;
                            readArrayC.resize(trm_size);
                            devBC->read_F2(addr_rx, trm_size, readArrayC.data());
                            if (test.cmpPack((void*)(readArrayC.data()), trmData, trm_size/NUMBYTEINOFDMSYM - 1, true))
                            {
                                if (it <= 1)
                                    stdOutput(tr("Сравнение буфера приёма КШ успешно"), tr("Comparison BC rec buffer OK"));
                            }
                            else
                            {
                                stdOutput(tr("Ошибка сравнения буфера приёма КШ"), tr("Comparison BC rec buffer wrong"));
                                stdOutput(tr("Длина данных = %1 байт").arg(readArrayC.size()), tr("Data size = %1").arg(readArrayC.size()));
                               /* stdOutput(tr("Первое прочитанное слово = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')),
                                          tr("First read word = %1").arg(*(int*)(readArrayC.data()), 8, 16, QLatin1Char('0')));*/
                                emit statsOutputReady("errCompare", 1);
                                errorOccured = true;
                            }
                        } //сравнение данных
                    }
                } // передача ОУ-КШ или циркулярный возврат

                if (errorOccured)
                {
                    emit statsOutputReady("totalErr", 1);
                    errorsWithin++;
                }

                if (pauseTime > 0)
                    thread()->msleep(pauseTime);
            } // цикл
            averageSpeed();

        } // devBC
    } // iterCycle != 0

    if (errorsBefore + errorsWithin)
        emit resultReady((int)AbstractTest::ErrorIsOccured);
    else
        emit resultReady((int)AbstractTest::Completed);
}

//lineEditSPIData->setInputMask(QApplication::translate("TestTrmSingle", "HHHH;_", 0));

void trmSingleObjToThread::setErrorsBeforeCycle(int errors)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    emit statsOutputReady("errBefore", errors);
    emit statsOutputReady("totalErr", errors);
}

void trmSingleObjToThread::setErrorsWithinCycle(bool fatal)
{
    emit resultReady((int)AbstractTest::ErrorIsOccured);
    if (fatal)
        emit statsOutputReady("errFatal", 1);
    emit statsOutputReady("totalErr", 1);
}

void trmSingleObjToThread::destroyData()
{
    if  (trmData)
        free (trmData);
    if  (recData)
        free (recData);
    //if  (testData)
      //  free (testData);
    trmData = 0;
    recData = 0;
    //testData = 0;
}

// Вывод средней скорости передачи данных
void trmSingleObjToThread::averageSpeed()
{
    if (timeMeasure == 0 || timeCounter == 0)
        return;

    if (timeMeasure == 3)
    {
        QTime endTime = QTime::currentTime();
        qDebug() << "Begin time: " << beginTime;
        qDebug() << "End time: " << endTime;
        long total = beginTime.msecsTo(endTime);
        stdOutput(tr("Полное время тестирования: %1 мсек., %2 итераций.").arg(total).arg(timeCounter), tr("Total test time: %1 ms, %2 iterations.").arg(total).arg(timeCounter));
        total /= timeCounter;
        stdOutput(tr("Среднее время итерации: %1 мсек.").arg(total), tr("Average iteration time: %1 ms.").arg(total));
        if (total > timeOverhead)
        {
            total -= timeOverhead;
            stdOutput(tr("Оценка времени передачи одного пакета: %1 мсек.").arg(total), tr("Estimated packege transmission time is %1 ms.").arg(total));
            stdOutput(tr("Всего данных в пакете %1 бит, полезных данных %4 бит, %2 %3.").arg(trm_size*8).arg(manipulation).arg(codec ? "с кодеком" : "без кодека").arg(wholePackBits),
                      tr("Total bits in package - %1, useful data - %4 bit, %2 %3.").arg(trm_size*8).arg(manipulation).arg(codec ? "with codec" : "w/o codec").arg(wholePackBits));
            long abps = wholePackBits * 1000 / totalTime;
            QString word = formattedLongInteger(abps);
            QString rus = tr("Оценка скорости передачи данных - ") + word + tr(" бит/с.");
            QString eng = tr("Estimated transmission speed = ") + word + tr(" bps.");
            stdOutput(rus, eng);
        }
        else
        {
            stdOutput(tr("Неверная поправка времени передачи."), tr("Wrong time correction in settings."));
            stdOutput(tr("Оценка скорости невозможна."), tr("Speed estimation is impossible."));
        }
    }
    else // timeMeasure равно 1 или 2 и здесь timeCounter > 0
    {
        double time_sec = ((double)totalTime) * 0.001 / timeCounter;
        stdOutput(tr("Среднее время передачи %1 бит равно %2 секунд.").arg(wholePackBits).arg(time_sec),
                  tr("Average transmission time for %1 bit is %2 seconds.").arg(wholePackBits).arg(time_sec));
        if (wholePackBits > 0 && time_sec > 0)
        {
            long abps = long(((long long)wholePackBits) * timeCounter * 1000 / totalTime);
            QString word = formattedLongInteger(abps);
            QString rus = tr("Оценка скорости передачи данных - ") + word + tr(" бит/с.");
            QString eng = tr("Estimated transmission speed = ") + word + tr(" bps.");
            stdOutput(rus, eng);
        }
        else if (timeMeasure > 1)
            stdOutput(tr("Оценка скорости невозможна."), tr("Speed estimation is impossible."));
        stdOutput(tr("Всего данных в пакете %1 бит, %2 %3, %4 итераций.").arg(trm_size*8).arg(manipulation).arg(codec ? "с кодеком" : "без кодека").arg(timeCounter),
                  tr("Total bits in package - %1, %2 %3, %4 iterations.").arg(trm_size*8).arg(manipulation).arg(codec ? "with codec" : "w/o codec").arg(timeCounter));
    }
}

void trmSingleObjToThread::initTimeMeasure()
{
    if (timeMeasure)
    {
        totalTime = 0;
        timeCounter = 0;
        beginTime = QTime::currentTime();
        wholePackBits = trm_size * nwrd * 8 / NUMWORDINOFDMSYM - 32; //  размер пакета с данными минус командное слово
    //  wholePackBits += (nwrd * sizeof(word32_t) * 8);  // плюс ответное слово - сейчас считаем это накладными расходами и не включаем в подсчёт данных
    }
}
