/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 003000  game routine */
void sub_003000(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x12C0);                                /* 003000  move.w  #$12c0,$266(a5) */
    wr16(V_0268, 7);                                     /* 003006  move.w  #$7,$268(a5) */
    wr16(V_026A, 7);                                     /* 00300C  move.w  #$7,$26a(a5) */
    SETW(d1, rd16(V_01C6));                              /* 003012  move.w  $1c6(a5),d1 */
    SETW(d1, d1 + 3);                                    /* 003016  addq.w  #3,d1 */
    wr16(V_026C, (d1 & 0xFFFF));                         /* 003018  move.w  d1,$26c(a5) */
    wr16(V_0230, (d1 & 0xFFFF));                         /* 00301C  move.w  d1,$230(a5) */
    a2 = 0x35EE;                                         /* 003020  lea  $35ee(pc),a2 */
    wr32(V_0234, a2);                                    /* 003024  move.l  a2,$234(a5) */
    wr16(V_0232, 0xFFFF);                                /* 003028  move.w  #$ffff,$232(a5) */
    SETW(d0, rd16(V_01C6));                              /* 00302E  move.w  $1c6(a5),d0 */
    d0 = (d0 & 0xFFFF) * 0xB40;                          /* 003032  mulu.w  #$b40,d0 */
    /* 003036  add.w  d0,$266(a5) */
    {
        uint32_t s = (d0 & 0xFFFF);
        uint32_t d = rd16(V_0266);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_0266, r);
        fx = (r < s);
    }
    CALL(sub_00344A, 0x303EU);                           /* 00303A  bsr  $344a */
L_00303E:
    CALL(wait_key, 0x3042U);                             /* 00303E  bsr  $31b6 */
L_003042:
    a0 = 0x3476;                                         /* 003042  lea  $3476(pc),a0 */
    a1 = rd32(V_01CC);                                   /* 003046  movea.l  $1cc(a5),a1 */
    d7 = 0x13;                                           /* 00304A  moveq  #19,d7 */
    do {
        BUDGET_CHECK(0x304CU);
L_00304C:
        /* 00304C  move.b  (a0)+,(a1)+ */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            wr8(a1, v);
            a1 += 1;
        }
        /* 00304E  dbra  d7,$304c */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 003052  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x303EU: goto L_00303E;
        case 0x3042U: goto L_003042;
        case 0x304CU: goto L_00304C;
        }
    }
}

