/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 00B09A  game routine */
void sub_00B09A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_ENEMIES;                                      /* 00B09A  lea  $412(a5),a6 */
    SETW(d7, 0x2F);                                      /* 00B09E  move.w  #$2f,d7 */
    do {
        BUDGET_CHECK(0xB0A2U);
L_00B0A2:
        /* 00B0A2  tst.w  $4(a6) */
        /* 00B0A6  beq  $b0ae */
        if (rd16(a6 + 0x4) != 0) {
L_00B0A8:
            /* 00B0A8  move.w  d7,-(a7) */
            {
                uint32_t v = (d7 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
            }
            CALL(enemy_script_step, 0xB0ACU);                    /* 00B0AA  bsr  $b0b8 */
L_00B0AC:
            /* 00B0AC  move.w  (a7)+,d7 */
            SETW(d7, rd16(a7));
            a7 += 2;
        }
L_00B0AE:
        a6 = a6 + 0x40;                                      /* 00B0AE  lea  $40(a6),a6 */
        /* 00B0B2  dbra  d7,$b0a2 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B0B6:
    RTS();                                               /* 00B0B6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB0A2U: goto L_00B0A2;
        case 0xB0A8U: goto L_00B0A8;
        case 0xB0ACU: goto L_00B0AC;
        case 0xB0AEU: goto L_00B0AE;
        case 0xB0B6U: goto L_00B0B6;
        }
    }
}

/* 00B0B8  Runs one step of an enemy's script (movement, shots, animation changes) */
void enemy_script_step(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = 0;                                              /* 00B0B8  moveq  #0,d0 */
    /* 00B0BA  tst.w  $a(a6) */
    /* 00B0BE  beq  $b0e0 */
    if (rd16(a6 + 0xA) != 0) {
L_00B0C0:
        /* 00B0C0  subi.w  #$1,$a(a6) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(a6 + 0xA);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(a6 + 0xA, r);
            fx = (s > d);
        }
L_00B0C6:
        /* 00B0C6  tst.w  $26(a6) */
        /* 00B0CA  bne  $b1c8 */
        if (rd16(a6 + 0x26) != 0) {
            goto L_00B1C8;
        }
L_00B0CE:
        SETW(d0, rd16(a6 + 0xC));                            /* 00B0CE  move.w  $c(a6),d0 */
        wr16(a6, rd16(a6) + (d0 & 0xFFFF));                  /* 00B0D2  add.w  d0,(a6) */
        SETW(d0, rd16(a6 + 0xE));                            /* 00B0D4  move.w  $e(a6),d0 */
        /* 00B0D8  add.w  d0,$2(a6) */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t d = rd16(a6 + 0x2);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(a6 + 0x2, r);
            fx = (r < s);
        }
        /* 00B0DC  bra  $b1c8 */
    } else {
L_00B0E0:
        a0 = rd32(a6 + 0x10);                                /* 00B0E0  movea.l  $10(a6),a0 */
L_00B0E4:
        /* 00B0E4  move.b  (a0)+,d0 */
        SETB(d0, rd8(a0));
        a0 += 1;
        /* 00B0E6  cmp.b  #$ff,d0 */
        /* 00B0EA  bne  $b0f6 */
        if ((d0 & 0xFF) == 0xFF) {
            wr16(a6 + 0x4, 0);                                   /* 00B0EE  clr.w  $4(a6) */
            /* 00B0F2  bra  $b1c8 */
        } else {
L_00B0F6:
            /* 00B0F6  cmp.b  #$fd,d0 */
            /* 00B0FA  beq  $b140 */
            if ((d0 & 0xFF) != 0xFD) {
L_00B0FE:
                /* 00B0FE  cmp.b  #$fe,d0 */
                /* 00B102  beq  $b184 */
                if ((d0 & 0xFF) == 0xFE) {
                    goto L_00B184;
                }
L_00B106:
                /* 00B106  cmp.b  #$fc,d0 */
                /* 00B10A  beq  $b124 */
                if ((d0 & 0xFF) != 0xFC) {
L_00B10E:
                    /* 00B10E  tst.w  $26(a6) */
                    /* 00B112  bne  $b1c8 */
                    if (rd16(a6 + 0x26) != 0) {
                        goto L_00B1C8;
                    }
L_00B116:
                    CALL(sub_00B1E4, 0xB11AU);                           /* 00B116  bsr  $b1e4 */
L_00B11A:
                    /* 00B11A  move.b  (a0)+,d0 */
                    SETB(d0, rd8(a0));
                    a0 += 1;
                    wr16(a6 + 0x1E, (d0 & 0xFFFF));                      /* 00B11C  move.w  d0,$1e(a6) */
                    wr16(a6 + 0x20, (d0 & 0xFFFF));                      /* 00B120  move.w  d0,$20(a6) */
                }
L_00B124:
                /* 00B124  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                wr16(a6 + 0xA, (d0 & 0xFFFF));                       /* 00B126  move.w  d0,$a(a6) */
                /* 00B12A  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                SETW(d0, SX8(d0));                                   /* 00B12C  ext.w  d0 */
                wr16(a6 + 0xC, (d0 & 0xFFFF));                       /* 00B12E  move.w  d0,$c(a6) */
                /* 00B132  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                SETW(d0, SX8(d0));                                   /* 00B134  ext.w  d0 */
                wr16(a6 + 0xE, (d0 & 0xFFFF));                       /* 00B136  move.w  d0,$e(a6) */
                wr32(a6 + 0x10, a0);                                 /* 00B13A  move.l  a0,$10(a6) */
                /* 00B13E  rts */
                if (g_cpu.patch[1]) {
                    { BUDGET_CHECK(0xB0C6U); goto L_00B0C6; }
                }
                RTS();
            }
L_00B140:
            /* 00B140  tst.w  $26(a6) */
            /* 00B144  bne  $b1c8 */
            if (rd16(a6 + 0x26) != 0) {
                goto L_00B1C8;
            }
L_00B148:
            /* 00B148  movem.l  a6/a5/a4/a3/a2/a1/a0/d1/d0,-(a7) */
            PUSH32(a6);
            PUSH32(a5);
            PUSH32(a4);
            PUSH32(a3);
            PUSH32(a2);
            PUSH32(a1);
            PUSH32(a0);
            PUSH32(d1);
            PUSH32(d0);
            d2 = 0;                                              /* 00B14C  moveq  #0,d2 */
            d3 = 0;                                              /* 00B14E  moveq  #0,d3 */
            a0 = 0xB294;                                         /* 00B150  lea  $b294(pc),a0 */
            SETW(d0, rd16(a6 + 0x8));                            /* 00B154  move.w  $8(a6),d0 */
            for (;;) {
                BUDGET_CHECK(0xB158U);
L_00B158:
                /* 00B158  move.w  (a0)+,d1 */
                {
                    uint32_t v = rd16(a0);
                    a0 += 2;
                    SETW(d1, v);
                    FL_LOGIC(v, 0xFFFF, 0x8000);
                }
                /* 00B15A  bmi  $b17c */
                if (COND(11)) {
                    goto L_00B17C;
                }
L_00B15C:
                /* 00B15C  cmp.w  d0,d1 */
                /* 00B15E  beq  $b166 */
                if ((d1 & 0xFFFF) == (d0 & 0xFFFF)) {
                    goto L_00B166;
                }
L_00B160:
                a0 = a0 + 0x8;                                       /* 00B160  lea  $8(a0),a0 */
                /* 00B164  bra  $b158 */
            }
L_00B166:
            /* 00B166  move.w  (a0)+,d2 */
            SETW(d2, rd16(a0));
            a0 += 2;
            /* 00B168  move.w  (a0)+,d3 */
            SETW(d3, rd16(a0));
            a0 += 2;
            SETW(d0, rd16(a6));                                  /* 00B16A  move.w  (a6),d0 */
            SETW(d1, rd16(a6 + 0x2));                            /* 00B16C  move.w  $2(a6),d1 */
            /* 00B170  add.w  (a0)+,d0 */
            {
                uint32_t s = rd16(a0);
                a0 += 2;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d0, r);
            }
            /* 00B172  add.w  (a0)+,d1 */
            {
                uint32_t s = rd16(a0);
                a0 += 2;
                uint32_t d = (d1 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d1, r);
                fx = (r < s);
            }
            SETW(d5, 0xFFFF);                                    /* 00B174  move.w  #$ffff,d5 */
            CALL(sub_006BEA, 0xB17CU);                           /* 00B178  bsr  $6bea */
L_00B17C:
            /* 00B17C  movem.l  (a7)+,d0-d1/a0-a6 */
            d0 = POP32();
            d1 = POP32();
            a0 = POP32();
            a1 = POP32();
            a2 = POP32();
            a3 = POP32();
            a4 = POP32();
            a5 = POP32();
            a6 = POP32();
            /* 00B180  bra  $b0e4 */
            BUDGET_CHECK(0xB0E4U);
            goto L_00B0E4;
L_00B184:
            /* 00B184  tst.w  $26(a6) */
            /* 00B188  bne  $b1c8 */
            if (rd16(a6 + 0x26) == 0) {
L_00B18A:
                /* 00B18A  movem.l  a6/a0/d1/d0,-(a7) */
                PUSH32(a6);
                PUSH32(a0);
                PUSH32(d1);
                PUSH32(d0);
                d3 = 0;                                              /* 00B18E  moveq  #0,d3 */
                d4 = 0;                                              /* 00B190  moveq  #0,d4 */
                a0 = 0xB294;                                         /* 00B192  lea  $b294(pc),a0 */
                SETW(d0, rd16(a6 + 0x8));                            /* 00B196  move.w  $8(a6),d0 */
                for (;;) {
                    BUDGET_CHECK(0xB19AU);
L_00B19A:
                    /* 00B19A  move.w  (a0)+,d1 */
                    {
                        uint32_t v = rd16(a0);
                        a0 += 2;
                        SETW(d1, v);
                        FL_LOGIC(v, 0xFFFF, 0x8000);
                    }
                    /* 00B19C  bmi  $b1c0 */
                    if (COND(11)) {
                        goto L_00B1C0;
                    }
L_00B19E:
                    /* 00B19E  cmp.w  d0,d1 */
                    /* 00B1A0  beq  $b1a8 */
                    if ((d1 & 0xFFFF) == (d0 & 0xFFFF)) {
                        goto L_00B1A8;
                    }
L_00B1A2:
                    a0 = a0 + 0x8;                                       /* 00B1A2  lea  $8(a0),a0 */
                    /* 00B1A6  bra  $b19a */
                }
L_00B1A8:
                /* 00B1A8  move.w  (a0)+,d3 */
                SETW(d3, rd16(a0));
                a0 += 2;
                /* 00B1AA  move.w  (a0)+,d4 */
                SETW(d4, rd16(a0));
                a0 += 2;
                SETW(d0, rd16(a6));                                  /* 00B1AC  move.w  (a6),d0 */
                SETW(d1, rd16(a6 + 0x2));                            /* 00B1AE  move.w  $2(a6),d1 */
                /* 00B1B2  add.w  (a0)+,d0 */
                {
                    uint32_t s = rd16(a0);
                    a0 += 2;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d0, r);
                }
                /* 00B1B4  add.w  (a0)+,d1 */
                {
                    uint32_t s = rd16(a0);
                    a0 += 2;
                    uint32_t d = (d1 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d1, r);
                    fx = (r < s);
                }
                SETW(d2, 0x18);                                      /* 00B1B6  move.w  #$18,d2 */
                d5 = 0xFFFFFFFF;                                     /* 00B1BA  moveq  #-1,d5 */
                CALL(sub_006B74, 0xB1C0U);                           /* 00B1BC  bsr  $6b74 */
L_00B1C0:
                /* 00B1C0  movem.l  (a7)+,d0-d1/a0/a6 */
                d0 = POP32();
                d1 = POP32();
                a0 = POP32();
                a6 = POP32();
                /* 00B1C4  bra  $b0e4 */
                BUDGET_CHECK(0xB0E4U);
                goto L_00B0E4;
            }
        }
    }
