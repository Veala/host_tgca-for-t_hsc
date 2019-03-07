#include "nulltest.h"
#include "../registers.h"

#include "../testutil.h"
#include "../ctestbc.h"

void NullTest::setSettings(QVBoxLayout *b, QDialog *d, bool ch, QString tType, QString fName, QString markStr, QTextBrowser *pB, QTextBrowser *tB, QWidget *d2)
{
    AbstractTest::setSettings(b,d,ch,tType,fName,markStr,pB,tB,d2);

    changeConnections();

    defineFields();
    load(fName);

    radioButtonUnlimited->setChecked(!radioButtonEnter->isChecked());
    onRadioCycle();

    addDevicesTolist();

    connect(radioButtonUnlimited, SIGNAL(toggled(bool)), this, SLOT(onRadioCycle()));

    objToThread = new nullObjToThread();

    connectThread();

    statsMap.insert("totalIter", stats->findChild<QLabel*>("totalIter"));
    statsMap.insert("errData", stats->findChild<QLabel*>("errData"));
    statsMap.insert("errConnect", stats->findChild<QLabel*>("errConnect"));
    statsMap.insert("errOther", stats->findChild<QLabel*>("errOther"));
    connectStatisticSlots();

    testThread.start();
}

void NullTest::onRadioCycle()
{
    spinBoxCycle->setEnabled(radioButtonEnter->isChecked());
}

void NullTest::defineFields()
{
    // генерация данных
    dataGen.settings();
    // зацикливание
    radioButtonUnlimited = settings->findChild<QRadioButton*>("radioButtonUnlimited");
    radioButtonEnter = settings->findChild<QRadioButton*>("radioButtonEnter");
    spinBoxCycle = settings->findChild<QSpinBox*>("spinBoxCycle");
    // пауза между итерациями
    lineEditPause = settings->findChild<QLineEdit*>("lineEditPause");
    // устройства
    lineEditDevBC = settings->findChild<QLineEdit*>("lineEditDevBC");
    lineEditDevRT = settings->findChild<QLineEdit*>("lineEditDevRT");
    checkBoxDevBC = settings->findChild<QCheckBox *>("checkBoxDevBC");
    checkBoxDevRT = settings->findChild<QCheckBox *>("checkBoxDevRT");
    checkBoxConnBC = settings->findChild<QCheckBox *>("checkBoxConnBC");
    checkBoxConnRT = settings->findChild<QCheckBox *>("checkBoxConnRT");
    // вывод
    comboBoxOut = settings->findChild<QComboBox*>("comboBoxOut");
    // параметры заданий
    lineEditSleepTime = settings->findChild<QLineEdit*>("lineEditSleepTime");
    lineEditErrBit = settings->findChild<QLineEdit*>("lineEditErrBit");
    comboBoxManType = settings->findChild<QComboBox*>("comboBoxManType");
    checkBoxCodec = settings->findChild<QCheckBox*>("checkBoxCodec");
}

void NullTest::load(QString fName)
{
top_1
        QString genDataType = out.readLine();
        QString genDataBegin = out.readLine();
        QString genDataStep = out.readLine();
        QString genDataNumStep = out.readLine();
        QString genDataUnit = out.readLine();
        QString genDataLen = out.readLine();
        dataGen.init(genDataType, genDataBegin, genDataStep, genDataNumStep, genDataUnit, genDataLen);

        radioButtonEnter->setChecked(!out.readLine().isEmpty());
        spinBoxCycle->setValue(out.readLine().toInt(0,16));
        lineEditPause->setText(out.readLine());
        lineEditDevBC->setText(out.readLine());
        checkBoxDevBC->setChecked(!out.readLine().isEmpty());
        checkBoxConnBC->setChecked(!out.readLine().isEmpty());
        lineEditDevRT->setText(out.readLine());
        checkBoxDevRT->setChecked(!out.readLine().isEmpty());
        checkBoxConnRT->setChecked(!out.readLine().isEmpty());
        comboBoxOut->setCurrentText(out.readLine());

        comboBoxManType->setCurrentText(out.readLine());
        checkBoxCodec->setChecked(!out.readLine().isEmpty());
        lineEditSleepTime->setText(out.readLine());
        lineEditErrBit->setText(out.readLine());
}

