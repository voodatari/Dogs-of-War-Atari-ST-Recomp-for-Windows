/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 00C012  game routine */
void sub_00C012(void)
{
    /* 00C012  bclr  d2,(a6)+ */
    {
        uint32_t ea = a6;
        a6 += 1;
        fz = !(rd8(ea) & (1u << (d2 & 7)));
        wr8(ea, rd8(ea) & ~(1u << (d2 & 7)));
    }
    MISS(0xC014U);
}

/* 00C02A  game routine */
void sub_00C02A(void)
{
    /* 00C02A  bchg  #$4e,d3 */
    fz = !(d3 & 0x4000);
    d3 ^= 0x4000;
    /* 00C02E  movea.l  (a1)+,a0 */
    a0 = rd32(a1);
    a1 += 4;
    MISS(0xC030U);
}

/* 00C70E  game routine */
void sub_00C70E(void)
{
    SETB(d0, d0 | 0x77);                                 /* 00C70E  ori.b  #$77,d0 */
    wr8(a2, rd8(a2) | 0x14);                             /* 00C712  ori.b  #$14,(a2) */
    /* 00C716  ori.b  #$36,-(a5) */
    {
        uint32_t s = 0x36;
        a5 -= 1;
        uint32_t d = rd8(a5);
        uint32_t r = (d | s) & 0xFF;
        wr8(a5, r);
    }
    SETW(d6, d6 & 0x356);                                /* 00C71A  andi.w  #$356,d6 */
    /* 00C71E  subi.w  #$740,-(a7) */
    {
        uint32_t s = 0x740;
        a7 -= 2;
        uint32_t d = rd16(a7);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(a7, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    MISS(0xC722U);
}

/* 00C72E  game routine */
void sub_00C72E(void)
{
    SETB(d0, d0 | 0x76);                                 /* 00C72E  ori.b  #$76,d0 */
    wr8(a4, rd8(a4) ^ (1u << (d3 & 7)));                 /* 00C732  bchg  d3,(a4) */
    SETW(d2, d2 + 0x532);                                /* 00C734  addi.w  #$532,d2 */
    /* 00C738  subi.b  #$10,-(a1) */
    {
        uint32_t s = 0x10;
        a1 -= 1;
        uint32_t d = rd8(a1);
        uint32_t r = (d - s) & 0xFF;
        wr8(a1, r);
        fx = (s > d);
    }
    /* 00C73C  andi.b  #$0,d0 */
    {
        uint32_t s = 0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d & s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xC740U);
}

/* 00C74E  game routine */
void sub_00C74E(void)
{
    SETB(d0, d0 | 0);                                    /* 00C74E  ori.b  #$0,d0 */
    /* 00C752  btst  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C756  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C75A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C75E  ori.b  #$0,d0 */
    /* 00C762  btst  #$0,d0 */
    /* 00C766  btst  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C76A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C76E  ori.b  #$0,d0 */
    SETB(d6, (d0 & 0xFF));                               /* 00C772  move.b  d0,d6 */
    SETW(d7, (d0 & 0xFFFF));                             /* 00C774  move.w  d0,d7 */
    SETW(d7, (d0 & 0xFFFF));                             /* 00C776  move.w  d0,d7 */
    SETW(d7, (d0 & 0xFFFF));                             /* 00C778  move.w  d0,d7 */
    SETB(d6, (d0 & 0xFF));                               /* 00C77A  move.b  d0,d6 */
    SETB(d0, d0 | 0);                                    /* 00C77C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C780  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C784  ori.b  #$0,d0 */
    SETB(d6, (d0 & 0xFF));                               /* 00C788  move.b  d0,d6 */
    /* 00C78A  btst  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C78E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C792  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C796  ori.b  #$0,d0 */
    /* 00C79A  btst  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C79E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C7A2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C7A6  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C7AA  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C7AE  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00C7B2  ori.b  #$0,d0 */
    /* 00C7B6  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_00C7BA(); return; }
}

/* 00C7BA  game routine */
void sub_00C7BA(void)
{
    wr32(a2 + 0x4B + SX16(d0), rd32(a2 + 0x4B + SX16(d0)) | 0x48009F); /* 00C7BA  ori.l  #$48009f,$4b(a2,d0.w) */
    /* 00C7C2  ori.l  #$520072,-(a3) */
    {
        uint32_t s = 0x520072;
        a3 -= 4;
        uint32_t d = rd32(a3);
        uint32_t r = (d | s) & 0xFFFFFFFF;
        wr32(a3, r);
    }
    wr16(a3, rd16(a3) | 0xA9);                           /* 00C7C8  ori.w  #$a9,(a3) */
    /* 00C7CC  ori.w  #$59,(a1)+ */
    {
        uint32_t s = 0x59;
        uint32_t ea = a1;
        a1 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(ea, r);
    }
    /* 00C7D0  ori.w  #$65,-(a3) */
    {
        uint32_t s = 0x65;
        a3 -= 2;
        uint32_t d = rd16(a3);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a3, r);
    }
    /* 00C7D4  ori.w  #$cd,-(a4) */
    {
        uint32_t s = 0xCD;
        a4 -= 2;
        uint32_t d = rd16(a4);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a4, r);
    }
    /* 00C7D8  ori.w  #$f3,-(a5) */
    {
        uint32_t s = 0xF3;
        a5 -= 2;
        uint32_t d = rd16(a5);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a5, r);
    }
    /* 00C7DC  ori.w  #$68,$87(a6) */
    {
        uint32_t s = 0x68;
        uint32_t d = rd16(a6 + 0x87);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a6 + 0x87, r);
        FL_LOGIC(r, 0xFFFF, 0x8000);
    }
    MISS(0xC7E2U);
}

/* 00C7EA  game routine */
void sub_00C7EA(void)
{
    SETB(d0, d0 | 0xC8);                                 /* 00C7EA  ori.b  #$c8,d0 */
    SETB(d0, d0 | 0x10);                                 /* 00C7EE  ori.b  #$10,d0 */
    SETB(d0, d0 | 0xC8);                                 /* 00C7F2  ori.b  #$c8,d0 */
    SETB(d0, d0 | 0x58);                                 /* 00C7F6  ori.b  #$58,d0 */
    SETB(d0, d0 | 0x70);                                 /* 00C7FA  ori.b  #$70,d0 */
    SETB(d0, d0 | 0xE0);                                 /* 00C7FE  ori.b  #$e0,d0 */
    SETB(d0, d0 | 0x98);                                 /* 00C802  ori.b  #$98,d0 */
    SETB(d0, d0 | 0x70);                                 /* 00C806  ori.b  #$70,d0 */
    SETB(d0, d0 | 0xB0);                                 /* 00C80A  ori.b  #$b0,d0 */
    SETB(d0, d0 | 0x88);                                 /* 00C80E  ori.b  #$88,d0 */
    SETB(d0, d0 | 0x88);                                 /* 00C812  ori.b  #$88,d0 */
    /* 00C816  ori.b  #$28,d0 */
    SETB(d0, d0 | 0x28);
    { sub_00C81A(); return; }
}