L_00B1C8:
    RTS();                                               /* 00B1C8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB0C0U: goto L_00B0C0;
        case 0xB0CEU: goto L_00B0CE;
        case 0xB0E0U: goto L_00B0E0;
        case 0xB0E4U: goto L_00B0E4;
        case 0xB0F6U: goto L_00B0F6;
        case 0xB0FEU: goto L_00B0FE;
        case 0xB106U: goto L_00B106;
        case 0xB10EU: goto L_00B10E;
        case 0xB116U: goto L_00B116;
        case 0xB11AU: goto L_00B11A;
        case 0xB124U: goto L_00B124;
        case 0xB140U: goto L_00B140;
        case 0xB148U: goto L_00B148;
        case 0xB158U: goto L_00B158;
        case 0xB15CU: goto L_00B15C;
        case 0xB160U: goto L_00B160;
        case 0xB166U: goto L_00B166;
        case 0xB17CU: goto L_00B17C;
        case 0xB184U: goto L_00B184;
        case 0xB18AU: goto L_00B18A;
        case 0xB19AU: goto L_00B19A;
        case 0xB19EU: goto L_00B19E;
        case 0xB1A2U: goto L_00B1A2;
        case 0xB1A8U: goto L_00B1A8;
        case 0xB1C0U: goto L_00B1C0;
        case 0xB1C8U: goto L_00B1C8;
        }
    }
}

/* 00B1E4  game routine */
void sub_00B1E4(void)
{
    PUSH32(a0);                                          /* 00B1E4  movem.l  a0,-(a7) */
    a0 = 0xB228;                                         /* 00B1E8  lea  $b228(pc),a0 */
    a1 = 0xB20E;                                         /* 00B1EC  lea  $b20e(pc),a1 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 00B1F0  lsl.w  #1,d0 */
    SETW(d0, rd16(a1 + SX16(d0)));                       /* 00B1F2  move.w  $0(a1,d0.w),d0 */
    a0 += SX16((d0 & 0xFFFF));                           /* 00B1F6  adda.w  d0,a0 */
    wr32(a6 + 0x14, a0);                                 /* 00B1F8  move.l  a0,$14(a6) */
    /* 00B1FC  move.b  (a0)+,d0 */
    SETB(d0, rd8(a0));
    a0 += 1;
    wr16(a6 + 0x8, (d0 & 0xFFFF));                       /* 00B1FE  move.w  d0,$8(a6) */
    wr32(a6 + 0x18, a0);                                 /* 00B202  move.l  a0,$18(a6) */
    d0 = 0;                                              /* 00B206  moveq  #0,d0 */
    a0 = POP32();                                        /* 00B208  movem.l  (a7)+,a0 */
    RTS();                                               /* 00B20C  rts */
}

/* 00B20E  game routine */
void sub_00B20E(void)
{
    SETB(d0, d0 | 9);                                    /* 00B20E  ori.b  #$9,d0 */
    wr8(a6, rd8(a6) | 0x23);                             /* 00B212  ori.b  #$23,(a6) */
    wr8(a3 + 0x35, rd8(a3 + 0x35) | 0x34);               /* 00B216  ori.b  #$34,$35(a3) */
    wr8(a6 + 0x4C + SX16(d0), rd8(a6 + 0x4C + SX16(d0)) | 0x3F); /* 00B21C  ori.b  #$3f,$4c(a6,d0.w) */
    /* 00B222  ori.w  #$61,(a1)+ */
    {
        uint32_t s = 0x61;
        uint32_t ea = a1;
        a1 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(ea, r);
    }
    /* 00B226  ori.w  #$0,$102(a2) */
    wr16(a2 + 0x102, rd16(a2 + 0x102) | 0);
    { sub_00B22C(); return; }
}

/* 00B228  game routine */
void sub_00B228(void)
{
    /* 00B228  ori.b  #$2,d0 */
    SETB(d0, d0 | 2);
    { sub_00B22C(); return; }
}

/* 00B22C  game routine */
void sub_00B22C(void)
{
    /* 00B22C  btst  d1,d3 */
    SETB(d1, d1 & 4);                                    /* 00B22E  andi.b  #$4,d1 */
    SETB(d4, d4 - 5);                                    /* 00B232  subi.b  #$5,d4 */
    /* 00B236  btst  d2,d6 */
    /* 00B238  addi.b  #$7,d6 */
    {
        uint32_t s = 7;
        uint32_t d = (d6 & 0xFF);
        uint32_t r = (d + s) & 0xFF;
        SETB(d6, r);
        FL_ADD(s, d, r, 0xFF, 0x80, 1);
    }
    MISS(0xB23CU);
}

