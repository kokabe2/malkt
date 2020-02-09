// Copyright(c) 2020 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#ifndef EXTERNAL_UTKERNEL_UTKERNEL_H_
#define EXTERNAL_UTKERNEL_UTKERNEL_H_

#define CONST const

#define TA_HLNG 0x00000001
#define TA_TFIFO 0x00000000
#define TA_MFIFO 0x00000000
#define TA_RNG0 0x00000000
#define TA_STA 0x00000002
#define TA_PHS 0x00000004
#define TA_USERBUF 0x00000020

#define TTS_RUN 0x00000001
#define TTS_RDY 0x00000002
#define TTS_WAI 0x00000004
#define TTS_SUS 0x00000008
#define TTS_WAS 0x0000000c
#define TTS_DMT 0x00000010

#define TCYC_STP 0x00
#define TCYC_STA 0x01

#define TALM_STP 0x00
#define TALM_STA 0x01

#define E_OK (0)
#define E_PAR (-17)
#define E_ID (-18)
#define E_CTX (-25)
#define E_LIMIT (-34)
#define E_OBJ (-41)
#define E_NOEXS (-42)
#define E_TMOUT (-50)

#define TMO_POL (0)
#define TMO_FEVR (-1)

#define CFN_MAX_TSKID (16)
#define CFN_MAX_MBXID (16)
#define CFN_MAX_MPLID (16)
#define CFN_MAX_CYCID (16)
#define CFN_MAX_ALMID (16)
#define TK_MAX_TSKPRI (16)

typedef signed long W;
typedef unsigned char UB;
typedef unsigned long UW;
typedef signed int INT;
typedef unsigned int UINT;
typedef UW ATR;
typedef INT PRI;
typedef W SZ;
typedef INT ID;
typedef INT ER;
typedef W TMO;
typedef UW RELTIM;
typedef void (*FP)();

typedef struct {
  void *exinf;
  ATR tskatr;
  FP task;
  PRI itskpri;
  SZ stksz;
  SZ sstksz;
  void *stkptr;
  void *uatb;
  INT lsid;
  ID resid;
  UB dsname[8];
  void *bufptr;
} T_CTSK;
typedef struct {
  void *exinf;
  ATR mbxatr;
  UB dsname[8];
} T_CMBX;
typedef struct {
  void *msgque;
  int dummy;
} T_MSG;
typedef struct {
  void *exinf;
  ATR mplatr;
  SZ mplsz;
  UB dsname[8];
  void *bufptr;
} T_CMPL;
typedef struct {
  void *exinf;
  ATR mpfatr;
  SZ mpfcnt;
  SZ blfsz;
  void *bufptr;
} T_CMPF;
typedef struct {
  W hi;
  UW lo;
} SYSTIM;
typedef struct {
  void *exinf;
  ATR cycatr;
  FP cychdr;
  RELTIM cyctim;
  RELTIM cycphs;
  UB dsname[8];
} T_CCYC;
typedef struct {
  void *exinf;
  RELTIM lfttim;
  UINT cycstat;
} T_RCYC;
typedef struct {
  void *exinf;
  ATR almatr;
  FP almhdr;
  UB dsname[8];
} T_CALM;
typedef struct {
  void *exinf;
  RELTIM lfttim;
  UINT almstat;
} T_RALM;
typedef struct {
  ATR intatr;
  FP inthdr;
} T_DINT;

ID tk_cre_tsk(CONST T_CTSK *pk_ctsk);
ER tk_del_tsk(ID tskid);
ER tk_sta_tsk(ID tskid, INT stacd);
void tk_ext_tsk(void);
void tk_exd_tsk(void);
ER tk_ter_tsk(ID tskid);

ER tk_slp_tsk(TMO tmout);
ER tk_wup_tsk(ID tskid);
ER tk_rel_wai(ID tskid);
ER tk_sus_tsk(ID tskid);
ER tk_rsm_tsk(ID tskid);
ER tk_dly_tsk(RELTIM dlytim);

ID tk_get_tid(void);

ID tk_cre_mbx(CONST T_CMBX *pk_cmbx);
ER tk_del_mbx(ID mbxid);
ER tk_snd_mbx(ID mbxid, T_MSG *pk_msg);
ER tk_rcv_mbx(ID mbxid, T_MSG **ppk_msg, TMO tmout);

ID tk_cre_mpf(CONST T_CMPF *pk_cmpf);
ER tk_del_mpf(ID mpfid);
ER tk_get_mpf(ID mpfid, void **p_blf, TMO tmout);
ER tk_rel_mpf(ID mpfid, void *blf);
ID tk_cre_mpl(CONST T_CMPL *pk_cmpl);
ER tk_del_mpl(ID mplid);
ER tk_get_mpl(ID mplid, SZ blksz, void **p_blk, TMO tmout);
ER tk_rel_mpl(ID mplid, void *blk);

ER tk_set_tim(CONST SYSTIM *pk_tim);
ER tk_get_tim(SYSTIM *pk_tim);

ID tk_cre_cyc(CONST T_CCYC *pk_ccyc);
ER tk_del_cyc(ID cycid);
ER tk_sta_cyc(ID cycid);
ER tk_stp_cyc(ID cycid);
ER tk_ref_cyc(ID cycid, T_RCYC *pk_rcyc);

ID tk_cre_alm(CONST T_CALM *pk_calm);
ER tk_del_alm(ID almid);
ER tk_sta_alm(ID almid, RELTIM almtim);
ER tk_stp_alm(ID almid);
ER tk_ref_alm(ID almid, T_RALM *pk_ralm);

ER tk_def_int(UINT intno, CONST T_DINT *pk_dint);
void EnableInt(UINT intno, INT level);
void DisableInt(UINT intno);

#endif  // EXTERNAL_UTKERNEL_UTKERNEL_H_
