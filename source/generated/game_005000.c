/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 0052FA  game routine */
void sub_0052FA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0052FA  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a4 = V_1B5C;                                         /* 0052FE  lea  $1b5c(a5),a4 */
    SETW(d7, 0xF);                                       /* 005302  move.w  #$f,d7 */
    do {
        BUDGET_CHECK(0x5306U);
L_005306:
        /* 005306  tst.w  (a4) */
        /* 005308  bne  $5322 */
        if (rd16(a4) == 0) {
L_00530A:
            wr16(a4 + 0x6, (d0 & 0xFFFF));                       /* 00530A  move.w  d0,$6(a4) */
            wr8(a4 + 0x2, 0);                                    /* 00530E  clr.b  $2(a4) */
            wr8(a4 + 0x4, 0);                                    /* 005312  clr.b  $4(a4) */
            /* 005316  move.b  (a0)+,$3(a4) */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                wr8(a4 + 0x3, v);
            }
            /* 00531A  move.b  (a0)+,$5(a4) */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                wr8(a4 + 0x5, v);
            }
            wr8(a4, 0xFF);                                       /* 00531E  st  (a4) */
            /* 005320  bra  $532c */
            goto L_00532C;
        }
L_005322:
        a4 = a4 + 0xE;                                       /* 005322  lea  $e(a4),a4 */
        /* 005326  dbra  d7,$5306 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    a0 += 6;                                             /* 00532A  addq.l  #6,a0 */
