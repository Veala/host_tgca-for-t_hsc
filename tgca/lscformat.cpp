#include "lscformat.h"

void C_LSC_Format::addMessage(int &addrStackRT, int &addrBC)
{
    QByteArray arr;
    addControlWord(arr);
    addCommandWord(arr);
    addDataBC(arr, addrBC);
    devBC->write_F2(0x4000+addrBC*4, arr.data(), arr.size());
    curCommandStackRT = addrStackRT;
    addDataRT(addrStackRT);
    addrBC += length();
}

word32_t C_LSC_Format::readRW(word32_t addr)
{
    word32_t answer;
    devBC->read_F2(0x4000+addr*4, sizeof(word32_t), (char*)(&answer));
    return answer;
}

void C_LSC_Format::writeRam1Word(word32_t addr, word32_t val)
{
    devBC->write_F2(0x4000+addr*4, (char*)(&val), sizeof(word32_t));
}

bool C_LSC_Format::check()
{
    return checkRW() && checkData();
}

void C_LSC_Format::readMemBlkRT(int addr, word32_t* buf, int num_words)
{
    devRT->read_F2(0x4000+addr*4, num_words*4, (char*)(buf));
}

/////////////////////////////////////////////////////////////////////////

C_LSC_Format1::C_LSC_Format1(int rtaddr, int saddr, int num, word32_t val, bool my):
    C_LSC_Format(),
    sa(saddr),
    nw(num),
    dataVal(val),
    own(my)
{
    rta = own ? rtaddr : (rtaddr == 2 ? 1 : 2);
}

void C_LSC_Format1::addDataBC(QByteArray &arr, int &addrBC)
{
    for (int i=0; i<nw; i++)
        arr.append((char*)dataVal, 4);
    arr.append((char*)initVal, 4);
    arr.append((char*)initVal, 4);
    addrRW = addrBC+nw+3;
    //expectedRW = own ? initVal : (rta<<11);
}

void C_LSC_Format1::spoilRW()
{
    writeRam1Word(addrRW, initVal);
}

void C_LSC_Format1::spoilData()
{
    word32_t arr_spoiler[64];
    for (int i=0; i<64; i++)
        arr_spoiler[i] = initVal;
    devRT->write_F2(0x4000+getSABufRec(sa)*4, (char*)(arr_spoiler), 64*sizeof(word32_t));
}

bool C_LSC_Format1::checkData()
{
    if (own)
    {
        word32_t pStackRT[4];
        readMemBlkRT(curCommandStackRT, pStackRT, 4);
        if (pStackRT[0]!=0xA000 || pStackRT[3]!=(rta<<11))
            return false;

        // сравнить данные
        word32_t pData = pStackRT[2];
        word32_t data[32];
        readMemBlkRT(pData, data, nw);
        for (int i=0; i<nw; i++)
            if (data[i] != dataVal)
                return false;
    }
    return true;
}

void C_LSC_Format1::addDataRT(int &addrStackRT)
{
    if (own)
        addrStackRT+=4;
}