/* 00C81A  game routine */
void sub_00C81A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1CF8;                                         /* 00C81A  lea  $1cf8(a5),a6 */
    SETW(d7, 0xB);                                       /* 00C81E  move.w  #$b,d7 */
    do {
        BUDGET_CHECK(0xC822U);
L_00C822:
        /* 00C822  tst.w  $6(a6) */
        /* 00C826  beq  $c874 */
        if (rd16(a6 + 0x6) != 0) {
L_00C828:
            SETW(d1, rd16(a6));                                  /* 00C828  move.w  (a6),d1 */
            SETW(d2, rd16(a6 + 0x2));                            /* 00C82A  move.w  $2(a6),d2 */
            SETW(d3, rd16(a6 + 0x4));                            /* 00C82E  move.w  $4(a6),d3 */
            /* 00C832  tst.w  $e(a6) */
            /* 00C836  bne  $c844 */
            if (rd16(a6 + 0xE) == 0) {
L_00C838:
                /* 00C838  movem.l  a6/d7,-(a7) */
                PUSH32(a6);
                PUSH32(d7);
                CALL(sub_00C87E, 0xC840U);                           /* 00C83C  bsr  $c87e */
L_00C840:
                /* 00C840  movem.l  (a7)+,d7/a6 */
                d7 = POP32();
                a6 = POP32();
            }
L_00C844:
            /* 00C844  tst.w  $e(a6) */
            /* 00C848  beq  $c850 */
            if (rd16(a6 + 0xE) != 0) {
L_00C84A:
                /* 00C84A  subq.w  #1,$e(a6) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a6 + 0xE);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(a6 + 0xE, r);
                    fx = (s > d);
                }
                /* 00C84E  bra  $c874 */
            } else {
L_00C850:
                /* 00C850  subq.w  #1,$c(a6) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a6 + 0xC);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(a6 + 0xC, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 00C854  bne  $c874 */
                if (COND(7)) {
L_00C856:
                    wr16(a6 + 0xC, 6);                                   /* 00C856  move.w  #$6,$c(a6) */
                    /* 00C85C  addi.w  #$1,$4(a6) */
                    {
                        uint32_t s = 1;
                        uint32_t d = rd16(a6 + 0x4);
                        uint32_t r = (d + s) & 0xFFFF;
                        wr16(a6 + 0x4, r);
                        fx = (r < s);
                    }
                    /* 00C862  cmpi.w  #$5,$4(a6) */
                    /* 00C868  bls  $c874 */
                    if (rd16(a6 + 0x4) > 5) {
L_00C86A:
                        wr16(a6 + 0x4, 0);                                   /* 00C86A  clr.w  $4(a6) */
                        wr16(a6 + 0xE, 0x28);                                /* 00C86E  move.w  #$28,$e(a6) */
                    }
                }
            }
        }
L_00C874:
        a6 = a6 + 0x10;                                      /* 00C874  lea  $10(a6),a6 */
        /* 00C878  dbra  d7,$c822 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00C87C:
    RTS();                                               /* 00C87C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC822U: goto L_00C822;
        case 0xC828U: goto L_00C828;
        case 0xC838U: goto L_00C838;
        case 0xC840U: goto L_00C840;
        case 0xC844U: goto L_00C844;
        case 0xC84AU: goto L_00C84A;
        case 0xC850U: goto L_00C850;
        case 0xC856U: goto L_00C856;
        case 0xC86AU: goto L_00C86A;
        case 0xC874U: goto L_00C874;
        case 0xC87CU: goto L_00C87C;
        }
    }
}

/* 00C87E  game routine */
void sub_00C87E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a2 = 0xC74E;                                         /* 00C87E  lea  $c74e(pc),a2 */
    d3 = (d3 & 0xFFFF) * 0x12;                           /* 00C882  mulu.w  #$12,d3 */
    a2 = a2 + SX16(d3);                                  /* 00C886  lea  $0(a2,d3.w),a2 */
    CALL(sub_00C8C8, 0xC88CU);                           /* 00C88A  bsr  $c8c8 */
L_00C88C:
    wr32(a6 + 0x8, a0);                                  /* 00C88C  move.l  a0,$8(a6) */
    d7 = 8;                                              /* 00C890  moveq  #8,d7 */
    a2 = 0xBF42;                                         /* 00C892  lea  $bf42(pc),a2 */
    do {
        BUDGET_CHECK(0xC896U);
L_00C896:
        /* 00C896  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
        wr8(a0, rd8(a0) | (d0 & 0xFF));                      /* 00C898  or.b  d0,(a0) */
        SETB(d0, ~(d0 & 0xFF));                              /* 00C89A  not.b  d0 */
        wr8(a0 + 0x2, rd8(a0 + 0x2) & (d0 & 0xFF));          /* 00C89C  and.b  d0,$2(a0) */
        wr8(a0 + 0x4, rd8(a0 + 0x4) & (d0 & 0xFF));          /* 00C8A0  and.b  d0,$4(a0) */
        wr8(a0 + 0x6, rd8(a0 + 0x6) & (d0 & 0xFF));          /* 00C8A4  and.b  d0,$6(a0) */
        /* 00C8A8  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
        wr8(a1, rd8(a1) | (d0 & 0xFF));                      /* 00C8AA  or.b  d0,(a1) */
        SETB(d0, ~(d0 & 0xFF));                              /* 00C8AC  not.b  d0 */
        wr8(a1 + 0x2, rd8(a1 + 0x2) & (d0 & 0xFF));          /* 00C8AE  and.b  d0,$2(a1) */
        wr8(a1 + 0x4, rd8(a1 + 0x4) & (d0 & 0xFF));          /* 00C8B2  and.b  d0,$4(a1) */
        wr8(a1 + 0x6, rd8(a1 + 0x6) & (d0 & 0xFF));          /* 00C8B6  and.b  d0,$6(a1) */
        a0 = a0 + 0xA0;                                      /* 00C8BA  lea  $a0(a0),a0 */
        a1 = a1 + 0xA0;                                      /* 00C8BE  lea  $a0(a1),a1 */
        /* 00C8C2  dbra  d7,$c896 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00C8C6:
    RTS();                                               /* 00C8C6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC88CU: goto L_00C88C;
        case 0xC896U: goto L_00C896;
        case 0xC8C6U: goto L_00C8C6;
        }
    }
}

/* 00C8C8  game routine */
void sub_00C8C8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = rd32(V_DRAW_BUFFER);                            /* 00C8C8  movea.l  $28e(a5),a0 */
    SETW(d0, (d2 & 0xFFFF));                             /* 00C8CC  move.w  d2,d0 */
    d0 = (d0 & 0xFFFF) * 0xA0;                           /* 00C8CE  mulu.w  #$a0,d0 */
    a0 += d0;                                            /* 00C8D2  adda.l  d0,a0 */
    a1 = a0;                                             /* 00C8D4  movea.l  a0,a1 */
    a1 += 1;                                             /* 00C8D6  addq.l  #1,a1 */
    SETW(d0, (d1 & 0xFFFF));                             /* 00C8D8  move.w  d1,d0 */
    SETW(d0, d0 - 4);                                    /* 00C8DA  subq.w  #4,d0 */
    d1 = d0;                                             /* 00C8DC  move.l  d0,d1 */
    d1 &= 7;                                             /* 00C8DE  andi.l  #$7,d1 */
    SETW(d0, (d0 & 0xFFFF) >> 3);                        /* 00C8E4  lsr.w  #3,d0 */
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 00C8E6  lsr.w  #1,d0 */
    /* 00C8E8  bcc  $c8ee */
    if (COND(5)) {
L_00C8EA:
        a0 += 1;                                             /* 00C8EA  addq.l  #1,a0 */
        a1 += 7;                                             /* 00C8EC  addq.l  #7,a1 */
    }
L_00C8EE:
    SETW(d0, (d0 & 0xFFFF) << 3);                        /* 00C8EE  lsl.w  #3,d0 */
    a0 += d0;                                            /* 00C8F0  adda.l  d0,a0 */
    a1 += d0;                                            /* 00C8F2  adda.l  d0,a1 */
    a3 = 0xBF42;                                         /* 00C8F4  lea  $bf42(pc),a3 */
    d7 = 8;                                              /* 00C8F8  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xC8FAU);
L_00C8FA:
        /* 00C8FA  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), (d1 & 63), 16)); /* 00C8FC  lsr.w  d1,d0 */
        /* 00C8FE  move.w  d0,(a3)+ */
        {
            uint32_t v = (d0 & 0xFFFF);
            wr16(a3, v);
            a3 += 2;
        }
        /* 00C900  dbra  d7,$c8fa */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00C904:
    RTS();                                               /* 00C904  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC8EAU: goto L_00C8EA;
        case 0xC8EEU: goto L_00C8EE;
        case 0xC8FAU: goto L_00C8FA;
        case 0xC904U: goto L_00C904;
        }
    }
}

/* 00C906  game routine */
void sub_00C906(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1CF8;                                         /* 00C906  lea  $1cf8(a5),a6 */
    SETW(d7, 0xB);                                       /* 00C90A  move.w  #$b,d7 */
    do {
        BUDGET_CHECK(0xC90EU);
L_00C90E:
        /* 00C90E  tst.l  $8(a6) */
        /* 00C912  beq  $c942 */
        if (rd32(a6 + 0x8) != 0) {
L_00C914:
            d0 = rd32(a6 + 0x8);                                 /* 00C914  move.l  $8(a6),d0 */
            d0 &= ~0x1;                                          /* 00C918  bclr  #$0,d0 */
            a0 = d0;                                             /* 00C91C  movea.l  d0,a0 */
            d0 &= 0x7FFF;                                        /* 00C91E  andi.l  #$7fff,d0 */
            a1 = 0x18000;                                        /* 00C924  lea  $18000.l,a1 */
            a1 += d0;                                            /* 00C92A  adda.l  d0,a1 */
            d5 = 8;                                              /* 00C92C  moveq  #8,d5 */
            do {
                BUDGET_CHECK(0xC92EU);
L_00C92E:
                /* 00C92E  move.l  (a1)+,(a0)+ */
                {
                    uint32_t v = rd32(a1);
                    a1 += 4;
                    wr32(a0, v);
                    a0 += 4;
                }
                /* 00C930  move.l  (a1)+,(a0)+ */
                {
                    uint32_t v = rd32(a1);
                    a1 += 4;
                    wr32(a0, v);
                    a0 += 4;
                }
                /* 00C932  move.l  (a1)+,(a0)+ */
                {
                    uint32_t v = rd32(a1);
                    a1 += 4;
                    wr32(a0, v);
                    a0 += 4;
                }
                wr32(a0, rd32(a1));                                  /* 00C934  move.l  (a1),(a0) */
                a0 += SX16(0x94);                                    /* 00C936  adda.w  #$94,a0 */
                a1 += SX16(0x94);                                    /* 00C93A  adda.w  #$94,a1 */
                /* 00C93E  dbra  d5,$c92e */
                SETW(d5, d5 - 1);
            } while ((d5 & 0xFFFF) != 0xFFFF);
        }
L_00C942:
        a6 = a6 + 0x10;                                      /* 00C942  lea  $10(a6),a6 */
        /* 00C946  dbra  d7,$c90e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00C94A:
    RTS();                                               /* 00C94A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC90EU: goto L_00C90E;
        case 0xC914U: goto L_00C914;
        case 0xC92EU: goto L_00C92E;
        case 0xC942U: goto L_00C942;
        case 0xC94AU: goto L_00C94A;
        }
    }
}

/* 00C94C  game routine */
void sub_00C94C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0xC012;                                         /* 00C94C  lea  $c012(pc),a0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 00C950  lsl.w  #1,d0 */
    SETW(d0, rd16(a0 + SX16(d0)));                       /* 00C952  move.w  $0(a0,d0.w),d0 */
    a0 = 0xC02A;                                         /* 00C956  lea  $c02a(pc),a0 */
    a0 = a0 + SX16(d0);                                  /* 00C95A  lea  $0(a0,d0.w),a0 */
    wr16(V_026C, 0xB);                                   /* 00C95E  move.w  #$b,$26c(a5) */
    wr16(V_0266, 0);                                     /* 00C964  move.w  #$0,$266(a5) */
    wr16(V_0268, 0);                                     /* 00C96A  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 00C970  move.w  #$0,$26a(a5) */
    CALL(sub_008584, 0xC97AU);                           /* 00C976  bsr  $8584 */
L_00C97A:
    RTS();                                               /* 00C97A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC97AU: goto L_00C97A;
        }
    }
}

/* 00C97C  game routine */
void sub_00C97C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x20280;                                        /* 00C97C  lea  $20280.l,a0 */
    d0 = 0;                                              /* 00C982  moveq  #0,d0 */
    SETW(d6, 0x28);                                      /* 00C984  move.w  #$28,d6 */
    do {
        BUDGET_CHECK(0xC988U);
L_00C988:
        /* 00C988  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C98A  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C98C  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C98E  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C990  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C992  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C994  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00C996  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        a0 = a0 + 0x80;                                      /* 00C998  lea  $80(a0),a0 */
        /* 00C99C  dbra  d6,$c988 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00C9A0:
    RTS();                                               /* 00C9A0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC988U: goto L_00C988;
        case 0xC9A0U: goto L_00C9A0;
        }
    }
}

/* 00C9A2  game routine */
void sub_00C9A2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x18500;                                        /* 00C9A2  lea  $18500.l,a0 */
    a1 = 0x20280;                                        /* 00C9A8  lea  $20280.l,a1 */
    SETW(d6, 0x24);                                      /* 00C9AE  move.w  #$24,d6 */
    do {
        BUDGET_CHECK(0xC9B2U);
L_00C9B2:
        /* 00C9B2  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9B4  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9B6  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9B8  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9BA  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9BC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9BE  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00C9C0  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        a0 = a0 + 0x80;                                      /* 00C9C2  lea  $80(a0),a0 */
        a1 = a1 + 0x80;                                      /* 00C9C6  lea  $80(a1),a1 */
        /* 00C9CA  dbra  d6,$c9b2 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00C9CE:
    RTS();                                               /* 00C9CE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xC9B2U: goto L_00C9B2;
        case 0xC9CEU: goto L_00C9CE;
        }
    }
}

/* 00CB68  game routine */
void sub_00CB68(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
L_00CB68:
    CALL(sub_00CC6A, 0xCB6CU);                           /* 00CB68  bsr  $cc6a */
L_00CB6C:
    CALL(sub_00CD3C, 0xCB70U);                           /* 00CB6C  bsr  $cd3c */
L_00CB70:
    CALL(wait_vbl, 0xCB74U);                             /* 00CB70  bsr  $79dc */
L_00CB74:
    CALL(sub_00CD0C, 0xCB78U);                           /* 00CB74  bsr  $cd0c */
L_00CB78:
    CALL(sub_00CBD8, 0xCB7CU);                           /* 00CB78  bsr  $cbd8 */
L_00CB7C:
    wr16(V_01F4, (d1 & 0xFFFF));                         /* 00CB7C  move.w  d1,$1f4(a5) */
    /* 00CB80  tst.w  d1 */
    /* 00CB82  bmi  $cb68 */
    if ((int16_t)(d1 & 0xFFFF) < 0) {
        BUDGET_CHECK(0xCB68U);
        goto L_00CB68;
    }
L_00CB84:
    a0 = a4 + 0x66;                                      /* 00CB84  lea  $66(a4),a0 */
    d0 = 0x11;                                           /* 00CB88  moveq  #17,d0 */
    do {
        BUDGET_CHECK(0xCB8AU);
L_00CB8A:
        /* 00CB8A  tst.b  (a0)+ */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 00CB8C  bne  $cb98 */
        if (COND(6)) {
            goto L_00CB98;
        }
L_00CB8E:
        /* 00CB8E  dbra  d0,$cb8a */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_00CB92:
    /* 00CB92  cmp.w  #$11,d1 */
    /* 00CB96  bhi  $cb68 */
    if ((d1 & 0xFFFF) > 0x11) {
        BUDGET_CHECK(0xCB68U);
        goto L_00CB68;
    }
L_00CB98:
    /* 00CB98  cmp.w  #$19,d1 */
    /* 00CB9C  bne  $cba4 */
    if ((d1 & 0xFFFF) == 0x19) {
        CALL(sub_00D078, 0xCBA2U);                           /* 00CB9E  bsr  $d078 */
L_00CBA2:
        /* 00CBA2  bra  $cb68 */
        BUDGET_CHECK(0xCB68U);
        goto L_00CB68;
    }
L_00CBA4:
    CALL(sub_00CEE4, 0xCBA8U);                           /* 00CBA4  bsr  $cee4 */
L_00CBA8:
    /* 00CBA8  cmp.w  #$1d,d1 */
    /* 00CBAC  bne  $cb68 */
    if ((d1 & 0xFFFF) != 0x1D) {
        BUDGET_CHECK(0xCB68U);
        goto L_00CB68;
    }
L_00CBAE:
    a0 = a4 + 0x66;                                      /* 00CBAE  lea  $66(a4),a0 */
    SETW(d0, 0x11);                                      /* 00CBB2  move.w  #$11,d0 */
    do {
        BUDGET_CHECK(0xCBB6U);
L_00CBB6:
        /* 00CBB6  tst.b  (a0)+ */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 00CBB8  bne  $cbc0 */
        if (COND(6)) {
            goto L_00CBC0;
        }
L_00CBBA:
        /* 00CBBA  dbra  d0,$cbb6 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
    /* 00CBBE  bra  $cb68 */
    BUDGET_CHECK(0xCB68U);
    goto L_00CB68;
L_00CBC0:
    /* 00CBC0  tst.w  $7a(a4) */
    /* 00CBC4  beq  $cbca */
    if (rd16(a4 + 0x7A) != 0) {
        wr8(a4 + 0x78, 0xFF);                                /* 00CBC6  st  $78(a4) */
    }
L_00CBCA:
    /* 00CBCA  tst.b  $75(a4) */
    /* 00CBCE  beq  $cbd6 */
    if (rd8(a4 + 0x75) != 0) {
        wr16(a4 + 0x92, 1);                                  /* 00CBD0  move.w  #$1,$92(a4) */
    }
L_00CBD6:
    RTS();                                               /* 00CBD6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCB6CU: goto L_00CB6C;
        case 0xCB70U: goto L_00CB70;
        case 0xCB74U: goto L_00CB74;
        case 0xCB78U: goto L_00CB78;
        case 0xCB7CU: goto L_00CB7C;
        case 0xCB84U: goto L_00CB84;
        case 0xCB8AU: goto L_00CB8A;
        case 0xCB8EU: goto L_00CB8E;
        case 0xCB92U: goto L_00CB92;
        case 0xCB98U: goto L_00CB98;
        case 0xCBA2U: goto L_00CBA2;
        case 0xCBA4U: goto L_00CBA4;
        case 0xCBA8U: goto L_00CBA8;
        case 0xCBAEU: goto L_00CBAE;
        case 0xCBB6U: goto L_00CBB6;
        case 0xCBBAU: goto L_00CBBA;
        case 0xCBC0U: goto L_00CBC0;
        case 0xCBCAU: goto L_00CBCA;
        case 0xCBD6U: goto L_00CBD6;
        }
    }
}

/* 00CBD8  game routine */
void sub_00CBD8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00CBD8  tst.w  $140(a5) */
    /* 00CBDC  beq  $cc02 */
    if (rd16(V_0140) != 0) {
L_00CBDE:
        d5 = 9;                                              /* 00CBDE  moveq  #9,d5 */
        do {
            BUDGET_CHECK(0xCBE0U);
L_00CBE0:
            CALL(wait_vbl, 0xCBE4U);                             /* 00CBE0  bsr  $79dc */
L_00CBE4:
            /* 00CBE4  tst.w  $27a(a5) */
            /* 00CBE8  bne  $cbf2 */
            if (rd16(V_027A) == 0) {
L_00CBEA:
                /* 00CBEA  move.b  $2ec(a5),d1 */
                /* 00CBEE  bne  $cbfe */
                SETB(d1, rd8(V_02EC));
                if ((d1 & 0xFF) != 0) {
                    goto L_00CBFE;
                }
L_00CBF0:
                /* 00CBF0  bra  $cbf8 */
            } else {
                /* 00CBF2  move.b  $388(a5),d1 */
                /* 00CBF6  bne  $cbfe */
                SETB(d1, rd8(V_0388));
                if ((d1 & 0xFF) != 0) {
                    goto L_00CBFE;
                }
            }
L_00CBF8:
            wr16(V_0140, 0);                                     /* 00CBF8  clr.w  $140(a5) */
            /* 00CBFC  bra  $cc02 */
            goto L_00CC02;
L_00CBFE:
            /* 00CBFE  dbra  d5,$cbe0 */
            SETW(d5, d5 - 1);
        } while ((d5 & 0xFFFF) != 0xFFFF);
    }
L_00CC02:
    wr16(V_0140, 0);                                     /* 00CC02  clr.w  $140(a5) */
    /* 00CC06  tst.w  $27a(a5) */
    /* 00CC0A  bne  $cc14 */
    if (rd16(V_027A) == 0) {
L_00CC0C:
        /* 00CC0C  move.b  $2ec(a5),d1 */
        /* 00CC10  beq  $cc1e */
        SETB(d1, rd8(V_02EC));
        if ((d1 & 0xFF) == 0) {
            goto L_00CC1E;
        }
L_00CC12:
        /* 00CC12  bra  $cc1a */
    } else {
        /* 00CC14  move.b  $388(a5),d1 */
        /* 00CC18  beq  $cc1e */
        SETB(d1, rd8(V_0388));
        if ((d1 & 0xFF) == 0) {
            goto L_00CC1E;
        }
    }
L_00CC1A:
    wr8(V_0140, 0xFF);                                   /* 00CC1A  st  $140(a5) */
L_00CC1E:
    CALL(sub_00CC54, 0xCC22U);                           /* 00CC1E  bsr  $cc54 */
L_00CC22:
    wr16(V_01F2, (d0 & 0xFFFF));                         /* 00CC22  move.w  d0,$1f2(a5) */
    /* 00CC26  cmp.w  $1f0(a5),d0 */
    /* 00CC2A  beq  $cc38 */
    if ((d0 & 0xFFFF) != rd16(V_01F0)) {
L_00CC2C:
        wr16(V_01F0, (d0 & 0xFFFF));                         /* 00CC2C  move.w  d0,$1f0(a5) */
        /* 00CC30  move.w  d0,-(a7) */
        {
            uint32_t v = (d0 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
        }
        CALL(sub_00CDE0, 0xCC36U);                           /* 00CC32  bsr  $cde0 */
L_00CC36:
        /* 00CC36  move.w  (a7)+,d0 */
        SETW(d0, rd16(a7));
        a7 += 2;
    }
L_00CC38:
    SETW(d1, 0);                                         /* 00CC38  clr.w  d1 */
    d1 = 0xFFFFFFFF;                                     /* 00CC3A  moveq  #-1,d1 */
    /* 00CC3C  tst.w  $27a(a5) */
    /* 00CC40  bne  $cc4a */
    if (rd16(V_027A) == 0) {
L_00CC42:
        /* 00CC42  move.b  $2ec(a5),d1 */
        /* 00CC46  beq  $cc52 */
        SETB(d1, rd8(V_02EC));
        if ((d1 & 0xFF) == 0) {
            goto L_00CC52;
        }
L_00CC48:
        /* 00CC48  bra  $cc50 */
    } else {
        /* 00CC4A  move.b  $388(a5),d1 */
        /* 00CC4E  beq  $cc52 */
        SETB(d1, rd8(V_0388));
        if ((d1 & 0xFF) == 0) {
            goto L_00CC52;
        }
    }
L_00CC50:
    SETW(d1, (d0 & 0xFFFF));                             /* 00CC50  move.w  d0,d1 */
L_00CC52:
    RTS();                                               /* 00CC52  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCBDEU: goto L_00CBDE;
        case 0xCBE0U: goto L_00CBE0;
        case 0xCBE4U: goto L_00CBE4;
        case 0xCBEAU: goto L_00CBEA;
        case 0xCBF0U: goto L_00CBF0;
        case 0xCBF8U: goto L_00CBF8;
        case 0xCBFEU: goto L_00CBFE;
        case 0xCC02U: goto L_00CC02;
        case 0xCC0CU: goto L_00CC0C;
        case 0xCC12U: goto L_00CC12;
        case 0xCC1AU: goto L_00CC1A;
        case 0xCC1EU: goto L_00CC1E;
        case 0xCC22U: goto L_00CC22;
        case 0xCC2CU: goto L_00CC2C;
        case 0xCC36U: goto L_00CC36;
        case 0xCC38U: goto L_00CC38;
        case 0xCC42U: goto L_00CC42;
        case 0xCC48U: goto L_00CC48;
        case 0xCC50U: goto L_00CC50;
        case 0xCC52U: goto L_00CC52;
        }
    }
}

/* 00CC54  game routine */
void sub_00CC54(void)
{
    SETW(d0, rd16(V_01EE));                              /* 00CC54  move.w  $1ee(a5),d0 */
    /* 00CC58  divu.w  #$18,d0 */
    {
        { uint32_t s, ea1; (void)ea1;
        s = 0x18U;
        if (s == 0) SOFT(0xCC58U);
        uint32_t dd = d0, q = dd / s, rem = dd % s;
        if (q > 0xFFFFU) { fv = 1; fc = 0; fz = 0; } else { d0 = (rem << 16) | q; fn = !!(q & 0x8000U); fz = (q == 0); fv = 0; fc = 0; } }
    }
    d0 = (d0 & 0xFFFF) * 5;                              /* 00CC5C  mulu.w  #$5,d0 */
    SETW(d0, (d0 & 0xFFFF) << 6);                        /* 00CC60  lsl.w  #6,d0 */
    SETW(d0, d0 + rd16(V_01EC));                         /* 00CC62  add.w  $1ec(a5),d0 */
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 6, 16));      /* 00CC66  lsr.w  #6,d0 */
    RTS();                                               /* 00CC68  rts */
}

/* 00CC6A  game routine */
void sub_00CC6A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00CC6A  tst.w  $27a(a5) */
    /* 00CC6E  bne  $cc78 */
    if (rd16(V_027A) == 0) {
L_00CC70:
        /* 00CC70  move.b  $2e9(a5),d0 */
        /* 00CC74  beq  $cc84 */
        SETB(d0, rd8(V_02E9));
        if ((d0 & 0xFF) == 0) {
            goto L_00CC84;
        }
L_00CC76:
        /* 00CC76  bra  $cc7e */
    } else {
        /* 00CC78  move.b  $385(a5),d0 */
        /* 00CC7C  beq  $cc84 */
        SETB(d0, rd8(V_0385));
        if ((d0 & 0xFF) == 0) {
            goto L_00CC84;
        }
    }
L_00CC7E:
    /* 00CC7E  subi.w  #$4,$1ec(a5) */
    {
        uint32_t s = 4;
        uint32_t d = rd16(V_01EC);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_01EC, r);
        fx = (s > d);
    }
L_00CC84:
    /* 00CC84  tst.w  $27a(a5) */
    /* 00CC88  bne  $cc92 */
    if (rd16(V_027A) == 0) {
L_00CC8A:
        /* 00CC8A  move.b  $2eb(a5),d0 */
        /* 00CC8E  beq  $cc9e */
        SETB(d0, rd8(V_02EB));
        if ((d0 & 0xFF) == 0) {
            goto L_00CC9E;
        }
L_00CC90:
        /* 00CC90  bra  $cc98 */
    } else {
        /* 00CC92  move.b  $387(a5),d0 */
        /* 00CC96  beq  $cc9e */
        SETB(d0, rd8(V_0387));
        if ((d0 & 0xFF) == 0) {
            goto L_00CC9E;
        }
    }
L_00CC98:
    /* 00CC98  addi.w  #$4,$1ee(a5) */
    {
        uint32_t s = 4;
        uint32_t d = rd16(V_01EE);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_01EE, r);
        fx = (r < s);
    }
L_00CC9E:
    /* 00CC9E  tst.w  $27a(a5) */
    /* 00CCA2  bne  $ccac */
    if (rd16(V_027A) == 0) {
L_00CCA4:
        /* 00CCA4  move.b  $2e8(a5),d0 */
        /* 00CCA8  beq  $ccb8 */
        SETB(d0, rd8(V_02E8));
        if ((d0 & 0xFF) == 0) {
            goto L_00CCB8;
        }
L_00CCAA:
        /* 00CCAA  bra  $ccb2 */
    } else {
        /* 00CCAC  move.b  $384(a5),d0 */
        /* 00CCB0  beq  $ccb8 */
        SETB(d0, rd8(V_0384));
        if ((d0 & 0xFF) == 0) {
            goto L_00CCB8;
        }
    }
L_00CCB2:
    /* 00CCB2  addi.w  #$4,$1ec(a5) */
    {
        uint32_t s = 4;
        uint32_t d = rd16(V_01EC);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_01EC, r);
        fx = (r < s);
    }
L_00CCB8:
    /* 00CCB8  tst.w  $27a(a5) */
    /* 00CCBC  bne  $ccc6 */
    if (rd16(V_027A) == 0) {
L_00CCBE:
        /* 00CCBE  move.b  $2ea(a5),d0 */
        /* 00CCC2  beq  $ccd2 */
        SETB(d0, rd8(V_02EA));
        if ((d0 & 0xFF) == 0) {
            goto L_00CCD2;
        }
L_00CCC4:
        /* 00CCC4  bra  $cccc */
    } else {
        /* 00CCC6  move.b  $386(a5),d0 */
        /* 00CCCA  beq  $ccd2 */
        SETB(d0, rd8(V_0386));
        if ((d0 & 0xFF) == 0) {
            goto L_00CCD2;
        }
    }
L_00CCCC:
    /* 00CCCC  subi.w  #$4,$1ee(a5) */
    {
        uint32_t s = 4;
        uint32_t d = rd16(V_01EE);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_01EE, r);
        fx = (s > d);
    }
L_00CCD2:
    SETW(d0, rd16(V_01EC));                              /* 00CCD2  move.w  $1ec(a5),d0 */
    /* 00CCD6  cmp.w  #$6,d0 */
    /* 00CCDA  bhi  $cce0 */
    if ((d0 & 0xFFFF) <= 6) {
L_00CCDC:
        SETW(d0, 7);                                         /* 00CCDC  move.w  #$7,d0 */
    }
L_00CCE0:
    /* 00CCE0  cmp.w  #$138,d0 */
    /* 00CCE4  bls  $ccea */
    if ((d0 & 0xFFFF) > 0x138) {
        SETW(d0, 0x138);                                     /* 00CCE6  move.w  #$138,d0 */
    }
L_00CCEA:
    wr16(V_01EC, (d0 & 0xFFFF));                         /* 00CCEA  move.w  d0,$1ec(a5) */
    SETW(d0, rd16(V_01EE));                              /* 00CCEE  move.w  $1ee(a5),d0 */
    /* 00CCF2  cmp.w  #$7,d0 */
    /* 00CCF6  bhi  $ccfc */
    if ((d0 & 0xFFFF) <= 7) {
L_00CCF8:
        SETW(d0, 8);                                         /* 00CCF8  move.w  #$8,d0 */
    }
L_00CCFC:
    /* 00CCFC  cmp.w  #$84,d0 */
    /* 00CD00  bls  $cd06 */
    if ((d0 & 0xFFFF) > 0x84) {
        SETW(d0, 0x84);                                      /* 00CD02  move.w  #$84,d0 */
    }
L_00CD06:
    wr16(V_01EE, (d0 & 0xFFFF));                         /* 00CD06  move.w  d0,$1ee(a5) */
    RTS();                                               /* 00CD0A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCC70U: goto L_00CC70;
        case 0xCC76U: goto L_00CC76;
        case 0xCC7EU: goto L_00CC7E;
        case 0xCC84U: goto L_00CC84;
        case 0xCC8AU: goto L_00CC8A;
        case 0xCC90U: goto L_00CC90;
        case 0xCC98U: goto L_00CC98;
        case 0xCC9EU: goto L_00CC9E;
        case 0xCCA4U: goto L_00CCA4;
        case 0xCCAAU: goto L_00CCAA;
        case 0xCCB2U: goto L_00CCB2;
        case 0xCCB8U: goto L_00CCB8;
        case 0xCCBEU: goto L_00CCBE;
        case 0xCCC4U: goto L_00CCC4;
        case 0xCCCCU: goto L_00CCCC;
        case 0xCCD2U: goto L_00CCD2;
        case 0xCCDCU: goto L_00CCDC;
        case 0xCCE0U: goto L_00CCE0;
        case 0xCCEAU: goto L_00CCEA;
        case 0xCCF8U: goto L_00CCF8;
        case 0xCCFCU: goto L_00CCFC;
        case 0xCD06U: goto L_00CD06;
        }
    }
}

/* 00CD0C  game routine */
void sub_00CD0C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = rd32(V_01F6);                                   /* 00CD0C  move.l  $1f6(a5),d0 */
    d0 &= ~0x1;                                          /* 00CD10  bclr  #$0,d0 */
    a0 = d0;                                             /* 00CD14  movea.l  d0,a0 */
    d0 &= 0x7FFF;                                        /* 00CD16  andi.l  #$7fff,d0 */
    a1 = 0x18000;                                        /* 00CD1C  lea  $18000.l,a1 */
    a1 += d0;                                            /* 00CD22  adda.l  d0,a1 */
    d7 = 8;                                              /* 00CD24  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xCD26U);
L_00CD26:
        /* 00CD26  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00CD28  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00CD2A  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        wr32(a0, rd32(a1));                                  /* 00CD2C  move.l  (a1),(a0) */
        a0 += SX16(0x94);                                    /* 00CD2E  adda.w  #$94,a0 */
        a1 += SX16(0x94);                                    /* 00CD32  adda.w  #$94,a1 */
        /* 00CD36  dbra  d7,$cd26 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00CD3A:
    RTS();                                               /* 00CD3A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCD26U: goto L_00CD26;
        case 0xCD3AU: goto L_00CD3A;
        }
    }
}

/* 00CD3C  game routine */
void sub_00CD3C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00CD72, 0xCD3EU);                           /* 00CD3C  bsr  $cd72 */
L_00CD3E:
    wr32(V_01F6, a0);                                    /* 00CD3E  move.l  a0,$1f6(a5) */
    d7 = 8;                                              /* 00CD42  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xCD44U);
L_00CD44:
        /* 00CD44  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
        wr8(a0, rd8(a0) | (d0 & 0xFF));                      /* 00CD46  or.b  d0,(a0) */
        wr8(a0 + 0x2, rd8(a0 + 0x2) | (d0 & 0xFF));          /* 00CD48  or.b  d0,$2(a0) */
        wr8(a0 + 0x4, rd8(a0 + 0x4) | (d0 & 0xFF));          /* 00CD4C  or.b  d0,$4(a0) */
        wr8(a0 + 0x6, rd8(a0 + 0x6) | (d0 & 0xFF));          /* 00CD50  or.b  d0,$6(a0) */
        /* 00CD54  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
        wr8(a1, rd8(a1) | (d0 & 0xFF));                      /* 00CD56  or.b  d0,(a1) */
        wr8(a1 + 0x2, rd8(a1 + 0x2) | (d0 & 0xFF));          /* 00CD58  or.b  d0,$2(a1) */
        wr8(a1 + 0x4, rd8(a1 + 0x4) | (d0 & 0xFF));          /* 00CD5C  or.b  d0,$4(a1) */
        wr8(a1 + 0x6, rd8(a1 + 0x6) | (d0 & 0xFF));          /* 00CD60  or.b  d0,$6(a1) */
        a0 = a0 + 0xA0;                                      /* 00CD64  lea  $a0(a0),a0 */
        a1 = a1 + 0xA0;                                      /* 00CD68  lea  $a0(a1),a1 */
        /* 00CD6C  dbra  d7,$cd44 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00CD70:
    RTS();                                               /* 00CD70  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCD3EU: goto L_00CD3E;
        case 0xCD44U: goto L_00CD44;
        case 0xCD70U: goto L_00CD70;
        }
    }
}

/* 00CD72  game routine */
void sub_00CD72(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = rd32(V_DRAW_BUFFER);                            /* 00CD72  movea.l  $28e(a5),a0 */
    SETW(d0, rd16(V_01EE));                              /* 00CD76  move.w  $1ee(a5),d0 */
    SETW(d0, d0 - 4);                                    /* 00CD7A  subq.w  #4,d0 */
    d0 = (d0 & 0xFFFF) * 0xA0;                           /* 00CD7C  mulu.w  #$a0,d0 */
    a0 += d0;                                            /* 00CD80  adda.l  d0,a0 */
    a1 = a0;                                             /* 00CD82  movea.l  a0,a1 */
    a1 += 1;                                             /* 00CD84  addq.l  #1,a1 */
    SETW(d0, rd16(V_01EC));                              /* 00CD86  move.w  $1ec.l,d0 */
    SETW(d0, d0 - 4);                                    /* 00CD8C  subq.w  #4,d0 */
    d1 = d0;                                             /* 00CD8E  move.l  d0,d1 */
    d1 &= 7;                                             /* 00CD90  andi.l  #$7,d1 */
    SETW(d0, (d0 & 0xFFFF) >> 3);                        /* 00CD96  lsr.w  #3,d0 */
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 00CD98  lsr.w  #1,d0 */
    /* 00CD9A  bcc  $cda0 */
    if (COND(5)) {
L_00CD9C:
        a0 += 1;                                             /* 00CD9C  addq.l  #1,a0 */
        a1 += 7;                                             /* 00CD9E  addq.l  #7,a1 */
    }
L_00CDA0:
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 00CDA0  lsl.w  #3,d0 */
    a0 += d0;                                            /* 00CDA2  adda.l  d0,a0 */
    a1 += d0;                                            /* 00CDA4  adda.l  d0,a1 */
    a2 = 0xCDBC;                                         /* 00CDA6  lea  $cdbc(pc),a2 */
    a3 = 0xCDCE;                                         /* 00CDAA  lea  $cdce(pc),a3 */
    d7 = 8;                                              /* 00CDAE  moveq  #8,d7 */
    do {
        BUDGET_CHECK(0xCDB0U);
L_00CDB0:
        /* 00CDB0  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), (d1 & 63), 16)); /* 00CDB2  lsr.w  d1,d0 */
        /* 00CDB4  move.w  d0,(a3)+ */
        {
            uint32_t v = (d0 & 0xFFFF);
            wr16(a3, v);
            a3 += 2;
        }
        /* 00CDB6  dbra  d7,$cdb0 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00CDBA:
    RTS();                                               /* 00CDBA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCD9CU: goto L_00CD9C;
        case 0xCDA0U: goto L_00CDA0;
        case 0xCDB0U: goto L_00CDB0;
        case 0xCDBAU: goto L_00CDBA;
        }
    }
}

/* 00CDBC  game routine */
void sub_00CDBC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_00CDBC;
L_009740:
    SETB(d3, d3 | 0x27);                                 /* 009740  ori.b  #$27,d3 */
    wr16(a2 + 0x1 + SX16(d0), rd16(a2 + 0x1 + SX16(d0)) | 0xFFFD); /* 009744  ori.w  #$fffd,$1(a2,d0.w) */
    /* 00974A  ori.b  #$6f,-(a2) */
    {
        uint32_t s = 0x6F;
        a2 -= 1;
        uint32_t d = rd8(a2);
        uint32_t r = (d | s) & 0xFF;
        wr8(a2, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x974EU);                                       /* 00974E  soft */
L_00CDBC:
    /* 00CDBC  move.w  d0,d7 */
    {
        uint32_t v = (d0 & 0xFFFF);
        SETW(d7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 00CDBE  bmi  $9740 */
    if (COND(11)) { BUDGET_CHECK(0x9740U); goto L_009740; }
    MISS(0xCDC2U);

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9740U: goto L_009740;
        }
    }
}

/* 00CDCE  game routine */
void sub_00CDCE(void)
{
    /* 00CDCE  bset  d1,-(a0) */
    a0 -= 1;
    fz = !(rd8(a0) & (1u << (d1 & 7)));
    wr8(a0, rd8(a0) | (1u << (d1 & 7)));
    MISS(0xCDD0U);
}

/* 00CDE0  game routine */
void sub_00CDE0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(0xFF825C, 0);                                   /* 00CDE0  clr.w  $ff825c.l */
    a0 = 0x1DA00;                                        /* 00CDE6  lea  $1da00.l,a0 */
    a1 = 0x25A00;                                        /* 00CDEC  lea  $25a00.l,a1 */
    SETW(d7, 0x25);                                      /* 00CDF2  move.w  #$25,d7 */
    do {
        BUDGET_CHECK(0xCDF6U);
L_00CDF6:
        SETW(d1, 0x27);                                      /* 00CDF6  move.w  #$27,d1 */
        do {
            BUDGET_CHECK(0xCDFAU);
L_00CDFA:
            /* 00CDFA  move.l  (a0)+,(a1)+ */
            {
                uint32_t v = rd32(a0);
                a0 += 4;
                wr32(a1, v);
                a1 += 4;
            }
            /* 00CDFC  dbra  d1,$cdfa */
            SETW(d1, d1 - 1);
        } while ((d1 & 0xFFFF) != 0xFFFF);
L_00CE00:
        /* 00CE00  dbra  d7,$cdf6 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00CE04:
    /* 00CE04  move.w  d0,-(a7) */
    {
        uint32_t v = (d0 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    a0 = 0xD3AC;                                         /* 00CE06  lea  $d3ac(pc),a0 */
    a1 = 0xCEA8;                                         /* 00CE0A  lea  $cea8(pc),a1 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 00CE0E  lsl.w  #1,d0 */
    SETW(d1, rd16(a1 + SX16(d0)));                       /* 00CE10  move.w  $0(a1,d0.w),d1 */
    a0 = a0 + SX16(d1);                                  /* 00CE14  lea  $0(a0,d1.w),a0 */
    wr16(V_026C, 0xE);                                   /* 00CE18  move.w  #$e,$26c(a5) */
    wr16(V_0266, 0x5A00);                                /* 00CE1E  move.w  #$5a00,$266(a5) */
    wr16(V_0268, 1);                                     /* 00CE24  move.w  #$1,$268(a5) */
    wr16(V_026A, 1);                                     /* 00CE2A  move.w  #$1,$26a(a5) */
    CALL(sub_008584, 0xCE34U);                           /* 00CE30  bsr  $8584 */
L_00CE34:
    wr16(0xFF825C, 0x47);                                /* 00CE34  move.w  #$47,$ff825c.l */
    /* 00CE3C  move.w  (a7)+,d0 */
    SETW(d0, rd16(a7));
    a7 += 2;
    wr16(V_0266, 0x74E0);                                /* 00CE3E  move.w  #$74e0,$266(a5) */
    wr16(V_0268, 0xB);                                   /* 00CE44  move.w  #$b,$268(a5) */
    wr16(V_026A, 0xB);                                   /* 00CE4A  move.w  #$b,$26a(a5) */
    a0 = 0xCFF8;                                         /* 00CE50  lea  $cff8(pc),a0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 2, 16));      /* 00CE54  lsl.w  #2,d0 */
    /* 00CE56  move.l  $0(a0,d0.w),d0 */
    /* 00CE5A  bmi  $ce6a */
    d0 = rd32(a0 + SX16(d0));
    if ((int32_t)d0 >= 0) {
L_00CE5C:
        CALL(sub_007D3E, 0xCE60U);                           /* 00CE5C  bsr  $7d3e */
L_00CE60:
        a0 = V_025F;                                         /* 00CE60  lea  $25f(a5),a0 */
        CALL(sub_008058, 0xCE68U);                           /* 00CE64  bsr  $8058 */
L_00CE68:
        /* 00CE68  bra  $ce72 */
    } else {
L_00CE6A:
        a0 = 0xD070;                                         /* 00CE6A  lea  $d070(pc),a0 */
        CALL(sub_008058, 0xCE72U);                           /* 00CE6E  bsr  $8058 */
    }
L_00CE72:
    RTS();                                               /* 00CE72  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCDF6U: goto L_00CDF6;
        case 0xCDFAU: goto L_00CDFA;
        case 0xCE00U: goto L_00CE00;
        case 0xCE04U: goto L_00CE04;
        case 0xCE34U: goto L_00CE34;
        case 0xCE5CU: goto L_00CE5C;
        case 0xCE60U: goto L_00CE60;
        case 0xCE68U: goto L_00CE68;
        case 0xCE6AU: goto L_00CE6A;
        case 0xCE72U: goto L_00CE72;
        }
    }
}

/* 00CE74  game routine */
void sub_00CE74(void)
{
    wr16(a5, 0);                                         /* 00CE74  clr.w  (a5) */
    /* 00CE76  subq.b  #4,-(a0) */
    {
        uint32_t s = 4;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0, r);
    }
    a7 -= 4;                                             /* 00CE78  subq.w  #4,a7 */
    /* 00CE7A  subq.w  #2,(a2) */
    {
        uint32_t s = 2;
        uint32_t d = rd16(a2);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(a2, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    /* 00CE7C  movea.l  (a7),a0 */
    a0 = rd32(a7);
    MISS(0xCE7EU);
}

/* 00CE8E  game routine */
void sub_00CE8E(void)
{
    wr16(a5, 0);                                         /* 00CE8E  clr.w  (a5) */
    /* 00CE90  subq.b  #4,-(a0) */
    {
        uint32_t s = 4;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0, r);
    }
    a7 -= 4;                                             /* 00CE92  subq.w  #4,a7 */
    /* 00CE94  subq.w  #2,(a2) */
    {
        uint32_t s = 2;
        uint32_t d = rd16(a2);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(a2, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    /* 00CE96  movea.l  (a7),a0 */
    a0 = rd32(a7);
    MISS(0xCE98U);
}

/* 00CEA8  game routine */
void sub_00CEA8(void)
{
    /* 00CEA8  ori.b  #$9c,d0 */
    {
        uint32_t s = 0x9C;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        fn = !!(r & 0x80);
        fv = 0;
        fc = 0;
    }
    /* 00CEAC  btst  d0,$1d4.w */
    fz = !(rd8(V_01D4) & (1u << (d0 & 7)));
    MISS(0xCEB0U);
}

/* 00CEE4  game routine */
void sub_00CEE4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00CEE4  move.w  d1,-(a7) */
    {
        uint32_t v = (d1 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    /* 00CEE6  cmp.w  #$1d,d1 */
    /* 00CEEA  beq  $cff4 */
    if ((d1 & 0xFFFF) != 0x1D) {
L_00CEEE:
        d6 = 0;                                              /* 00CEEE  moveq  #0,d6 */
        /* 00CEF0  cmp.w  #$12,d1 */
        /* 00CEF4  bcs  $cf7e */
        if ((d1 & 0xFFFF) >= 0x12) {
L_00CEF8:
            /* 00CEF8  move.w  d1,-(a7) */
            {
                uint32_t v = (d1 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
            }
            SETW(d1, d1 - 0x12);                                 /* 00CEFA  subi.w  #$12,d1 */
            a3 = 0xD22A;                                         /* 00CEFE  lea  $d22a(pc),a3 */
            SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 1, 16));      /* 00CF02  lsl.w  #1,d1 */
            /* 00CF04  move.w  $0(a3,d1.w),d5 */
            /* 00CF08  bmi  $cff4 */
            SETW(d5, rd16(a3 + SX16(d1)));
            if ((int16_t)(d5 & 0xFFFF) < 0) {
                goto L_00CFF4;
            }
L_00CF0C:
            a3 = a4 + 0x7A;                                      /* 00CF0C  lea  $7a(a4),a3 */
            SETW(d2, (d1 & 0xFFFF));                             /* 00CF10  move.w  d1,d2 */
            /* 00CF12  move.w  (a7)+,d1 */
            SETW(d1, rd16(a7));
            a7 += 2;
            SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 2, 16));      /* 00CF14  lsl.w  #2,d1 */
            a0 = 0xCFF8;                                         /* 00CF16  lea  $cff8(pc),a0 */
            a0 = a0 + SX16(d1);                                  /* 00CF1A  lea  $0(a0,d1.w),a0 */
            /* 00CF1E  move.l  (a0),d6 */
            /* 00CF20  bmi  $cff4 */
            d6 = rd32(a0);
            if ((int32_t)d6 < 0) {
                goto L_00CFF4;
            }
L_00CF24:
            d0 = rd32(a4 + 0x58);                                /* 00CF24  move.l  $58(a4),d0 */
            /* 00CF28  cmp.l  d0,d6 */
            /* 00CF2A  bls  $cf42 */
            if (d6 > d0) {
                d0 = 0x19;                                           /* 00CF2C  move.l  #$19,d0 */
                d1 = 0;                                              /* 00CF32  move.l  #$0,d1 */
                CALL(sub_041C04, 0xCF3EU);                           /* 00CF38  jsr  $41c04.l */
L_00CF3E:
                /* 00CF3E  bra  $cff4 */
                goto L_00CFF4;
            }
L_00CF42:
            /* 00CF42  cmpi.w  #$26ac,$0(a3,d2.w) */
            /* 00CF48  bcs  $cf62 */
            if (rd16(a3 + SX16(d2)) >= 0x26AC) {
                d0 = 0xD;                                            /* 00CF4C  move.l  #$d,d0 */
                d1 = 0;                                              /* 00CF52  move.l  #$0,d1 */
                CALL(sub_041C04, 0xCF5EU);                           /* 00CF58  jsr  $41c04.l */
L_00CF5E:
                /* 00CF5E  bra  $cff4 */
                goto L_00CFF4;
            }
L_00CF62:
            wr32(a4 + 0x58, rd32(a4 + 0x58) - d6);               /* 00CF62  sub.l  d6,$58(a4) */
            /* 00CF66  add.w  d5,$0(a3,d2.w) */
            {
                uint32_t s = (d5 & 0xFFFF);
                uint32_t d = rd16(a3 + SX16(d2));
                uint32_t r = (d + s) & 0xFFFF;
                wr16(a3 + SX16(d2), r);
                fx = (r < s);
            }
            d0 = 9;                                              /* 00CF6A  move.l  #$9,d0 */
            d1 = 0;                                              /* 00CF70  move.l  #$0,d1 */
            CALL(sub_041C04, 0xCF7CU);                           /* 00CF76  jsr  $41c04.l */
L_00CF7C:
            /* 00CF7C  bra  $cfd2 */
        } else {
L_00CF7E:
            a2 = a4 + 0x66;                                      /* 00CF7E  lea  $66(a4),a2 */
            /* 00CF82  tst.b  $0(a2,d1.w) */
            /* 00CF86  bne  $cff4 */
            if (rd8(a2 + SX16(d1)) != 0) {
                goto L_00CFF4;
            }
L_00CF8A:
            SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 2, 16));      /* 00CF8A  lsl.w  #2,d1 */
            a0 = 0xCFF8;                                         /* 00CF8C  lea  $cff8(pc),a0 */
            a0 = a0 + SX16(d1);                                  /* 00CF90  lea  $0(a0,d1.w),a0 */
            /* 00CF94  move.l  (a0),d6 */
            /* 00CF96  bmi  $cff4 */
            d6 = rd32(a0);
            if ((int32_t)d6 < 0) {
                goto L_00CFF4;
            }
L_00CF98:
            d0 = rd32(a4 + 0x58);                                /* 00CF98  move.l  $58(a4),d0 */
            /* 00CF9C  cmp.l  d0,d6 */
            /* 00CF9E  bls  $cfb6 */
            if (d6 > d0) {
                d0 = 0x19;                                           /* 00CFA0  move.l  #$19,d0 */
                d1 = 0;                                              /* 00CFA6  move.l  #$0,d1 */
                CALL(sub_041C04, 0xCFB2U);                           /* 00CFAC  jsr  $41c04.l */
L_00CFB2:
                /* 00CFB2  bra  $cff4 */
                goto L_00CFF4;
            }
L_00CFB6:
            wr32(a4 + 0x58, rd32(a4 + 0x58) - d6);               /* 00CFB6  sub.l  d6,$58(a4) */
            SETW(d1, shift_op(1, 0, (d1 & 0xFFFF), 2, 16));      /* 00CFBA  lsr.w  #2,d1 */
            wr8(a2 + SX16(d1), 0xFF);                            /* 00CFBC  st  $0(a2,d1.w) */
            d0 = 9;                                              /* 00CFC0  move.l  #$9,d0 */
            d1 = 0;                                              /* 00CFC6  move.l  #$0,d1 */
            CALL(sub_041C04, 0xCFD2U);                           /* 00CFCC  jsr  $41c04.l */
        }
L_00CFD2:
        d0 = rd32(a4 + 0x58);                                /* 00CFD2  move.l  $58(a4),d0 */
        CALL(sub_007D3E, 0xCFDAU);                           /* 00CFD6  bsr  $7d3e */
L_00CFDA:
        a0 = V_025E;                                         /* 00CFDA  lea  $25e(a5),a0 */
        wr16(V_0266, 0x74E0);                                /* 00CFDE  move.w  #$74e0,$266(a5) */
L_00CFE4:
        wr16(V_0268, 0x1C);                                  /* 00CFE4  move.w  #$1c,$268(a5) */
L_00CFEA:
        wr16(V_026A, 0x1C);                                  /* 00CFEA  move.w  #$1c,$26a(a5) */
L_00CFF0:
        CALL(sub_008058, 0xCFF4U);                           /* 00CFF0  bsr  $8058 */
    }
L_00CFF4:
    /* 00CFF4  move.w  (a7)+,d1 */
    SETW(d1, rd16(a7));
    a7 += 2;
L_00CFF6:
    RTS();                                               /* 00CFF6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCEEEU: goto L_00CEEE;
        case 0xCEF8U: goto L_00CEF8;
        case 0xCF0CU: goto L_00CF0C;
        case 0xCF24U: goto L_00CF24;
        case 0xCF3EU: goto L_00CF3E;
        case 0xCF42U: goto L_00CF42;
        case 0xCF5EU: goto L_00CF5E;
        case 0xCF62U: goto L_00CF62;
        case 0xCF7CU: goto L_00CF7C;
        case 0xCF7EU: goto L_00CF7E;
        case 0xCF8AU: goto L_00CF8A;
        case 0xCF98U: goto L_00CF98;
        case 0xCFB2U: goto L_00CFB2;
        case 0xCFB6U: goto L_00CFB6;
        case 0xCFD2U: goto L_00CFD2;
        case 0xCFDAU: goto L_00CFDA;
        case 0xCFE4U: goto L_00CFE4;
        case 0xCFEAU: goto L_00CFEA;
        case 0xCFF0U: goto L_00CFF0;
        case 0xCFF4U: goto L_00CFF4;
        case 0xCFF6U: goto L_00CFF6;
        }
    }
}

/* 00CFF8  game routine */
void sub_00CFF8(void)
{
    SETB(d0, d0 | 0x64);                                 /* 00CFF8  ori.b  #$64,d0 */
    SETB(d0, d0 | 0x6E);                                 /* 00CFFC  ori.b  #$6e,d0 */
    SETB(d0, d0 | 0x78);                                 /* 00D000  ori.b  #$78,d0 */
    SETB(d0, d0 | 0x20);                                 /* 00D004  ori.b  #$20,d0 */
    SETB(d0, d0 | 0xBC);                                 /* 00D008  ori.b  #$bc,d0 */
    SETB(d0, d0 | 0xDC);                                 /* 00D00C  ori.b  #$dc,d0 */
    SETB(d0, d0 | 0x20);                                 /* 00D010  ori.b  #$20,d0 */
    SETB(d0, d0 | 0x84);                                 /* 00D014  ori.b  #$84,d0 */
    SETB(d0, d0 | 0xDC);                                 /* 00D018  ori.b  #$dc,d0 */
    SETB(d0, d0 | 0x24);                                 /* 00D01C  ori.b  #$24,d0 */
    SETB(d0, d0 | 0x44);                                 /* 00D020  ori.b  #$44,d0 */
    SETB(d0, d0 | 0x78);                                 /* 00D024  ori.b  #$78,d0 */
    SETB(d0, d0 | 0x38);                                 /* 00D028  ori.b  #$38,d0 */
    SETB(d0, d0 | 0xA0);                                 /* 00D02C  ori.b  #$a0,d0 */
    SETB(d0, d0 | 0x28);                                 /* 00D030  ori.b  #$28,d0 */
    SETB(d0, d0 | 0x54);                                 /* 00D034  ori.b  #$54,d0 */
    SETB(d0, d0 | 0x34);                                 /* 00D038  ori.b  #$34,d0 */
    SETB(d0, d0 | 0xAC);                                 /* 00D03C  ori.b  #$ac,d0 */
    SETB(d0, d0 | 0xFA);                                 /* 00D040  ori.b  #$fa,d0 */
    SETB(d0, d0 | 0x32);                                 /* 00D044  ori.b  #$32,d0 */
    SETB(d0, d0 | 0x37);                                 /* 00D048  ori.b  #$37,d0 */
    SETB(d0, d0 | 0x41);                                 /* 00D04C  ori.b  #$41,d0 */
    SETB(d0, d0 | 0x3C);                                 /* 00D050  ori.b  #$3c,d0 */
    SETB(d0, d0 | 0x50);                                 /* 00D054  ori.b  #$50,d0 */
    /* 00D058  ori.b  #$78,d0 */
    {
        uint32_t s = 0x78;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0xD05CU);                                       /* 00D05C  soft */
}