/* 003054  game routine */
void sub_003054(void)
{
    SETW(d0, 0);                                         /* 003054  clr.w  d0 */
    a0 = V_0170;                                         /* 003056  lea  $170(a5),a0 */
    d1 = rd32(V_01E4);                                   /* 00305A  move.l  $1e4(a5),d1 */
    /* 00305E  cmp.l  (a0)+,d1 */
    {
        uint32_t s = rd32(a0);
        a0 += 4;
        uint32_t d = d1;
        uint32_t r = (d - s) & 0xFFFFFFFF;
        fz = (r == 0);
        fc = (s > d);
    }
    /* 003060  bhi  $308e */
    if (COND(3)) {
        /* 003062  addq.w  #1,d0 */
        {
            uint32_t s = 1;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        /* 003064  cmp.l  (a0)+,d1 */
        {
            uint32_t s = rd32(a0);
            a0 += 4;
            uint32_t d = d1;
            uint32_t r = (d - s) & 0xFFFFFFFF;
            fz = (r == 0);
            fc = (s > d);
        }
        /* 003066  bhi  $308e */
        if (COND(3)) {
            /* 003068  addq.w  #1,d0 */
            {
                uint32_t s = 1;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d0, r);
                fx = (r < s);
            }
            /* 00306A  cmp.l  (a0)+,d1 */
            {
                uint32_t s = rd32(a0);
                a0 += 4;
                uint32_t d = d1;
                uint32_t r = (d - s) & 0xFFFFFFFF;
                fz = (r == 0);
                fc = (s > d);
            }
            /* 00306C  bhi  $308e */
            if (COND(3)) {
                /* 00306E  addq.w  #1,d0 */
                {
                    uint32_t s = 1;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d0, r);
                    fx = (r < s);
                }
                /* 003070  cmp.l  (a0)+,d1 */
                {
                    uint32_t s = rd32(a0);
                    a0 += 4;
                    uint32_t d = d1;
                    uint32_t r = (d - s) & 0xFFFFFFFF;
                    fz = (r == 0);
                    fc = (s > d);
                }
                /* 003072  bhi  $308e */
                if (COND(3)) {
                    /* 003074  addq.w  #1,d0 */
                    {
                        uint32_t s = 1;
                        uint32_t d = (d0 & 0xFFFF);
                        uint32_t r = (d + s) & 0xFFFF;
                        SETW(d0, r);
                        fx = (r < s);
                    }
                    /* 003076  cmp.l  (a0)+,d1 */
                    {
                        uint32_t s = rd32(a0);
                        a0 += 4;
                        uint32_t d = d1;
                        uint32_t r = (d - s) & 0xFFFFFFFF;
                        fz = (r == 0);
                        fc = (s > d);
                    }
                    /* 003078  bhi  $308e */
                    if (COND(3)) {
                        /* 00307A  addq.w  #1,d0 */
                        {
                            uint32_t s = 1;
                            uint32_t d = (d0 & 0xFFFF);
                            uint32_t r = (d + s) & 0xFFFF;
                            SETW(d0, r);
                            fx = (r < s);
                        }
                        /* 00307C  cmp.l  (a0)+,d1 */
                        {
                            uint32_t s = rd32(a0);
                            a0 += 4;
                            uint32_t d = d1;
                            uint32_t r = (d - s) & 0xFFFFFFFF;
                            fz = (r == 0);
                            fc = (s > d);
                        }
                        /* 00307E  bhi  $308e */
                        if (COND(3)) {
                            /* 003080  addq.w  #1,d0 */
                            {
                                uint32_t s = 1;
                                uint32_t d = (d0 & 0xFFFF);
                                uint32_t r = (d + s) & 0xFFFF;
                                SETW(d0, r);
                                fx = (r < s);
                            }
                            /* 003082  cmp.l  (a0)+,d1 */
                            {
                                uint32_t s = rd32(a0);
                                a0 += 4;
                                uint32_t d = d1;
                                uint32_t r = (d - s) & 0xFFFFFFFF;
                                fz = (r == 0);
                                fc = (s > d);
                            }
                            /* 003084  bhi  $308e */
                            if (COND(3)) {
                                /* 003086  addq.w  #1,d0 */
                                {
                                    uint32_t s = 1;
                                    uint32_t d = (d0 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d0, r);
                                    fx = (r < s);
                                }
                                /* 003088  cmp.l  (a0)+,d1 */
                                {
                                    uint32_t s = rd32(a0);
                                    a0 += 4;
                                    uint32_t d = d1;
                                    uint32_t r = (d - s) & 0xFFFFFFFF;
                                    fz = (r == 0);
                                    fc = (s > d);
                                }
                                /* 00308A  bhi  $308e */
                                if (COND(3)) {
                                    /* 00308C  addq.w  #1,d0 */
                                    {
                                        uint32_t s = 1;
                                        uint32_t d = (d0 & 0xFFFF);
                                        uint32_t r = (d + s) & 0xFFFF;
                                        SETW(d0, r);
                                        fx = (r < s);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    wr16(V_01C6, (d0 & 0xFFFF));                         /* 00308E  move.w  d0,$1c6(a5) */
    RTS();                                               /* 003092  rts */
}

/* 0030AA  game routine */
void sub_0030AA(void)
{
    /* 0030AA  addq.w  #2,a1 */
    a1 += 2;
    MISS(0x30ACU);
}

/* 0030EC  game routine */
void sub_0030EC(void)
{
    /* 0030EC  subq.w  #1,(a4) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(a4);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(a4, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    MISS(0x30EEU);
}

/* 003102  game routine */
void sub_003102(void)
{
    /* 003102  addq.w  #8,a0 */
    a0 += 8;
    MISS(0x3104U);
}

/* 00312C  game routine */
void sub_00312C(void)
{
    /* 00312C  btst  d6,d0 */
    { sub_00312E(); return; }
}

/* 00312E  game routine */
void sub_00312E(void)
{
    /* 00312E  addq.w  #2,(a2) */
    {
        uint32_t s = 2;
        uint32_t d = rd16(a2);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(a2, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    /* 003130  subq.w  #4,a0 */
    a0 -= 4;
    MISS(0x3132U);
}

/* 003142  game routine */
void sub_003142(void)
{
    SETB(d0, d0 | 0x2D);                                 /* 003142  ori.b  #$2d,d0 */
    /* 003146  move.l  $2d2d(a5),-(a6) */
    {
        uint32_t v = rd32(0x2D2D);
        a6 -= 4;
        wr32(a6, v);
    }
    /* 00314A  move.l  $2d2d(a5),-(a6) */
    {
        uint32_t v = rd32(0x2D2D);
        a6 -= 4;
        wr32(a6, v);
    }
    /* 00314E  move.l  $2d2d(a5),-(a6) */
    {
        uint32_t v = rd32(0x2D2D);
        a6 -= 4;
        wr32(a6, v);
    }
    /* 003152  move.l  $2d2d(a5),-(a6) */
    {
        uint32_t v = rd32(0x2D2D);
        a6 -= 4;
        wr32(a6, v);
    }
    /* 003156  move.l  $0(a5),-(a6) */
    {
        uint32_t v = rd32(0x0);
        a6 -= 4;
        wr32(a6, v);
    }
    SETB(d0, d0 | 0);                                    /* 00315A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00315E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003162  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003166  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00316A  ori.b  #$0,d0 */
    /* 00316E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_003172(); return; }
}

/* 003172  game routine */
void sub_003172(void)
{
    SETB(d0, d0 | 0);                                    /* 003172  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003176  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00317A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00317E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003182  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003186  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00318A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00318E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003192  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 003196  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00319A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00319E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 0031A2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 0031A6  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 0031AA  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 0031AE  ori.b  #$0,d0 */
    /* 0031B2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { wait_key(); return; }
}

/* 0031B6  Waits for a key */
void wait_key(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
L_0031B6:
    /* 0031B6  move.b  $1de2(a5),d0 */
    /* 0031BA  beq  $31b6 */
    IDLE_EVERY(0x31B6U);
    SETB(d0, rd8(V_1DE2));
    if ((d0 & 0xFF) == 0) {
        BUDGET_CHECK(0x31B6U);
        goto L_0031B6;
    }
    /* 0031BC  ext.w  d0 */
    SETW(d0, SX8(d0));
    FL_LOGIC((d0 & 0xFFFF), 0xFFFF, 0x8000);
    /* 0031BE  bmi  $31e0 */
    if (COND(10)) {
        CALL(sub_00348C, 0x31C4U);                           /* 0031C0  bsr  $348c */
L_0031C4:
        /* 0031C4  tst.b  d0 */
        /* 0031C6  beq  $31e0 */
        if ((d0 & 0xFF) != 0) {
            /* 0031C8  cmp.b  #$d,d0 */
            /* 0031CC  beq  $31e6 */
            if ((d0 & 0xFF) == 0xD) {
                goto L_0031E6;
            }
            /* 0031D0  cmp.b  #$ff,d0 */
            /* 0031D4  bne  $31dc */
            if ((d0 & 0xFF) == 0xFF) {
                CALL(sub_0033EE, 0x31DAU);                           /* 0031D6  bsr  $33ee */
L_0031DA:
                /* 0031DA  bra  $31e0 */
            } else {
                CALL(sub_003414, 0x31E0U);                           /* 0031DC  bsr  $3414 */
            }
        }
    }
L_0031E0:
    wr8(V_1DE2, 0);                                      /* 0031E0  clr.b  $1de2(a5) */
    /* 0031E4  bra  $31b6 */
    BUDGET_CHECK(0x31B6U);
    goto L_0031B6;
L_0031E6:
    /* 0031E6  tst.w  $1c8(a5) */
    /* 0031EA  bne  $3210 */
    if (rd16(V_01C8) == 0) {
        SETW(d0, rd16(V_VBL_COUNTER));                       /* 0031EC  move.w  $1dc6(a5),d0 */
        SETW(d0, (d0 & 0xFFFF) >> 6);                        /* 0031F0  lsr.w  #6,d0 */
        SETW(d0, d0 & 0x1F);                                 /* 0031F2  andi.w  #$1f,d0 */
        a1 = 0x322C;                                         /* 0031F6  lea  $322c(pc),a1 */
        for (;;) {
            BUDGET_CHECK(0x31FAU);
L_0031FA:
            /* 0031FA  subq.w  #1,d0 */
            {
                uint32_t s = 1;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fn = !!(r & 0x8000);
                fx = (s > d);
            }
            /* 0031FC  bmi  $3204 */
            if (COND(11)) {
                goto L_003204;
            }
            do {
                BUDGET_CHECK(0x31FEU);
L_0031FE:
                /* 0031FE  tst.b  (a1)+ */
                {
                    uint32_t v = rd8(a1);
                    a1 += 1;
                    FL_LOGIC(v, 0xFF, 0x80);
                }
                /* 003200  bne  $31fe */
            } while (COND(6));
            /* 003202  bra  $31fa */
        }
L_003204:
        a0 = 0x3476;                                         /* 003204  lea  $3476(pc),a0 */
        for (;;) {
            BUDGET_CHECK(0x3208U);
L_003208:
            /* 003208  tst.b  (a1) */
            /* 00320A  beq  $3210 */
            if (rd8(a1) == 0) {
                goto L_003210;
            }
            /* 00320C  move.b  (a1)+,(a0)+ */
            {
                uint32_t v = rd8(a1);
                a1 += 1;
                wr8(a0, v);
                a0 += 1;
            }
            /* 00320E  bra  $3208 */
        }
    }
L_003210:
    RTS();                                               /* 003210  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x31C4U: goto L_0031C4;
        case 0x31DAU: goto L_0031DA;
        case 0x31E0U: goto L_0031E0;
        case 0x31FAU: goto L_0031FA;
        case 0x31FEU: goto L_0031FE;
        case 0x3208U: goto L_003208;
        }
    }
}

/* 00322C  game routine */
void sub_00322C(void)
{
    /* 00322C  subq.w  #1,d3 */
    {
        uint32_t s = 1;
        uint32_t d = (d3 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d3, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    /* 00322E  pea  (a7) */
    PUSH32(a7);
    MISS(0x3230U);
}

/* 0033EE  game routine */
void sub_0033EE(void)
{
    /* 0033EE  subi.w  #$1,$1c8(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_01C8);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_01C8, r);
        fn = !!(r & 0x8000);
        fx = (s > d);
    }
    /* 0033F4  bpl  $33fc */
    if (COND(11)) {
        /* 0033F6  addi.w  #$1,$1c8(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01C8);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_01C8, r);
            fx = (r < s);
        }
    }
    a0 = 0x3476;                                         /* 0033FC  lea  $3476(pc),a0 */
    SETW(d1, rd16(V_01C8));                              /* 003400  move.w  $1c8(a5),d1 */
    wr8(a0 + SX16(d1), 0x20);                            /* 003404  move.b  #$20,$0(a0,d1.w) */
    wr8(a0 + 0x1 + SX16(d1), 0x20);                      /* 00340A  move.b  #$20,$1(a0,d1.w) */
    /* 003410  bra  $3434 */
    { sub_003434(); return; }
}

/* 003414  game routine */
void sub_003414(void)
{
    a0 = 0x3476;                                         /* 003414  lea  $3476(pc),a0 */
    SETW(d1, rd16(V_01C8));                              /* 003418  move.w  $1c8(a5),d1 */
    wr8(a0 + SX16(d1), (d0 & 0xFF));                     /* 00341C  move.b  d0,$0(a0,d1.w) */
    /* 003420  addi.w  #$1,$1c8(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_01C8);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_01C8, r);
        fx = (r < s);
    }
    /* 003426  cmpi.w  #$14,$1c8(a5) */
    /* 00342C  bne  $3434 */
    if (rd16(V_01C8) != 0x14) {
        { sub_003434(); return; }
    }
    /* 00342E  subi.w  #$1,$1c8(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_01C8);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_01C8, r);
        fx = (s > d);
    }
    { sub_003434(); return; }
}

/* 003434  game routine */
void sub_003434(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, rd16(V_026A));                              /* 003434  move.w  $26a(a5),d0 */
    wr16(V_0268, (d0 & 0xFFFF));                         /* 003438  move.w  d0,$268(a5) */
    a0 = 0x3476;                                         /* 00343C  lea  $3476(pc),a0 */
    CALL(sub_008584, 0x3444U);                           /* 003440  bsr  $8584 */
L_003444:
    wr8(V_1DE2, 0);                                      /* 003444  clr.b  $1de2(a5) */
    RTS();                                               /* 003448  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3444U: goto L_003444;
        }
    }
}

/* 00344A  game routine */
void sub_00344A(void)
{
    wr8(V_1DE2, 0);                                      /* 00344A  clr.b  $1de2(a5) */
    wr16(V_01C8, 0);                                     /* 00344E  clr.w  $1c8(a5) */
    a0 = 0x3476;                                         /* 003452  lea  $3476(pc),a0 */
    /* 003456  move.l  #$20202020,(a0)+ */
    {
        uint32_t v = 0x20202020;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00345C  move.l  #$20202020,(a0)+ */
    {
        uint32_t v = 0x20202020;
        wr32(a0, v);
        a0 += 4;
    }
    /* 003462  move.l  #$20202020,(a0)+ */
    {
        uint32_t v = 0x20202020;
        wr32(a0, v);
        a0 += 4;
    }
    /* 003468  move.l  #$20202020,(a0)+ */
    {
        uint32_t v = 0x20202020;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00346E  move.l  #$20202020,(a0)+ */
    {
        uint32_t v = 0x20202020;
        wr32(a0, v);
        a0 += 4;
    }
    /* 003474  bra  $3434 */
    { sub_003434(); return; }
}

/* 003476  game routine */
void sub_003476(void)
{
    /* 003476  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003478  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00347A  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00347C  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00347E  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003480  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003482  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003484  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003486  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003488  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    SETB(d0, d0 | 0xFA);                                 /* 00348A  ori.b  #$fa,d0 */
    wr8(a0, rd8(a0) | 0x3C);                             /* 00348E  ori.b  #$3c,(a0) */
    wr8(0x401030, rd8(0x401030) | 2);                    /* 003492  ori.b  #$2,$42401030.l */
    /* 00349A  ori.b  #$75,d0 */
    SETB(d0, d0 | 0x75);
    { sub_00349E(); return; }
}

/* 00348C  game routine */
void sub_00348C(void)
{
    a0 = 0x349E;                                         /* 00348C  lea  $349e(pc),a0 */
    /* 003490  cmp.b  #$39,d0 */
    /* 003494  bls  $3498 */
    if ((d0 & 0xFF) > 0x39) {
        SETW(d0, 0);                                         /* 003496  clr.w  d0 */
    }
    SETB(d0, rd8(a0 + SX16(d0)));                        /* 003498  move.b  $0(a0,d0.w),d0 */
    RTS();                                               /* 00349C  rts */
}

/* 00349E  game routine */
void sub_00349E(void)
{
    /* 00349E  ori.b  #$32,d0 */
    {
        uint32_t s = 0x32;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x34A2U);
}

/* 0034FA  game routine */
void sub_0034FA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0034FA  bclr  #$7,d0 */
    fz = !(d0 & 0x80);
    d0 &= ~0x80;
    /* 0034FE  beq  $3508 */
    if (COND(6)) {
        SETW(d0, SX8(d0));                                   /* 003500  ext.w  d0 */
        wr16(V_026C, (d0 & 0xFFFF));                         /* 003502  move.w  d0,$26c(a5) */
        RTS();                                               /* 003506  rts */
    }
    /* 003508  cmp.b  #$d,d0 */
    /* 00350C  bne  $351e */
    if ((d0 & 0xFF) == 0xD) {
        SETW(d0, rd16(V_026A));                              /* 00350E  move.w  $26a(a5),d0 */
        wr16(V_0268, (d0 & 0xFFFF));                         /* 003512  move.w  d0,$268(a5) */
        /* 003516  addi.w  #$5a0,$266(a5) */
        wr16(V_0266, rd16(V_0266) + 0x5A0);
        { sub_00351C(); return; }
    }
    /* 00351E  subi.b  #$20,d0 */
    {
        uint32_t s = 0x20;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d - s) & 0xFF;
        SETB(d0, r);
        fn = !!(r & 0x80);
        fz = (r == 0);
    }
    /* 003522  beq  $3530 */
    if (COND(6)) {
        /* 003524  bpl  $3530 */
        if (COND(11)) {
            SETB(d0, d0 + 0x20);                                 /* 003526  addi.b  #$20,d0 */
            wr16(V_0268, rd16(V_0268) + (d0 & 0xFFFF));          /* 00352A  add.w  d0,$268(a5) */
            RTS();                                               /* 00352E  rts */
        }
    }
    SETW(d0, (d0 & 0xFFFF) << 3);                        /* 003530  lsl.w  #3,d0 */
    a1 = 0x8700;                                         /* 003532  lea  $8700(pc),a1 */
    a1 = a1 + SX16(d0);                                  /* 003536  lea  $0(a1,d0.w),a1 */
    a2 = 0x20000;                                        /* 00353A  lea  $20000.l,a2 */
    d0 = 0;                                              /* 003540  clr.l  d0 */
    SETW(d0, rd16(V_0266));                              /* 003542  move.w  $266(a5),d0 */
    a2 = a2 + SX16(d0);                                  /* 003546  lea  $0(a2,d0.w),a2 */
    SETW(d0, rd16(V_0268));                              /* 00354A  move.w  $268(a5),d0 */
    d1 = 8;                                              /* 00354E  moveq  #8,d1 */
    SETW(d2, (d0 & 0xFFFF));                             /* 003550  move.w  d0,d2 */
    SETW(d0, d0 & 0xFE);                                 /* 003552  andi.w  #$fe,d0 */
    SETW(d0, (d0 & 0xFFFF) << 2);                        /* 003556  lsl.w  #2,d0 */
    a2 = a2 + SX16(d0);                                  /* 003558  lea  $0(a2,d0.w),a2 */
    fz = !(d2 & 0x1);                                    /* 00355C  btst  #$0,d2 */
    /* 003560  beq  $3564 */
    if (COND(6)) {
        a2 += 1;                                             /* 003562  addq.l  #1,a2 */
    }
    do {
        BUDGET_CHECK(0x3564U);
L_003564:
        SETW(d0, 0);                                         /* 003564  clr.w  d0 */
        SETB(d0, rd8(a1));                                   /* 003566  move.b  (a1),d0 */
        SETW(d0, (d0 & 0xFFFF) << 8);                        /* 003568  lsl.w  #8,d0 */
        SETB(d0, rd8(a1));                                   /* 00356A  move.b  (a1),d0 */
        d0 = (d0 << 16) | (d0 >> 16);                        /* 00356C  swap  d0 */
        SETB(d0, rd8(a1));                                   /* 00356E  move.b  (a1),d0 */
        SETW(d0, (d0 & 0xFFFF) << 8);                        /* 003570  lsl.w  #8,d0 */
        /* 003572  move.b  (a1)+,d0 */
        SETB(d0, rd8(a1));
        a1 += 1;
        a2 -= 0x8000;                                        /* 003574  suba.l  #$8000,a2 */
        d5 = (rd8(a2 + 0) << 24) | (rd8(a2 + 0 + 2) << 16) | (rd8(a2 + 0 + 4) << 8) | rd8(a2 + 0 + 6); /* 00357A  movep.l  0(a2),d5 */
        a2 += 0x8000;                                        /* 00357E  adda.l  #$8000,a2 */
        d0 = ~d0;                                            /* 003584  not.l  d0 */
        d5 &= d0;                                            /* 003586  and.l  d0,d5 */
        d0 = ~d0;                                            /* 003588  not.l  d0 */
        SETW(d7, rd16(V_026C));                              /* 00358A  move.w  $26c(a5),d7 */
        fz = !(d7 & 0x1);                                    /* 00358E  btst  #$0,d7 */
        /* 003592  bne  $359a */
        if (COND(7)) {
            d0 &= 0xFFFFFF;                                      /* 003594  andi.l  #$ffffff,d0 */
        }
        fz = !(d7 & 0x2);                                    /* 00359A  btst  #$1,d7 */
        /* 00359E  bne  $35a6 */
        if (COND(7)) {
            d0 &= 0xFF00FFFF;                                    /* 0035A0  andi.l  #$ff00ffff,d0 */
        }
        fz = !(d7 & 0x4);                                    /* 0035A6  btst  #$2,d7 */
        /* 0035AA  bne  $35b0 */
        if (COND(7)) {
            SETW(d0, d0 & 0xFF);                                 /* 0035AC  andi.w  #$ff,d0 */
        }
        fz = !(d7 & 0x8);                                    /* 0035B0  btst  #$3,d7 */
        /* 0035B4  bne  $35b8 */
        if (COND(7)) {
            SETB(d0, 0);                                         /* 0035B6  clr.b  d0 */
        }
        d5 |= d0;                                            /* 0035B8  or.l  d0,d5 */
        /* 0035BA  movep.l  d5,0(a2) */
        wr8(a2 + 0 + 0, d5 >> 24);
        wr8(a2 + 0 + 2, d5 >> 16);
        wr8(a2 + 0 + 4, d5 >> 8);
        wr8(a2 + 0 + 6, d5 >> 0);
        a2 = a2 + 0xA0;                                      /* 0035BE  lea  $a0(a2),a2 */
        /* 0035C2  subq.b  #1,d1 */
        {
            uint32_t s = 1;
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d1, r);
            fz = (r == 0);
        }
        /* 0035C4  bne  $3564 */
    } while (COND(6));
    wr16(V_0268, rd16(V_0268) + 1);                      /* 0035C6  addq.w  #1,$268(a5) */
    RTS();                                               /* 0035CA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3564U: goto L_003564;
        }
    }
}

/* 00351C  game routine */
void sub_00351C(void)
{
    RTS();                                               /* 00351C  rts */
}

/* 0035CC  game routine */
void sub_0035CC(void)
{
    /* 0035CC  btst  d6,d0 */
    { sub_0035CE(); return; }
}

/* 0035CE  game routine */
void sub_0035CE(void)
{
    /* 0035CE  ori.b  #$60,d0 */
    {
        uint32_t s = 0x60;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        fn = !!(r & 0x80);
        fv = 0;
        fc = 0;
    }
    /* 0035D2  bchg  d3,-(a0) */
    a0 -= 1;
    wr8(a0, rd8(a0) ^ (1u << (d3 & 7)));
    { sub_0035D4(); return; }
}

/* 0035D4  game routine */
void sub_0035D4(void)
{
    /* 0035D4  bchg  d3,d0 */
    fz = !(d0 & (1u << (d3 & 31)));
    d0 ^= (1u << (d3 & 31));
    MISS(0x35D6U);
}

/* 0035EE  game routine */
void sub_0035EE(void)
{
    SETB(d0, d0 - 0);                                    /* 0035EE  subi.b  #$0,d0 */
    /* 0035F2  addi.b  #$0,d0 */
    {
        uint32_t s = 0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d + s) & 0xFF;
        SETB(d0, r);
        fn = !!(r & 0x80);
        fv = !!((~(s ^ d) & (s ^ r)) & 0x80);
        fc = (r < s);
        fx = fc;
    }
    /* 0035F6  btst  d3,(a0) */
    /* 0035F8  btst  d3,-(a0) */
    a0 -= 1;
    fz = !(rd8(a0) & (1u << (d3 & 7)));
    MISS(0x35FAU);
}

/* 00360C  game routine */
void sub_00360C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00360C  move.l  d0,-(a7) */
    {
        uint32_t v = d0;
        a7 -= 4;
        wr32(a7, v);
    }
    do {
        BUDGET_CHECK(0x360EU);
L_00360E:
        CALL(sub_007576, 0x3612U);                           /* 00360E  bsr  $7576 */
L_003612:
        SETW(d0, d0 & 0xF);                                  /* 003612  andi.w  #$f,d0 */
        /* 003616  cmp.w  #$7,d0 */
        /* 00361A  bhi  $360e */
    } while ((d0 & 0xFFFF) > 7);
    a1 = 0x362E;                                         /* 00361C  lea  $362e(pc),a1 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 003620  lsl.w  #1,d0 */
    SETW(d0, rd16(a1 + SX16(d0)));                       /* 003622  move.w  $0(a1,d0.w),d0 */
    a0 = 0x3640 + SX16(d0);                              /* 003626  lea  $3640(pc,d0.w),a0 */
    /* 00362A  move.l  (a7)+,d0 */
    d0 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 00362C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x360EU: goto L_00360E;
        case 0x3612U: goto L_003612;
        }
    }
}

/* 00362E  game routine */
void sub_00362E(void)
{
    SETB(d0, d0 | 0x1C);                                 /* 00362E  ori.b  #$1c,d0 */
    wr8(0x72008B, rd8(0x72008B) | 0x56);                 /* 003632  ori.b  #$56,$72008b.l */
    /* 00363A  ori.l  #$c000db,-(a4) */
    {
        uint32_t s = 0xC000DB;
        a4 -= 4;
        uint32_t d = rd32(a4);
        uint32_t r = (d | s) & 0xFFFFFFFF;
        wr32(a4, r);
    }
    /* 003640  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003642  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003644  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003646  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003648  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00364A  move.l  -(a0),d0 */
    a0 -= 4;
    {
        uint32_t v = rd32(a0);
        d0 = v;
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
    /* 00364C  movea.l  (a0),a0 */
    a0 = rd32(a0);
    MISS(0x364EU);
}

/* 00373A  game routine */
void sub_00373A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_007626, 0x373EU);                           /* 00373A  bsr  $7626 */
L_00373E:
    /* 00373E  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_003742:
    CALL(sub_0083B2, 0x3746U);                           /* 003742  bsr  $83b2 */
L_003746:
    a5 -= a5;                                            /* 003746  suba.l  a5,a5 */
    a7 = 0x2400;                                         /* 003748  lea  $2400.l,a7 */
    a0 = V_01B8;                                         /* 00374E  lea  $1b8(a5),a0 */
    a1 = V_1E0E;                                         /* 003752  lea  $1e0e(a5),a1 */
    CALL(sub_00759C, 0x375AU);                           /* 003756  bsr  $759c */
L_00375A:
    /* 00375A  move.l  #$0,$160(a5) */
    wr32(V_0160, 0);
    { sub_003762(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x373EU: goto L_00373E;
        case 0x3742U: goto L_003742;
        case 0x3746U: goto L_003746;
        case 0x375AU: goto L_00375A;
        }
    }
}

/* 003762  game routine */
void sub_003762(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_007626, 0x3766U);                           /* 003762  bsr  $7626 */
L_003766:
    wr32(V_DRAW_BUFFER, 0x20000);                        /* 003766  move.l  #$20000,$28e(a5) */
    a0 = 0x8258;                                         /* 00376E  lea  $8258(pc),a0 */
    wr32(V_0256, a0);                                    /* 003772  move.l  a0,$256(a5) */
    CALL(sub_00765A, 0x377AU);                           /* 003776  bsr  $765a */
L_00377A:
    /* 00377A  move  #$2300,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2300U;
    set_sr(s); }
L_00377E:
    CALL(sub_005FFE, 0x3782U);                           /* 00377E  bsr  $5ffe */
L_003782:
    CALL(sub_008938, 0x3786U);                           /* 003782  bsr  $8938 */
L_003786:
    d7 = rd32(V_0160);                                   /* 003786  move.l  $160(a5),d7 */
    /* 00378A  tst.l  d7 */
    /* 00378C  bne  $382e */
    if (d7 != 0) {
        { sub_00382E(); return; }
    }
L_003790:
    CALL(sub_007626, 0x3794U);                           /* 003790  bsr  $7626 */
L_003794:
    a1 = 0x8C24;                                         /* 003794  lea  $8c24(pc),a1 */
    wr16(a1 + 0x1C, 0x25);                               /* 003798  move.w  #$25,$1c(a1) */
    CALL(sub_0075E2, 0x37A2U);                           /* 00379E  bsr  $75e2 */
L_0037A2:
    wr16(V_0266, 0x1900);                                /* 0037A2  move.w  #$1900,$266(a5) */
    wr16(V_0268, 0);                                     /* 0037A8  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 0037AE  move.w  #$0,$26a(a5) */
    wr16(V_026C, 2);                                     /* 0037B4  move.w  #$2,$26c(a5) */
    a0 = 0x37C6;                                         /* 0037BA  lea  $37c6(pc),a0 */
    CALL(sub_008934, 0x37C2U);                           /* 0037BE  bsr  $8934 */
L_0037C2:
    /* 0037C2  bra  $88f8 */
L_0088F8:
    /* 0088F8  bra  $ebf6 */
L_00EBF6:
    wr8(V_029C, 0xFF);                                   /* 00EBF6  st  $29c(a5) */
    /* 00EBFA  bsr  $ec40 */
    CALL(sub_00EC40, 0xEBFEU);
    { wait_fire(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3766U: goto L_003766;
        case 0x377AU: goto L_00377A;
        case 0x377EU: goto L_00377E;
        case 0x3782U: goto L_003782;
        case 0x3786U: goto L_003786;
        case 0x3790U: goto L_003790;
        case 0x3794U: goto L_003794;
        case 0x37A2U: goto L_0037A2;
        case 0x37C2U: goto L_0037C2;
        case 0x88F8U: goto L_0088F8;
        case 0xEBF6U: goto L_00EBF6;
        }
    }
}

/* 0037C6  game routine */
void sub_0037C6(void)
{
    /* 0037C6  or.b  -(a0),d3 */
    a0 -= 1;
    SETB(d3, d3 | rd8(a0));
    /* 0037C8  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 0037CA  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 0037CC  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 0037CE  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 0037D0  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 0037D2  move.l  -(a0),d0 */
    a0 -= 4;
    {
        uint32_t v = rd32(a0);
        d0 = v;
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
    /* 0037D4  movea.l  d4,a0 */
    a0 = d4;
    MISS(0x37D6U);
}

/* 00382E  game routine */
void sub_00382E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_007626, 0x3832U);                           /* 00382E  bsr  $7626 */
L_003832:
    wr16(V_0202, 0);                                     /* 003832  clr.w  $202(a5) */
    wr16(V_0200, 0);                                     /* 003836  clr.w  $200(a5) */
    CALL(sub_00AF9E, 0x383EU);                           /* 00383A  bsr  $af9e */
L_00383E:
    CALL(sub_00751A, 0x3842U);                           /* 00383E  bsr  $751a */
L_003842:
    CALL(sub_007626, 0x3846U);                           /* 003842  bsr  $7626 */
L_003846:
    /* 003846  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_00384A:
    a7 = 0x2400;                                         /* 00384A  lea  $2400.l,a7 */
    a5 -= a5;                                            /* 003850  suba.l  a5,a5 */
    wr16(V_029C, 0);                                     /* 003852  clr.w  $29c(a5) */
    wr16(V_SECTION, 0);                                  /* 003856  clr.w  $21a(a5) */
    CALL(sub_009048, 0x385EU);                           /* 00385A  bsr  $9048 */
L_00385E:
    CALL(sub_0090C2, 0x3862U);                           /* 00385E  bsr  $90c2 */
L_003862:
    /* 003862  move  #$2300,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2300U;
    set_sr(s); }
    for (;;) {
        BUDGET_CHECK(0x3866U);
L_003866:
        CALL(sub_009048, 0x386AU);                           /* 003866  bsr  $9048 */
L_00386A:
        CALL(sub_0091D0, 0x386EU);                           /* 00386A  bsr  $91d0 */
L_00386E:
        CALL(sub_009134, 0x3872U);                           /* 00386E  bsr  $9134 */
L_003872:
        CALL(sub_008914, 0x3876U);                           /* 003872  bsr  $8914 */
L_003876:
        CALL(sub_0086BC, 0x387AU);                           /* 003876  bsr  $86bc */
L_00387A:
        CALL(sub_007626, 0x387EU);                           /* 00387A  bsr  $7626 */
L_00387E:
        wr16(V_0266, 0x3840);                                /* 00387E  move.w  #$3840,$266(a5) */
        wr16(V_0268, 9);                                     /* 003884  move.w  #$9,$268(a5) */
        wr16(V_026A, 9);                                     /* 00388A  move.w  #$9,$26a(a5) */
        wr16(V_026C, 1);                                     /* 003890  move.w  #$1,$26c(a5) */
        a0 = 0x3924;                                         /* 003896  lea  $3924(pc),a0 */
        CALL(sub_008584, 0x389EU);                           /* 00389A  bsr  $8584 */
L_00389E:
        CALL(sub_008910, 0x38A2U);                           /* 00389E  bsr  $8910 */
L_0038A2:
        CALL(sub_009062, 0x38A6U);                           /* 0038A2  bsr  $9062 */
L_0038A6:
        CALL(sub_007626, 0x38AAU);                           /* 0038A6  bsr  $7626 */
L_0038AA:
        CALL(sub_008C64, 0x38AEU);                           /* 0038AA  bsr  $8c64 */
L_0038AE:
        wr8(V_1DF2, 0xFF);                                   /* 0038AE  st  $1df2(a5) */
        wr16(V_MISSION_END, 0);                              /* 0038B2  clr.w  $206(a5) */
        d0 = 0;                                              /* 0038B6  moveq  #0,d0 */
        CALL(sub_0074F4, 0x38BCU);                           /* 0038B8  bsr  $74f4 */
L_0038BC:
        wr8(0x42502, 0xD);                                   /* 0038BC  move.b  #$d,$42502.l */
        CALL(sub_00868A, 0x38C8U);                           /* 0038C4  bsr  $868a */
L_0038C8:
        wr8(V_1DE2, 0);                                      /* 0038C8  clr.b  $1de2(a5) */
        CALL(frame_vertical, 0x38D0U);                       /* 0038CC  bsr  $8eac */
L_0038D0:
        wr8(0x42502, 0xF);                                   /* 0038D0  move.b  #$f,$42502.l */
        /* 0038D8  tst.w  $206(a5) */
        /* 0038DC  bne  $393c */
        if (rd16(V_MISSION_END) == 0) {
            CALL(sub_00A914, 0x38E2U);                           /* 0038DE  bsr  $a914 */
L_0038E2:
            wr16(V_01B6, 0);                                     /* 0038E2  clr.w  $1b6(a5) */
            CALL(sub_002D9A, 0x38EAU);                           /* 0038E6  bsr  $2d9a */
L_0038EA:
            d7 = 0x895440;                                       /* 0038EA  move.l  #$895440,d7 */
            CALL(delay_loop, 0x38F4U);                           /* 0038F0  bsr  $75ae */
L_0038F4:
            wr32(V_0160, d7);                                    /* 0038F4  move.l  d7,$160(a5) */
            wr8(V_01B6, (d1 & 0xFF));                            /* 0038F8  move.b  d1,$1b6(a5) */
            fz = !(d1 & 0x80);                                   /* 0038FC  btst  #$7,d1 */
            /* 003900  bne  $390a */
            if (COND(7)) {
                wr16(V_01B4, 0);                                     /* 003902  clr.w  $1b4(a5) */
                /* 003906  bra  $3910 */
            } else {
                wr16(V_01B4, 0xFFFF);                                /* 00390A  move.w  #$ffff,$1b4(a5) */
            }
            CALL(sub_00751A, 0x3914U);                           /* 003910  bsr  $751a */
L_003914:
            a0 = V_01B8;                                         /* 003914  lea  $1b8(a5),a0 */
            a1 = V_1E0E;                                         /* 003918  lea  $1e0e(a5),a1 */
            CALL(sub_00759C, 0x3920U);                           /* 00391C  bsr  $759c */
L_003920:
            /* 003920  bra  $3762 */
            { sub_003762(); return; }
        }
        /* 00393C  cmpi.l  #$18000,$28e(a5) */
        /* 003944  beq  $3956 */
        if (rd32(V_DRAW_BUFFER) != 0x18000) {
            a1 = 0x18000;                                        /* 003946  lea  $18000.l,a1 */
            a0 = 0x20000;                                        /* 00394C  lea  $20000.l,a0 */
            CALL(sub_007564, 0x3956U);                           /* 003952  bsr  $7564 */
        }
L_003956:
        CALL(sub_007558, 0x395AU);                           /* 003956  bsr  $7558 */
L_00395A:
        d0 = 0x20000;                                        /* 00395A  move.l  #$20000,d0 */
        wr32(V_DRAW_BUFFER, d0);                             /* 003960  move.l  d0,$28e(a5) */
        CALL(sub_007B2A, 0x3968U);                           /* 003964  bsr  $7b2a */
L_003968:
        CALL(sub_008924, 0x396CU);                           /* 003968  bsr  $8924 */
L_00396C:
        d0 = 3;                                              /* 00396C  moveq  #3,d0 */
        CALL(sub_0074F4, 0x3972U);                           /* 00396E  bsr  $74f4 */
        do {
            BUDGET_CHECK(0x3972U);
L_003972:
            /* 003972  tst.b  $424f2.l */
            /* 003978  bne  $3972 */
            IDLE_EVERY(0x3972U);
        } while (rd8(0x424F2) != 0);
        CALL(sub_008528, 0x397EU);                           /* 00397A  bsr  $8528 */
L_00397E:
        SETW(d0, rd16(V_021C));                              /* 00397E  move.w  $21c(a5),d0 */
        a0 = V_023A;                                         /* 003982  lea  $23a(a5),a0 */
        wr8(a0 + SX16(d0), 0xFF);                            /* 003986  move.b  #$ff,$0(a0,d0.w) */
        a0 = V_023A;                                         /* 00398C  lea  $23a(a5),a0 */
        SETW(d7, 0xB);                                       /* 003990  move.w  #$b,d7 */
        do {
            BUDGET_CHECK(0x3994U);
L_003994:
            /* 003994  tst.b  (a0)+ */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                FL_LOGIC(v, 0xFF, 0x80);
            }
            /* 003996  beq  $39a0 */
            if (COND(7)) {
                goto L_0039A0;
            }
            /* 003998  dbra  d7,$3994 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
        /* 00399C  bra  $39a4 */
        goto L_0039A4;
L_0039A0:
        /* 0039A0  bra  $3866 */
    }
L_0039A4:
    d0 = 0x20000;                                        /* 0039A4  move.l  #$20000,d0 */
    CALL(sub_007626, 0x39AEU);                           /* 0039AA  bsr  $7626 */
L_0039AE:
    a1 = 0x8C24;                                         /* 0039AE  lea  $8c24(pc),a1 */
    CALL(sub_0075E2, 0x39B6U);                           /* 0039B2  bsr  $75e2 */
L_0039B6:
    wr16(V_026C, 8);                                     /* 0039B6  move.w  #$8,$26c(a5) */
    wr16(V_0266, 0x3980);                                /* 0039BC  move.w  #$3980,$266(a5) */
    wr16(V_0268, 0);                                     /* 0039C2  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 0039C8  move.w  #$0,$26a(a5) */
    a0 = 0x3A3C;                                         /* 0039CE  lea  $3a3c(pc),a0 */
    CALL(sub_008934, 0x39D6U);                           /* 0039D2  bsr  $8934 */
L_0039D6:
    d7 = 0x7A120;                                        /* 0039D6  move.l  #$7a120,d7 */
    CALL(sub_0075A4, 0x39E0U);                           /* 0039DC  bsr  $75a4 */
L_0039E0:
    d7 = 0x895440;                                       /* 0039E0  move.l  #$895440,d7 */
    CALL(delay_loop, 0x39EAU);                           /* 0039E6  bsr  $75ae */
L_0039EA:
    CALL(sub_002D9A, 0x39EEU);                           /* 0039EA  bsr  $2d9a */
L_0039EE:
    d7 = 0x895440;                                       /* 0039EE  move.l  #$895440,d7 */
    CALL(delay_loop, 0x39F8U);                           /* 0039F4  bsr  $75ae */
L_0039F8:
    CALL(sub_00751A, 0x39FCU);                           /* 0039F8  bsr  $751a */
L_0039FC:
    d0 = 0x20000;                                        /* 0039FC  move.l  #$20000,d0 */
    CALL(sub_007626, 0x3A06U);                           /* 003A02  bsr  $7626 */
L_003A06:
    a1 = 0x8C24;                                         /* 003A06  lea  $8c24(pc),a1 */
    CALL(sub_0075E2, 0x3A0EU);                           /* 003A0A  bsr  $75e2 */
L_003A0E:
    wr16(V_026C, 8);                                     /* 003A0E  move.w  #$8,$26c(a5) */
    wr16(V_0266, 0x3980);                                /* 003A14  move.w  #$3980,$266(a5) */
    wr16(V_0268, 0);                                     /* 003A1A  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 003A20  move.w  #$0,$26a(a5) */
    a0 = 0x3A3C;                                         /* 003A26  lea  $3a3c(pc),a0 */
    CALL(sub_008934, 0x3A2EU);                           /* 003A2A  bsr  $8934 */
L_003A2E:
    d7 = 0x7A120;                                        /* 003A2E  move.l  #$7a120,d7 */
    CALL(sub_0075A4, 0x3A38U);                           /* 003A34  bsr  $75a4 */
L_003A38:
    /* 003A38  bra  $373a */
    { sub_00373A(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3832U: goto L_003832;
        case 0x383EU: goto L_00383E;
        case 0x3842U: goto L_003842;
        case 0x3846U: goto L_003846;
        case 0x384AU: goto L_00384A;
        case 0x385EU: goto L_00385E;
        case 0x3862U: goto L_003862;
        case 0x3866U: goto L_003866;
        case 0x386AU: goto L_00386A;
        case 0x386EU: goto L_00386E;
        case 0x3872U: goto L_003872;
        case 0x3876U: goto L_003876;
        case 0x387AU: goto L_00387A;
        case 0x387EU: goto L_00387E;
        case 0x389EU: goto L_00389E;
        case 0x38A2U: goto L_0038A2;
        case 0x38A6U: goto L_0038A6;
        case 0x38AAU: goto L_0038AA;
        case 0x38AEU: goto L_0038AE;
        case 0x38BCU: goto L_0038BC;
        case 0x38C8U: goto L_0038C8;
        case 0x38D0U: goto L_0038D0;
        case 0x38E2U: goto L_0038E2;
        case 0x38EAU: goto L_0038EA;
        case 0x38F4U: goto L_0038F4;
        case 0x3914U: goto L_003914;
        case 0x3920U: goto L_003920;
        case 0x3956U: goto L_003956;
        case 0x395AU: goto L_00395A;
        case 0x3968U: goto L_003968;
        case 0x396CU: goto L_00396C;
        case 0x3972U: goto L_003972;
        case 0x397EU: goto L_00397E;
        case 0x3994U: goto L_003994;
        case 0x39AEU: goto L_0039AE;
        case 0x39B6U: goto L_0039B6;
        case 0x39D6U: goto L_0039D6;
        case 0x39E0U: goto L_0039E0;
        case 0x39EAU: goto L_0039EA;
        case 0x39EEU: goto L_0039EE;
        case 0x39F8U: goto L_0039F8;
        case 0x39FCU: goto L_0039FC;
        case 0x3A06U: goto L_003A06;
        case 0x3A0EU: goto L_003A0E;
        case 0x3A2EU: goto L_003A2E;
        case 0x3A38U: goto L_003A38;
        }
    }
}

/* 003A3C  game routine */
void sub_003A3C(void)
{
    /* 003A3C  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003A3E  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003A40  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003A42  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003A44  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 003A46  move.l  -(a0),d0 */
    a0 -= 4;
    {
        uint32_t v = rd32(a0);
        d0 = v;
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
    MISS(0x3A48U);
}

/* 003A8E  game routine */
void sub_003A8E(void)
{
    SETB(d0, d0 | 0x15);                                 /* 003A8E  ori.b  #$15,d0 */
    wr8(a2 + 0x49, rd8(a2 + 0x49) | 0x3E);               /* 003A92  ori.b  #$3e,$49(a2) */
    /* 003A98  ori.w  #$82,-(a3) */
    {
        uint32_t s = 0x82;
        a3 -= 2;
        uint32_t d = rd16(a3);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a3, r);
    }
    /* 003A9C  ori.l  #$c000df,(a4)+ */
    {
        uint32_t s = 0xC000DF;
        uint32_t ea = a4;
        a4 += 4;
        uint32_t d = rd32(ea);
        uint32_t r = (d | s) & 0xFFFFFFFF;
        wr32(ea, r);
        FL_LOGIC(r, 0xFFFFFFFF, 0x80000000);
    }
    MISS(0x3AA2U);
}

/* 003B6E  game routine */
void sub_003B6E(void)
{
    /* 003B6E  ori.b  #$0,d1 */
    {
        uint32_t s = 0;
        uint32_t d = (d1 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d1, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x3B72U);
}