void NullTest::save()
{
    AbstractTest::save();
top_2(saveFileNameStr)

    in << dataGen.genType() << endl;
    in << dataGen.begin() << endl;
    in << dataGen.step() << endl;
    in << dataGen.numStep() << endl;
    in << dataGen.unit() << endl;
    in << dataGen.dataLen() << endl;
    in << (radioButtonEnter->isChecked() ? "1" : "") << endl;
    in << spinBoxCycle->text() << endl;
    in << lineEditPause->text() << endl;
    in << lineEditDevBC->text() << endl;
    in << (checkBoxDevBC->isChecked() ? "1" : "") << endl;
    in << (checkBoxConnBC->isChecked() ? "1" : "") << endl;
    in << lineEditDevRT->text() << endl;
    in << (checkBoxDevRT->isChecked() ? "1" : "") << endl;
    in << (checkBoxConnRT->isChecked() ? "1" : "") << endl;
    in << comboBoxOut->currentText() << endl;
    in << comboBoxManType->currentText() << endl;
    in << (checkBoxCodec->isChecked() ? "1" : "") << endl;
    in << lineEditSleepTime->text() << endl;
    in << lineEditErrBit->text() << endl;

    settingsFile.close();
}

void NullTest::startTest()
{
    nullObjToThread* curThread = (nullObjToThread*)objToThread;

    curThread->iterCount = radioButtonEnter->isChecked() ? spinBoxCycle->value() : -1;
    curThread->iterTime = lineEditPause->text().toInt();
    curThread->bConnectBC = checkBoxConnBC->isChecked();
    curThread->bConnectRT = checkBoxConnRT->isChecked();

    curThread->devBC = deviceList.at(0);
    curThread->devRT = deviceList.at(0);

    int outMode = comboBoxOut->currentIndex();
    curThread->bDebugOut = (outMode == 3);
    curThread->setOutEnabled(outMode%2);

    curThread->timeTest = !lineEditSleepTime->text().isEmpty();
    curThread->timeSleep = curThread->timeTest ? lineEditSleepTime->text().toInt() : 0;
    curThread->compTest = !lineEditErrBit->text().isEmpty();
    curThread->wrongBit = curThread->compTest ? lineEditErrBit->text().toInt() : 0;
    curThread->manType = comboBoxManType->currentIndex();
    curThread->codec = checkBoxCodec->isChecked();

    curThread->testData = dataGen.createData(dataGen.getDataLen(), 2);

    int index = 0;
    if (checkBoxDevBC->isChecked())
    {
        curThread->devBC = deviceList.at(0);
        index ++;
    }
    else
        curThread->devBC = 0;
    if (checkBoxDevRT->isChecked())
    {
        curThread->devRT = deviceList.at(index);
        index ++;
    }
    else
        curThread->devRT = 0;

    emit startTestTh();
}

void NullTest::setEnabledSpecial(bool)
{
}

/////////////////////////////////////////////////////////////////////////

void nullObjToThread::stdOutput(QString message_rus, QString message_eng)
{
    if (outMode%2)    // 1 и 3 - вывод в окно тестов
        emit outputReady(message_rus);
    else if (outMode) //  0 - нет вывода, 2 - вывод на консоль
        qDebug() << message_eng;
}

