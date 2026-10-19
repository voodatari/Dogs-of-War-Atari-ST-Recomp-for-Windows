/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 00A082  game routine */
void sub_00A082(void)
{
    SETB(d0, d0 | 0x36);                                 /* 00A082  ori.b  #$36,d0 */
    /* 00A086  ori.w  #$0,(a5)+ */
    {
        uint32_t s = 0;
        uint32_t ea = a5;
        a5 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(ea, r);
    }
    SETB(d0, d0 | 1);                                    /* 00A08A  ori.b  #$1,d0 */
    SETB(d0, d0 | 0x36);                                 /* 00A08E  ori.b  #$36,d0 */
    /* 00A092  ori.w  #$0,(a5)+ */
    {
        uint32_t s = 0;
        uint32_t ea = a5;
        a5 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(ea, r);
    }
    SETB(d0, d0 | 1);                                    /* 00A096  ori.b  #$1,d0 */
    SETB(d1, d1 | 0x2C);                                 /* 00A09A  ori.b  #$2c,d1 */
    wr16(a7 + SX16(d0), rd16(a7 + SX16(d0)) | 0);        /* 00A09E  ori.w  #$0,$0(a7,d0.w) */
    /* 00A0A4  ori.b  #$ff,d1 */
    {
        uint32_t s = 0xFF;
        uint32_t d = (d1 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d1, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xA0A8U);                                       /* 00A0A8  soft */
}

/* 00A2C2  game routine */
void sub_00A2C2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1614;                                         /* 00A2C2  lea  $1614(a5),a6 */
    SETW(d7, 0x13);                                      /* 00A2C6  move.w  #$13,d7 */
    do {
        BUDGET_CHECK(0xA2CAU);
L_00A2CA:
        /* 00A2CA  tst.w  (a6) */
        /* 00A2CC  beq  $a3f4 */
        if (rd16(a6) != 0) {
L_00A2D0:
            /* 00A2D0  cmpi.w  #$10,$6(a6) */
            /* 00A2D6  bcs  $a3f4 */
            if (rd16(a6 + 0x6) >= 0x10) {
L_00A2DA:
                /* 00A2DA  cmpi.w  #$17,$6(a6) */
                /* 00A2E0  bhi  $a3f4 */
                if (rd16(a6 + 0x6) <= 0x17) {
                    a4 = V_18E6;                                         /* 00A2E4  lea  $18e6(a5),a4 */
                    SETW(d6, 0x13);                                      /* 00A2E8  move.w  #$13,d6 */
                    do {
                        BUDGET_CHECK(0xA2ECU);
L_00A2EC:
                        /* 00A2EC  tst.w  $4(a4) */
                        /* 00A2F0  beq  $a3ec */
                        if (rd16(a4 + 0x4) != 0) {
                            /* 00A2F4  tst.w  $c(a4) */
                            /* 00A2F8  bmi  $a3ec */
                            if ((int16_t)rd16(a4 + 0xC) >= 0) {
                                SETW(d0, rd16(a6 + 0x2));                            /* 00A2FC  move.w  $2(a6),d0 */
                                SETW(d1, rd16(a6 + 0x4));                            /* 00A300  move.w  $4(a6),d1 */
                                SETW(d2, rd16(a4));                                  /* 00A304  move.w  (a4),d2 */
                                SETW(d3, rd16(a4 + 0x2));                            /* 00A306  move.w  $2(a4),d3 */
                                SETW(d3, d3 + 0x60);                                 /* 00A30A  addi.w  #$60,d3 */
                                SETW(d2, d2 + 0x20);                                 /* 00A30E  addi.w  #$20,d2 */
                                /* 00A312  sub.w  d3,d1 */
                                {
                                    uint32_t s = (d3 & 0xFFFF);
                                    uint32_t d = (d1 & 0xFFFF);
                                    uint32_t r = (d - s) & 0xFFFF;
                                    SETW(d1, r);
                                    fx = (s > d);
                                }
                                /* 00A314  cmp.w  #$1b,d1 */
                                /* 00A318  bhi  $a3ec */
                                if ((d1 & 0xFFFF) <= 0x1B) {
                                    /* 00A31C  cmp.w  #$0,d1 */
                                    /* 00A320  bcs  $a3ec */
                                    if ((d1 & 0xFFFF) >= 0) {
                                        /* 00A324  sub.w  d2,d0 */
                                        {
                                            uint32_t s = (d2 & 0xFFFF);
                                            uint32_t d = (d0 & 0xFFFF);
                                            uint32_t r = (d - s) & 0xFFFF;
                                            SETW(d0, r);
                                            fx = (s > d);
                                        }
                                        /* 00A326  cmp.w  #$20,d0 */
                                        /* 00A32A  bhi  $a3ec */
                                        if ((d0 & 0xFFFF) <= 0x20) {
                                            /* 00A32E  cmp.w  #$0,d0 */
                                            /* 00A332  bcs  $a3ec */
                                            if ((d0 & 0xFFFF) >= 0) {
                                                wr32(V_02E2, rd32(V_02E2) + 0x3E8);                  /* 00A336  addi.l  #$3e8,$2e2(a5) */
                                                /* 00A33E  tst.w  $e(a6) */
                                                /* 00A342  beq  $a354 */
                                                if (rd16(a6 + 0xE) != 0) {
                                                    wr32(V_02E2, rd32(V_02E2) - 0x3E8);                  /* 00A344  subi.l  #$3e8,$2e2(a5) */
                                                    wr32(V_037E, rd32(V_037E) + 0x3E8);                  /* 00A34C  addi.l  #$3e8,$37e(a5) */
                                                }
                                                wr16(a4 + 0xA, 0xF5);                                /* 00A354  move.w  #$f5,$a(a4) */
                                                wr16(a6, 0);                                         /* 00A35A  clr.w  (a6) */
                                                wr16(a4 + 0xC, 0xFFFF);                              /* 00A35C  move.w  #$ffff,$c(a4) */
                                                SETW(d0, rd16(a4));                                  /* 00A362  move.w  (a4),d0 */
                                                SETW(d1, rd16(a4 + 0x2));                            /* 00A364  move.w  $2(a4),d1 */
                                                fz = !(rd8(V_1DC7) & 0x10);                          /* 00A368  btst  #$4,$1dc7(a5) */
                                                /* 00A36E  bne  $a3ae */
                                                if (COND(7)) {
                                                    SETW(d1, d1 + 0x56);                                 /* 00A370  addi.w  #$56,d1 */
                                                    /* 00A374  addi.w  #$23,d0 */
                                                    {
                                                        uint32_t s = 0x23;
                                                        uint32_t d = (d0 & 0xFFFF);
                                                        uint32_t r = (d + s) & 0xFFFF;
                                                        SETW(d0, r);
                                                        fx = (r < s);
                                                    }
                                                    d2 = 0;                                              /* 00A378  moveq  #0,d2 */
                                                    CALL(sub_0063A4, 0xA37EU);                           /* 00A37A  bsr  $63a4 */
L_00A37E:
                                                    SETW(d0, d0 + 0x10);                                 /* 00A37E  addi.w  #$10,d0 */
                                                    /* 00A382  addi.w  #$10,d1 */
                                                    {
                                                        uint32_t s = 0x10;
                                                        uint32_t d = (d1 & 0xFFFF);
                                                        uint32_t r = (d + s) & 0xFFFF;
                                                        SETW(d1, r);
                                                        fx = (r < s);
                                                    }
                                                    d2 = 0;                                              /* 00A386  moveq  #0,d2 */
                                                    CALL(sub_0063A4, 0xA38CU);                           /* 00A388  bsr  $63a4 */
L_00A38C:
                                                    SETW(d0, d0 + 0x10);                                 /* 00A38C  addi.w  #$10,d0 */
                                                    /* 00A390  subi.w  #$5,d1 */
                                                    {
                                                        uint32_t s = 5;
                                                        uint32_t d = (d1 & 0xFFFF);
                                                        uint32_t r = (d - s) & 0xFFFF;
                                                        SETW(d1, r);
                                                        fx = (s > d);
                                                    }
                                                    SETW(d2, 0xFFFF);                                    /* 00A394  move.w  #$ffff,d2 */
                                                    CALL(sub_006CA8, 0xA39CU);                           /* 00A398  bsr  $6ca8 */
L_00A39C:
                                                    SETW(d1, d1 + 0x10);                                 /* 00A39C  addi.w  #$10,d1 */
                                                    /* 00A3A0  subi.w  #$20,d0 */
                                                    {
                                                        uint32_t s = 0x20;
                                                        uint32_t d = (d0 & 0xFFFF);
                                                        uint32_t r = (d - s) & 0xFFFF;
                                                        SETW(d0, r);
                                                        fx = (s > d);
                                                    }
                                                    SETW(d2, 0xFFFF);                                    /* 00A3A4  move.w  #$ffff,d2 */
                                                    CALL(sub_006CA8, 0xA3ACU);                           /* 00A3A8  bsr  $6ca8 */
L_00A3AC:
                                                    /* 00A3AC  bra  $a3f4 */
                                                    goto L_00A3F4;
                                                }
                                                SETW(d1, d1 + 0x5E);                                 /* 00A3AE  addi.w  #$5e,d1 */
                                                /* 00A3B2  addi.w  #$20,d0 */
                                                {
                                                    uint32_t s = 0x20;
                                                    uint32_t d = (d0 & 0xFFFF);
                                                    uint32_t r = (d + s) & 0xFFFF;
                                                    SETW(d0, r);
                                                    fx = (r < s);
                                                }
                                                SETW(d2, 0xFFFF);                                    /* 00A3B6  move.w  #$ffff,d2 */
                                                CALL(sub_006CA8, 0xA3BEU);                           /* 00A3BA  bsr  $6ca8 */
L_00A3BE:
                                                SETW(d0, d0 + 0x20);                                 /* 00A3BE  addi.w  #$20,d0 */
                                                /* 00A3C2  addi.w  #$10,d1 */
                                                {
                                                    uint32_t s = 0x10;
                                                    uint32_t d = (d1 & 0xFFFF);
                                                    uint32_t r = (d + s) & 0xFFFF;
                                                    SETW(d1, r);
                                                    fx = (r < s);
                                                }
                                                SETW(d2, 0xFFFF);                                    /* 00A3C6  move.w  #$ffff,d2 */
                                                CALL(sub_006CA8, 0xA3CEU);                           /* 00A3CA  bsr  $6ca8 */
L_00A3CE:
                                                SETW(d0, d0 - 0x10);                                 /* 00A3CE  subi.w  #$10,d0 */
                                                /* 00A3D2  subi.w  #$19,d1 */
                                                {
                                                    uint32_t s = 0x19;
                                                    uint32_t d = (d1 & 0xFFFF);
                                                    uint32_t r = (d - s) & 0xFFFF;
                                                    SETW(d1, r);
                                                    fx = (s > d);
                                                }
                                                d2 = 0;                                              /* 00A3D6  moveq  #0,d2 */
                                                CALL(sub_0063A4, 0xA3DCU);                           /* 00A3D8  bsr  $63a4 */
L_00A3DC:
                                                SETW(d1, d1 + 0x10);                                 /* 00A3DC  addi.w  #$10,d1 */
                                                /* 00A3E0  subi.w  #$10,d0 */
                                                {
                                                    uint32_t s = 0x10;
                                                    uint32_t d = (d0 & 0xFFFF);
                                                    uint32_t r = (d - s) & 0xFFFF;
                                                    SETW(d0, r);
                                                    fx = (s > d);
                                                }
                                                d2 = 0;                                              /* 00A3E4  moveq  #0,d2 */
                                                CALL(sub_0063A4, 0xA3EAU);                           /* 00A3E6  bsr  $63a4 */
L_00A3EA:
                                                /* 00A3EA  bra  $a3f4 */
                                                goto L_00A3F4;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        a4 = a4 + 0x14;                                      /* 00A3EC  lea  $14(a4),a4 */
                        /* 00A3F0  dbra  d6,$a2ec */
                        SETW(d6, d6 - 1);
                    } while ((d6 & 0xFFFF) != 0xFFFF);
                }
            }
        }
L_00A3F4:
        a6 = a6 + 0x10;                                      /* 00A3F4  lea  $10(a6),a6 */
        /* 00A3F8  dbra  d7,$a2ca */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00A3FC:
    RTS();                                               /* 00A3FC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA2CAU: goto L_00A2CA;
        case 0xA2D0U: goto L_00A2D0;
        case 0xA2DAU: goto L_00A2DA;
        case 0xA2ECU: goto L_00A2EC;
        case 0xA37EU: goto L_00A37E;
        case 0xA38CU: goto L_00A38C;
        case 0xA39CU: goto L_00A39C;
        case 0xA3ACU: goto L_00A3AC;
        case 0xA3BEU: goto L_00A3BE;
        case 0xA3CEU: goto L_00A3CE;
        case 0xA3DCU: goto L_00A3DC;
        case 0xA3EAU: goto L_00A3EA;
        case 0xA3F4U: goto L_00A3F4;
        case 0xA3FCU: goto L_00A3FC;
        }
    }
}

/* 00A3FE  game routine */
void sub_00A3FE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_18E6;                                         /* 00A3FE  lea  $18e6(a5),a6 */
    SETW(d7, 0x13);                                      /* 00A402  move.w  #$13,d7 */
    do {
        BUDGET_CHECK(0xA406U);
L_00A406:
        /* 00A406  tst.w  $4(a6) */
        /* 00A40A  beq  $a47c */
        if (rd16(a6 + 0x4) != 0) {
            /* 00A40E  tst.w  $c(a6) */
            /* 00A412  bmi  $a47c */
            if ((int16_t)rd16(a6 + 0xC) >= 0) {
                for (;;) {
                    BUDGET_CHECK(0xA414U);
L_00A414:
                    /* 00A414  tst.w  $12(a6) */
                    /* 00A418  beq  $a420 */
                    if (rd16(a6 + 0x12) != 0) {
                        /* 00A41A  subq.w  #1,$12(a6) */
                        {
                            uint32_t s = 1;
                            uint32_t d = rd16(a6 + 0x12);
                            uint32_t r = (d - s) & 0xFFFF;
                            wr16(a6 + 0x12, r);
                            fx = (s > d);
                        }
                        /* 00A41E  bra  $a47c */
                        goto L_00A47C;
                    }
                    a0 = 0xA486;                                         /* 00A420  lea  $a486(pc),a0 */
                    d0 = 0;                                              /* 00A424  moveq  #0,d0 */
                    SETW(d0, rd16(a6 + 0xE));                            /* 00A426  move.w  $e(a6),d0 */
                    d0 = (d0 & 0xFFFF) * 0x50;                           /* 00A42A  mulu.w  #$50,d0 */
                    a0 += d0;                                            /* 00A42E  adda.l  d0,a0 */
                    SETW(d6, rd16(a6 + 0x10));                           /* 00A430  move.w  $10(a6),d6 */
                    /* 00A434  addi.w  #$a,$10(a6) */
                    {
                        uint32_t s = 0xA;
                        uint32_t d = rd16(a6 + 0x10);
                        uint32_t r = (d + s) & 0xFFFF;
                        wr16(a6 + 0x10, r);
                        fx = (r < s);
                    }
                    /* 00A43A  cmpi.w  #$50,$10(a6) */
                    /* 00A440  bls  $a44a */
                    if (rd16(a6 + 0x10) > 0x50) {
                        wr16(a6 + 0x10, 0);                                  /* 00A442  clr.w  $10(a6) */
                        /* 00A446  bra  $a47c */
                        goto L_00A47C;
                    }
                    SETW(d0, rd16(a6));                                  /* 00A44A  move.w  (a6),d0 */
                    SETW(d1, rd16(a6 + 0x2));                            /* 00A44C  move.w  $2(a6),d1 */
                    SETW(d0, d0 + 0x20);                                 /* 00A450  addi.w  #$20,d0 */
                    SETW(d1, d1 + 0x64);                                 /* 00A454  addi.w  #$64,d1 */
                    SETW(d0, d0 + rd16(a0 + SX16(d6)));                  /* 00A458  add.w  $0(a0,d6.w),d0 */
                    /* 00A45C  add.w  $2(a0,d6.w),d1 */
                    {
                        uint32_t s = rd16(a0 + 0x2 + SX16(d6));
                        uint32_t d = (d1 & 0xFFFF);
                        uint32_t r = (d + s) & 0xFFFF;
                        SETW(d1, r);
                        fx = (r < s);
                    }
                    SETW(d3, rd16(a0 + 0x4 + SX16(d6)));                 /* 00A460  move.w  $4(a0,d6.w),d3 */
                    SETW(d4, rd16(a0 + 0x6 + SX16(d6)));                 /* 00A464  move.w  $6(a0,d6.w),d4 */
                    wr16(a6 + 0x12, rd16(a0 + 0x8 + SX16(d6)));          /* 00A468  move.w  $8(a0,d6.w),$12(a6) */
                    SETW(d5, 0xFFFF);                                    /* 00A46E  move.w  #$ffff,d5 */
                    SETW(d2, 0xC8);                                      /* 00A472  move.w  #$c8,d2 */
                    CALL(sub_006B74, 0xA47AU);                           /* 00A476  bsr  $6b74 */
L_00A47A:
                    /* 00A47A  bra  $a414 */
                }
            }
        }
L_00A47C:
        a6 = a6 + 0x14;                                      /* 00A47C  lea  $14(a6),a6 */
        /* 00A480  dbra  d7,$a406 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00A484:
    RTS();                                               /* 00A484  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA406U: goto L_00A406;
        case 0xA414U: goto L_00A414;
        case 0xA47AU: goto L_00A47A;
        case 0xA47CU: goto L_00A47C;
        case 0xA484U: goto L_00A484;
        }
    }
}

/* 00A486  game routine */
void sub_00A486(void)
{
    wr8(a2, rd8(a2) | 0xFE);                             /* 00A486  ori.b  #$fe,(a2) */
    SETB(d0, d0 | 0xFC);                                 /* 00A48A  ori.b  #$fc,d0 */
    /* 00A48E  ori.b  #$2,d2 */
    {
        uint32_t s = 2;
        uint32_t d = (d2 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d2, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xA492U);                                       /* 00A492  soft */
}

/* 00A526  game routine */
void sub_00A526(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_VEHICLES;                                     /* 00A526  lea  $1a76(a5),a6 */
    SETW(d7, 4);                                         /* 00A52A  move.w  #$4,d7 */
    do {
        BUDGET_CHECK(0xA52EU);
L_00A52E:
        /* 00A52E  tst.w  (a6) */
        /* 00A530  beq  $a53a */
        if (rd16(a6) != 0) {
L_00A532:
            SETW(d0, rd16(a6 + 0x6));                            /* 00A532  move.w  $6(a6),d0 */
            /* 00A536  add.w  d0,$4(a6) */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t d = rd16(a6 + 0x4);
                uint32_t r = (d + s) & 0xFFFF;
                wr16(a6 + 0x4, r);
                fx = (r < s);
            }
        }
L_00A53A:
        a6 = a6 + 0x2E;                                      /* 00A53A  lea  $2e(a6),a6 */
        /* 00A53E  dbra  d7,$a52e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00A542:
    RTS();                                               /* 00A542  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA52EU: goto L_00A52E;
        case 0xA532U: goto L_00A532;
        case 0xA53AU: goto L_00A53A;
        case 0xA542U: goto L_00A542;
        }
    }
}

/* 00A544  game routine */
void sub_00A544(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_02DA;                                         /* 00A544  lea  $2da(a5),a6 */
    /* 00A548  tst.w  (a6) */
    /* 00A54A  beq  $a552 */
    if (rd16(a6) != 0) {
L_00A54C:
        wr16(V_027A, 0);                                     /* 00A54C  clr.w  $27a(a5) */
        CALL(sub_00A562, 0xA552U);                           /* 00A550  bsr  $a562 */
    }
L_00A552:
    a6 = V_0376;                                         /* 00A552  lea  $376(a5),a6 */
    /* 00A556  tst.w  (a6) */
    /* 00A558  beq  $a5fa */
    if (rd16(a6) == 0) {
        { sub_00A5FA(); return; }
    }
    /* 00A55C  move.w  #$1,$27a(a5) */
    wr16(V_027A, 1);
    { sub_00A562(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA54CU: goto L_00A54C;
        case 0xA552U: goto L_00A552;
        }
    }
}

/* 00A562  game routine */
void sub_00A562(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00A562  tst.w  $2a(a6) */
    /* 00A566  bne  $a5fa */
    if (rd16(a6 + 0x2A) != 0) {
        { sub_00A5FA(); return; }
    }
L_00A56A:
    /* 00A56A  tst.w  $50(a6) */
    /* 00A56E  bne  $a5fa */
    if (rd16(a6 + 0x50) != 0) {
        { sub_00A5FA(); return; }
    }
L_00A572:
    a4 = V_ENEMIES;                                      /* 00A572  lea  $412(a5),a4 */
    SETW(d7, 0x18);                                      /* 00A576  move.w  #$18,d7 */
L_00A57A:
    /* 00A57A  tst.w  $4(a4) */
    /* 00A57E  beq  $a5f2 */
    if (rd16(a4 + 0x4) != 0) {
L_00A582:
        /* 00A582  tst.w  $26(a4) */
        /* 00A586  bmi  $a5f2 */
        if ((int16_t)rd16(a4 + 0x26) >= 0) {
L_00A588:
            SETW(d0, rd16(a6 + 0x2));                            /* 00A588  move.w  $2(a6),d0 */
            SETW(d1, rd16(a6 + 0x4));                            /* 00A58C  move.w  $4(a6),d1 */
            SETW(d2, rd16(a4));                                  /* 00A590  move.w  (a4),d2 */
            SETW(d3, rd16(a4 + 0x2));                            /* 00A592  move.w  $2(a4),d3 */
            SETW(d0, d0 + 0x24);                                 /* 00A596  addi.w  #$24,d0 */
            SETW(d1, d1 + 0x64);                                 /* 00A59A  addi.w  #$64,d1 */
            SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00A59E  sub.w  d3,d1 */
            SETW(d1, d1 + 0x16);                                 /* 00A5A0  addi.w  #$16,d1 */
            /* 00A5A4  cmp.w  #$22,d1 */
            /* 00A5A8  bhi  $a5f2 */
            if ((d1 & 0xFFFF) <= 0x22) {
L_00A5AA:
                SETW(d0, d0 - (d2 & 0xFFFF));                        /* 00A5AA  sub.w  d2,d0 */
                /* 00A5AC  addi.w  #$1a,d0 */
                {
                    uint32_t s = 0x1A;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d0, r);
                    fx = (r < s);
                }
                /* 00A5B0  cmp.w  #$24,d0 */
                /* 00A5B4  bcc  $a5f2 */
                if ((d0 & 0xFFFF) < 0x24) {
L_00A5B6:
                    /* 00A5B6  tst.w  $254(a5) */
                    /* 00A5BA  bne  $a5c6 */
                    if (rd16(V_0254) == 0) {
L_00A5BC:
                        /* 00A5BC  move.l  a4,-(a7) */
                        {
                            uint32_t v = a4;
                            a7 -= 4;
                            wr32(a7, v);
                        }
                        a4 = a6;                                             /* 00A5BE  movea.l  a6,a4 */
                        CALL(player_start_death, 0xA5C4U);                   /* 00A5C0  bsr  $aa42 */
L_00A5C4:
                        /* 00A5C4  movea.l  (a7)+,a4 */
                        a4 = rd32(a7);
                        a7 += 4;
                    }
                    a6 = a4;                                             /* 00A5C6  movea.l  a4,a6 */
                    wr16(a6 + 0x26, 0xFFFF);                             /* 00A5C8  move.w  #$ffff,$26(a6) */
                    wr16(a6 + 0xC, 0);                                   /* 00A5CE  clr.w  $c(a6) */
                    wr16(a6 + 0xE, 0);                                   /* 00A5D2  clr.w  $e(a6) */
                    SETW(d0, rd16(a6 + 0x8));                            /* 00A5D6  move.w  $8(a6),d0 */
                    /* 00A5DA  cmp.w  #$11,d0 */
                    /* 00A5DE  bcc  $a5ea */
                    if ((d0 & 0xFFFF) < 0x11) {
L_00A5E0:
                        d0 = 3;                                              /* 00A5E0  moveq  #3,d0 */
                        CALL(sub_00B1E4, 0xA5E6U);                           /* 00A5E2  bsr  $b1e4 */
L_00A5E6:
                        /* 00A5E6  bra  $a5fa */
                        { sub_00A5FA(); return; }
                    }
L_00A5EA:
                    d0 = 0xA;                                            /* 00A5EA  moveq  #10,d0 */
                    CALL(sub_00B1E4, 0xA5F0U);                           /* 00A5EC  bsr  $b1e4 */
L_00A5F0:
                    /* 00A5F0  bra  $a5fa */
                    { sub_00A5FA(); return; }
                }
            }
        }
    }
L_00A5F2:
    a4 = a4 + 0x40;                                      /* 00A5F2  lea  $40(a4),a4 */
    /* 00A5F6  dbra  d7,$a57a */
    SETW(d7, d7 - 1);
    if ((d7 & 0xFFFF) != 0xFFFF) { BUDGET_CHECK(0xA57AU); goto L_00A57A; }
    { sub_00A5FA(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA56AU: goto L_00A56A;
        case 0xA572U: goto L_00A572;
        case 0xA57AU: goto L_00A57A;
        case 0xA582U: goto L_00A582;
        case 0xA588U: goto L_00A588;
        case 0xA5AAU: goto L_00A5AA;
        case 0xA5B6U: goto L_00A5B6;
        case 0xA5BCU: goto L_00A5BC;
        case 0xA5C4U: goto L_00A5C4;
        case 0xA5E0U: goto L_00A5E0;
        case 0xA5E6U: goto L_00A5E6;
        case 0xA5EAU: goto L_00A5EA;
        case 0xA5F0U: goto L_00A5F0;
        case 0xA5F2U: goto L_00A5F2;
        }
    }
}

/* 00A5FA  game routine */
void sub_00A5FA(void)
{
    RTS();                                               /* 00A5FA  rts */
}

/* 00A5FC  game routine */
void sub_00A5FC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1472;                                         /* 00A5FC  lea  $1472(a5),a4 */
    SETW(d7, 0x10);                                      /* 00A600  move.w  #$10,d7 */
    do {
        BUDGET_CHECK(0xA604U);
L_00A604:
        /* 00A604  tst.w  $4(a4) */
        /* 00A608  beq  $a854 */
        if (rd16(a4 + 0x4) != 0) {
L_00A60C:
            /* 00A60C  cmpi.w  #$9,$6(a4) */
            /* 00A612  bcc  $a854 */
            if (rd16(a4 + 0x6) < 9) {
L_00A616:
                a6 = V_ENEMIES;                                      /* 00A616  lea  $412(a5),a6 */
                SETW(d6, 0x2F);                                      /* 00A61A  move.w  #$2f,d6 */
                do {
                    BUDGET_CHECK(0xA61EU);
L_00A61E:
                    /* 00A61E  tst.w  $4(a6) */
                    /* 00A622  beq  $a6a2 */
                    if (rd16(a6 + 0x4) != 0) {
L_00A626:
                        /* 00A626  tst.w  $26(a6) */
                        /* 00A62A  bne  $a6a2 */
                        if (rd16(a6 + 0x26) == 0) {
L_00A62E:
                            SETW(d0, rd16(a4));                                  /* 00A62E  move.w  (a4),d0 */
                            SETW(d1, rd16(a4 + 0x2));                            /* 00A630  move.w  $2(a4),d1 */
                            /* 00A634  tst.w  $a(a4) */
                            /* 00A638  bmi  $a792 */
                            if ((int16_t)rd16(a4 + 0xA) < 0) {
                                goto L_00A792;
                            }
                            SETW(d2, rd16(a6));                                  /* 00A63C  move.w  (a6),d2 */
                            SETW(d3, rd16(a6 + 0x2));                            /* 00A63E  move.w  $2(a6),d3 */
                            SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00A642  sub.w  d3,d1 */
                            /* 00A644  addi.w  #$14,d1 */
                            {
                                uint32_t s = 0x14;
                                uint32_t d = (d1 & 0xFFFF);
                                uint32_t r = (d + s) & 0xFFFF;
                                SETW(d1, r);
                                fx = (r < s);
                            }
                            /* 00A648  cmp.w  #$2a,d1 */
                            /* 00A64C  bhi  $a6a2 */
                            if ((d1 & 0xFFFF) <= 0x2A) {
                                SETW(d0, d0 - (d2 & 0xFFFF));                        /* 00A64E  sub.w  d2,d0 */
                                /* 00A650  addi.w  #$1e,d0 */
                                {
                                    uint32_t s = 0x1E;
                                    uint32_t d = (d0 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d0, r);
                                    fx = (r < s);
                                }
                                /* 00A654  cmp.w  #$2c,d0 */
                                /* 00A658  bcc  $a6a2 */
                                if ((d0 & 0xFFFF) < 0x2C) {
                                    d0 = 4;                                              /* 00A65A  moveq  #4,d0 */
                                    CALL(sub_00E756, 0xA660U);                           /* 00A65C  bsr  $e756 */
L_00A660:
                                    /* 00A660  cmpi.w  #$1,$a(a4) */
                                    /* 00A666  beq  $a672 */
                                    if (rd16(a4 + 0xA) != 1) {
                                        /* 00A668  addi.l  #$c8,$2e2(a5) */
                                        {
                                            uint32_t s = 0xC8;
                                            uint32_t d = rd32(V_02E2);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_02E2, r);
                                            fx = (r < s);
                                        }
                                        /* 00A670  bra  $a67a */
                                    } else {
                                        /* 00A672  addi.l  #$c8,$37e(a5) */
                                        {
                                            uint32_t s = 0xC8;
                                            uint32_t d = rd32(V_037E);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_037E, r);
                                            fx = (r < s);
                                        }
                                    }
                                    wr16(a6 + 0x26, 0xFFFF);                             /* 00A67A  move.w  #$ffff,$26(a6) */
                                    wr16(a6 + 0xC, 0);                                   /* 00A680  clr.w  $c(a6) */
                                    wr16(a6 + 0xE, 0);                                   /* 00A684  clr.w  $e(a6) */
                                    SETW(d0, rd16(a6 + 0x8));                            /* 00A688  move.w  $8(a6),d0 */
                                    /* 00A68C  cmp.w  #$11,d0 */
                                    /* 00A690  bcc  $a69c */
                                    if ((d0 & 0xFFFF) < 0x11) {
                                        d0 = 3;                                              /* 00A692  moveq  #3,d0 */
                                        CALL(sub_00B1E4, 0xA698U);                           /* 00A694  bsr  $b1e4 */
L_00A698:
                                        /* 00A698  bra  $a6a2 */
                                    } else {
                                        d0 = 0xA;                                            /* 00A69C  moveq  #10,d0 */
                                        CALL(sub_00B1E4, 0xA6A2U);                           /* 00A69E  bsr  $b1e4 */
                                    }
                                }
                            }
                        }
                    }
L_00A6A2:
                    a6 = a6 + 0x40;                                      /* 00A6A2  lea  $40(a6),a6 */
                    /* 00A6A6  dbra  d6,$a61e */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
                a6 = V_1C84;                                         /* 00A6AA  lea  $1c84(a5),a6 */
                SETW(d6, 5);                                         /* 00A6AE  move.w  #$5,d6 */
                do {
                    BUDGET_CHECK(0xA6B2U);
L_00A6B2:
                    /* 00A6B2  tst.w  (a6) */
                    /* 00A6B4  beq  $a70a */
                    if (rd16(a6) != 0) {
                        /* 00A6B8  tst.w  $e(a6) */
                        /* 00A6BC  bmi  $a70a */
                        if ((int16_t)rd16(a6 + 0xE) >= 0) {
                            SETW(d0, rd16(a4));                                  /* 00A6BE  move.w  (a4),d0 */
                            SETW(d1, rd16(a4 + 0x2));                            /* 00A6C0  move.w  $2(a4),d1 */
                            SETW(d2, rd16(a6 + 0x4));                            /* 00A6C4  move.w  $4(a6),d2 */
                            SETW(d3, rd16(a6 + 0x6));                            /* 00A6C8  move.w  $6(a6),d3 */
                            SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00A6CC  sub.w  d3,d1 */
                            /* 00A6CE  addi.w  #$16,d1 */
                            {
                                uint32_t s = 0x16;
                                uint32_t d = (d1 & 0xFFFF);
                                uint32_t r = (d + s) & 0xFFFF;
                                SETW(d1, r);
                                fx = (r < s);
                            }
                            /* 00A6D2  cmp.w  #$26,d1 */
                            /* 00A6D6  bhi  $a70a */
                            if ((d1 & 0xFFFF) <= 0x26) {
                                SETW(d0, d0 - (d2 & 0xFFFF));                        /* 00A6D8  sub.w  d2,d0 */
                                /* 00A6DA  addi.w  #$1e,d0 */
                                {
                                    uint32_t s = 0x1E;
                                    uint32_t d = (d0 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d0, r);
                                    fx = (r < s);
                                }
                                /* 00A6DE  cmp.w  #$2c,d0 */
                                /* 00A6E2  bcc  $a70a */
                                if ((d0 & 0xFFFF) < 0x2C) {
                                    /* 00A6E4  cmpi.w  #$1,$a(a4) */
                                    /* 00A6EA  beq  $a6f6 */
                                    if (rd16(a4 + 0xA) != 1) {
                                        /* 00A6EC  addi.l  #$c8,$2e2(a5) */
                                        {
                                            uint32_t s = 0xC8;
                                            uint32_t d = rd32(V_02E2);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_02E2, r);
                                            fx = (r < s);
                                        }
                                        /* 00A6F4  bra  $a6fe */
                                    } else {
                                        /* 00A6F6  addi.l  #$c8,$37e(a5) */
                                        {
                                            uint32_t s = 0xC8;
                                            uint32_t d = rd32(V_037E);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_037E, r);
                                            fx = (r < s);
                                        }
                                    }
                                    wr16(a6 + 0xA, 8);                                   /* 00A6FE  move.w  #$8,$a(a6) */
                                    wr16(a6 + 0xE, 0xFFFF);                              /* 00A704  move.w  #$ffff,$e(a6) */
                                }
                            }
                        }
                    }
                    a6 = a6 + 0x12;                                      /* 00A70A  lea  $12(a6),a6 */
                    /* 00A70E  dbra  d6,$a6b2 */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
                a6 = V_1B5C;                                         /* 00A712  lea  $1b5c(a5),a6 */
                SETW(d6, 0xF);                                       /* 00A716  move.w  #$f,d6 */
                do {
                    BUDGET_CHECK(0xA71AU);
L_00A71A:
                    /* 00A71A  tst.w  (a6) */
                    /* 00A71C  beq  $a78a */
                    if (rd16(a6) != 0) {
                        SETW(d0, rd16(a4));                                  /* 00A720  move.w  (a4),d0 */
                        SETW(d1, rd16(a4 + 0x2));                            /* 00A722  move.w  $2(a4),d1 */
                        SETW(d2, rd16(a6 + 0x2));                            /* 00A726  move.w  $2(a6),d2 */
                        SETW(d3, rd16(a6 + 0x4));                            /* 00A72A  move.w  $4(a6),d3 */
                        SETW(d3, d3 + 0x64);                                 /* 00A72E  addi.w  #$64,d3 */
                        SETW(d2, d2 + 0x24);                                 /* 00A732  addi.w  #$24,d2 */
                        SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00A736  sub.w  d3,d1 */
                        /* 00A738  addi.w  #$16,d1 */
                        {
                            uint32_t s = 0x16;
                            uint32_t d = (d1 & 0xFFFF);
                            uint32_t r = (d + s) & 0xFFFF;
                            SETW(d1, r);
                            fx = (r < s);
                        }
                        /* 00A73C  cmp.w  #$26,d1 */
                        /* 00A740  bhi  $a78a */
                        if ((d1 & 0xFFFF) <= 0x26) {
                            SETW(d0, d0 - (d2 & 0xFFFF));                        /* 00A742  sub.w  d2,d0 */
                            /* 00A744  addi.w  #$1e,d0 */
                            {
                                uint32_t s = 0x1E;
                                uint32_t d = (d0 & 0xFFFF);
                                uint32_t r = (d + s) & 0xFFFF;
                                SETW(d0, r);
                                fx = (r < s);
                            }
                            /* 00A748  cmp.w  #$2c,d0 */
                            /* 00A74C  bcc  $a78a */
                            if ((d0 & 0xFFFF) < 0x2C) {
                                /* 00A74E  cmpi.w  #$1,$a(a4) */
                                /* 00A754  beq  $a760 */
                                if (rd16(a4 + 0xA) != 1) {
                                    wr32(V_02E2, rd32(V_02E2) + 0xC8);                   /* 00A756  addi.l  #$c8,$2e2(a5) */
                                    /* 00A75E  bra  $a768 */
                                } else {
                                    wr32(V_037E, rd32(V_037E) + 0xC8);                   /* 00A760  addi.l  #$c8,$37e(a5) */
                                }
                                wr16(a6, 0);                                         /* 00A768  clr.w  (a6) */
                                /* 00A76A  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
                                SETW(d0, rd16(a6 + 0x2));                            /* 00A76E  move.w  $2(a6),d0 */
                                SETW(d0, d0 + 0x2C);                                 /* 00A772  addi.w  #$2c,d0 */
                                SETW(d1, rd16(a6 + 0x4));                            /* 00A776  move.w  $4(a6),d1 */
                                /* 00A77A  addi.w  #$60,d1 */
                                {
                                    uint32_t s = 0x60;
                                    uint32_t d = (d1 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d1, r);
                                    fx = (r < s);
                                }
                                SETW(d2, rd16(a4 + 0xA));                            /* 00A77E  move.w  $a(a4),d2 */
                                CALL(sub_006CA8, 0xA786U);                           /* 00A782  bsr  $6ca8 */
L_00A786:
                                /* 00A786  movem.l  (a7)+,d0-d7/a0-a6 */
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
                            }
                        }
                    }
                    a6 = a6 + 0xE;                                       /* 00A78A  lea  $e(a6),a6 */
                    /* 00A78E  dbra  d6,$a71a */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
L_00A792:
                /* 00A792  tst.w  $254(a5) */
                /* 00A796  bne  $a854 */
                if (rd16(V_0254) == 0) {
L_00A79A:
                    /* 00A79A  cmpi.w  #$4,$6(a4) */
                    /* 00A7A0  bcc  $a7f6 */
                    if (rd16(a4 + 0x6) < 4) {
L_00A7A4:
                        SETW(d0, rd16(a4));                                  /* 00A7A4  move.w  (a4),d0 */
                        SETW(d1, rd16(a4 + 0x2));                            /* 00A7A6  move.w  $2(a4),d1 */
                        wr16(V_027A, 0);                                     /* 00A7AA  clr.w  $27a(a5) */
                        /* 00A7AE  tst.w  $2da.l */
                        /* 00A7B4  beq  $a7f6 */
                        if (rd16(V_02DA) != 0) {
L_00A7B6:
                            /* 00A7B6  tst.w  $32a.l */
                            /* 00A7BC  bne  $a7f6 */
                            if (rd16(V_032A) == 0) {
L_00A7BE:
                                /* 00A7BE  tst.w  $304.l */
                                /* 00A7C4  bne  $a7f6 */
                                if (rd16(V_0304) == 0) {
L_00A7C6:
                                    SETW(d2, rd16(V_02DC));                              /* 00A7C6  move.w  $2dc(a5),d2 */
                                    SETW(d3, rd16(V_02DE));                              /* 00A7CA  move.w  $2de(a5),d3 */
                                    SETW(d2, d2 + 0x24);                                 /* 00A7CE  addi.w  #$24,d2 */
                                    SETW(d3, d3 + 0x64);                                 /* 00A7D2  addi.w  #$64,d3 */
                                    SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00A7D6  sub.w  d3,d1 */
                                    /* 00A7D8  addi.w  #$a,d1 */
                                    {
                                        uint32_t s = 0xA;
                                        uint32_t d = (d1 & 0xFFFF);
                                        uint32_t r = (d + s) & 0xFFFF;
                                        SETW(d1, r);
                                        fx = (r < s);
                                    }
                                    /* 00A7DC  cmp.w  #$1e,d1 */
                                    /* 00A7E0  bhi  $a7f6 */
                                    if ((d1 & 0xFFFF) <= 0x1E) {
                                        /* 00A7E2  sub.w  d2,d0 */
                                        {
                                            uint32_t s = (d2 & 0xFFFF);
                                            uint32_t d = (d0 & 0xFFFF);
                                            uint32_t r = (d - s) & 0xFFFF;
                                            SETW(d0, r);
                                            fx = (s > d);
                                        }
                                        /* 00A7E4  cmp.w  #$20,d0 */
                                        /* 00A7E8  bcc  $a7f6 */
                                        if ((d0 & 0xFFFF) < 0x20) {
                                            /* 00A7EA  move.l  a4,-(a7) */
                                            {
                                                uint32_t v = a4;
                                                a7 -= 4;
                                                wr32(a7, v);
                                            }
                                            a4 = V_02DA;                                         /* 00A7EC  lea  $2da(a5),a4 */
                                            CALL(player_start_death, 0xA7F4U);                   /* 00A7F0  bsr  $aa42 */
L_00A7F4:
                                            /* 00A7F4  movea.l  (a7)+,a4 */
                                            a4 = rd32(a7);
                                            a7 += 4;
                                        }
                                    }
                                }
                            }
                        }
                    }
L_00A7F6:
                    /* 00A7F6  cmpi.w  #$4,$6(a4) */
                    /* 00A7FC  bcc  $a854 */
                    if (rd16(a4 + 0x6) < 4) {
L_00A800:
                        SETW(d0, rd16(a4));                                  /* 00A800  move.w  (a4),d0 */
                        SETW(d1, rd16(a4 + 0x2));                            /* 00A802  move.w  $2(a4),d1 */
                        wr16(V_027A, 1);                                     /* 00A806  move.w  #$1,$27a(a5) */
                        /* 00A80C  tst.w  $376.l */
                        /* 00A812  beq  $a854 */
                        if (rd16(V_0376) != 0) {
                            /* 00A814  tst.w  $3c6.l */
                            /* 00A81A  bne  $a854 */
                            if (rd16(V_03C6) == 0) {
                                /* 00A81C  tst.w  $3a0.l */
                                /* 00A822  bne  $a854 */
                                if (rd16(V_03A0) == 0) {
                                    SETW(d2, rd16(V_0378));                              /* 00A824  move.w  $378(a5),d2 */
                                    SETW(d3, rd16(V_037A));                              /* 00A828  move.w  $37a(a5),d3 */
                                    SETW(d2, d2 + 0x24);                                 /* 00A82C  addi.w  #$24,d2 */
                                    SETW(d3, d3 + 0x64);                                 /* 00A830  addi.w  #$64,d3 */
                                    SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00A834  sub.w  d3,d1 */
                                    /* 00A836  addi.w  #$a,d1 */
                                    {
                                        uint32_t s = 0xA;
                                        uint32_t d = (d1 & 0xFFFF);
                                        uint32_t r = (d + s) & 0xFFFF;
                                        SETW(d1, r);
                                        fx = (r < s);
                                    }
                                    /* 00A83A  cmp.w  #$1e,d1 */
                                    /* 00A83E  bhi  $a854 */
                                    if ((d1 & 0xFFFF) <= 0x1E) {
                                        /* 00A840  sub.w  d2,d0 */
                                        {
                                            uint32_t s = (d2 & 0xFFFF);
                                            uint32_t d = (d0 & 0xFFFF);
                                            uint32_t r = (d - s) & 0xFFFF;
                                            SETW(d0, r);
                                            fx = (s > d);
                                        }
                                        /* 00A842  cmp.w  #$20,d0 */
                                        /* 00A846  bcc  $a854 */
                                        if ((d0 & 0xFFFF) < 0x20) {
                                            /* 00A848  move.l  a4,-(a7) */
                                            {
                                                uint32_t v = a4;
                                                a7 -= 4;
                                                wr32(a7, v);
                                            }
                                            a4 = V_0376;                                         /* 00A84A  lea  $376(a5),a4 */
                                            CALL(player_start_death, 0xA852U);                   /* 00A84E  bsr  $aa42 */
L_00A852:
                                            /* 00A852  movea.l  (a7)+,a4 */
                                            a4 = rd32(a7);
                                            a7 += 4;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
L_00A854:
        a4 = a4 + 0xE;                                       /* 00A854  lea  $e(a4),a4 */
        /* 00A858  dbra  d7,$a604 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00A85C:
    RTS();                                               /* 00A85C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA604U: goto L_00A604;
        case 0xA60CU: goto L_00A60C;
        case 0xA616U: goto L_00A616;
        case 0xA61EU: goto L_00A61E;
        case 0xA626U: goto L_00A626;
        case 0xA62EU: goto L_00A62E;
        case 0xA660U: goto L_00A660;
        case 0xA698U: goto L_00A698;
        case 0xA6A2U: goto L_00A6A2;
        case 0xA6B2U: goto L_00A6B2;
        case 0xA71AU: goto L_00A71A;
        case 0xA786U: goto L_00A786;
        case 0xA792U: goto L_00A792;
        case 0xA79AU: goto L_00A79A;
        case 0xA7A4U: goto L_00A7A4;
        case 0xA7B6U: goto L_00A7B6;
        case 0xA7BEU: goto L_00A7BE;
        case 0xA7C6U: goto L_00A7C6;
        case 0xA7F4U: goto L_00A7F4;
        case 0xA7F6U: goto L_00A7F6;
        case 0xA800U: goto L_00A800;
        case 0xA852U: goto L_00A852;
        case 0xA854U: goto L_00A854;
        case 0xA85CU: goto L_00A85C;
        }
    }
}

/* 00A85E  game routine */
void sub_00A85E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00A85E  movem.l  a3/d7,-(a7) */
    PUSH32(a3);
    PUSH32(d7);
    SETW(d7, 0x13);                                      /* 00A862  move.w  #$13,d7 */
    a3 = V_1614;                                         /* 00A866  lea  $1614(a5),a3 */
    do {
        BUDGET_CHECK(0xA86AU);
L_00A86A:
        /* 00A86A  tst.w  (a3) */
        /* 00A86C  bne  $a88e */
        if (rd16(a3) == 0) {
L_00A86E:
            wr16(a3 + 0x2, (d0 & 0xFFFF));                       /* 00A86E  move.w  d0,$2(a3) */
            wr16(a3 + 0x4, (d1 & 0xFFFF));                       /* 00A872  move.w  d1,$4(a3) */
            wr16(a3 + 0x6, (d2 & 0xFFFF));                       /* 00A876  move.w  d2,$6(a3) */
            wr16(a3 + 0xA, 0);                                   /* 00A87A  clr.w  $a(a3) */
            wr16(a3 + 0xC, 0);                                   /* 00A87E  clr.w  $c(a3) */
            wr16(a3 + 0x8, 3);                                   /* 00A882  move.w  #$3,$8(a3) */
            wr16(a3, 0xFFFF);                                    /* 00A888  move.w  #$ffff,(a3) */
            /* 00A88C  bra  $a896 */
            goto L_00A896;
        }
        a3 = a3 + 0x10;                                      /* 00A88E  lea  $10(a3),a3 */
        /* 00A892  dbra  d7,$a86a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00A896:
    /* 00A896  movem.l  (a7)+,d7/a3 */
    d7 = POP32();
    a3 = POP32();
    RTS();                                               /* 00A89A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA86AU: goto L_00A86A;
        case 0xA86EU: goto L_00A86E;
        case 0xA896U: goto L_00A896;
        }
    }
}

/* 00A89C  game routine */
void sub_00A89C(void)
{
    /* 00A89C  ori.b  #$5a,$0(a0,d0.w) */
    {
        uint32_t s = 0x5A;
        uint32_t d = rd8(a0 + SX16(d0));
        uint32_t r = (d | s) & 0xFF;
        wr8(a0 + SX16(d0), r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xA8A2U);                                       /* 00A8A2  soft */
}

/* 00A914  game routine */
void sub_00A914(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_007626, 0xA918U);                           /* 00A914  bsr  $7626 */
L_00A918:
    wr32(V_DRAW_BUFFER, 0x20000);                        /* 00A918  move.l  #$20000,$28e(a5) */
    d0 = 7;                                              /* 00A920  moveq  #7,d0 */
    CALL(sub_0074F4, 0xA926U);                           /* 00A922  bsr  $74f4 */
L_00A926:
    wr16(V_0266, 0x32A0);                                /* 00A926  move.w  #$32a0,$266(a5) */
    wr16(V_0268, 6);                                     /* 00A92C  move.w  #$6,$268(a5) */
    wr16(V_026A, 6);                                     /* 00A932  move.w  #$6,$26a(a5) */
    wr16(V_026C, 1);                                     /* 00A938  move.w  #$1,$26c(a5) */
    a0 = 0xA9FA;                                         /* 00A93E  lea  $a9fa(pc),a0 */
    CALL(sub_00A9D8, 0xA946U);                           /* 00A942  bsr  $a9d8 */
L_00A946:
    d7 = 0x72BF0;                                        /* 00A946  move.l  #$72bf0,d7 */
    CALL(sub_0075A4, 0xA950U);                           /* 00A94C  bsr  $75a4 */
L_00A950:
    wr16(V_0266, 0x4420);                                /* 00A950  move.w  #$4420,$266(a5) */
    wr16(V_0268, 6);                                     /* 00A956  move.w  #$6,$268(a5) */
    wr16(V_026A, 6);                                     /* 00A95C  move.w  #$6,$26a(a5) */
    wr16(V_026C, 6);                                     /* 00A962  move.w  #$6,$26c(a5) */
    a0 = 0xAA17;                                         /* 00A968  lea  $aa17(pc),a0 */
    CALL(sub_00A9D8, 0xA970U);                           /* 00A96C  bsr  $a9d8 */
L_00A970:
    d7 = 0xE7EF0;                                        /* 00A970  move.l  #$e7ef0,d7 */
    CALL(sub_0075A4, 0xA97AU);                           /* 00A976  bsr  $75a4 */
L_00A97A:
    while (hold_fail_screen()) { g_cpu.pc = 0xA97AU; yield_to_host(RC_STOP); }      /* (added by hand) the mission failed screen waits for its music to end, or for a button */
    RTS();                                               /* 00A97A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA918U: goto L_00A918;
        case 0xA926U: goto L_00A926;
        case 0xA946U: goto L_00A946;
        case 0xA950U: goto L_00A950;
        case 0xA970U: goto L_00A970;
        case 0xA97AU: goto L_00A97A;
        }
    }
}

/* 00A9D8  game routine */
void sub_00A9D8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    for (;;) {
        BUDGET_CHECK(0xA9D8U);
        d0 = 0;                                              /* 00A9D8  moveq  #0,d0 */
        /* 00A9DA  move.b  (a0)+,d0 */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 00A9DC  beq  $a9f8 */
        if (COND(7)) {
            goto L_00A9F8;
        }
L_00A9DE:
        /* 00A9DE  move.l  a0,-(a7) */
        {
            uint32_t v = a0;
            a7 -= 4;
            wr32(a7, v);
        }
        CALL(sub_00858E, 0xA9E4U);                           /* 00A9E0  bsr  $858e */
L_00A9E4:
        CALL(wait_vbl, 0xA9E8U);                             /* 00A9E4  bsr  $79dc */
L_00A9E8:
        CALL(wait_vbl, 0xA9ECU);                             /* 00A9E8  bsr  $79dc */
L_00A9EC:
        CALL(wait_vbl, 0xA9F0U);                             /* 00A9EC  bsr  $79dc */
L_00A9F0:
        CALL(wait_vbl, 0xA9F4U);                             /* 00A9F0  bsr  $79dc */
L_00A9F4:
        /* 00A9F4  movea.l  (a7)+,a0 */
        a0 = rd32(a7);
        a7 += 4;
        /* 00A9F6  bra  $a9d8 */
    }
L_00A9F8:
    RTS();                                               /* 00A9F8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xA9DEU: goto L_00A9DE;
        case 0xA9E4U: goto L_00A9E4;
        case 0xA9E8U: goto L_00A9E8;
        case 0xA9ECU: goto L_00A9EC;
        case 0xA9F0U: goto L_00A9F0;
        case 0xA9F4U: goto L_00A9F4;
        case 0xA9F8U: goto L_00A9F8;
        }
    }
}

/* 00A9FA  game routine */
void sub_00A9FA(void)
{
    a7 -= 4;                                             /* 00A9FA  subq.w  #4,a7 */
    /* 00A9FC  subq.b  #2,-(a0) */
    {
        uint32_t s = 2;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0, r);
    }
    d1 = (d1 << 16) | (d1 >> 16);                        /* 00A9FE  swap  d1 */
    /* 00AA00  addq.w  #3,d5 */
    {
        uint32_t s = 3;
        uint32_t d = (d5 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d5, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    /* 00AA02  movea.l  d6,a0 */
    a0 = d6;
    MISS(0xAA04U);
}

/* 00AA2E  game routine */
void sub_00AA2E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = rd32(a4 + 0x5C);                                /* 00AA2E  movea.l  $5c(a4),a0 */
    d0 = 0;                                              /* 00AA32  moveq  #0,d0 */
    /* 00AA34  move.b  (a0)+,d0 */
    {
        uint32_t v = rd8(a0);
        a0 += 1;
        SETB(d0, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 00AA36  bmi  $aa84 */
    if (COND(10)) {
L_00AA38:
        wr16(a4 + 0x6, (d0 & 0xFFFF));                       /* 00AA38  move.w  d0,$6(a4) */
        wr32(a4 + 0x5C, a0);                                 /* 00AA3C  move.l  a0,$5c(a4) */
        RTS();                                               /* 00AA40  rts */
    }
L_00AA84:
    wr16(a4, rd16(a4) - 1);                              /* 00AA84  subq.w  #1,(a4) */
    wr16(a4 + 0x2A, 0x50);                               /* 00AA86  move.w  #$50,$2a(a4) */
    wr16(a4 + 0x50, 0);                                  /* 00AA8C  clr.w  $50(a4) */
    /* 00AA90  tst.w  $27a(a5) */
    /* 00AA94  beq  $aa9e */
    if (rd16(V_027A) != 0) {
        wr16(a4 + 0x6, 0);                                   /* 00AA96  move.w  #$0,$6(a4) */
        /* 00AA9C  bra  $aaa4 */
    } else {
L_00AA9E:
        wr16(a4 + 0x6, 0x28);                                /* 00AA9E  move.w  #$28,$6(a4) */
    }
    wr16(a4 + 0x24, 0x14);                               /* 00AAA4  move.w  #$14,$24(a4) */
    wr8(a4 + 0x36, 0);                                   /* 00AAAA  clr.b  $36(a4) */
    RTS();                                               /* 00AAAE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAA38U: goto L_00AA38;
        case 0xAA84U: goto L_00AA84;
        case 0xAA9EU: goto L_00AA9E;
        }
    }
}

/* 00AA42  Starts a player's death (immunity patches its first instruction with RTS) */
void player_start_death(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00AA42  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
    if (g_cpu.patch[0]) {
        RTS();
    }
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
    d7 = 0;                                              /* 00AA46  moveq  #0,d7 */
    SETB(d7, rd8(a4 + 0x36));                            /* 00AA48  move.b  $36(a4),d7 */
    a1 = 0xAAE8;                                         /* 00AA4C  lea  $aae8(pc),a1 */
    /* 00AA50  move.b  $0(a1,d7.w),d7 */
    /* 00AA54  bpl  $aa58 */
    SETB(d7, rd8(a1 + SX16(d7)));
    if ((int8_t)(d7 & 0xFF) < 0) {
        d7 = 0;                                              /* 00AA56  moveq  #0,d7 */
    }
L_00AA58:
    wr16(a4 + 0x50, 0xFFFF);                             /* 00AA58  move.w  #$ffff,$50(a4) */
    wr16(V_0196, rd16(V_027A));                          /* 00AA5E  move.w  $27a(a5),$196(a5) */
    a0 = 0xAAB0;                                         /* 00AA64  lea  $aab0(pc),a0 */
    /* 00AA68  tst.w  $27a(a5) */
    /* 00AA6C  beq  $aa72 */
    if (rd16(V_027A) != 0) {
        a0 = 0xAACC;                                         /* 00AA6E  lea  $aacc(pc),a0 */
    }
L_00AA72:
    a0 = a0 + SX16(d7);                                  /* 00AA72  lea  $0(a0,d7.w),a0 */
    wr32(a4 + 0x5C, a0);                                 /* 00AA76  move.l  a0,$5c(a4) */
    wr16(a4 + 0x24, 0);                                  /* 00AA7A  clr.w  $24(a4) */
    /* 00AA7E  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 00AA82  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAA58U: goto L_00AA58;
        case 0xAA72U: goto L_00AA72;
        }
    }
}

/* 00AAB0  game routine */
void sub_00AAB0(void)
{
    a4 = d4;                                             /* 00AAB0  movea.l  d4,a4 */
    SETW(d2, rd16(0x2C3C));                              /* 00AAB2  move.w  $2c3c.w,d2 */
    a0 = SX16((d0 & 0xFFFF));                            /* 00AAB6  movea.w  d0,a0 */
    a4 = d4;                                             /* 00AAB8  movea.l  d4,a4 */
    SETW(d2, rd16(0x2C50));                              /* 00AABA  move.w  $2c50.w,d2 */
    wr16(a1, rd16(a1) + 8);                              /* 00AABE  addq.w  #8,(a1) */
    wr16(a2, rd16(a2) - 8);                              /* 00AAC0  subq.w  #8,(a2) */
    wr16(a3, rd16(a3) + 1);                              /* 00AAC2  addq.w  #1,(a3) */
    wr16(a4, rd16(a4) - 1);                              /* 00AAC4  subq.w  #1,(a4) */
    wr16(a5, rd16(a5) + 2);                              /* 00AAC6  addq.w  #2,(a5) */
    /* 00AAC8  addq.w  #3,(a7) */
    {
        uint32_t s = 3;
        uint32_t d = rd16(a7);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(a7, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    SOFT(0xAACAU);                                       /* 00AACA  soft */
}

/* 00AACC  game routine */
void sub_00AACC(void)
{
    /* 00AACC  ori.b  #$10,(a4)+ */
    {
        uint32_t s = 0x10;
        uint32_t ea = a4;
        a4 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    wr8(a4, rd8(a4) - 0x18);                             /* 00AAD0  subi.b  #$18,(a4) */
    /* 00AAD4  ori.b  #$10,(a4)+ */
    {
        uint32_t s = 0x10;
        uint32_t ea = a4;
        a4 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    /* 00AAD8  subi.w  #$5859,(a0)+ */
    {
        uint32_t s = 0x5859;
        uint32_t ea = a0;
        a0 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(ea, r);
    }
    /* 00AADC  subq.w  #4,(a2)+ */
    {
        uint32_t s = 4;
        uint32_t ea = a2;
        a2 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(ea, r);
    }
    /* 00AADE  addq.w  #5,(a3)+ */
    {
        uint32_t s = 5;
        uint32_t ea = a3;
        a3 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(ea, r);
    }
    wr16(a4, rd16(a4) - 5);                              /* 00AAE0  subq.w  #5,(a4) */
    wr16(a5, rd16(a5) + 2);                              /* 00AAE2  addq.w  #2,(a5) */
    /* 00AAE4  subq.w  #2,(a6) */
    {
        uint32_t s = 2;
        uint32_t d = rd16(a6);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(a6, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    MISS(0xAAE6U);
}

/* 00AAE8  game routine */
void sub_00AAE8(void)
{
    SOFT(0xAAE8U);                                       /* 00AAE8  soft */
}

/* 00AAF8  game routine */
void sub_00AAF8(void)
{
    a0 = 0xAB10;                                         /* 00AAF8  lea  $ab10(pc),a0 */
    SETW(d0, rd16(a4 + 0xC));                            /* 00AAFC  move.w  $c(a4),d0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 2, 16));      /* 00AB00  lsl.w  #2,d0 */
    a0 = a0 + SX16(d0);                                  /* 00AB02  lea  $0(a0,d0.w),a0 */
    SETW(d1, rd16(a4 + 0x60));                           /* 00AB06  move.w  $60(a4),d1 */
    SETB(d0, rd8(a0 + SX16(d1)));                        /* 00AB0A  move.b  $0(a0,d1.w),d0 */
    RTS();                                               /* 00AB0E  rts */
}

/* 00AB10  game routine */
void sub_00AB10(void)
{
    /* 00AB10  addq.b  #1,d0 */
    {
        uint32_t s = 1;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d + s) & 0xFF;
        SETB(d0, r);
        fx = (r < s);
    }
    SETB(d0, d0 | 0);                                    /* 00AB12  ori.b  #$0,d0 */
    SETB(d0, d0 | 0x52);                                 /* 00AB16  ori.b  #$52,d0 */
    SETB(d0, d0 | 0x52);                                 /* 00AB1A  ori.b  #$52,d0 */
    SETB(d0, d0 | 0x52);                                 /* 00AB1E  ori.b  #$52,d0 */
    SETB(d0, d0 | 0x52);                                 /* 00AB22  ori.b  #$52,d0 */
    SETB(d0, d0 | 0x52);                                 /* 00AB26  ori.b  #$52,d0 */
    SETB(d0, d0 | 0x52);                                 /* 00AB2A  ori.b  #$52,d0 */
    /* 00AB2E  ori.b  #$52,d0 */
    {
        uint32_t s = 0x52;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xAB32U);
}

/* 00AB5C  game routine */
void sub_00AB5C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0xABBE;                                         /* 00AB5C  lea  $abbe(pc),a0 */
    d0 = 0;                                              /* 00AB60  moveq  #0,d0 */
    SETB(d0, rd8(a4 + 0x36));                            /* 00AB62  move.b  $36(a4),d0 */
    SETW(d0, d0 & 0xF);                                  /* 00AB66  andi.w  #$f,d0 */
    d0 = (d0 & 0xFFFF) * 0xA;                            /* 00AB6A  mulu.w  #$a,d0 */
    a0 += d0;                                            /* 00AB6E  adda.l  d0,a0 */
    SETW(d0, rd16(a4 + 0x2));                            /* 00AB70  move.w  $2(a4),d0 */
    SETW(d1, rd16(a4 + 0x4));                            /* 00AB74  move.w  $4(a4),d1 */
    /* 00AB78  tst.w  (a0) */
    /* 00AB7A  bmi  $abbc */
    if ((int16_t)rd16(a0) >= 0) {
        /* 00AB7C  add.w  (a0)+,d0 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
        }
        /* 00AB7E  add.w  (a0)+,d1 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
        }
        /* 00AB80  move.w  (a0)+,d2 */
        SETW(d2, rd16(a0));
        a0 += 2;
        /* 00AB82  move.w  (a0)+,d3 */
        SETW(d3, rd16(a0));
        a0 += 2;
        /* 00AB84  move.w  (a0)+,d4 */
        SETW(d4, rd16(a0));
        a0 += 2;
        SETW(d7, 0x13);                                      /* 00AB86  move.w  #$13,d7 */
        a6 = V_135A;                                         /* 00AB8A  lea  $135a(a5),a6 */
        do {
            BUDGET_CHECK(0xAB8EU);
L_00AB8E:
            /* 00AB8E  tst.w  $4(a6) */
            /* 00AB92  bne  $abb4 */
            if (rd16(a6 + 0x4) == 0) {
                wr16(a6, (d0 & 0xFFFF));                             /* 00AB94  move.w  d0,(a6) */
                wr16(a6 + 0x2, (d1 & 0xFFFF));                       /* 00AB96  move.w  d1,$2(a6) */
                wr16(a6 + 0xA, (d2 & 0xFFFF));                       /* 00AB9A  move.w  d2,$a(a6) */
                wr16(a6 + 0xC, (d3 & 0xFFFF));                       /* 00AB9E  move.w  d3,$c(a6) */
                wr16(a6 + 0x6, (d4 & 0xFFFF));                       /* 00ABA2  move.w  d4,$6(a6) */
                wr16(a6 + 0x4, 0xFFFF);                              /* 00ABA6  move.w  #$ffff,$4(a6) */
                wr16(a6 + 0x8, rd16(V_027A));                        /* 00ABAC  move.w  $27a(a5),$8(a6) */
                /* 00ABB2  bra  $abbc */
                goto L_00ABBC;
            }
            a6 = a6 + 0xE;                                       /* 00ABB4  lea  $e(a6),a6 */
            /* 00ABB8  dbra  d7,$ab8e */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
    }
L_00ABBC:
    RTS();                                               /* 00ABBC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAB8EU: goto L_00AB8E;
        }
    }
}

/* 00ABBE  game routine */
void sub_00ABBE(void)
{
    /* 00ABBE  ori.b  #$60,$0(a6,d0.w) */
    {
        uint32_t s = 0x60;
        uint32_t d = rd8(a6 + SX16(d0));
        uint32_t r = (d | s) & 0xFF;
        wr8(a6 + SX16(d0), r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xABC4U);                                       /* 00ABC4  soft */
}

/* 00AC00  game routine */
void sub_00AC00(void)
{
    /* 00AC00  ori.b  #$e,d6 */
    {
        uint32_t s = 0xE;
        uint32_t d = (d6 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d6, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xAC04U);                                       /* 00AC04  soft */
}

/* 00AC5E  game routine */
void sub_00AC5E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d7, 0x13);                                      /* 00AC5E  move.w  #$13,d7 */
    a6 = V_135A;                                         /* 00AC62  lea  $135a(a5),a6 */
    do {
        BUDGET_CHECK(0xAC66U);
L_00AC66:
        /* 00AC66  tst.w  $4(a6) */
        /* 00AC6A  beq  $adb2 */
        if (rd16(a6 + 0x4) != 0) {
            /* 00AC6E  tst.w  $8(a6) */
            /* 00AC72  bmi  $ad82 */
            if ((int16_t)rd16(a6 + 0x8) >= 0) {
                SETW(d3, rd16(a6));                                  /* 00AC76  move.w  (a6),d3 */
                SETW(d4, rd16(a6 + 0x2));                            /* 00AC78  move.w  $2(a6),d4 */
                SETW(d5, 0x64);                                      /* 00AC7C  move.w  #$64,d5 */
                a4 = V_ENEMIES;                                      /* 00AC80  lea  $412(a5),a4 */
                SETW(d0, (d3 & 0xFFFF));                             /* 00AC84  move.w  d3,d0 */
                SETW(d1, (d4 & 0xFFFF));                             /* 00AC86  move.w  d4,d1 */
                d6 = 0x18;                                           /* 00AC88  moveq  #24,d6 */
                do {
                    BUDGET_CHECK(0xAC8AU);
L_00AC8A:
                    /* 00AC8A  tst.w  $4(a4) */
                    /* 00AC8E  beq  $ad12 */
                    if (rd16(a4 + 0x4) != 0) {
                        /* 00AC92  tst.w  $26(a4) */
                        /* 00AC96  bne  $ad12 */
                        if (rd16(a4 + 0x26) == 0) {
                            SETW(d2, (d0 & 0xFFFF));                             /* 00AC98  move.w  d0,d2 */
                            /* 00AC9A  sub.w  (a4),d2 */
                            {
                                uint32_t s = rd16(a4);
                                uint32_t d = (d2 & 0xFFFF);
                                uint32_t r = (d - s) & 0xFFFF;
                                SETW(d2, r);
                                fx = (s > d);
                            }
                            /* 00AC9C  cmp.w  #$10,d2 */
                            /* 00ACA0  bhi  $ad12 */
                            if ((d2 & 0xFFFF) <= 0x10) {
                                SETW(d2, (d1 & 0xFFFF));                             /* 00ACA2  move.w  d1,d2 */
                                /* 00ACA4  sub.w  $2(a4),d2 */
                                {
                                    uint32_t s = rd16(a4 + 0x2);
                                    uint32_t d = (d2 & 0xFFFF);
                                    uint32_t r = (d - s) & 0xFFFF;
                                    SETW(d2, r);
                                    fx = (s > d);
                                }
                                /* 00ACA8  cmp.w  #$15,d2 */
                                /* 00ACAC  bhi  $ad12 */
                                if ((d2 & 0xFFFF) <= 0x15) {
                                    wr16(a6 + 0x4, 0);                                   /* 00ACAE  clr.w  $4(a6) */
                                    SETW(d0, rd16(a6));                                  /* 00ACB2  move.w  (a6),d0 */
                                    SETW(d1, rd16(a6 + 0x2));                            /* 00ACB4  move.w  $2(a6),d1 */
                                    SETW(d2, rd16(a6 + 0x8));                            /* 00ACB8  move.w  $8(a6),d2 */
                                    /* 00ACBC  subi.w  #$9,d1 */
                                    {
                                        uint32_t s = 9;
                                        uint32_t d = (d1 & 0xFFFF);
                                        uint32_t r = (d - s) & 0xFFFF;
                                        SETW(d1, r);
                                        fx = (s > d);
                                    }
                                    CALL(sub_006CA8, 0xACC4U);                           /* 00ACC0  bsr  $6ca8 */
L_00ACC4:
                                    wr16(a4 + 0x26, 0xFFFF);                             /* 00ACC4  move.w  #$ffff,$26(a4) */
                                    /* 00ACCA  cmpi.w  #$11,$8(a4) */
                                    /* 00ACD0  bcc  $acd6 */
                                    if (rd16(a4 + 0x8) < 0x11) {
                                        d0 = 3;                                              /* 00ACD2  moveq  #3,d0 */
                                        /* 00ACD4  bra  $acd8 */
                                    } else {
                                        d0 = 0xA;                                            /* 00ACD6  moveq  #10,d0 */
                                    }
                                    /* 00ACD8  move.l  a6,-(a7) */
                                    {
                                        uint32_t v = a6;
                                        a7 -= 4;
                                        wr32(a7, v);
                                    }
                                    a6 = a4;                                             /* 00ACDA  movea.l  a4,a6 */
                                    CALL(sub_00B1E4, 0xACE0U);                           /* 00ACDC  bsr  $b1e4 */
L_00ACE0:
                                    /* 00ACE0  movea.l  (a7)+,a6 */
                                    a6 = rd32(a7);
                                    a7 += 4;
                                    wr16(a4 + 0xC, 0);                                   /* 00ACE2  clr.w  $c(a4) */
                                    wr16(a4 + 0xE, 0);                                   /* 00ACE6  clr.w  $e(a4) */
                                    d0 = 4;                                              /* 00ACEA  moveq  #4,d0 */
                                    CALL(sub_00E756, 0xACF0U);                           /* 00ACEC  bsr  $e756 */
L_00ACF0:
                                    /* 00ACF0  addi.l  #$32,$2e2(a5) */
                                    {
                                        uint32_t s = 0x32;
                                        uint32_t d = rd32(V_02E2);
                                        uint32_t r = (d + s) & 0xFFFFFFFF;
                                        wr32(V_02E2, r);
                                        fx = (r < s);
                                    }
                                    /* 00ACF8  tst.w  $8(a6) */
                                    /* 00ACFC  beq  $ad0e */
                                    if (rd16(a6 + 0x8) != 0) {
                                        wr32(V_02E2, rd32(V_02E2) - 0x32);                   /* 00ACFE  subi.l  #$32,$2e2(a5) */
                                        /* 00AD06  addi.l  #$32,$37e(a5) */
                                        {
                                            uint32_t s = 0x32;
                                            uint32_t d = rd32(V_037E);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_037E, r);
                                            fx = (r < s);
                                        }
                                    }
                                    /* 00AD0E  bra  $ad82 */
                                    goto L_00AD82;
                                }
                            }
                        }
                    }
                    a4 = a4 + 0x40;                                      /* 00AD12  lea  $40(a4),a4 */
                    /* 00AD16  dbra  d6,$ac8a */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
                a4 = V_1C84;                                         /* 00AD1A  lea  $1c84(a5),a4 */
                SETW(d0, (d3 & 0xFFFF));                             /* 00AD1E  move.w  d3,d0 */
                SETW(d1, (d4 & 0xFFFF));                             /* 00AD20  move.w  d4,d1 */
                d6 = 5;                                              /* 00AD22  moveq  #5,d6 */
                do {
                    BUDGET_CHECK(0xAD24U);
L_00AD24:
                    /* 00AD24  tst.w  (a4) */
                    /* 00AD26  beq  $ad7a */
                    if (rd16(a4) != 0) {
                        /* 00AD28  tst.w  $e(a4) */
                        /* 00AD2C  bne  $ad7a */
                        if (rd16(a4 + 0xE) == 0) {
                            SETW(d2, (d0 & 0xFFFF));                             /* 00AD2E  move.w  d0,d2 */
                            /* 00AD30  sub.w  $4(a4),d2 */
                            {
                                uint32_t s = rd16(a4 + 0x4);
                                uint32_t d = (d2 & 0xFFFF);
                                uint32_t r = (d - s) & 0xFFFF;
                                SETW(d2, r);
                                fx = (s > d);
                            }
                            /* 00AD34  cmp.w  #$e,d2 */
                            /* 00AD38  bhi  $ad7a */
                            if ((d2 & 0xFFFF) <= 0xE) {
                                SETW(d2, (d1 & 0xFFFF));                             /* 00AD3A  move.w  d1,d2 */
                                /* 00AD3C  sub.w  $6(a4),d2 */
                                {
                                    uint32_t s = rd16(a4 + 0x6);
                                    uint32_t d = (d2 & 0xFFFF);
                                    uint32_t r = (d - s) & 0xFFFF;
                                    SETW(d2, r);
                                    fx = (s > d);
                                }
                                /* 00AD40  cmp.w  #$10,d2 */
                                /* 00AD44  bhi  $ad7a */
                                if ((d2 & 0xFFFF) <= 0x10) {
                                    wr16(a6 + 0x4, 0);                                   /* 00AD46  clr.w  $4(a6) */
                                    SETW(d0, rd16(a4 + 0x4));                            /* 00AD4A  move.w  $4(a4),d0 */
                                    SETW(d1, rd16(a4 + 0x6));                            /* 00AD4E  move.w  $6(a4),d1 */
                                    SETW(d2, rd16(a6 + 0x8));                            /* 00AD52  move.w  $8(a6),d2 */
                                    CALL(sub_006CA8, 0xAD5AU);                           /* 00AD56  bsr  $6ca8 */
L_00AD5A:
                                    /* 00AD5A  addi.l  #$32,$2e2(a5) */
                                    {
                                        uint32_t s = 0x32;
                                        uint32_t d = rd32(V_02E2);
                                        uint32_t r = (d + s) & 0xFFFFFFFF;
                                        wr32(V_02E2, r);
                                        fx = (r < s);
                                    }
                                    /* 00AD62  tst.w  $8(a6) */
                                    /* 00AD66  beq  $ad78 */
                                    if (rd16(a6 + 0x8) != 0) {
                                        wr32(V_02E2, rd32(V_02E2) - 0x32);                   /* 00AD68  subi.l  #$32,$2e2(a5) */
                                        /* 00AD70  addi.l  #$32,$37e(a5) */
                                        {
                                            uint32_t s = 0x32;
                                            uint32_t d = rd32(V_037E);
                                            uint32_t r = (d + s) & 0xFFFFFFFF;
                                            wr32(V_037E, r);
                                            fx = (r < s);
                                        }
                                    }
                                    /* 00AD78  bra  $ad82 */
                                    goto L_00AD82;
                                }
                            }
                        }
                    }
                    a4 = a4 + 0x12;                                      /* 00AD7A  lea  $12(a4),a4 */
                    /* 00AD7E  dbra  d6,$ad24 */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
            }
L_00AD82:
            /* 00AD82  tst.w  $4(a6) */
            /* 00AD86  beq  $adb2 */
            if (rd16(a6 + 0x4) != 0) {
                SETW(d0, rd16(a6 + 0xA));                            /* 00AD88  move.w  $a(a6),d0 */
                wr16(a6, rd16(a6) + (d0 & 0xFFFF));                  /* 00AD8C  add.w  d0,(a6) */
                SETW(d0, rd16(a6 + 0xC));                            /* 00AD8E  move.w  $c(a6),d0 */
                wr16(a6 + 0x2, rd16(a6 + 0x2) + (d0 & 0xFFFF));      /* 00AD92  add.w  d0,$2(a6) */
                /* 00AD96  subq.w  #1,$6(a6) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a6 + 0x6);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(a6 + 0x6, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 00AD9A  bne  $adb2 */
                if (COND(7)) {
                    SETW(d0, rd16(a6));                                  /* 00AD9C  move.w  (a6),d0 */
                    SETW(d1, rd16(a6 + 0x2));                            /* 00AD9E  move.w  $2(a6),d1 */
                    SETW(d2, rd16(a6 + 0x8));                            /* 00ADA2  move.w  $8(a6),d2 */
                    /* 00ADA6  subi.w  #$9,d1 */
                    {
                        uint32_t s = 9;
                        uint32_t d = (d1 & 0xFFFF);
                        uint32_t r = (d - s) & 0xFFFF;
                        SETW(d1, r);
                        fx = (s > d);
                    }
                    CALL(sub_006CA8, 0xADAEU);                           /* 00ADAA  bsr  $6ca8 */
L_00ADAE:
                    wr16(a6 + 0x4, 0);                                   /* 00ADAE  clr.w  $4(a6) */
                }
            }
        }
L_00ADB2:
        a6 = a6 + 0xE;                                       /* 00ADB2  lea  $e(a6),a6 */
        /* 00ADB6  dbra  d7,$ac66 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00ADBA:
    RTS();                                               /* 00ADBA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAC66U: goto L_00AC66;
        case 0xAC8AU: goto L_00AC8A;
        case 0xACC4U: goto L_00ACC4;
        case 0xACE0U: goto L_00ACE0;
        case 0xACF0U: goto L_00ACF0;
        case 0xAD24U: goto L_00AD24;
        case 0xAD5AU: goto L_00AD5A;
        case 0xADAEU: goto L_00ADAE;
        case 0xADB2U: goto L_00ADB2;
        case 0xADBAU: goto L_00ADBA;
        }
    }
}

/* 00ADF2  game routine */
void sub_00ADF2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00ADF2  movem.l  a4/a3/d0,-(a7) */
    PUSH32(a4);
    PUSH32(a3);
    PUSH32(d0);
    d0 = 0;                                              /* 00ADF6  moveq  #0,d0 */
    SETW(d0, rd16(a4 + 0x64));                           /* 00ADF8  move.w  $64(a4),d0 */
    a3 = a4 + 0x7A;                                      /* 00ADFC  lea  $7a(a4),a3 */
    SETW(d0, (d0 & 0xFFFF) << 1);                        /* 00AE00  lsl.w  #1,d0 */
    /* 00AE02  tst.w  $0(a3,d0.w) */
    /* 00AE06  beq  $ae16 */
    if (rd16(a3 + SX16(d0)) == 0) {
        goto L_00AE16;
    }
L_00AE08:
    wr16(a3 + SX16(d0), rd16(a3 + SX16(d0)) - 1);        /* 00AE08  subi.w  #$1,$0(a3,d0.w) */
    d6 = 0;                                              /* 00AE0E  moveq  #0,d6 */
L_00AE10:
    /* 00AE10  movem.l  (a7)+,d0/a3-a4 */
    d0 = POP32();
    a3 = POP32();
    a4 = POP32();
    RTS();                                               /* 00AE14  rts */
L_00AE16:
    d6 = 0xFFFFFFFF;                                     /* 00AE16  moveq  #-1,d6 */
    /* 00AE18  bra  $ae10 */
    BUDGET_CHECK(0xAE10U);
    goto L_00AE10;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAE08U: goto L_00AE08;
        case 0xAE10U: goto L_00AE10;
        case 0xAE16U: goto L_00AE16;
        }
    }
}

/* 00AE1A  game routine */
void sub_00AE1A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1CF0;                                         /* 00AE1A  lea  $1cf0(a5),a4 */
    a6 = V_02DA;                                         /* 00AE1E  lea  $2da(a5),a6 */
    CALL(sub_00AE2A, 0xAE26U);                           /* 00AE22  bsr  $ae2a */
L_00AE26:
    /* 00AE26  lea  $376(a5),a6 */
    a6 = V_0376;
    { sub_00AE2A(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAE26U: goto L_00AE26;
        }
    }
}

/* 00AE2A  game routine */
void sub_00AE2A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00AE2A  tst.w  (a6) */
    /* 00AE2C  beq  $ae78 */
    if (rd16(a6) != 0) {
L_00AE2E:
        /* 00AE2E  tst.w  $50(a6) */
        /* 00AE32  bne  $ae78 */
        if (rd16(a6 + 0x50) == 0) {
L_00AE34:
            SETW(d2, rd16(a6 + 0x2));                            /* 00AE34  move.w  $2(a6),d2 */
            SETW(d3, rd16(a6 + 0x4));                            /* 00AE38  move.w  $4(a6),d3 */
            /* 00AE3C  tst.w  (a4) */
            /* 00AE3E  beq  $ae78 */
            if (rd16(a4) != 0) {
                SETW(d0, rd16(a4 + 0x2));                            /* 00AE40  move.w  $2(a4),d0 */
                SETW(d1, rd16(a4 + 0x4));                            /* 00AE44  move.w  $4(a4),d1 */
                SETW(d2, d2 + 0x24);                                 /* 00AE48  addi.w  #$24,d2 */
                SETW(d3, d3 + 0x64);                                 /* 00AE4C  addi.w  #$64,d3 */
                SETW(d1, d1 - (d3 & 0xFFFF));                        /* 00AE50  sub.w  d3,d1 */
                SETW(d1, d1 + 0x14);                                 /* 00AE52  addi.w  #$14,d1 */
                /* 00AE56  cmp.w  #$32,d1 */
                /* 00AE5A  bhi  $ae78 */
                if ((d1 & 0xFFFF) <= 0x32) {
                    SETW(d0, d0 - (d2 & 0xFFFF));                        /* 00AE5C  sub.w  d2,d0 */
                    SETW(d0, d0 + 0x10);                                 /* 00AE5E  addi.w  #$10,d0 */
                    /* 00AE62  cmp.w  #$20,d0 */
                    /* 00AE66  bcc  $ae78 */
                    if ((d0 & 0xFFFF) < 0x20) {
                        wr16(V_MISSION_END, 0xFFFF);                         /* 00AE68  move.w  #$ffff,$206(a5) */
                        wr16(a4, 0);                                         /* 00AE6E  clr.w  (a4) */
                        wr32(a6 + 0x8, rd32(a6 + 0x8) + 0x7D0);              /* 00AE70  addi.l  #$7d0,$8(a6) */
                    }
                }
            }
        }
    }
L_00AE78:
    RTS();                                               /* 00AE78  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAE2EU: goto L_00AE2E;
        case 0xAE34U: goto L_00AE34;
        case 0xAE78U: goto L_00AE78;
        }
    }
}

/* 00AE7A  game routine */
void sub_00AE7A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_VEHICLES;                                     /* 00AE7A  lea  $1a76(a5),a6 */
    SETW(d7, 4);                                         /* 00AE7E  move.w  #$4,d7 */
    do {
        BUDGET_CHECK(0xAE82U);
L_00AE82:
        /* 00AE82  tst.w  (a6) */
        /* 00AE84  beq  $aeec */
        if (rd16(a6) != 0) {
L_00AE88:
            /* 00AE88  tst.w  $c(a6) */
            /* 00AE8C  bmi  $aeec */
            if ((int16_t)rd16(a6 + 0xC) >= 0) {
L_00AE8E:
                /* 00AE8E  cmpi.w  #$4,$10(a6) */
                /* 00AE94  bne  $aeec */
                if (rd16(a6 + 0x10) == 4) {
                    for (;;) {
                        BUDGET_CHECK(0xAE96U);
L_00AE96:
                        /* 00AE96  tst.w  $2a(a6) */
                        /* 00AE9A  beq  $aea2 */
                        if (rd16(a6 + 0x2A) != 0) {
                            /* 00AE9C  subq.w  #1,$2a(a6) */
                            {
                                uint32_t s = 1;
                                uint32_t d = rd16(a6 + 0x2A);
                                uint32_t r = (d - s) & 0xFFFF;
                                wr16(a6 + 0x2A, r);
                                fx = (s > d);
                            }
                            /* 00AEA0  bra  $aeec */
                            goto L_00AEEC;
                        }
                        a0 = 0xAEF6;                                         /* 00AEA2  lea  $aef6(pc),a0 */
                        SETW(d6, rd16(a6 + 0x2C));                           /* 00AEA6  move.w  $2c(a6),d6 */
                        /* 00AEAA  addi.w  #$a,$2c(a6) */
                        {
                            uint32_t s = 0xA;
                            uint32_t d = rd16(a6 + 0x2C);
                            uint32_t r = (d + s) & 0xFFFF;
                            wr16(a6 + 0x2C, r);
                            fx = (r < s);
                        }
                        /* 00AEB0  cmpi.w  #$50,$2c(a6) */
                        /* 00AEB6  bls  $aec0 */
                        if (rd16(a6 + 0x2C) > 0x50) {
                            wr16(a6 + 0x2C, 0);                                  /* 00AEB8  clr.w  $2c(a6) */
                            /* 00AEBC  bra  $aeec */
                            goto L_00AEEC;
                        }
                        SETW(d0, rd16(a6 + 0x2));                            /* 00AEC0  move.w  $2(a6),d0 */
                        SETW(d1, rd16(a6 + 0x4));                            /* 00AEC4  move.w  $4(a6),d1 */
                        SETW(d0, d0 + rd16(a0 + SX16(d6)));                  /* 00AEC8  add.w  $0(a0,d6.w),d0 */
                        /* 00AECC  add.w  $2(a0,d6.w),d1 */
                        {
                            uint32_t s = rd16(a0 + 0x2 + SX16(d6));
                            uint32_t d = (d1 & 0xFFFF);
                            uint32_t r = (d + s) & 0xFFFF;
                            SETW(d1, r);
                            fx = (r < s);
                        }
                        SETW(d3, rd16(a0 + 0x4 + SX16(d6)));                 /* 00AED0  move.w  $4(a0,d6.w),d3 */
                        SETW(d4, rd16(a0 + 0x6 + SX16(d6)));                 /* 00AED4  move.w  $6(a0,d6.w),d4 */
                        wr16(a6 + 0x2A, rd16(a0 + 0x8 + SX16(d6)));          /* 00AED8  move.w  $8(a0,d6.w),$2a(a6) */
                        SETW(d5, 0xFFFE);                                    /* 00AEDE  move.w  #$fffe,d5 */
                        SETW(d2, 0xC8);                                      /* 00AEE2  move.w  #$c8,d2 */
                        CALL(sub_006B74, 0xAEEAU);                           /* 00AEE6  bsr  $6b74 */
L_00AEEA:
                        /* 00AEEA  bra  $ae96 */
                    }
                }
            }
        }
L_00AEEC:
        a6 = a6 + 0x2E;                                      /* 00AEEC  lea  $2e(a6),a6 */
        /* 00AEF0  dbra  d7,$ae82 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00AEF4:
    RTS();                                               /* 00AEF4  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAE82U: goto L_00AE82;
        case 0xAE88U: goto L_00AE88;
        case 0xAE8EU: goto L_00AE8E;
        case 0xAE96U: goto L_00AE96;
        case 0xAEEAU: goto L_00AEEA;
        case 0xAEECU: goto L_00AEEC;
        case 0xAEF4U: goto L_00AEF4;
        }
    }
}

/* 00AF46  game routine */
void sub_00AF46(void)
{
    SETB(d1, d1 | 1);                                    /* 00AF46  ori.b  #$1,d1 */
    SETB(d3, d3 | 2);                                    /* 00AF4A  ori.b  #$2,d3 */
    SETB(d2, d2 | 3);                                    /* 00AF4E  ori.b  #$3,d2 */
    SETB(d3, d3 | 3);                                    /* 00AF52  ori.b  #$3,d3 */
    SETB(d3, d3 | 2);                                    /* 00AF56  ori.b  #$2,d3 */
    SETB(d2, d2 | 2);                                    /* 00AF5A  ori.b  #$2,d2 */
    SETB(d2, d2 | 2);                                    /* 00AF5E  ori.b  #$2,d2 */
    SETB(d0, d0 | 0);                                    /* 00AF62  ori.b  #$0,d0 */
    /* 00AF66  ori.b  #$8,d0 */
    SETB(d0, d0 | 8);
    { sub_00AF6A(); return; }
}

/* 00AF6A  game routine */
void sub_00AF6A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00AF6A  move.w  d0,-(a7) */
    {
        uint32_t v = (d0 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    a6 = 0x2400;                                         /* 00AF6C  lea  $2400.l,a6 */
    SETW(d2, rd16(V_SCROLL));                            /* 00AF72  move.w  $280(a5),d2 */
    SETW(d2, d2 - (d1 & 0xFFFF));                        /* 00AF76  sub.w  d1,d2 */
    SETW(d2, d2 & 0xFFF0);                               /* 00AF78  andi.w  #$fff0,d2 */
    SETW(d2, d2 & 0x3FF);                                /* 00AF7C  andi.w  #$3ff,d2 */
    SETW(d0, (d0 & 0xFFFF) >> 4);                        /* 00AF80  lsr.w  #4,d0 */
    SETW(d2, d2 + (d0 & 0xFFFF));                        /* 00AF82  add.w  d0,d2 */
    /* 00AF84  cmp.w  #$20,d2 */
    /* 00AF88  bcc  $af8e */
    if ((d2 & 0xFFFF) < 0x20) {
L_00AF8A:
        SETW(d2, d2 + 0x400);                                /* 00AF8A  addi.w  #$400,d2 */
    }
L_00AF8E:
    SETB(d2, rd8(a6 + SX16(d2)));                        /* 00AF8E  move.b  $0(a6,d2.w),d2 */
    /* 00AF92  move.w  (a7)+,d0 */
    SETW(d0, rd16(a7));
    a7 += 2;
    RTS();                                               /* 00AF94  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAF8AU: goto L_00AF8A;
        case 0xAF8EU: goto L_00AF8E;
        }
    }
}

/* 00AF96  game routine */
void sub_00AF96(void)
{
    /* 00AF96  bra  $74f0 */
    { sub_0074F0(); return; }
}

/* 00AF9A  game routine */
void sub_00AF9A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_00AF9A;
L_004B8C:
    wr16(V_0168, 0x4F);                                  /* 004B8C  move.w  #$4f,$168(a5) */
    wr32(V_016C, 0x18000);                               /* 004B92  move.l  #$18000,$16c(a5) */
    CALL(sub_004BBE, 0x4B9CU);                           /* 004B9A  bsr  $4bbe */
L_004B9C:
    wr32(V_0154, a0);                                    /* 004B9C  move.l  a0,$154(a5) */
    wr32(V_0194, a0);                                    /* 004BA0  move.l  a0,$194(a5) */
    wr16(V_0168, 0x4E);                                  /* 004BA4  move.w  #$4e,$168(a5) */
    wr32(V_016C, 0x18000);                               /* 004BAA  move.l  #$18000,$16c(a5) */
    CALL(sub_004BBE, 0x4BB4U);                           /* 004BB2  bsr  $4bbe */
L_004BB4:
    wr32(V_015C, a0);                                    /* 004BB4  move.l  a0,$15c(a5) */
    wr32(V_0198, a0);                                    /* 004BB8  move.l  a0,$198(a5) */
    RTS();                                               /* 004BBC  rts */
    for (;;) {
        BUDGET_CHECK(0x86C0U);
L_0086C0:
        /* 0086C0  bra  $4b8c */
        BUDGET_CHECK(0x4B8CU);
        goto L_004B8C;
L_00AF9A:
        /* 00AF9A  bra  $86c0 */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4B8CU: goto L_004B8C;
        case 0x4B9CU: goto L_004B9C;
        case 0x4BB4U: goto L_004BB4;
        case 0x86C0U: goto L_0086C0;
        }
    }
}

/* 00AF9E  game routine */
void sub_00AF9E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00AF9E  bra  $fdc8 */
L_00FDC8:
    wr16(V_0266, 0x3980);                                /* 00FDC8  move.w  #$3980,$266(a5) */
    wr16(V_0268, 0xA);                                   /* 00FDCE  move.w  #$a,$268(a5) */
    wr16(V_026A, 0xA);                                   /* 00FDD4  move.w  #$a,$26a(a5) */
    wr16(0xFF8242, 0x777);                               /* 00FDDA  move.w  #$777,$ff8242.l */
    wr16(V_026C, 1);                                     /* 00FDE2  move.w  #$1,$26c(a5) */
    a0 = 0xFE1E;                                         /* 00FDE8  lea  $fe1e(pc),a0 */
    CALL(sub_008584, 0xFDF0U);                           /* 00FDEC  bsr  $8584 */
    do {
        BUDGET_CHECK(0xFDF0U);
L_00FDF0:
        /* 00FDF0  cmpi.b  #$6d,$1de2(a5) */
        /* 00FDF6  beq  $fe00 */
        IDLE_EVERY(0xFDF0U);
        if (rd8(V_1DE2) != 0x6D) {
L_00FDF8:
            /* 00FDF8  cmpi.b  #$2,$1de2(a5) */
            /* 00FDFE  bne  $fe06 */
            if (rd8(V_1DE2) != 2) {
                goto L_00FE06;
            }
        }
L_00FE00:
        wr16(V_01B4, 0);                                     /* 00FE00  clr.w  $1b4(a5) */
        /* 00FE04  bra  $fe1c */
        goto L_00FE1C;
L_00FE06:
        /* 00FE06  cmpi.b  #$6e,$1de2(a5) */
        /* 00FE0C  beq  $fe16 */
        if (rd8(V_1DE2) == 0x6E) {
            goto L_00FE16;
        }
L_00FE0E:
        /* 00FE0E  cmpi.b  #$3,$1de2(a5) */
        /* 00FE14  bne  $fdf0 */
    } while (rd8(V_1DE2) != 3);
L_00FE16:
    wr16(V_01B4, 0xFFFF);                                /* 00FE16  move.w  #$ffff,$1b4(a5) */
L_00FE1C:
    RTS();                                               /* 00FE1C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFDC8U: goto L_00FDC8;
        case 0xFDF0U: goto L_00FDF0;
        case 0xFDF8U: goto L_00FDF8;
        case 0xFE00U: goto L_00FE00;
        case 0xFE06U: goto L_00FE06;
        case 0xFE0EU: goto L_00FE0E;
        case 0xFE1CU: goto L_00FE1C;
        }
    }
}

/* 00AFA2  game routine */
void sub_00AFA2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
L_00AFA2:
    d0 = 0;                                              /* 00AFA2  moveq  #0,d0 */
    SETB(d0, rd8(V_1DF4));                               /* 00AFA4  move.b  $1df4(a5),d0 */
    a0 = rd32(V_1DF6);                                   /* 00AFA8  movea.l  $1df6(a5),a0 */
    SETB(d1, rd8(a0));                                   /* 00AFAC  move.b  (a0),d1 */
    /* 00AFAE  cmp.b  #$ff,d1 */
    /* 00AFB2  beq  $afb8 */
    if ((d1 & 0xFF) != 0xFF) {
L_00AFB4:
        /* 00AFB4  cmp.b  d0,d1 */
        /* 00AFB6  beq  $afba */
        if ((d1 & 0xFF) == (d0 & 0xFF)) {
            goto L_00AFBA;
        }
    }
L_00AFB8:
    RTS();                                               /* 00AFB8  rts */
L_00AFBA:
    a0 += 1;                                             /* 00AFBA  addq.l  #1,a0 */
    SETW(d0, 0);                                         /* 00AFBC  clr.w  d0 */
    /* 00AFBE  move.b  (a0)+,d0 */
    SETB(d0, rd8(a0));
    a0 += 1;
    /* 00AFC0  cmp.b  #$ea,d0 */
    /* 00AFC4  bcs  $afd2 */
    if ((d0 & 0xFF) >= 0xEA) {
L_00AFC6:
        /* 00AFC6  cmp.b  #$ef,d0 */
        /* 00AFCA  bhi  $afd2 */
        if ((d0 & 0xFF) <= 0xEF) {
            CALL(sub_004C2E, 0xAFD0U);                           /* 00AFCC  bsr  $4c2e */
L_00AFD0:
            /* 00AFD0  bra  $afa2 */
            BUDGET_CHECK(0xAFA2U);
            goto L_00AFA2;
        }
    }
L_00AFD2:
    /* 00AFD2  cmp.b  #$f0,d0 */
    /* 00AFD6  bcs  $afe4 */
    if ((d0 & 0xFF) >= 0xF0) {
L_00AFD8:
        /* 00AFD8  cmp.b  #$f5,d0 */
        /* 00AFDC  bhi  $afe4 */
        if ((d0 & 0xFF) <= 0xF5) {
L_00AFDE:
            CALL(sub_0052FA, 0xAFE2U);                           /* 00AFDE  bsr  $52fa */
L_00AFE2:
            /* 00AFE2  bra  $afa2 */
            BUDGET_CHECK(0xAFA2U);
            goto L_00AFA2;
        }
    }
L_00AFE4:
    /* 00AFE4  cmp.b  #$fe,d0 */
    {
        uint32_t s = 0xFE;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d - s) & 0xFF;
        fz = (r == 0);
    }
L_00AFE8:
    /* 00AFE8  beq  $b662 */
    if (COND(6)) {
L_00AFEC:
        /* 00AFEC  cmp.b  #$fd,d0 */
        {
            uint32_t s = 0xFD;
            uint32_t d = (d0 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            fz = (r == 0);
        }
L_00AFF0:
        /* 00AFF0  beq  $b606 */
        if (COND(6)) {
L_00AFF4:
            /* 00AFF4  cmp.b  #$f6,d0 */
            {
                uint32_t s = 0xF6;
                uint32_t d = (d0 & 0xFF);
                uint32_t r = (d - s) & 0xFF;
                fc = (s > d);
            }
L_00AFF8:
            /* 00AFF8  bcs  $b008 */
            if (COND(4)) {
                /* 00AFFC  cmp.b  #$fc,d0 */
                /* 00B000  bhi  $b008 */
                if ((d0 & 0xFF) > 0xFC) {
                    goto L_00B008;
                }
                /* 00B004  bra  $b400 */
            } else {
L_00B008:
                d5 = 0;                                              /* 00B008  moveq  #0,d5 */
                d6 = 0;                                              /* 00B00A  moveq  #0,d6 */
                /* 00B00C  move.b  (a0)+,d5 */
                SETB(d5, rd8(a0));
                a0 += 1;
                /* 00B00E  cmp.b  #$bf,d5 */
                /* 00B012  bls  $b016 */
                if ((d5 & 0xFF) > 0xBF) {
L_00B014:
                    SETW(d5, SX8(d5));                                   /* 00B014  ext.w  d5 */
                }
L_00B016:
                /* 00B016  move.b  (a0)+,d6 */
                SETB(d6, rd8(a0));
                a0 += 1;
                /* 00B018  cmp.b  #$b0,d6 */
                /* 00B01C  bls  $b020 */
                if ((d6 & 0xFF) > 0xB0) {
L_00B01E:
                    SETW(d6, SX8(d6));                                   /* 00B01E  ext.w  d6 */
                }
L_00B020:
                SETW(d5, d5 + 0x64);                                 /* 00B020  addi.w  #$64,d5 */
                SETW(d6, d6 + 0x64);                                 /* 00B024  addi.w  #$64,d6 */
                wr32(V_1DF6, a0);                                    /* 00B028  move.l  a0,$1df6(a5) */
                a1 = 0x3A8E;                                         /* 00B02C  lea  $3a8e(pc),a1 */
                SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 00B030  lsl.w  #1,d0 */
                SETW(d0, rd16(a1 + SX16(d0)));                       /* 00B032  move.w  $0(a1,d0.w),d0 */
                a0 = 0x3B6E;                                         /* 00B036  lea  $3b6e(pc),a0 */
                a0 = a0 + SX16(d0);                                  /* 00B03A  lea  $0(a0,d0.w),a0 */
                a6 = V_ENEMIES;                                      /* 00B03E  lea  $412(a5),a6 */
                SETW(d7, 0x18);                                      /* 00B042  move.w  #$18,d7 */
                do {
                    BUDGET_CHECK(0xB046U);
L_00B046:
                    /* 00B046  tst.w  $4(a6) */
                    /* 00B04A  beq  $b058 */
                    if (rd16(a6 + 0x4) == 0) {
                        goto L_00B058;
                    }
L_00B04C:
                    a6 = a6 + 0x40;                                      /* 00B04C  lea  $40(a6),a6 */
                    /* 00B050  dbra  d7,$b046 */
                    SETW(d7, d7 - 1);
                } while ((d7 & 0xFFFF) != 0xFFFF);
                /* 00B054  bra  $afa2 */
                BUDGET_CHECK(0xAFA2U);
                goto L_00AFA2;
L_00B058:
                d0 = 0;                                              /* 00B058  moveq  #0,d0 */
                wr16(a6, (d5 & 0xFFFF));                             /* 00B05A  move.w  d5,(a6) */
                wr16(a6 + 0x2, (d6 & 0xFFFF));                       /* 00B05C  move.w  d6,$2(a6) */
                /* 00B060  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                CALL(sub_00B1E4, 0xB066U);                           /* 00B062  bsr  $b1e4 */
L_00B066:
                /* 00B066  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                wr16(a6 + 0x1E, (d0 & 0xFFFF));                      /* 00B068  move.w  d0,$1e(a6) */
                wr16(a6 + 0x20, (d0 & 0xFFFF));                      /* 00B06C  move.w  d0,$20(a6) */
                /* 00B070  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                wr16(a6 + 0xA, (d0 & 0xFFFF));                       /* 00B072  move.w  d0,$a(a6) */
                /* 00B076  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                SETW(d0, SX8(d0));                                   /* 00B078  ext.w  d0 */
                wr16(a6 + 0xC, (d0 & 0xFFFF));                       /* 00B07A  move.w  d0,$c(a6) */
                SETW(d0, 0);                                         /* 00B07E  clr.w  d0 */
                /* 00B080  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                SETW(d0, SX8(d0));                                   /* 00B082  ext.w  d0 */
                wr16(a6 + 0xE, (d0 & 0xFFFF));                       /* 00B084  move.w  d0,$e(a6) */
                wr32(a6 + 0x10, a0);                                 /* 00B088  move.l  a0,$10(a6) */
                wr16(a6 + 0x4, 1);                                   /* 00B08C  move.w  #$1,$4(a6) */
                wr16(a6 + 0x26, 0);                                  /* 00B092  clr.w  $26(a6) */
                /* 00B096  bra  $afa2 */
                BUDGET_CHECK(0xAFA2U);
                goto L_00AFA2;
            }
            SETW(d3, (d0 & 0xFFFF));                             /* 00B400  move.w  d0,d3 */
            d1 = 0;                                              /* 00B402  moveq  #0,d1 */
            d2 = 0;                                              /* 00B404  moveq  #0,d2 */
            d4 = 0;                                              /* 00B406  moveq  #0,d4 */
            d5 = 0;                                              /* 00B408  moveq  #0,d5 */
            /* 00B40A  move.b  (a0)+,d0 */
            SETB(d0, rd8(a0));
            a0 += 1;
            /* 00B40C  move.b  (a0)+,d1 */
            SETB(d1, rd8(a0));
            a0 += 1;
            /* 00B40E  move.b  (a0)+,d2 */
            SETB(d2, rd8(a0));
            a0 += 1;
            /* 00B410  move.b  (a0)+,d4 */
            SETB(d4, rd8(a0));
            a0 += 1;
            /* 00B412  move.b  (a0)+,d5 */
            SETB(d5, rd8(a0));
            a0 += 1;
            /* 00B414  addi.w  #$64,d0 */
            {
                uint32_t s = 0x64;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d0, r);
                fx = (r < s);
            }
            CALL(sub_006026, 0xB41CU);                           /* 00B418  bsr  $6026 */
L_00B41C:
            wr32(V_1DF6, a0);                                    /* 00B41C  move.l  a0,$1df6(a5) */
            /* 00B420  bra  $afa2 */
            BUDGET_CHECK(0xAFA2U);
            goto L_00AFA2;
        }
        /* 00B606  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
        a4 = V_1C84;                                         /* 00B60A  lea  $1c84(a5),a4 */
        SETW(d7, 5);                                         /* 00B60E  move.w  #$5,d7 */
        do {
            BUDGET_CHECK(0xB612U);
L_00B612:
            /* 00B612  tst.w  (a4) */
            /* 00B614  bne  $b652 */
            if (rd16(a4) == 0) {
                d0 = 0;                                              /* 00B616  moveq  #0,d0 */
                d1 = 0;                                              /* 00B618  moveq  #0,d1 */
                d2 = 0;                                              /* 00B61A  moveq  #0,d2 */
                d3 = 0;                                              /* 00B61C  moveq  #0,d3 */
                /* 00B61E  move.b  (a0)+,d0 */
                SETB(d0, rd8(a0));
                a0 += 1;
                /* 00B620  move.b  (a0)+,d1 */
                SETB(d1, rd8(a0));
                a0 += 1;
                /* 00B622  addi.w  #$64,d1 */
                {
                    uint32_t s = 0x64;
                    uint32_t d = (d1 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d1, r);
                    fx = (r < s);
                }
                /* 00B626  move.b  (a0)+,d2 */
                SETB(d2, rd8(a0));
                a0 += 1;
                /* 00B628  move.b  (a0)+,d3 */
                SETB(d3, rd8(a0));
                a0 += 1;
                wr16(a4 + 0x2, (d0 & 0xFFFF));                       /* 00B62A  move.w  d0,$2(a4) */
                wr16(a4 + 0x4, (d1 & 0xFFFF));                       /* 00B62E  move.w  d1,$4(a4) */
                wr16(a4 + 0x6, (d2 & 0xFFFF));                       /* 00B632  move.w  d2,$6(a4) */
                wr16(a4 + 0x8, (d3 & 0xFFFF));                       /* 00B636  move.w  d3,$8(a4) */
                wr16(a4 + 0xA, 0);                                   /* 00B63A  clr.w  $a(a4) */
                wr16(a4 + 0xE, 0);                                   /* 00B63E  clr.w  $e(a4) */
                wr16(a4, 0xFFFF);                                    /* 00B642  move.w  #$ffff,(a4) */
                wr16(a4 + 0x10, 1);                                  /* 00B646  move.w  #$1,$10(a4) */
                wr32(V_1DF6, a0);                                    /* 00B64C  move.l  a0,$1df6(a5) */
                /* 00B650  bra  $b65a */
                goto L_00B65A;
            }
            a4 = a4 + 0x12;                                      /* 00B652  lea  $12(a4),a4 */
            /* 00B656  dbra  d7,$b612 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_00B65A:
        /* 00B65A  movem.l  (a7)+,d0-d7/a0-a6 */
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
        /* 00B65E  bra  $afa2 */
        BUDGET_CHECK(0xAFA2U);
        goto L_00AFA2;
    }
    /* 00B662  movem.l  a3/d2/d1/d0,-(a7) */
    PUSH32(a3);
    PUSH32(d2);
    PUSH32(d1);
    PUSH32(d0);
    a3 = V_1CF0;                                         /* 00B666  lea  $1cf0(a5),a3 */
    d0 = 0;                                              /* 00B66A  moveq  #0,d0 */
    d1 = 0;                                              /* 00B66C  moveq  #0,d1 */
    d2 = 0;                                              /* 00B66E  moveq  #0,d2 */
    /* 00B670  move.b  (a0)+,d0 */
    SETB(d0, rd8(a0));
    a0 += 1;
    /* 00B672  move.b  (a0)+,d1 */
    SETB(d1, rd8(a0));
    a0 += 1;
    /* 00B674  move.b  (a0)+,d2 */
    SETB(d2, rd8(a0));
    a0 += 1;
    /* 00B676  cmp.b  #$bf,d0 */
    /* 00B67A  bls  $b67e */
    if ((d0 & 0xFF) > 0xBF) {
        SETW(d0, SX8(d0));                                   /* 00B67C  ext.w  d0 */
    }
    /* 00B67E  cmp.b  #$b0,d1 */
    /* 00B682  bls  $b686 */
    if ((d1 & 0xFF) > 0xB0) {
        SETW(d1, SX8(d1));                                   /* 00B684  ext.w  d1 */
    }
    /* 00B686  addi.w  #$64,d0 */
    {
        uint32_t s = 0x64;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
        fx = (r < s);
    }
    wr32(V_1DF6, a0);                                    /* 00B68A  move.l  a0,$1df6(a5) */
    wr16(a3 + 0x2, (d0 & 0xFFFF));                       /* 00B68E  move.w  d0,$2(a3) */
    wr16(a3 + 0x4, (d1 & 0xFFFF));                       /* 00B692  move.w  d1,$4(a3) */
    wr16(a3 + 0x6, (d2 & 0xFFFF));                       /* 00B696  move.w  d2,$6(a3) */
    wr16(a3, 0xFFFF);                                    /* 00B69A  move.w  #$ffff,(a3) */
    /* 00B69E  movem.l  (a7)+,d0-d2/a3 */
    d0 = POP32();
    d1 = POP32();
    d2 = POP32();
    a3 = POP32();
    /* 00B6A2  bra  $afa2 */
    BUDGET_CHECK(0xAFA2U);
    goto L_00AFA2;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xAFB4U: goto L_00AFB4;
        case 0xAFB8U: goto L_00AFB8;
        case 0xAFBAU: goto L_00AFBA;
        case 0xAFC6U: goto L_00AFC6;
        case 0xAFD0U: goto L_00AFD0;
        case 0xAFD2U: goto L_00AFD2;
        case 0xAFD8U: goto L_00AFD8;
        case 0xAFDEU: goto L_00AFDE;
        case 0xAFE2U: goto L_00AFE2;
        case 0xAFE4U: goto L_00AFE4;
        case 0xAFE8U: goto L_00AFE8;
        case 0xAFECU: goto L_00AFEC;
        case 0xAFF0U: goto L_00AFF0;
        case 0xAFF4U: goto L_00AFF4;
        case 0xAFF8U: goto L_00AFF8;
        case 0xB008U: goto L_00B008;
        case 0xB014U: goto L_00B014;
        case 0xB016U: goto L_00B016;
        case 0xB01EU: goto L_00B01E;
        case 0xB020U: goto L_00B020;
        case 0xB046U: goto L_00B046;
        case 0xB04CU: goto L_00B04C;
        case 0xB058U: goto L_00B058;
        case 0xB066U: goto L_00B066;
        case 0xB41CU: goto L_00B41C;
        case 0xB612U: goto L_00B612;
        }
    }
}