L_00532C:
    wr32(V_1DF6, a0);                                    /* 00532C  move.l  a0,$1df6(a5) */
    /* 005330  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 005334  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5306U: goto L_005306;
        case 0x530AU: goto L_00530A;
        case 0x5322U: goto L_005322;
        case 0x532CU: goto L_00532C;
        }
    }
}

/* 005336  game routine */
void sub_005336(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1B5C;                                         /* 005336  lea  $1b5c(a5),a4 */
    d7 = 0xF;                                            /* 00533A  moveq  #15,d7 */
    do {
        BUDGET_CHECK(0x533CU);
L_00533C:
        /* 00533C  tst.w  (a4) */
        /* 00533E  beq  $546a */
        if (rd16(a4) != 0) {
L_005342:
            /* 005342  cmpi.w  #$26,$2(a4) */
            /* 005348  bcs  $5352 */
            if (rd16(a4 + 0x2) >= 0x26) {
L_00534A:
                /* 00534A  cmpi.w  #$10a,$2(a4) */
                /* 005350  bcs  $5358 */
                if (rd16(a4 + 0x2) < 0x10A) {
                    goto L_005358;
                }
            }
L_005352:
            wr16(a4, 0);                                         /* 005352  clr.w  (a4) */
            /* 005354  bra  $546a */
            goto L_00546A;
L_005358:
            /* 005358  cmpi.w  #$f3,$6(a4) */
            /* 00535E  bne  $53aa */
            if (rd16(a4 + 0x6) == 0xF3) {
L_005360:
                /* 005360  tst.w  $8(a4) */
                /* 005364  beq  $536e */
                if (rd16(a4 + 0x8) != 0) {
L_005366:
                    /* 005366  subq.w  #1,$8(a4) */
                    {
                        uint32_t s = 1;
                        uint32_t d = rd16(a4 + 0x8);
                        uint32_t r = (d - s) & 0xFFFF;
                        wr16(a4 + 0x8, r);
                        fx = (s > d);
                    }
                    /* 00536A  bra  $546a */
                    goto L_00546A;
                }
L_00536E:
                CALL(sub_007576, 0x5372U);                           /* 00536E  bsr  $7576 */
L_005372:
                SETW(d0, d0 & 0x1F);                                 /* 005372  andi.w  #$1f,d0 */
                wr16(a4 + 0x8, (d0 & 0xFFFF));                       /* 005376  move.w  d0,$8(a4) */
                SETW(d0, rd16(a4 + 0x2));                            /* 00537A  move.w  $2(a4),d0 */
                SETW(d1, rd16(a4 + 0x4));                            /* 00537E  move.w  $4(a4),d1 */
                SETW(d0, d0 + 0x24);                                 /* 005382  addi.w  #$24,d0 */
                SETW(d1, d1 + 0x64);                                 /* 005386  addi.w  #$64,d1 */
                SETW(d1, d1 + 0x10);                                 /* 00538A  addi.w  #$10,d1 */
                /* 00538E  addi.w  #$6,d0 */
                {
                    uint32_t s = 6;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d0, r);
                    fx = (r < s);
                }
                SETW(d3, 0);                                         /* 005392  move.w  #$0,d3 */
                SETW(d4, 3);                                         /* 005396  move.w  #$3,d4 */
                SETW(d5, 0xFFFF);                                    /* 00539A  move.w  #$ffff,d5 */
                SETW(d2, 0x32);                                      /* 00539E  move.w  #$32,d2 */
                CALL(sub_006B74, 0x53A6U);                           /* 0053A2  bsr  $6b74 */
L_0053A6:
                /* 0053A6  bra  $546a */
            } else {
L_0053AA:
                /* 0053AA  cmpi.w  #$f4,$6(a4) */
                /* 0053B0  bne  $53f8 */
                if (rd16(a4 + 0x6) == 0xF4) {
L_0053B2:
                    /* 0053B2  tst.w  $8(a4) */
                    /* 0053B6  beq  $53c0 */
                    if (rd16(a4 + 0x8) != 0) {
L_0053B8:
                        /* 0053B8  subq.w  #1,$8(a4) */
                        {
                            uint32_t s = 1;
                            uint32_t d = rd16(a4 + 0x8);
                            uint32_t r = (d - s) & 0xFFFF;
                            wr16(a4 + 0x8, r);
                            fx = (s > d);
                        }
                        /* 0053BC  bra  $546a */
                        goto L_00546A;
                    }
L_0053C0:
                    CALL(sub_007576, 0x53C4U);                           /* 0053C0  bsr  $7576 */
L_0053C4:
                    SETW(d0, d0 & 0x1F);                                 /* 0053C4  andi.w  #$1f,d0 */
                    wr16(a4 + 0x8, (d0 & 0xFFFF));                       /* 0053C8  move.w  d0,$8(a4) */
                    SETW(d0, rd16(a4 + 0x2));                            /* 0053CC  move.w  $2(a4),d0 */
                    SETW(d1, rd16(a4 + 0x4));                            /* 0053D0  move.w  $4(a4),d1 */
                    SETW(d0, d0 + 0x24);                                 /* 0053D4  addi.w  #$24,d0 */
                    SETW(d1, d1 + 0x64);                                 /* 0053D8  addi.w  #$64,d1 */
                    /* 0053DC  addi.w  #$6,d0 */
                    {
                        uint32_t s = 6;
                        uint32_t d = (d0 & 0xFFFF);
                        uint32_t r = (d + s) & 0xFFFF;
                        SETW(d0, r);
                        fx = (r < s);
                    }
                    SETW(d3, 0);                                         /* 0053E0  move.w  #$0,d3 */
                    SETW(d4, 0xFFFD);                                    /* 0053E4  move.w  #$fffd,d4 */
                    SETW(d5, 0xFFFF);                                    /* 0053E8  move.w  #$ffff,d5 */
                    SETW(d2, 0x32);                                      /* 0053EC  move.w  #$32,d2 */
                    CALL(sub_006B74, 0x53F4U);                           /* 0053F0  bsr  $6b74 */
L_0053F4:
                    /* 0053F4  bra  $546a */
                } else {
L_0053F8:
                    /* 0053F8  cmpi.w  #$f2,$6(a4) */
                    /* 0053FE  bne  $546a */
                    if (rd16(a4 + 0x6) == 0xF2) {
L_005402:
                        CALL(sub_005680, 0x5406U);                           /* 005402  bsr  $5680 */
L_005406:
                        /* 005406  tst.w  d6 */
                        /* 005408  bne  $546a */
                        if ((d6 & 0xFFFF) == 0) {
L_00540C:
                            /* 00540C  tst.w  $8(a4) */
                            /* 005410  beq  $541a */
                            if (rd16(a4 + 0x8) != 0) {
L_005412:
                                /* 005412  subq.w  #1,$8(a4) */
                                {
                                    uint32_t s = 1;
                                    uint32_t d = rd16(a4 + 0x8);
                                    uint32_t r = (d - s) & 0xFFFF;
                                    wr16(a4 + 0x8, r);
                                    fx = (s > d);
                                }
                                /* 005416  bra  $546a */
                            } else {
L_00541A:
                                wr16(a4 + 0xA, rd16(a4 + 0xA) + 1);                  /* 00541A  addq.w  #1,$a(a4) */
                                /* 00541E  cmpi.w  #$e,$a(a4) */
                                /* 005424  bne  $5436 */
                                if (rd16(a4 + 0xA) == 0xE) {
L_005426:
                                    CALL(sub_007576, 0x542AU);                           /* 005426  bsr  $7576 */
L_00542A:
                                    SETW(d0, d0 & 0x63);                                 /* 00542A  andi.w  #$63,d0 */
                                    wr16(a4 + 0x8, (d0 & 0xFFFF));                       /* 00542E  move.w  d0,$8(a4) */
                                    wr16(a4 + 0xA, 0);                                   /* 005432  clr.w  $a(a4) */
                                }
L_005436:
                                SETW(d0, rd16(a4 + 0x2));                            /* 005436  move.w  $2(a4),d0 */
                                SETW(d1, rd16(a4 + 0x4));                            /* 00543A  move.w  $4(a4),d1 */
                                SETW(d0, d0 + 0x24);                                 /* 00543E  addi.w  #$24,d0 */
                                SETW(d1, d1 + 0x64);                                 /* 005442  addi.w  #$64,d1 */
                                /* 005446  addi.w  #$2,d1 */
                                {
                                    uint32_t s = 2;
                                    uint32_t d = (d1 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d1, r);
                                    fx = (r < s);
                                }
                                SETW(d2, 0xC8);                                      /* 00544A  move.w  #$c8,d2 */
                                SETW(d3, 0);                                         /* 00544E  move.w  #$0,d3 */
                                SETW(d4, 8);                                         /* 005452  move.w  #$8,d4 */
                                SETW(d5, 0xFFFF);                                    /* 005456  move.w  #$ffff,d5 */
                                /* 00545A  tst.w  $c(a4) */
                                /* 00545E  beq  $5466 */
                                if (rd16(a4 + 0xC) != 0) {
L_005460:
                                    SETW(d4, ~(d4 & 0xFFFF));                            /* 005460  not.w  d4 */
                                    /* 005462  subi.w  #$4,d1 */
                                    {
                                        uint32_t s = 4;
                                        uint32_t d = (d1 & 0xFFFF);
                                        uint32_t r = (d - s) & 0xFFFF;
                                        SETW(d1, r);
                                        fx = (s > d);
                                    }
                                }
                                CALL(sub_00B6A6, 0x546AU);                           /* 005466  bsr  $b6a6 */
                            }
                        }
                    }
                }
            }
        }
L_00546A:
        a4 = a4 + 0xE;                                       /* 00546A  lea  $e(a4),a4 */
        /* 00546E  dbra  d7,$533c */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_005472:
    RTS();                                               /* 005472  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x533CU: goto L_00533C;
        case 0x5342U: goto L_005342;
        case 0x534AU: goto L_00534A;
        case 0x5352U: goto L_005352;
        case 0x5358U: goto L_005358;
        case 0x5360U: goto L_005360;
        case 0x5366U: goto L_005366;
        case 0x536EU: goto L_00536E;
        case 0x5372U: goto L_005372;
        case 0x53A6U: goto L_0053A6;
        case 0x53AAU: goto L_0053AA;
        case 0x53B2U: goto L_0053B2;
        case 0x53B8U: goto L_0053B8;
        case 0x53C0U: goto L_0053C0;
        case 0x53C4U: goto L_0053C4;
        case 0x53F4U: goto L_0053F4;
        case 0x53F8U: goto L_0053F8;
        case 0x5402U: goto L_005402;
        case 0x5406U: goto L_005406;
        case 0x540CU: goto L_00540C;
        case 0x5412U: goto L_005412;
        case 0x541AU: goto L_00541A;
        case 0x5426U: goto L_005426;
        case 0x542AU: goto L_00542A;
        case 0x5436U: goto L_005436;
        case 0x5460U: goto L_005460;
        case 0x546AU: goto L_00546A;
        case 0x5472U: goto L_005472;
        }
    }
}

/* 0054AA  game routine */
void sub_0054AA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1B5C;                                         /* 0054AA  lea  $1b5c(a5),a4 */
    d7 = 0xF;                                            /* 0054AE  moveq  #15,d7 */
    do {
        BUDGET_CHECK(0x54B0U);
L_0054B0:
        /* 0054B0  tst.w  (a4) */
        /* 0054B2  beq  $550a */
        if (rd16(a4) != 0) {
L_0054B4:
            a1 = 0x46336;                                        /* 0054B4  lea  $46336.l,a1 */
            SETW(d0, rd16(a4 + 0x6));                            /* 0054BA  move.w  $6(a4),d0 */
            SETW(d0, d0 - 0xF0);                                 /* 0054BE  subi.w  #$f0,d0 */
            d0 = (d0 & 0xFFFF) * 0x80;                           /* 0054C2  mulu.w  #$80,d0 */
            a1 = a1 + SX16(d0);                                  /* 0054C6  lea  $0(a1,d0.w),a1 */
            a0 = rd32(V_DRAW_BUFFER);                            /* 0054CA  movea.l  $28e(a5),a0 */
            a0 = a0 + 0x20;                                      /* 0054CE  lea  $20(a0),a0 */
            SETW(d0, rd16(a4 + 0x4));                            /* 0054D2  move.w  $4(a4),d0 */
            SETW(d0, (d0 & 0xFFFF) << 5);                        /* 0054D6  lsl.w  #5,d0 */
            a0 = a0 + SX16(d0);                                  /* 0054D8  lea  $0(a0,d0.w),a0 */
            SETW(d0, (d0 & 0xFFFF) << 2);                        /* 0054DC  lsl.w  #2,d0 */
            a0 = a0 + SX16(d0);                                  /* 0054DE  lea  $0(a0,d0.w),a0 */
            SETW(d0, rd16(a4 + 0x2));                            /* 0054E2  move.w  $2(a4),d0 */
            SETW(d0, d0 - 0x40);                                 /* 0054E6  subi.w  #$40,d0 */
            SETW(d0, shift_op(0, 0, (d0 & 0xFFFF), 1, 16));      /* 0054EA  asr.w  #1,d0 */
            SETW(d0, d0 & 0xFFF8);                               /* 0054EC  andi.w  #$fff8,d0 */
            a0 = a0 + SX16(d0);                                  /* 0054F0  lea  $0(a0,d0.w),a0 */
            wr16(V_0212, 0x10);                                  /* 0054F4  move.w  #$10,$212(a5) */
            SETW(d0, rd16(a4 + 0x2));                            /* 0054FA  move.w  $2(a4),d0 */
            /* 0054FE  movem.l  a4/d7,-(a7) */
            PUSH32(a4);
            PUSH32(d7);
            CALL(sub_005514, 0x5506U);                           /* 005502  bsr  $5514 */
L_005506:
            /* 005506  movem.l  (a7)+,d7/a4 */
            d7 = POP32();
            a4 = POP32();
        }
L_00550A:
        a4 = a4 + 0xE;                                       /* 00550A  lea  $e(a4),a4 */
        /* 00550E  dbra  d7,$54b0 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_005512:
    RTS();                                               /* 005512  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x54B0U: goto L_0054B0;
        case 0x54B4U: goto L_0054B4;
        case 0x5506U: goto L_005506;
        case 0x550AU: goto L_00550A;
        case 0x5512U: goto L_005512;
        }
    }
}

/* 005514  game routine */
void sub_005514(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = a0 + 0xE;                                       /* 005514  lea  $e(a0),a0 */
    wr8(V_020C, 0xFF);                                   /* 005518  st  $20c(a5) */
    wr8(V_020E, 0xFF);                                   /* 00551C  st  $20e(a5) */
    /* 005520  cmp.w  #$f0,d0 */
    /* 005524  bls  $5532 */
    if ((d0 & 0xFFFF) > 0xF0) {
L_005526:
        wr16(V_020E, 0);                                     /* 005526  clr.w  $20e(a5) */
        /* 00552A  cmp.w  #$100,d0 */
        /* 00552E  bcc  $567e */
        if ((d0 & 0xFFFF) >= 0x100) {
            goto L_00567E;
        }
    }
L_005532:
    /* 005532  cmp.w  #$40,d0 */
    /* 005536  bcc  $5544 */
    if ((d0 & 0xFFFF) < 0x40) {
L_005538:
        wr16(V_020C, 0);                                     /* 005538  clr.w  $20c(a5) */
        /* 00553C  cmp.w  #$30,d0 */
        /* 005540  bls  $567e */
        if ((d0 & 0xFFFF) <= 0x30) {
            goto L_00567E;
        }
    }
L_005544:
    /* 005544  andi.w  #$f,d0 */
    {
        uint32_t s = 0xF;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d & s) & 0xFFFF;
        SETW(d0, r);
        fz = (r == 0);
    }
    /* 005548  beq  $5652 */
    if (COND(6)) {
L_00554C:
        /* 00554C  cmp.w  #$8,d0 */
        /* 005550  bls  $55d2 */
        if ((d0 & 0xFFFF) > 8) {
L_005554:
            /* 005554  neg.w  d0 */
            { uint32_t d, r, ea; (void)ea;
            d = (d0 & 0xFFFFU);
            r = (0 - d) & 0xFFFFU;
            d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
            FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
            SETW(d0, d0 + 0x10);                                 /* 005556  addi.w  #$10,d0 */
            SETW(d6, (d0 & 0xFFFF));                             /* 00555A  move.w  d0,d6 */
            d7 = 0x10;                                           /* 00555C  moveq  #16,d7 */
            SETW(d7, d7 - (d6 & 0xFFFF));                        /* 00555E  sub.w  d6,d7 */
            do {
                BUDGET_CHECK(0x5560U);
L_005560:
                d0 = 0;                                              /* 005560  moveq  #0,d0 */
                d1 = 0;                                              /* 005562  moveq  #0,d1 */
                d2 = 0;                                              /* 005564  moveq  #0,d2 */
                d3 = 0;                                              /* 005566  moveq  #0,d3 */
                /* 005568  move.w  (a1)+,d0 */
                SETW(d0, rd16(a1));
                a1 += 2;
                /* 00556A  move.w  (a1)+,d1 */
                SETW(d1, rd16(a1));
                a1 += 2;
                /* 00556C  move.w  (a1)+,d2 */
                SETW(d2, rd16(a1));
                a1 += 2;
                /* 00556E  move.w  (a1)+,d3 */
                SETW(d3, rd16(a1));
                a1 += 2;
                d0 = shift_op(1, 1, d0, (d6 & 63), 32);              /* 005570  lsl.l  d6,d0 */
                d1 = shift_op(1, 1, d1, (d6 & 63), 32);              /* 005572  lsl.l  d6,d1 */
                d2 = shift_op(1, 1, d2, (d6 & 63), 32);              /* 005574  lsl.l  d6,d2 */
                d3 = shift_op(1, 1, d3, (d6 & 63), 32);              /* 005576  lsl.l  d6,d3 */
                d5 = d0;                                             /* 005578  move.l  d0,d5 */
                d5 |= d1;                                            /* 00557A  or.l  d1,d5 */
                d5 |= d2;                                            /* 00557C  or.l  d2,d5 */
                d5 |= d3;                                            /* 00557E  or.l  d3,d5 */
                d5 = ~d5;                                            /* 005580  not.l  d5 */
                /* 005582  tst.w  $20e(a5) */
                /* 005586  bne  $558e */
                if (rd16(V_020E) == 0) {
L_005588:
                    a0 = a0 - 0x6;                                       /* 005588  lea  -$6(a0),a0 */
                    /* 00558C  bra  $559e */
                } else {
L_00558E:
                    wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 00558E  and.w  d5,(a0) */
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005590  or.w  d3,(a0) */
                    /* 005592  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005594  or.w  d2,(a0) */
                    /* 005596  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005598  or.w  d1,(a0) */
                    /* 00559A  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 00559C  or.w  d0,(a0) */
                }
L_00559E:
                /* 00559E  tst.w  $20c(a5) */
                /* 0055A2  bne  $55aa */
                if (rd16(V_020C) == 0) {
L_0055A4:
                    a0 = a0 - 0x8;                                       /* 0055A4  lea  -$8(a0),a0 */
                    /* 0055A8  bra  $55c4 */
                } else {
L_0055AA:
                    d0 = (d0 << 16) | (d0 >> 16);                        /* 0055AA  swap  d0 */
                    d1 = (d1 << 16) | (d1 >> 16);                        /* 0055AC  swap  d1 */
                    d2 = (d2 << 16) | (d2 >> 16);                        /* 0055AE  swap  d2 */
                    d3 = (d3 << 16) | (d3 >> 16);                        /* 0055B0  swap  d3 */
                    d5 = (d5 << 16) | (d5 >> 16);                        /* 0055B2  swap  d5 */
                    /* 0055B4  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0055B6  or.w  d3,(a0) */
                    /* 0055B8  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0055BA  or.w  d2,(a0) */
                    /* 0055BC  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0055BE  or.w  d1,(a0) */
                    /* 0055C0  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0055C2  or.w  d0,(a0) */
                }
L_0055C4:
                a0 = a0 + 0xAE;                                      /* 0055C4  lea  $ae(a0),a0 */
                /* 0055C8  subq.w  #1,$212(a5) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(V_0212);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(V_0212, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 0055CC  bne  $5560 */
            } while (COND(6));
L_0055CE:
            /* 0055CE  bra  $567e */
            goto L_00567E;
        }
L_0055D2:
        SETW(d6, (d0 & 0xFFFF));                             /* 0055D2  move.w  d0,d6 */
        d7 = 0x10;                                           /* 0055D4  moveq  #16,d7 */
        SETW(d7, d7 - (d6 & 0xFFFF));                        /* 0055D6  sub.w  d6,d7 */
        do {
            BUDGET_CHECK(0x55D8U);
L_0055D8:
            d0 = 0;                                              /* 0055D8  moveq  #0,d0 */
            d1 = 0;                                              /* 0055DA  moveq  #0,d1 */
            d2 = 0;                                              /* 0055DC  moveq  #0,d2 */
            d3 = 0;                                              /* 0055DE  moveq  #0,d3 */
            /* 0055E0  move.w  (a1)+,d0 */
            SETW(d0, rd16(a1));
            a1 += 2;
            /* 0055E2  move.w  (a1)+,d1 */
            SETW(d1, rd16(a1));
            a1 += 2;
            /* 0055E4  move.w  (a1)+,d2 */
            SETW(d2, rd16(a1));
            a1 += 2;
            /* 0055E6  move.w  (a1)+,d3 */
            SETW(d3, rd16(a1));
            a1 += 2;
            d0 = (d0 << 16) | (d0 >> 16);                        /* 0055E8  swap  d0 */
            d1 = (d1 << 16) | (d1 >> 16);                        /* 0055EA  swap  d1 */
            d2 = (d2 << 16) | (d2 >> 16);                        /* 0055EC  swap  d2 */
            d3 = (d3 << 16) | (d3 >> 16);                        /* 0055EE  swap  d3 */
            d0 = shift_op(1, 0, d0, (d6 & 63), 32);              /* 0055F0  lsr.l  d6,d0 */
            d1 = shift_op(1, 0, d1, (d6 & 63), 32);              /* 0055F2  lsr.l  d6,d1 */
            d2 = shift_op(1, 0, d2, (d6 & 63), 32);              /* 0055F4  lsr.l  d6,d2 */
            d3 = shift_op(1, 0, d3, (d6 & 63), 32);              /* 0055F6  lsr.l  d6,d3 */
            d5 = d0;                                             /* 0055F8  move.l  d0,d5 */
            d5 |= d1;                                            /* 0055FA  or.l  d1,d5 */
            d5 |= d2;                                            /* 0055FC  or.l  d2,d5 */
            d5 |= d3;                                            /* 0055FE  or.l  d3,d5 */
            d5 = ~d5;                                            /* 005600  not.l  d5 */
            /* 005602  tst.w  $20e(a5) */
            /* 005606  bne  $560e */
            if (rd16(V_020E) == 0) {
L_005608:
                a0 = a0 - 0x6;                                       /* 005608  lea  -$6(a0),a0 */
                /* 00560C  bra  $561e */
            } else {
L_00560E:
                wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 00560E  and.w  d5,(a0) */
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005610  or.w  d3,(a0) */
                /* 005612  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005614  or.w  d2,(a0) */
                /* 005616  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005618  or.w  d1,(a0) */
                /* 00561A  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 00561C  or.w  d0,(a0) */
            }
L_00561E:
            /* 00561E  tst.w  $20c(a5) */
            /* 005622  bne  $562a */
            if (rd16(V_020C) == 0) {
L_005624:
                a0 = a0 - 0x8;                                       /* 005624  lea  -$8(a0),a0 */
                /* 005628  bra  $5644 */
            } else {
L_00562A:
                d0 = (d0 << 16) | (d0 >> 16);                        /* 00562A  swap  d0 */
                d1 = (d1 << 16) | (d1 >> 16);                        /* 00562C  swap  d1 */
                d2 = (d2 << 16) | (d2 >> 16);                        /* 00562E  swap  d2 */
                d3 = (d3 << 16) | (d3 >> 16);                        /* 005630  swap  d3 */
                d5 = (d5 << 16) | (d5 >> 16);                        /* 005632  swap  d5 */
                /* 005634  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005636  or.w  d3,(a0) */
                /* 005638  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 00563A  or.w  d2,(a0) */
                /* 00563C  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00563E  or.w  d1,(a0) */
                /* 005640  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005642  or.w  d0,(a0) */
            }
L_005644:
            a0 = a0 + 0xAE;                                      /* 005644  lea  $ae(a0),a0 */
            /* 005648  subq.w  #1,$212(a5) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(V_0212);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(V_0212, r);
                fz = (r == 0);
                fx = (s > d);
            }
            /* 00564C  bne  $55d8 */
        } while (COND(6));
L_00564E:
        /* 00564E  bra  $567e */
    } else {
L_005652:
        a0 = a0 - 0x8;                                       /* 005652  lea  -$8(a0),a0 */
        do {
            BUDGET_CHECK(0x5656U);
L_005656:
            /* 005656  movem.w  (a1)+,d0-d3 */
            { uint32_t ea;
            ea = a1;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            d3 = SX16(rd16(ea));
            ea += 2;
            a1 = ea;
            }
            SETW(d6, (d0 & 0xFFFF));                             /* 00565A  move.w  d0,d6 */
            SETW(d6, d6 | (d1 & 0xFFFF));                        /* 00565C  or.w  d1,d6 */
            SETW(d6, d6 | (d2 & 0xFFFF));                        /* 00565E  or.w  d2,d6 */
            SETW(d6, d6 | (d3 & 0xFFFF));                        /* 005660  or.w  d3,d6 */
            SETW(d6, ~(d6 & 0xFFFF));                            /* 005662  not.w  d6 */
            wr16(a0, rd16(a0) & (d6 & 0xFFFF));                  /* 005664  and.w  d6,(a0) */
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005666  or.w  d3,(a0) */
            /* 005668  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 00566A  or.w  d2,(a0) */
            /* 00566C  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00566E  or.w  d1,(a0) */
            /* 005670  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005672  or.w  d0,(a0) */
            a0 = a0 + 0xA6;                                      /* 005674  lea  $a6(a0),a0 */
            /* 005678  subq.w  #1,$212(a5) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(V_0212);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(V_0212, r);
                fz = (r == 0);
                fx = (s > d);
            }
            /* 00567C  bne  $5656 */
        } while (COND(6));
    }
L_00567E:
    RTS();                                               /* 00567E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5526U: goto L_005526;
        case 0x5532U: goto L_005532;
        case 0x5538U: goto L_005538;
        case 0x5544U: goto L_005544;
        case 0x554CU: goto L_00554C;
        case 0x5554U: goto L_005554;
        case 0x5560U: goto L_005560;
        case 0x5588U: goto L_005588;
        case 0x558EU: goto L_00558E;
        case 0x559EU: goto L_00559E;
        case 0x55A4U: goto L_0055A4;
        case 0x55AAU: goto L_0055AA;
        case 0x55C4U: goto L_0055C4;
        case 0x55CEU: goto L_0055CE;
        case 0x55D2U: goto L_0055D2;
        case 0x55D8U: goto L_0055D8;
        case 0x5608U: goto L_005608;
        case 0x560EU: goto L_00560E;
        case 0x561EU: goto L_00561E;
        case 0x5624U: goto L_005624;
        case 0x562AU: goto L_00562A;
        case 0x5644U: goto L_005644;
        case 0x564EU: goto L_00564E;
        case 0x5652U: goto L_005652;
        case 0x5656U: goto L_005656;
        case 0x567EU: goto L_00567E;
        }
    }
}

/* 005680  game routine */
void sub_005680(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d6 = 0xFFFFFFFF;                                     /* 005680  moveq  #-1,d6 */
    a3 = V_02DA;                                         /* 005682  lea  $2da(a5),a3 */
    /* 005686  tst.w  (a3) */
    /* 005688  beq  $56a2 */
    if (rd16(a3) != 0) {
L_00568A:
        SETW(d0, rd16(a4 + 0x2));                            /* 00568A  move.w  $2(a4),d0 */
        SETW(d1, rd16(a3 + 0x2));                            /* 00568E  move.w  $2(a3),d1 */
        SETW(d0, d0 - (d1 & 0xFFFF));                        /* 005692  sub.w  d1,d0 */
        /* 005694  addi.w  #$1e,d0 */
        {
            uint32_t s = 0x1E;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        /* 005698  cmp.w  #$3c,d0 */
        /* 00569C  bhi  $56a2 */
        if ((d0 & 0xFFFF) > 0x3C) {
            goto L_0056A2;
        }
L_00569E:
        d6 = 0;                                              /* 00569E  moveq  #0,d6 */
        /* 0056A0  bra  $56c0 */
    } else {
L_0056A2:
        a3 = V_0376;                                         /* 0056A2  lea  $376(a5),a3 */
        /* 0056A6  tst.w  (a3) */
        /* 0056A8  beq  $5702 */
        if (rd16(a3) == 0) {
            goto L_005702;
        }
        SETW(d0, rd16(a4 + 0x2));                            /* 0056AA  move.w  $2(a4),d0 */
        SETW(d1, rd16(a3 + 0x2));                            /* 0056AE  move.w  $2(a3),d1 */
        SETW(d0, d0 - (d1 & 0xFFFF));                        /* 0056B2  sub.w  d1,d0 */
        /* 0056B4  addi.w  #$1e,d0 */
        {
            uint32_t s = 0x1E;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        /* 0056B8  cmp.w  #$3c,d0 */
        /* 0056BC  bhi  $5702 */
        if ((d0 & 0xFFFF) > 0x3C) {
            goto L_005702;
        }
        d6 = 0;                                              /* 0056BE  moveq  #0,d6 */
    }
L_0056C0:
    a3 = V_02DA;                                         /* 0056C0  lea  $2da(a5),a3 */
    /* 0056C4  tst.w  (a3) */
    /* 0056C6  beq  $56e0 */
    if (rd16(a3) != 0) {
L_0056C8:
        SETW(d0, rd16(a4 + 0x4));                            /* 0056C8  move.w  $4(a4),d0 */
        SETW(d0, d0 + 7);                                    /* 0056CC  addq.w  #7,d0 */
        SETW(d1, rd16(a3 + 0x4));                            /* 0056CE  move.w  $4(a3),d1 */
        SETW(d1, d1 + 0x10);                                 /* 0056D2  addi.w  #$10,d1 */
        /* 0056D6  sub.w  d1,d0 */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d0, r);
            fn = !!(r & 0x8000);
            fx = (s > d);
        }
        /* 0056D8  bpl  $56e0 */
        if (COND(10)) {
            goto L_0056E0;
        }
        wr16(a4 + 0xC, 0);                                   /* 0056DA  clr.w  $c(a4) */
        /* 0056DE  bra  $5702 */
    } else {
L_0056E0:
        a3 = V_0376;                                         /* 0056E0  lea  $376(a5),a3 */
        /* 0056E4  tst.w  (a3) */
        /* 0056E6  beq  $56fc */
        if (rd16(a3) != 0) {
            SETW(d0, rd16(a4 + 0x4));                            /* 0056E8  move.w  $4(a4),d0 */
            SETW(d1, rd16(a3 + 0x4));                            /* 0056EC  move.w  $4(a3),d1 */
            /* 0056F0  sub.w  d1,d0 */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fn = !!(r & 0x8000);
                fx = (s > d);
            }
            /* 0056F2  bpl  $56fc */
            if (COND(10)) {
                goto L_0056FC;
            }
            wr16(a4 + 0xC, 0);                                   /* 0056F4  move.w  #$0,$c(a4) */
            /* 0056FA  bra  $5702 */
        } else {
L_0056FC:
            wr16(a4 + 0xC, 1);                                   /* 0056FC  move.w  #$1,$c(a4) */
        }
    }
L_005702:
    RTS();                                               /* 005702  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x568AU: goto L_00568A;
        case 0x569EU: goto L_00569E;
        case 0x56A2U: goto L_0056A2;
        case 0x56C0U: goto L_0056C0;
        case 0x56C8U: goto L_0056C8;
        case 0x56E0U: goto L_0056E0;
        case 0x56FCU: goto L_0056FC;
        case 0x5702U: goto L_005702;
        }
    }
}

/* 005904  game routine */
void sub_005904(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 005904  movem.l  a6/d7,-(a7) */
    PUSH32(a6);
    PUSH32(d7);
    SETW(d7, 0x13);                                      /* 005908  move.w  #$13,d7 */
    a6 = V_135A;                                         /* 00590C  lea  $135a(a5),a6 */
    do {
        BUDGET_CHECK(0x5910U);
L_005910:
        /* 005910  tst.w  $4(a6) */
        /* 005914  bne  $5938 */
        if (rd16(a6 + 0x4) == 0) {
            wr16(a6, (d0 & 0xFFFF));                             /* 005916  move.w  d0,$0(a6) */
            wr16(a6 + 0x2, (d1 & 0xFFFF));                       /* 00591A  move.w  d1,$2(a6) */
            wr16(a6 + 0xA, (d2 & 0xFFFF));                       /* 00591E  move.w  d2,$a(a6) */
            wr16(a6 + 0xC, (d3 & 0xFFFF));                       /* 005922  move.w  d3,$c(a6) */
            wr16(a6 + 0x6, (d4 & 0xFFFF));                       /* 005926  move.w  d4,$6(a6) */
            wr16(a6 + 0x4, 0xFFFF);                              /* 00592A  move.w  #$ffff,$4(a6) */
            wr16(a6 + 0x8, 0xFFFF);                              /* 005930  move.w  #$ffff,$8(a6) */
            /* 005936  bra  $5940 */
            goto L_005940;
        }
        a6 = a6 + 0xE;                                       /* 005938  lea  $e(a6),a6 */
        /* 00593C  dbra  d7,$5910 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_005940:
    /* 005940  movem.l  (a7)+,d7/a6 */
    d7 = POP32();
    a6 = POP32();
    RTS();                                               /* 005944  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5910U: goto L_005910;
        }
    }
}

/* 005946  game routine */
void sub_005946(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, rd16(V_02DE));                              /* 005946  move.w  $2de(a5),d0 */
    /* 00594A  cmp.w  $37a(a5),d0 */
    /* 00594E  bhi  $5966 */
    if ((d0 & 0xFFFF) <= rd16(V_037A)) {
L_005950:
        a4 = V_02DA;                                         /* 005950  lea  $2da(a5),a4 */
        wr16(V_027A, 0);                                     /* 005954  clr.w  $27a(a5) */
        CALL(draw_player, 0x595AU);                          /* 005958  bsr  $597a */
L_00595A:
        /* 00595A  lea  $376(a5),a4 */
        HOOK(5, 0x595AU);
        a4 = V_0376;
        wr16(V_027A, 1);                                     /* 00595E  move.w  #$1,$27a(a5) */
        /* 005964  bra  $597a */
        { draw_player(); return; }
    }
L_005966:
    a4 = V_0376;                                         /* 005966  lea  $376(a5),a4 */
    wr16(V_027A, 1);                                     /* 00596A  move.w  #$1,$27a(a5) */
    CALL(draw_player, 0x5972U);                          /* 005970  bsr  $597a */
L_005972:
    /* 005972  lea  $2da(a5),a4 */
    HOOK(6, 0x5972U);
    a4 = V_02DA;
    /* 005976  clr.w  $27a(a5) */
    wr16(V_027A, 0);
    { draw_player(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5950U: goto L_005950;
        case 0x595AU: goto L_00595A;
        case 0x5966U: goto L_005966;
        case 0x5972U: goto L_005972;
        }
    }
}

/* 00597A  Draws a player */
void draw_player(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00597A  tst.w  (a4) */
    /* 00597C  beq  $7de2 */
    HOOK(7, 0x597AU);
    if (rd16(a4) == 0) {
        { sub_007DE2(); return; }
    }
L_005980:
    /* 005980  tst.w  $24(a4) */
    /* 005984  beq  $598e */
    if (rd16(a4 + 0x24) != 0) {
L_005986:
        wr16(a4 + 0x24, rd16(a4 + 0x24) - 1);                /* 005986  subq.w  #1,$24(a4) */
        /* 00598A  bra  $7de2 */
        { sub_007DE2(); return; }
    }
L_00598E:
    /* 00598E  tst.w  $2a(a4) */
    /* 005992  beq  $59a4 */
    if (rd16(a4 + 0x2A) != 0) {
L_005994:
        wr16(a4 + 0x2A, rd16(a4 + 0x2A) - 1);                /* 005994  subq.w  #1,$2a(a4) */
        SETW(d0, rd16(a4 + 0x2A));                           /* 005998  move.w  $2a(a4),d0 */
        fz = !(d0 & 0x2);                                    /* 00599C  btst  #$1,d0 */
        /* 0059A0  beq  $7de2 */
        if (COND(7)) { sub_007DE2(); return; }
    }
L_0059A4:
    a0 = rd32(V_DRAW_BUFFER);                            /* 0059A4  movea.l  $28e(a5),a0 */
    SETW(d0, rd16(a4 + 0x4));                            /* 0059A8  move.w  $4(a4),d0 */
    d0 = (d0 & 0xFFFF) * 0xA0;                           /* 0059AC  mulu.w  #$a0,d0 */
    a0 = a0 + SX16(d0);                                  /* 0059B0  lea  $0(a0,d0.w),a0 */
    SETW(d0, rd16(a4 + 0x2));                            /* 0059B4  move.w  $2(a4),d0 */
    SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 0059B8  lsr.w  #1,d0 */
    SETW(d0, d0 & 0xFFF8);                               /* 0059BA  andi.w  #$fff8,d0 */
    a0 = a0 + SX16(d0);                                  /* 0059BE  lea  $0(a0,d0.w),a0 */
    a1 = 0x2F91C;                                        /* 0059C2  lea  $2f91c.l,a1 */
    SETW(d0, rd16(a4 + 0x6));                            /* 0059C8  move.w  $6(a4),d0 */
    d0 = (d0 & 0xFFFF) * 0x190;                          /* 0059CC  mulu.w  #$190,d0 */
    a1 += d0;                                            /* 0059D0  adda.l  d0,a1 */
    wr16(V_02D4, 0x19);                                  /* 0059D2  move.w  #$19,$2d4(a5) */
    a0 = a0 + 0x16;                                      /* 0059D8  lea  $16(a0),a0 */
    SETW(d0, rd16(a4 + 0x2));                            /* 0059DC  move.w  $2(a4),d0 */
    /* 0059E0  andi.w  #$f,d0 */
    {
        uint32_t s = 0xF;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d & s) & 0xFFFF;
        SETW(d0, r);
        fz = (r == 0);
    }
    /* 0059E4  beq  $5aea */
    if (COND(6)) {
L_0059E8:
        /* 0059E8  cmp.w  #$8,d0 */
        /* 0059EC  bls  $5a70 */
        if ((d0 & 0xFFFF) > 8) {
L_0059F0:
            /* 0059F0  neg.w  d0 */
            { uint32_t d, r, ea; (void)ea;
            d = (d0 & 0xFFFFU);
            r = (0 - d) & 0xFFFFU;
            d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
            FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
            SETW(d0, d0 + 0x10);                                 /* 0059F2  addi.w  #$10,d0 */
            SETW(d6, (d0 & 0xFFFF));                             /* 0059F6  move.w  d0,d6 */
            d7 = 0x10;                                           /* 0059F8  moveq  #16,d7 */
            SETW(d7, d7 - (d6 & 0xFFFF));                        /* 0059FA  sub.w  d6,d7 */
            do {
                BUDGET_CHECK(0x59FCU);
L_0059FC:
                /* 0059FC  movem.l  (a1),d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                }
                SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), (d6 & 63), 16)); /* 005A00  lsl.w  d6,d0 */
                SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), (d6 & 63), 16)); /* 005A02  lsl.w  d6,d1 */
                SETW(d2, shift_op(1, 1, (d2 & 0xFFFF), (d6 & 63), 16)); /* 005A04  lsl.w  d6,d2 */
                SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), (d6 & 63), 16)); /* 005A06  lsl.w  d6,d3 */
                SETW(d5, (d0 & 0xFFFF));                             /* 005A08  move.w  d0,d5 */
                SETW(d5, d5 | (d1 & 0xFFFF));                        /* 005A0A  or.w  d1,d5 */
                SETW(d5, d5 | (d2 & 0xFFFF));                        /* 005A0C  or.w  d2,d5 */
                SETW(d5, d5 | (d3 & 0xFFFF));                        /* 005A0E  or.w  d3,d5 */
                SETW(d5, ~(d5 & 0xFFFF));                            /* 005A10  not.w  d5 */
                wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 005A12  and.w  d5,(a0) */
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005A14  or.w  d3,(a0) */
                /* 005A16  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005A18  or.w  d2,(a0) */
                /* 005A1A  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005A1C  or.w  d1,(a0) */
                /* 005A1E  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005A20  or.w  d0,(a0) */
                /* 005A22  movem.l  (a1)+,d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                a1 = ea;
                }
                d0 = shift_op(1, 0, d0, (d7 & 63), 32);              /* 005A26  lsr.l  d7,d0 */
                d1 = shift_op(1, 0, d1, (d7 & 63), 32);              /* 005A28  lsr.l  d7,d1 */
                d2 = shift_op(1, 0, d2, (d7 & 63), 32);              /* 005A2A  lsr.l  d7,d2 */
                d3 = shift_op(1, 0, d3, (d7 & 63), 32);              /* 005A2C  lsr.l  d7,d3 */
                d5 = d0;                                             /* 005A2E  move.l  d0,d5 */
                d5 |= d1;                                            /* 005A30  or.l  d1,d5 */
                d5 |= d2;                                            /* 005A32  or.l  d2,d5 */
                d5 |= d3;                                            /* 005A34  or.l  d3,d5 */
                d5 = ~d5;                                            /* 005A36  not.l  d5 */
                /* 005A38  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005A3A  or.w  d3,(a0) */
                /* 005A3C  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005A3E  or.w  d2,(a0) */
                /* 005A40  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005A42  or.w  d1,(a0) */
                /* 005A44  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005A46  or.w  d0,(a0) */
                d0 = (d0 << 16) | (d0 >> 16);                        /* 005A48  swap  d0 */
                d1 = (d1 << 16) | (d1 >> 16);                        /* 005A4A  swap  d1 */
                d2 = (d2 << 16) | (d2 >> 16);                        /* 005A4C  swap  d2 */
                d3 = (d3 << 16) | (d3 >> 16);                        /* 005A4E  swap  d3 */
                d5 = (d5 << 16) | (d5 >> 16);                        /* 005A50  swap  d5 */
                /* 005A52  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005A54  or.w  d3,(a0) */
                /* 005A56  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005A58  or.w  d2,(a0) */
                /* 005A5A  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005A5C  or.w  d1,(a0) */
                /* 005A5E  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005A60  or.w  d0,(a0) */
                a0 = a0 + 0xB6;                                      /* 005A62  lea  $b6(a0),a0 */
                /* 005A66  subq.w  #1,$2d4(a5) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(V_02D4);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(V_02D4, r);
                    fz = (r == 0);
                }
                /* 005A6A  bne  $59fc */
            } while (COND(6));
L_005A6C:
            /* 005A6C  bra  $5b30 */
            goto L_005B30;
        }