void nullObjToThread::perform()
{
    emit statsOutputReady("totalIter", 1);
    int errCounter = 0;

    if (testData == 0)
    {
        errCounter++;
        emit statsOutputReady("errData", 1);
    }

    stdOutput(QObject::tr("Тест тестов запущен"), tr("Null-test started"));
    if (bConnectBC)
    {
        if (devBC)
        {
            AbstractTest::RunningState ans = connectBC();
            switch (ans)
            {
            case AbstractTest::Running:
                break;
            case AbstractTest::ErrorIsOccured:
                emit statsOutputReady("errConnect", 1);
            default:
                stdOutput(tr("Непредусмотренный код ответа connectBC(): %1").arg(ans), tr("Invalid return from connectBC(): %1").arg(ans));
                emit statsOutputReady("errOther", 1);
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                if (testData)
                    free (testData);
                return;
            }
        }
        else
        {
            emit statsOutputReady("errOther", 1);
        }
    }

    if (bConnectRT)
    {
        if (devRT)
        {
            AbstractTest::RunningState ans = connectRT();
            switch (ans)
            {
            case AbstractTest::Running:
                break;
            case AbstractTest::ErrorIsOccured:
                emit statsOutputReady("errConnect", 1);
            default:
                stdOutput(tr("Непредусмотренный код ответа connectRT(): %1").arg(ans), tr("Invalid return from connectRT(): %1").arg(ans));
                emit statsOutputReady("errOther", 1);
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                if (testData)
                    free (testData);
                return;
            }
        }
        else
        {
            emit statsOutputReady("errOther", 1);
            emit resultReady((int)AbstractTest::ErrorIsOccured);
            if (testData)
                free (testData);
            return;
        }
    }

    for (int iter=0; iter!=iterCount; iter++)
    {
        if (iterTime > 0)
            thread()->msleep(iterTime);

        if (bDebugOut && (iter%20 == 0))
            stdOutput(tr("Итерация %1").arg(iter), tr("Iter = %1").arg(iter));
        qDebug() << "iter " << iter;

   /*     QTime curTime;
        curTime.start(); // = QTime::currentTime();
        qDebug() << "Time before: " << curTime;

        int time1 =  curTime.msecsSinceStartOfDay();

        qDebug() << "time1 = " << time1;
        qDebug() << "Time now: " << curTime;

        thread()->msleep(5);

          QTime newTime = QTime::currentTime();
          qDebug() << "Time after: " << newTime;
          int t1 = curTime.msecsTo(newTime); //(QTime::currentTime());
          int t2 = curTime.elapsed();
            int time2 =  newTime.msecsSinceStartOfDay();
            int diff = time2 >= time1 ? time2-time1 : -1;

            qDebug() << "msecsTo = " << t1;
            qDebug() << "elapsed = " << t2;
            qDebug() << "time2 = " << time2;
            qDebug() << "diff = " << diff;*/

        if (timeTest)
        {
        }

        if (compTest && testData!=0)
        {
            char trmBuf[MAXPACKAGESIZE];
            CTestBC test;
            test.setConfigFlds(manType, codec);
            int mb = test.maxNumByte();
            if (!test.createCommandPack((void*)trmBuf, MAXPACKAGESIZE, testData, mb, 0, tgca_tr_REC, 0))
            {
                stdOutput(tr("Ошибка создания командного пакета"), tr("Command pack creation error"));
                emit statsOutputReady("otherErr", 1);
                emit resultReady((int)AbstractTest::ErrorIsOccured);
                    free (testData);
                return;
            }
            // портим несколько бит
            for (int i=0; i<wrongBit && i<256; i++)
            {
                trmBuf[i*NUMBYTEINOFDMSYM + 8] = trmBuf[i*NUMBYTEINOFDMSYM + 8]  ^ (char)1;
            }
            int err_1, err_eq, err_gt;
            bool b_1  = test.cmpPackDataBit(trmBuf, testData, mb, &err_1, 1 );
            qDebug() << 1 << " " << err_1 << " " << b_1;
            bool b_eq = test.cmpPackDataBit(trmBuf, testData, mb, &err_eq, wrongBit+1 );
            qDebug() << wrongBit+1 << " " << err_eq << " " << b_eq;
            if (wrongBit > 2)
            {
                bool b_gt = test.cmpPackDataBit(trmBuf, testData, mb, &err_gt, wrongBit-1 );
                qDebug() << wrongBit-1 << " " << err_gt << " " << b_gt;
            }
        }

        if (pause_stop() == -1)
        {
            if (testData)
                free (testData);
            return;
        }
    }
    if (testData)
        free (testData);
    emit resultReady(errCounter == 0 ? (int)(AbstractTest::Completed) : (int)(AbstractTest::ErrorIsOccured));
}
