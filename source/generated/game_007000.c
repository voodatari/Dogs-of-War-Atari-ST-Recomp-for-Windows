/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 007084  game routine */
void sub_007084(void)
{
    SETB(d0, d0 | 7);                                    /* 007084  ori.b  #$7,d0 */
    SETB(d2, d2 | 7);                                    /* 007088  ori.b  #$7,d2 */
    SETB(d4, d4 | 6);                                    /* 00708C  ori.b  #$6,d4 */
    SETB(d6, d6 | 4);                                    /* 007090  ori.b  #$4,d6 */
    SETB(d7, d7 | 2);                                    /* 007094  ori.b  #$2,d7 */
    SETB(d7, d7 | 0);                                    /* 007098  ori.b  #$0,d7 */
    SETB(d7, d7 | 0xFE);                                 /* 00709C  ori.b  #$fe,d7 */
    SETB(d6, d6 | 0xFC);                                 /* 0070A0  ori.b  #$fc,d6 */
    SETB(d4, d4 | 0xFA);                                 /* 0070A4  ori.b  #$fa,d4 */
    SETB(d2, d2 | 0xF9);                                 /* 0070A8  ori.b  #$f9,d2 */
    /* 0070AC  ori.b  #$f9,d0 */
    {
        uint32_t s = 0xF9;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x70B0U);                                       /* 0070B0  soft */
}

