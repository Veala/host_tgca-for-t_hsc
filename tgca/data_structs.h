#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#include <QtGlobal>
#include "device.h"

class BaseReg
{
public:
    BaseReg(quint32 address) : address(address) {  }
    const quint32 address;
    void setData(quint32& data) {
        *((quint32*)this+1) = data;
    }
    quint32 getData() {
        return *((quint32*)this+1);
    }
};

class REG_HSC_ram_tx_rx : public BaseReg
{
public:
    REG_HSC_ram_tx_rx(quint32 buf_1 = 0, quint32 buf_2 = 0, quint32 preamble_1_mem = 0, quint32 preamble_2_mem = 0, quint32 one_per_HF = 0, quint32 MSEQ = 0, quint32 K_per_M = 0, quint32 KF = 0, quint32 mem_QAM16_QAM64 = 0) : BaseReg(0x80),
        buf_1(buf_1), buf_2(buf_2), preamble_1_mem(preamble_1_mem), preamble_2_mem(preamble_2_mem), one_per_HF(one_per_HF), MSEQ(MSEQ), K_per_M(K_per_M), KF(KF), mem_QAM16_QAM64(mem_QAM16_QAM64) { }
    quint32 buf_1 : 1;
    quint32 buf_2 : 1;
    quint32 preamble_1_mem : 1;
    quint32 preamble_2_mem : 1;
    quint32 one_per_HF : 1;
    quint32 MSEQ : 1;
    quint32 K_per_M : 1;
    quint32 KF : 1;
    quint32 mem_QAM16_QAM64 : 1;
};

class REG_HSC_id : public BaseReg
{
public:
    REG_HSC_id(quint32 data = 0) : BaseReg(0x88),
        data(data) { }
    quint32 data;
};

class REG_HSC_status : public BaseReg
{
public:
    REG_HSC_status(quint32 rt_bc_int = 0, quint32 rs_err = 0, quint32 no_aw_err = 0, quint32 yes_aw_gr_err = 0, quint32 tx_num_buf = 0, quint32 rx_num_buf = 0) : BaseReg(0x8C),
        rt_bc_int(rt_bc_int), rs_err(rs_err), no_aw_err(no_aw_err), yes_aw_gr_err(yes_aw_gr_err), tx_num_buf(tx_num_buf), rx_num_buf(rx_num_buf) { }
    quint32 rt_bc_int : 1;
    quint32 : 3;
    quint32 rs_err : 1;
    quint32 no_aw_err : 1;
    quint32 yes_aw_gr_err : 1;
    quint32 : 3;
    quint32 tx_num_buf : 1;
    quint32 rx_num_buf : 1;
};

class REG_HSC_cfg : public BaseReg
{
public:
    REG_HSC_cfg(quint32 type_man = 0, quint32 ena_codec = 0, quint32 ena_aru = 0, quint32 ena_mem_vsk = 0, quint32 rtavsk_ena = 0, quint32 rtavsk = 0, quint32 rt_bc = 0, quint32 en_rt_bc_int = 0) : BaseReg(0x90),
        type_man(type_man), ena_codec(ena_codec), ena_aru(ena_aru), ena_mem_vsk(ena_mem_vsk), rtavsk_ena(rtavsk_ena), rtavsk(rtavsk), rt_bc(rt_bc), en_rt_bc_int(en_rt_bc_int) { }
    quint32 type_man : 2;
    quint32 ena_codec : 1;
    quint32 ena_aru : 1;
    quint32 ena_mem_vsk : 1;
    quint32 : 1;
    quint32 rtavsk_ena : 1;
    quint32 rtavsk : 5;
    quint32 rt_bc : 1;
    quint32 en_rt_bc_int : 1;
};

class REG_HSC_tx_cntr : public BaseReg
{
public:
    REG_HSC_tx_cntr(quint32 data = 0) : BaseReg(0x94),
        data(data) { }
    quint32 data;
};