/* 00B294  game routine */
void sub_00B294(void)
{
    SETB(d0, d0 | 0);                                    /* 00B294  ori.b  #$0,d0 */
    /* 00B298  ori.b  #$4,d3 */
    {
        uint32_t s = 4;
        uint32_t d = (d3 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d3, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xB29CU);
}

/* 00B386  game routine */
void sub_00B386(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_ENEMIES;                                      /* 00B386  lea  $412(a5),a6 */
    SETW(d7, 0x2F);                                      /* 00B38A  move.w  #$2f,d7 */
    do {
        BUDGET_CHECK(0xB38EU);
L_00B38E:
        /* 00B38E  tst.w  $4(a6) */
        /* 00B392  beq  $b396 */
        if (rd16(a6 + 0x4) != 0) {
L_00B394:
            CALL(sub_00B3A0, 0xB396U);                           /* 00B394  bsr  $b3a0 */
        }
L_00B396:
        a6 = a6 + 0x40;                                      /* 00B396  lea  $40(a6),a6 */
        /* 00B39A  dbra  d7,$b38e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B39E:
    RTS();                                               /* 00B39E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB38EU: goto L_00B38E;
        case 0xB394U: goto L_00B394;
        case 0xB396U: goto L_00B396;
        case 0xB39EU: goto L_00B39E;
        }
    }
}

/* 00B3A0  game routine */
void sub_00B3A0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00B3A0  move.w  $c(a6),d0 */
    /* 00B3A4  bpl  $b3a8 */
    SETW(d0, rd16(a6 + 0xC));
    if ((int16_t)(d0 & 0xFFFF) < 0) {
L_00B3A6:
        /* 00B3A6  neg.w  d0 */
        { uint32_t d, r, ea; (void)ea;
        d = (d0 & 0xFFFFU);
        r = (0 - d) & 0xFFFFU;
        d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
        FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
    }
L_00B3A8:
    /* 00B3A8  move.w  $e(a6),d1 */
    /* 00B3AC  bpl  $b3b0 */
    SETW(d1, rd16(a6 + 0xE));
    if ((int16_t)(d1 & 0xFFFF) < 0) {
L_00B3AE:
        /* 00B3AE  neg.w  d1 */
        { uint32_t d, r, ea; (void)ea;
        d = (d1 & 0xFFFFU);
        r = (0 - d) & 0xFFFFU;
        d1 = (d1 & 0xFFFF0000U) | ((r) & 0xFFFFU);
        FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
    }
L_00B3B0:
    /* 00B3B0  cmp.w  d1,d0 */
    /* 00B3B2  bcc  $b3b6 */
    if ((d0 & 0xFFFF) < (d1 & 0xFFFF)) {
L_00B3B4:
        SETW(d0, (d1 & 0xFFFF));                             /* 00B3B4  move.w  d1,d0 */
    }
L_00B3B6:
    /* 00B3B6  tst.w  d0 */
    /* 00B3B8  bne  $b3bc */
    if ((d0 & 0xFFFF) == 0) {
L_00B3BA:
        d0 = 1;                                              /* 00B3BA  moveq  #1,d0 */
    }
L_00B3BC:
    /* 00B3BC  cmp.w  #$5,d0 */
    /* 00B3C0  bls  $b3c6 */
    if ((d0 & 0xFFFF) > 5) {
        SETW(d0, 5);                                         /* 00B3C2  move.w  #$5,d0 */
    }
L_00B3C6:
    SETW(d0, (d0 & 0xFFFF) << 1);                        /* 00B3C6  lsl.w  #1,d0 */
L_00B3C8:
    /* 00B3C8  sub.w  d0,$28(a6) */
    {
        uint32_t s = (d0 & 0xFFFF);
        uint32_t d = rd16(a6 + 0x28);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(a6 + 0x28, r);
        fn = !!(r & 0x8000);
        fx = (s > d);
    }
    /* 00B3CC  bpl  $b3fe */
    if (COND(11)) {
        do {
            BUDGET_CHECK(0xB3CEU);
L_00B3CE:
            /* 00B3CE  addq.w  #4,$28(a6) */
            {
                uint32_t s = 4;
                uint32_t d = rd16(a6 + 0x28);
                uint32_t r = (d + s) & 0xFFFF;
                wr16(a6 + 0x28, r);
                fx = (r < s);
            }
            do {
                BUDGET_CHECK(0xB3D2U);
L_00B3D2:
                d0 = 0;                                              /* 00B3D2  moveq  #0,d0 */
L_00B3D4:
                a0 = rd32(a6 + 0x18);                                /* 00B3D4  movea.l  $18(a6),a0 */
                /* 00B3D8  move.b  (a0)+,d0 */
                {
                    uint32_t v = rd8(a0);
                    a0 += 1;
                    SETB(d0, v);
                    FL_LOGIC(v, 0xFF, 0x80);
                }
                /* 00B3DA  bmi  $b3e6 */
                if (COND(10)) {
L_00B3DC:
                    wr32(a6 + 0x18, a0);                                 /* 00B3DC  move.l  a0,$18(a6) */
                    wr16(a6 + 0x8, (d0 & 0xFFFF));                       /* 00B3E0  move.w  d0,$8(a6) */
                    /* 00B3E4  bra  $b3f8 */
                    goto L_00B3F8;
                }
L_00B3E6:
                wr32(a6 + 0x18, rd32(a6 + 0x14));                    /* 00B3E6  move.l  $14(a6),$18(a6) */
                /* 00B3EC  tst.w  $26(a6) */
                /* 00B3F0  beq  $b3d2 */
            } while (rd16(a6 + 0x26) == 0);
L_00B3F2:
            wr16(a6 + 0x4, 0);                                   /* 00B3F2  clr.w  $4(a6) */
            RTS();                                               /* 00B3F6  rts */
L_00B3F8:
            /* 00B3F8  tst.w  $28(a6) */
            /* 00B3FC  bmi  $b3ce */
        } while ((int16_t)rd16(a6 + 0x28) < 0);
    }
L_00B3FE:
    RTS();                                               /* 00B3FE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB3A6U: goto L_00B3A6;
        case 0xB3A8U: goto L_00B3A8;
        case 0xB3AEU: goto L_00B3AE;
        case 0xB3B0U: goto L_00B3B0;
        case 0xB3B4U: goto L_00B3B4;
        case 0xB3B6U: goto L_00B3B6;
        case 0xB3BAU: goto L_00B3BA;
        case 0xB3BCU: goto L_00B3BC;
        case 0xB3C6U: goto L_00B3C6;
        case 0xB3C8U: goto L_00B3C8;
        case 0xB3CEU: goto L_00B3CE;
        case 0xB3D2U: goto L_00B3D2;
        case 0xB3D4U: goto L_00B3D4;
        case 0xB3DCU: goto L_00B3DC;
        case 0xB3E6U: goto L_00B3E6;
        case 0xB3F2U: goto L_00B3F2;
        case 0xB3F8U: goto L_00B3F8;
        case 0xB3FEU: goto L_00B3FE;
        }
    }
}

/* 00B424  game routine */
void sub_00B424(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00B424  movem.l  a6/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
    PUSH32(a6);
    PUSH32(d7);
    PUSH32(d6);
    PUSH32(d5);
    PUSH32(d4);
    PUSH32(d3);
    PUSH32(d2);
    PUSH32(d1);
    PUSH32(d0);
    a6 = V_1C84;                                         /* 00B428  lea  $1c84(a5),a6 */
    SETW(d7, 5);                                         /* 00B42C  move.w  #$5,d7 */
    do {
        BUDGET_CHECK(0xB430U);
L_00B430:
        /* 00B430  tst.w  (a6) */
        /* 00B432  bne  $b45a */
        if (rd16(a6) == 0) {
            /* 00B436  addi.w  #$64,d1 */
            {
                uint32_t s = 0x64;
                uint32_t d = (d1 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d1, r);
                fx = (r < s);
            }
            wr16(a6 + 0x2, (d0 & 0xFFFF));                       /* 00B43A  move.w  d0,$2(a6) */
            wr16(a6 + 0x4, (d1 & 0xFFFF));                       /* 00B43E  move.w  d1,$4(a6) */
            wr16(a6 + 0x6, (d2 & 0xFFFF));                       /* 00B442  move.w  d2,$6(a6) */
            wr16(a6 + 0x8, (d3 & 0xFFFF));                       /* 00B446  move.w  d3,$8(a6) */
            wr16(a6 + 0xA, 0);                                   /* 00B44A  clr.w  $a(a6) */
            wr16(a6, 0xFFFF);                                    /* 00B44E  move.w  #$ffff,(a6) */
            wr16(a4 + 0x10, 0);                                  /* 00B452  move.w  #$0,$10(a4) */
            /* 00B458  bra  $b464 */
            goto L_00B464;
        }
        a6 = a6 + 0x12;                                      /* 00B45A  lea  $12(a6),a6 */
        /* 00B45E  dbra  d7,$b430 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    a6 -= a6;                                            /* 00B462  suba.l  a6,a6 */
L_00B464:
    wr32(V_02D6, a6);                                    /* 00B464  move.l  a6,$2d6(a5) */
    /* 00B468  movem.l  (a7)+,d0-d7/a6 */
    d0 = POP32();
    d1 = POP32();
    d2 = POP32();
    d3 = POP32();
    d4 = POP32();
    d5 = POP32();
    d6 = POP32();
    d7 = POP32();
    a6 = POP32();
    RTS();                                               /* 00B46C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB430U: goto L_00B430;
        }
    }
}

/* 00B46E  game routine */
void sub_00B46E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1C84;                                         /* 00B46E  lea  $1c84(a5),a6 */
    SETW(d7, 5);                                         /* 00B472  move.w  #$5,d7 */
    do {
        BUDGET_CHECK(0xB476U);
L_00B476:
        /* 00B476  tst.w  (a6) */
        /* 00B478  beq  $b488 */
        if (rd16(a6) != 0) {
            /* 00B47A  tst.w  $e(a6) */
            /* 00B47E  bmi  $b488 */
            if ((int16_t)rd16(a6 + 0xE) >= 0) {
                CALL(sub_00B492, 0xB484U);                           /* 00B480  bsr  $b492 */
L_00B484:
                CALL(sub_00B512, 0xB488U);                           /* 00B484  bsr  $b512 */
            }
        }
L_00B488:
        a6 = a6 + 0x12;                                      /* 00B488  lea  $12(a6),a6 */
        /* 00B48C  dbra  d7,$b476 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B490:
    RTS();                                               /* 00B490  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB476U: goto L_00B476;
        case 0xB484U: goto L_00B484;
        case 0xB488U: goto L_00B488;
        case 0xB490U: goto L_00B490;
        }
    }
}

/* 00B492  game routine */
void sub_00B492(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00B492  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a4 = V_02DA;                                         /* 00B496  lea  $2da(a5),a4 */
    /* 00B49A  tst.w  (a4) */
    /* 00B49C  bne  $b4a2 */
    if (rd16(a4) == 0) {
        a4 = V_0376;                                         /* 00B49E  lea  $376(a5),a4 */
    }
    SETW(d0, rd16(a4 + 0x2));                            /* 00B4A2  move.w  $2(a4),d0 */
    SETW(d0, d0 + 0x24);                                 /* 00B4A6  addi.w  #$24,d0 */
    SETW(d1, rd16(a4 + 0x4));                            /* 00B4AA  move.w  $4(a4),d1 */
    SETW(d1, d1 + 0x64);                                 /* 00B4AE  addi.w  #$64,d1 */
    SETW(d2, rd16(a6 + 0x4));                            /* 00B4B2  move.w  $4(a6),d2 */
    SETW(d3, rd16(a6 + 0x6));                            /* 00B4B6  move.w  $6(a6),d3 */
    d5 = 0;                                              /* 00B4BA  moveq  #0,d5 */
    SETW(d2, d2 - (d0 & 0xFFFF));                        /* 00B4BC  sub.w  d0,d2 */
    SETW(d2, d2 + 0x14);                                 /* 00B4BE  addi.w  #$14,d2 */
    /* 00B4C2  tst.w  d2 */
    /* 00B4C4  bmi  $b4ce */
    if ((int16_t)(d2 & 0xFFFF) >= 0) {
        /* 00B4C6  cmp.w  #$28,d2 */
        /* 00B4CA  bhi  $b4d2 */
        if ((d2 & 0xFFFF) > 0x28) {
            goto L_00B4D2;
        }
        /* 00B4CC  bra  $b4d4 */
    } else {
        SETW(d5, d5 + 1);                                    /* 00B4CE  addq.w  #1,d5 */
        /* 00B4D0  bra  $b4d4 */
        goto L_00B4D4;
L_00B4D2:
        SETW(d5, d5 + 4);                                    /* 00B4D2  addq.w  #4,d5 */
    }
L_00B4D4:
    SETW(d3, d3 - (d1 & 0xFFFF));                        /* 00B4D4  sub.w  d1,d3 */
    /* 00B4D6  addi.w  #$14,d3 */
    {
        uint32_t s = 0x14;
        uint32_t d = (d3 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d3, r);
        fx = (r < s);
    }
    /* 00B4DA  tst.w  d3 */
    /* 00B4DC  bmi  $b4e6 */
    if ((int16_t)(d3 & 0xFFFF) >= 0) {
        /* 00B4DE  cmp.w  #$28,d3 */
        /* 00B4E2  bhi  $b4ea */
        if ((d3 & 0xFFFF) > 0x28) {
            goto L_00B4EA;
        }
        /* 00B4E4  bra  $b4ec */
    } else {
        /* 00B4E6  addq.w  #2,d5 */
        {
            uint32_t s = 2;
            uint32_t d = (d5 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d5, r);
            fx = (r < s);
        }
        /* 00B4E8  bra  $b4ec */
        goto L_00B4EC;
L_00B4EA:
        /* 00B4EA  addq.w  #8,d5 */
        {
            uint32_t s = 8;
            uint32_t d = (d5 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d5, r);
            fx = (r < s);
        }
    }
L_00B4EC:
    /* 00B4EC  tst.w  d5 */
    /* 00B4EE  bne  $b4f2 */
    if ((d5 & 0xFFFF) == 0) {
        /* 00B4F0  addq.w  #1,d5 */
        {
            uint32_t s = 1;
            uint32_t d = (d5 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d5, r);
            fx = (r < s);
        }
    }
    CALL(sub_00B4FC, 0xB4F6U);                           /* 00B4F2  bsr  $b4fc */
L_00B4F6:
    /* 00B4F6  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 00B4FA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB4F6U: goto L_00B4F6;
        }
    }
}

/* 00B4FC  game routine */
void sub_00B4FC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0xB5E6;                                         /* 00B4FC  lea  $b5e6(pc),a0 */
    for (;;) {
        BUDGET_CHECK(0xB500U);
L_00B500:
        SETW(d4, rd16(a0));                                  /* 00B500  move.w  (a0),d4 */
        /* 00B502  cmp.w  d5,d4 */
        /* 00B504  beq  $b50a */
        if ((d4 & 0xFFFF) == (d5 & 0xFFFF)) {
            goto L_00B50A;
        }
        a0 += 4;                                             /* 00B506  addq.l  #4,a0 */
        /* 00B508  bra  $b500 */
    }
L_00B50A:
    wr16(a6 + 0xA, rd16(a0 + 0x2));                      /* 00B50A  move.w  $2(a0),$a(a6) */
    RTS();                                               /* 00B510  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB500U: goto L_00B500;
        }
    }
}

/* 00B512  game routine */
void sub_00B512(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00B512  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    /* 00B516  tst.w  $c(a6) */
    /* 00B51A  beq  $b522 */
    if (rd16(a6 + 0xC) != 0) {
        /* 00B51C  subq.w  #1,$c(a6) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(a6 + 0xC);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(a6 + 0xC, r);
            fx = (s > d);
        }
        /* 00B520  bra  $b53a */
    } else {
        CALL(sub_007576, 0xB526U);                           /* 00B522  bsr  $7576 */
L_00B526:
        d0 = rd32(V_1E12);                                   /* 00B526  move.l  $1e12(a5),d0 */
        SETW(d0, d0 & 0x1F);                                 /* 00B52A  andi.w  #$1f,d0 */
        wr16(a6 + 0xC, (d0 & 0xFFFF));                       /* 00B52E  move.w  d0,$c(a6) */
        SETW(d0, rd16(a6 + 0xA));                            /* 00B532  move.w  $a(a6),d0 */
        CALL(sub_00B540, 0xB53AU);                           /* 00B536  bsr  $b540 */
    }
L_00B53A:
    /* 00B53A  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 00B53E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB526U: goto L_00B526;
        case 0xB53AU: goto L_00B53A;
        }
    }
}

/* 00B540  game routine */
void sub_00B540(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0xB586;                                         /* 00B540  lea  $b586(pc),a0 */
    SETW(d7, 6);                                         /* 00B544  move.w  #$6,d7 */
    do {
        BUDGET_CHECK(0xB548U);
L_00B548:
        SETW(d1, rd16(a0));                                  /* 00B548  move.w  (a0),d1 */
        /* 00B54A  cmp.w  d0,d1 */
        /* 00B54C  beq  $b556 */
        if ((d1 & 0xFFFF) == (d0 & 0xFFFF)) {
            goto L_00B556;
        }
        a0 = a0 + 0xC;                                       /* 00B54E  lea  $c(a0),a0 */
        /* 00B552  dbra  d7,$b548 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B556:
    a0 += 2;                                             /* 00B556  adda.l  #$2,a0 */
    /* 00B55C  move.w  (a0)+,d0 */
    SETW(d0, rd16(a0));
    a0 += 2;
    SETW(d0, d0 + rd16(a6 + 0x4));                       /* 00B55E  add.w  $4(a6),d0 */
    /* 00B562  move.w  (a0)+,d1 */
    SETW(d1, rd16(a0));
    a0 += 2;
    /* 00B564  add.w  $6(a6),d1 */
    {
        uint32_t s = rd16(a6 + 0x6);
        uint32_t d = (d1 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d1, r);
        fx = (r < s);
    }
    /* 00B568  move.w  (a0)+,d3 */
    SETW(d3, rd16(a0));
    a0 += 2;
    /* 00B56A  move.w  (a0)+,d4 */
    SETW(d4, rd16(a0));
    a0 += 2;
    SETW(d2, 0x12C);                                     /* 00B56C  move.w  #$12c,d2 */
    /* 00B570  tst.w  $10(a6) */
    /* 00B574  bne  $b57c */
    if (rd16(a6 + 0x10) == 0) {
        SETW(d5, 0xFFFE);                                    /* 00B576  move.w  #$fffe,d5 */
        /* 00B57A  bra  $b580 */
    } else {
        SETW(d5, 0xFFFC);                                    /* 00B57C  move.w  #$fffc,d5 */
    }
    CALL(sub_006B74, 0xB584U);                           /* 00B580  bsr  $6b74 */
L_00B584:
    RTS();                                               /* 00B584  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB548U: goto L_00B548;
        case 0xB584U: goto L_00B584;
        }
    }
}

/* 00B586  game routine */
void sub_00B586(void)
{
    /* 00B586  ori.b  #$6,d0 */
    {
        uint32_t s = 6;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xB58AU);                                       /* 00B58A  soft */
}

/* 00B6A6  game routine */
void sub_00B6A6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00B6A6  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a0 = V_1754;                                         /* 00B6AA  lea  $1754(a5),a0 */
    SETW(d7, 0x18);                                      /* 00B6AE  move.w  #$18,d7 */
    do {
        BUDGET_CHECK(0xB6B2U);
L_00B6B2:
        /* 00B6B2  tst.w  $8(a0) */
        /* 00B6B6  bne  $b6dc */
        if (rd16(a0 + 0x8) == 0) {
L_00B6B8:
            wr16(a0, (d0 & 0xFFFF));                             /* 00B6B8  move.w  d0,(a0) */
            wr16(a0 + 0x2, (d1 & 0xFFFF));                       /* 00B6BA  move.w  d1,$2(a0) */
            wr16(a0 + 0x4, (d3 & 0xFFFF));                       /* 00B6BE  move.w  d3,$4(a0) */
            wr16(a0 + 0x6, (d4 & 0xFFFF));                       /* 00B6C2  move.w  d4,$6(a0) */
            wr16(a0 + 0xA, (d5 & 0xFFFF));                       /* 00B6C6  move.w  d5,$a(a0) */
            wr16(a0 + 0xC, (d2 & 0xFFFF));                       /* 00B6CA  move.w  d2,$c(a0) */
            wr16(a0 + 0xE, 0);                                   /* 00B6CE  clr.w  $e(a0) */
            wr16(a0 + 0x8, 0xFFFF);                              /* 00B6D2  move.w  #$ffff,$8(a0) */
            /* 00B6D8  bra  $b6e4 */
            goto L_00B6E4;
        }
L_00B6DC:
        a0 = a0 + 0x10;                                      /* 00B6DC  lea  $10(a0),a0 */
        /* 00B6E0  dbra  d7,$b6b2 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B6E4:
    /* 00B6E4  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 00B6E8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB6B2U: goto L_00B6B2;
        case 0xB6B8U: goto L_00B6B8;
        case 0xB6DCU: goto L_00B6DC;
        case 0xB6E4U: goto L_00B6E4;
        }
    }
}

/* 00B6EA  Draws effects */
void draw_effects(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1754;                                         /* 00B6EA  lea  $1754(a5),a6 */
    SETW(d5, 0x18);                                      /* 00B6EE  move.w  #$18,d5 */
L_00B6F2:
    /* 00B6F2  tst.w  $8(a6) */
    /* 00B6F6  bne  $b702 */
    HOOK(33, 0xB6F2U);
    if (rd16(a6 + 0x8) != 0) {
        goto L_00B702;
    }
L_00B6F8:
    /* 00B6F8  lea  $10(a6),a6 */
    HOOK(34, 0xB6F8U);
    a6 = a6 + 0x10;
    /* 00B6FC  dbra  d5,$b6f2 */
    SETW(d5, d5 - 1);
    if ((d5 & 0xFFFF) != 0xFFFF) {
        BUDGET_CHECK(0xB6F2U);
        goto L_00B6F2;
    }
L_00B700:
    RTS();                                               /* 00B700  rts */
L_00B702:
    SETW(d0, rd16(a6));                                  /* 00B702  move.w  (a6),d0 */
    SETW(d2, rd16(a6 + 0x2));                            /* 00B704  move.w  $2(a6),d2 */
    SETW(d3, rd16(V_0272));                              /* 00B708  move.w  $272(a5),d3 */
    SETW(d3, d3 & 1);                                    /* 00B70C  andi.w  #$1,d3 */
    /* 00B710  add.w  $e(a6),d3 */
    {
        uint32_t s = rd16(a6 + 0xE);
        uint32_t d = (d3 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d3, r);
        fx = (r < s);
    }
    /* 00B714  cmp.w  #$b,d3 */
    /* 00B718  bls  $b71c */
    if ((d3 & 0xFFFF) > 0xB) {
L_00B71A:
        d3 = 0xA;                                            /* 00B71A  moveq  #10,d3 */
    }
L_00B71C:
    /* 00B71C  movem.l  a6/d5,-(a7) */
    PUSH32(a6);
    PUSH32(d5);
    CALL(sub_00B728, 0xB722U);                           /* 00B720  bsr  $b728 */
L_00B722:
    /* 00B722  movem.l  (a7)+,d5/a6 */
    d5 = POP32();
    a6 = POP32();
    /* 00B726  bra  $b6f8 */
    BUDGET_CHECK(0xB6F8U);
    goto L_00B6F8;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB6F2U: goto L_00B6F2;
        case 0xB6F8U: goto L_00B6F8;
        case 0xB700U: goto L_00B700;
        case 0xB702U: goto L_00B702;
        case 0xB71AU: goto L_00B71A;
        case 0xB71CU: goto L_00B71C;
        case 0xB722U: goto L_00B722;
        }
    }
}

/* 00B728  game routine */
void sub_00B728(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d7, 0xF);                                       /* 00B728  move.w  #$f,d7 */
    d5 = 0;                                              /* 00B72C  moveq  #0,d5 */
    /* 00B72E  cmp.w  #$64,d2 */
    /* 00B732  bcc  $b752 */
    if ((d2 & 0xFFFF) < 0x64) {
L_00B734:
        /* 00B734  cmp.w  #$54,d2 */
        /* 00B738  bhi  $b740 */
        if ((d2 & 0xFFFF) <= 0x54) {
            wr16(a6 + 0x8, 0);                                   /* 00B73A  clr.w  $8(a6) */
            /* 00B73E  bra  $b7ac */
            goto L_00B7AC;
        }
L_00B740:
        SETW(d6, 0x64);                                      /* 00B740  move.w  #$64,d6 */
        SETW(d6, d6 - (d2 & 0xFFFF));                        /* 00B744  sub.w  d2,d6 */
        SETW(d7, d7 - (d6 & 0xFFFF));                        /* 00B746  sub.w  d6,d7 */
        SETW(d5, d5 + (d6 & 0xFFFF));                        /* 00B748  add.w  d6,d5 */
        SETW(d5, (d5 & 0xFFFF) << 3);                        /* 00B74A  lsl.w  #3,d5 */
        SETW(d2, 0x64);                                      /* 00B74C  move.w  #$64,d2 */
        /* 00B750  bra  $b76c */
    } else {
L_00B752:
        /* 00B752  cmp.w  #$11c,d2 */
        /* 00B756  bls  $b76c */
        if ((d2 & 0xFFFF) > 0x11C) {
            /* 00B758  cmp.w  #$12d,d2 */
            /* 00B75C  bcs  $b764 */
            if ((d2 & 0xFFFF) >= 0x12D) {
                wr16(a6 + 0x8, 0);                                   /* 00B75E  clr.w  $8(a6) */
                RTS();                                               /* 00B762  rts */
            }
            SETW(d4, (d2 & 0xFFFF));                             /* 00B764  move.w  d2,d4 */
            SETW(d4, d4 - 0x11D);                                /* 00B766  subi.w  #$11d,d4 */
            SETW(d7, d7 - (d4 & 0xFFFF));                        /* 00B76A  sub.w  d4,d7 */
        }
    }
L_00B76C:
    /* 00B76C  cmp.w  #$64,d0 */
    /* 00B770  bcs  $b8cc */
    if ((d0 & 0xFFFF) < 0x64) {
        goto L_00B8CC;
    }
L_00B774:
    /* 00B774  cmp.w  #$114,d0 */
    /* 00B778  bhi  $b854 */
    if ((d0 & 0xFFFF) > 0x114) {
        goto L_00B854;
    }
L_00B77C:
    SETW(d2, d2 - 0x64);                                 /* 00B77C  subi.w  #$64,d2 */
    a1 = rd32(V_DRAW_BUFFER);                            /* 00B780  movea.l  $28e(a5),a1 */
    SETW(d2, (d2 & 0xFFFF) << 5);                        /* 00B784  lsl.w  #5,d2 */
    a1 = a1 + SX16(d2);                                  /* 00B786  lea  $0(a1,d2.w),a1 */
    SETW(d2, (d2 & 0xFFFF) << 2);                        /* 00B78A  lsl.w  #2,d2 */
    a1 = a1 + SX16(d2);                                  /* 00B78C  lea  $0(a1,d2.w),a1 */
    SETW(d0, d0 - 0x64);                                 /* 00B790  subi.w  #$64,d0 */
    a1 += 0x20;                                          /* 00B794  adda.l  #$20,a1 */
    SETW(d1, (d0 & 0xFFFF));                             /* 00B79A  move.w  d0,d1 */
    SETW(d1, d1 & 0xE);                                  /* 00B79C  andi.w  #$e,d1 */
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 4, 16));      /* 00B7A0  lsr.w  #4,d0 */
    /* 00B7A2  tst.w  d1 */
    /* 00B7A4  beq  $b7aa */
    if ((d1 & 0xFFFF) != 0) {
L_00B7A6:
        CALL(sub_00B7EC, 0xB7A8U);                           /* 00B7A6  bsr  $b7ec */
L_00B7A8:
        RTS();                                               /* 00B7A8  rts */
    }
L_00B7AA:
    CALL(sub_00B7AE, 0xB7ACU);                           /* 00B7AA  bsr  $b7ae */
L_00B7AC:
    RTS();                                               /* 00B7AC  rts */
L_00B854:
    /* 00B854  subi.w  #$114,d0 */
    {
        uint32_t s = 0x114;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d0, r);
        fx = (s > d);
    }
    SETW(d5, (d0 & 0xFFFF));                             /* 00B858  move.w  d0,d5 */
    /* 00B85A  cmp.w  #$10,d5 */
    /* 00B85E  bcs  $b866 */
    if ((d5 & 0xFFFF) >= 0x10) {
        wr16(a6 + 0x8, 0);                                   /* 00B860  clr.w  $8(a6) */
        RTS();                                               /* 00B864  rts */
    }
    d5 &= ~0x1;                                          /* 00B866  bclr  #$0,d5 */
    SETW(d2, d2 - 0x64);                                 /* 00B86A  subi.w  #$64,d2 */
    a1 = rd32(V_DRAW_BUFFER);                            /* 00B86E  movea.l  $28e(a5),a1 */
    SETW(d2, (d2 & 0xFFFF) << 5);                        /* 00B872  lsl.w  #5,d2 */
    a1 = a1 + SX16(d2);                                  /* 00B874  lea  $0(a1,d2.w),a1 */
    SETW(d2, (d2 & 0xFFFF) << 2);                        /* 00B878  lsl.w  #2,d2 */
    a1 = a1 + SX16(d2);                                  /* 00B87A  lea  $0(a1,d2.w),a1 */
    a2 = 0x49A3A;                                        /* 00B87E  lea  $49a3a.l,a2 */
    SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 7, 16));      /* 00B884  lsl.w  #7,d3 */
    a2 = a2 + SX16(d3);                                  /* 00B886  lea  $0(a2,d3.w),a2 */
    a1 += 0x78;                                          /* 00B88A  adda.l  #$78,a1 */
    do {
        BUDGET_CHECK(0xB890U);
L_00B890:
        d0 = 0;                                              /* 00B890  moveq  #0,d0 */
        d1 = 0;                                              /* 00B892  moveq  #0,d1 */
        d2 = 0;                                              /* 00B894  moveq  #0,d2 */
        d3 = 0;                                              /* 00B896  moveq  #0,d3 */
        /* 00B898  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        /* 00B89A  move.w  (a2)+,d1 */
        SETW(d1, rd16(a2));
        a2 += 2;
        /* 00B89C  move.w  (a2)+,d2 */
        SETW(d2, rd16(a2));
        a2 += 2;
        /* 00B89E  move.w  (a2)+,d3 */
        SETW(d3, rd16(a2));
        a2 += 2;
        d0 = shift_op(3, 0, d0, (d5 & 63), 32);              /* 00B8A0  ror.l  d5,d0 */
        d1 = shift_op(3, 0, d1, (d5 & 63), 32);              /* 00B8A2  ror.l  d5,d1 */
        d2 = shift_op(3, 0, d2, (d5 & 63), 32);              /* 00B8A4  ror.l  d5,d2 */
        d3 = shift_op(3, 0, d3, (d5 & 63), 32);              /* 00B8A6  ror.l  d5,d3 */
        SETW(d6, (d0 & 0xFFFF));                             /* 00B8A8  move.w  d0,d6 */
        SETW(d6, d6 | (d1 & 0xFFFF));                        /* 00B8AA  or.w  d1,d6 */
        SETW(d6, d6 | (d2 & 0xFFFF));                        /* 00B8AC  or.w  d2,d6 */
        SETW(d6, d6 | (d3 & 0xFFFF));                        /* 00B8AE  or.w  d3,d6 */
        SETW(d6, ~(d6 & 0xFFFF));                            /* 00B8B0  not.w  d6 */
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B8B2  and.w  d6,(a1) */
        /* 00B8B4  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B8B6  and.w  d6,(a1) */
        /* 00B8B8  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B8BA  and.w  d6,(a1) */
        /* 00B8BC  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B8BE  and.w  d6,(a1) */
        wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 00B8C0  or.w  d3,(a1) */
        a1 = a1 + 0x9A;                                      /* 00B8C2  lea  $9a(a1),a1 */
        /* 00B8C6  dbra  d7,$b890 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 00B8CA  rts */
L_00B8CC:
    SETW(d5, 0x64);                                      /* 00B8CC  move.w  #$64,d5 */
    /* 00B8D0  sub.w  d0,d5 */
    {
        uint32_t s = (d0 & 0xFFFF);
        uint32_t d = (d5 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d5, r);
        fx = (s > d);
    }
    /* 00B8D2  cmp.w  #$10,d5 */
    /* 00B8D6  bcs  $b8de */
    if ((d5 & 0xFFFF) >= 0x10) {
        wr16(a6 + 0x8, 0);                                   /* 00B8D8  clr.w  $8(a6) */
        RTS();                                               /* 00B8DC  rts */
    }
    d5 &= ~0x1;                                          /* 00B8DE  bclr  #$0,d5 */
    SETW(d2, d2 - 0x64);                                 /* 00B8E2  subi.w  #$64,d2 */
    a1 = rd32(V_DRAW_BUFFER);                            /* 00B8E6  movea.l  $28e(a5),a1 */
    SETW(d2, (d2 & 0xFFFF) << 5);                        /* 00B8EA  lsl.w  #5,d2 */
    a1 = a1 + SX16(d2);                                  /* 00B8EC  lea  $0(a1,d2.w),a1 */
    SETW(d2, (d2 & 0xFFFF) << 2);                        /* 00B8F0  lsl.w  #2,d2 */
    a1 = a1 + SX16(d2);                                  /* 00B8F2  lea  $0(a1,d2.w),a1 */
    a2 = 0x49A3A;                                        /* 00B8F6  lea  $49a3a.l,a2 */
    SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 7, 16));      /* 00B8FC  lsl.w  #7,d3 */
    a2 = a2 + SX16(d3);                                  /* 00B8FE  lea  $0(a2,d3.w),a2 */
    a1 += 0x20;                                          /* 00B902  adda.l  #$20,a1 */
    do {
        BUDGET_CHECK(0xB908U);
L_00B908:
        d0 = 0;                                              /* 00B908  moveq  #0,d0 */
        d1 = 0;                                              /* 00B90A  moveq  #0,d1 */
        d2 = 0;                                              /* 00B90C  moveq  #0,d2 */
        d3 = 0;                                              /* 00B90E  moveq  #0,d3 */
        /* 00B910  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        /* 00B912  move.w  (a2)+,d1 */
        SETW(d1, rd16(a2));
        a2 += 2;
        /* 00B914  move.w  (a2)+,d2 */
        SETW(d2, rd16(a2));
        a2 += 2;
        /* 00B916  move.w  (a2)+,d3 */
        SETW(d3, rd16(a2));
        a2 += 2;
        d0 = shift_op(3, 1, d0, (d5 & 63), 32);              /* 00B918  rol.l  d5,d0 */
        d1 = shift_op(3, 1, d1, (d5 & 63), 32);              /* 00B91A  rol.l  d5,d1 */
        d2 = shift_op(3, 1, d2, (d5 & 63), 32);              /* 00B91C  rol.l  d5,d2 */
        d3 = shift_op(3, 1, d3, (d5 & 63), 32);              /* 00B91E  rol.l  d5,d3 */
        SETW(d6, (d0 & 0xFFFF));                             /* 00B920  move.w  d0,d6 */
        SETW(d6, d6 | (d1 & 0xFFFF));                        /* 00B922  or.w  d1,d6 */
        SETW(d6, d6 | (d2 & 0xFFFF));                        /* 00B924  or.w  d2,d6 */
        SETW(d6, d6 | (d3 & 0xFFFF));                        /* 00B926  or.w  d3,d6 */
        SETW(d6, ~(d6 & 0xFFFF));                            /* 00B928  not.w  d6 */
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B92A  and.w  d6,(a1) */
        /* 00B92C  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B92E  and.w  d6,(a1) */
        /* 00B930  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B932  and.w  d6,(a1) */
        /* 00B934  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 00B936  and.w  d6,(a1) */
        wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 00B938  or.w  d3,(a1) */
        a1 = a1 + 0x9A;                                      /* 00B93A  lea  $9a(a1),a1 */
        /* 00B93E  dbra  d7,$b908 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 00B942  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB734U: goto L_00B734;
        case 0xB740U: goto L_00B740;
        case 0xB752U: goto L_00B752;
        case 0xB76CU: goto L_00B76C;
        case 0xB774U: goto L_00B774;
        case 0xB77CU: goto L_00B77C;
        case 0xB7A6U: goto L_00B7A6;
        case 0xB7A8U: goto L_00B7A8;
        case 0xB7AAU: goto L_00B7AA;
        case 0xB7ACU: goto L_00B7AC;
        case 0xB890U: goto L_00B890;
        case 0xB908U: goto L_00B908;
        }
    }
}

/* 00B7AE  game routine */
void sub_00B7AE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, (d0 & 0xFFFF) << 3);                        /* 00B7AE  lsl.w  #3,d0 */
    a1 = a1 + SX16(d0);                                  /* 00B7B0  lea  $0(a1,d0.w),a1 */
    a2 = 0x49A3A;                                        /* 00B7B4  lea  $49a3a.l,a2 */
    SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 7, 16));      /* 00B7BA  lsl.w  #7,d3 */
    a2 = a2 + SX16(d3);                                  /* 00B7BC  lea  $0(a2,d3.w),a2 */
    a2 = a2 + SX16(d5);                                  /* 00B7C0  lea  $0(a2,d5.w),a2 */
    do {
        BUDGET_CHECK(0xB7C4U);
L_00B7C4:
        /* 00B7C4  movem.w  (a2)+,d0-d3 */
        { uint32_t ea;
        ea = a2;
        d0 = SX16(rd16(ea));
        ea += 2;
        d1 = SX16(rd16(ea));
        ea += 2;
        d2 = SX16(rd16(ea));
        ea += 2;
        d3 = SX16(rd16(ea));
        ea += 2;
        a2 = ea;
        }
        SETW(d4, (d0 & 0xFFFF));                             /* 00B7C8  move.w  d0,d4 */
        SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00B7CA  or.w  d1,d4 */
        SETW(d4, d4 | (d2 & 0xFFFF));                        /* 00B7CC  or.w  d2,d4 */
        SETW(d4, d4 | (d3 & 0xFFFF));                        /* 00B7CE  or.w  d3,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 00B7D0  not.w  d4 */
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B7D2  and.w  d4,(a1) */
        /* 00B7D4  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B7D6  and.w  d4,(a1) */
        /* 00B7D8  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B7DA  and.w  d4,(a1) */
        /* 00B7DC  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B7DE  and.w  d4,(a1) */
        wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 00B7E0  or.w  d3,(a1) */
        a1 = a1 + 0x9A;                                      /* 00B7E2  lea  $9a(a1),a1 */
        /* 00B7E6  dbra  d7,$b7c4 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B7EA:
    RTS();                                               /* 00B7EA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB7C4U: goto L_00B7C4;
        case 0xB7EAU: goto L_00B7EA;
        }
    }
}

/* 00B7EC  game routine */
void sub_00B7EC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a2 = 0x49A3A;                                        /* 00B7EC  lea  $49a3a.l,a2 */
    d3 = (d3 & 0xFFFF) * 0x100;                          /* 00B7F2  mulu.w  #$100,d3 */
    a2 = a2 + SX16(d3);                                  /* 00B7F6  lea  $0(a2,d3.w),a2 */
    SETW(d5, (d5 & 0xFFFF) << 1);                        /* 00B7FA  lsl.w  #1,d5 */
    a2 += d5;                                            /* 00B7FC  adda.l  d5,a2 */
    SETW(d1, (d1 & 0xFFFF) >> 1);                        /* 00B7FE  lsr.w  #1,d1 */
    d1 = (d1 & 0xFFFF) * 0xC00;                          /* 00B800  mulu.w  #$c00,d1 */
    a2 = a2 + d1;                                        /* 00B804  lea  $0(a2,d1.l),a2 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 00B808  lsl.w  #3,d0 */
    a1 = a1 + SX16(d0);                                  /* 00B80A  lea  $0(a1,d0.w),a1 */
    do {
        BUDGET_CHECK(0xB80EU);
L_00B80E:
        /* 00B80E  movem.w  (a2)+,d0-d3 */
        { uint32_t ea;
        ea = a2;
        d0 = SX16(rd16(ea));
        ea += 2;
        d1 = SX16(rd16(ea));
        ea += 2;
        d2 = SX16(rd16(ea));
        ea += 2;
        d3 = SX16(rd16(ea));
        ea += 2;
        a2 = ea;
        }
        SETW(d4, (d0 & 0xFFFF));                             /* 00B812  move.w  d0,d4 */
        SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00B814  or.w  d1,d4 */
        SETW(d4, d4 | (d2 & 0xFFFF));                        /* 00B816  or.w  d2,d4 */
        SETW(d4, d4 | (d3 & 0xFFFF));                        /* 00B818  or.w  d3,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 00B81A  not.w  d4 */
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B81C  and.w  d4,(a1) */
        /* 00B81E  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B820  and.w  d4,(a1) */
        /* 00B822  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B824  and.w  d4,(a1) */
        /* 00B826  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B828  and.w  d4,(a1) */
        /* 00B82A  or.w  d3,(a1)+ */
        {
            uint32_t s = (d3 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 00B82C  movem.w  (a2)+,d0-d3 */
        { uint32_t ea;
        ea = a2;
        d0 = SX16(rd16(ea));
        ea += 2;
        d1 = SX16(rd16(ea));
        ea += 2;
        d2 = SX16(rd16(ea));
        ea += 2;
        d3 = SX16(rd16(ea));
        ea += 2;
        a2 = ea;
        }
        SETW(d4, (d0 & 0xFFFF));                             /* 00B830  move.w  d0,d4 */
        SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00B832  or.w  d1,d4 */
        SETW(d4, d4 | (d2 & 0xFFFF));                        /* 00B834  or.w  d2,d4 */
        SETW(d4, d4 | (d3 & 0xFFFF));                        /* 00B836  or.w  d3,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 00B838  not.w  d4 */
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B83A  and.w  d4,(a1) */
        /* 00B83C  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B83E  and.w  d4,(a1) */
        /* 00B840  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B842  and.w  d4,(a1) */
        /* 00B844  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00B846  and.w  d4,(a1) */
        wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 00B848  or.w  d3,(a1) */
        a1 = a1 + 0x92;                                      /* 00B84A  lea  $92(a1),a1 */
        /* 00B84E  dbra  d7,$b80e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B852:
    RTS();                                               /* 00B852  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB80EU: goto L_00B80E;
        case 0xB852U: goto L_00B852;
        }
    }
}

/* 00B944  game routine */
void sub_00B944(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = V_1754;                                         /* 00B944  lea  $1754(a5),a0 */
    d7 = 0x18;                                           /* 00B948  moveq  #24,d7 */
    do {
        BUDGET_CHECK(0xB94AU);
L_00B94A:
        /* 00B94A  tst.w  $8(a0) */
        /* 00B94E  beq  $bab0 */
        if (rd16(a0 + 0x8) != 0) {
L_00B952:
            a4 = V_ENEMIES;                                      /* 00B952  lea  $412(a5),a4 */
            SETW(d0, rd16(a0));                                  /* 00B956  move.w  (a0),d0 */
            SETW(d1, rd16(a0 + 0x2));                            /* 00B958  move.w  $2(a0),d1 */
            d6 = 0x18;                                           /* 00B95C  moveq  #24,d6 */
            do {
                BUDGET_CHECK(0xB95EU);
L_00B95E:
                /* 00B95E  tst.w  $4(a4) */
                /* 00B962  beq  $ba08 */
                if (rd16(a4 + 0x4) != 0) {
L_00B966:
                    /* 00B966  tst.w  $26(a4) */
                    /* 00B96A  bne  $ba08 */
                    if (rd16(a4 + 0x26) == 0) {
L_00B96E:
                        SETW(d2, (d0 & 0xFFFF));                             /* 00B96E  move.w  d0,d2 */
                        SETW(d2, d2 - rd16(a4));                             /* 00B970  sub.w  (a4),d2 */
                        SETW(d2, d2 + 0xE);                                  /* 00B972  addi.w  #$e,d2 */
                        /* 00B976  cmp.w  #$1e,d2 */
                        /* 00B97A  bhi  $ba08 */
                        if ((d2 & 0xFFFF) <= 0x1E) {
L_00B97E:
                            SETW(d2, (d1 & 0xFFFF));                             /* 00B97E  move.w  d1,d2 */
                            SETW(d2, d2 - rd16(a4 + 0x2));                       /* 00B980  sub.w  $2(a4),d2 */
                            SETW(d2, d2 + 0xE);                                  /* 00B984  addi.w  #$e,d2 */
                            /* 00B988  cmp.w  #$23,d2 */
                            /* 00B98C  bhi  $ba08 */
                            if ((d2 & 0xFFFF) <= 0x23) {
L_00B98E:
                                wr16(a0 + 0x8, 0);                                   /* 00B98E  clr.w  $8(a0) */
                                wr16(a4 + 0x26, 0xFFFF);                             /* 00B992  move.w  #$ffff,$26(a4) */
                                /* 00B998  cmpi.w  #$11,$8(a4) */
                                /* 00B99E  bcc  $b9a4 */
                                if (rd16(a4 + 0x8) < 0x11) {
L_00B9A0:
                                    d0 = 4;                                              /* 00B9A0  moveq  #4,d0 */
                                    /* 00B9A2  bra  $b9aa */
                                } else {
L_00B9A4:
                                    d0 = 0xB;                                            /* 00B9A4  move.l  #$b,d0 */
                                }
L_00B9AA:
                                a6 = a4;                                             /* 00B9AA  movea.l  a4,a6 */
                                CALL(sub_00B1E4, 0xB9B0U);                           /* 00B9AC  bsr  $b1e4 */
L_00B9B0:
                                wr16(a4 + 0xC, 0);                                   /* 00B9B0  clr.w  $c(a4) */
                                wr16(a4 + 0xE, 0);                                   /* 00B9B4  clr.w  $e(a4) */
                                CALL(sub_007576, 0xB9BCU);                           /* 00B9B8  bsr  $7576 */
L_00B9BC:
                                SETW(d0, (d0 & 0xFFFF) >> 2);                        /* 00B9BC  lsr.w  #2,d0 */
                                /* 00B9BE  andi.w  #$3,d0 */
                                {
                                    uint32_t s = 3;
                                    uint32_t d = (d0 & 0xFFFF);
                                    uint32_t r = (d & s) & 0xFFFF;
                                    SETW(d0, r);
                                    fz = (r == 0);
                                }
                                /* 00B9C2  beq  $b9c6 */
                                if (COND(6)) {
L_00B9C4:
                                    SETW(d0, d0 - 1);                                    /* 00B9C4  subq.w  #1,d0 */
                                }
L_00B9C6:
                                SETW(d0, d0 + 4);                                    /* 00B9C6  addq.w  #4,d0 */
                                CALL(sub_00E756, 0xB9CCU);                           /* 00B9C8  bsr  $e756 */
L_00B9CC:
                                wr32(V_02E2, rd32(V_02E2) + 0x32);                   /* 00B9CC  addi.l  #$32,$2e2(a5) */
                                /* 00B9D4  addi.w  #$1,$370(a5) */
                                {
                                    uint32_t s = 1;
                                    uint32_t d = rd16(V_0370);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    wr16(V_0370, r);
                                    fx = (r < s);
                                }
                                /* 00B9DA  tst.w  $a(a0) */
                                /* 00B9DE  beq  $ba04 */
                                if (rd16(a0 + 0xA) != 0) {
L_00B9E0:
                                    wr32(V_02E2, rd32(V_02E2) - 0x32);                   /* 00B9E0  subi.l  #$32,$2e2(a5) */
                                    /* 00B9E8  subi.w  #$1,$370(a5) */
                                    {
                                        uint32_t s = 1;
                                        uint32_t d = rd16(V_0370);
                                        uint32_t r = (d - s) & 0xFFFF;
                                        wr16(V_0370, r);
                                        fx = (s > d);
                                    }
                                    /* 00B9EE  cmpi.w  #$1,$a(a0) */
                                    /* 00B9F4  bne  $ba04 */
                                    if (rd16(a0 + 0xA) == 1) {
                                        wr16(V_040C, rd16(V_040C) + 1);                      /* 00B9F6  addi.w  #$1,$40c(a5) */
                                        /* 00B9FC  addi.l  #$32,$37e(a5) */
                                        {
                                            uint32_t s = 0x32;
                                            uint32_t d = rd32(V_037E);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_037E, r);
                                            fx = (r < s);
                                        }
                                    }
                                }
L_00BA04:
                                /* 00BA04  bra  $bab0 */
                                goto L_00BAB0;
                            }
                        }
                    }
                }
L_00BA08:
                a4 = a4 + 0x40;                                      /* 00BA08  lea  $40(a4),a4 */
                /* 00BA0C  dbra  d6,$b95e */
                SETW(d6, d6 - 1);
            } while ((d6 & 0xFFFF) != 0xFFFF);
L_00BA10:
            /* 00BA10  tst.w  $254(a5) */
            /* 00BA14  bne  $ba7c */
            if (rd16(V_0254) == 0) {
L_00BA16:
                a4 = V_02DA;                                         /* 00BA16  lea  $2da(a5),a4 */
                wr16(V_027A, 0);                                     /* 00BA1A  clr.w  $27a(a5) */
                d6 = 1;                                              /* 00BA1E  moveq  #1,d6 */
                do {
                    BUDGET_CHECK(0xBA20U);
L_00BA20:
                    SETW(d0, rd16(a0));                                  /* 00BA20  move.w  (a0),d0 */
                    SETW(d1, rd16(a0 + 0x2));                            /* 00BA22  move.w  $2(a0),d1 */
                    /* 00BA26  tst.w  (a4) */
                    /* 00BA28  beq  $ba70 */
                    if (rd16(a4) != 0) {
L_00BA2A:
                        /* 00BA2A  tst.w  $50(a4) */
                        /* 00BA2E  bne  $ba70 */
                        if (rd16(a4 + 0x50) == 0) {
L_00BA30:
                            /* 00BA30  tst.w  $2a(a4) */
                            /* 00BA34  bne  $ba70 */
                            if (rd16(a4 + 0x2A) == 0) {
L_00BA36:
                                SETW(d5, rd16(a0 + 0xA));                            /* 00BA36  move.w  $a(a0),d5 */
                                /* 00BA3A  cmp.w  $27a(a5),d5 */
                                /* 00BA3E  beq  $ba70 */
                                if ((d5 & 0xFFFF) != rd16(V_027A)) {
L_00BA40:
                                    SETW(d2, (d0 & 0xFFFF));                             /* 00BA40  move.w  d0,d2 */
                                    SETW(d2, d2 - rd16(a4 + 0x2));                       /* 00BA42  sub.w  $2(a4),d2 */
                                    SETW(d2, d2 - 0x24);                                 /* 00BA46  subi.w  #$24,d2 */
                                    SETW(d2, d2 + 9);                                    /* 00BA4A  addi.w  #$9,d2 */
                                    /* 00BA4E  cmp.w  #$22,d2 */
                                    /* 00BA52  bhi  $ba70 */
                                    if ((d2 & 0xFFFF) <= 0x22) {
L_00BA54:
                                        SETW(d2, (d1 & 0xFFFF));                             /* 00BA54  move.w  d1,d2 */
                                        SETW(d2, d2 - rd16(a4 + 0x4));                       /* 00BA56  sub.w  $4(a4),d2 */
                                        SETW(d2, d2 - 0x64);                                 /* 00BA5A  subi.w  #$64,d2 */
                                        /* 00BA5E  addi.w  #$e,d2 */
                                        {
                                            uint32_t s = 0xE;
                                            uint32_t d = (d2 & 0xFFFF);
                                            uint32_t r = (d + s) & 0xFFFF;
                                            SETW(d2, r);
                                            fx = (r < s);
                                        }
                                        /* 00BA62  cmp.w  #$25,d2 */
                                        /* 00BA66  bhi  $ba70 */
                                        if ((d2 & 0xFFFF) <= 0x25) {
L_00BA68:
                                            wr16(a0 + 0x8, 0);                                   /* 00BA68  clr.w  $8(a0) */
                                            CALL(player_start_death, 0xBA70U);                   /* 00BA6C  bsr  $aa42 */
                                        }
                                    }
                                }
                            }
                        }
                    }
L_00BA70:
                    wr16(V_027A, rd16(V_027A) + 1);                      /* 00BA70  addq.w  #1,$27a(a5) */
                    a4 = V_0376;                                         /* 00BA74  lea  $376(a5),a4 */
                    /* 00BA78  dbra  d6,$ba20 */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
            }
L_00BA7C:
            SETW(d0, rd16(a0 + 0x4));                            /* 00BA7C  move.w  $4(a0),d0 */
            wr16(a0, rd16(a0) + (d0 & 0xFFFF));                  /* 00BA80  add.w  d0,(a0) */
            SETW(d0, rd16(a0 + 0x6));                            /* 00BA82  move.w  $6(a0),d0 */
            wr16(a0 + 0x2, rd16(a0 + 0x2) + (d0 & 0xFFFF));      /* 00BA86  add.w  d0,$2(a0) */
            /* 00BA8A  addi.w  #$1,$e(a0) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a0 + 0xE);
                uint32_t r = (d + s) & 0xFFFF;
                wr16(a0 + 0xE, r);
                fx = (r < s);
            }
            /* 00BA90  cmpi.w  #$c,$e(a0) */
            /* 00BA96  bne  $ba9e */
            if (rd16(a0 + 0xE) == 0xC) {
L_00BA98:
                wr16(a0 + 0x8, 0);                                   /* 00BA98  clr.w  $8(a0) */
                /* 00BA9C  bra  $bab0 */
            } else {
L_00BA9E:
                /* 00BA9E  tst.w  $c(a0) */
                /* 00BAA2  bne  $baaa */
                if (rd16(a0 + 0xC) == 0) {
                    wr16(a0 + 0x8, 0);                                   /* 00BAA4  clr.w  $8(a0) */
                    /* 00BAA8  bra  $bab0 */
                } else {
L_00BAAA:
                    /* 00BAAA  subi.w  #$1,$c(a0) */
                    {
                        uint32_t s = 1;
                        uint32_t d = rd16(a0 + 0xC);
                        uint32_t r = (d - s) & 0xFFFF;
                        wr16(a0 + 0xC, r);
                        fx = (s > d);
                    }
                }
            }
        }
L_00BAB0:
        a0 = a0 + 0x10;                                      /* 00BAB0  lea  $10(a0),a0 */
        /* 00BAB4  dbra  d7,$b94a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BAB8:
    RTS();                                               /* 00BAB8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xB94AU: goto L_00B94A;
        case 0xB952U: goto L_00B952;
        case 0xB95EU: goto L_00B95E;
        case 0xB966U: goto L_00B966;
        case 0xB96EU: goto L_00B96E;
        case 0xB97EU: goto L_00B97E;
        case 0xB98EU: goto L_00B98E;
        case 0xB9A0U: goto L_00B9A0;
        case 0xB9A4U: goto L_00B9A4;
        case 0xB9AAU: goto L_00B9AA;
        case 0xB9B0U: goto L_00B9B0;
        case 0xB9BCU: goto L_00B9BC;
        case 0xB9C4U: goto L_00B9C4;
        case 0xB9C6U: goto L_00B9C6;
        case 0xB9CCU: goto L_00B9CC;
        case 0xB9E0U: goto L_00B9E0;
        case 0xBA04U: goto L_00BA04;
        case 0xBA08U: goto L_00BA08;
        case 0xBA10U: goto L_00BA10;
        case 0xBA16U: goto L_00BA16;
        case 0xBA20U: goto L_00BA20;
        case 0xBA2AU: goto L_00BA2A;
        case 0xBA30U: goto L_00BA30;
        case 0xBA36U: goto L_00BA36;
        case 0xBA40U: goto L_00BA40;
        case 0xBA54U: goto L_00BA54;
        case 0xBA68U: goto L_00BA68;
        case 0xBA70U: goto L_00BA70;
        case 0xBA7CU: goto L_00BA7C;
        case 0xBA98U: goto L_00BA98;
        case 0xBA9EU: goto L_00BA9E;
        case 0xBAAAU: goto L_00BAAA;
        case 0xBAB0U: goto L_00BAB0;
        case 0xBAB8U: goto L_00BAB8;
        }
    }
}

/* 00BD22  game routine */
void sub_00BD22(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5649A;                                        /* 00BD22  lea  $5649a.l,a0 */
    d0 = (d0 & 0xFFFF) * 0x200;                          /* 00BD28  mulu.w  #$200,d0 */
    a0 = a0 + SX16(d0);                                  /* 00BD2C  lea  $0(a0,d0.w),a0 */
    a1 = 0x20468;                                        /* 00BD30  lea  $20468.l,a1 */
    SETW(d5, 0x1F);                                      /* 00BD36  move.w  #$1f,d5 */
    do {
        BUDGET_CHECK(0xBD3AU);
L_00BD3A:
        /* 00BD3A  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD3C  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD3E  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD40  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        a1 = a1 + 0x90;                                      /* 00BD42  lea  $90(a1),a1 */
        /* 00BD46  dbra  d5,$bd3a */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
L_00BD4A:
    RTS();                                               /* 00BD4A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBD3AU: goto L_00BD3A;
        case 0xBD4AU: goto L_00BD4A;
        }
    }
}

/* 00BD4C  game routine */
void sub_00BD4C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x20468;                                        /* 00BD4C  lea  $20468.l,a1 */
    SETW(d7, 0x23);                                      /* 00BD52  move.w  #$23,d7 */
    do {
        BUDGET_CHECK(0xBD56U);
L_00BD56:
        /* 00BD56  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 00BD58  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 00BD5A  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        /* 00BD5C  clr.l  (a1)+ */
        wr32(a1, 0);
        a1 += 4;
        a1 = a1 + 0x90;                                      /* 00BD5E  lea  $90(a1),a1 */
        /* 00BD62  dbra  d7,$bd56 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BD66:
    a1 = 0x20020;                                        /* 00BD66  lea  $20020.l,a1 */
    SETW(d7, 0x2C);                                      /* 00BD6C  move.w  #$2c,d7 */
    d0 = 0;                                              /* 00BD70  moveq  #0,d0 */
    do {
        BUDGET_CHECK(0xBD72U);
L_00BD72:
        wr8(a1 - 0x1, 0);                                    /* 00BD72  clr.b  -$1(a1) */
        wr8(a1 - 0x2, 0);                                    /* 00BD76  clr.b  -$2(a1) */
        wr8(a1 - 0x3, 0);                                    /* 00BD7A  clr.b  -$3(a1) */
        /* 00BD7E  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD80  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD82  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD84  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD86  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD88  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD8A  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD8C  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD8E  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD90  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD92  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD94  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD96  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD98  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD9A  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD9C  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BD9E  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDA0  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDA2  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDA4  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDA6  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDA8  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDAA  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDAC  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDAE  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDB0  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDB2  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDB4  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDB6  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDB8  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDBA  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00BDBC  move.l  d0,(a1)+ */
        {
            uint32_t v = d0;
            wr32(a1, v);
            a1 += 4;
        }
        a1 = a1 + 0x20;                                      /* 00BDBE  lea  $20(a1),a1 */
        /* 00BDC2  dbra  d7,$bd72 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BDC6:
    RTS();                                               /* 00BDC6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBD56U: goto L_00BD56;
        case 0xBD66U: goto L_00BD66;
        case 0xBD72U: goto L_00BD72;
        case 0xBDC6U: goto L_00BDC6;
        }
    }
}

/* 00BDC8  game routine */
void sub_00BDC8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00BDC8  tst.w  $2da(a5) */
    /* 00BDCC  bne  $bdd4 */
    if (rd16(V_02DA) == 0) {
        SETB(d0, rd8(V_0385));                               /* 00BDCE  move.b  $385(a5),d0 */
        /* 00BDD2  bra  $bdd8 */
    } else {
L_00BDD4:
        SETB(d0, rd8(V_02E9));                               /* 00BDD4  move.b  $2e9(a5),d0 */
    }
    /* 00BDD8  tst.b  d0 */
    /* 00BDDA  beq  $bde2 */
    if ((d0 & 0xFF) != 0) {
        /* 00BDDC  subi.w  #$1,$1ec(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01EC);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_01EC, r);
            fx = (s > d);
        }
    }
L_00BDE2:
    /* 00BDE2  tst.w  $2da(a5) */
    /* 00BDE6  bne  $bdee */
    if (rd16(V_02DA) == 0) {
        SETB(d0, rd8(V_0387));                               /* 00BDE8  move.b  $387(a5),d0 */
        /* 00BDEC  bra  $bdf2 */
    } else {
L_00BDEE:
        SETB(d0, rd8(V_02EB));                               /* 00BDEE  move.b  $2eb(a5),d0 */
    }
    /* 00BDF2  tst.b  d0 */
    /* 00BDF4  beq  $bdfc */
    if ((d0 & 0xFF) != 0) {
L_00BDF6:
        /* 00BDF6  addi.w  #$1,$1ee(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01EE);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_01EE, r);
            fx = (r < s);
        }
    }
L_00BDFC:
    /* 00BDFC  tst.w  $2da(a5) */
    /* 00BE00  bne  $be08 */
    if (rd16(V_02DA) == 0) {
        SETB(d0, rd8(V_0384));                               /* 00BE02  move.b  $384(a5),d0 */
        /* 00BE06  bra  $be0c */
    } else {
L_00BE08:
        SETB(d0, rd8(V_02E8));                               /* 00BE08  move.b  $2e8(a5),d0 */
    }
    /* 00BE0C  tst.b  d0 */
    /* 00BE0E  beq  $be16 */
    if ((d0 & 0xFF) != 0) {
L_00BE10:
        /* 00BE10  addi.w  #$1,$1ec(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01EC);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_01EC, r);
            fx = (r < s);
        }
    }
L_00BE16:
    /* 00BE16  tst.w  $2da(a5) */
    /* 00BE1A  bne  $be22 */
    if (rd16(V_02DA) == 0) {
        SETB(d0, rd8(V_0386));                               /* 00BE1C  move.b  $386(a5),d0 */
        /* 00BE20  bra  $be26 */
    } else {
L_00BE22:
        SETB(d0, rd8(V_02EA));                               /* 00BE22  move.b  $2ea(a5),d0 */
    }
    /* 00BE26  tst.b  d0 */
    /* 00BE28  beq  $be30 */
    if ((d0 & 0xFF) != 0) {
L_00BE2A:
        /* 00BE2A  subi.w  #$1,$1ee(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01EE);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_01EE, r);
            fx = (s > d);
        }
    }
L_00BE30:
    SETW(d0, rd16(V_01EC));                              /* 00BE30  move.w  $1ec(a5),d0 */
    /* 00BE34  cmp.w  #$6,d0 */
    /* 00BE38  bhi  $be3e */
    if ((d0 & 0xFFFF) <= 6) {
        SETW(d0, 7);                                         /* 00BE3A  move.w  #$7,d0 */
    }
L_00BE3E:
    /* 00BE3E  cmp.w  #$138,d0 */
    /* 00BE42  bls  $be48 */
    if ((d0 & 0xFFFF) > 0x138) {
        SETW(d0, 0x138);                                     /* 00BE44  move.w  #$138,d0 */
    }
L_00BE48:
    wr16(V_01EC, (d0 & 0xFFFF));                         /* 00BE48  move.w  d0,$1ec(a5) */
    SETW(d0, rd16(V_01EE));                              /* 00BE4C  move.w  $1ee(a5),d0 */
    /* 00BE50  cmp.w  #$3b,d0 */
    /* 00BE54  bhi  $be5a */
    if ((d0 & 0xFFFF) <= 0x3B) {
        SETW(d0, 0x3B);                                      /* 00BE56  move.w  #$3b,d0 */
    }
L_00BE5A:
    /* 00BE5A  cmp.w  #$c3,d0 */
    /* 00BE5E  bls  $be64 */
    if ((d0 & 0xFFFF) > 0xC3) {
        SETW(d0, 0xC3);                                      /* 00BE60  move.w  #$c3,d0 */
    }
L_00BE64:
    wr16(V_01EE, (d0 & 0xFFFF));                         /* 00BE64  move.w  d0,$1ee(a5) */
    RTS();                                               /* 00BE68  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBDD4U: goto L_00BDD4;
        case 0xBDE2U: goto L_00BDE2;
        case 0xBDEEU: goto L_00BDEE;
        case 0xBDF6U: goto L_00BDF6;
        case 0xBDFCU: goto L_00BDFC;
        case 0xBE08U: goto L_00BE08;
        case 0xBE10U: goto L_00BE10;
        case 0xBE16U: goto L_00BE16;
        case 0xBE22U: goto L_00BE22;
        case 0xBE2AU: goto L_00BE2A;
        case 0xBE30U: goto L_00BE30;
        case 0xBE3EU: goto L_00BE3E;
        case 0xBE48U: goto L_00BE48;
        case 0xBE5AU: goto L_00BE5A;
        case 0xBE64U: goto L_00BE64;
        }
    }
}

/* 00BE6A  game routine */
void sub_00BE6A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = rd32(V_01F6);                                   /* 00BE6A  move.l  $1f6(a5),d0 */
    d0 &= ~0x1;                                          /* 00BE6E  bclr  #$0,d0 */
    a0 = d0;                                             /* 00BE72  movea.l  d0,a0 */
    d0 &= 0x7FFF;                                        /* 00BE74  andi.l  #$7fff,d0 */
    a1 = 0x18000;                                        /* 00BE7A  lea  $18000.l,a1 */
    a1 += d0;                                            /* 00BE80  adda.l  d0,a1 */
    d7 = 8;                                              /* 00BE82  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xBE84U);
L_00BE84:
        /* 00BE84  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00BE86  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00BE88  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        wr32(a0, rd32(a1));                                  /* 00BE8A  move.l  (a1),(a0) */
        a0 += SX16(0x94);                                    /* 00BE8C  adda.w  #$94,a0 */
        a1 += SX16(0x94);                                    /* 00BE90  adda.w  #$94,a1 */
        /* 00BE94  dbra  d7,$be84 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BE98:
    RTS();                                               /* 00BE98  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBE84U: goto L_00BE84;
        case 0xBE98U: goto L_00BE98;
        }
    }
}

/* 00BE9A  game routine */
void sub_00BE9A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00BED4, 0xBE9CU);                           /* 00BE9A  bsr  $bed4 */
L_00BE9C:
    wr32(V_01F6, a0);                                    /* 00BE9C  move.l  a0,$1f6(a5) */
    d7 = 8;                                              /* 00BEA0  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xBEA2U);
L_00BEA2:
        /* 00BEA2  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
        wr8(a0, rd8(a0) | (d0 & 0xFF));                      /* 00BEA4  or.b  d0,(a0) */
        SETB(d0, ~(d0 & 0xFF));                              /* 00BEA6  not.b  d0 */
        wr8(a0 + 0x2, rd8(a0 + 0x2) & (d0 & 0xFF));          /* 00BEA8  and.b  d0,$2(a0) */
        wr8(a0 + 0x4, rd8(a0 + 0x4) & (d0 & 0xFF));          /* 00BEAC  and.b  d0,$4(a0) */
        wr8(a0 + 0x6, rd8(a0 + 0x6) & (d0 & 0xFF));          /* 00BEB0  and.b  d0,$6(a0) */
        /* 00BEB4  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
        wr8(a1, rd8(a1) | (d0 & 0xFF));                      /* 00BEB6  or.b  d0,(a1) */
        SETB(d0, ~(d0 & 0xFF));                              /* 00BEB8  not.b  d0 */
        wr8(a1 + 0x2, rd8(a1 + 0x2) & (d0 & 0xFF));          /* 00BEBA  and.b  d0,$2(a1) */
        wr8(a1 + 0x4, rd8(a1 + 0x4) & (d0 & 0xFF));          /* 00BEBE  and.b  d0,$4(a1) */
        wr8(a1 + 0x6, rd8(a1 + 0x6) & (d0 & 0xFF));          /* 00BEC2  and.b  d0,$6(a1) */
        a0 = a0 + 0xA0;                                      /* 00BEC6  lea  $a0(a0),a0 */
        a1 = a1 + 0xA0;                                      /* 00BECA  lea  $a0(a1),a1 */
        /* 00BECE  dbra  d7,$bea2 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BED2:
    RTS();                                               /* 00BED2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBE9CU: goto L_00BE9C;
        case 0xBEA2U: goto L_00BEA2;
        case 0xBED2U: goto L_00BED2;
        }
    }
}

/* 00BED4  game routine */
void sub_00BED4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = rd32(V_DRAW_BUFFER);                            /* 00BED4  movea.l  $28e(a5),a0 */
    SETW(d0, rd16(V_01EE));                              /* 00BED8  move.w  $1ee(a5),d0 */
    SETW(d0, d0 - 4);                                    /* 00BEDC  subq.w  #4,d0 */
    d0 = (d0 & 0xFFFF) * 0xA0;                           /* 00BEDE  mulu.w  #$a0,d0 */
    a0 += d0;                                            /* 00BEE2  adda.l  d0,a0 */
    a1 = a0;                                             /* 00BEE4  movea.l  a0,a1 */
    a1 += 1;                                             /* 00BEE6  addq.l  #1,a1 */
    SETW(d0, rd16(V_01EC));                              /* 00BEE8  move.w  $1ec.l,d0 */
    SETW(d0, d0 - 4);                                    /* 00BEEE  subq.w  #4,d0 */
    d1 = d0;                                             /* 00BEF0  move.l  d0,d1 */
    d1 &= 7;                                             /* 00BEF2  andi.l  #$7,d1 */
    SETW(d0, (d0 & 0xFFFF) >> 3);                        /* 00BEF8  lsr.w  #3,d0 */
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 00BEFA  lsr.w  #1,d0 */
    /* 00BEFC  bcc  $bf02 */
    if (COND(5)) {
L_00BEFE:
        a0 += 1;                                             /* 00BEFE  addq.l  #1,a0 */
        a1 += 7;                                             /* 00BF00  addq.l  #7,a1 */
    }
L_00BF02:
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 00BF02  lsl.w  #3,d0 */
    a0 += d0;                                            /* 00BF04  adda.l  d0,a0 */
    a1 += d0;                                            /* 00BF06  adda.l  d0,a1 */
    a2 = 0xBF1E;                                         /* 00BF08  lea  $bf1e(pc),a2 */
    a3 = 0xBF30;                                         /* 00BF0C  lea  $bf30(pc),a3 */
    d7 = 8;                                              /* 00BF10  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xBF12U);
L_00BF12:
        /* 00BF12  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), (d1 & 63), 16)); /* 00BF14  lsr.w  d1,d0 */
        /* 00BF16  move.w  d0,(a3)+ */
        {
            uint32_t v = (d0 & 0xFFFF);
            wr16(a3, v);
            a3 += 2;
        }
        /* 00BF18  dbra  d7,$bf12 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BF1C:
    RTS();                                               /* 00BF1C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBEFEU: goto L_00BEFE;
        case 0xBF02U: goto L_00BF02;
        case 0xBF12U: goto L_00BF12;
        case 0xBF1CU: goto L_00BF1C;
        }
    }
}

/* 00BF1E  game routine */
void sub_00BF1E(void)
{
    SOFT(0xBF1EU);                                       /* 00BF1E  soft */
}

/* 00BF30  game routine */
void sub_00BF30(void)
{
    d0 |= (1u << (d1 & 31));                             /* 00BF30  bset  d1,d0 */
    d0 &= ~(1u << (d1 & 31));                            /* 00BF32  bclr  d1,d0 */
    /* 00BF34  bset  d1,d0 */
    fz = !(d0 & (1u << (d1 & 31)));
    d0 |= (1u << (d1 & 31));
    MISS(0xBF36U);
}

/* 00BF42  game routine */
void sub_00BF42(void)
{
    SETB(d0, d0 | 0);                                    /* 00BF42  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00BF46  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00BF4A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00BF4E  ori.b  #$0,d0 */
    /* 00BF52  ori.b  #$ff,d0 */
    SETB(d0, d0 | 0xFF);
    { sub_00BF56(); return; }
}

/* 00BF54  game routine */
void sub_00BF54(void)
{
    /* 00BF54  moveq  #-1,d7 */
    d7 = 0xFFFFFFFF;
    { sub_00BF56(); return; }
}

/* 00BF56  game routine */
void sub_00BF56(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, rd16(V_01EC));                              /* 00BF56  move.w  $1ec(a5),d0 */
    SETW(d1, rd16(V_01EE));                              /* 00BF5A  move.w  $1ee(a5),d1 */
    a0 = 0xBF98;                                         /* 00BF5E  lea  $bf98(pc),a0 */
    for (;;) {
        BUDGET_CHECK(0xBF62U);
L_00BF62:
        /* 00BF62  move.w  (a0),d2 */
        /* 00BF64  bmi  $bf96 */
        SETW(d2, rd16(a0));
        if ((int16_t)(d2 & 0xFFFF) < 0) {
            goto L_00BF96;
        }
L_00BF66:
        /* 00BF66  cmp.w  d2,d0 */
        /* 00BF68  bcs  $bf90 */
        if ((d0 & 0xFFFF) >= (d2 & 0xFFFF)) {
L_00BF6C:
            SETW(d2, rd16(a0 + 0x2));                            /* 00BF6C  move.w  $2(a0),d2 */
            /* 00BF70  cmp.w  d2,d0 */
            /* 00BF72  bhi  $bf90 */
            if ((d0 & 0xFFFF) <= (d2 & 0xFFFF)) {
L_00BF76:
                SETW(d2, rd16(a0 + 0x4));                            /* 00BF76  move.w  $4(a0),d2 */
                /* 00BF7A  cmp.w  d2,d1 */
                /* 00BF7C  bcs  $bf90 */
                if ((d1 & 0xFFFF) >= (d2 & 0xFFFF)) {
L_00BF80:
                    SETW(d2, rd16(a0 + 0x6));                            /* 00BF80  move.w  $6(a0),d2 */
                    /* 00BF84  cmp.w  d2,d1 */
                    /* 00BF86  bhi  $bf90 */
                    if ((d1 & 0xFFFF) <= (d2 & 0xFFFF)) {
L_00BF8A:
                        SETW(d7, rd16(a0 + 0x8));                            /* 00BF8A  move.w  $8(a0),d7 */
                        /* 00BF8E  bra  $bf96 */
                        goto L_00BF96;
                    }
                }
            }
        }
L_00BF90:
        a0 = a0 + 0xA;                                       /* 00BF90  lea  $a(a0),a0 */
        /* 00BF94  bra  $bf62 */
    }
L_00BF96:
    RTS();                                               /* 00BF96  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBF62U: goto L_00BF62;
        case 0xBF66U: goto L_00BF66;
        case 0xBF6CU: goto L_00BF6C;
        case 0xBF76U: goto L_00BF76;
        case 0xBF80U: goto L_00BF80;
        case 0xBF8AU: goto L_00BF8A;
        case 0xBF90U: goto L_00BF90;
        case 0xBF96U: goto L_00BF96;
        }
    }
}

/* 00BF98  game routine */
void sub_00BF98(void)
{
    wr32(a4 + 0x51 + SX16(d0), rd32(a4 + 0x51 + SX16(d0)) | 0xB9004B); /* 00BF98  ori.l  #$b9004b,$51(a4,d0.w) */
    SETB(d0, d0 | 0xA0);                                 /* 00BFA0  ori.b  #$a0,d0 */
    /* 00BFA4  ori.l  #$4d0053,-(a5) */
    {
        uint32_t s = 0x4D0053;
        a5 -= 4;
        uint32_t d = rd32(a5);
        uint32_t r = (d | s) & 0xFFFFFFFF;
        wr32(a5, r);
    }
    SETB(d1, d1 | 0xA5);                                 /* 00BFAA  ori.b  #$a5,d1 */
    wr32(a2 + 0x2, rd32(a2 + 0x2) | 0x56005B);           /* 00BFAE  ori.l  #$56005b,$2(a2) */
    wr16(a3 + 0x56 + SX16(d0), rd16(a3 + 0x56 + SX16(d0)) | 0x79); /* 00BFB6  ori.w  #$79,$56(a3,d0.w) */
    /* 00BFBC  ori.w  #$3,(a4)+ */
    {
        uint32_t s = 3;
        uint32_t ea = a4;
        a4 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(ea, r);
    }
    wr32(a3 + 0x62, rd32(a3 + 0x62) | 0xB0005D);         /* 00BFC0  ori.l  #$b0005d,$62(a3) */
    SETB(d4, d4 | 0x5B);                                 /* 00BFC8  ori.b  #$5b,d4 */
    /* 00BFCC  ori.w  #$67,-(a0) */
    {
        uint32_t s = 0x67;
        a0 -= 2;
        uint32_t d = rd16(a0);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a0, r);
    }
    wr16(a4 + 0x67, rd16(a4 + 0x67) | 5);                /* 00BFD0  ori.w  #$5,$67(a4) */
    wr16(a4 + 0x6D, rd16(a4 + 0x6D) | 0x67);             /* 00BFD6  ori.w  #$67,$6d(a4) */
    /* 00BFDC  ori.b  #$ce,d6 */
    {
        uint32_t s = 0xCE;
        uint32_t d = (d6 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d6, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xBFE0U);
}