/* 007134  game routine */
void sub_007134(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x2E01C;                                        /* 007134  lea  $2e01c.l,a0 */
    a1 = 0x20000;                                        /* 00713A  lea  $20000.l,a1 */
    SETW(d0, 0xC7);                                      /* 007140  move.w  #$c7,d0 */
    do {
        BUDGET_CHECK(0x7144U);
L_007144:
        /* 007144  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007146  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007148  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00714A  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00714C  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00714E  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007150  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007152  move.l  (a0)+,(a1) */
        wr32(a1, rd32(a0));
        a0 += 4;
        a1 = a1 + 0x84;                                      /* 007154  lea  $84(a1),a1 */
        /* 007158  dbra  d0,$7144 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_00715C:
    a1 = 0x20000;                                        /* 00715C  lea  $20000.l,a1 */
    a0 = 0x20098;                                        /* 007162  lea  $20098.l,a0 */
    SETW(d7, 0xC7);                                      /* 007168  move.w  #$c7,d7 */
    do {
        BUDGET_CHECK(0x716CU);
L_00716C:
        a2 = a0;                                             /* 00716C  movea.l  a0,a2 */
        a3 = a1;                                             /* 00716E  movea.l  a1,a3 */
        d6 = 3;                                              /* 007170  moveq  #3,d6 */
        do {
            BUDGET_CHECK(0x7172U);
L_007172:
            /* 007172  move.w  (a1)+,d1 */
            SETW(d1, rd16(a1));
            a1 += 2;
            CALL(sub_0071A6, 0x7176U);                           /* 007174  bsr  $71a6 */
L_007176:
            wr16(a0, (d0 & 0xFFFF));                             /* 007176  move.w  d0,(a0) */
            /* 007178  move.w  (a1)+,d1 */
            SETW(d1, rd16(a1));
            a1 += 2;
            CALL(sub_0071A6, 0x717CU);                           /* 00717A  bsr  $71a6 */
L_00717C:
            wr16(a0 + 0x2, (d0 & 0xFFFF));                       /* 00717C  move.w  d0,$2(a0) */
            /* 007180  move.w  (a1)+,d1 */
            SETW(d1, rd16(a1));
            a1 += 2;
            CALL(sub_0071A6, 0x7184U);                           /* 007182  bsr  $71a6 */
L_007184:
            wr16(a0 + 0x4, (d0 & 0xFFFF));                       /* 007184  move.w  d0,$4(a0) */
            /* 007188  move.w  (a1)+,d1 */
            SETW(d1, rd16(a1));
            a1 += 2;
            CALL(sub_0071A6, 0x718CU);                           /* 00718A  bsr  $71a6 */
L_00718C:
            wr16(a0 + 0x6, (d0 & 0xFFFF));                       /* 00718C  move.w  d0,$6(a0) */
            a0 = a0 - 0x8;                                       /* 007190  lea  -$8(a0),a0 */
            /* 007194  dbra  d6,$7172 */
            SETW(d6, d6 - 1);
        } while ((d6 & 0xFFFF) != 0xFFFF);
L_007198:
        a0 = a2 + 0xA0;                                      /* 007198  lea  $a0(a2),a0 */
        a1 = a3 + 0xA0;                                      /* 00719C  lea  $a0(a3),a1 */
        /* 0071A0  dbra  d7,$716c */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_0071A4:
    RTS();                                               /* 0071A4  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7144U: goto L_007144;
        case 0x715CU: goto L_00715C;
        case 0x716CU: goto L_00716C;
        case 0x7172U: goto L_007172;
        case 0x7176U: goto L_007176;
        case 0x717CU: goto L_00717C;
        case 0x7184U: goto L_007184;
        case 0x718CU: goto L_00718C;
        case 0x7198U: goto L_007198;
        case 0x71A4U: goto L_0071A4;
        }
    }
}

/* 0071A6  game routine */
void sub_0071A6(void)
{
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071A6  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071A8  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071AA  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071AC  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071AE  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071B0  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071B2  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071B4  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071B6  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071B8  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071BA  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071BC  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071BE  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071C0  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071C2  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071C4  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071C6  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071C8  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071CA  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071CC  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071CE  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071D0  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071D2  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071D4  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071D6  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071D8  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071DA  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071DC  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071DE  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071E0  roxl.w  #1,d0 */
    SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 1, 16));      /* 0071E2  lsr.w  #1,d1 */
    SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0071E4  roxl.w  #1,d0 */
    RTS();                                               /* 0071E6  rts */
}

/* 0071E8  game routine */
void sub_0071E8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1614;                                         /* 0071E8  lea  $1614(a5),a6 */
    SETW(d7, 0x13);                                      /* 0071EC  move.w  #$13,d7 */
L_0071F0:
    /* 0071F0  tst.w  (a6) */
    /* 0071F2  beq  $72fe */
    if (rd16(a6) != 0) {
L_0071F6:
        /* 0071F6  cmpi.w  #$10,$6(a6) */
        /* 0071FC  bcs  $72fe */
        if (rd16(a6 + 0x6) >= 0x10) {
L_007200:
            /* 007200  cmpi.w  #$17,$6(a6) */
            /* 007206  bhi  $72fe */
            if (rd16(a6 + 0x6) <= 0x17) {
                a4 = V_VEHICLES;                                     /* 00720A  lea  $1a76(a5),a4 */
                SETW(d6, 4);                                         /* 00720E  move.w  #$4,d6 */
                do {
                    BUDGET_CHECK(0x7212U);
L_007212:
                    /* 007212  tst.w  (a4) */
                    /* 007214  beq  $72f6 */
                    if (rd16(a4) != 0) {
                        SETW(d0, rd16(a6 + 0x2));                            /* 007218  move.w  $2(a6),d0 */
                        SETW(d1, rd16(a6 + 0x4));                            /* 00721C  move.w  $4(a6),d1 */
                        SETW(d0, d0 - 8);                                    /* 007220  subi.w  #$8,d0 */
                        SETW(d1, d1 + 4);                                    /* 007224  addi.w  #$4,d1 */
                        SETW(d2, rd16(a4 + 0x2));                            /* 007228  move.w  $2(a4),d2 */
                        SETW(d3, rd16(a4 + 0x4));                            /* 00722C  move.w  $4(a4),d3 */
                        SETW(d4, rd16(a4 + 0xE));                            /* 007230  move.w  $e(a4),d4 */
                        SETW(d5, rd16(a4 + 0xA));                            /* 007234  move.w  $a(a4),d5 */
                        /* 007238  sub.w  d3,d1 */
                        {
                            uint32_t s = (d3 & 0xFFFF);
                            uint32_t d = (d1 & 0xFFFF);
                            uint32_t r = (d - s) & 0xFFFF;
                            SETW(d1, r);
                            fx = (s > d);
                        }
                        /* 00723A  cmp.w  d5,d1 */
                        /* 00723C  bhi  $72f6 */
                        if ((d1 & 0xFFFF) <= (d5 & 0xFFFF)) {
                            /* 007240  cmp.w  #$0,d1 */
                            /* 007244  bcs  $72f6 */
                            if ((d1 & 0xFFFF) >= 0) {
                                SETW(d0, d0 - (d2 & 0xFFFF));                        /* 007248  sub.w  d2,d0 */
                                /* 00724A  addi.w  #$a,d0 */
                                {
                                    uint32_t s = 0xA;
                                    uint32_t d = (d0 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d0, r);
                                    fx = (r < s);
                                }
                                /* 00724E  cmp.w  d4,d0 */
                                /* 007250  bhi  $72f6 */
                                if ((d0 & 0xFFFF) <= (d4 & 0xFFFF)) {
                                    /* 007254  cmp.w  #$0,d0 */
                                    /* 007258  bcs  $72f6 */
                                    if ((d0 & 0xFFFF) >= 0) {
                                        wr16(a6, 0);                                         /* 00725C  clr.w  (a6) */
                                        /* 00725E  tst.w  $c(a4) */
                                        /* 007262  bne  $7308 */
                                        if (rd16(a4 + 0xC) != 0) {
                                            goto L_007308;
                                        }
                                        wr32(V_02E2, rd32(V_02E2) + 0x7D0);                  /* 007266  addi.l  #$7d0,$2e2(a5) */
                                        /* 00726E  tst.w  $e(a6) */
                                        /* 007272  beq  $7284 */
                                        if (rd16(a6 + 0xE) != 0) {
                                            wr32(V_02E2, rd32(V_02E2) - 0x7D0);                  /* 007274  subi.l  #$7d0,$2e2(a5) */
                                            wr32(V_037E, rd32(V_037E) + 0x7D0);                  /* 00727C  addi.l  #$7d0,$37e(a5) */
                                        }
                                        wr16(a4 + 0x6, 0);                                   /* 007284  clr.w  $6(a4) */
                                        wr16(a4 + 0xC, 0xFFFF);                              /* 007288  move.w  #$ffff,$c(a4) */
                                        /* 00728E  addi.w  #$1,$8(a4) */
                                        {
                                            uint32_t s = 1;
                                            uint32_t d = rd16(a4 + 0x8);
                                            uint32_t r = (d + s) & 0xFFFF;
                                            wr16(a4 + 0x8, r);
                                            fx = (r < s);
                                        }
                                        a2 = rd32(a4 + 0x12);                                /* 007294  movea.l  $12(a4),a2 */
                                        /* 007298  cmpa.l  #$0,a2 */
                                        /* 00729E  beq  $72e8 */
                                        if (a2 != 0) {
                                            wr16(a2, 0);                                         /* 0072A0  clr.w  (a2) */
                                            a2 = rd32(a4 + 0x16);                                /* 0072A2  movea.l  $16(a4),a2 */
                                            /* 0072A6  cmpa.l  #$0,a2 */
                                            /* 0072AC  beq  $72e8 */
                                            if (a2 != 0) {
                                                wr16(a2, 0);                                         /* 0072AE  clr.w  (a2) */
                                                a2 = rd32(a4 + 0x1A);                                /* 0072B0  movea.l  $1a(a4),a2 */
                                                /* 0072B4  cmpa.l  #$0,a2 */
                                                /* 0072BA  beq  $72e8 */
                                                if (a2 != 0) {
                                                    wr16(a2, 0);                                         /* 0072BC  clr.w  (a2) */
                                                    a2 = rd32(a4 + 0x1E);                                /* 0072BE  movea.l  $1e(a4),a2 */
                                                    /* 0072C2  cmpa.l  #$0,a2 */
                                                    /* 0072C8  beq  $72e8 */
                                                    if (a2 != 0) {
                                                        wr16(a2, 0);                                         /* 0072CA  clr.w  (a2) */
                                                        a2 = rd32(a4 + 0x22);                                /* 0072CC  movea.l  $22(a4),a2 */
                                                        /* 0072D0  cmpa.l  #$0,a2 */
                                                        /* 0072D6  beq  $72e8 */
                                                        if (a2 != 0) {
                                                            wr16(a2, 0);                                         /* 0072D8  clr.w  (a2) */
                                                            a2 = rd32(a4 + 0x26);                                /* 0072DA  movea.l  $26(a4),a2 */
                                                            /* 0072DE  cmpa.l  #$0,a2 */
                                                            /* 0072E4  beq  $72e8 */
                                                            if (a2 != 0) {
                                                                wr16(a2, 0);                                         /* 0072E6  clr.w  (a2) */
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        d0 = 3;                                              /* 0072E8  moveq  #3,d0 */
                                        CALL(sub_00E756, 0x72EEU);                           /* 0072EA  bsr  $e756 */
L_0072EE:
                                        CALL(sub_007324, 0x72F2U);                           /* 0072EE  bsr  $7324 */
L_0072F2:
                                        /* 0072F2  bra  $72fe */
                                        goto L_0072FE;
                                    }
                                }
                            }
                        }
                    }
                    a4 = a4 + 0x2E;                                      /* 0072F6  lea  $2e(a4),a4 */
                    /* 0072FA  dbra  d6,$7212 */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
            }
        }
    }
L_0072FE:
    a6 = a6 + 0x10;                                      /* 0072FE  lea  $10(a6),a6 */
    /* 007302  dbra  d7,$71f0 */
    SETW(d7, d7 - 1);
    if ((d7 & 0xFFFF) != 0xFFFF) {
        BUDGET_CHECK(0x71F0U);
        goto L_0071F0;
    }
L_007306:
    RTS();                                               /* 007306  rts */
L_007308:
    d3 = 0;                                              /* 007308  moveq  #0,d3 */
    SETW(d0, rd16(a6 + 0x2));                            /* 00730A  move.w  $2(a6),d0 */
    SETW(d1, rd16(a6 + 0x4));                            /* 00730E  move.w  $4(a6),d1 */
    SETW(d2, rd16(a6 + 0xE));                            /* 007312  move.w  $e(a6),d2 */
    SETW(d0, d0 - 2);                                    /* 007316  subi.w  #$2,d0 */
    /* 00731A  subi.w  #$8,d1 */
    {
        uint32_t s = 8;
        uint32_t d = (d1 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d1, r);
        fx = (s > d);
    }
    CALL(sub_006CA8, 0x7322U);                           /* 00731E  bsr  $6ca8 */
L_007322:
    /* 007322  bra  $72fe */
    BUDGET_CHECK(0x72FEU);
    goto L_0072FE;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x71F0U: goto L_0071F0;
        case 0x71F6U: goto L_0071F6;
        case 0x7200U: goto L_007200;
        case 0x7212U: goto L_007212;
        case 0x72EEU: goto L_0072EE;
        case 0x72F2U: goto L_0072F2;
        case 0x72FEU: goto L_0072FE;
        case 0x7306U: goto L_007306;
        case 0x7322U: goto L_007322;
        }
    }
}

/* 007324  game routine */
void sub_007324(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 007324  cmpi.w  #$0,$10(a4) */
    /* 00732A  bne  $7332 */
    if (rd16(a4 + 0x10) == 0) {
        a0 = 0x738A;                                         /* 00732C  lea  $738a(pc),a0 */
        /* 007330  bra  $7352 */
    } else {
        /* 007332  cmpi.w  #$2,$10(a4) */
        /* 007338  bne  $7340 */
        if (rd16(a4 + 0x10) == 2) {
            a0 = 0x7400;                                         /* 00733A  lea  $7400(pc),a0 */
            /* 00733E  bra  $7352 */
        } else {
            /* 007340  cmpi.w  #$4,$10(a4) */
            /* 007346  bne  $734e */
            if (rd16(a4 + 0x10) == 4) {
                a0 = 0x7476;                                         /* 007348  lea  $7476(pc),a0 */
                /* 00734C  bra  $7352 */
            } else {
                a0 = 0x74C4;                                         /* 00734E  lea  $74c4(pc),a0 */
            }
        }
    }
L_007352:
    /* 007352  move.w  (a0)+,d0 */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        SETW(d0, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007354  bmi  $7388 */
    if (COND(10)) {
        /* 007356  cmp.w  #$1,d0 */
        /* 00735A  beq  $7372 */
        if ((d0 & 0xFFFF) != 1) {
            /* 00735E  move.w  (a0)+,d0 */
            SETW(d0, rd16(a0));
            a0 += 2;
            /* 007360  move.w  (a0)+,d1 */
            SETW(d1, rd16(a0));
            a0 += 2;
            /* 007362  move.w  (a0)+,d2 */
            SETW(d2, rd16(a0));
            a0 += 2;
            SETW(d0, d0 + rd16(a4 + 0x2));                       /* 007364  add.w  $2(a4),d0 */
            /* 007368  add.w  $4(a4),d1 */
            {
                uint32_t s = rd16(a4 + 0x4);
                uint32_t d = (d1 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d1, r);
                fx = (r < s);
            }
            CALL(sub_0063A4, 0x7370U);                           /* 00736C  bsr  $63a4 */
L_007370:
            /* 007370  bra  $7352 */
            BUDGET_CHECK(0x7352U);
            goto L_007352;
        }
        /* 007372  move.w  (a0)+,d0 */
        SETW(d0, rd16(a0));
        a0 += 2;
        /* 007374  move.w  (a0)+,d1 */
        SETW(d1, rd16(a0));
        a0 += 2;
        SETW(d0, d0 + rd16(a4 + 0x2));                       /* 007376  add.w  $2(a4),d0 */
        /* 00737A  add.w  $4(a4),d1 */
        {
            uint32_t s = rd16(a4 + 0x4);
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
            fx = (r < s);
        }
        SETW(d2, 0xFFFF);                                    /* 00737E  move.w  #$ffff,d2 */
        CALL(sub_006CA8, 0x7386U);                           /* 007382  bsr  $6ca8 */
L_007386:
        /* 007386  bra  $7352 */
        BUDGET_CHECK(0x7352U);
        goto L_007352;
    }
    RTS();                                               /* 007388  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7352U: goto L_007352;
        case 0x7370U: goto L_007370;
        case 0x7386U: goto L_007386;
        }
    }
}

/* 00738A  game routine */
void sub_00738A(void)
{
    SETB(d0, d0 | 0);                                    /* 00738A  ori.b  #$0,d0 */
    SETB(d5, d5 | 8);                                    /* 00738E  ori.b  #$8,d5 */
    SETB(d0, d0 | 0x10);                                 /* 007392  ori.b  #$10,d0 */
    /* 007396  ori.b  #$c,(a0)+ */
    {
        uint32_t s = 0xC;
        uint32_t ea = a0;
        a0 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    /* 00739A  ori.b  #$f0,d0 */
    {
        uint32_t s = 0xF0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x739EU);                                       /* 00739E  soft */
}

/* 007400  game routine */
void sub_007400(void)
{
    /* 007400  ori.b  #$f0,d0 */
    {
        uint32_t s = 0xF0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x7404U);                                       /* 007404  soft */
}

/* 007476  game routine */
void sub_007476(void)
{
    /* 007476  ori.b  #$f0,d0 */
    {
        uint32_t s = 0xF0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x747AU);                                       /* 00747A  soft */
}

/* 0074C4  game routine */
void sub_0074C4(void)
{
    /* 0074C4  ori.b  #$f0,d0 */
    {
        uint32_t s = 0xF0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x74C8U);
}

/* 0074F0  game routine */
void sub_0074F0(void)
{
    /* 0074F0  bra  $75e2 */
    { sub_0075E2(); return; }
}

/* 0074F4  game routine */
void sub_0074F4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr8(V_01E8, 0);                                      /* 0074F4  sf  $1e8(a5) */
    wr32(V_0198, 0x41BCC);                               /* 0074F8  move.l  #$41bcc,$198(a5) */
    CALL(sub_041BCC, 0x7506U);                           /* 007500  jsr  $41bcc.l */
L_007506:
    wr8(V_01E8, 0xFF);                                   /* 007506  st  $1e8(a5) */
    RTS();                                               /* 00750A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7506U: goto L_007506;
        }
    }
}

/* 00750C  game routine */
void sub_00750C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00750C  tst.w  $1e8(a5) */
    /* 007510  beq  $7518 */
    if (rd16(V_01E8) != 0) {
L_007512:
        CALL(sub_041BDA, 0x7518U);                           /* 007512  jsr  $41bda.l */
    }
L_007518:
    RTS();                                               /* 007518  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7512U: goto L_007512;
        case 0x7518U: goto L_007518;
        }
    }
}

/* 00751A  game routine */
void sub_00751A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00751A  tst.b  $424f2.l */
    /* 007520  beq  $7556 */
    if (rd8(0x424F2) != 0) {
L_007522:
        wr8(V_01A2, rd8(0x42502));                           /* 007522  move.b  $42502.l,$1a2(a5) */
        /* 00752A  movem.w  d1/d0,-(a7) */
        PUSH16(d1);
        PUSH16(d0);
        SETW(d1, 0xE);                                       /* 00752E  move.w  #$e,d1 */
        do {
            BUDGET_CHECK(0x7532U);
L_007532:
            SETW(d0, 0xA);                                       /* 007532  move.w  #$a,d0 */
            do {
                BUDGET_CHECK(0x7536U);
L_007536:
                CALL(wait_vbl, 0x753AU);                             /* 007536  bsr  $79dc */
L_00753A:
                /* 00753A  dbra  d0,$7536 */
                SETW(d0, d0 - 1);
            } while ((d0 & 0xFFFF) != 0xFFFF);
L_00753E:
            wr8(0x42502, (d1 & 0xFF));                           /* 00753E  move.b  d1,$42502.l */
            /* 007544  subq.w  #1,d1 */
            {
                uint32_t s = 1;
                uint32_t d = (d1 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d1, r);
                fz = (r == 0);
                fx = (s > d);
            }
            /* 007546  bne  $7532 */
        } while (COND(6));
L_007548:
        wr8(V_01E8, 0);                                      /* 007548  sf  $1e8(a5) */
        CALL(sub_041BE8, 0x7552U);                           /* 00754C  jsr  $41be8.l */
L_007552:
        /* 007552  movem.w  (a7)+,d0-d1 */
        d0 = SX16(POP16());
        d1 = SX16(POP16());
    }
    RTS();                                               /* 007556  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7522U: goto L_007522;
        case 0x7532U: goto L_007532;
        case 0x7536U: goto L_007536;
        case 0x753AU: goto L_00753A;
        case 0x753EU: goto L_00753E;
        case 0x7548U: goto L_007548;
        case 0x7552U: goto L_007552;
        }
    }
}

/* 007558  game routine */
void sub_007558(void)
{
    a0 = 0x20000;                                        /* 007558  lea  $20000.l,a0 */
    /* 00755E  lea  $18000.l,a1 */
    a1 = 0x18000;
    { sub_007564(); return; }
}

/* 007564  game routine */
void sub_007564(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, 0x7CF);                                     /* 007564  move.w  #$7cf,d0 */
    do {
        BUDGET_CHECK(0x7568U);
L_007568:
        /* 007568  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00756A  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00756C  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00756E  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007570  dbra  d0,$7568 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_007574:
    RTS();                                               /* 007574  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7568U: goto L_007568;
        case 0x7574U: goto L_007574;
        }
    }
}

/* 007576  game routine */
void sub_007576(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = rd32(V_1E12);                                   /* 007576  move.l  $1e12(a5),d0 */
    d0 = shift_op(1, 1, d0, 1, 32);                      /* 00757A  lsl.l  #1,d0 */
    fz = !(d0 & 0x800000);                               /* 00757C  btst  #$17,d0 */
    /* 007580  bne  $758e */
    if (COND(7)) {
L_007582:
        fz = !(d0 & 0x2);                                    /* 007582  btst  #$1,d0 */
        /* 007586  bne  $7594 */
        if (COND(6)) {
            goto L_007594;
        }
L_007588:
        wr32(V_1E12, d0);                                    /* 007588  move.l  d0,$1e12(a5) */
        RTS();                                               /* 00758C  rts */
    }
L_00758E:
    fz = !(d0 & 0x2);                                    /* 00758E  btst  #$1,d0 */
    /* 007592  bne  $7596 */
    if (COND(6)) {
        goto L_007596;
    }
L_007594:
    /* 007594  addq.l  #1,d0 */
    {
        uint32_t s = 1;
        uint32_t d = d0;
        uint32_t r = (d + s) & 0xFFFFFFFF;
        d0 = r;
        fx = (r < s);
    }
L_007596:
    wr32(V_1E12, d0);                                    /* 007596  move.l  d0,$1e12(a5) */
    RTS();                                               /* 00759A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7582U: goto L_007582;
        case 0x7588U: goto L_007588;
        case 0x758EU: goto L_00758E;
        case 0x7594U: goto L_007594;
        case 0x7596U: goto L_007596;
        }
    }
}

/* 00759C  game routine */
void sub_00759C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x759CU);
        /* 00759C  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 00759E  cmpa.l  a1,a0 */
        /* 0075A0  bcs  $759c */
    } while (a0 < a1);
L_0075A2:
    RTS();                                               /* 0075A2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x75A2U: goto L_0075A2;
        }
    }
}

/* 0075A4  game routine */
void sub_0075A4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x75A4U);
        if (skip_black_delay()) d7 = 1;                      /* (added by hand) the black wait after the loader text is cut short */
        /* 0075A4  subi.l  #$1,d7 */
        IDLE_DELAY(0x75A4U, 6163);
        {
            uint32_t s = 1;
            uint32_t d = d7;
            uint32_t r = (d - s) & 0xFFFFFFFF;
            d7 = r;
            fz = (r == 0);
            fx = (s > d);
        }
        /* 0075AA  bne  $75a4 */
    } while (COND(6));
L_0075AC:
    RTS();                                               /* 0075AC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x75ACU: goto L_0075AC;
        }
    }
}

/* 0075AE  CPU delay loop (one VBL per turn) */
void delay_loop(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x75AEU);
        SETB(d0, rd8(V_02EC));                               /* 0075AE  move.b  $2ec(a5),d0 */
        SETB(d1, rd8(V_0388));                               /* 0075B2  move.b  $388(a5),d1 */
        /* 0075B6  add.b  d1,d0 */
        {
            uint32_t s = (d1 & 0xFF);
            uint32_t d = (d0 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d0, r);
            fx = (r < s);
        }
        fz = !(d0 & 0x80);                                   /* 0075B8  btst  #$7,d0 */
        /* 0075BC  bne  $75c2 */
        if (COND(6)) {
            goto L_0075C2;
        }
L_0075BE:
        /* 0075BE  subq.l  #1,d7 */
        IDLE_DELAY(0x75BEU, 2504);
        {
            uint32_t s = 1;
            uint32_t d = d7;
            uint32_t r = (d - s) & 0xFFFFFFFF;
            d7 = r;
            fz = (r == 0);
            fx = (s > d);
        }
        /* 0075C0  bne  $75ae */
    } while (COND(6));
L_0075C2:
    RTS();                                               /* 0075C2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x75BEU: goto L_0075BE;
        }
    }
}

/* 0075C4  game routine */
void sub_0075C4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0075C4  cmpi.b  #$42,$1de2(a5) */
    /* 0075CA  bne  $75e0 */
    if (rd8(V_1DE2) == 0x42) {
        do {
            BUDGET_CHECK(0x75CCU);
L_0075CC:
            /* 0075CC  tst.b  $2ec(a5) */
            /* 0075D0  bne  $75e0 */
            if (rd8(V_02EC) != 0) {
                goto L_0075E0;
            }
            /* 0075D2  tst.b  $388(a5) */
            /* 0075D6  bne  $75e0 */
            if (rd8(V_0388) != 0) {
                goto L_0075E0;
            }
            /* 0075D8  cmpi.b  #$43,$1de2(a5) */
            /* 0075DE  bne  $75cc */
        } while (rd8(V_1DE2) != 0x43);
    }
L_0075E0:
    RTS();                                               /* 0075E0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x75CCU: goto L_0075CC;
        case 0x75E0U: goto L_0075E0;
        }
    }
}

/* 0075E2  game routine */
void sub_0075E2(void)
{
    a0 = HW_PALETTE;                                     /* 0075E2  lea  $ff8240.l,a0 */
    /* 0075E8  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075EA  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075EC  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075EE  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075F0  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075F2  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075F4  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 0075F6  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    RTS();                                               /* 0075F8  rts */
}

/* 007626  game routine */
void sub_007626(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = 0x20000;                                        /* 007626  move.l  #$20000,d0 */
    wr32(V_DRAW_BUFFER, d0);                             /* 00762C  move.l  d0,$28e(a5) */
    CALL(sub_007B2A, 0x7634U);                           /* 007630  bsr  $7b2a */
L_007634:
    a0 = 0x20000;                                        /* 007634  lea  $20000.l,a0 */
    a1 = 0x18000;                                        /* 00763A  lea  $18000.l,a1 */
    SETW(d7, 0x7FF);                                     /* 007640  move.w  #$7ff,d7 */
    do {
        BUDGET_CHECK(0x7644U);
L_007644:
        /* 007644  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 007646  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 007648  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 00764A  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 00764C  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 00764E  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 007650  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 007652  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 007654  dbra  d7,$7644 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_007658:
    RTS();                                               /* 007658  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7634U: goto L_007634;
        case 0x7644U: goto L_007644;
        case 0x7658U: goto L_007658;
        }
    }
}

/* 00765A  game routine */
void sub_00765A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00765A  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_00765E:
    a0 = 0x7B3C;                                         /* 00765E  lea  $7b3c(pc),a0 */
    wr32(0x8, a0);                                       /* 007662  move.l  a0,$8.l */
    a0 = 0x7B46;                                         /* 007668  lea  $7b46(pc),a0 */
    wr32(0xC, a0);                                       /* 00766C  move.l  a0,$c.l */
    a0 = 0x7B50;                                         /* 007672  lea  $7b50(pc),a0 */
    wr32(0x10, a0);                                      /* 007676  move.l  a0,$10.l */
    a0 = 0x7B5A;                                         /* 00767C  lea  $7b5a(pc),a0 */
    wr32(0x14, a0);                                      /* 007680  move.l  a0,$14.l */
    a0 = 0x7B64;                                         /* 007686  lea  $7b64(pc),a0 */
    wr32(0x18, a0);                                      /* 00768A  move.l  a0,$18.l */
    a0 = 0x7B6E;                                         /* 007690  lea  $7b6e(pc),a0 */
    wr32(0x1C, a0);                                      /* 007694  move.l  a0,$1c.l */
    a0 = 0x7B78;                                         /* 00769A  lea  $7b78(pc),a0 */
    wr32(0x20, a0);                                      /* 00769E  move.l  a0,$20.l */
    a0 = 0x7B82;                                         /* 0076A4  lea  $7b82(pc),a0 */
    wr32(0x24, a0);                                      /* 0076A8  move.l  a0,$24.l */
    a0 = 0x7B8C;                                         /* 0076AE  lea  $7b8c(pc),a0 */
    wr32(0x28, a0);                                      /* 0076B2  move.l  a0,$28.l */
    a0 = 0x7B96;                                         /* 0076B8  lea  $7b96(pc),a0 */
    wr32(0x2C, a0);                                      /* 0076BC  move.l  a0,$2c.l */
    a0 = 0x79EC;                                         /* 0076C2  lea  $79ec(pc),a0 */
    wr32(V_0118, a0);                                    /* 0076C6  move.l  a0,$118.l */
    wr32(0x78, a0);                                      /* 0076CC  move.l  a0,$78.l */
    wr8(HW_MFP_IMRA, 0);                                 /* 0076D2  clr.b  $fffa13.l */
    wr8(HW_MFP_IERA, 0);                                 /* 0076D8  clr.b  $fffa07.l */
    wr8(0xFFFA0B, 0);                                    /* 0076DE  clr.b  $fffa0b.l */
    wr8(HW_MFP_ISRA, 0);                                 /* 0076E4  clr.b  $fffa0f.l */
    wr8(HW_MFP_IMRB, 0x40);                              /* 0076EA  move.b  #$40,$fffa15.l */
    wr8(HW_MFP_IERB, 0x40);                              /* 0076F2  move.b  #$40,$fffa09.l */
    a0 = 0x78A8;                                         /* 0076FA  lea  $78a8(pc),a0 */
    wr32(0x70, a0);                                      /* 0076FE  move.l  a0,$70.l */
    wr8(HW_MFP_TACR, 0);                                 /* 007704  clr.b  $fffa19.l */
    /* 00770A  nop */
    /* 00770C  nop */
    /* 00770E  nop */
    wr8(HW_MFP_TADR, 0x22);                              /* 007710  move.b  #$22,$fffa1f.l */
    /* 007718  nop */
    /* 00771A  nop */
    /* 00771C  nop */
    a0 = 0xE660;                                         /* 00771E  lea  $e660(pc),a0 */
    wr32(V_0134, a0);                                    /* 007722  move.l  a0,$134.l */
    CALL(sub_007866, 0x772CU);                           /* 007728  bsr  $7866 */
L_00772C:
    a0 = 0x7932;                                         /* 00772C  lea  $7932(pc),a0 */
    wr32(V_0120, a0);                                    /* 007730  move.l  a0,$120.l */
    wr8(HW_MFP_TBCR, 0);                                 /* 007736  clr.b  $fffa1b.l */
    /* 00773C  nop */
    /* 00773E  nop */
    /* 007740  nop */
    wr8(HW_MFP_TBDR, 0x36);                              /* 007742  move.b  #$36,$fffa21.l */
    /* 00774A  nop */
    /* 00774C  nop */
    /* 00774E  nop */
    wr8(HW_MFP_TBCR, 8);                                 /* 007750  move.b  #$8,$fffa1b.l */
    a0 = 0x7896;                                         /* 007758  lea  $7896(pc),a0 */
    wr32(V_0114, a0);                                    /* 00775C  move.l  a0,$114.l */
    wr8(HW_MFP_TCDR, 0xFD);                              /* 007762  move.b  #$fd,$fffa23.l */
    SETB(d0, rd8(HW_MFP_TCDCR));                         /* 00776A  move.b  $fffa1d.l,d0 */
    SETB(d0, d0 & 0x8F);                                 /* 007770  andi.b  #$8f,d0 */
    SETB(d0, d0 | 0x70);                                 /* 007774  ori.b  #$70,d0 */
    wr8(HW_MFP_TCDCR, (d0 & 0xFF));                      /* 007778  move.b  d0,$fffa1d.l */
    wr8(HW_MFP_IERA, 0);                                 /* 00777E  clr.b  $fffa07.l */
    wr8(HW_MFP_IERB, 0);                                 /* 007784  clr.b  $fffa09.l */
    wr8(HW_MFP_IMRA, 0);                                 /* 00778A  clr.b  $fffa13.l */
    wr8(HW_MFP_IMRB, 0);                                 /* 007790  clr.b  $fffa15.l */
    wr8(0xFFFA0B, 0);                                    /* 007796  clr.b  $fffa0b.l */
    wr8(0xFFFA0D, 0);                                    /* 00779C  clr.b  $fffa0d.l */
    wr8(HW_MFP_ISRA, 0);                                 /* 0077A2  clr.b  $fffa0f.l */
    wr8(HW_MFP_ISRB, 0);                                 /* 0077A8  clr.b  $fffa11.l */
    wr8(HW_MFP_IMRB, 0x60);                              /* 0077AE  move.b  #$60,$fffa15.l */
    wr8(HW_MFP_IERB, 0x60);                              /* 0077B6  move.b  #$60,$fffa09.l */
    wr8(HW_MFP_IMRA, 1);                                 /* 0077BE  move.b  #$1,$fffa13.l */
    wr8(HW_MFP_IERA, 1);                                 /* 0077C6  move.b  #$1,$fffa07.l */
    wr8(0xFFFA05, rd8(0xFFFA05) & ~0x1);                 /* 0077CE  bclr  #$0,$fffa05.l */
    /* 0077D6  move  #$2500,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2500U;
    set_sr(s); }
L_0077DA:
    SETB(d0, 0x80);                                      /* 0077DA  move.b  #$80,d0 */
    CALL(sub_007818, 0x77E0U);                           /* 0077DE  bsr  $7818 */
L_0077E0:
    SETB(d0, 1);                                         /* 0077E0  move.b  #$1,d0 */
    CALL(sub_007818, 0x77E6U);                           /* 0077E4  bsr  $7818 */
L_0077E6:
    d7 = 0x7A120;                                        /* 0077E6  move.l  #$7a120,d7 */
    CALL(sub_0075A4, 0x77F0U);                           /* 0077EC  bsr  $75a4 */
L_0077F0:
    SETB(d0, 0x14);                                      /* 0077F0  move.b  #$14,d0 */
    /* 0077F4  bsr  $7818 */
    CALL(sub_007818, 0x77F6U);
    { sub_0077F6(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x765EU: goto L_00765E;
        case 0x772CU: goto L_00772C;
        case 0x77DAU: goto L_0077DA;
        case 0x77E0U: goto L_0077E0;
        case 0x77E6U: goto L_0077E6;
        case 0x77F0U: goto L_0077F0;
        }
    }
}

/* 0077F6  game routine */
void sub_0077F6(void)
{
    SETB(d0, rd8(HW_IKBD_CTRL));                         /* 0077F6  move.b  $fffc00.l,d0 */
    SETB(d0, rd8(HW_IKBD_DATA));                         /* 0077FC  move.b  $fffc02.l,d0 */
    SETB(d0, rd8(0xFFFC04));                             /* 007802  move.b  $fffc04.l,d0 */
    SETB(d0, rd8(0xFFFC06));                             /* 007808  move.b  $fffc06.l,d0 */
    wr8(HW_MFP_ISRB, rd8(HW_MFP_ISRB) & ~0x40);          /* 00780E  bclr  #$6,$fffa11.l */
    RTS();                                               /* 007816  rts */
}

/* 007818  game routine */
void sub_007818(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x7818U);
        SETB(d1, rd8(HW_IKBD_CTRL));                         /* 007818  move.b  $fffc00.l,d1 */
        fz = !(d1 & 0x2);                                    /* 00781E  btst  #$1,d1 */
        /* 007822  beq  $7818 */
    } while (COND(7));
L_007824:
    wr8(HW_IKBD_DATA, (d0 & 0xFF));                      /* 007824  move.b  d0,$fffc02.l */
    RTS();                                               /* 00782A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7824U: goto L_007824;
        }
    }
}

/* 00782C  game routine */
void sub_00782C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00782C  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    /* 00782E  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_007832:
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 007832  bclr  #$5,$fffa0f.l */
    wr8(0xFFFA0B, rd8(0xFFFA0B) & ~0x20);                /* 00783A  bclr  #$5,$fffa0b.l */
    wr8(HW_MFP_IMRA, rd8(HW_MFP_IMRA) | 0x20);           /* 007842  bset  #$5,$fffa13.l */
    wr8(HW_MFP_IERA, rd8(HW_MFP_IERA) | 0x20);           /* 00784A  bset  #$5,$fffa07.l */
    wr8(HW_MFP_TADR, 0x32);                              /* 007852  move.b  #$32,$fffa1f.l */
    wr8(HW_MFP_TACR, 2);                                 /* 00785A  move.b  #$2,$fffa19.l */
    /* 007862  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_007864:
    RTS();                                               /* 007864  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7832U: goto L_007832;
        case 0x7864U: goto L_007864;
        }
    }
}

/* 007866  game routine */
void sub_007866(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 007866  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    /* 007868  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_00786C:
    wr8(HW_MFP_IERA, rd8(HW_MFP_IERA) & ~0x20);          /* 00786C  bclr  #$5,$fffa07.l */
    wr8(HW_MFP_IMRA, rd8(HW_MFP_IMRA) & ~0x20);          /* 007874  bclr  #$5,$fffa13.l */
    wr8(HW_MFP_TACR, 0);                                 /* 00787C  clr.b  $fffa19.l */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 007882  bclr  #$5,$fffa0f.l */
    wr8(0xFFFA0B, rd8(0xFFFA0B) & ~0x20);                /* 00788A  bclr  #$5,$fffa0b.l */
    /* 007892  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_007894:
    RTS();                                               /* 007894  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x786CU: goto L_00786C;
        case 0x7894U: goto L_007894;
        }
    }
}

/* 007896  Timer C (vector $114): music player (the game programs it at ~48.6 Hz) */
void timer_c_isr(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr8(HW_MFP_ISRB, rd8(HW_MFP_ISRB) & ~0x20);          /* 007896  bclr  #$5,$fffa11.l */
    /* 00789E  move  #$2500,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2500U;
    set_sr(s); }
L_0078A2:
    CALL(sub_00750C, 0x78A6U);                           /* 0078A2  bsr  $750c */
L_0078A6:
    RTE();                                               /* 0078A6  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x78A2U: goto L_0078A2;
        case 0x78A6U: goto L_0078A6;
        }
    }
}

/* 0078A8  VBL interrupt (vector $70): palette, counters and game timing */
void vbl_isr(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0078A8  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
    PUSH32(a6);
    PUSH32(a5);
    PUSH32(a4);
    PUSH32(a3);
    PUSH32(a2);
    PUSH32(a1);
    PUSH32(a0);
    PUSH32(d7);
    PUSH32(d6);
    PUSH32(d5);
    PUSH32(d4);
    PUSH32(d3);
    PUSH32(d2);
    PUSH32(d1);
    PUSH32(d0);
    CALL(sub_00EAD8, 0x78B0U);                           /* 0078AC  bsr  $ead8 */
L_0078B0:
    /* 0078B0  tst.w  $1fe(a5) */
    /* 0078B4  beq  $78d0 */
    if (rd16(V_01FE) != 0) {
L_0078B6:
        a1 = 0xC72E;                                         /* 0078B6  lea  $c72e(pc),a1 */
        CALL(sub_0075E2, 0x78BEU);                           /* 0078BA  bsr  $75e2 */
L_0078BE:
        wr8(HW_MFP_TBDR, 0x30);                              /* 0078BE  move.b  #$30,$fffa21.l */
        wr8(HW_MFP_TBCR, 8);                                 /* 0078C6  move.b  #$8,$fffa1b.l */
        /* 0078CE  bra  $7910 */
    } else {
L_0078D0:
        /* 0078D0  tst.w  $200(a5) */
        /* 0078D4  beq  $78f0 */
        if (rd16(V_0200) != 0) {
L_0078D6:
            a1 = 0xEB76;                                         /* 0078D6  lea  $eb76(pc),a1 */
            CALL(sub_0075E2, 0x78DEU);                           /* 0078DA  bsr  $75e2 */
L_0078DE:
            wr8(HW_MFP_TBDR, 0x96);                              /* 0078DE  move.b  #$96,$fffa21.l */
            wr8(HW_MFP_TBCR, 8);                                 /* 0078E6  move.b  #$8,$fffa1b.l */
            /* 0078EE  bra  $7910 */
        } else {
L_0078F0:
            /* 0078F0  tst.w  $202(a5) */
            /* 0078F4  beq  $7910 */
            if (rd16(V_0202) != 0) {
L_0078F6:
                a1 = 0xEB76;                                         /* 0078F6  lea  $eb76(pc),a1 */
                CALL(sub_0075E2, 0x78FEU);                           /* 0078FA  bsr  $75e2 */
L_0078FE:
                wr8(HW_MFP_TBDR, 0x90);                              /* 0078FE  move.b  #$90,$fffa21.l */
                wr8(HW_MFP_TBCR, 8);                                 /* 007906  move.b  #$8,$fffa1b.l */
                /* 00790E  nop */
            }
        }
    }
L_007910:
    wr16(V_VBL_COUNTER, rd16(V_VBL_COUNTER) + 1);        /* 007910  addq.w  #1,$1dc6(a5) */
    /* 007914  subq.w  #1,$1df2(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_1DF2);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_1DF2, r);
        fn = !!(r & 0x8000);
    }
    /* 007918  bpl  $7920 */
    if (COND(11)) {
L_00791A:
        wr8(V_1DF3, rd8(V_1DF3) | 0x1);                      /* 00791A  bset  #$0,$1df3(a5) */
    }
L_007920:
    wr8(V_0270, 0xFF);                                   /* 007920  st  $270(a5) */
    CALL(sub_00E7C0, 0x7928U);                           /* 007924  bsr  $e7c0 */
L_007928:
    CALL(sub_008648, 0x792CU);                           /* 007928  bsr  $8648 */
L_00792C:
    /* 00792C  movem.l  (a7)+,d0-d7/a0-a6 */
    d0 = POP32();
    d1 = POP32();
    d2 = POP32();
    d3 = POP32();
    d4 = POP32();
    d5 = POP32();
    d6 = POP32();
    d7 = POP32();
    a0 = POP32();
    a1 = POP32();
    a2 = POP32();
    a3 = POP32();
    a4 = POP32();
    a5 = POP32();
    a6 = POP32();
    RTE();                                               /* 007930  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x78B0U: goto L_0078B0;
        case 0x78B6U: goto L_0078B6;
        case 0x78BEU: goto L_0078BE;
        case 0x78D0U: goto L_0078D0;
        case 0x78D6U: goto L_0078D6;
        case 0x78DEU: goto L_0078DE;
        case 0x78F0U: goto L_0078F0;
        case 0x78F6U: goto L_0078F6;
        case 0x78FEU: goto L_0078FE;
        case 0x7910U: goto L_007910;
        case 0x791AU: goto L_00791A;
        case 0x7920U: goto L_007920;
        case 0x7928U: goto L_007928;
        case 0x792CU: goto L_00792C;
        }
    }
}

/* 007932  Timer B (vector $120): palette changes per screen line */
void timer_b_isr(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 007932  tst.w  $1fe(a5) */
    /* 007936  beq  $794a */
    if (rd16(V_01FE) != 0) {
L_007938:
        /* 007938  movem.l  a1/a0,-(a7) */
        PUSH32(a1);
        PUSH32(a0);
        a1 = 0xC70E;                                         /* 00793C  lea  $c70e(pc),a1 */
        CALL(sub_0075E2, 0x7944U);                           /* 007940  bsr  $75e2 */
L_007944:
        /* 007944  movem.l  (a7)+,a0-a1 */
        a0 = POP32();
        a1 = POP32();
        /* 007948  bra  $79a2 */
    } else {
L_00794A:
        /* 00794A  tst.w  $200(a5) */
        /* 00794E  beq  $7962 */
        if (rd16(V_0200) != 0) {
L_007950:
            /* 007950  movem.l  a1/a0,-(a7) */
            PUSH32(a1);
            PUSH32(a0);
            a1 = 0xEBB6;                                         /* 007954  lea  $ebb6(pc),a1 */
            CALL(sub_0075E2, 0x795CU);                           /* 007958  bsr  $75e2 */
L_00795C:
            /* 00795C  movem.l  (a7)+,a0-a1 */
            a0 = POP32();
            a1 = POP32();
            /* 007960  bra  $79a2 */
        } else {
L_007962:
            /* 007962  tst.w  $202(a5) */
            /* 007966  beq  $79a2 */
            if (rd16(V_0202) != 0) {
L_007968:
                /* 007968  movem.l  a1/a0,-(a7) */
                PUSH32(a1);
                PUSH32(a0);
                a1 = 0xEBD6;                                         /* 00796C  lea  $ebd6(pc),a1 */
                CALL(sub_0075E2, 0x7974U);                           /* 007970  bsr  $75e2 */
L_007974:
                a0 = 0x79B2;                                         /* 007974  lea  $79b2(pc),a0 */
                wr32(V_0120, a0);                                    /* 007978  move.l  a0,$120.l */
                /* 00797E  movem.l  (a7)+,a0-a1 */
                a0 = POP32();
                a1 = POP32();
                wr8(HW_MFP_TBCR, 0);                                 /* 007982  clr.b  $fffa1b.l */
                wr8(HW_MFP_TBDR, 0x28);                              /* 007988  move.b  #$28,$fffa21.l */
                wr8(HW_MFP_TBCR, 8);                                 /* 007990  move.b  #$8,$fffa1b.l */
                /* 007998  bclr  #$0,$fffa0f.l */
                wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x1);
                { sub_0079A0(); return; }
            }
        }
    }
L_0079A2:
    wr8(HW_MFP_TBCR, 0);                                 /* 0079A2  clr.b  $fffa1b.l */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x1);           /* 0079A8  bclr  #$0,$fffa0f.l */
    RTE();                                               /* 0079B0  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7938U: goto L_007938;
        case 0x7944U: goto L_007944;
        case 0x794AU: goto L_00794A;
        case 0x7950U: goto L_007950;
        case 0x795CU: goto L_00795C;
        case 0x7962U: goto L_007962;
        case 0x7968U: goto L_007968;
        case 0x7974U: goto L_007974;
        case 0x79A2U: goto L_0079A2;
        }
    }
}

/* 0079A0  game routine */
void sub_0079A0(void)
{
    RTE();                                               /* 0079A0  rte */
}

/* 0079B2  game routine */
void sub_0079B2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0079B2  movem.l  a1/a0,-(a7) */
    PUSH32(a1);
    PUSH32(a0);
    a0 = 0x7932;                                         /* 0079B6  lea  $7932(pc),a0 */
    wr32(V_0120, a0);                                    /* 0079BA  move.l  a0,$120.l */
    a1 = 0xEB76;                                         /* 0079C0  lea  $eb76(pc),a1 */
    CALL(sub_0075E2, 0x79C8U);                           /* 0079C4  bsr  $75e2 */
L_0079C8:
    wr8(HW_MFP_TBCR, 0);                                 /* 0079C8  clr.b  $fffa1b.l */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x1);           /* 0079CE  bclr  #$0,$fffa0f.l */
    /* 0079D6  movem.l  (a7)+,a0-a1 */
    a0 = POP32();
    a1 = POP32();
    RTE();                                               /* 0079DA  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x79C8U: goto L_0079C8;
        }
    }
}

/* 0079DC  Waits for a VBL to pass (loop on the V_VBL_COUNTER counter) */
void wait_vbl(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0079DC  move.w  d0,-(a7) */
    IDLE_ENTRY();
    {
        uint32_t v = (d0 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    SETW(d0, rd16(V_VBL_COUNTER));                       /* 0079DE  move.w  $1dc6(a5),d0 */
    do {
        BUDGET_CHECK(0x79E2U);
L_0079E2:
        /* 0079E2  cmp.w  $1dc6(a5),d0 */
        /* 0079E6  beq  $79e2 */
        IDLE_SPIN(0x79E2U);
    } while ((d0 & 0xFFFF) == rd16(V_VBL_COUNTER));
L_0079E8:
    /* 0079E8  move.w  (a7)+,d0 */
    SETW(d0, rd16(a7));
    a7 += 2;
    RTS();                                               /* 0079EA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x79E2U: goto L_0079E2;
        case 0x79E8U: goto L_0079E8;
        }
    }
}

/* 0079EC  Keyboard/joystick (IKBD ACIA, vector $118) */
void acia_isr(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0079EC  movem.l  a5/a4/a0/d1/d0,-(a7) */
    PUSH32(a5);
    PUSH32(a4);
    PUSH32(a0);
    PUSH32(d1);
    PUSH32(d0);
    SETB(d0, rd8(0xFFFC04));                             /* 0079F0  move.b  $fffc04.l,d0 */
    fz = !(d0 & 0x80);                                   /* 0079F6  btst  #$7,d0 */
    /* 0079FA  beq  $7a02 */
    if (COND(6)) {
        SETB(d0, rd8(0xFFFC06));                             /* 0079FC  move.b  $fffc06.l,d0 */
    }
L_007A02:
    SETB(d0, rd8(HW_IKBD_CTRL));                         /* 007A02  move.b  $fffc00.l,d0 */
    fz = !(d0 & 0x80);                                   /* 007A08  btst  #$7,d0 */
    /* 007A0C  beq  $7a10 */
    if (COND(6)) {
L_007A0E:
        CALL(sub_007A1E, 0x7A10U);                           /* 007A0E  bsr  $7a1e */
    }
L_007A10:
    wr8(HW_MFP_ISRB, rd8(HW_MFP_ISRB) & ~0x40);          /* 007A10  bclr  #$6,$fffa11.l */
    /* 007A18  movem.l  (a7)+,d0-d1/a0/a4-a5 */
    d0 = POP32();
    d1 = POP32();
    a0 = POP32();
    a4 = POP32();
    a5 = POP32();
    RTE();                                               /* 007A1C  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7A02U: goto L_007A02;
        case 0x7A0EU: goto L_007A0E;
        case 0x7A10U: goto L_007A10;
        }
    }
}

/* 007A1E  game routine */
void sub_007A1E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETB(d0, rd8(HW_IKBD_DATA));                         /* 007A1E  move.b  $fffc02.l,d0 */
    /* 007A24  tst.b  $1dd4(a5) */
    /* 007A28  beq  $7a30 */
    if (rd8(V_1DD4) == 0) {
        goto L_007A30;
    }
L_007A2A:
    CALL(sub_007ADA, 0x7A2EU);                           /* 007A2A  bsr  $7ada */
L_007A2E:
    RTS();                                               /* 007A2E  rts */
L_007A30:
    /* 007A30  cmp.b  #$f6,d0 */
    /* 007A34  bcs  $7a3c */
    if ((d0 & 0xFF) >= 0xF6) {
L_007A36:
        wr8(V_1DD4, (d0 & 0xFF));                            /* 007A36  move.b  d0,$1dd4(a5) */
        /* 007A3A  bra  $7a2e */
        BUDGET_CHECK(0x7A2EU);
        goto L_007A2E;
    }
L_007A3C:
    wr8(V_1DE2, (d0 & 0xFF));                            /* 007A3C  move.b  d0,$1de2(a5) */
    /* 007A40  move.l  d0,-(a7) */
    {
        uint32_t v = d0;
        a7 -= 4;
        wr32(a7, v);
    }
    CALL(sub_007A48, 0x7A44U);                           /* 007A42  bsr  $7a48 */
L_007A44:
    /* 007A44  move.l  (a7)+,d0 */
    d0 = rd32(a7);
    a7 += 4;
    /* 007A46  bra  $7a2e */
    BUDGET_CHECK(0x7A2EU);
    goto L_007A2E;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7A2AU: goto L_007A2A;
        case 0x7A2EU: goto L_007A2E;
        case 0x7A30U: goto L_007A30;
        case 0x7A36U: goto L_007A36;
        case 0x7A3CU: goto L_007A3C;
        case 0x7A44U: goto L_007A44;
        }
    }
}

/* 007A48  game routine */
void sub_007A48(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_007A48;
    for (;;) {
        BUDGET_CHECK(0x2104U);
L_002104:
        /* 002104  cmp.b  #$e5,d0 */
        /* 002108  beq  $213e */
        if ((d0 & 0xFF) != 0xE5) {
L_00210A:
            /* 00210A  cmp.b  #$e6,d0 */
            /* 00210E  beq  $2118 */
            if ((d0 & 0xFF) == 0xE6) { goto L_002118; }
            { sub_002110(); return; }
L_002118:
            /* 002118  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
            PUSH32(a6);
            PUSH32(a5);
            PUSH32(a4);
            PUSH32(a3);
            PUSH32(a2);
            PUSH32(a1);
            PUSH32(a0);
            PUSH32(d7);
            PUSH32(d6);
            PUSH32(d5);
            PUSH32(d4);
            PUSH32(d3);
            PUSH32(d2);
            PUSH32(d1);
            PUSH32(d0);
            /* 00211C  moveq  #55,d0 */
            d0 = 0x37;
            FL_LOGIC(d0, 0xFFFFFFFF, 0x80000000);
            /* 00211E  bra  $2154 */
        } else {
            /* 00213E  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
            PUSH32(a6);
            PUSH32(a5);
            PUSH32(a4);
            PUSH32(a3);
            PUSH32(a2);
            PUSH32(a1);
            PUSH32(a0);
            PUSH32(d7);
            PUSH32(d6);
            PUSH32(d5);
            PUSH32(d4);
            PUSH32(d3);
            PUSH32(d2);
            PUSH32(d1);
            PUSH32(d0);
            a0 = rd32(0x20FA);                                   /* 002142  movea.l  $20fa(pc),a0 */
            a0 -= 8;                                             /* 002146  subq.l  #8,a0 */
            /* 002148  move.l  #$6356616c,(a0)+ */
            {
                uint32_t v = 0x6356616C;
                wr32(a0, v);
                a0 += 4;
            }
            wr32(a0, rd32(0x20EE));                              /* 00214E  move.l  $20ee(pc),(a0) */
            /* 002152  moveq  #0,d0 */
            d0 = 0;
            FL_LOGIC(d0, 0xFFFFFFFF, 0x80000000);
        }
        a0 = 0x21D4;                                         /* 002154  lea  $21d4(pc),a0 */
        /* 002158  move  sr,$50(a0) */
        { uint32_t ea; (void)ea;
        ea = a0 + 0x50U;
        wr16(ea, get_sr());
        }
        /* 00215C  move  #$2700,sr */
        { uint32_t s, ea1; (void)ea1;
        s = 0x2700U;
        set_sr(s); }
        a2 = 0x20EE;                                         /* 002160  lea  $20ee(pc),a2 */
        wr32(a2, 0xFFFFFFFF);                                /* 002164  move.l  #$ffffffff,(a2) */
        a2 = 0x2120;                                         /* 00216A  lea  $2120(pc),a2 */
        a1 = rd32(0x217A);                                   /* 00216E  movea.l  $217a(pc),a1 */
        /* 002172  move.w  $c(a1),d1 */
        {
            uint32_t v = rd16(a1 + 0xC);
            SETW(d1, v);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
        /* 002176  jsr  $0(a1,d1.w) */
        { uint32_t ea;
        ea = a1 + 0x0U + SX16(d1);
        CALL_DYN(ea, 0x217AU); }
        MISS(0x217AU);
L_007A48:
        /* 007A48  tst.w  $252(a5) */
        /* 007A4C  beq  $7a66 */
        if (rd16(V_0252) != 0) {
            /* 007A4E  cmp.b  #$70,d0 */
            /* 007A52  beq  $8282 */
            if ((d0 & 0xFF) == 0x70) {
                goto L_008282;
            }
            /* 007A56  cmp.b  #$72,d0 */
            /* 007A5A  beq  $8318 */
            if ((d0 & 0xFF) == 0x72) {
                goto L_008318;
            }
            /* 007A5E  cmp.b  #$f,d0 */
            /* 007A62  beq  $8318 */
            if ((d0 & 0xFF) == 0xF) {
                goto L_008318;
            }
        }
L_007A66:
        /* 007A66  jmp  $2104.w */
    }
L_008282:
    /* 008282  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    CALL(sub_0083B2, 0x828AU);                           /* 008286  bsr  $83b2 */
L_00828A:
    CALL(sub_007626, 0x828EU);                           /* 00828A  bsr  $7626 */
L_00828E:
    CALL(sub_00765A, 0x8292U);                           /* 00828E  bsr  $765a */
L_008292:
    /* 008292  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    wr16(V_0266, 0x7800);                                /* 008296  move.w  #$7800,$266(a5) */
    wr16(V_0268, 0xA);                                   /* 00829C  move.w  #$a,$268(a5) */
    wr16(V_026A, 0xA);                                   /* 0082A2  move.w  #$a,$26a(a5) */
    wr16(V_026C, 5);                                     /* 0082A8  move.w  #$5,$26c(a5) */
    a0 = 0x826B;                                         /* 0082AE  lea  $826b(pc),a0 */
    CALL(sub_008584, 0x82B6U);                           /* 0082B2  bsr  $8584 */
L_0082B6:
    a1 = 0x73B92;                                        /* 0082B6  lea  $73b92.l,a1 */
    a0 = HW_YM_SELECT;                                   /* 0082BC  lea  $ff8800.l,a0 */
    wr8(a0, 7);                                          /* 0082C2  move.b  #$7,(a0) */
    SETB(d0, rd8(a0));                                   /* 0082C6  move.b  (a0),d0 */
    d0 &= ~0x80;                                         /* 0082C8  bclr  #$7,d0 */
    wr8(a0 + 0x2, (d0 & 0xFF));                          /* 0082CC  move.b  d0,$2(a0) */
    wr8(0xFFFA05, rd8(0xFFFA05) & ~0x1);                 /* 0082D0  bclr  #$0,$fffa05.l */
    a6 = HW_IKBD_DATA;                                   /* 0082D8  lea  $fffc02.l,a6 */
    d6 = 0x19;                                           /* 0082DE  moveq  #25,d6 */
    SETB(d0, rd8(a0));                                   /* 0082E0  move.b  (a0),d0 */
    SETB(d1, (d0 & 0xFF));                               /* 0082E2  move.b  d0,d1 */
    d0 &= ~0x20;                                         /* 0082E4  bclr  #$5,d0 */
    d1 |= 0x20;                                          /* 0082E8  bset  #$5,d1 */
    d2 = 0xE;                                            /* 0082EC  moveq  #14,d2 */
    a2 = a0 + 0x2;                                       /* 0082EE  lea  $2(a0),a2 */
    d3 = 0x19;                                           /* 0082F2  moveq  #25,d3 */
    a4 = HW_MFP_GPIP;                                    /* 0082F4  lea  $fffa01.l,a4 */
    d4 = 0;                                              /* 0082FA  moveq  #0,d4 */
    d5 = 0xF;                                            /* 0082FC  moveq  #15,d5 */
L_0082FE:
    wr8(a0, (d2 & 0xFF));                                /* 0082FE  move.b  d2,(a0) */
    wr8(a2, (d0 & 0xFF));                                /* 008300  move.b  d0,(a2) */
    do {
        BUDGET_CHECK(0x8302U);
L_008302:
        fz = !(rd8(a4) & (1u << (d4 & 7)));                  /* 008302  btst  d4,(a4) */
        /* 008304  bne  $830e */
        if (COND(7)) {
            wr8(a2, (d1 & 0xFF));                                /* 008306  move.b  d1,(a2) */
            wr8(a0, (d5 & 0xFF));                                /* 008308  move.b  d5,(a0) */
            /* 00830A  move.b  (a0),(a1)+ */
            {
                uint32_t v = rd8(a0);
                wr8(a1, v);
                a1 += 1;
            }
            /* 00830C  bra  $82fe */
            BUDGET_CHECK(0x82FEU);
            goto L_0082FE;
        }
        /* 00830E  cmp.b  (a6),d3 */
        /* 008310  bne  $8302 */
    } while ((d3 & 0xFF) != rd8(a6));
    wr8(a2, (d1 & 0xFF));                                /* 008312  move.b  d1,(a2) */
    /* 008314  bra  $373a */
    { sub_00373A(); return; }
L_008318:
    CALL(sub_041BE8, 0x831EU);                           /* 008318  jsr  $41be8.l */
L_00831E:
    /* 00831E  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    CALL(sub_007626, 0x8326U);                           /* 008322  bsr  $7626 */
L_008326:
    CALL(sub_00765A, 0x832AU);                           /* 008326  bsr  $765a */
L_00832A:
    /* 00832A  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    wr16(V_0266, 0x7800);                                /* 00832E  move.w  #$7800,$266(a5) */
    wr16(V_0268, 0xA);                                   /* 008334  move.w  #$a,$268(a5) */
    wr16(V_026A, 0xA);                                   /* 00833A  move.w  #$a,$26a(a5) */
    wr16(V_026C, 0xF);                                   /* 008340  move.w  #$f,$26c(a5) */
    a0 = 0x825E;                                         /* 008346  lea  $825e(pc),a0 */
    CALL(sub_008584, 0x834EU);                           /* 00834A  bsr  $8584 */
L_00834E:
    a1 = 0x20000;                                        /* 00834E  lea  $20000.l,a1 */
    a0 = HW_YM_SELECT;                                   /* 008354  lea  $ff8800.l,a0 */
    wr8(a0, 7);                                          /* 00835A  move.b  #$7,(a0) */
    SETB(d0, rd8(a0));                                   /* 00835E  move.b  (a0),d0 */
    d0 &= ~0x80;                                         /* 008360  bclr  #$7,d0 */
    wr8(a0 + 0x2, (d0 & 0xFF));                          /* 008364  move.b  d0,$2(a0) */
    wr8(0xFFFA05, rd8(0xFFFA05) & ~0x1);                 /* 008368  bclr  #$0,$fffa05.l */
    a6 = HW_IKBD_DATA;                                   /* 008370  lea  $fffc02.l,a6 */
    d6 = 0x19;                                           /* 008376  moveq  #25,d6 */
    SETB(d0, rd8(a0));                                   /* 008378  move.b  (a0),d0 */
    SETB(d1, (d0 & 0xFF));                               /* 00837A  move.b  d0,d1 */
    d0 &= ~0x20;                                         /* 00837C  bclr  #$5,d0 */
    d1 |= 0x20;                                          /* 008380  bset  #$5,d1 */
    d2 = 0xE;                                            /* 008384  moveq  #14,d2 */
    a2 = a0 + 0x2;                                       /* 008386  lea  $2(a0),a2 */
    d3 = 0x19;                                           /* 00838A  moveq  #25,d3 */
    a4 = HW_MFP_GPIP;                                    /* 00838C  lea  $fffa01.l,a4 */
    d4 = 0;                                              /* 008392  moveq  #0,d4 */
    d5 = 0xF;                                            /* 008394  moveq  #15,d5 */
L_008396:
    wr8(a0, (d2 & 0xFF));                                /* 008396  move.b  d2,(a0) */
    wr8(a2, (d0 & 0xFF));                                /* 008398  move.b  d0,(a2) */
    do {
        BUDGET_CHECK(0x839AU);
L_00839A:
        fz = !(rd8(a4) & (1u << (d4 & 7)));                  /* 00839A  btst  d4,(a4) */
        /* 00839C  bne  $83a6 */
        if (COND(7)) {
            wr8(a2, (d1 & 0xFF));                                /* 00839E  move.b  d1,(a2) */
            wr8(a0, (d5 & 0xFF));                                /* 0083A0  move.b  d5,(a0) */
            /* 0083A2  move.b  (a0),(a1)+ */
            {
                uint32_t v = rd8(a0);
                wr8(a1, v);
                a1 += 1;
            }
            /* 0083A4  bra  $8396 */
            BUDGET_CHECK(0x8396U);
            goto L_008396;
        }
        /* 0083A6  cmp.b  (a6),d3 */
        /* 0083A8  bne  $839a */
    } while ((d3 & 0xFF) != rd8(a6));
    wr8(a2, (d1 & 0xFF));                                /* 0083AA  move.b  d1,(a2) */
    /* 0083AC  jmp  $2001c.l */
    /* 02001C  ori.b  #$3e,$f72f.w */
    {
        uint32_t s = 0x3E;
        uint32_t d = rd8(0xFFF72F);
        uint32_t r = (d | s) & 0xFF;
        wr8(0xFFF72F, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x20022U);

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2104U: goto L_002104;
        case 0x210AU: goto L_00210A;
        case 0x7A66U: goto L_007A66;
        case 0x828AU: goto L_00828A;
        case 0x828EU: goto L_00828E;
        case 0x8292U: goto L_008292;
        case 0x82B6U: goto L_0082B6;
        case 0x82FEU: goto L_0082FE;
        case 0x8302U: goto L_008302;
        case 0x831EU: goto L_00831E;
        case 0x8326U: goto L_008326;
        case 0x832AU: goto L_00832A;
        case 0x834EU: goto L_00834E;
        case 0x8396U: goto L_008396;
        case 0x839AU: goto L_00839A;
        }
    }
}

/* 007ADA  game routine */
void sub_007ADA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_02DA;                                         /* 007ADA  lea  $2da(a5),a4 */
    /* 007ADE  cmpi.b  #$ff,$1dd4(a5) */
    /* 007AE4  beq  $7b04 */
    if (rd8(V_1DD4) != 0xFF) {
        a4 += 0x9C;                                          /* 007AE6  adda.l  #$9c,a4 */
        /* 007AEC  cmpi.b  #$fe,$1dd4(a5) */
        /* 007AF2  beq  $7b04 */
        if (rd8(V_1DD4) != 0xFE) {
            wr8(V_1DD4, 0xFE);                                   /* 007AF4  move.b  #$fe,$1dd4(a5) */
            CALL(sub_007ADA, 0x7AFCU);                           /* 007AFA  bsr  $7ada */
L_007AFC:
            wr8(V_1DD4, 0xFF);                                   /* 007AFC  move.b  #$ff,$1dd4(a5) */
            RTS();                                               /* 007B02  rts */
        }
    }
L_007B04:
    fz = !(d0 & 0x80);                                   /* 007B04  btst  #$7,d0 */
    wr8(a4 + 0x12, (COND(6) ? 0xFF : 0));                /* 007B08  sne  $12(a4) */
    SETB(d0, shift_op(1, 0, (d0 & 0xFF), 1, 8));         /* 007B0C  lsr.b  #1,d0 */
    wr8(a4 + 0x10, (COND(5) ? 0xFF : 0));                /* 007B0E  scs  $10(a4) */
    SETB(d0, shift_op(1, 0, (d0 & 0xFF), 1, 8));         /* 007B12  lsr.b  #1,d0 */
    wr8(a4 + 0x11, (COND(5) ? 0xFF : 0));                /* 007B14  scs  $11(a4) */
    SETB(d0, shift_op(1, 0, (d0 & 0xFF), 1, 8));         /* 007B18  lsr.b  #1,d0 */
    wr8(a4 + 0xF, (COND(5) ? 0xFF : 0));                 /* 007B1A  scs  $f(a4) */
    SETB(d0, shift_op(1, 0, (d0 & 0xFF), 1, 8));         /* 007B1E  lsr.b  #1,d0 */
    wr8(a4 + 0xE, (COND(5) ? 0xFF : 0));                 /* 007B20  scs  $e(a4) */
    wr8(V_1DD4, 0);                                      /* 007B24  clr.b  $1dd4(a5) */
    RTS();                                               /* 007B28  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7AFCU: goto L_007AFC;
        case 0x7B04U: goto L_007B04;
        }
    }
}

/* 007B2A  game routine */
void sub_007B2A(void)
{
    d0 = d0 >> 8;                                        /* 007B2A  lsr.l  #8,d0 */
    wr8(HW_VIDEO_BASE_MID, (d0 & 0xFF));                 /* 007B2C  move.b  d0,$ff8203.l */
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 8, 16));      /* 007B32  lsr.w  #8,d0 */
    wr8(HW_VIDEO_BASE_HI, (d0 & 0xFF));                  /* 007B34  move.b  d0,$ff8201.l */
    RTS();                                               /* 007B3A  rts */
}

/* 007B3C  game routine */
void sub_007B3C(void)
{
    /* 007B3C  move.w  #$2,$20000.l */
    {
        uint32_t v = 2;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B44  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B46  game routine */
void sub_007B46(void)
{
    /* 007B46  move.w  #$3,$20000.l */
    {
        uint32_t v = 3;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B4E  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B50  game routine */
void sub_007B50(void)
{
    /* 007B50  move.w  #$4,$20000.l */
    {
        uint32_t v = 4;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B58  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B5A  game routine */
void sub_007B5A(void)
{
    /* 007B5A  move.w  #$5,$20000.l */
    {
        uint32_t v = 5;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B62  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B64  game routine */
void sub_007B64(void)
{
    /* 007B64  move.w  #$6,$20000.l */
    {
        uint32_t v = 6;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B6C  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B6E  game routine */
void sub_007B6E(void)
{
    /* 007B6E  move.w  #$7,$20000.l */
    {
        uint32_t v = 7;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B76  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B78  game routine */
void sub_007B78(void)
{
    /* 007B78  move.w  #$8,$20000.l */
    {
        uint32_t v = 8;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B80  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B82  game routine */
void sub_007B82(void)
{
    /* 007B82  move.w  #$9,$20000.l */
    {
        uint32_t v = 9;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B8A  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B8C  game routine */
void sub_007B8C(void)
{
    /* 007B8C  move.w  #$a,$20000.l */
    {
        uint32_t v = 0xA;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 007B94  bra  $7b9e */
    { sub_007B9E(); return; }
}

/* 007B96  game routine */
void sub_007B96(void)
{
    /* 007B96  move.w  #$b,$20000.l */
    {
        uint32_t v = 0xB;
        wr16(0x20000, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    { sub_007B9E(); return; }
}

/* 007B9E  game routine */
void sub_007B9E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 007B9E  move  sr,$3c8.l */
    { uint32_t ea; (void)ea;
    ea = 0x3C8U;
    wr16(ea, get_sr());
    }
    /* 007BA4  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    wr32(V_0380, 0x54494D2E);                            /* 007BA8  move.l  #$54494d2e,$380.l */
    /* 007BB2  movem.l  d0-d7,$384.l */
    { uint32_t ea;
    ea = 0x384U;
    wr32(ea, d0); ea += 4;
    wr32(ea, d1); ea += 4;
    wr32(ea, d2); ea += 4;
    wr32(ea, d3); ea += 4;
    wr32(ea, d4); ea += 4;
    wr32(ea, d5); ea += 4;
    wr32(ea, d6); ea += 4;
    wr32(ea, d7); ea += 4;
    }
    /* 007BBA  movem.l  a0-a7,$3a4.l */
    { uint32_t ea;
    ea = 0x3A4U;
    wr32(ea, a0); ea += 4;
    wr32(ea, a1); ea += 4;
    wr32(ea, a2); ea += 4;
    wr32(ea, a3); ea += 4;
    wr32(ea, a4); ea += 4;
    wr32(ea, a5); ea += 4;
    wr32(ea, a6); ea += 4;
    wr32(ea, a7); ea += 4;
    }
    a0 = V_03C4;                                         /* 007BC2  lea  $3c4.l,a0 */
    /* 007BC8  move.w  $20000.l,(a0)+ */
    {
        uint32_t v = rd16(0x20000);
        wr16(a0, v);
        a0 += 2;
    }
    /* 007BCE  move.w  (a7)+,d6 */
    SETW(d6, rd16(a7));
    a7 += 2;
    /* 007BD0  move.w  (a7)+,(a0)+ */
    {
        uint32_t v = rd16(a7);
        a7 += 2;
        wr16(a0, v);
        a0 += 2;
    }
    /* 007BD2  move.l  (a7)+,(a0)+ */
    {
        uint32_t v = rd32(a7);
        a7 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 007BD4  clr.l  (a0)+ */
    wr32(a0, 0);
    a0 += 4;
    d0 = 0x27;                                           /* 007BD6  moveq  #39,d0 */
    do {
        BUDGET_CHECK(0x7BD8U);
L_007BD8:
        /* 007BD8  move.l  (a7)+,(a0)+ */
        {
            uint32_t v = rd32(a7);
            a7 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 007BDA  dbra  d0,$7bd8 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
    a1 = 0x2C00;                                         /* 007BDE  lea  $2c00(pc),a1 */
    wr32(a0, a1);                                        /* 007BE2  move.l  a1,(a0) */
    a5 = 0x0;                                            /* 007BE4  lea  $0.l,a5 */
    a7 = 0x2400;                                         /* 007BEA  lea  $2400.l,a7 */
    d0 = 0x20000;                                        /* 007BF0  move.l  #$20000,d0 */
    wr32(V_DRAW_BUFFER, d0);                             /* 007BF6  move.l  d0,$28e(a5) */
    CALL(sub_007B2A, 0x7BFEU);                           /* 007BFA  bsr  $7b2a */
L_007BFE:
    a0 = rd32(V_DRAW_BUFFER);                            /* 007BFE  movea.l  $28e(a5),a0 */
    d0 = 0;                                              /* 007C02  moveq  #0,d0 */
    SETW(d1, 0x1FFF);                                    /* 007C04  move.w  #$1fff,d1 */
    do {
        BUDGET_CHECK(0x7C08U);
L_007C08:
        /* 007C08  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 007C0A  dbra  d1,$7c08 */
        SETW(d1, d1 - 1);
    } while ((d1 & 0xFFFF) != 0xFFFF);
    wr16(HW_PALETTE, 0);                                 /* 007C0E  clr.w  $ff8240.l */
    wr16(0xFF8242, 0x780);                               /* 007C14  move.w  #$780,$ff8242.l */
    wr16(0xFF8244, 0x630);                               /* 007C1C  move.w  #$630,$ff8244.l */
    wr16(0xFF8246, 0x5D8);                               /* 007C24  move.w  #$5d8,$ff8246.l */
    wr16(0xFF8248, 0x60);                                /* 007C2C  move.w  #$60,$ff8248.l */
    wr16(0xFF824A, 0x606);                               /* 007C34  move.w  #$606,$ff824a.l */
    wr16(0xFF824C, 0x37);                                /* 007C3C  move.w  #$37,$ff824c.l */
    wr16(0xFF824E, 0x500);                               /* 007C44  move.w  #$500,$ff824e.l */
    wr16(V_0266, 0);                                     /* 007C4C  move.w  #$0,$266(a5) */
    wr16(V_0268, 0);                                     /* 007C52  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 007C58  move.w  #$0,$26a(a5) */
    wr16(V_026C, 3);                                     /* 007C5E  move.w  #$3,$26c(a5) */
    a0 = V_0380;                                         /* 007C64  lea  $380.l,a0 */
    d5 = 8;                                              /* 007C6A  moveq  #8,d5 */
    do {
        BUDGET_CHECK(0x7C6CU);
L_007C6C:
        /* 007C6C  move.l  (a0)+,d0 */
        d0 = rd32(a0);
        a0 += 4;
        CALL(sub_007D06, 0x7C72U);                           /* 007C6E  bsr  $7d06 */
L_007C72:
        /* 007C72  dbra  d5,$7c6c */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
    d0 = 0xD;                                            /* 007C76  moveq  #13,d0 */
    CALL(sub_00858E, 0x7C7CU);                           /* 007C78  bsr  $858e */
L_007C7C:
    d0 = 0xD;                                            /* 007C7C  moveq  #13,d0 */
    CALL(sub_00858E, 0x7C82U);                           /* 007C7E  bsr  $858e */
L_007C82:
    wr16(V_026C, 4);                                     /* 007C82  move.w  #$4,$26c(a5) */
    d5 = 7;                                              /* 007C88  moveq  #7,d5 */
    do {
        BUDGET_CHECK(0x7C8AU);
L_007C8A:
        /* 007C8A  move.l  (a0)+,d0 */
        d0 = rd32(a0);
        a0 += 4;
        CALL(sub_007D06, 0x7C8EU);                           /* 007C8C  bsr  $7d06 */
L_007C8E:
        /* 007C8E  dbra  d5,$7c8a */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
    wr16(V_026C, 6);                                     /* 007C92  move.w  #$6,$26c(a5) */
    wr16(V_0266, 0);                                     /* 007C98  move.w  #$0,$266(a5) */
    wr16(V_0268, 0x16);                                  /* 007C9E  move.w  #$16,$268(a5) */
    wr16(V_026A, 0x16);                                  /* 007CA4  move.w  #$16,$26a(a5) */
    d5 = 0x13;                                           /* 007CAA  moveq  #19,d5 */
    do {
        BUDGET_CHECK(0x7CACU);
L_007CAC:
        /* 007CAC  move.l  (a0)+,d0 */
        d0 = rd32(a0);
        a0 += 4;
        CALL(sub_007D06, 0x7CB0U);                           /* 007CAE  bsr  $7d06 */
L_007CB0:
        /* 007CB0  dbra  d5,$7cac */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
    a0 = 0x73B92;                                        /* 007CB4  lea  $73b92.l,a0 */
    wr16(V_0266, 0);                                     /* 007CBA  move.w  #$0,$266(a5) */
    wr16(V_0268, 0x20);                                  /* 007CC0  move.w  #$20,$268(a5) */
    wr16(V_026A, 0x20);                                  /* 007CC6  move.w  #$20,$26a(a5) */
    d5 = 0x13;                                           /* 007CCC  moveq  #19,d5 */
    do {
        BUDGET_CHECK(0x7CCEU);
L_007CCE:
        /* 007CCE  move.l  (a0)+,d0 */
        d0 = rd32(a0);
        a0 += 4;
        CALL(sub_007D06, 0x7CD2U);                           /* 007CD0  bsr  $7d06 */
L_007CD2:
        /* 007CD2  dbra  d5,$7cce */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
    wr16(V_026C, 7);                                     /* 007CD6  move.w  #$7,$26c(a5) */
    wr16(V_0266, 0);                                     /* 007CDC  move.w  #$0,$266(a5) */
    wr16(V_0268, 0xB);                                   /* 007CE2  move.w  #$b,$268(a5) */
    wr16(V_026A, 0xB);                                   /* 007CE8  move.w  #$b,$26a(a5) */
    d5 = 4;                                              /* 007CEE  moveq  #4,d5 */
    a0 = V_0194;                                         /* 007CF0  lea  $194(a5),a0 */
    do {
        BUDGET_CHECK(0x7CF4U);
L_007CF4:
        /* 007CF4  move.l  (a0)+,d0 */
        d0 = rd32(a0);
        a0 += 4;
        CALL(sub_007D06, 0x7CF8U);                           /* 007CF6  bsr  $7d06 */
L_007CF8:
        /* 007CF8  dbra  d5,$7cf4 */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
    CALL(sub_00765A, 0x7D00U);                           /* 007CFC  bsr  $765a */
L_007D00:
    /* 007D00  move  #$2300,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2300U;
    set_sr(s); }
    for (;;) {
        BUDGET_CHECK(0x7D04U);
L_007D04:
        /* 007D04  bra  $7d04 */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7BD8U: goto L_007BD8;
        case 0x7BFEU: goto L_007BFE;
        case 0x7C08U: goto L_007C08;
        case 0x7C6CU: goto L_007C6C;
        case 0x7C72U: goto L_007C72;
        case 0x7C7CU: goto L_007C7C;
        case 0x7C82U: goto L_007C82;
        case 0x7C8AU: goto L_007C8A;
        case 0x7C8EU: goto L_007C8E;
        case 0x7CACU: goto L_007CAC;
        case 0x7CB0U: goto L_007CB0;
        case 0x7CCEU: goto L_007CCE;
        case 0x7CD2U: goto L_007CD2;
        case 0x7CF4U: goto L_007CF4;
        case 0x7CF8U: goto L_007CF8;
        case 0x7D00U: goto L_007D00;
        case 0x7D04U: goto L_007D04;
        }
    }
}

/* 007D06  game routine */
void sub_007D06(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d4 = 7;                                              /* 007D06  moveq  #7,d4 */
    do {
        BUDGET_CHECK(0x7D08U);
L_007D08:
        d0 = shift_op(3, 1, d0, 4, 32);                      /* 007D08  rol.l  #4,d0 */
        /* 007D0A  movem.l  a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
        PUSH32(a0);
        PUSH32(d7);
        PUSH32(d6);
        PUSH32(d5);
        PUSH32(d4);
        PUSH32(d3);
        PUSH32(d2);
        PUSH32(d1);
        PUSH32(d0);
        SETW(d0, d0 & 0xF);                                  /* 007D0E  andi.w  #$f,d0 */
        /* 007D12  addi.b  #$30,d0 */
        {
            uint32_t s = 0x30;
            uint32_t d = (d0 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d0, r);
            fx = (r < s);
        }
        /* 007D16  cmp.b  #$39,d0 */
        /* 007D1A  bls  $7d20 */
        if ((d0 & 0xFF) > 0x39) {
            /* 007D1C  addi.b  #$7,d0 */
            {
                uint32_t s = 7;
                uint32_t d = (d0 & 0xFF);
                uint32_t r = (d + s) & 0xFF;
                SETB(d0, r);
                fx = (r < s);
            }
        }
        CALL(sub_00858E, 0x7D24U);                           /* 007D20  bsr  $858e */
L_007D24:
        /* 007D24  movem.l  (a7)+,d0-d7/a0 */
        d0 = POP32();
        d1 = POP32();
        d2 = POP32();
        d3 = POP32();
        d4 = POP32();
        d5 = POP32();
        d6 = POP32();
        d7 = POP32();
        a0 = POP32();
        /* 007D28  dbra  d4,$7d08 */
        SETW(d4, d4 - 1);
    } while ((d4 & 0xFFFF) != 0xFFFF);
    d0 = 0xD;                                            /* 007D2C  moveq  #13,d0 */
    /* 007D2E  bra  $858e */
    { sub_00858E(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7D08U: goto L_007D08;
        case 0x7D24U: goto L_007D24;
        }
    }
}

/* 007D38  game routine */
void sub_007D38(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_007D3E, 0x7D3AU);                           /* 007D38  bsr  $7d3e */
L_007D3A:
    /* 007D3A  bra  $8584 */
    { sub_008584(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7D3AU: goto L_007D3A;
        }
    }
}

/* 007D3E  game routine */
void sub_007D3E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr32(V_025E, 0x30303030);                            /* 007D3E  move.l  #$30303030,$25e(a5) */
    wr32(V_0262, 0x30303000);                            /* 007D46  move.l  #$30303000,$262(a5) */
    for (;;) {
        BUDGET_CHECK(0x7D4EU);
L_007D4E:
        /* 007D4E  cmp.l  #$98967f,d0 */
        /* 007D54  bls  $7d5e */
        if (d0 <= 0x98967F) {
            goto L_007D5E;
        }
        d0 -= 0x989680;                                      /* 007D56  subi.l  #$989680,d0 */
        /* 007D5C  bra  $7d4e */
    }
    for (;;) {
        BUDGET_CHECK(0x7D5EU);
L_007D5E:
        /* 007D5E  cmp.l  #$f423f,d0 */
        /* 007D64  bls  $7d74 */
        if (d0 <= 0xF423F) {
            goto L_007D74;
        }
        d0 -= 0xF4240;                                       /* 007D66  subi.l  #$f4240,d0 */
        wr8(V_025E, rd8(V_025E) + 1);                        /* 007D6C  addi.b  #$1,$25e(a5) */
        /* 007D72  bra  $7d5e */
    }
    for (;;) {
        BUDGET_CHECK(0x7D74U);
L_007D74:
        /* 007D74  cmp.l  #$1869f,d0 */
        /* 007D7A  bls  $7d8a */
        if (d0 <= 0x1869F) {
            goto L_007D8A;
        }
        d0 -= 0x186A0;                                       /* 007D7C  subi.l  #$186a0,d0 */
        wr8(V_025F, rd8(V_025F) + 1);                        /* 007D82  addi.b  #$1,$25f(a5) */
        /* 007D88  bra  $7d74 */
    }
    for (;;) {
        BUDGET_CHECK(0x7D8AU);
L_007D8A:
        /* 007D8A  cmp.l  #$270f,d0 */
        /* 007D90  bls  $7da0 */
        if (d0 <= 0x270F) {
            goto L_007DA0;
        }
L_007D92:
        d0 -= 0x2710;                                        /* 007D92  subi.l  #$2710,d0 */
        wr8(V_0260, rd8(V_0260) + 1);                        /* 007D98  addi.b  #$1,$260(a5) */
        /* 007D9E  bra  $7d8a */
    }
    for (;;) {
        BUDGET_CHECK(0x7DA0U);
L_007DA0:
        /* 007DA0  cmp.w  #$3e7,d0 */
        /* 007DA4  bls  $7db2 */
        if ((d0 & 0xFFFF) <= 0x3E7) {
            goto L_007DB2;
        }
L_007DA6:
        SETW(d0, d0 - 0x3E8);                                /* 007DA6  subi.w  #$3e8,d0 */
        wr8(V_0261, rd8(V_0261) + 1);                        /* 007DAA  addi.b  #$1,$261(a5) */
        /* 007DB0  bra  $7da0 */
    }
    for (;;) {
        BUDGET_CHECK(0x7DB2U);
L_007DB2:
        /* 007DB2  cmp.w  #$63,d0 */
        /* 007DB6  bls  $7dc4 */
        if ((d0 & 0xFFFF) <= 0x63) {
            goto L_007DC4;
        }
L_007DB8:
        SETW(d0, d0 - 0x64);                                 /* 007DB8  subi.w  #$64,d0 */
        wr8(V_0262, rd8(V_0262) + 1);                        /* 007DBC  addi.b  #$1,$262(a5) */
        /* 007DC2  bra  $7db2 */
    }
    for (;;) {
        BUDGET_CHECK(0x7DC4U);
L_007DC4:
        /* 007DC4  cmp.b  #$9,d0 */
        /* 007DC8  bls  $7dd6 */
        if ((d0 & 0xFF) <= 9) {
            goto L_007DD6;
        }
L_007DCA:
        SETB(d0, d0 - 0xA);                                  /* 007DCA  subi.b  #$a,d0 */
        wr8(V_0263, rd8(V_0263) + 1);                        /* 007DCE  addi.b  #$1,$263(a5) */
        /* 007DD4  bra  $7dc4 */
    }
L_007DD6:
    SETB(d0, d0 + 0x30);                                 /* 007DD6  addi.b  #$30,d0 */
    wr8(V_0264, (d0 & 0xFF));                            /* 007DDA  move.b  d0,$264(a5) */
    /* 007DDE  lea  $25e(a5),a0 */
    a0 = V_025E;
    { sub_007DE2(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7D4EU: goto L_007D4E;
        case 0x7D5EU: goto L_007D5E;
        case 0x7D74U: goto L_007D74;
        case 0x7D8AU: goto L_007D8A;
        case 0x7D92U: goto L_007D92;
        case 0x7DA0U: goto L_007DA0;
        case 0x7DA6U: goto L_007DA6;
        case 0x7DB2U: goto L_007DB2;
        case 0x7DB8U: goto L_007DB8;
        case 0x7DC4U: goto L_007DC4;
        case 0x7DCAU: goto L_007DCA;
        case 0x7DD6U: goto L_007DD6;
        }
    }
}

/* 007DE2  game routine */
void sub_007DE2(void)
{
    RTS();                                               /* 007DE2  rts */
}

/* 007E06  game routine */
void sub_007E06(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_007E06;
L_007DE4:
    a1 = a0 - 0x8000;                                    /* 007DE4  lea  -$8000(a0),a1 */
    do {
        BUDGET_CHECK(0x7DE8U);
L_007DE8:
        /* 007DE8  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007DEA  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007DEC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007DEE  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007DF0  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007DF2  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 007DF4  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        wr32(a1, rd32(a0));                                  /* 007DF6  move.l  (a0),(a1) */
        a0 = a0 + 0x84;                                      /* 007DF8  lea  $84(a0),a0 */
        a1 = a1 + 0x84;                                      /* 007DFC  lea  $84(a1),a1 */
        /* 007E00  dbra  d7,$7de8 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_007E04:
    RTS();                                               /* 007E04  rts */
L_007E06:
    /* 007E06  tst.w  $25a(a5) */
    /* 007E0A  bne  $7e44 */
    if (rd16(V_025A) == 0) {
L_007E0C:
        /* 007E0C  addq.w  #1,$25a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_025A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_025A, r);
            fx = (r < s);
        }
        wr16(V_0266, 0x1F40);                                /* 007E10  move.w  #$1f40,$266(a5) */
        wr16(V_0268, 1);                                     /* 007E16  move.w  #$1,$268(a5) */
        wr16(V_026A, 1);                                     /* 007E1C  move.w  #$1,$26a(a5) */
        a4 = V_02DA;                                         /* 007E22  lea  $2da(a5),a4 */
        /* 007E26  move.l  $28e(a5),-(a7) */
        {
            uint32_t v = rd32(V_DRAW_BUFFER);
            a7 -= 4;
            wr32(a7, v);
        }
        wr32(V_DRAW_BUFFER, 0x20000);                        /* 007E2A  move.l  #$20000,$28e(a5) */
        CALL(sub_008038, 0x7E36U);                           /* 007E32  bsr  $8038 */
L_007E36:
        /* 007E36  move.l  (a7)+,$28e(a5) */
        wr32(V_DRAW_BUFFER, rd32(a7));
        a7 += 4;
        a0 = 0x21F40;                                        /* 007E3A  lea  $21f40.l,a0 */
        d7 = 7;                                              /* 007E40  moveq  #7,d7 */
        /* 007E42  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007E44:
    /* 007E44  cmpi.w  #$1,$25a(a5) */
    /* 007E4A  bne  $7e86 */
    if (rd16(V_025A) == 1) {
L_007E4C:
        /* 007E4C  addq.w  #1,$25a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_025A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_025A, r);
            fx = (r < s);
        }
        wr16(V_0266, 0x1F40);                                /* 007E50  move.w  #$1f40,$266(a5) */
        wr16(V_0268, 0x21);                                  /* 007E56  move.w  #$21,$268(a5) */
        wr16(V_026A, 0x21);                                  /* 007E5C  move.w  #$21,$26a(a5) */
        a4 = V_0376;                                         /* 007E62  lea  $376(a5),a4 */
        /* 007E66  move.l  $28e(a5),-(a7) */
        {
            uint32_t v = rd32(V_DRAW_BUFFER);
            a7 -= 4;
            wr32(a7, v);
        }
        wr32(V_DRAW_BUFFER, 0x20000);                        /* 007E6A  move.l  #$20000,$28e(a5) */
        CALL(sub_008038, 0x7E76U);                           /* 007E72  bsr  $8038 */
L_007E76:
        /* 007E76  move.l  (a7)+,$28e(a5) */
        wr32(V_DRAW_BUFFER, rd32(a7));
        a7 += 4;
        a0 = 0x21FC0;                                        /* 007E7A  lea  $21fc0.l,a0 */
        d7 = 7;                                              /* 007E80  moveq  #7,d7 */
        /* 007E82  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007E86:
    /* 007E86  cmpi.w  #$2,$25a(a5) */
    /* 007E8C  bne  $7eac */
    if (rd16(V_025A) == 2) {
L_007E8E:
        /* 007E8E  addq.w  #1,$25a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_025A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_025A, r);
            fx = (r < s);
        }
        a4 = V_02DA;                                         /* 007E92  lea  $2da(a5),a4 */
        a0 = 0x26F41;                                        /* 007E96  lea  $26f41.l,a0 */
        CALL(sub_0080AA, 0x7EA0U);                           /* 007E9C  bsr  $80aa */
L_007EA0:
        a0 = 0x26F40;                                        /* 007EA0  lea  $26f40.l,a0 */
        d7 = 0xF;                                            /* 007EA6  moveq  #15,d7 */
        /* 007EA8  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007EAC:
    /* 007EAC  cmpi.w  #$3,$25a(a5) */
    /* 007EB2  bne  $7ed2 */
    if (rd16(V_025A) == 3) {
L_007EB4:
        /* 007EB4  addq.w  #1,$25a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_025A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_025A, r);
            fx = (r < s);
        }
        a4 = V_0376;                                         /* 007EB8  lea  $376(a5),a4 */
        a0 = 0x26FC1;                                        /* 007EBC  lea  $26fc1.l,a0 */
        CALL(sub_0080AA, 0x7EC6U);                           /* 007EC2  bsr  $80aa */
L_007EC6:
        a0 = 0x26FC0;                                        /* 007EC6  lea  $26fc0.l,a0 */
        d7 = 0xF;                                            /* 007ECC  moveq  #15,d7 */
        /* 007ECE  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007ED2:
    /* 007ED2  cmpi.w  #$4,$25a(a5) */
    /* 007ED8  bne  $7f1e */
    if (rd16(V_025A) == 4) {
L_007EDA:
        /* 007EDA  addq.w  #1,$25a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_025A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_025A, r);
            fx = (r < s);
        }
        /* 007EDE  tst.w  $2da(a5) */
        /* 007EE2  beq  $8036 */
        if (rd16(V_02DA) == 0) {
            goto L_008036;
        }
L_007EE6:
        wr16(V_0266, 0x7260);                                /* 007EE6  move.w  #$7260,$266(a5) */
        wr16(V_0268, 6);                                     /* 007EEC  move.w  #$6,$268(a5) */
        wr16(V_026A, 6);                                     /* 007EF2  move.w  #$6,$26a(a5) */
        d0 = 0;                                              /* 007EF8  moveq  #0,d0 */
        SETW(d0, rd16(V_033C));                              /* 007EFA  move.w  $33c(a5),d0 */
        /* 007EFE  move.l  $28e(a5),-(a7) */
        {
            uint32_t v = rd32(V_DRAW_BUFFER);
            a7 -= 4;
            wr32(a7, v);
        }
        wr32(V_DRAW_BUFFER, 0x20000);                        /* 007F02  move.l  #$20000,$28e(a5) */
        CALL(sub_00858E, 0x7F0EU);                           /* 007F0A  bsr  $858e */
L_007F0E:
        /* 007F0E  move.l  (a7)+,$28e(a5) */
        wr32(V_DRAW_BUFFER, rd32(a7));
        a7 += 4;
        a0 = 0x27260;                                        /* 007F12  lea  $27260.l,a0 */
        d7 = 7;                                              /* 007F18  moveq  #7,d7 */
        /* 007F1A  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007F1E:
    /* 007F1E  cmpi.w  #$5,$25a(a5) */
    /* 007F24  bne  $7f78 */
    if (rd16(V_025A) == 5) {
L_007F26:
        wr16(V_026C, 8);                                     /* 007F26  move.w  #$8,$26c(a5) */
        /* 007F2C  addq.w  #1,$25a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_025A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_025A, r);
            fx = (r < s);
        }
        /* 007F30  tst.w  $376(a5) */
        /* 007F34  beq  $8036 */
        if (rd16(V_0376) == 0) {
            goto L_008036;
        }
        /* 007F38  tst.w  $1b4(a5) */
        /* 007F3C  beq  $8036 */
        if (rd16(V_01B4) == 0) {
            goto L_008036;
        }
        wr16(V_0266, 0x7260);                                /* 007F40  move.w  #$7260,$266(a5) */
        wr16(V_0268, 0x26);                                  /* 007F46  move.w  #$26,$268(a5) */
        wr16(V_026A, 0x26);                                  /* 007F4C  move.w  #$26,$26a(a5) */
        d0 = 0;                                              /* 007F52  moveq  #0,d0 */
        SETW(d0, rd16(V_03D8));                              /* 007F54  move.w  $3d8(a5),d0 */
        /* 007F58  move.l  $28e(a5),-(a7) */
        {
            uint32_t v = rd32(V_DRAW_BUFFER);
            a7 -= 4;
            wr32(a7, v);
        }
        wr32(V_DRAW_BUFFER, 0x20000);                        /* 007F5C  move.l  #$20000,$28e(a5) */
        CALL(sub_00858E, 0x7F68U);                           /* 007F64  bsr  $858e */
L_007F68:
        /* 007F68  move.l  (a7)+,$28e(a5) */
        wr32(V_DRAW_BUFFER, rd32(a7));
        a7 += 4;
        a0 = 0x272E0;                                        /* 007F6C  lea  $272e0.l,a0 */
        d7 = 7;                                              /* 007F72  moveq  #7,d7 */
        /* 007F74  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007F78:
    /* 007F78  cmpi.w  #$6,$25a(a5) */
    /* 007F7E  bne  $7fd8 */
    if (rd16(V_025A) == 6) {
L_007F80:
        wr16(V_025A, rd16(V_025A) + 1);                      /* 007F80  addq.w  #1,$25a(a5) */
        a4 = V_02DA;                                         /* 007F84  lea  $2da(a5),a4 */
        d0 = 0;                                              /* 007F88  moveq  #0,d0 */
        SETW(d0, rd16(a4 + 0x64));                           /* 007F8A  move.w  $64(a4),d0 */
        a3 = a4 + 0x7A;                                      /* 007F8E  lea  $7a(a4),a3 */
        SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 007F92  lsl.w  #1,d0 */
        SETW(d0, rd16(a3 + SX16(d0)));                       /* 007F94  move.w  $0(a3,d0.w),d0 */
        CALL(sub_007D3E, 0x7F9CU);                           /* 007F98  bsr  $7d3e */
L_007F9C:
        wr16(V_026C, 8);                                     /* 007F9C  move.w  #$8,$26c(a5) */
        wr16(V_0266, 0x50A0);                                /* 007FA2  move.w  #$50a0,$266(a5) */
        wr16(V_0268, 2);                                     /* 007FA8  move.w  #$2,$268(a5) */
        wr16(V_026A, 2);                                     /* 007FAE  move.w  #$2,$26a(a5) */
        a0 = V_0261;                                         /* 007FB4  lea  $261(a5),a0 */
        /* 007FB8  move.l  $28e(a5),-(a7) */
        {
            uint32_t v = rd32(V_DRAW_BUFFER);
            a7 -= 4;
            wr32(a7, v);
        }
        wr32(V_DRAW_BUFFER, 0x20000);                        /* 007FBC  move.l  #$20000,$28e(a5) */
        CALL(sub_008584, 0x7FC8U);                           /* 007FC4  bsr  $8584 */
L_007FC8:
        /* 007FC8  move.l  (a7)+,$28e(a5) */
        wr32(V_DRAW_BUFFER, rd32(a7));
        a7 += 4;
        a0 = 0x250A0;                                        /* 007FCC  lea  $250a0.l,a0 */
        d7 = 7;                                              /* 007FD2  moveq  #7,d7 */
        /* 007FD4  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_007FD8:
    wr16(V_025A, 0);                                     /* 007FD8  clr.w  $25a(a5) */
    /* 007FDC  tst.w  $1b4(a5) */
    {
        uint32_t v = rd16(V_01B4);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
L_007FE0:
    /* 007FE0  beq  $8036 */
    if (COND(6)) {
        a4 = V_0376;                                         /* 007FE2  lea  $376(a5),a4 */
        d0 = 0;                                              /* 007FE6  moveq  #0,d0 */
        SETW(d0, rd16(a4 + 0x64));                           /* 007FE8  move.w  $64(a4),d0 */
        a3 = a4 + 0x7A;                                      /* 007FEC  lea  $7a(a4),a3 */
        SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 007FF0  lsl.w  #1,d0 */
        SETW(d0, rd16(a3 + SX16(d0)));                       /* 007FF2  move.w  $0(a3,d0.w),d0 */
        CALL(sub_007D3E, 0x7FFAU);                           /* 007FF6  bsr  $7d3e */
L_007FFA:
        wr16(V_026C, 8);                                     /* 007FFA  move.w  #$8,$26c(a5) */
        wr16(V_0266, 0x50A0);                                /* 008000  move.w  #$50a0,$266(a5) */
        wr16(V_0268, 0x22);                                  /* 008006  move.w  #$22,$268(a5) */
        wr16(V_026A, 0x22);                                  /* 00800C  move.w  #$22,$26a(a5) */
        a0 = V_0261;                                         /* 008012  lea  $261(a5),a0 */
        /* 008016  move.l  $28e(a5),-(a7) */
        {
            uint32_t v = rd32(V_DRAW_BUFFER);
            a7 -= 4;
            wr32(a7, v);
        }
        wr32(V_DRAW_BUFFER, 0x20000);                        /* 00801A  move.l  #$20000,$28e(a5) */
        CALL(sub_008584, 0x8026U);                           /* 008022  bsr  $8584 */
L_008026:
        /* 008026  move.l  (a7)+,$28e(a5) */
        wr32(V_DRAW_BUFFER, rd32(a7));
        a7 += 4;
        a0 = 0x25120;                                        /* 00802A  lea  $25120.l,a0 */
        d7 = 7;                                              /* 008030  moveq  #7,d7 */
        /* 008032  bra  $7de4 */
        BUDGET_CHECK(0x7DE4U);
        goto L_007DE4;
    }
L_008036:
    RTS();                                               /* 008036  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7DE4U: goto L_007DE4;
        case 0x7DE8U: goto L_007DE8;
        case 0x7E04U: goto L_007E04;
        case 0x7E0CU: goto L_007E0C;
        case 0x7E36U: goto L_007E36;
        case 0x7E44U: goto L_007E44;
        case 0x7E4CU: goto L_007E4C;
        case 0x7E76U: goto L_007E76;
        case 0x7E86U: goto L_007E86;
        case 0x7E8EU: goto L_007E8E;
        case 0x7EA0U: goto L_007EA0;
        case 0x7EACU: goto L_007EAC;
        case 0x7EB4U: goto L_007EB4;
        case 0x7EC6U: goto L_007EC6;
        case 0x7ED2U: goto L_007ED2;
        case 0x7EDAU: goto L_007EDA;
        case 0x7EE6U: goto L_007EE6;
        case 0x7F0EU: goto L_007F0E;
        case 0x7F1EU: goto L_007F1E;
        case 0x7F26U: goto L_007F26;
        case 0x7F68U: goto L_007F68;
        case 0x7F78U: goto L_007F78;
        case 0x7F80U: goto L_007F80;
        case 0x7F9CU: goto L_007F9C;
        case 0x7FC8U: goto L_007FC8;
        case 0x7FD8U: goto L_007FD8;
        case 0x7FE0U: goto L_007FE0;
        case 0x7FFAU: goto L_007FFA;
        case 0x8026U: goto L_008026;
        case 0x8036U: goto L_008036;
        }
    }
}