class REG_HSC_rx_cntr : public BaseReg
{
public:
    REG_HSC_rx_cntr(quint32 max_Rn_sync_pre = 0, quint32 prcs_max_sync__sync_kf = 0) : BaseReg(0x9C),
        max_Rn_sync_pre(max_Rn_sync_pre), prcs_max_sync__sync_kf(prcs_max_sync__sync_kf) { }
    quint32 max_Rn_sync_pre : 1;
    quint32 prcs_max_sync__sync_kf : 1;
};

class REG_HSC_creg : public BaseReg
{
public:
    REG_HSC_creg(quint32 com_res = 0, quint32 tx_res = 0, quint32 rx_res = 0, quint32 start_bc = 0) : BaseReg(0xA0),
        com_res(com_res), tx_res(tx_res), rx_res(rx_res), start_bc(start_bc) { }
    quint32 com_res : 1;
    quint32 tx_res : 1;
    quint32 rx_res : 1;
    quint32 : 1;
    quint32 start_bc : 1;
};

class REG_HSC_cr_spi : public BaseReg
{
public:
    REG_HSC_cr_spi(quint32 spi_en = 0, quint32 dr8_16_32 = 0, quint32 spif = 0) : BaseReg(0xA4),
        spi_en(spi_en), dr8_16_32(dr8_16_32), spif(spif) { }
    quint32 spi_en : 2;
    quint32 : 6;
    quint32 dr8_16_32 : 2;
    quint32 : 5;
    quint32 spif : 1;
};

class REG_HSC_dr_spi_msw : public BaseReg
{
public:
    REG_HSC_dr_spi_msw(quint32 data = 0) : BaseReg(0xA8),
        data(data) { }
    quint32 data;
};

class REG_HSC_dr_spi_lsw : public BaseReg
{
public:
    REG_HSC_dr_spi_lsw(quint32 data = 0) : BaseReg(0xAC),
        data(data) { }
    quint32 data;
};

class REG_HSC_time_rsp : public BaseReg
{
public:
    REG_HSC_time_rsp(quint32 time = 0) : BaseReg(0xB4),
        time(time) { }
    quint32 time;
};