L_005A70:
        SETW(d6, (d0 & 0xFFFF));                             /* 005A70  move.w  d0,d6 */
        d7 = 0x10;                                           /* 005A72  moveq  #16,d7 */
        SETW(d7, d7 - (d6 & 0xFFFF));                        /* 005A74  sub.w  d6,d7 */
        do {
            BUDGET_CHECK(0x5A76U);
L_005A76:
            /* 005A76  movem.l  (a1),d0-d3 */
            { uint32_t ea;
            ea = a1;
            d0 = rd32(ea);
            ea += 4;
            d1 = rd32(ea);
            ea += 4;
            d2 = rd32(ea);
            ea += 4;
            d3 = rd32(ea);
            ea += 4;
            }
            d0 = shift_op(1, 1, d0, (d7 & 63), 32);              /* 005A7A  lsl.l  d7,d0 */
            d1 = shift_op(1, 1, d1, (d7 & 63), 32);              /* 005A7C  lsl.l  d7,d1 */
            d2 = shift_op(1, 1, d2, (d7 & 63), 32);              /* 005A7E  lsl.l  d7,d2 */
            d3 = shift_op(1, 1, d3, (d7 & 63), 32);              /* 005A80  lsl.l  d7,d3 */
            SETW(d5, (d0 & 0xFFFF));                             /* 005A82  move.w  d0,d5 */
            SETW(d5, d5 | (d1 & 0xFFFF));                        /* 005A84  or.w  d1,d5 */
            SETW(d5, d5 | (d2 & 0xFFFF));                        /* 005A86  or.w  d2,d5 */
            SETW(d5, d5 | (d3 & 0xFFFF));                        /* 005A88  or.w  d3,d5 */
            SETW(d5, ~(d5 & 0xFFFF));                            /* 005A8A  not.w  d5 */
            wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 005A8C  and.w  d5,(a0) */
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005A8E  or.w  d3,(a0) */
            /* 005A90  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005A92  or.w  d2,(a0) */
            /* 005A94  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005A96  or.w  d1,(a0) */
            /* 005A98  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005A9A  or.w  d0,(a0) */
            /* 005A9C  movem.l  (a1)+,d0-d3 */
            { uint32_t ea;
            ea = a1;
            d0 = rd32(ea);
            ea += 4;
            d1 = rd32(ea);
            ea += 4;
            d2 = rd32(ea);
            ea += 4;
            d3 = rd32(ea);
            ea += 4;
            a1 = ea;
            }
            d0 = shift_op(1, 0, d0, (d6 & 63), 32);              /* 005AA0  lsr.l  d6,d0 */
            d1 = shift_op(1, 0, d1, (d6 & 63), 32);              /* 005AA2  lsr.l  d6,d1 */
            d2 = shift_op(1, 0, d2, (d6 & 63), 32);              /* 005AA4  lsr.l  d6,d2 */
            d3 = shift_op(1, 0, d3, (d6 & 63), 32);              /* 005AA6  lsr.l  d6,d3 */
            d5 = d0;                                             /* 005AA8  move.l  d0,d5 */
            d5 |= d1;                                            /* 005AAA  or.l  d1,d5 */
            d5 |= d2;                                            /* 005AAC  or.l  d2,d5 */
            d5 |= d3;                                            /* 005AAE  or.l  d3,d5 */
            d5 = ~d5;                                            /* 005AB0  not.l  d5 */
            /* 005AB2  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005AB4  or.w  d3,(a0) */
            /* 005AB6  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005AB8  or.w  d2,(a0) */
            /* 005ABA  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005ABC  or.w  d1,(a0) */
            /* 005ABE  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005AC0  or.w  d0,(a0) */
            d0 = (d0 << 16) | (d0 >> 16);                        /* 005AC2  swap  d0 */
            d1 = (d1 << 16) | (d1 >> 16);                        /* 005AC4  swap  d1 */
            d2 = (d2 << 16) | (d2 >> 16);                        /* 005AC6  swap  d2 */
            d3 = (d3 << 16) | (d3 >> 16);                        /* 005AC8  swap  d3 */
            d5 = (d5 << 16) | (d5 >> 16);                        /* 005ACA  swap  d5 */
            /* 005ACC  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005ACE  or.w  d3,(a0) */
            /* 005AD0  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005AD2  or.w  d2,(a0) */
            /* 005AD4  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005AD6  or.w  d1,(a0) */
            /* 005AD8  and.w  d5,-(a0) */
            {
                uint32_t s = (d5 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005ADA  or.w  d0,(a0) */
            a0 = a0 + 0xB6;                                      /* 005ADC  lea  $b6(a0),a0 */
            /* 005AE0  subq.w  #1,$2d4(a5) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(V_02D4);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(V_02D4, r);
                fz = (r == 0);
            }
            /* 005AE4  bne  $5a76 */
        } while (COND(6));
L_005AE6:
        /* 005AE6  bra  $5b30 */
    } else {
L_005AEA:
        a0 = a0 - 0x8;                                       /* 005AEA  lea  -$8(a0),a0 */
        do {
            BUDGET_CHECK(0x5AEEU);
L_005AEE:
            /* 005AEE  movem.l  (a1)+,d0-d3 */
            { uint32_t ea;
            ea = a1;
            d0 = rd32(ea);
            ea += 4;
            d1 = rd32(ea);
            ea += 4;
            d2 = rd32(ea);
            ea += 4;
            d3 = rd32(ea);
            ea += 4;
            a1 = ea;
            }
            d6 = d0;                                             /* 005AF2  move.l  d0,d6 */
            d6 |= d1;                                            /* 005AF4  or.l  d1,d6 */
            d6 |= d2;                                            /* 005AF6  or.l  d2,d6 */
            d6 |= d3;                                            /* 005AF8  or.l  d3,d6 */
            d6 = ~d6;                                            /* 005AFA  not.l  d6 */
            wr16(a0, rd16(a0) & (d6 & 0xFFFF));                  /* 005AFC  and.w  d6,(a0) */
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005AFE  or.w  d3,(a0) */
            /* 005B00  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005B02  or.w  d2,(a0) */
            /* 005B04  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005B06  or.w  d1,(a0) */
            /* 005B08  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005B0A  or.w  d0,(a0) */
            d0 = (d0 << 16) | (d0 >> 16);                        /* 005B0C  swap  d0 */
            d1 = (d1 << 16) | (d1 >> 16);                        /* 005B0E  swap  d1 */
            d2 = (d2 << 16) | (d2 >> 16);                        /* 005B10  swap  d2 */
            d3 = (d3 << 16) | (d3 >> 16);                        /* 005B12  swap  d3 */
            d6 = (d6 << 16) | (d6 >> 16);                        /* 005B14  swap  d6 */
            /* 005B16  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005B18  or.w  d3,(a0) */
            /* 005B1A  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005B1C  or.w  d2,(a0) */
            /* 005B1E  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005B20  or.w  d1,(a0) */
            /* 005B22  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005B24  or.w  d0,(a0) */
            a0 = a0 + 0xAE;                                      /* 005B26  lea  $ae(a0),a0 */
            /* 005B2A  subq.w  #1,$2d4(a5) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(V_02D4);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(V_02D4, r);
                fz = (r == 0);
            }
            /* 005B2E  bne  $5aee */
        } while (COND(6));
    }
L_005B30:
    RTS();                                               /* 005B30  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5980U: goto L_005980;
        case 0x5986U: goto L_005986;
        case 0x598EU: goto L_00598E;
        case 0x5994U: goto L_005994;
        case 0x59A4U: goto L_0059A4;
        case 0x59E8U: goto L_0059E8;
        case 0x59F0U: goto L_0059F0;
        case 0x59FCU: goto L_0059FC;
        case 0x5A6CU: goto L_005A6C;
        case 0x5A70U: goto L_005A70;
        case 0x5A76U: goto L_005A76;
        case 0x5AE6U: goto L_005AE6;
        case 0x5AEAU: goto L_005AEA;
        case 0x5AEEU: goto L_005AEE;
        case 0x5B30U: goto L_005B30;
        }
    }
}

/* 005B32  game routine */
void sub_005B32(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_ENEMIES;                                      /* 005B32  lea  $412(a5),a6 */
    a0 = V_029E;                                         /* 005B36  lea  $29e(a5),a0 */
    SETW(d5, 0x18);                                      /* 005B3A  move.w  #$18,d5 */
    do {
        BUDGET_CHECK(0x5B3EU);
L_005B3E:
        /* 005B3E  tst.w  $4(a6) */
        /* 005B42  beq  $5b4a */
        if (rd16(a6 + 0x4) != 0) {
L_005B44:
            SETW(d0, rd16(a6 + 0x2));                            /* 005B44  move.w  $2(a6),d0 */
            /* 005B48  bra  $5b4c */
        } else {
L_005B4A:
            d0 = 0xFFFFFFFF;                                     /* 005B4A  moveq  #-1,d0 */
        }
L_005B4C:
        /* 005B4C  move.w  d0,(a0)+ */
        {
            uint32_t v = (d0 & 0xFFFF);
            wr16(a0, v);
            a0 += 2;
        }
        a6 = a6 + 0x40;                                      /* 005B4E  lea  $40(a6),a6 */
        /* 005B52  dbra  d5,$5b3e */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
L_005B56:
    a2 = V_029E;                                         /* 005B56  lea  $29e(a5),a2 */
    a1 = V_02D0;                                         /* 005B5A  lea  $2d0(a5),a1 */
    SETW(d5, 0x18);                                      /* 005B5E  move.w  #$18,d5 */
    for (;;) {
        BUDGET_CHECK(0x5B62U);
L_005B62:
        a0 = a1;                                             /* 005B62  movea.l  a1,a0 */
        SETW(d4, (d5 & 0xFFFF));                             /* 005B64  move.w  d5,d4 */
        SETW(d6, (d4 & 0xFFFF));                             /* 005B66  move.w  d4,d6 */
        d1 = 0xFFFFFFFF;                                     /* 005B68  moveq  #-1,d1 */
        do {
            BUDGET_CHECK(0x5B6AU);
L_005B6A:
            /* 005B6A  move.w  -(a0),d0 */
            a0 -= 2;
            SETW(d0, rd16(a0));
            /* 005B6C  cmp.w  d1,d0 */
            /* 005B6E  bcc  $5b74 */
            if ((d0 & 0xFFFF) < (d1 & 0xFFFF)) {
L_005B70:
                SETW(d1, (d0 & 0xFFFF));                             /* 005B70  move.w  d0,d1 */
                SETW(d6, (d4 & 0xFFFF));                             /* 005B72  move.w  d4,d6 */
            }
L_005B74:
            /* 005B74  dbra  d4,$5b6a */
            SETW(d4, d4 - 1);
        } while ((d4 & 0xFFFF) != 0xFFFF);
L_005B78:
        /* 005B78  tst.w  d1 */
        /* 005B7A  bmi  $5ba6 */
        if ((int16_t)(d1 & 0xFFFF) < 0) {
            goto L_005BA6;
        }
L_005B7C:
        SETW(d6, (d6 & 0xFFFF) << 1);                        /* 005B7C  lsl.w  #1,d6 */
        wr16(a2 + SX16(d6), 0xFFFF);                         /* 005B7E  move.w  #$ffff,$0(a2,d6.w) */
        SETW(d6, shift_op(1, 1, (d6 & 0xFFFF), 5, 16));      /* 005B84  lsl.w  #5,d6 */
        /* 005B86  movem.l  a6/a2/a1/a0/d5,-(a7) */
        PUSH32(a6);
        PUSH32(a2);
        PUSH32(a1);
        PUSH32(a0);
        PUSH32(d5);
        a6 = V_ENEMIES;                                      /* 005B8A  lea  $412(a5),a6 */
        a6 = a6 + SX16(d6);                                  /* 005B8E  lea  $0(a6,d6.w),a6 */
        SETW(d0, rd16(a6));                                  /* 005B92  move.w  $0(a6),d0 */
        SETW(d2, rd16(a6 + 0x2));                            /* 005B96  move.w  $2(a6),d2 */
        SETW(d3, rd16(a6 + 0x8));                            /* 005B9A  move.w  $8(a6),d3 */
        CALL(draw_enemy, 0x5BA0U);                           /* 005B9E  bsr  $5ba8 */
L_005BA0:
        /* 005BA0  movem.l  (a7)+,d5/a0-a2/a6 */
        HOOK(8, 0x5BA0U);
        d5 = POP32();
        a0 = POP32();
        a1 = POP32();
        a2 = POP32();
        a6 = POP32();
        /* 005BA4  bra  $5b62 */
    }
L_005BA6:
    RTS();                                               /* 005BA6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5B3EU: goto L_005B3E;
        case 0x5B44U: goto L_005B44;
        case 0x5B4AU: goto L_005B4A;
        case 0x5B4CU: goto L_005B4C;
        case 0x5B56U: goto L_005B56;
        case 0x5B62U: goto L_005B62;
        case 0x5B6AU: goto L_005B6A;
        case 0x5B70U: goto L_005B70;
        case 0x5B74U: goto L_005B74;
        case 0x5B78U: goto L_005B78;
        case 0x5B7CU: goto L_005B7C;
        case 0x5BA0U: goto L_005BA0;
        case 0x5BA6U: goto L_005BA6;
        }
    }
}

/* 005BA8  Draws an enemy */
void draw_enemy(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 005BA8  move.w  #$15,d7 */
    HOOK(9, 0x5BA8U);
    SETW(d7, 0x15);
    d5 = 0;                                              /* 005BAC  moveq  #0,d5 */
    /* 005BAE  cmp.w  #$64,d2 */
    /* 005BB2  bcc  $5bca */
    if ((d2 & 0xFFFF) < 0x64) {
L_005BB6:
        SETW(d6, 0x64);                                      /* 005BB6  move.w  #$64,d6 */
        SETW(d6, d6 - (d2 & 0xFFFF));                        /* 005BBA  sub.w  d2,d6 */
        SETW(d7, d7 - (d6 & 0xFFFF));                        /* 005BBC  sub.w  d6,d7 */
        /* 005BBE  add.w  d6,d5 */
        {
            uint32_t s = (d6 & 0xFFFF);
            uint32_t d = (d5 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d5, r);
            fx = (r < s);
        }
        d5 = (d5 & 0xFFFF) * 6;                              /* 005BC0  mulu.w  #$6,d5 */
        SETW(d2, 0x64);                                      /* 005BC4  move.w  #$64,d2 */
        /* 005BC8  bra  $5be6 */
    } else {
L_005BCA:
        /* 005BCA  cmp.w  #$116,d2 */
        /* 005BCE  bls  $5be6 */
        if ((d2 & 0xFFFF) > 0x116) {
L_005BD2:
            /* 005BD2  cmp.w  #$12d,d2 */
            /* 005BD6  bcs  $5bde */
            if ((d2 & 0xFFFF) >= 0x12D) {
L_005BD8:
                wr16(a6 + 0x4, 0);                                   /* 005BD8  clr.w  $4(a6) */
                RTS();                                               /* 005BDC  rts */
            }
L_005BDE:
            SETW(d4, (d2 & 0xFFFF));                             /* 005BDE  move.w  d2,d4 */
            SETW(d4, d4 - 0x117);                                /* 005BE0  subi.w  #$117,d4 */
            /* 005BE4  sub.w  d4,d7 */
            {
                uint32_t s = (d4 & 0xFFFF);
                uint32_t d = (d7 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d7, r);
                fx = (s > d);
            }
        }
    }
L_005BE6:
    /* 005BE6  cmp.w  #$64,d0 */
    /* 005BEA  bcs  $5d50 */
    if ((d0 & 0xFFFF) >= 0x64) {
L_005BEE:
        /* 005BEE  cmp.w  #$114,d0 */
        /* 005BF2  bhi  $5cd4 */
        if ((d0 & 0xFFFF) <= 0x114) {
L_005BF6:
            SETW(d2, d2 - 0x64);                                 /* 005BF6  subi.w  #$64,d2 */
            a1 = rd32(V_DRAW_BUFFER);                            /* 005BFA  movea.l  $28e(a5),a1 */
            SETW(d2, (d2 & 0xFFFF) << 5);                        /* 005BFE  lsl.w  #5,d2 */
            a1 = a1 + SX16(d2);                                  /* 005C00  lea  $0(a1,d2.w),a1 */
            SETW(d2, (d2 & 0xFFFF) << 2);                        /* 005C04  lsl.w  #2,d2 */
            a1 = a1 + SX16(d2);                                  /* 005C06  lea  $0(a1,d2.w),a1 */
            SETW(d0, d0 - 0x64);                                 /* 005C0A  subi.w  #$64,d0 */
            a1 += 0x20;                                          /* 005C0E  adda.l  #$20,a1 */
            SETW(d1, (d0 & 0xFFFF));                             /* 005C14  move.w  d0,d1 */
            SETW(d1, d1 & 0xE);                                  /* 005C16  andi.w  #$e,d1 */
            SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 4, 16));      /* 005C1A  lsr.w  #4,d0 */
            /* 005C1C  tst.w  d1 */
            /* 005C1E  beq  $5c24 */
            if ((d1 & 0xFFFF) != 0) {
L_005C20:
                CALL(sub_005C68, 0x5C22U);                           /* 005C20  bsr  $5c68 */
L_005C22:
                RTS();                                               /* 005C22  rts */
            }
L_005C24:
            CALL(sub_005C28, 0x5C26U);                           /* 005C24  bsr  $5c28 */
L_005C26:
            RTS();                                               /* 005C26  rts */
        }
L_005CD4:
        /* 005CD4  tst.w  d7 */
        /* 005CD6  bmi  $5d4e */
        if ((int16_t)(d7 & 0xFFFF) >= 0) {
L_005CDA:
            d4 = d5;                                             /* 005CDA  move.l  d5,d4 */
            /* 005CDC  subi.w  #$114,d0 */
            {
                uint32_t s = 0x114;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            SETW(d5, (d0 & 0xFFFF));                             /* 005CE0  move.w  d0,d5 */
            /* 005CE2  cmp.w  #$10,d5 */
            /* 005CE6  bcs  $5cee */
            if ((d5 & 0xFFFF) >= 0x10) {
L_005CE8:
                wr16(a6 + 0x4, 0);                                   /* 005CE8  clr.w  $4(a6) */
                RTS();                                               /* 005CEC  rts */
            }
L_005CEE:
            d5 &= ~0x1;                                          /* 005CEE  bclr  #$0,d5 */
            SETW(d2, d2 - 0x64);                                 /* 005CF2  subi.w  #$64,d2 */
            a1 = rd32(V_DRAW_BUFFER);                            /* 005CF6  movea.l  $28e(a5),a1 */
            SETW(d2, (d2 & 0xFFFF) << 5);                        /* 005CFA  lsl.w  #5,d2 */
            a1 = a1 + SX16(d2);                                  /* 005CFC  lea  $0(a1,d2.w),a1 */
            SETW(d2, shift_op(1, 1, (d2 & 0xFFFF), 2, 16));      /* 005D00  lsl.w  #2,d2 */
            a1 = a1 + SX16(d2);                                  /* 005D02  lea  $0(a1,d2.w),a1 */
            a2 = 0x5E60A;                                        /* 005D06  lea  $5e60a.l,a2 */
            d3 = (d3 & 0xFFFF) * 0x84;                           /* 005D0C  mulu.w  #$84,d3 */
            a2 = a2 + SX16(d3);                                  /* 005D10  lea  $0(a2,d3.w),a2 */
            a2 += d4;                                            /* 005D14  adda.l  d4,a2 */
            a1 += 0x78;                                          /* 005D16  adda.l  #$78,a1 */
            do {
                BUDGET_CHECK(0x5D1CU);
L_005D1C:
                d0 = 0;                                              /* 005D1C  moveq  #0,d0 */
                d1 = 0;                                              /* 005D1E  moveq  #0,d1 */
                d2 = 0;                                              /* 005D20  moveq  #0,d2 */
                /* 005D22  move.w  (a2)+,d0 */
                SETW(d0, rd16(a2));
                a2 += 2;
                /* 005D24  move.w  (a2)+,d1 */
                SETW(d1, rd16(a2));
                a2 += 2;
                /* 005D26  move.w  (a2)+,d2 */
                SETW(d2, rd16(a2));
                a2 += 2;
                d0 = shift_op(3, 0, d0, (d5 & 63), 32);              /* 005D28  ror.l  d5,d0 */
                d1 = shift_op(3, 0, d1, (d5 & 63), 32);              /* 005D2A  ror.l  d5,d1 */
                d2 = shift_op(3, 0, d2, (d5 & 63), 32);              /* 005D2C  ror.l  d5,d2 */
                SETW(d6, (d0 & 0xFFFF));                             /* 005D2E  move.w  d0,d6 */
                SETW(d6, d6 | (d1 & 0xFFFF));                        /* 005D30  or.w  d1,d6 */
                SETW(d6, d6 | (d2 & 0xFFFF));                        /* 005D32  or.w  d2,d6 */
                wr16(a1 + 0x6, rd16(a1 + 0x6) | (d6 & 0xFFFF));      /* 005D34  or.w  d6,$6(a1) */
                SETW(d6, ~(d6 & 0xFFFF));                            /* 005D38  not.w  d6 */
                wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 005D3A  and.w  d6,(a1) */
                /* 005D3C  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 005D3E  and.w  d6,(a1) */
                /* 005D40  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 005D42  and.w  d6,(a1) */
                wr16(a1, rd16(a1) | (d2 & 0xFFFF));                  /* 005D44  or.w  d2,(a1) */
                a1 = a1 + 0x9C;                                      /* 005D46  lea  $9c(a1),a1 */
                /* 005D4A  dbra  d7,$5d1c */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
L_005D4E:
        RTS();                                               /* 005D4E  rts */
    }
L_005D50:
    /* 005D50  tst.w  d7 */
    /* 005D52  bmi  $5dca */
    if ((int16_t)(d7 & 0xFFFF) >= 0) {
L_005D56:
        d4 = d5;                                             /* 005D56  move.l  d5,d4 */
        SETW(d5, 0x64);                                      /* 005D58  move.w  #$64,d5 */
        /* 005D5C  sub.w  d0,d5 */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t d = (d5 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d5, r);
            fx = (s > d);
        }
        /* 005D5E  cmp.w  #$10,d5 */
        /* 005D62  bcs  $5d6a */
        if ((d5 & 0xFFFF) >= 0x10) {
L_005D64:
            wr16(a6 + 0x4, 0);                                   /* 005D64  clr.w  $4(a6) */
            RTS();                                               /* 005D68  rts */
        }
L_005D6A:
        d5 &= ~0x1;                                          /* 005D6A  bclr  #$0,d5 */
        SETW(d2, d2 - 0x64);                                 /* 005D6E  subi.w  #$64,d2 */
        a1 = rd32(V_DRAW_BUFFER);                            /* 005D72  movea.l  $28e(a5),a1 */
        SETW(d2, (d2 & 0xFFFF) << 5);                        /* 005D76  lsl.w  #5,d2 */
        a1 = a1 + SX16(d2);                                  /* 005D78  lea  $0(a1,d2.w),a1 */
        SETW(d2, shift_op(1, 1, (d2 & 0xFFFF), 2, 16));      /* 005D7C  lsl.w  #2,d2 */
        a1 = a1 + SX16(d2);                                  /* 005D7E  lea  $0(a1,d2.w),a1 */
        a2 = 0x5E60A;                                        /* 005D82  lea  $5e60a.l,a2 */
        d3 = (d3 & 0xFFFF) * 0x84;                           /* 005D88  mulu.w  #$84,d3 */
        a2 = a2 + SX16(d3);                                  /* 005D8C  lea  $0(a2,d3.w),a2 */
        a2 += d4;                                            /* 005D90  adda.l  d4,a2 */
        a1 += 0x20;                                          /* 005D92  adda.l  #$20,a1 */
        do {
            BUDGET_CHECK(0x5D98U);
L_005D98:
            d0 = 0;                                              /* 005D98  moveq  #0,d0 */
            d1 = 0;                                              /* 005D9A  moveq  #0,d1 */
            d2 = 0;                                              /* 005D9C  moveq  #0,d2 */
            /* 005D9E  move.w  (a2)+,d0 */
            SETW(d0, rd16(a2));
            a2 += 2;
            /* 005DA0  move.w  (a2)+,d1 */
            SETW(d1, rd16(a2));
            a2 += 2;
            /* 005DA2  move.w  (a2)+,d2 */
            SETW(d2, rd16(a2));
            a2 += 2;
            d0 = shift_op(3, 1, d0, (d5 & 63), 32);              /* 005DA4  rol.l  d5,d0 */
            d1 = shift_op(3, 1, d1, (d5 & 63), 32);              /* 005DA6  rol.l  d5,d1 */
            d2 = shift_op(3, 1, d2, (d5 & 63), 32);              /* 005DA8  rol.l  d5,d2 */
            SETW(d6, (d0 & 0xFFFF));                             /* 005DAA  move.w  d0,d6 */
            SETW(d6, d6 | (d1 & 0xFFFF));                        /* 005DAC  or.w  d1,d6 */
            SETW(d6, d6 | (d2 & 0xFFFF));                        /* 005DAE  or.w  d2,d6 */
            wr16(a1 + 0x6, rd16(a1 + 0x6) | (d6 & 0xFFFF));      /* 005DB0  or.w  d6,$6(a1) */
            SETW(d6, ~(d6 & 0xFFFF));                            /* 005DB4  not.w  d6 */
            wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 005DB6  and.w  d6,(a1) */
            /* 005DB8  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 005DBA  and.w  d6,(a1) */
            /* 005DBC  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d6 & 0xFFFF));                  /* 005DBE  and.w  d6,(a1) */
            wr16(a1, rd16(a1) | (d2 & 0xFFFF));                  /* 005DC0  or.w  d2,(a1) */
            a1 = a1 + 0x9C;                                      /* 005DC2  lea  $9c(a1),a1 */
            /* 005DC6  dbra  d7,$5d98 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
    }
L_005DCA:
    RTS();                                               /* 005DCA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5BB6U: goto L_005BB6;
        case 0x5BCAU: goto L_005BCA;
        case 0x5BD2U: goto L_005BD2;
        case 0x5BD8U: goto L_005BD8;
        case 0x5BDEU: goto L_005BDE;
        case 0x5BE6U: goto L_005BE6;
        case 0x5BEEU: goto L_005BEE;
        case 0x5BF6U: goto L_005BF6;
        case 0x5C20U: goto L_005C20;
        case 0x5C22U: goto L_005C22;
        case 0x5C24U: goto L_005C24;
        case 0x5C26U: goto L_005C26;
        case 0x5CD4U: goto L_005CD4;
        case 0x5CDAU: goto L_005CDA;
        case 0x5CE8U: goto L_005CE8;
        case 0x5CEEU: goto L_005CEE;
        case 0x5D1CU: goto L_005D1C;
        case 0x5D4EU: goto L_005D4E;
        case 0x5D50U: goto L_005D50;
        case 0x5D56U: goto L_005D56;
        case 0x5D64U: goto L_005D64;
        case 0x5D6AU: goto L_005D6A;
        case 0x5D98U: goto L_005D98;
        case 0x5DCAU: goto L_005DCA;
        }
    }
}

/* 005C28  game routine */
void sub_005C28(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 005C28  tst.w  d7 */
    /* 005C2A  bmi  $5c66 */
    if ((int16_t)(d7 & 0xFFFF) >= 0) {
L_005C2C:
        SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 005C2C  lsl.w  #3,d0 */
        a1 = a1 + SX16(d0);                                  /* 005C2E  lea  $0(a1,d0.w),a1 */
        a2 = 0x5E60A;                                        /* 005C32  lea  $5e60a.l,a2 */
        d3 = (d3 & 0xFFFF) * 0x84;                           /* 005C38  mulu.w  #$84,d3 */
        a2 = a2 + SX16(d3);                                  /* 005C3C  lea  $0(a2,d3.w),a2 */
        a2 += d5;                                            /* 005C40  adda.l  d5,a2 */
        do {
            BUDGET_CHECK(0x5C42U);
L_005C42:
            /* 005C42  movem.w  (a2)+,d0-d2 */
            { uint32_t ea;
            ea = a2;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            a2 = ea;
            }
            SETW(d4, (d0 & 0xFFFF));                             /* 005C46  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 005C48  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 005C4A  or.w  d2,d4 */
            wr16(a1 + 0x6, rd16(a1 + 0x6) | (d4 & 0xFFFF));      /* 005C4C  or.w  d4,$6(a1) */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 005C50  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005C52  and.w  d4,(a1) */
            /* 005C54  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005C56  and.w  d4,(a1) */
            /* 005C58  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005C5A  and.w  d4,(a1) */
            wr16(a1, rd16(a1) | (d2 & 0xFFFF));                  /* 005C5C  or.w  d2,(a1) */
            a1 = a1 + 0x9C;                                      /* 005C5E  lea  $9c(a1),a1 */
            /* 005C62  dbra  d7,$5c42 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
    }
L_005C66:
    RTS();                                               /* 005C66  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5C2CU: goto L_005C2C;
        case 0x5C42U: goto L_005C42;
        case 0x5C66U: goto L_005C66;
        }
    }
}

/* 005C68  game routine */
void sub_005C68(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 005C68  tst.w  d7 */
    /* 005C6A  bmi  $5cd2 */
    if ((int16_t)(d7 & 0xFFFF) >= 0) {
L_005C6E:
        a2 = 0x5E60A;                                        /* 005C6E  lea  $5e60a.l,a2 */
        d3 = (d3 & 0xFFFF) * 0x108;                          /* 005C74  mulu.w  #$108,d3 */
        a2 = a2 + SX16(d3);                                  /* 005C78  lea  $0(a2,d3.w),a2 */
        SETW(d5, (d5 & 0xFFFF) << 1);                        /* 005C7C  lsl.w  #1,d5 */
        a2 += d5;                                            /* 005C7E  adda.l  d5,a2 */
        SETW(d1, (d1 & 0xFFFF) >> 1);                        /* 005C80  lsr.w  #1,d1 */
        d1 = (d1 & 0xFFFF) * 0x2838;                         /* 005C82  mulu.w  #$2838,d1 */
        a2 = a2 + d1;                                        /* 005C86  lea  $0(a2,d1.l),a2 */
        SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 005C8A  lsl.w  #3,d0 */
        a1 = a1 + SX16(d0);                                  /* 005C8C  lea  $0(a1,d0.w),a1 */
        do {
            BUDGET_CHECK(0x5C90U);
L_005C90:
            /* 005C90  movem.w  (a2)+,d0-d2 */
            { uint32_t ea;
            ea = a2;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            a2 = ea;
            }
            SETW(d4, (d0 & 0xFFFF));                             /* 005C94  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 005C96  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 005C98  or.w  d2,d4 */
            wr16(a1 + 0x6, rd16(a1 + 0x6) | (d4 & 0xFFFF));      /* 005C9A  or.w  d4,$6(a1) */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 005C9E  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005CA0  and.w  d4,(a1) */
            /* 005CA2  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005CA4  and.w  d4,(a1) */
            /* 005CA6  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005CA8  and.w  d4,(a1) */
            wr16(a1, rd16(a1) | (d2 & 0xFFFF));                  /* 005CAA  or.w  d2,(a1) */
            a1 += 4;                                             /* 005CAC  addq.l  #4,a1 */
            /* 005CAE  movem.w  (a2)+,d0-d2 */
            { uint32_t ea;
            ea = a2;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            a2 = ea;
            }
            SETW(d4, (d0 & 0xFFFF));                             /* 005CB2  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 005CB4  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 005CB6  or.w  d2,d4 */
            wr16(a1 + 0x6, rd16(a1 + 0x6) | (d4 & 0xFFFF));      /* 005CB8  or.w  d4,$6(a1) */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 005CBC  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005CBE  and.w  d4,(a1) */
            /* 005CC0  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005CC2  and.w  d4,(a1) */
            /* 005CC4  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 005CC6  and.w  d4,(a1) */
            wr16(a1, rd16(a1) | (d2 & 0xFFFF));                  /* 005CC8  or.w  d2,(a1) */
            a1 = a1 + 0x94;                                      /* 005CCA  lea  $94(a1),a1 */
            /* 005CCE  dbra  d7,$5c90 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
    }
L_005CD2:
    RTS();                                               /* 005CD2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5C6EU: goto L_005C6E;
        case 0x5C90U: goto L_005C90;
        case 0x5CD2U: goto L_005CD2;
        }
    }
}

/* 005E32  Draws objects of type A */
void draw_objects_a(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1206;                                         /* 005E32  lea  $1206(a5),a6 */
    SETW(d6, 0x10);                                      /* 005E36  move.w  #$10,d6 */
    do {
        BUDGET_CHECK(0x5E3AU);
L_005E3A:
        /* 005E3A  tst.w  $4(a6) */
        /* 005E3E  beq  $5f10 */
        HOOK(10, 0x5E3AU);
        if (rd16(a6 + 0x4) != 0) {
L_005E42:
            SETW(d0, rd16(a6 + 0x2));                            /* 005E42  move.w  $2(a6),d0 */
            /* 005E46  subi.w  #$64,d0 */
            {
                uint32_t s = 0x64;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 005E4A  cmp.w  #$c0,d0 */
            /* 005E4E  bls  $5e58 */
            if ((d0 & 0xFFFF) <= 0xC0) {
                goto L_005E58;
            }
L_005E50:
            wr16(a6 + 0x4, 0);                                   /* 005E50  clr.w  $4(a6) */
            /* 005E54  bra  $5f10 */
            goto L_005F10;
L_005E58:
            d0 = (d0 & 0xFFFF) * 0xA0;                           /* 005E58  mulu.w  #$a0,d0 */
            a0 = rd32(V_DRAW_BUFFER);                            /* 005E5C  movea.l  $28e(a5),a0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 005E60  adda.w  d0,a0 */
            SETW(d0, rd16(a6));                                  /* 005E62  move.w  $0(a6),d0 */
            /* 005E66  subi.w  #$24,d0 */
            {
                uint32_t s = 0x24;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 005E6A  cmp.w  #$42,d0 */
            /* 005E6E  bls  $5e50 */
            if ((d0 & 0xFFFF) <= 0x42) {
                BUDGET_CHECK(0x5E50U);
                goto L_005E50;
            }
L_005E70:
            /* 005E70  cmp.w  #$f8,d0 */
            /* 005E74  bhi  $5e50 */
            if ((d0 & 0xFFFF) > 0xF8) {
                BUDGET_CHECK(0x5E50U);
                goto L_005E50;
            }
L_005E76:
            SETW(d1, (d0 & 0xFFFF));                             /* 005E76  move.w  d0,d1 */
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 005E78  lsr.w  #1,d0 */
            SETW(d0, d0 & 0xFFF8);                               /* 005E7A  andi.w  #$fff8,d0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 005E7E  adda.w  d0,a0 */
            d7 = 8;                                              /* 005E80  moveq  #8,d7 */
            a1 = a0;                                             /* 005E82  movea.l  a0,a1 */
            a1 += 1;                                             /* 005E84  addq.w  #1,a1 */
            d1 &= 0xF;                                           /* 005E86  andi.l  #$f,d1 */
            /* 005E8C  bclr  #$3,d1 */
            fz = !(d1 & 0x8);
            d1 &= ~0x8;
            /* 005E90  beq  $5e96 */
            if (COND(6)) {
L_005E92:
                a0 += 1;                                             /* 005E92  addq.w  #1,a0 */
                a1 += 7;                                             /* 005E94  addq.w  #7,a1 */
            }
L_005E96:
            d2 = 0;                                              /* 005E96  moveq  #0,d2 */
            SETW(d2, rd16(a6 + 0xA));                            /* 005E98  move.w  $a(a6),d2 */
            d2 = (d2 & 0xFFFF) * 0x36;                           /* 005E9C  mulu.w  #$36,d2 */
            d1 = (d1 & 0xFFFF) * 0x61E;                          /* 005EA0  mulu.w  #$61e,d1 */
            /* 005EA4  add.l  d1,d2 */
            {
                uint32_t s = d1;
                uint32_t d = d2;
                uint32_t r = (d + s) & 0xFFFFFFFF;
                d2 = r;
                fx = (r < s);
            }
            a2 = 0x4694A;                                        /* 005EA6  lea  $4694a.l,a2 */
            a2 += d2;                                            /* 005EAC  adda.l  d2,a2 */
            do {
                BUDGET_CHECK(0x5EAEU);
L_005EAE:
                /* 005EAE  move.b  (a2)+,d0 */
                SETB(d0, rd8(a2));
                a2 += 1;
                SETB(d1, rd8(a2 + 0x1));                             /* 005EB0  move.b  $1(a2),d1 */
                SETB(d2, rd8(a2 + 0x3));                             /* 005EB4  move.b  $3(a2),d2 */
                SETB(d4, (d0 & 0xFF));                               /* 005EB8  move.b  d0,d4 */
                SETB(d4, d4 | (d1 & 0xFF));                          /* 005EBA  or.b  d1,d4 */
                SETB(d4, d4 | (d2 & 0xFF));                          /* 005EBC  or.b  d2,d4 */
                wr8(a0 + 0x6, rd8(a0 + 0x6) | (d4 & 0xFF));          /* 005EBE  or.b  d4,$6(a0) */
                SETB(d4, ~(d4 & 0xFF));                              /* 005EC2  not.b  d4 */
                wr8(a0, rd8(a0) & (d4 & 0xFF));                      /* 005EC4  and.b  d4,(a0) */
                wr8(a0 + 0x2, rd8(a0 + 0x2) & (d4 & 0xFF));          /* 005EC6  and.b  d4,$2(a0) */
                wr8(a0 + 0x4, rd8(a0 + 0x4) & (d4 & 0xFF));          /* 005ECA  and.b  d4,$4(a0) */
                wr8(a0, rd8(a0) | (d0 & 0xFF));                      /* 005ECE  or.b  d0,(a0) */
                wr8(a0 + 0x2, rd8(a0 + 0x2) | (d1 & 0xFF));          /* 005ED0  or.b  d1,$2(a0) */
                wr8(a0 + 0x4, rd8(a0 + 0x4) | (d2 & 0xFF));          /* 005ED4  or.b  d2,$4(a0) */
                SETB(d0, rd8(a2));                                   /* 005ED8  move.b  (a2),d0 */
                SETB(d1, rd8(a2 + 0x2));                             /* 005EDA  move.b  $2(a2),d1 */
                SETB(d2, rd8(a2 + 0x4));                             /* 005EDE  move.b  $4(a2),d2 */
                SETB(d4, (d0 & 0xFF));                               /* 005EE2  move.b  d0,d4 */
                SETB(d4, d4 | (d1 & 0xFF));                          /* 005EE4  or.b  d1,d4 */
                SETB(d4, d4 | (d2 & 0xFF));                          /* 005EE6  or.b  d2,d4 */
                wr8(a1 + 0x6, rd8(a1 + 0x6) | (d4 & 0xFF));          /* 005EE8  or.b  d4,$6(a1) */
                SETB(d4, ~(d4 & 0xFF));                              /* 005EEC  not.b  d4 */
                wr8(a1, rd8(a1) & (d4 & 0xFF));                      /* 005EEE  and.b  d4,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) & (d4 & 0xFF));          /* 005EF0  and.b  d4,$2(a1) */
                wr8(a1 + 0x4, rd8(a1 + 0x4) & (d4 & 0xFF));          /* 005EF4  and.b  d4,$4(a1) */
                wr8(a1, rd8(a1) | (d0 & 0xFF));                      /* 005EF8  or.b  d0,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) | (d1 & 0xFF));          /* 005EFA  or.b  d1,$2(a1) */
                wr8(a1 + 0x4, rd8(a1 + 0x4) | (d2 & 0xFF));          /* 005EFE  or.b  d2,$4(a1) */
                a0 += SX16(0xA0);                                    /* 005F02  adda.w  #$a0,a0 */
                a1 += SX16(0xA0);                                    /* 005F06  adda.w  #$a0,a1 */
                a2 += 5;                                             /* 005F0A  addq.l  #5,a2 */
                /* 005F0C  dbra  d7,$5eae */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
L_005F10:
        /* 005F10  lea  $14(a6),a6 */
        HOOK(11, 0x5F10U);
        a6 = a6 + 0x14;
        /* 005F14  dbra  d6,$5e3a */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_005F18:
    RTS();                                               /* 005F18  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5E3AU: goto L_005E3A;
        case 0x5E42U: goto L_005E42;
        case 0x5E50U: goto L_005E50;
        case 0x5E58U: goto L_005E58;
        case 0x5E70U: goto L_005E70;
        case 0x5E76U: goto L_005E76;
        case 0x5E92U: goto L_005E92;
        case 0x5E96U: goto L_005E96;
        case 0x5EAEU: goto L_005EAE;
        case 0x5F10U: goto L_005F10;
        case 0x5F18U: goto L_005F18;
        }
    }
}

/* 005F1A  Draws objects of type B */
void draw_objects_b(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1614;                                         /* 005F1A  lea  $1614(a5),a6 */
    SETW(d6, 0x13);                                      /* 005F1E  move.w  #$13,d6 */
    do {
        BUDGET_CHECK(0x5F22U);
L_005F22:
        /* 005F22  tst.w  (a6) */
        /* 005F24  beq  $5ff4 */
        HOOK(12, 0x5F22U);
        if (rd16(a6) != 0) {
L_005F28:
            SETW(d0, rd16(a6 + 0x4));                            /* 005F28  move.w  $4(a6),d0 */
            /* 005F2C  subi.w  #$64,d0 */
            {
                uint32_t s = 0x64;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 005F30  cmp.w  #$c0,d0 */
            /* 005F34  bls  $5f3c */
            if ((d0 & 0xFFFF) <= 0xC0) {
                goto L_005F3C;
            }
L_005F36:
            wr16(a6, 0);                                         /* 005F36  clr.w  (a6) */
            /* 005F38  bra  $5ff4 */
            goto L_005FF4;
L_005F3C:
            d0 = (d0 & 0xFFFF) * 0xA0;                           /* 005F3C  mulu.w  #$a0,d0 */
            a0 = rd32(V_DRAW_BUFFER);                            /* 005F40  movea.l  $28e(a5),a0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 005F44  adda.w  d0,a0 */
            SETW(d0, rd16(a6 + 0x2));                            /* 005F46  move.w  $2(a6),d0 */
            /* 005F4A  subi.w  #$24,d0 */
            {
                uint32_t s = 0x24;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 005F4E  cmp.w  #$40,d0 */
            /* 005F52  bcs  $5f36 */
            if ((d0 & 0xFFFF) < 0x40) {
                BUDGET_CHECK(0x5F36U);
                goto L_005F36;
            }
L_005F54:
            /* 005F54  cmp.w  #$f4,d0 */
            /* 005F58  bhi  $5f36 */
            if ((d0 & 0xFFFF) > 0xF4) {
                BUDGET_CHECK(0x5F36U);
                goto L_005F36;
            }
L_005F5A:
            SETW(d1, (d0 & 0xFFFF));                             /* 005F5A  move.w  d0,d1 */
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 005F5C  lsr.w  #1,d0 */
            SETW(d0, d0 & 0xFFF8);                               /* 005F5E  andi.w  #$fff8,d0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 005F62  adda.w  d0,a0 */
            d7 = 8;                                              /* 005F64  moveq  #8,d7 */
            a1 = a0;                                             /* 005F66  movea.l  a0,a1 */
            a1 += 1;                                             /* 005F68  addq.w  #1,a1 */
            d1 &= 0xF;                                           /* 005F6A  andi.l  #$f,d1 */
            /* 005F70  bclr  #$3,d1 */
            fz = !(d1 & 0x8);
            d1 &= ~0x8;
            /* 005F74  beq  $5f7a */
            if (COND(6)) {
L_005F76:
                a0 += 1;                                             /* 005F76  addq.w  #1,a0 */
                a1 += 7;                                             /* 005F78  addq.w  #7,a1 */
            }
L_005F7A:
            d2 = 0;                                              /* 005F7A  moveq  #0,d2 */
            SETW(d2, rd16(a6 + 0x6));                            /* 005F7C  move.w  $6(a6),d2 */
            d2 = (d2 & 0xFFFF) * 0x36;                           /* 005F80  mulu.w  #$36,d2 */
            d1 = (d1 & 0xFFFF) * 0x61E;                          /* 005F84  mulu.w  #$61e,d1 */
            /* 005F88  add.l  d1,d2 */
            {
                uint32_t s = d1;
                uint32_t d = d2;
                uint32_t r = (d + s) & 0xFFFFFFFF;
                d2 = r;
                fx = (r < s);
            }
            a2 = 0x4694A;                                        /* 005F8A  lea  $4694a.l,a2 */
            a2 += d2;                                            /* 005F90  adda.l  d2,a2 */
            do {
                BUDGET_CHECK(0x5F92U);
L_005F92:
                /* 005F92  move.b  (a2)+,d0 */
                SETB(d0, rd8(a2));
                a2 += 1;
                SETB(d1, rd8(a2 + 0x1));                             /* 005F94  move.b  $1(a2),d1 */
                SETB(d2, rd8(a2 + 0x3));                             /* 005F98  move.b  $3(a2),d2 */
                SETB(d4, (d0 & 0xFF));                               /* 005F9C  move.b  d0,d4 */
                SETB(d4, d4 | (d1 & 0xFF));                          /* 005F9E  or.b  d1,d4 */
                SETB(d4, d4 | (d2 & 0xFF));                          /* 005FA0  or.b  d2,d4 */
                SETB(d4, ~(d4 & 0xFF));                              /* 005FA2  not.b  d4 */
                wr8(a0, rd8(a0) & (d4 & 0xFF));                      /* 005FA4  and.b  d4,(a0) */
                wr8(a0 + 0x2, rd8(a0 + 0x2) & (d4 & 0xFF));          /* 005FA6  and.b  d4,$2(a0) */
                wr8(a0 + 0x4, rd8(a0 + 0x4) & (d4 & 0xFF));          /* 005FAA  and.b  d4,$4(a0) */
                wr8(a0 + 0x6, rd8(a0 + 0x6) & (d4 & 0xFF));          /* 005FAE  and.b  d4,$6(a0) */
                wr8(a0, rd8(a0) | (d0 & 0xFF));                      /* 005FB2  or.b  d0,(a0) */
                wr8(a0 + 0x2, rd8(a0 + 0x2) | (d1 & 0xFF));          /* 005FB4  or.b  d1,$2(a0) */
                wr8(a0 + 0x4, rd8(a0 + 0x4) | (d2 & 0xFF));          /* 005FB8  or.b  d2,$4(a0) */
                SETB(d0, rd8(a2));                                   /* 005FBC  move.b  (a2),d0 */
                SETB(d1, rd8(a2 + 0x2));                             /* 005FBE  move.b  $2(a2),d1 */
                SETB(d2, rd8(a2 + 0x4));                             /* 005FC2  move.b  $4(a2),d2 */
                SETB(d4, (d0 & 0xFF));                               /* 005FC6  move.b  d0,d4 */
                SETB(d4, d4 | (d1 & 0xFF));                          /* 005FC8  or.b  d1,d4 */
                SETB(d4, d4 | (d2 & 0xFF));                          /* 005FCA  or.b  d2,d4 */
                SETB(d4, ~(d4 & 0xFF));                              /* 005FCC  not.b  d4 */
                wr8(a1, rd8(a1) & (d4 & 0xFF));                      /* 005FCE  and.b  d4,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) & (d4 & 0xFF));          /* 005FD0  and.b  d4,$2(a1) */
                wr8(a1 + 0x4, rd8(a1 + 0x4) & (d4 & 0xFF));          /* 005FD4  and.b  d4,$4(a1) */
                wr8(a1 + 0x6, rd8(a1 + 0x6) & (d4 & 0xFF));          /* 005FD8  and.b  d4,$6(a1) */
                wr8(a1, rd8(a1) | (d0 & 0xFF));                      /* 005FDC  or.b  d0,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) | (d1 & 0xFF));          /* 005FDE  or.b  d1,$2(a1) */
L_005FE2:
                wr8(a1 + 0x4, rd8(a1 + 0x4) | (d2 & 0xFF));          /* 005FE2  or.b  d2,$4(a1) */
L_005FE6:
                a0 += SX16(0xA0);                                    /* 005FE6  adda.w  #$a0,a0 */
L_005FEA:
                a1 += SX16(0xA0);                                    /* 005FEA  adda.w  #$a0,a1 */
L_005FEE:
                a2 += 5;                                             /* 005FEE  addq.l  #5,a2 */
L_005FF0:
                /* 005FF0  dbra  d7,$5f92 */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
L_005FF4:
        /* 005FF4  lea  $10(a6),a6 */
        HOOK(13, 0x5FF4U);
        a6 = a6 + 0x10;
L_005FF8:
        /* 005FF8  dbra  d6,$5f22 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_005FFC:
    RTS();                                               /* 005FFC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5F22U: goto L_005F22;
        case 0x5F28U: goto L_005F28;
        case 0x5F36U: goto L_005F36;
        case 0x5F3CU: goto L_005F3C;
        case 0x5F54U: goto L_005F54;
        case 0x5F5AU: goto L_005F5A;
        case 0x5F76U: goto L_005F76;
        case 0x5F7AU: goto L_005F7A;
        case 0x5F92U: goto L_005F92;
        case 0x5FE2U: goto L_005FE2;
        case 0x5FE6U: goto L_005FE6;
        case 0x5FEAU: goto L_005FEA;
        case 0x5FEEU: goto L_005FEE;
        case 0x5FF0U: goto L_005FF0;
        case 0x5FF4U: goto L_005FF4;
        case 0x5FF8U: goto L_005FF8;
        case 0x5FFCU: goto L_005FFC;
        }
    }
}

/* 005FFE  game routine */
void sub_005FFE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x4694A;                                        /* 005FFE  lea  $4694a.l,a1 */
L_006004:
    a0 = 0x46F68;                                        /* 006004  lea  $46f68.l,a0 */
    d6 = 6;                                              /* 00600A  moveq  #6,d6 */
    do {
        BUDGET_CHECK(0x600CU);
L_00600C:
        SETW(d7, 0x104);                                     /* 00600C  move.w  #$104,d7 */
        do {
            BUDGET_CHECK(0x6010U);
L_006010:
            /* 006010  move.l  (a1)+,d0 */
            d0 = rd32(a1);
            a1 += 4;
            d0 = d0 >> 1;                                        /* 006012  lsr.l  #1,d0 */
            /* 006014  move.l  d0,(a0)+ */
            {
                uint32_t v = d0;
                wr32(a0, v);
                a0 += 4;
            }
            /* 006016  move.w  (a1)+,d0 */
            SETW(d0, rd16(a1));
            a1 += 2;
            SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 006018  lsr.w  #1,d0 */
            /* 00601A  move.w  d0,(a0)+ */
            {
                uint32_t v = (d0 & 0xFFFF);
                wr16(a0, v);
                a0 += 2;
            }
            /* 00601C  dbra  d7,$6010 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_006020:
        /* 006020  dbra  d6,$600c */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_006024:
    RTS();                                               /* 006024  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6004U: goto L_006004;
        case 0x600CU: goto L_00600C;
        case 0x6010U: goto L_006010;
        case 0x6020U: goto L_006020;
        case 0x6024U: goto L_006024;
        }
    }
}

