#ifndef DEVICE_H
#define DEVICE_H

#define ADDR_MEM2P_BUF_TX_0 0x40000
#define ADDR_MEM2P_BUF_TX_1 0x80000
#define ADDR_MEM2P_BUF_RX_0 0x60000
#define ADDR_MEM2P_BUF_RX_1 0xA0000
#define ADDR_MEM2P_DELTA    0x200  //128words*4b=d512b=h200
#define VAL_reg_hsc_cfg_type_man_QPSK   0x00
#define VAL_reg_hsc_cfg_type_man_QAM16  0x01
#define VAL_reg_hsc_cfg_type_man_QAM64  0x10

#include <QFrame>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QTextBrowser>
#include <QVector>
#include <QByteArray>
#include "connection.h"
#include "configuration.h"
#include "funclib.h"
#include "data_structs.h"

namespace Ui {
class Device;
}

class Device : public QFrame
{
    Q_OBJECT

signals:
    void error(QAbstractSocket::SocketError err);

    //signals to tests
    void sigDelete(QString);
    void sigConnectedDevice();
    void sigDisconnectedDevice();

public:
    explicit Device(QWidget *parent = 0, QString name = "default", QTextBrowser *tB = NULL);
    ~Device();

    Connection connection;
    Configuration configuration;

    bool isMonitor();
    void setName(QString);
    QString getName() const;
    void setSocket(QTcpSocket*);

    void write_F1(char* writeArray, int size);
    void write_F2(int startAddr, char* writeArray, int size);
    void read_F1(char *writeArray, char *readArray, int wrsize);
    void read_F2(int startAddr, int count, char *readArray);
    void write_Echo(QString& text);
    void writeReg(BaseReg* reg);
    void readReg(BaseReg* reg);
    void writeRegs(QVector<BaseReg*>& regs);
    void readRegs(QVector<BaseReg*>& regs);
    void sendDataToRT(quint32 addrRT, char *writeArray, int size);
    void recvDataFromRT(quint32 addrRT, char *writeArray, int size);
    void writeDataToMem(char *writeArray, int size);
    REG_HSC_ram_tx_rx reg_hsc_ram_tx_rx; REG_HSC_id reg_hsc_id; REG_HSC_status reg_hsc_status; REG_HSC_cfg reg_hsc_cfg; REG_HSC_tx_cntr reg_hsc_tx_cntr; REG_HSC_rx_cntr reg_hsc_rx_cntr;
    REG_HSC_creg reg_hsc_creg; REG_HSC_cr_spi reg_hsc_cr_spi; REG_HSC_dr_spi_msw reg_hsc_dr_spi_msw; REG_HSC_dr_spi_lsw reg_hsc_dr_spi_lsw; REG_HSC_time_rsp reg_hsc_time_rsp;
    REG_HSC_cnt_pct_tx_msw reg_hsc_cnt_pct_tx_msw; REG_HSC_cnt_pct_tx_lsw reg_hsc_cnt_pct_tx_lsw; REG_HSC_cnt_pct_rx_msw reg_hsc_cnt_pct_rx_msw; REG_HSC_cnt_pct_rx_lsw reg_hsc_cnt_pct_rx_lsw;
    REG_HSC_lvl_sync_kf_rx_msw reg_hsc_lvl_sync_kf_rx_msw; REG_HSC_lvl_sync_kf_rx_lsw reg_hsc_lvl_sync_kf_rx_lsw; REG_HSC_prcs_max_sync_msw reg_hsc_prcs_max_sync_msw; REG_HSC_prcs_max_sync_lsw reg_hsc_prcs_max_sync_lsw;
    REG_HSC_lvl_sync_pre_rx_msw reg_hsc_lvl_sync_pre_rx_msw; REG_HSC_lvl_sync_pre_rx_lsw reg_hsc_lvl_sync_pre_rx_lsw; REG_HSC_prs_level_max_Rn_msw reg_hsc_prs_level_max_Rn_msw; REG_HSC_prs_level_max_Rn_lsw reg_hsc_prs_level_max_Rn_lsw;
    REG_HSC_lvl_qam16 reg_hsc_lvl_qam16; REG_HSC_lvl_qam64_low reg_hsc_lvl_qam64_low; REG_HSC_lvl_qam64_midle reg_hsc_lvl_qam64_midle; REG_HSC_lvl_qam64_high reg_hsc_lvl_qam64_high;
    REG_HSC_amplificatoin_factor reg_hsc_amplificatoin_factor; REG_HSC_amplitude_signal reg_hsc_amplitude_signal; REG_HSC_g_sp reg_hsc_g_sp; REG_HSC_g_1_sp_high reg_hsc_g_1_sp_high; REG_HSC_g_1_sp_low reg_hsc_g_1_sp_low; REG_HSC_pll_reg reg_hsc_pll_reg;
    REG_AUX_rtaddr reg_aux_rtaddr; REG_AUX_interruption reg_aux_interruption; REG_AUX_winmode reg_aux_winmode; REG_AUX_bulb reg_aux_bulb;

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::Device *ui;
    QMenu menu;
    QTextBrowser* projectBrowser;
    void message(QString);

    //CMDHead head;
    QTcpSocket* sock;
    void readAll(char* array, int size);
    void writeAll(char* array, int size);
    QByteArray littleAnswer;

    int BaseRegLen = sizeof(BaseReg);
    int RegLen = 2*sizeof(BaseReg);
    CommandWord commandWord;
    ResponseWord responseWord;
    int symbolLength(); //in bytes

    int maxNumSymbols = 256;


private slots:
    void checkDevice();
    void doError(QAbstractSocket::SocketError err);
};

#endif // DEVICE_H
