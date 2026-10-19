/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 00D070  game routine */
void sub_00D070(void)
{
    /* 00D070  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00D072  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00D074  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    SETB(d0, d0 | 0xFA);                                 /* 00D076  ori.b  #$fa,d0 */
    /* 00D07A  cmpa.l  a2,a5 */
    { sub_00D07C(); return; }
}

/* 00D078  game routine */
void sub_00D078(void)
{
    /* 00D078  lea  $8c44(pc),a1 */
    a1 = 0x8C44;
    { sub_00D07C(); return; }
}

/* 00D07C  game routine */
void sub_00D07C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_0075E2, 0xD080U);                           /* 00D07C  bsr  $75e2 */
L_00D080:
    CALL(wait_vbl, 0xD084U);                             /* 00D080  bsr  $79dc */
L_00D084:
    a0 = 0x20000;                                        /* 00D084  lea  $20000.l,a0 */
    a1 = 0x18000;                                        /* 00D08A  lea  $18000.l,a1 */
    SETW(d7, 0x1FFF);                                    /* 00D090  move.w  #$1fff,d7 */
    d0 = 0;                                              /* 00D094  moveq  #0,d0 */
    do {
        BUDGET_CHECK(0xD096U);
L_00D096:
        /* 00D096  move.l  (a0),(a1)+ */
        {
            uint32_t v = rd32(a0);
            wr32(a1, v);
            a1 += 4;
        }
        /* 00D098  move.l  d0,(a0)+ */
        {
            uint32_t v = d0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 00D09A  dbra  d7,$d096 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    wr16(V_0266, 0);                                     /* 00D09E  move.w  #$0,$266(a5) */
    wr16(V_0268, 0);                                     /* 00D0A4  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 00D0AA  move.w  #$0,$26a(a5) */
    wr16(V_026C, 0xF);                                   /* 00D0B0  move.w  #$f,$26c(a5) */
    a0 = 0xD2F2;                                         /* 00D0B6  lea  $d2f2(pc),a0 */
    CALL(sub_008584, 0xD0BEU);                           /* 00D0BA  bsr  $8584 */
L_00D0BE:
    CALL(sub_00D336, 0xD0C2U);                           /* 00D0BE  bsr  $d336 */
L_00D0C2:
    wr16(V_026C, 6);                                     /* 00D0C2  move.w  #$6,$26c(a5) */
    wr16(V_0266, 0x960);                                 /* 00D0C8  move.w  #$960,$266(a5) */
    wr16(V_0268, 0);                                     /* 00D0CE  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 00D0D4  move.w  #$0,$26a(a5) */
    a3 = a4 + 0x66;                                      /* 00D0DA  lea  $66(a4),a3 */
    SETW(d6, 0x11);                                      /* 00D0DE  move.w  #$11,d6 */
    d5 = 0xFFFFFFFF;                                     /* 00D0E2  moveq  #-1,d5 */
    do {
        BUDGET_CHECK(0xD0E4U);
L_00D0E4:
        /* 00D0E4  addq.w  #1,d5 */
        {
            uint32_t s = 1;
            uint32_t d = (d5 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d5, r);
            fx = (r < s);
        }
        /* 00D0E6  tst.b  (a3)+ */
        {
            uint32_t v = rd8(a3);
            a3 += 1;
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 00D0E8  beq  $d0ec */
        if (COND(6)) {
            CALL(sub_00D14E, 0xD0ECU);                           /* 00D0EA  bsr  $d14e */
        }
L_00D0EC:
        /* 00D0EC  dbra  d6,$d0e4 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
    CALL(wait_vbl, 0xD0F4U);                             /* 00D0F0  bsr  $79dc */
L_00D0F4:
    a1 = 0xD316;                                         /* 00D0F4  lea  $d316(pc),a1 */
    CALL(sub_00AF96, 0xD0FCU);                           /* 00D0F8  bsr  $af96 */
L_00D0FC:
    wr8(V_02EC, 0);                                      /* 00D0FC  clr.b  $2ec(a5) */
    wr8(V_0388, 0);                                      /* 00D100  clr.b  $388(a5) */
L_00D104:
    /* 00D104  tst.w  $27a(a5) */
    /* 00D108  bne  $d112 */
    if (rd16(V_027A) == 0) {
        /* 00D10A  move.b  $2ec(a5),d1 */
        /* 00D10E  beq  $d104 */
        SETB(d1, rd8(V_02EC));
        if ((d1 & 0xFF) == 0) {
            BUDGET_CHECK(0xD104U);
            goto L_00D104;
        }
        /* 00D110  bra  $d118 */
    } else {
        /* 00D112  move.b  $388(a5),d1 */
        /* 00D116  beq  $d104 */
        SETB(d1, rd8(V_0388));
        if ((d1 & 0xFF) == 0) {
            BUDGET_CHECK(0xD104U);
            goto L_00D104;
        }
    }
    SETW(d1, (d0 & 0xFFFF));                             /* 00D118  move.w  d0,d1 */
L_00D11A:
    /* 00D11A  tst.w  $27a(a5) */
    /* 00D11E  bne  $d128 */
    if (rd16(V_027A) == 0) {
        /* 00D120  move.b  $2ec(a5),d0 */
        /* 00D124  bne  $d11a */
        SETB(d0, rd8(V_02EC));
        if ((d0 & 0xFF) != 0) {
            BUDGET_CHECK(0xD11AU);
            goto L_00D11A;
        }
        /* 00D126  bra  $d12e */
    } else {
        /* 00D128  move.b  $388(a5),d0 */
        /* 00D12C  bne  $d11a */
        SETB(d0, rd8(V_0388));
        if ((d0 & 0xFF) != 0) {
            BUDGET_CHECK(0xD11AU);
            goto L_00D11A;
        }
    }
    a1 = 0x8C44;                                         /* 00D12E  lea  $8c44(pc),a1 */
    CALL(sub_00AF96, 0xD136U);                           /* 00D132  bsr  $af96 */
L_00D136:
    a0 = 0x18000;                                        /* 00D136  lea  $18000.l,a0 */
    a1 = 0x20000;                                        /* 00D13C  lea  $20000.l,a1 */
    SETW(d7, 0x1FFF);                                    /* 00D142  move.w  #$1fff,d7 */
    do {
        BUDGET_CHECK(0xD146U);
L_00D146:
        /* 00D146  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00D148  dbra  d7,$d146 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    /* 00D14C  bra  $d172 */
    a1 = 0x8C24;                                         /* 00D172  lea  $8c24(pc),a1 */
    CALL(sub_00AF96, 0xD17AU);                           /* 00D176  bsr  $af96 */
L_00D17A:
    wr16(0xFF825C, 0x47);                                /* 00D17A  move.w  #$47,$ff825c.l */
    RTS();                                               /* 00D182  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xD080U: goto L_00D080;
        case 0xD084U: goto L_00D084;
        case 0xD096U: goto L_00D096;
        case 0xD0BEU: goto L_00D0BE;
        case 0xD0C2U: goto L_00D0C2;
        case 0xD0E4U: goto L_00D0E4;
        case 0xD0ECU: goto L_00D0EC;
        case 0xD0F4U: goto L_00D0F4;
        case 0xD0FCU: goto L_00D0FC;
        case 0xD104U: goto L_00D104;
        case 0xD11AU: goto L_00D11A;
        case 0xD136U: goto L_00D136;
        case 0xD146U: goto L_00D146;
        case 0xD17AU: goto L_00D17A;
        }
    }
}

/* 00D14E  game routine */
void sub_00D14E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d4, (d5 & 0xFFFF));                             /* 00D14E  move.w  d5,d4 */
    SETW(d4, shift_op(1, 1, (d4 & 0xFFFF), 1, 16));      /* 00D150  lsl.w  #1,d4 */
    a2 = 0xD184;                                         /* 00D152  lea  $d184(pc),a2 */
    SETW(d1, rd16(a2 + SX16(d4)));                       /* 00D156  move.w  $0(a2,d4.w),d1 */
    a0 = 0xD1A8;                                         /* 00D15A  lea  $d1a8(pc),a0 */
    a0 = a0 + SX16(d1);                                  /* 00D15E  lea  $0(a0,d1.w),a0 */
    CALL(sub_008584, 0xD166U);                           /* 00D162  bsr  $8584 */
L_00D166:
    /* 00D166  addi.w  #$5a0,$266(a5) */
    {
        uint32_t s = 0x5A0;
        uint32_t d = rd16(V_0266);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_0266, r);
        fx = (r < s);
    }
    wr16(V_0268, 0);                                     /* 00D16C  clr.w  $268(a5) */
    RTS();                                               /* 00D170  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xD166U: goto L_00D166;
        }
    }
}

/* 00D184  game routine */
void sub_00D184(void)
{
    SETB(d0, d0 | 8);                                    /* 00D184  ori.b  #$8,d0 */
    wr8(a4, rd8(a4) | 0x19);                             /* 00D188  ori.b  #$19,(a4) */
    /* 00D18C  ori.b  #$23,(a7)+ */
    {
        uint32_t s = 0x23;
        uint32_t ea = a7;
        a7 += 2;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    wr8(0x3D, rd8(0x3D) | 0x39);                         /* 00D190  ori.b  #$39,$3d(a5) */
    /* 00D196  ori.w  #$4a,d6 */
    {
        uint32_t s = 0x4A;
        uint32_t d = (d6 & 0xFFFF);
        uint32_t r = (d | s) & 0xFFFF;
        SETW(d6, r);
        FL_LOGIC(r, 0xFFFF, 0x8000);
    }
    MISS(0xD19AU);
}

/* 00D22A  game routine */
void sub_00D22A(void)
{
    SETB(d5, d5 | 0x64);                                 /* 00D22A  ori.b  #$64,d5 */
    /* 00D22E  ori.w  #$64,-(a4) */
    {
        uint32_t s = 0x64;
        a4 -= 2;
        uint32_t d = rd16(a4);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a4, r);
    }
    /* 00D232  ori.w  #$64,-(a4) */
    {
        uint32_t s = 0x64;
        a4 -= 2;
        uint32_t d = rd16(a4);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a4, r);
    }
    SETB(d1, d1 | 0xFF);                                 /* 00D236  ori.b  #$ff,d1 */
    SETB(d1, d1 | 5);                                    /* 00D23A  ori.b  #$5,d1 */
    /* 00D23E  ori.b  #$ff,$0(a2,d0.w) */
    {
        uint32_t s = 0xFF;
        uint32_t d = rd8(a2 + SX16(d0));
        uint32_t r = (d | s) & 0xFF;
        wr8(a2 + SX16(d0), r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xD244U);
}

/* 00D242  game routine */
void sub_00D242(void)
{
    SETB(d0, d0 | 0xF);                                  /* 00D242  ori.b  #$f,d0 */
    /* 00D246  ori.b  #$2d,(a6)+ */
    {
        uint32_t s = 0x2D;
        uint32_t ea = a6;
        a6 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    set_ccr((get_ccr() | 0x4BU) & 0x1FU);                /* 00D24A  ori  #$4b,ccr */
    /* 00D24E  ori.w  #$69,(a2)+ */
    {
        uint32_t s = 0x69;
        uint32_t ea = a2;
        a2 += 2;
        uint32_t d = rd16(ea);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(ea, r);
    }
    /* 00D252  ori.w  #$79,$88(a2) */
    wr16(a2 + 0x88, rd16(a2 + 0x88) | 0x79);
    { sub_00D258(); return; }
}

/* 00D258  game routine */
void sub_00D258(void)
{
    /* 00D258  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00D25A  move.l  -(a0),d0 */
    a0 -= 4;
    {
        uint32_t v = rd32(a0);
        d0 = v;
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
    MISS(0xD25CU);
}

/* 00D2F2  game routine */
void sub_00D2F2(void)
{
    /* 00D2F2  subq.w  #3,d5 */
    {
        uint32_t s = 3;
        uint32_t d = (d5 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d5, r);
        FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
    }
    MISS(0xD2F4U);
}

/* 00D316  game routine */
void sub_00D316(void)
{
    SETB(d0, d0 | 0);                                    /* 00D316  ori.b  #$0,d0 */
    wr8(a0, rd8(a0) - 0x40);                             /* 00D31A  subi.b  #$40,(a0) */
    a0 -= 1;                                             /* 00D31E  btst  d2,-(a0) */
    /* 00D320  addi.w  #$610,-(a0) */
    {
        uint32_t s = 0x610;
        a0 -= 2;
        uint32_t d = rd16(a0);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(a0, r);
        fx = (r < s);
    }
    /* 00D324  ori.w  #$310,-(a2) */
    {
        uint32_t s = 0x310;
        a2 -= 2;
        uint32_t d = rd16(a2);
        uint32_t r = (d | s) & 0xFFFF;
        wr16(a2, r);
    }
    /* 00D328  ori.b  #$52,-(a7) */
    {
        uint32_t s = 0x52;
        a7 -= 2;
        uint32_t d = rd8(a7);
        uint32_t r = (d | s) & 0xFF;
        wr8(a7, r);
        fn = !!(r & 0x80);
        fv = 0;
        fc = 0;
    }
    /* 00D32C  btst  d2,d6 */
    /* 00D32E  btst  d1,d4 */
    fz = !(d4 & (1u << (d1 & 31)));
    MISS(0xD330U);
}

/* 00D336  game routine */
void sub_00D336(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x960);                                 /* 00D336  move.w  #$960,$266(a5) */
    wr16(V_0268, 0xF);                                   /* 00D33C  move.w  #$f,$268(a5) */
    wr16(V_026A, 0xF);                                   /* 00D342  move.w  #$f,$26a(a5) */
    SETW(d7, 9);                                         /* 00D348  move.w  #$9,d7 */
    d1 = 0;                                              /* 00D34C  moveq  #0,d1 */
    a3 = a4 + 0x7A;                                      /* 00D34E  lea  $7a(a4),a3 */
    do {
        BUDGET_CHECK(0xD352U);
L_00D352:
        a1 = 0xD242;                                         /* 00D352  lea  $d242(pc),a1 */
        SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 1, 16));      /* 00D356  lsl.w  #1,d1 */
        SETW(d2, rd16(a1 + SX16(d1)));                       /* 00D358  move.w  $0(a1,d1.w),d2 */
        a0 = 0xD258;                                         /* 00D35C  lea  $d258(pc),a0 */
        a0 = a0 + SX16(d2);                                  /* 00D360  lea  $0(a0,d2.w),a0 */
        /* 00D364  movem.w  d7/d1/d0,-(a7) */
        PUSH16(d7);
        PUSH16(d1);
        PUSH16(d0);
        /* 00D368  move.w  d1,-(a7) */
        {
            uint32_t v = (d1 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
        }
        wr16(V_026C, 0xA);                                   /* 00D36A  move.w  #$a,$26c(a5) */
        CALL(sub_008584, 0xD374U);                           /* 00D370  bsr  $8584 */
L_00D374:
        /* 00D374  move.w  (a7)+,d1 */
        SETW(d1, rd16(a7));
        a7 += 2;
        d0 = 0;                                              /* 00D376  moveq  #0,d0 */
        SETW(d0, rd16(a3 + SX16(d1)));                       /* 00D378  move.w  $0(a3,d1.w),d0 */
        CALL(sub_007D3E, 0xD380U);                           /* 00D37C  bsr  $7d3e */
L_00D380:
        wr16(V_026C, 0xD);                                   /* 00D380  move.w  #$d,$26c(a5) */
        a0 = V_0261;                                         /* 00D386  lea  $261(a5),a0 */
        CALL(sub_008584, 0xD38EU);                           /* 00D38A  bsr  $8584 */
L_00D38E:
        a0 = 0xD2EF;                                         /* 00D38E  lea  $d2ef(pc),a0 */
        CALL(sub_008584, 0xD396U);                           /* 00D392  bsr  $8584 */
L_00D396:
        /* 00D396  movem.w  (a7)+,d0-d1/d7 */
        d0 = SX16(POP16());
        d1 = SX16(POP16());
        d7 = SX16(POP16());
        SETW(d1, (d1 & 0xFFFF) >> 1);                        /* 00D39A  lsr.w  #1,d1 */
        SETW(d1, d1 + 1);                                    /* 00D39C  addq.w  #1,d1 */
        /* 00D39E  cmp.w  #$7,d1 */
        /* 00D3A2  bne  $d3a6 */
        if ((d1 & 0xFFFF) == 7) {
            d1 = 8;                                              /* 00D3A4  moveq  #8,d1 */
        }
        /* 00D3A6  dbra  d7,$d352 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 00D3AA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xD352U: goto L_00D352;
        case 0xD374U: goto L_00D374;
        case 0xD380U: goto L_00D380;
        case 0xD38EU: goto L_00D38E;
        case 0xD396U: goto L_00D396;
        }
    }
}

/* 00D3AC  game routine */
void sub_00D3AC(void)
{
    /* 00D3AC  movea.l  d3,a0 */
    a0 = d3;
    MISS(0xD3AEU);
}