class REG_HSC_cnt_pct_tx_msw : public BaseReg
{
public:
    REG_HSC_cnt_pct_tx_msw(quint32 counter = 0) : BaseReg(0xB8),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_cnt_pct_tx_lsw : public BaseReg
{
public:
    REG_HSC_cnt_pct_tx_lsw(quint32 counter = 0) : BaseReg(0xBC),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_cnt_pct_rx_msw : public BaseReg
{
public:
    REG_HSC_cnt_pct_rx_msw(quint32 counter = 0) : BaseReg(0xC0),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_cnt_pct_rx_lsw : public BaseReg
{
public:
    REG_HSC_cnt_pct_rx_lsw(quint32 counter = 0) : BaseReg(0xC4),
        counter(counter) { }
    quint32 counter;
};

class REG_HSC_lvl_sync_kf_rx_msw : public BaseReg
{
public:
    REG_HSC_lvl_sync_kf_rx_msw(quint32 top = 0) : BaseReg(0xC8),
        top(top) { }
    quint32 top;
};

class REG_HSC_prcs_max_sync_msw : public BaseReg
{
public:
    REG_HSC_prcs_max_sync_msw(quint32 top = 0) : BaseReg(0xC8),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_sync_kf_rx_lsw : public BaseReg
{
public:
    REG_HSC_lvl_sync_kf_rx_lsw(quint32 top = 0) : BaseReg(0xCC),
        top(top) { }
    quint32 top;
};

class REG_HSC_prcs_max_sync_lsw : public BaseReg
{
public:
    REG_HSC_prcs_max_sync_lsw(quint32 top = 0) : BaseReg(0xCC),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_sync_pre_rx_msw : public BaseReg
{
public:
    REG_HSC_lvl_sync_pre_rx_msw(quint32 top = 0) : BaseReg(0xD0),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_sync_pre_rx_lsw : public BaseReg
{
public:
    REG_HSC_lvl_sync_pre_rx_lsw(quint32 top = 0) : BaseReg(0xD4),
        top(top) { }
    quint32 top;
};

class REG_HSC_prs_level_max_Rn_msw : public BaseReg
{
public:
    REG_HSC_prs_level_max_Rn_msw(quint32 top = 0) : BaseReg(0xD0),
        top(top) { }
    quint32 top;
};

class REG_HSC_prs_level_max_Rn_lsw : public BaseReg
{
public:
    REG_HSC_prs_level_max_Rn_lsw(quint32 top = 0) : BaseReg(0xD4),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam16 : public BaseReg
{
public:
    REG_HSC_lvl_qam16(quint32 top = 0) : BaseReg(0xD8),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam64_low : public BaseReg
{
public:
    REG_HSC_lvl_qam64_low(quint32 top = 0) : BaseReg(0xDC),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam64_midle : public BaseReg
{
public:
    REG_HSC_lvl_qam64_midle(quint32 top = 0) : BaseReg(0xE0),
        top(top) { }
    quint32 top;
};

class REG_HSC_lvl_qam64_high : public BaseReg
{
public:
    REG_HSC_lvl_qam64_high(quint32 top = 0) : BaseReg(0xE4),
        top(top) { }
    quint32 top;
};

class REG_HSC_amplificatoin_factor : public BaseReg
{
public:
    REG_HSC_amplificatoin_factor(quint32 rx_ampl = 0, quint32 rx_oe_ampl = 0) : BaseReg(0xE8),
        rx_ampl(rx_ampl), rx_oe_ampl(rx_oe_ampl) { }
    quint32 rx_ampl : 4;
    quint32 rx_oe_ampl : 1;
};

class REG_HSC_amplitude_signal : public BaseReg
{
public:
    REG_HSC_amplitude_signal(quint32 amplitude = 0) : BaseReg(0xEC),
        amplitude(amplitude) { }
    quint32 amplitude;
};

class REG_HSC_g_sp : public BaseReg
{
public:
    REG_HSC_g_sp(quint32 data = 0) : BaseReg(0xF0),
        data(data) { }
    quint32 data;
};

class REG_HSC_g_1_sp_high : public BaseReg
{
public:
    REG_HSC_g_1_sp_high(quint32 data = 0) : BaseReg(0xF4),
        data(data) { }
    quint32 data;
};

class REG_HSC_g_1_sp_low : public BaseReg
{
public:
    REG_HSC_g_1_sp_low(quint32 data = 0) : BaseReg(0xF8),
        data(data) { }
    quint32 data;
};

class REG_HSC_pll_reg : public BaseReg
{
public:
    REG_HSC_pll_reg(quint32 ns = 0, quint32 ms = 0, quint32 frange = 0, quint32 pd = 0) : BaseReg(0xFC),
        ns(ns), ms(ms), frange(frange), pd(pd) { }
    quint32 ns : 6;
    quint32 ms : 6;
    quint32 frange : 1;
    quint32 pd : 1;
};

class REG_AUX_rtaddr : public BaseReg
{
public:
    REG_AUX_rtaddr(quint32 addr = 0) : BaseReg(0x180),
        addr(addr) { }
    quint32 addr;
};

class REG_AUX_interruption : public BaseReg
{
public:
    REG_AUX_interruption(quint32 inter = 0) : BaseReg(0x184),
        inter(inter) { }
    quint32 inter;
};

class REG_AUX_winmode : public BaseReg
{
public:
    REG_AUX_winmode(quint32 mode = 0) : BaseReg(0x190),
        mode(mode) { }
    quint32 mode;
};

class REG_AUX_bulb : public BaseReg
{
public:
    REG_AUX_bulb(quint32 first = 0, quint32 second = 0, quint32 third = 0, quint32 fourth = 0) : BaseReg(0x194),
        first(first), second(second), third(third), fourth(fourth) { }
    quint32 first : 1;
    quint32 second : 1;
    quint32 third : 1;
    quint32 fourth : 1;
};


#endif // DATA_STRUCTS_H
