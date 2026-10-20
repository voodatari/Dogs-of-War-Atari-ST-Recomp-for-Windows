/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 009048  game routine */
void sub_009048(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = V_1DC4;                                         /* 009048  lea  $1dc4(a5),a1 */
    a0 = V_1E0A;                                         /* 00904C  lea  $1e0a(a5),a0 */
    CALL(sub_00759C, 0x9054U);                           /* 009050  bsr  $759c */
L_009054:
    wr16(V_SECTION, 0xFFFF);                             /* 009054  move.w  #$ffff,$21a(a5) */
    wr16(V_0246, 0xFFFF);                                /* 00905A  move.w  #$ffff,$246(a5) */
    RTS();                                               /* 009060  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9054U: goto L_009054;
        }
    }
}

/* 009062  game routine */
void sub_009062(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_SCROLL, 0xDF);                                /* 009062  move.w  #$df,$280(a5) */
    a0 = 0x73BAE;                                        /* 009068  lea  $73bae.l,a0 */
    wr16(V_SECTION, rd16(V_SECTION) + 1);                /* 00906E  addq.w  #1,$21a(a5) */
    SETW(d1, rd16(V_SECTION));                           /* 009072  move.w  $21a(a5),d1 */
    SETW(d1, (d1 & 0xFFFF) << 1);                        /* 009076  lsl.w  #1,d1 */
    SETW(d0, rd16(a0 + SX16(d1)));                       /* 009078  move.w  $0(a0,d1.w),d0 */
    a0 = a0 + 0x40 + SX16(d0);                           /* 00907C  lea  $40(a0,d0.w),a0 */
    wr32(V_027C, a0);                                    /* 009080  move.l  a0,$27c(a5) */
    CALL(sub_0090AA, 0x9086U);                           /* 009084  bsr  $90aa */
L_009086:
    a0 = 0x73BAE;                                        /* 009086  lea  $73bae.l,a0 */
    wr16(V_0246, rd16(V_0246) + 1);                      /* 00908C  addq.w  #1,$246(a5) */
    SETW(d1, rd16(V_0246));                              /* 009090  move.w  $246(a5),d1 */
    SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 1, 16));      /* 009094  lsl.w  #1,d1 */
    SETW(d0, rd16(a0 + 0x8 + SX16(d1)));                 /* 009096  move.w  $8(a0,d1.w),d0 */
    a0 = a0 + SX16(d0);                                  /* 00909A  lea  $0(a0,d0.w),a0 */
    wr32(V_1DF6, a0);                                    /* 00909E  move.l  a0,$1df6(a5) */
    wr8(V_1DF4, 0xB8);                                   /* 0090A2  move.b  #$b8,$1df4(a5) */
    RTS();                                               /* 0090A8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9086U: goto L_009086;
        }
    }
}

/* 0090AA  game routine */
void sub_0090AA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x2400;                                         /* 0090AA  lea  $2400.l,a0 */
    SETW(d7, 0x43);                                      /* 0090B0  move.w  #$43,d7 */
    do {
        BUDGET_CHECK(0x90B4U);
L_0090B4:
        /* 0090B4  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 0090B6  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 0090B8  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 0090BA  clr.l  (a0)+ */
        wr32(a0, 0);
        a0 += 4;
        /* 0090BC  dbra  d7,$90b4 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_0090C0:
    RTS();                                               /* 0090C0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x90B4U: goto L_0090B4;
        case 0x90C0U: goto L_0090C0;
        }
    }
}

/* 0090C2  game routine */
void sub_0090C2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_02DA;                                         /* 0090C2  lea  $2da(a5),a4 */
    d7 = 1;                                              /* 0090C6  moveq  #1,d7 */
    /* 0090C8  tst.w  $1b4(a5) */
    /* 0090CC  bne  $90d0 */
    if (rd16(V_01B4) == 0) {
L_0090CE:
        SETW(d7, 0);                                         /* 0090CE  clr.w  d7 */
    }
    do {
        BUDGET_CHECK(0x90D0U);
L_0090D0:
        wr16(a4, 3);                                         /* 0090D0  move.w  #$3,(a4) */
        SETW(d0, (d7 & 0xFFFF));                             /* 0090D4  move.w  d7,d0 */
        d0 = (d0 & 0xFFFF) * 6;                              /* 0090D6  mulu.w  #$6,d0 */
        a0 = 0x91C4;                                         /* 0090DA  lea  $91c4(pc),a0 */
        wr16(a4 + 0x2, rd16(a0 + SX16(d0)));                 /* 0090DE  move.w  $0(a0,d0.w),$2(a4) */
        wr16(a4 + 0x4, rd16(a0 + 0x2 + SX16(d0)));           /* 0090E4  move.w  $2(a0,d0.w),$4(a4) */
        wr16(a4 + 0x6, rd16(a0 + 0x4 + SX16(d0)));           /* 0090EA  move.w  $4(a0,d0.w),$6(a4) */
        wr16(a4 + 0x44, 2);                                  /* 0090F0  move.w  #$2,$44(a4) */
        wr16(a4 + 0xC, 0);                                   /* 0090F6  clr.w  $c(a4) */
        wr16(a4 + 0x2A, 0);                                  /* 0090FA  clr.w  $2a(a4) */
        wr16(a4 + 0x50, 0);                                  /* 0090FE  clr.w  $50(a4) */
        wr8(a4 + 0x19, 0);                                   /* 009102  sf  $19(a4) */
        wr8(a4 + 0x12, 0);                                   /* 009106  sf  $12(a4) */
        wr16(a4 + 0x96, 0);                                  /* 00910A  clr.w  $96(a4) */
        wr32(a4 + 0x58, 0x2710);                             /* 00910E  move.l  #$2710,$58(a4) */
        a4 = a4 + 0x9C;                                      /* 009116  lea  $9c(a4),a4 */
        /* 00911A  dbra  d7,$90d0 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00911E:
    /* 00911E  tst.w  $1b4(a5) */
    /* 009122  bne  $9132 */
    if (rd16(V_01B4) == 0) {
L_009126:
        wr16(V_02E0, 0x28);                                  /* 009126  move.w  #$28,$2e0(a5) */
        wr16(V_02DC, 0x92);                                  /* 00912C  move.w  #$92,$2dc(a5) */
    }
    RTS();                                               /* 009132  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x90CEU: goto L_0090CE;
        case 0x90D0U: goto L_0090D0;
        case 0x911EU: goto L_00911E;
        case 0x9126U: goto L_009126;
        }
    }
}

/* 009134  game routine */
void sub_009134(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_02DA;                                         /* 009134  lea  $2da(a5),a4 */
    d7 = 1;                                              /* 009138  moveq  #1,d7 */
    /* 00913A  tst.w  $1b4(a5) */
    /* 00913E  bne  $9142 */
    if (rd16(V_01B4) == 0) {
L_009140:
        SETW(d7, 0);                                         /* 009140  clr.w  d7 */
    }
    do {
        BUDGET_CHECK(0x9142U);
L_009142:
        SETW(d0, (d7 & 0xFFFF));                             /* 009142  move.w  d7,d0 */
        d0 = (d0 & 0xFFFF) * 6;                              /* 009144  mulu.w  #$6,d0 */
        a0 = 0x91C4;                                         /* 009148  lea  $91c4(pc),a0 */
        wr16(a4 + 0x2, rd16(a0 + SX16(d0)));                 /* 00914C  move.w  $0(a0,d0.w),$2(a4) */
        wr16(a4 + 0x4, rd16(a0 + 0x2 + SX16(d0)));           /* 009152  move.w  $2(a0,d0.w),$4(a4) */
        wr16(a4 + 0x6, rd16(a0 + 0x4 + SX16(d0)));           /* 009158  move.w  $4(a0,d0.w),$6(a4) */
        wr16(a4 + 0x44, 2);                                  /* 00915E  move.w  #$2,$44(a4) */
        wr16(a4 + 0x50, 0);                                  /* 009164  clr.w  $50(a4) */
        wr16(a4 + 0x2A, 0);                                  /* 009168  clr.w  $2a(a4) */
        wr8(a4 + 0x19, 0);                                   /* 00916C  sf  $19(a4) */
        wr8(a4 + 0x12, 0);                                   /* 009170  sf  $12(a4) */
        wr16(a4 + 0x96, 0);                                  /* 009174  clr.w  $96(a4) */
        wr16(a4 + 0x36, 1);                                  /* 009178  move.w  #$1,$36(a4) */
        wr16(a4 + 0x94, 0);                                  /* 00917E  clr.w  $94(a4) */
        /* 009182  cmpi.w  #$3,(a4) */
        /* 009186  bhi  $9190 */
        if (rd16(a4) <= 3) {
L_009188:
            /* 009188  tst.w  (a4) */
            /* 00918A  beq  $9190 */
            if (rd16(a4) != 0) {
L_00918C:
                wr16(a4, 3);                                         /* 00918C  move.w  #$3,(a4) */
            }
        }
        a4 = a4 + 0x9C;                                      /* 009190  lea  $9c(a4),a4 */
        /* 009194  dbra  d7,$9142 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_009198:
    /* 009198  tst.w  $376(a5) */
    /* 00919C  bne  $91ae */
    if (rd16(V_0376) == 0) {
L_0091A0:
        wr16(V_02E0, 0x28);                                  /* 0091A0  move.w  #$28,$2e0(a5) */
        wr16(V_02DC, 0x92);                                  /* 0091A6  move.w  #$92,$2dc(a5) */
        /* 0091AC  bra  $91c2 */
    } else {
        /* 0091AE  tst.w  $2da(a5) */
        /* 0091B2  bne  $91c2 */
        if (rd16(V_02DA) == 0) {
            wr16(V_037C, 0);                                     /* 0091B6  move.w  #$0,$37c(a5) */
            wr16(V_0378, 0x92);                                  /* 0091BC  move.w  #$92,$378(a5) */
        }
    }
L_0091C2:
    RTS();                                               /* 0091C2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9140U: goto L_009140;
        case 0x9142U: goto L_009142;
        case 0x9188U: goto L_009188;
        case 0x918CU: goto L_00918C;
        case 0x9198U: goto L_009198;
        case 0x91A0U: goto L_0091A0;
        case 0x91C2U: goto L_0091C2;
        }
    }
}

/* 0091C4  game routine */
void sub_0091C4(void)
{
    wr32(a0 + 0x76, rd32(a0 + 0x76) | 0x940000);         /* 0091C4  ori.l  #$940000,$76(a0) */
    wr32(a4, rd32(a4) | 0x2841ED);                       /* 0091CC  ori.l  #$2841ed,(a4) */
    /* 0091D2  subi.b  #$ed,(a2) */
    {
        uint32_t s = 0xED;
        uint32_t d = rd8(a2);
        uint32_t r = (d - s) & 0xFF;
        wr8(a2, r);
        FL_SUB(s, d, r, 0xFF, 0x80, 1);
    }
    MISS(0x91D6U);
}

/* 0091D0  game routine */
void sub_0091D0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = V_ENEMIES;                                      /* 0091D0  lea  $412(a5),a0 */
    a1 = V_1DC2;                                         /* 0091D4  lea  $1dc2(a5),a1 */
    do {
        BUDGET_CHECK(0x91D8U);
L_0091D8:
        /* 0091D8  clr.b  (a0)+ */
        wr8(a0, 0);
        a0 += 1;
        /* 0091DA  cmpa.l  a1,a0 */
        /* 0091DC  bls  $91d8 */
    } while (a0 <= a1);
L_0091DE:
    RTS();                                               /* 0091DE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x91D8U: goto L_0091D8;
        case 0x91DEU: goto L_0091DE;
        }
    }
}

/* 0091E0  End of every logic frame: waits two VBLs (the game runs at 25 Hz) and swaps the screen buffers */
void wait_two_vbl(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x91E0U);
        /* 0091E0  tst.w  $26e(a5) */
        /* 0091E4  beq  $91ec */
        IDLE_EVERY(0x91E0U);
        if (rd16(V_026E) != 0) {
            /* 0091E6  move.w  #$0,$ff8240.l */
            wr16(HW_PALETTE, 0);
        } else {
L_0091EC:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 0091EC  or.w  d0,d1 */
        }
        /* 0091EE  tst.w  $1df2(a5) */
        /* 0091F2  bpl  $91e0 */
    } while ((int16_t)rd16(V_1DF2) >= 0);
L_0091F4:
    wr16(V_1DF2, 1);                                     /* 0091F4  move.w  #$1,$1df2(a5) */
    d0 = rd32(V_DRAW_BUFFER);                            /* 0091FA  move.l  $28e(a5),d0 */
    CALL(sub_007B2A, 0x9202U);                           /* 0091FE  bsr  $7b2a */
L_009202:
    CALL(wait_vbl, 0x9206U);                             /* 009202  bsr  $79dc */
L_009206:
    wr16(HW_PALETTE, 0);                                 /* 009206  clr.w  $ff8240.l */
    wr16(V_0270, 0);                                     /* 00920C  clr.w  $270(a5) */
    CALL(sub_0075C4, 0x9214U);                           /* 009210  bsr  $75c4 */
L_009214:
    d1 = 0x20000;                                        /* 009214  move.l  #$20000,d1 */
    d0 = 0x18000;                                        /* 00921A  move.l  #$18000,d0 */
    /* 009220  cmp.l  $28e(a5),d0 */
    /* 009224  bne  $9228 */
    if (d0 == rd32(V_DRAW_BUFFER)) {
L_009226:
        /* 009226  exg  d0,d1 */
        { uint32_t t = d0; d0 = d1; d1 = t; }
    }
L_009228:
    wr32(V_DRAW_BUFFER, d0);                             /* 009228  move.l  d0,$28e(a5) */
    RTS();                                               /* 00922C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x91ECU: goto L_0091EC;
        case 0x91F4U: goto L_0091F4;
        case 0x9202U: goto L_009202;
        case 0x9206U: goto L_009206;
        case 0x9214U: goto L_009214;
        case 0x9226U: goto L_009226;
        case 0x9228U: goto L_009228;
        }
    }
}

/* 00922E  game routine */
void sub_00922E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_004C06, 0x9232U);                           /* 00922E  bsr  $4c06 */
L_009232:
    a1 = 0x2400;                                         /* 009232  lea  $2400.l,a1 */
    a0 = 0x2800;                                         /* 009238  lea  $2800.l,a0 */
    /* 00923E  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009240  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009242  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009244  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009246  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009248  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00924A  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00924C  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00924E  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009250  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009252  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009254  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009256  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009258  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00925A  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00925C  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00925E  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 009260  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    wr16(V_0284, rd16(V_02DC));                          /* 009262  move.w  $2dc(a5),$284(a5) */
    wr16(V_0286, rd16(V_0378));                          /* 009268  move.w  $378(a5),$286(a5) */
    wr16(V_0288, rd16(V_02DE));                          /* 00926E  move.w  $2de(a5),$288(a5) */
    wr16(V_028A, rd16(V_037A));                          /* 009274  move.w  $37a(a5),$28a(a5) */
    a4 = V_02DA;                                         /* 00927A  lea  $2da(a5),a4 */
    wr16(V_027A, 0);                                     /* 00927E  clr.w  $27a(a5) */
    CALL(sub_009290, 0x9286U);                           /* 009282  bsr  $9290 */
L_009286:
    a4 = V_0376;                                         /* 009286  lea  $376(a5),a4 */
    /* 00928A  move.w  #$1,$27a(a5) */
    wr16(V_027A, 1);
    { sub_009290(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9232U: goto L_009232;
        case 0x9286U: goto L_009286;
        }
    }
}

/* 009290  game routine */
void sub_009290(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 009290  tst.w  (a4) */
    /* 009292  beq  $9604 */
    if (rd16(a4) != 0) {
L_009296:
        /* 009296  tst.w  $50(a4) */
        /* 00929A  beq  $92a4 */
        if (rd16(a4 + 0x50) != 0) {
L_00929C:
            CALL(sub_00AA2E, 0x92A0U);                           /* 00929C  bsr  $aa2e */
L_0092A0:
            /* 0092A0  bra  $9604 */
        } else {
L_0092A4:
            /* 0092A4  tst.w  $98(a4) */
            /* 0092A8  beq  $92be */
            if (rd16(a4 + 0x98) != 0) {
                /* 0092AA  subq.w  #1,$98(a4) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a4 + 0x98);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(a4 + 0x98, r);
                    fz = (r == 0);
                }
                /* 0092AE  bne  $92be */
                if (COND(7)) {
                    /* 0092B0  tst.w  $54(a4) */
                    /* 0092B4  bne  $92be */
                    if (rd16(a4 + 0x54) == 0) {
                        SETW(d0, rd16(a4 + 0x9A));                           /* 0092B6  move.w  $9a(a4),d0 */
                        wr16(a4 + 0x6, (d0 & 0xFFFF));                       /* 0092BA  move.w  d0,$6(a4) */
                    }
                }
            }
L_0092BE:
            wr32(a4 + 0x2E, 0);                                  /* 0092BE  clr.l  $2e(a4) */
            wr32(a4 + 0x32, 0);                                  /* 0092C2  clr.l  $32(a4) */
            wr16(a4 + 0x54, 0);                                  /* 0092C6  clr.w  $54(a4) */
            /* 0092CA  tst.w  $24(a4) */
            /* 0092CE  bne  $9604 */
            if (rd16(a4 + 0x24) == 0) {
L_0092D2:
                wr16(V_028C, 0);                                     /* 0092D2  clr.w  $28c(a5) */
                CALL(sub_009860, 0x92DAU);                           /* 0092D6  bsr  $9860 */
L_0092DA:
                /* 0092DA  tst.b  $12(a4) */
                /* 0092DE  bne  $92e4 */
                if (rd8(a4 + 0x12) == 0) {
L_0092E0:
                    wr16(a4 + 0x56, 0);                                  /* 0092E0  clr.w  $56(a4) */
                }
L_0092E4:
                /* 0092E4  tst.w  $56(a4) */
                /* 0092E8  bne  $9306 */
                if (rd16(a4 + 0x56) == 0) {
L_0092EA:
                    /* 0092EA  tst.b  $12(a4) */
                    /* 0092EE  beq  $9306 */
                    if (rd8(a4 + 0x12) != 0) {
L_0092F0:
                        SETB(d7, rd8(a4 + 0xE));                             /* 0092F0  move.b  $e(a4),d7 */
                        SETB(d7, d7 | rd8(a4 + 0xF));                        /* 0092F4  or.b  $f(a4),d7 */
                        SETB(d7, d7 | rd8(a4 + 0x10));                       /* 0092F8  or.b  $10(a4),d7 */
                        /* 0092FC  or.b  $11(a4),d7 */
                        {
                            uint32_t s = rd8(a4 + 0x11);
                            uint32_t d = (d7 & 0xFF);
                            uint32_t r = (d | s) & 0xFF;
                            SETB(d7, r);
                            fz = (r == 0);
                        }
                        /* 009300  bne  $9306 */
                        if (COND(7)) {
                            wr8(a4 + 0x56, 0xFF);                                /* 009302  st  $56(a4) */
                        }
                    }
                }
L_009306:
                /* 009306  cmpi.w  #$e1,$2(a4) */
                /* 00930C  bhi  $9330 */
                HOOK(32, 0x9306U);
                if (rd16(a4 + 0x2) <= 0xE1) {
L_00930E:
                    /* 00930E  tst.b  $e(a4) */
                    /* 009312  beq  $9330 */
                    if (rd8(a4 + 0xE) != 0) {
L_009314:
                        wr8(a4 + 0x30, 0xFF);                                /* 009314  st  $30(a4) */
                        /* 009318  tst.b  $14(a4) */
                        /* 00931C  beq  $9330 */
                        if (rd8(a4 + 0x14) != 0) {
L_00931E:
                            /* 00931E  tst.w  $56(a4) */
                            /* 009322  bne  $932c */
                            if (rd16(a4 + 0x56) == 0) {
L_009324:
                                wr16(a4 + 0x2, rd16(a4 + 0x2) + 3);                  /* 009324  addq.w  #3,$2(a4) */
                                wr8(a4 + 0x54, 0xFF);                                /* 009328  st  $54(a4) */
                            }
                            wr8(V_028C, 0xFF);                                   /* 00932C  st  $28c(a5) */
                        }
                    }
                }
L_009330:
                /* 009330  tst.b  $f(a4) */
                /* 009334  beq  $935a */
                if (rd8(a4 + 0xF) != 0) {
L_009336:
                    wr8(a4 + 0x2E, 0xFF);                                /* 009336  st  $2e(a4) */
                    /* 00933A  cmpi.w  #$42,$2(a4) */
                    /* 009340  bls  $935a */
                    if (rd16(a4 + 0x2) > 0x42) {
L_009342:
                        /* 009342  tst.b  $15(a4) */
                        /* 009346  beq  $935a */
                        if (rd8(a4 + 0x15) != 0) {
L_009348:
                            /* 009348  tst.w  $56(a4) */
                            /* 00934C  bne  $9356 */
                            if (rd16(a4 + 0x56) == 0) {
L_00934E:
                                wr16(a4 + 0x2, rd16(a4 + 0x2) - 3);                  /* 00934E  subq.w  #3,$2(a4) */
                                wr8(a4 + 0x54, 0xFF);                                /* 009352  st  $54(a4) */
                            }
                            wr8(V_028C, 0xFF);                                   /* 009356  st  $28c(a5) */
                        }
                    }
                }
L_00935A:
                /* 00935A  tst.b  $10(a4) */
                /* 00935E  beq  $9390 */
                if (rd8(a4 + 0x10) != 0) {
L_009360:
                    wr8(a4 + 0x32, 0xFF);                                /* 009360  st  $32(a4) */
                    /* 009364  cmpi.w  #$a,$4(a4) */
                    /* 00936A  bhi  $9378 */
                    if (rd16(a4 + 0x4) <= 0xA) {
L_00936C:
                        /* 00936C  tst.b  $10(a4) */
                        /* 009370  beq  $9378 */
                        if (rd8(a4 + 0x10) != 0) {
L_009372:
                            wr16(a4 + 0x4, 0xA);                                 /* 009372  move.w  #$a,$4(a4) */
                        }
                    }
L_009378:
                    /* 009378  tst.b  $17(a4) */
                    /* 00937C  beq  $9390 */
                    if (rd8(a4 + 0x17) != 0) {
L_00937E:
                        /* 00937E  tst.w  $56(a4) */
                        /* 009382  bne  $938c */
                        if (rd16(a4 + 0x56) == 0) {
L_009384:
                            wr16(a4 + 0x4, rd16(a4 + 0x4) - 3);                  /* 009384  subq.w  #3,$4(a4) */
                            wr8(a4 + 0x54, 0xFF);                                /* 009388  st  $54(a4) */
                        }
                        wr8(V_028C, 0xFF);                                   /* 00938C  st  $28c(a5) */
                    }
                }
L_009390:
                /* 009390  tst.b  $11(a4) */
                /* 009394  beq  $93ba */
                if (rd8(a4 + 0x11) != 0) {
                    wr8(a4 + 0x34, 0xFF);                                /* 009396  st  $34(a4) */
                    /* 00939A  cmpi.w  #$a8,$4(a4) */
                    /* 0093A0  bhi  $93ba */
                    if (rd16(a4 + 0x4) <= 0xA8) {
                        /* 0093A2  tst.b  $16(a4) */
                        /* 0093A6  beq  $93ba */
                        if (rd8(a4 + 0x16) != 0) {
                            /* 0093A8  tst.w  $56(a4) */
                            /* 0093AC  bne  $93b6 */
                            if (rd16(a4 + 0x56) == 0) {
                                wr16(a4 + 0x4, rd16(a4 + 0x4) + 3);                  /* 0093AE  addq.w  #3,$4(a4) */
                                wr8(a4 + 0x54, 0xFF);                                /* 0093B2  st  $54(a4) */
                            }
                            wr8(V_028C, 0xFF);                                   /* 0093B6  st  $28c(a5) */
                        }
                    }
                }
L_0093BA:
                /* 0093BA  tst.w  $142(a5) */
                CALL(sub_009CAC, 0x93C2U);                           /* 0093BE  bsr  $9cac */
L_0093C2:
                /* 0093C2  tst.b  $12(a4) */
                /* 0093C6  bne  $93d2 */
                if (rd8(a4 + 0x12) == 0) {
L_0093CA:
                    wr8(a4 + 0x19, 0);                                   /* 0093CA  clr.b  $19(a4) */
                    /* 0093CE  bra  $9542 */
                } else {
L_0093D2:
                    /* 0093D2  nop */
                    /* 0093D4  tst.b  $19(a4) */
                    /* 0093D8  bne  $9542 */
                    if (rd8(a4 + 0x19) == 0) {
L_0093DC:
                        /* 0093DC  cmpi.w  #$12,$c(a4) */
                        /* 0093E2  beq  $9474 */
                        if (rd16(a4 + 0xC) != 0x12) {
L_0093E6:
                            /* 0093E6  cmpi.w  #$11,$c(a4) */
                            /* 0093EC  beq  $9474 */
                            if (rd16(a4 + 0xC) == 0x11) {
                                goto L_009474;
                            }
L_0093F0:
                            SETB(d0, rd8(a4 + 0x4E));                            /* 0093F0  move.b  $4e(a4),d0 */
                            /* 0093F4  cmpi.w  #$41,$62(a4) */
                            /* 0093FA  beq  $93fe */
                            if (rd16(a4 + 0x62) != 0x41) {
L_0093FC:
                                d0 = 0xFFFFFFFF;                                     /* 0093FC  moveq  #-1,d0 */
                            }
                            wr8(a4 + 0x19, (d0 & 0xFF));                         /* 0093FE  move.b  d0,$19(a4) */
                            /* 009402  cmpi.w  #$f,$c(a4) */
                            /* 009408  beq  $944a */
                            if (rd16(a4 + 0xC) != 0xF) {
L_00940A:
                                /* 00940A  cmpi.w  #$10,$c(a4) */
                                /* 009410  beq  $944a */
                                if (rd16(a4 + 0xC) == 0x10) {
                                    goto L_00944A;
                                }
L_009412:
                                /* 009412  cmpi.w  #$e,$c(a4) */
                                /* 009418  beq  $944a */
                                if (rd16(a4 + 0xC) == 0xE) {
                                    goto L_00944A;
                                }
L_00941A:
                                /* 00941A  cmpi.w  #$47,$62(a4) */
                                /* 009420  beq  $945c */
                                if (rd16(a4 + 0x62) == 0x47) {
                                    goto L_00945C;
                                }
L_009424:
                                CALL(sub_00ADF2, 0x9428U);                           /* 009424  bsr  $adf2 */
L_009428:
                                /* 009428  tst.w  d6 */
                                /* 00942A  bmi  $9542 */
                                if ((int16_t)(d6 & 0xFFFF) < 0) {
                                    goto L_009542;
                                }
L_00942E:
                                SETW(d0, rd16(a4 + 0xC));                            /* 00942E  move.w  $c(a4),d0 */
                                /* 009432  move.l  a0,-(a7) */
                                {
                                    uint32_t v = a0;
                                    a7 -= 4;
                                    wr32(a7, v);
                                }
                                a0 = 0xAF46;                                         /* 009434  lea  $af46(pc),a0 */
                                SETW(d0, (d0 & 0xFFFF) << 1);                        /* 009438  lsl.w  #1,d0 */
                                SETW(d0, rd16(a0 + SX16(d0)));                       /* 00943A  move.w  $0(a0,d0.w),d0 */
                                CALL(sub_00E6EC, 0x9442U);                           /* 00943E  bsr  $e6ec */
L_009442:
                                /* 009442  movea.l  (a7)+,a0 */
                                a0 = rd32(a7);
                                a7 += 4;
                                CALL(sub_009614, 0x9448U);                           /* 009444  bsr  $9614 */
L_009448:
                                /* 009448  bra  $9454 */
                            } else {
L_00944A:
                                CALL(sub_00ADF2, 0x944EU);                           /* 00944A  bsr  $adf2 */
L_00944E:
                                /* 00944E  tst.w  d6 */
                                /* 009450  bmi  $9542 */
                                if ((int16_t)(d6 & 0xFFFF) < 0) {
                                    goto L_009542;
                                }
                            }
L_009454:
                            CALL(sub_009FE0, 0x9458U);                           /* 009454  bsr  $9fe0 */
L_009458:
                            /* 009458  bra  $9542 */
                            goto L_009542;
L_00945C:
                            CALL(sub_00ADF2, 0x9460U);                           /* 00945C  bsr  $adf2 */
L_009460:
                            /* 009460  tst.w  d6 */
                            /* 009462  bmi  $9542 */
                            if ((int16_t)(d6 & 0xFFFF) < 0) {
                                goto L_009542;
                            }
                            CALL(sub_00AB5C, 0x946AU);                           /* 009466  bsr  $ab5c */
L_00946A:
                            d0 = 0xFFFFFFFF;                                     /* 00946A  moveq  #-1,d0 */
                            wr8(a4 + 0x19, (d0 & 0xFF));                         /* 00946C  move.b  d0,$19(a4) */
                            /* 009470  bra  $9542 */
                        } else {
L_009474:
                            /* 009474  cmpi.w  #$12,$c(a4) */
                            /* 00947A  bne  $94f8 */
                            if (rd16(a4 + 0xC) == 0x12) {
                                CALL(sub_00ADF2, 0x9482U);                           /* 00947E  bsr  $adf2 */
L_009482:
                                /* 009482  tst.w  d6 */
                                /* 009484  bmi  $9604 */
                                if ((int16_t)(d6 & 0xFFFF) < 0) {
                                    goto L_009604;
                                }
                                /* 009488  tst.w  $98(a4) */
                                /* 00948C  bne  $9604 */
                                if (rd16(a4 + 0x98) != 0) {
                                    goto L_009604;
                                }
                                /* 009490  tst.b  $19(a4) */
                                /* 009494  bne  $9542 */
                                if (rd8(a4 + 0x19) != 0) {
                                    goto L_009542;
                                }
                                SETB(d0, rd8(a4 + 0x4E));                            /* 009498  move.b  $4e(a4),d0 */
                                wr8(a4 + 0x19, (d0 & 0xFF));                         /* 00949C  move.b  d0,$19(a4) */
                                wr16(a4 + 0x98, 4);                                  /* 0094A0  move.w  #$4,$98(a4) */
                                SETW(d0, rd16(a4 + 0x6));                            /* 0094A6  move.w  $6(a4),d0 */
                                wr16(a4 + 0x9A, (d0 & 0xFFFF));                      /* 0094AA  move.w  d0,$9a(a4) */
                                a0 = 0x9F04;                                         /* 0094AE  lea  $9f04(pc),a0 */
                                d0 = 0;                                              /* 0094B2  moveq  #0,d0 */
                                SETB(d0, rd8(a4 + 0x36));                            /* 0094B4  move.b  $36(a4),d0 */
                                SETW(d0, d0 & 0xF);                                  /* 0094B8  andi.w  #$f,d0 */
                                d0 = (d0 & 0xFFFF) * 0xA;                            /* 0094BC  mulu.w  #$a,d0 */
                                a0 += d0;                                            /* 0094C0  adda.l  d0,a0 */
                                SETW(d0, rd16(a4 + 0x2));                            /* 0094C2  move.w  $2(a4),d0 */
                                SETW(d1, rd16(a4 + 0x4));                            /* 0094C6  move.w  $4(a4),d1 */
                                /* 0094CA  tst.w  (a0) */
                                /* 0094CC  bmi  $9542 */
                                if ((int16_t)rd16(a0) < 0) {
                                    goto L_009542;
                                }
                                /* 0094CE  add.w  (a0)+,d0 */
                                {
                                    uint32_t s = rd16(a0);
                                    a0 += 2;
                                    uint32_t d = (d0 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d0, r);
                                }
                                /* 0094D0  add.w  (a0)+,d1 */
                                {
                                    uint32_t s = rd16(a0);
                                    a0 += 2;
                                    uint32_t d = (d1 & 0xFFFF);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    SETW(d1, r);
                                    fx = (r < s);
                                }
                                /* 0094D2  move.w  (a0)+,d2 */
                                SETW(d2, rd16(a0));
                                a0 += 2;
                                /* 0094D4  move.w  (a0)+,d3 */
                                SETW(d3, rd16(a0));
                                a0 += 2;
                                /* 0094D6  move.w  (a0)+,d4 */
                                SETW(d4, rd16(a0));
                                a0 += 2;
                                /* 0094D8  tst.w  $27a(a5) */
                                /* 0094DC  beq  $94e4 */
                                if (rd16(V_027A) != 0) {
                                    /* 0094E0  subi.w  #$28,d4 */
                                    {
                                        uint32_t s = 0x28;
                                        uint32_t d = (d4 & 0xFFFF);
                                        uint32_t r = (d - s) & 0xFFFF;
                                        SETW(d4, r);
                                        fx = (s > d);
                                    }
                                }
                                wr16(a4 + 0x6, (d4 & 0xFFFF));                       /* 0094E4  move.w  d4,$6(a4) */
                                SETW(d5, rd16(V_027A));                              /* 0094E8  move.w  $27a(a5),d5 */
                                CALL(sub_006BEA, 0x94F0U);                           /* 0094EC  bsr  $6bea */
L_0094F0:
                                wr8(a4 + 0x28, 0xFF);                                /* 0094F0  st  $28(a4) */
                                /* 0094F4  bra  $9604 */
                                goto L_009604;
                            }
                            CALL(sub_00ADF2, 0x94FCU);                           /* 0094F8  bsr  $adf2 */
L_0094FC:
                            /* 0094FC  tst.w  d6 */
                            /* 0094FE  bmi  $9542 */
                            if ((int16_t)(d6 & 0xFFFF) >= 0) {
                                d5 = 0;                                              /* 009502  moveq  #0,d5 */
                                SETB(d5, rd8(a4 + 0x36));                            /* 009504  move.b  $36(a4),d5 */
                                SETW(d5, d5 & 0xF);                                  /* 009508  andi.w  #$f,d5 */
                                d5 = (d5 & 0xFFFF) * 8;                              /* 00950C  mulu.w  #$8,d5 */
                                a0 = 0xA89C;                                         /* 009510  lea  $a89c(pc),a0 */
                                SETW(d0, rd16(a0 + SX16(d5)));                       /* 009514  move.w  $0(a0,d5.w),d0 */
                                /* 009518  tst.w  d0 */
                                /* 00951A  bmi  $9542 */
                                if ((int16_t)(d0 & 0xFFFF) >= 0) {
                                    SETW(d0, d0 + rd16(a4 + 0x2));                       /* 00951C  add.w  $2(a4),d0 */
                                    SETW(d1, rd16(a0 + 0x2 + SX16(d5)));                 /* 009520  move.w  $2(a0,d5.w),d1 */
                                    /* 009524  add.w  $4(a4),d1 */
                                    {
                                        uint32_t s = rd16(a4 + 0x4);
                                        uint32_t d = (d1 & 0xFFFF);
                                        uint32_t r = (d + s) & 0xFFFF;
                                        SETW(d1, r);
                                        fx = (r < s);
                                    }
                                    SETW(d2, 0x64);                                      /* 009528  move.w  #$64,d2 */
                                    SETW(d3, rd16(a0 + 0x4 + SX16(d5)));                 /* 00952C  move.w  $4(a0,d5.w),d3 */
                                    SETW(d4, rd16(a0 + 0x6 + SX16(d5)));                 /* 009530  move.w  $6(a0,d5.w),d4 */
                                    SETW(d5, rd16(V_027A));                              /* 009534  move.w  $27a(a5),d5 */
                                    CALL(sub_00B6A6, 0x953CU);                           /* 009538  bsr  $b6a6 */
L_00953C:
                                    d0 = 8;                                              /* 00953C  moveq  #8,d0 */
                                    CALL(sub_00E6EC, 0x9542U);                           /* 00953E  bsr  $e6ec */
                                }
                            }
                        }
                    }
                }
L_009542:
                wr16(a4 + 0x28, 0);                                  /* 009542  clr.w  $28(a4) */
                a0 = 0x9C2C;                                         /* 009546  lea  $9c2c(pc),a0 */
                d1 = 0;                                              /* 00954A  clr.l  d1 */
                SETW(d0, (d1 & 0xFFFF));                             /* 00954C  move.w  d1,d0 */
                /* 00954E  move.b  $1de2(a5),d1 */
                /* 009552  bmi  $95fa */
                SETB(d1, rd8(V_1DE2));
                if ((int8_t)(d1 & 0xFF) >= 0) {
L_009556:
                    SETB(d0, rd8(a0 + SX16(d1)));                        /* 009556  move.b  $0(a0,d1.w),d0 */
                    /* 00955A  cmp.b  #$2,d0 */
                    /* 00955E  bcs  $95b0 */
                    if ((d0 & 0xFF) >= 2) {
L_009560:
                        SETW(d3, rd16(V_027A));                              /* 009560  move.w  $27a(a5),d3 */
                        SETW(d3, d3 + 2);                                    /* 009564  addq.w  #2,d3 */
                        /* 009566  cmp.w  d3,d0 */
                        /* 009568  bne  $95fa */
                        if ((d0 & 0xFFFF) != (d3 & 0xFFFF)) {
                            goto L_0095FA;
                        }
                        wr8(V_1DE2, 0);                                      /* 00956C  clr.b  $1de2(a5) */
                        wr16(a4 + 0x60, rd16(a4 + 0x60) + 1);                /* 009570  addq.w  #1,$60(a4) */
                        wr8(V_1DE2, 0);                                      /* 009574  clr.b  $1de2(a5) */
                        for (;;) {
                            BUDGET_CHECK(0x9578U);
L_009578:
                            SETW(d0, rd16(a4 + 0xC));                            /* 009578  move.w  $c(a4),d0 */
                            SETW(d1, rd16(a4 + 0x60));                           /* 00957C  move.w  $60(a4),d1 */
                            CALL(sub_00AAF8, 0x9584U);                           /* 009580  bsr  $aaf8 */
L_009584:
                            /* 009584  tst.w  d0 */
                            /* 009586  bne  $958e */
                            if ((d0 & 0xFFFF) != 0) {
                                goto L_00958E;
                            }
                            wr16(a4 + 0x60, 0);                                  /* 009588  clr.w  $60(a4) */
                            /* 00958C  bra  $9578 */
                        }
L_00958E:
                        wr16(a4 + 0x62, (d0 & 0xFFFF));                      /* 00958E  move.w  d0,$62(a4) */
                        /* 009592  cmpi.w  #$8,$c(a4) */
                        /* 009598  bne  $95ae */
                        if (rd16(a4 + 0xC) == 8) {
                            wr16(a4 + 0x64, 1);                                  /* 00959A  move.w  #$1,$64(a4) */
                            /* 0095A0  cmpi.w  #$47,$62(a4) */
                            /* 0095A6  bne  $95ae */
                            if (rd16(a4 + 0x62) == 0x47) {
                                wr16(a4 + 0x64, 9);                                  /* 0095A8  move.w  #$9,$64(a4) */
                            }
                        }
                        /* 0095AE  bra  $95fa */
                    } else {
                        /* 0095B0  cmp.w  $27a(a5),d0 */
                        /* 0095B4  bne  $95fa */
                        if ((d0 & 0xFFFF) == rd16(V_027A)) {
                            wr8(V_1DE2, 0);                                      /* 0095B6  clr.b  $1de2(a5) */
                            do {
                                BUDGET_CHECK(0x95BAU);
L_0095BA:
                                /* 0095BA  addq.w  #1,$c(a4) */
                                {
                                    uint32_t s = 1;
                                    uint32_t d = rd16(a4 + 0xC);
                                    uint32_t r = (d + s) & 0xFFFF;
                                    wr16(a4 + 0xC, r);
                                    fx = (r < s);
                                }
                                wr16(a4 + 0x60, 0);                                  /* 0095BE  clr.w  $60(a4) */
                                /* 0095C2  cmpi.w  #$13,$c(a4) */
                                /* 0095C8  bcs  $95ce */
                                if (rd16(a4 + 0xC) >= 0x13) {
                                    wr16(a4 + 0xC, 0);                                   /* 0095CA  clr.w  $c(a4) */
                                }
                                for (;;) {
                                    BUDGET_CHECK(0x95CEU);
L_0095CE:
                                    SETW(d0, rd16(a4 + 0xC));                            /* 0095CE  move.w  $c(a4),d0 */
                                    SETW(d1, rd16(a4 + 0x60));                           /* 0095D2  move.w  $60(a4),d1 */
                                    CALL(sub_00AAF8, 0x95DAU);                           /* 0095D6  bsr  $aaf8 */
L_0095DA:
                                    /* 0095DA  tst.w  d0 */
                                    /* 0095DC  bne  $95e4 */
                                    if ((d0 & 0xFFFF) != 0) {
                                        goto L_0095E4;
                                    }
                                    wr16(a4 + 0x60, 0);                                  /* 0095DE  clr.w  $60(a4) */
                                    /* 0095E2  bra  $95ce */
                                }
L_0095E4:
                                wr16(a4 + 0x62, (d0 & 0xFFFF));                      /* 0095E4  move.w  d0,$62(a4) */
                                a2 = a4 + 0x66;                                      /* 0095E8  lea  $66(a4),a2 */
                                SETW(d0, rd16(a4 + 0xC));                            /* 0095EC  move.w  $c(a4),d0 */
                                /* 0095F0  tst.b  $0(a2,d0.w) */
                                /* 0095F4  beq  $95ba */
                            } while (rd8(a2 + SX16(d0)) == 0);
                            CALL(sub_009DB8, 0x95FAU);                           /* 0095F6  bsr  $9db8 */
                        }
                    }
                }
L_0095FA:
                /* 0095FA  subq.b  #1,$19(a4) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd8(a4 + 0x19);
                    uint32_t r = (d - s) & 0xFF;
                    wr8(a4 + 0x19, r);
                    fn = !!(r & 0x80);
                }
                /* 0095FE  bpl  $9604 */
                if (COND(11)) {
L_009600:
                    wr8(a4 + 0x19, rd8(a4 + 0x19) + 1);                  /* 009600  addq.b  #1,$19(a4) */
                }
            }
        }
    }
L_009604:
    /* 009604  cmpi.w  #$a8,$4(a4) */
    /* 00960A  bls  $9612 */
    if (rd16(a4 + 0x4) > 0xA8) {
L_00960C:
        wr16(a4 + 0x4, 0xA8);                                /* 00960C  move.w  #$a8,$4(a4) */
    }
L_009612:
    RTS();                                               /* 009612  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9296U: goto L_009296;
        case 0x929CU: goto L_00929C;
        case 0x92A0U: goto L_0092A0;
        case 0x92A4U: goto L_0092A4;
        case 0x92BEU: goto L_0092BE;
        case 0x92D2U: goto L_0092D2;
        case 0x92DAU: goto L_0092DA;
        case 0x92E0U: goto L_0092E0;
        case 0x92E4U: goto L_0092E4;
        case 0x92EAU: goto L_0092EA;
        case 0x92F0U: goto L_0092F0;
        case 0x9306U: goto L_009306;
        case 0x930EU: goto L_00930E;
        case 0x9314U: goto L_009314;
        case 0x931EU: goto L_00931E;
        case 0x9324U: goto L_009324;
        case 0x9330U: goto L_009330;
        case 0x9336U: goto L_009336;
        case 0x9342U: goto L_009342;
        case 0x9348U: goto L_009348;
        case 0x934EU: goto L_00934E;
        case 0x935AU: goto L_00935A;
        case 0x9360U: goto L_009360;
        case 0x936CU: goto L_00936C;
        case 0x9372U: goto L_009372;
        case 0x9378U: goto L_009378;
        case 0x937EU: goto L_00937E;
        case 0x9384U: goto L_009384;
        case 0x9390U: goto L_009390;
        case 0x93BAU: goto L_0093BA;
        case 0x93C2U: goto L_0093C2;
        case 0x93CAU: goto L_0093CA;
        case 0x93D2U: goto L_0093D2;
        case 0x93DCU: goto L_0093DC;
        case 0x93E6U: goto L_0093E6;
        case 0x93F0U: goto L_0093F0;
        case 0x93FCU: goto L_0093FC;
        case 0x940AU: goto L_00940A;
        case 0x9412U: goto L_009412;
        case 0x941AU: goto L_00941A;
        case 0x9424U: goto L_009424;
        case 0x9428U: goto L_009428;
        case 0x942EU: goto L_00942E;
        case 0x9442U: goto L_009442;
        case 0x9448U: goto L_009448;
        case 0x944EU: goto L_00944E;
        case 0x9454U: goto L_009454;
        case 0x9458U: goto L_009458;
        case 0x9460U: goto L_009460;
        case 0x946AU: goto L_00946A;
        case 0x9482U: goto L_009482;
        case 0x94F0U: goto L_0094F0;
        case 0x94FCU: goto L_0094FC;
        case 0x953CU: goto L_00953C;
        case 0x9542U: goto L_009542;
        case 0x9556U: goto L_009556;
        case 0x9560U: goto L_009560;
        case 0x9578U: goto L_009578;
        case 0x9584U: goto L_009584;
        case 0x95BAU: goto L_0095BA;
        case 0x95CEU: goto L_0095CE;
        case 0x95DAU: goto L_0095DA;
        case 0x95FAU: goto L_0095FA;
        case 0x9600U: goto L_009600;
        case 0x9604U: goto L_009604;
        case 0x960CU: goto L_00960C;
        case 0x9612U: goto L_009612;
        }
    }
}

/* 009614  game routine */
void sub_009614(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 009614  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    SETW(d0, rd16(a4 + 0x2));                            /* 009618  move.w  $2(a4),d0 */
    SETW(d1, rd16(a4 + 0x4));                            /* 00961C  move.w  $4(a4),d1 */
    SETW(d7, 1);                                         /* 009620  move.w  #$1,d7 */
    d3 = 0;                                              /* 009624  moveq  #0,d3 */
    d5 = 0;                                              /* 009626  moveq  #0,d5 */
    d4 = 0;                                              /* 009628  moveq  #0,d4 */
    SETB(d5, rd8(a4 + 0x36));                            /* 00962A  move.b  $36(a4),d5 */
    /* 00962E  cmp.b  $48(a4),d5 */
    /* 009632  beq  $9656 */
    if ((d5 & 0xFF) != rd8(a4 + 0x48)) {
L_009636:
        SETB(d5, rd8(a4 + 0x48));                            /* 009636  move.b  $48(a4),d5 */
        d5 = (d5 & 0xFFFF) * 0xB;                            /* 00963A  mulu.w  #$b,d5 */
        a0 = 0x9786;                                         /* 00963E  lea  $9786(pc),a0 */
        a0 += d5;                                            /* 009642  adda.l  d5,a0 */
        d5 = 0;                                              /* 009644  moveq  #0,d5 */
        SETB(d5, rd8(a4 + 0x36));                            /* 009646  move.b  $36(a4),d5 */
        SETB(d4, rd8(a0 + SX16(d5)));                        /* 00964A  move.b  $0(a0,d5.w),d4 */
        d5 = 0;                                              /* 00964E  moveq  #0,d5 */
        SETB(d5, (d4 & 0xFF));                               /* 009650  move.b  d4,d5 */
        /* 009652  bra  $965a */
    } else {
L_009656:
        SETB(d5, rd8(a4 + 0x36));                            /* 009656  move.b  $36(a4),d5 */
    }
L_00965A:
    d6 = 0;                                              /* 00965A  moveq  #0,d6 */
    SETW(d6, rd16(a4 + 0x4A));                           /* 00965C  move.w  $4a(a4),d6 */
    SETW(d5, d5 & 0x1F);                                 /* 009660  andi.w  #$1f,d5 */
    SETW(d5, (d5 & 0xFFFF) << 3);                        /* 009664  lsl.w  #3,d5 */
    a0 = 0x9696;                                         /* 009666  lea  $9696(pc),a0 */
    SETW(d3, rd16(a0 + SX16(d5)));                       /* 00966A  move.w  $0(a0,d5.w),d3 */
    /* 00966E  cmp.w  #$7e,d3 */
    /* 009672  beq  $9690 */
    if ((d3 & 0xFFFF) != 0x7E) {
L_009674:
        d3 = (uint32_t)((int32_t)(int16_t)d3 * (int32_t)(int16_t)(d6 & 0xFFFF)); /* 009674  muls.w  d6,d3 */
        SETW(d4, rd16(a0 + 0x2 + SX16(d5)));                 /* 009676  move.w  $2(a0,d5.w),d4 */
        d4 = (uint32_t)((int32_t)(int16_t)d4 * (int32_t)(int16_t)(d6 & 0xFFFF)); /* 00967A  muls.w  d6,d4 */
        SETW(d0, d0 + rd16(a0 + 0x4 + SX16(d5)));            /* 00967C  add.w  $4(a0,d5.w),d0 */
        /* 009680  add.w  $6(a0,d5.w),d1 */
        {
            uint32_t s = rd16(a0 + 0x6 + SX16(d5));
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
            fx = (r < s);
        }
        SETW(d5, rd16(V_027A));                              /* 009684  move.w  $27a(a5),d5 */
        SETW(d2, rd16(a4 + 0x4C));                           /* 009688  move.w  $4c(a4),d2 */
        CALL(sub_006B74, 0x9690U);                           /* 00968C  bsr  $6b74 */
    }
L_009690:
    /* 009690  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 009694  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9636U: goto L_009636;
        case 0x9656U: goto L_009656;
        case 0x965AU: goto L_00965A;
        case 0x9674U: goto L_009674;
        case 0x9690U: goto L_009690;
        }
    }
}

/* 009696  game routine */
void sub_009696(void)
{
    SETB(d0, d0 | 0xFC);                                 /* 009696  ori.b  #$fc,d0 */
    /* 00969A  ori.b  #$64,$0.w */
    {
        uint32_t s = 0x64;
        uint32_t d = rd8(0x0);
        uint32_t r = (d | s) & 0xFF;
        wr8(0x0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x96A0U);                                       /* 0096A0  soft */
}

/* 009786  game routine */
void sub_009786(void)
{
    SETB(d0, d0 | 0);                                    /* 009786  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00978A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00978E  ori.b  #$0,d0 */
    /* 009792  btst  d0,d2 */
    /* 009794  ori.b  #$6,d5 */
    {
        uint32_t s = 6;
        uint32_t d = (d5 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d5, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x9798U);
}

/* 009800  game routine */
void sub_009800(void)
{
    wr8(a0 + 0x2A, rd8(a0 + 0x2A) | 0x29);               /* 009800  ori.b  #$29,$2a(a0) */
    wr8(a3 + 0x2D, rd8(a3 + 0x2D) | 0x2C);               /* 009806  ori.b  #$2c,$2d(a3) */
    wr8(a6 + 0x30, rd8(a6 + 0x30) | 0x2F);               /* 00980C  ori.b  #$2f,$30(a6) */
    wr8(a1 + 0x33 + SX16(d0), rd8(a1 + 0x33 + SX16(d0)) | 0x32); /* 009812  ori.b  #$32,$33(a1,d0.w) */
    SETW(d0, d0 | 0x41);                                 /* 009818  ori.w  #$41,d0 */
    /* 00981C  ori.w  #$43,d2 */
    {
        uint32_t s = 0x43;
        uint32_t d = (d2 & 0xFFFF);
        uint32_t r = (d | s) & 0xFFFF;
        SETW(d2, r);
        FL_LOGIC(r, 0xFFFF, 0x8000);
    }
    /* 009820  ori  #$3d,ccr */
    set_ccr((get_ccr() | 0x3DU) & 0x1FU);
    MISS(0x9824U);
}

/* 009840  game routine */
void sub_009840(void)
{
    SETB(d0, d0 | 8);                                    /* 009840  ori.b  #$8,d0 */
    SETB(d0, d0 | 0x10);                                 /* 009844  ori.b  #$10,d0 */
    /* 009848  ori.b  #$20,(a0)+ */
    {
        uint32_t s = 0x20;
        uint32_t ea = a0;
        a0 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    SETB(d0, d0 | 0x28);                                 /* 00984C  ori.b  #$28,d0 */
    wr8(a0 + SX16(d0), rd8(a0 + SX16(d0)) | 0x38);       /* 009850  ori.b  #$38,$0(a0,d0.w) */
    SETB(d0, d0 | 0);                                    /* 009856  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00985A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0xEC);                                 /* 00985E  ori.b  #$ec,d0 */
    wr8(a4, rd8(a4) | 0xEC);                             /* 009862  ori.b  #$ec,(a4) */
    wr8(a5, rd8(a5) | 0xEC);                             /* 009866  ori.b  #$ec,(a5) */
    wr8(a7, rd8(a7) | 0xEC);                             /* 00986A  ori.b  #$ec,(a7) */
    /* 00986E  ori.b  #$6d,(a6) */
    {
        uint32_t s = 0x6D;
        uint32_t d = rd8(a6);
        uint32_t r = (d | s) & 0xFF;
        wr8(a6, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x9872U);
}

/* 009860  game routine */
void sub_009860(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr8(a4 + 0x14, 0xFF);                                /* 009860  st  $14(a4) */
    wr8(a4 + 0x15, 0xFF);                                /* 009864  st  $15(a4) */
    wr8(a4 + 0x17, 0xFF);                                /* 009868  st  $17(a4) */
    wr8(a4 + 0x16, 0xFF);                                /* 00986C  st  $16(a4) */
    /* 009870  tst.w  $2da(a5) */
    /* 009874  beq  $9888 */
    if (rd16(V_02DA) != 0) {
L_009876:
        SETW(d0, rd16(V_0284));                              /* 009876  move.w  $284(a5),d0 */
        SETW(d0, d0 - rd16(a4 + 0x2));                       /* 00987A  sub.w  $2(a4),d0 */
        SETW(d1, rd16(V_0288));                              /* 00987E  move.w  $288(a5),d1 */
        SETW(d1, d1 - rd16(a4 + 0x4));                       /* 009882  sub.w  $4(a4),d1 */
        CALL(sub_0098A2, 0x9888U);                           /* 009886  bsr  $98a2 */
    }
L_009888:
    /* 009888  tst.w  $376(a5) */
    /* 00988C  beq  $98a0 */
    if (rd16(V_0376) != 0) {
        SETW(d0, rd16(V_0286));                              /* 00988E  move.w  $286(a5),d0 */
        SETW(d0, d0 - rd16(a4 + 0x2));                       /* 009892  sub.w  $2(a4),d0 */
        SETW(d1, rd16(V_028A));                              /* 009896  move.w  $28a(a5),d1 */
        SETW(d1, d1 - rd16(a4 + 0x4));                       /* 00989A  sub.w  $4(a4),d1 */
        CALL(sub_0098A2, 0x98A0U);                           /* 00989E  bsr  $98a2 */
    }
L_0098A0:
    RTS();                                               /* 0098A0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9876U: goto L_009876;
        case 0x9888U: goto L_009888;
        case 0x98A0U: goto L_0098A0;
        }
    }
}

/* 0098A2  game routine */
void sub_0098A2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0098A2  tst.w  d0 */
    /* 0098A4  bne  $98aa */
    if ((d0 & 0xFFFF) == 0) {
L_0098A6:
        /* 0098A6  tst.w  d1 */
        /* 0098A8  beq  $98f2 */
        if ((d1 & 0xFFFF) == 0) {
            goto L_0098F2;
        }
    }
    /* 0098AA  cmp.w  #$e,d1 */
    /* 0098AE  bhi  $98ce */
    if ((d1 & 0xFFFF) <= 0xE) {
        /* 0098B0  cmp.w  #$e,d0 */
        /* 0098B4  bhi  $98c0 */
        if ((d0 & 0xFFFF) <= 0xE) {
            wr8(a4 + 0x14, 0);                                   /* 0098B6  sf  $14(a4) */
            wr8(a4 + 0x16, 0);                                   /* 0098BA  sf  $16(a4) */
            /* 0098BE  bra  $98f2 */
            goto L_0098F2;
        }
        /* 0098C0  cmp.w  #$fff2,d0 */
        /* 0098C4  bls  $98f2 */
        if ((d0 & 0xFFFF) <= 0xFFF2) {
            goto L_0098F2;
        }
        wr8(a4 + 0x15, 0);                                   /* 0098C6  sf  $15(a4) */
        wr8(a4 + 0x16, 0);                                   /* 0098CA  sf  $16(a4) */
    }
    /* 0098CE  cmp.w  #$fff2,d1 */
    /* 0098D2  bls  $98f2 */
    if ((d1 & 0xFFFF) > 0xFFF2) {
        /* 0098D4  cmp.w  #$e,d0 */
        /* 0098D8  bhi  $98e4 */
        if ((d0 & 0xFFFF) <= 0xE) {
            wr8(a4 + 0x14, 0);                                   /* 0098DA  sf  $14(a4) */
            wr8(a4 + 0x17, 0);                                   /* 0098DE  sf  $17(a4) */
            /* 0098E2  bra  $98f2 */
        } else {
            /* 0098E4  cmp.w  #$fff2,d0 */
            /* 0098E8  bls  $98f2 */
            if ((d0 & 0xFFFF) > 0xFFF2) {
                wr8(a4 + 0x15, 0);                                   /* 0098EA  sf  $15(a4) */
                wr8(a4 + 0x17, 0);                                   /* 0098EE  sf  $17(a4) */
            }
        }
    }
L_0098F2:
    SETW(d1, rd16(a4 + 0x4));                            /* 0098F2  move.w  $4(a4),d1 */
    SETW(d0, rd16(a4 + 0x2));                            /* 0098F6  move.w  $2(a4),d0 */
    SETW(d0, d0 - 0x40);                                 /* 0098FA  subi.w  #$40,d0 */
    SETW(d0, d0 + 5);                                    /* 0098FE  addq.w  #5,d0 */
    SETW(d1, d1 + 7);                                    /* 009900  addq.w  #7,d1 */
    SETW(d1, d1 + 3);                                    /* 009902  addq.w  #3,d1 */
    CALL(sub_00AF6A, 0x9908U);                           /* 009904  bsr  $af6a */
L_009908:
    SETW(d1, d1 - 3);                                    /* 009908  subq.w  #3,d1 */
    wr8(V_0220, (d2 & 0xFF));                            /* 00990A  move.b  d2,$220(a5) */
    SETW(d0, d0 + 0xB);                                  /* 00990E  addi.w  #$b,d0 */
    CALL(sub_00AF6A, 0x9916U);                           /* 009912  bsr  $af6a */
L_009916:
    wr8(V_0221, (d2 & 0xFF));                            /* 009916  move.b  d2,$221(a5) */
    SETW(d0, d0 + 0xB);                                  /* 00991A  addi.w  #$b,d0 */
    SETW(d1, d1 + 3);                                    /* 00991E  addq.w  #3,d1 */
    CALL(sub_00AF6A, 0x9924U);                           /* 009920  bsr  $af6a */
L_009924:
    SETW(d1, d1 - 3);                                    /* 009924  subq.w  #3,d1 */
    wr8(V_0222, (d2 & 0xFF));                            /* 009926  move.b  d2,$222(a5) */
    SETW(d0, d0 + 3);                                    /* 00992A  addq.w  #3,d0 */
    SETW(d1, d1 + 0xA);                                  /* 00992C  addi.w  #$a,d1 */
    CALL(sub_00AF6A, 0x9934U);                           /* 009930  bsr  $af6a */
L_009934:
    wr8(V_0223, (d2 & 0xFF));                            /* 009934  move.b  d2,$223(a5) */
    SETW(d0, d0 - 3);                                    /* 009938  subq.w  #3,d0 */
    SETW(d1, d1 + 0xA);                                  /* 00993A  addi.w  #$a,d1 */
    SETW(d1, d1 - 3);                                    /* 00993E  subq.w  #3,d1 */
    CALL(sub_00AF6A, 0x9944U);                           /* 009940  bsr  $af6a */
L_009944:
    SETW(d1, d1 + 3);                                    /* 009944  addq.w  #3,d1 */
    wr8(V_0224, (d2 & 0xFF));                            /* 009946  move.b  d2,$224(a5) */
    SETW(d0, d0 - 0xB);                                  /* 00994A  subi.w  #$b,d0 */
    CALL(sub_00AF6A, 0x9952U);                           /* 00994E  bsr  $af6a */
L_009952:
    wr8(V_0225, (d2 & 0xFF));                            /* 009952  move.b  d2,$225(a5) */
    SETW(d0, d0 - 0xB);                                  /* 009956  subi.w  #$b,d0 */
    SETW(d1, d1 - 3);                                    /* 00995A  subq.w  #3,d1 */
    CALL(sub_00AF6A, 0x9960U);                           /* 00995C  bsr  $af6a */
L_009960:
    SETW(d1, d1 + 3);                                    /* 009960  addq.w  #3,d1 */
    wr8(V_0226, (d2 & 0xFF));                            /* 009962  move.b  d2,$226(a5) */
    SETW(d0, d0 - 3);                                    /* 009966  subq.w  #3,d0 */
    SETW(d1, d1 - 0xA);                                  /* 009968  subi.w  #$a,d1 */
    CALL(sub_00AF6A, 0x9970U);                           /* 00996C  bsr  $af6a */
L_009970:
    wr8(V_0227, (d2 & 0xFF));                            /* 009970  move.b  d2,$227(a5) */
    wr16(V_0228, 0);                                     /* 009974  clr.w  $228(a5) */
    wr16(V_022A, 0);                                     /* 009978  clr.w  $22a(a5) */
    wr16(V_022C, 0);                                     /* 00997C  clr.w  $22c(a5) */
    wr16(V_022E, 0);                                     /* 009980  clr.w  $22e(a5) */
    /* 009984  tst.b  $221(a5) */
    /* 009988  bne  $99c2 */
    if (rd8(V_0221) == 0) {
L_00998A:
        /* 00998A  tst.b  $10(a4) */
        /* 00998E  beq  $99c6 */
        if (rd8(a4 + 0x10) == 0) {
            goto L_0099C6;
        }
L_009990:
        /* 009990  tst.b  $220(a5) */
        /* 009994  beq  $99a8 */
        if (rd8(V_0220) != 0) {
L_009996:
            /* 009996  tst.b  $227(a5) */
            /* 00999A  bne  $99a8 */
            if (rd8(V_0227) == 0) {
L_00999C:
                wr16(a4 + 0x2, rd16(a4 + 0x2) + 3);                  /* 00999C  addq.w  #3,$2(a4) */
                wr8(V_0228, 0xFF);                                   /* 0099A0  st  $228(a5) */
                wr8(a4 + 0x17, 0);                                   /* 0099A4  sf  $17(a4) */
            }
        }
L_0099A8:
        /* 0099A8  tst.b  $222(a5) */
        /* 0099AC  beq  $99c0 */
        if (rd8(V_0222) != 0) {
L_0099AE:
            /* 0099AE  tst.b  $223(a5) */
            /* 0099B2  bne  $99c0 */
            if (rd8(V_0223) == 0) {
L_0099B4:
                wr16(a4 + 0x2, rd16(a4 + 0x2) - 3);                  /* 0099B4  subq.w  #3,$2(a4) */
                wr8(V_022A, 0xFF);                                   /* 0099B8  st  $22a(a5) */
                wr8(a4 + 0x17, 0);                                   /* 0099BC  sf  $17(a4) */
            }
        }
L_0099C0:
        /* 0099C0  bra  $99c6 */
    } else {
L_0099C2:
        wr8(a4 + 0x17, 0);                                   /* 0099C2  sf  $17(a4) */
    }
L_0099C6:
    /* 0099C6  tst.b  $225(a5) */
    /* 0099CA  bne  $9a10 */
    if (rd8(V_0225) == 0) {
L_0099CC:
        /* 0099CC  tst.b  $11(a4) */
        /* 0099D0  beq  $9a14 */
        if (rd8(a4 + 0x11) == 0) {
            goto L_009A14;
        }
        /* 0099D2  tst.b  $226(a5) */
        /* 0099D6  beq  $99f0 */
        if (rd8(V_0226) != 0) {
            /* 0099D8  tst.b  $227(a5) */
            /* 0099DC  bne  $99f0 */
            if (rd8(V_0227) == 0) {
                /* 0099DE  tst.w  $228(a5) */
                /* 0099E2  bne  $99f0 */
                if (rd16(V_0228) == 0) {
                    wr16(a4 + 0x2, rd16(a4 + 0x2) + 3);                  /* 0099E4  addq.w  #3,$2(a4) */
                    wr8(V_0228, 0xFF);                                   /* 0099E8  st  $228(a5) */
                    wr8(a4 + 0x16, 0);                                   /* 0099EC  sf  $16(a4) */
                }
            }
        }
        /* 0099F0  tst.b  $224(a5) */
        /* 0099F4  beq  $9a0e */
        if (rd8(V_0224) != 0) {
            /* 0099F6  tst.b  $223(a5) */
            /* 0099FA  bne  $9a0e */
            if (rd8(V_0223) == 0) {
                /* 0099FC  tst.w  $22a(a5) */
                /* 009A00  bne  $9a0e */
                if (rd16(V_022A) == 0) {
                    wr16(a4 + 0x2, rd16(a4 + 0x2) - 3);                  /* 009A02  subq.w  #3,$2(a4) */
                    wr8(V_022A, 0xFF);                                   /* 009A06  st  $22a(a5) */
                    wr8(a4 + 0x16, 0);                                   /* 009A0A  sf  $16(a4) */
                }
            }
        }
        /* 009A0E  bra  $9a14 */
    } else {
        wr8(a4 + 0x16, 0);                                   /* 009A10  sf  $16(a4) */
    }
L_009A14:
    wr8(V_0142, 0xFF);                                   /* 009A14  st  $142(a5) */
    SETB(d0, rd8(V_022A));                               /* 009A18  move.b  $22a(a5),d0 */
    /* 009A1C  add.b  $228(a5),d0 */
    {
        uint32_t s = rd8(V_0228);
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d + s) & 0xFF;
        SETB(d0, r);
        fz = (r == 0);
    }
    /* 009A20  bne  $9ab8 */
    if (COND(7)) {
L_009A24:
        wr16(V_0142, 0);                                     /* 009A24  clr.w  $142(a5) */
        /* 009A28  tst.b  $223(a5) */
        /* 009A2C  bne  $9a66 */
        if (rd8(V_0223) == 0) {
L_009A2E:
            /* 009A2E  tst.b  $e(a4) */
            /* 009A32  beq  $9a6a */
            if (rd8(a4 + 0xE) == 0) {
                goto L_009A6A;
            }
L_009A34:
            /* 009A34  tst.b  $222(a5) */
            /* 009A38  beq  $9a4c */
            if (rd8(V_0222) != 0) {
                /* 009A3A  tst.b  $221(a5) */
                /* 009A3E  bne  $9a4c */
                if (rd8(V_0221) == 0) {
                    wr16(a4 + 0x4, rd16(a4 + 0x4) + 3);                  /* 009A40  addq.w  #3,$4(a4) */
                    wr8(V_022C, 0xFF);                                   /* 009A44  st  $22c(a5) */
                    wr8(a4 + 0x14, 0);                                   /* 009A48  sf  $14(a4) */
                }
            }
L_009A4C:
            /* 009A4C  tst.b  $224(a5) */
            /* 009A50  beq  $9a64 */
            if (rd8(V_0224) != 0) {
                /* 009A52  tst.b  $225(a5) */
                /* 009A56  bne  $9a64 */
                if (rd8(V_0225) == 0) {
                    wr16(a4 + 0x4, rd16(a4 + 0x4) - 3);                  /* 009A58  subq.w  #3,$4(a4) */
                    wr8(V_022E, 0xFF);                                   /* 009A5C  st  $22e(a5) */
                    wr8(a4 + 0x14, 0);                                   /* 009A60  sf  $14(a4) */
                }
            }
L_009A64:
            /* 009A64  bra  $9a6a */
        } else {
L_009A66:
            wr8(a4 + 0x14, 0);                                   /* 009A66  sf  $14(a4) */
        }
L_009A6A:
        /* 009A6A  tst.b  $227(a5) */
        /* 009A6E  bne  $9ab4 */
        if (rd8(V_0227) == 0) {
L_009A70:
            /* 009A70  tst.b  $f(a4) */
            /* 009A74  beq  $9ab8 */
            if (rd8(a4 + 0xF) == 0) {
                goto L_009AB8;
            }
L_009A76:
            /* 009A76  tst.b  $220(a5) */
            /* 009A7A  beq  $9a94 */
            if (rd8(V_0220) != 0) {
                /* 009A7C  tst.b  $221(a5) */
                /* 009A80  bne  $9a94 */
                if (rd8(V_0221) == 0) {
                    /* 009A82  tst.w  $22c(a5) */
                    /* 009A86  bne  $9ab2 */
                    if (rd16(V_022C) != 0) {
                        goto L_009AB2;
                    }
                    wr16(a4 + 0x4, rd16(a4 + 0x4) + 3);                  /* 009A88  addq.w  #3,$4(a4) */
                    wr8(V_022C, 0xFF);                                   /* 009A8C  st  $22c(a5) */
                    wr8(a4 + 0x15, 0);                                   /* 009A90  sf  $15(a4) */
                }
            }
L_009A94:
            /* 009A94  tst.b  $226(a5) */
            /* 009A98  beq  $9ab2 */
            if (rd8(V_0226) != 0) {
                /* 009A9A  tst.b  $225(a5) */
                /* 009A9E  bne  $9ab2 */
                if (rd8(V_0225) == 0) {
                    /* 009AA0  tst.w  $22e(a5) */
                    /* 009AA4  bne  $9ab2 */
                    if (rd16(V_022E) == 0) {
                        wr16(a4 + 0x4, rd16(a4 + 0x4) - 3);                  /* 009AA6  subq.w  #3,$4(a4) */
                        wr8(V_022E, 0xFF);                                   /* 009AAA  st  $22e(a5) */
                        wr8(a4 + 0x15, 0);                                   /* 009AAE  sf  $15(a4) */
                    }
                }
            }
L_009AB2:
            /* 009AB2  bra  $9ab8 */
        } else {
L_009AB4:
            wr8(a4 + 0x15, 0);                                   /* 009AB4  sf  $15(a4) */
        }
    }
L_009AB8:
    wr8(V_0216, rd8(a4 + 0x17));                         /* 009AB8  move.b  $17(a4),$216(a5) */
    wr8(V_0217, rd8(a4 + 0x16));                         /* 009ABE  move.b  $16(a4),$217(a5) */
    wr8(V_0218, rd8(a4 + 0x14));                         /* 009AC4  move.b  $14(a4),$218(a5) */
    wr8(V_0219, rd8(a4 + 0x15));                         /* 009ACA  move.b  $15(a4),$219(a5) */
    wr8(a4 + 0x17, 0xFF);                                /* 009AD0  st  $17(a4) */
    wr8(a4 + 0x16, 0xFF);                                /* 009AD4  st  $16(a4) */
    wr8(a4 + 0x14, 0xFF);                                /* 009AD8  st  $14(a4) */
    wr8(a4 + 0x15, 0xFF);                                /* 009ADC  st  $15(a4) */
    SETW(d0, rd16(a4 + 0x2));                            /* 009AE0  move.w  $2(a4),d0 */
    SETW(d1, rd16(a4 + 0x4));                            /* 009AE4  move.w  $4(a4),d1 */
    SETW(d1, d1 + 0xE);                                  /* 009AE8  addi.w  #$e,d1 */
    /* 009AEC  tst.w  $248(a5) */
    /* 009AF0  bne  $9afa */
    if (rd16(V_0248) == 0) {
L_009AF2:
        SETW(d0, d0 + 0x24);                                 /* 009AF2  addi.w  #$24,d0 */
        SETW(d1, d1 + 0x64);                                 /* 009AF6  addi.w  #$64,d1 */
    }
L_009AFA:
    /* 009AFA  tst.w  $2a(a4) */
    /* 009AFE  bne  $9b40 */
    if (rd16(a4 + 0x2A) == 0) {
L_009B00:
        /* 009B00  tst.w  $50(a4) */
        /* 009B04  bne  $9b40 */
        if (rd16(a4 + 0x50) != 0) {
            goto L_009B40;
        }
L_009B06:
        /* 009B06  addi.w  #$c,d0 */
        {
            uint32_t s = 0xC;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        /* 009B0A  tst.w  $248(a5) */
        /* 009B0E  bne  $9b40 */
        if (rd16(V_0248) != 0) {
            goto L_009B40;
        }
L_009B10:
        CALL(sub_009BEA, 0x9B14U);                           /* 009B10  bsr  $9bea */
L_009B14:
        /* 009B14  tst.w  d7 */
        /* 009B16  beq  $9b40 */
        if ((d7 & 0xFFFF) == 0) {
            goto L_009B40;
        }
        /* 009B18  move.l  a0,-(a7) */
        {
            uint32_t v = a0;
            a7 -= 4;
            wr32(a7, v);
        }
        a0 = rd32(V_01FA);                                   /* 009B1A  movea.l  $1fa(a5),a0 */
        SETW(d3, rd16(a0 + 0xC));                            /* 009B1E  move.w  $c(a0),d3 */
        SETW(d2, rd16(a0 + 0x6));                            /* 009B22  move.w  $6(a0),d2 */
        /* 009B26  movea.l  (a7)+,a0 */
        a0 = rd32(a7);
        a7 += 4;
        /* 009B28  tst.w  d3 */
        /* 009B2A  bne  $9b40 */
        if ((d3 & 0xFFFF) != 0) {
            goto L_009B40;
        }
        /* 009B2C  tst.w  d2 */
        /* 009B2E  beq  $9b40 */
        if ((d2 & 0xFFFF) == 0) {
            goto L_009B40;
        }
        /* 009B30  tst.w  $254(a5) */
        /* 009B34  bne  $9be6 */
        if (rd16(V_0254) != 0) {
            goto L_009BE6;
        }
        CALL(player_start_death, 0x9B3CU);                   /* 009B38  bsr  $aa42 */
L_009B3C:
        /* 009B3C  bra  $9be6 */
    } else {
L_009B40:
        SETW(d0, d0 - 0xC);                                  /* 009B40  subi.w  #$c,d0 */
        SETW(d0, d0 + 4);                                    /* 009B44  addq.w  #4,d0 */
        CALL(sub_009BEA, 0x9B4AU);                           /* 009B46  bsr  $9bea */
L_009B4A:
        /* 009B4A  tst.w  d7 */
        /* 009B4C  beq  $9b56 */
        if ((d7 & 0xFFFF) != 0) {
            wr8(a4 + 0x17, 0);                                   /* 009B4E  sf  $17(a4) */
            wr8(a4 + 0x15, 0);                                   /* 009B52  sf  $15(a4) */
        }
L_009B56:
        SETW(d0, d0 + 0x14);                                 /* 009B56  addi.w  #$14,d0 */
        CALL(sub_009BEA, 0x9B5EU);                           /* 009B5A  bsr  $9bea */
L_009B5E:
        /* 009B5E  tst.w  d7 */
        /* 009B60  beq  $9b6a */
        if ((d7 & 0xFFFF) != 0) {
            wr8(a4 + 0x17, 0);                                   /* 009B62  sf  $17(a4) */
            wr8(a4 + 0x14, 0);                                   /* 009B66  sf  $14(a4) */
        }
L_009B6A:
        SETW(d1, d1 + 0xC);                                  /* 009B6A  addi.w  #$c,d1 */
        CALL(sub_009BEA, 0x9B72U);                           /* 009B6E  bsr  $9bea */
L_009B72:
        /* 009B72  tst.w  d7 */
        /* 009B74  beq  $9b7e */
        if ((d7 & 0xFFFF) != 0) {
            wr8(a4 + 0x16, 0);                                   /* 009B76  sf  $16(a4) */
            wr8(a4 + 0x14, 0);                                   /* 009B7A  sf  $14(a4) */
        }
L_009B7E:
        SETW(d0, d0 - 0x10);                                 /* 009B7E  subi.w  #$10,d0 */
        CALL(sub_009BEA, 0x9B86U);                           /* 009B82  bsr  $9bea */
L_009B86:
        /* 009B86  tst.w  d7 */
        /* 009B88  beq  $9b92 */
        if ((d7 & 0xFFFF) != 0) {
            wr8(a4 + 0x16, 0);                                   /* 009B8A  sf  $16(a4) */
            wr8(a4 + 0x15, 0);                                   /* 009B8E  sf  $15(a4) */
        }
L_009B92:
        /* 009B92  tst.w  $248(a5) */
        /* 009B96  bne  $9bbc */
        if (rd16(V_0248) == 0) {
L_009B98:
            SETB(d7, rd8(a4 + 0x16));                            /* 009B98  move.b  $16(a4),d7 */
            /* 009B9C  add.b  $17(a4),d7 */
            {
                uint32_t s = rd8(a4 + 0x17);
                uint32_t d = (d7 & 0xFF);
                uint32_t r = (d + s) & 0xFF;
                SETB(d7, r);
                fz = (r == 0);
            }
            /* 009BA0  bne  $9baa */
            if (COND(7)) {
                wr8(a4 + 0x16, 0xFF);                                /* 009BA2  st  $16(a4) */
                wr8(a4 + 0x17, 0xFF);                                /* 009BA6  st  $17(a4) */
            }
L_009BAA:
            SETB(d7, rd8(a4 + 0x15));                            /* 009BAA  move.b  $15(a4),d7 */
            /* 009BAE  add.b  $14(a4),d7 */
            {
                uint32_t s = rd8(a4 + 0x14);
                uint32_t d = (d7 & 0xFF);
                uint32_t r = (d + s) & 0xFF;
                SETB(d7, r);
                fz = (r == 0);
            }
            /* 009BB2  bne  $9bbc */
            if (COND(7)) {
                wr8(a4 + 0x15, 0xFF);                                /* 009BB4  st  $15(a4) */
                wr8(a4 + 0x14, 0xFF);                                /* 009BB8  st  $14(a4) */
            }
        }
L_009BBC:
        /* 009BBC  tst.b  $216(a5) */
        /* 009BC0  bne  $9bc6 */
        if (rd8(V_0216) == 0) {
L_009BC2:
            wr8(a4 + 0x17, 0);                                   /* 009BC2  sf  $17(a4) */
        }
L_009BC6:
        /* 009BC6  tst.b  $217(a5) */
        /* 009BCA  bne  $9bd0 */
        if (rd8(V_0217) == 0) {
            wr8(a4 + 0x16, 0);                                   /* 009BCC  sf  $16(a4) */
        }
L_009BD0:
        /* 009BD0  tst.b  $219(a5) */
        /* 009BD4  bne  $9bda */
        if (rd8(V_0219) == 0) {
L_009BD6:
            wr8(a4 + 0x15, 0);                                   /* 009BD6  sf  $15(a4) */
        }
L_009BDA:
        /* 009BDA  tst.b  $218(a5) */
        /* 009BDE  bne  $9be4 */
        if (rd8(V_0218) == 0) {
L_009BE0:
            wr8(a4 + 0x14, 0);                                   /* 009BE0  sf  $14(a4) */
        }
L_009BE4:
        RTS();                                               /* 009BE4  rts */
    }
L_009BE6:
    /* 009BE6  move.l  (a7)+,d7 */
    d7 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 009BE8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x98A6U: goto L_0098A6;
        case 0x98F2U: goto L_0098F2;
        case 0x9908U: goto L_009908;
        case 0x9916U: goto L_009916;
        case 0x9924U: goto L_009924;
        case 0x9934U: goto L_009934;
        case 0x9944U: goto L_009944;
        case 0x9952U: goto L_009952;
        case 0x9960U: goto L_009960;
        case 0x9970U: goto L_009970;
        case 0x998AU: goto L_00998A;
        case 0x9990U: goto L_009990;
        case 0x9996U: goto L_009996;
        case 0x999CU: goto L_00999C;
        case 0x99A8U: goto L_0099A8;
        case 0x99AEU: goto L_0099AE;
        case 0x99B4U: goto L_0099B4;
        case 0x99C0U: goto L_0099C0;
        case 0x99C2U: goto L_0099C2;
        case 0x99C6U: goto L_0099C6;
        case 0x99CCU: goto L_0099CC;
        case 0x9A14U: goto L_009A14;
        case 0x9A24U: goto L_009A24;
        case 0x9A2EU: goto L_009A2E;
        case 0x9A34U: goto L_009A34;
        case 0x9A4CU: goto L_009A4C;
        case 0x9A64U: goto L_009A64;
        case 0x9A66U: goto L_009A66;
        case 0x9A6AU: goto L_009A6A;
        case 0x9A70U: goto L_009A70;
        case 0x9A76U: goto L_009A76;
        case 0x9A94U: goto L_009A94;
        case 0x9AB2U: goto L_009AB2;
        case 0x9AB4U: goto L_009AB4;
        case 0x9AB8U: goto L_009AB8;
        case 0x9AF2U: goto L_009AF2;
        case 0x9AFAU: goto L_009AFA;
        case 0x9B00U: goto L_009B00;
        case 0x9B06U: goto L_009B06;
        case 0x9B10U: goto L_009B10;
        case 0x9B14U: goto L_009B14;
        case 0x9B3CU: goto L_009B3C;
        case 0x9B40U: goto L_009B40;
        case 0x9B4AU: goto L_009B4A;
        case 0x9B56U: goto L_009B56;
        case 0x9B5EU: goto L_009B5E;
        case 0x9B6AU: goto L_009B6A;
        case 0x9B72U: goto L_009B72;
        case 0x9B7EU: goto L_009B7E;
        case 0x9B86U: goto L_009B86;
        case 0x9B92U: goto L_009B92;
        case 0x9B98U: goto L_009B98;
        case 0x9BAAU: goto L_009BAA;
        case 0x9BBCU: goto L_009BBC;
        case 0x9BC2U: goto L_009BC2;
        case 0x9BC6U: goto L_009BC6;
        case 0x9BD0U: goto L_009BD0;
        case 0x9BD6U: goto L_009BD6;
        case 0x9BDAU: goto L_009BDA;
        case 0x9BE0U: goto L_009BE0;
        case 0x9BE4U: goto L_009BE4;
        }
    }
}

/* 009BEA  game routine */
void sub_009BEA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_009BEA;
    do {
        BUDGET_CHECK(0x4EA4U);
L_004EA4:
        /* 004EA4  movem.l  a0/d2/d1/d0,-(a7) */
        PUSH32(a0);
        PUSH32(d2);
        PUSH32(d1);
        PUSH32(d0);
        a0 = V_1B5C;                                         /* 004EA8  lea  $1b5c(a5),a0 */
        d7 = 0xF;                                            /* 004EAC  moveq  #15,d7 */
        do {
            BUDGET_CHECK(0x4EAEU);
L_004EAE:
            /* 004EAE  tst.w  (a0) */
            /* 004EB0  beq  $4ef8 */
            if (rd16(a0) != 0) {
L_004EB2:
                SETW(d2, (d0 & 0xFFFF));                             /* 004EB2  move.w  d0,d2 */
                SETW(d2, d2 - rd16(a0 + 0x2));                       /* 004EB4  sub.w  $2(a0),d2 */
                /* 004EB8  cmp.w  #$10,d2 */
                /* 004EBC  bhi  $4ef8 */
                if ((d2 & 0xFFFF) <= 0x10) {
L_004EBE:
                    SETW(d2, (d1 & 0xFFFF));                             /* 004EBE  move.w  d1,d2 */
                    SETW(d2, d2 - rd16(a0 + 0x4));                       /* 004EC0  sub.w  $4(a0),d2 */
                    /* 004EC4  cmp.w  #$10,d2 */
                    /* 004EC8  bhi  $4ef8 */
                    if ((d2 & 0xFFFF) <= 0x10) {
                        /* 004ECA  cmpi.w  #$f1,$6(a0) */
                        /* 004ED0  bhi  $4ef6 */
                        if (rd16(a0 + 0x6) <= 0xF1) {
                            SETW(d0, rd16(a0 + 0x2));                            /* 004ED2  move.w  $2(a0),d0 */
                            SETW(d1, rd16(a0 + 0x4));                            /* 004ED6  move.w  $4(a0),d1 */
                            SETW(d0, d0 + 0x1C);                                 /* 004EDA  addi.w  #$1c,d0 */
                            /* 004EDE  addi.w  #$58,d1 */
                            {
                                uint32_t s = 0x58;
                                uint32_t d = (d1 & 0xFFFF);
                                uint32_t r = (d + s) & 0xFFFF;
                                SETW(d1, r);
                                fx = (r < s);
                            }
                            d2 = 1;                                              /* 004EE2  moveq  #1,d2 */
                            wr16(a0, 0);                                         /* 004EE4  clr.w  (a0) */
                            CALL(sub_0063A4, 0x4EEAU);                           /* 004EE6  bsr  $63a4 */
L_004EEA:
                            /* 004EEA  tst.w  $2a(a4) */
                            /* 004EEE  bne  $4ef6 */
                            if (rd16(a4 + 0x2A) == 0) {
                                CALL(player_start_death, 0x4EF4U);                   /* 004EF0  bsr  $aa42 */
L_004EF4:
                                /* 004EF4  bra  $4f00 */
                                goto L_004F00;
                            }
                        }
                        /* 004EF6  nop */
                    }
                }
            }
L_004EF8:
            a0 = a0 + 0xE;                                       /* 004EF8  lea  $e(a0),a0 */
            /* 004EFC  dbra  d7,$4eae */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_004F00:
        a0 = V_1C3C;                                         /* 004F00  lea  $1c3c(a5),a0 */
        d7 = 5;                                              /* 004F04  moveq  #5,d7 */
        do {
            BUDGET_CHECK(0x4F06U);
L_004F06:
            /* 004F06  tst.w  (a0) */
            /* 004F08  beq  $4f22 */
            if (rd16(a0) != 0) {
                SETW(d2, (d0 & 0xFFFF));                             /* 004F0A  move.w  d0,d2 */
                SETW(d2, d2 - rd16(a0 + 0x2));                       /* 004F0C  sub.w  $2(a0),d2 */
                /* 004F10  cmp.w  #$20,d2 */
                /* 004F14  bhi  $4f22 */
                if ((d2 & 0xFFFF) <= 0x20) {
                    SETW(d2, (d1 & 0xFFFF));                             /* 004F16  move.w  d1,d2 */
                    SETW(d2, d2 - rd16(a0 + 0x4));                       /* 004F18  sub.w  $4(a0),d2 */
                    /* 004F1C  cmp.w  #$20,d2 */
                    /* 004F20  bls  $4f2a */
                    if ((d2 & 0xFFFF) <= 0x20) {
                        goto L_004F2A;
                    }
                }
            }
L_004F22:
            a0 = a0 + 0xC;                                       /* 004F22  lea  $c(a0),a0 */
            /* 004F26  dbra  d7,$4f06 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_004F2A:
        /* 004F2A  addq.w  #1,d7 */
        {
            uint32_t s = 1;
            uint32_t d = (d7 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d7, r);
            fx = (r < s);
        }
        /* 004F2C  movem.l  (a7)+,d0-d2/a0 */
        d0 = POP32();
        d1 = POP32();
        d2 = POP32();
        a0 = POP32();
        RTS();                                               /* 004F30  rts */
L_009BEA:
        /* 009BEA  tst.w  $248(a5) */
        /* 009BEE  bne  $4ea4 */
    } while (rd16(V_0248) != 0);
L_009BF2:
    /* 009BF2  movem.l  a0/d2/d1/d0,-(a7) */
    PUSH32(a0);
    PUSH32(d2);
    PUSH32(d1);
    PUSH32(d0);
    a0 = V_VEHICLES;                                     /* 009BF6  lea  $1a76(a5),a0 */
    d7 = 4;                                              /* 009BFA  moveq  #4,d7 */
    do {
        BUDGET_CHECK(0x9BFCU);
L_009BFC:
        /* 009BFC  tst.w  (a0) */
        /* 009BFE  beq  $9c18 */
        if (rd16(a0) != 0) {
L_009C00:
            SETW(d2, (d0 & 0xFFFF));                             /* 009C00  move.w  d0,d2 */
            SETW(d2, d2 - rd16(a0 + 0x2));                       /* 009C02  sub.w  $2(a0),d2 */
            /* 009C06  cmp.w  $e(a0),d2 */
            /* 009C0A  bhi  $9c18 */
            if ((d2 & 0xFFFF) <= rd16(a0 + 0xE)) {
L_009C0C:
                SETW(d2, (d1 & 0xFFFF));                             /* 009C0C  move.w  d1,d2 */
                SETW(d2, d2 - rd16(a0 + 0x4));                       /* 009C0E  sub.w  $4(a0),d2 */
                /* 009C12  cmp.w  $a(a0),d2 */
                /* 009C16  bls  $9c20 */
                if ((d2 & 0xFFFF) <= rd16(a0 + 0xA)) {
                    goto L_009C20;
                }
            }
        }
L_009C18:
        a0 = a0 + 0x2E;                                      /* 009C18  lea  $2e(a0),a0 */
        /* 009C1C  dbra  d7,$9bfc */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_009C20:
    /* 009C20  addq.w  #1,d7 */
    {
        uint32_t s = 1;
        uint32_t d = (d7 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d7, r);
        fx = (r < s);
    }
    wr32(V_01FA, a0);                                    /* 009C22  move.l  a0,$1fa(a5) */
    /* 009C26  movem.l  (a7)+,d0-d2/a0 */
    d0 = POP32();
    d1 = POP32();
    d2 = POP32();
    a0 = POP32();
    RTS();                                               /* 009C2A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4EA4U: goto L_004EA4;
        case 0x4EAEU: goto L_004EAE;
        case 0x4EB2U: goto L_004EB2;
        case 0x4EBEU: goto L_004EBE;
        case 0x4EEAU: goto L_004EEA;
        case 0x4EF4U: goto L_004EF4;
        case 0x4EF8U: goto L_004EF8;
        case 0x4F00U: goto L_004F00;
        case 0x4F06U: goto L_004F06;
        case 0x4F22U: goto L_004F22;
        case 0x4F2AU: goto L_004F2A;
        case 0x9BF2U: goto L_009BF2;
        case 0x9BFCU: goto L_009BFC;
        case 0x9C00U: goto L_009C00;
        case 0x9C0CU: goto L_009C0C;
        case 0x9C18U: goto L_009C18;
        case 0x9C20U: goto L_009C20;
        }
    }
}

/* 009C2C  game routine */
void sub_009C2C(void)
{
    SOFT(0x9C2CU);                                       /* 009C2C  soft */
}

/* 009CAC  game routine */
void sub_009CAC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d7 = 0;                                              /* 009CAC  moveq  #0,d7 */
    /* 009CAE  tst.w  $28c(a5) */
    /* 009CB2  beq  $9cdc */
    if (rd16(V_028C) != 0) {
L_009CB4:
        /* 009CB4  tst.b  $30(a4) */
        /* 009CB8  beq  $9cbe */
        if (rd8(a4 + 0x30) != 0) {
L_009CBA:
            d7 |= 0x8;                                           /* 009CBA  bset  #$3,d7 */
        }
L_009CBE:
        /* 009CBE  tst.b  $2e(a4) */
        /* 009CC2  beq  $9cc8 */
        if (rd8(a4 + 0x2E) != 0) {
L_009CC4:
            d7 |= 0x4;                                           /* 009CC4  bset  #$2,d7 */
        }
L_009CC8:
        /* 009CC8  tst.b  $32(a4) */
        /* 009CCC  beq  $9cd2 */
        if (rd8(a4 + 0x32) != 0) {
L_009CCE:
            d7 |= 0x1;                                           /* 009CCE  bset  #$0,d7 */
        }
L_009CD2:
        /* 009CD2  tst.b  $34(a4) */
        /* 009CD6  beq  $9cdc */
        if (rd8(a4 + 0x34) != 0) {
            d7 |= 0x2;                                           /* 009CD8  bset  #$1,d7 */
        }
    }
L_009CDC:
    SETB(d6, rd8(a4 + 0x36));                            /* 009CDC  move.b  $36(a4),d6 */
    wr8(a4 + 0x48, (d6 & 0xFF));                         /* 009CE0  move.b  d6,$48(a4) */
    d7 = host_aim(a4, d7);                               /* twin-stick: the right stick sets the facing / fire direction ($36) whatever the movement is */
    /* 009CE4  andi.w  #$f,d7 */
    {
        uint32_t s = 0xF;
        uint32_t d = (d7 & 0xFFFF);
        uint32_t r = (d & s) & 0xFFFF;
        SETW(d7, r);
        fz = (r == 0);
    }
    /* 009CE8  beq  $9d30 */
    if (COND(7)) {
        goto L_009D30;
    }
L_009CEA:
    wr8(a4 + 0x36, (d7 & 0xFF));                         /* 009CEA  move.b  d7,$36(a4) */
    wr16(a4 + 0x98, 0);                                  /* 009CEE  clr.w  $98(a4) */
    /* 009CF2  cmp.b  d7,d6 */
    /* 009CF4  beq  $9d26 */
    if ((d6 & 0xFF) != (d7 & 0xFF)) {
L_009CF8:
        wr16(a4 + 0x46, 0);                                  /* 009CF8  clr.w  $46(a4) */
        SETW(d7, d7 - 1);                                    /* 009CFC  subq.w  #1,d7 */
        a0 = 0x9800;                                         /* 009CFE  lea  $9800(pc),a0 */
        a1 = 0x9840;                                         /* 009D02  lea  $9840(pc),a1 */
        SETW(d7, (d7 & 0xFFFF) << 1);                        /* 009D06  lsl.w  #1,d7 */
        SETW(d7, rd16(a1 + SX16(d7)));                       /* 009D08  move.w  $0(a1,d7.w),d7 */
        a0 += SX16((d7 & 0xFFFF));                           /* 009D0C  adda.w  d7,a0 */
        wr32(a4 + 0x38, a0);                                 /* 009D0E  move.l  a0,$38(a4) */
        wr32(a4 + 0x3C, a0);                                 /* 009D12  move.l  a0,$3c(a4) */
        a0 += 8;                                             /* 009D16  adda.l  #$8,a0 */
        wr32(a4 + 0x40, a0);                                 /* 009D1C  move.l  a0,$40(a4) */
        wr16(a4 + 0x44, 2);                                  /* 009D20  move.w  #$2,$44(a4) */
    }
L_009D26:
    /* 009D26  tst.w  $46(a4) */
    /* 009D2A  beq  $9d32 */
    if (rd16(a4 + 0x46) == 0) {
        goto L_009D32;
    }
L_009D2C:
    wr16(a4 + 0x46, rd16(a4 + 0x46) - 1);                /* 009D2C  subq.w  #1,$46(a4) */
L_009D30:
    /* 009D30  bra  $9d64 */
    goto L_009D64;
L_009D32:
    wr16(a4 + 0x46, 1);                                  /* 009D32  move.w  #$1,$46(a4) */
    a0 = rd32(a4 + 0x3C);                                /* 009D38  movea.l  $3c(a4),a0 */
    a1 = rd32(a4 + 0x40);                                /* 009D3C  movea.l  $40(a4),a1 */
    /* 009D40  cmpa.l  a0,a1 */
    /* 009D42  beq  $9d66 */
    if (a1 == a0) {
        goto L_009D66;
    }
L_009D46:
    SETW(d0, rd16(a0));                                  /* 009D46  move.w  (a0),d0 */
    /* 009D48  tst.w  $27a(a5) */
    /* 009D4C  beq  $9d52 */
    if (rd16(V_027A) != 0) {
        SETW(d0, d0 - 0x28);                                 /* 009D4E  subi.w  #$28,d0 */
    }
L_009D52:
    wr16(a4 + 0x6, (d0 & 0xFFFF));                       /* 009D52  move.w  d0,$6(a4) */
    /* 009D56  tst.w  $54(a4) */
    /* 009D5A  beq  $9d64 */
    if (rd16(a4 + 0x54) != 0) {
L_009D5C:
        a0 += SX16(rd16(a4 + 0x44));                         /* 009D5C  adda.w  $44(a4),a0 */
        wr32(a4 + 0x3C, a0);                                 /* 009D60  move.l  a0,$3c(a4) */
    }
L_009D64:
    RTS();                                               /* 009D64  rts */
L_009D66:
    a0 = rd32(a4 + 0x38);                                /* 009D66  movea.l  $38(a4),a0 */
    wr32(a4 + 0x3C, a0);                                 /* 009D6A  move.l  a0,$3c(a4) */
    /* 009D6E  bra  $9d26 */
    BUDGET_CHECK(0x9D26U);
    goto L_009D26;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9CB4U: goto L_009CB4;
        case 0x9CBAU: goto L_009CBA;
        case 0x9CBEU: goto L_009CBE;
        case 0x9CC4U: goto L_009CC4;
        case 0x9CC8U: goto L_009CC8;
        case 0x9CCEU: goto L_009CCE;
        case 0x9CD2U: goto L_009CD2;
        case 0x9CDCU: goto L_009CDC;
        case 0x9CEAU: goto L_009CEA;
        case 0x9CF8U: goto L_009CF8;
        case 0x9D26U: goto L_009D26;
        case 0x9D2CU: goto L_009D2C;
        case 0x9D30U: goto L_009D30;
        case 0x9D32U: goto L_009D32;
        case 0x9D46U: goto L_009D46;
        case 0x9D52U: goto L_009D52;
        case 0x9D5CU: goto L_009D5C;
        case 0x9D64U: goto L_009D64;
        case 0x9D66U: goto L_009D66;
        }
    }
}

/* 009D70  game routine */
void sub_009D70(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_027A, 0);                                     /* 009D70  clr.w  $27a(a5) */
    a4 = V_02DA;                                         /* 009D74  lea  $2da(a5),a4 */
    a0 = a4 + 0x66;                                      /* 009D78  lea  $66(a4),a0 */
    d0 = 0xFFFFFFFF;                                     /* 009D7C  moveq  #-1,d0 */
    do {
        BUDGET_CHECK(0x9D7EU);
L_009D7E:
        /* 009D7E  addq.w  #1,d0 */
        {
            uint32_t s = 1;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        /* 009D80  tst.b  (a0)+ */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 009D82  beq  $9d7e */
    } while (COND(7));
L_009D84:
    wr16(a4 + 0xC, (d0 & 0xFFFF));                       /* 009D84  move.w  d0,$c(a4) */
    CALL(sub_009DB8, 0x9D8CU);                           /* 009D88  bsr  $9db8 */
L_009D8C:
    /* 009D8C  tst.w  $1b4(a5) */
    /* 009D90  beq  $9db2 */
    if (rd16(V_01B4) != 0) {
        wr16(V_027A, 1);                                     /* 009D94  move.w  #$1,$27a(a5) */
        a4 = V_0376;                                         /* 009D9A  lea  $376(a5),a4 */
        a0 = a4 + 0x66;                                      /* 009D9E  lea  $66(a4),a0 */
        d0 = 0xFFFFFFFF;                                     /* 009DA2  moveq  #-1,d0 */
        do {
            BUDGET_CHECK(0x9DA4U);
L_009DA4:
            /* 009DA4  addq.w  #1,d0 */
            {
                uint32_t s = 1;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d0, r);
                fx = (r < s);
            }
            /* 009DA6  tst.b  (a0)+ */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                FL_LOGIC(v, 0xFF, 0x80);
            }
            /* 009DA8  beq  $9da4 */
        } while (COND(7));
        wr16(a4 + 0xC, (d0 & 0xFFFF));                       /* 009DAA  move.w  d0,$c(a4) */
        CALL(sub_009DB8, 0x9DB2U);                           /* 009DAE  bsr  $9db8 */
    }
L_009DB2:
    wr16(V_027A, 0);                                     /* 009DB2  clr.w  $27a(a5) */
    RTS();                                               /* 009DB6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9D7EU: goto L_009D7E;
        case 0x9D84U: goto L_009D84;
        case 0x9D8CU: goto L_009D8C;
        case 0x9DA4U: goto L_009DA4;
        case 0x9DB2U: goto L_009DB2;
        }
    }
}

/* 009DB8  game routine */
void sub_009DB8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 009DB8  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    /* 009DBC  tst.w  $27a(a5) */
    /* 009DC0  beq  $9dcc */
    if (rd16(V_027A) != 0) {
        SETW(d1, 0x29D0);                                    /* 009DC4  move.w  #$29d0,d1 */
        /* 009DC8  bra  $9dd0 */
    } else {
L_009DCC:
        SETW(d1, 0x2948);                                    /* 009DCC  move.w  #$2948,d1 */
    }
    d0 = 0;                                              /* 009DD0  moveq  #0,d0 */
    SETW(d0, rd16(a4 + 0xC));                            /* 009DD2  move.w  $c(a4),d0 */
    d0 = (d0 & 0xFFFF) * 0x1D8;                          /* 009DD6  mulu.w  #$1d8,d0 */
    a0 = 0x39434;                                        /* 009DDA  lea  $39434.l,a0 */
    a0 += d0;                                            /* 009DE0  adda.l  d0,a0 */
    a1 = 0x20000;                                        /* 009DE2  lea  $20000.l,a1 */
    a2 = 0x18000;                                        /* 009DE8  lea  $18000.l,a2 */
    a1 += SX16((d1 & 0xFFFF));                           /* 009DEE  adda.w  d1,a1 */
    a2 += SX16((d1 & 0xFFFF));                           /* 009DF0  adda.w  d1,a2 */
    SETW(d7, 0x3A);                                      /* 009DF2  move.w  #$3a,d7 */
    do {
        BUDGET_CHECK(0x9DF6U);
L_009DF6:
        wr32(a1, rd32(a0));                                  /* 009DF6  move.l  (a0),(a1) */
        /* 009DF8  move.l  (a0)+,(a2) */
        wr32(a2, rd32(a0));
        a0 += 4;
        wr32(a1 + 0x4, rd32(a0));                            /* 009DFA  move.l  (a0),$4(a1) */
        /* 009DFE  move.l  (a0)+,$4(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x4, v);
        }
        a1 = a1 + 0xA0;                                      /* 009E02  lea  $a0(a1),a1 */
        a2 = a2 + 0xA0;                                      /* 009E06  lea  $a0(a2),a2 */
        /* 009E0A  dbra  d7,$9df6 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_009E0E:
    a0 = 0x9E46;                                         /* 009E0E  lea  $9e46(pc),a0 */
    SETW(d0, rd16(a4 + 0xC));                            /* 009E12  move.w  $c(a4),d0 */
    d0 = (d0 & 0xFFFF) * 0xA;                            /* 009E16  mulu.w  #$a,d0 */
    wr16(a4 + 0x4A, rd16(a0 + SX16(d0)));                /* 009E1A  move.w  $0(a0,d0.w),$4a(a4) */
    wr16(a4 + 0x4C, rd16(a0 + 0x2 + SX16(d0)));          /* 009E20  move.w  $2(a0,d0.w),$4c(a4) */
    wr16(a4 + 0x52, rd16(a0 + 0x6 + SX16(d0)));          /* 009E26  move.w  $6(a0,d0.w),$52(a4) */
    wr16(a4 + 0x64, rd16(a0 + 0x8 + SX16(d0)));          /* 009E2C  move.w  $8(a0,d0.w),$64(a4) */
    SETW(d0, rd16(a0 + 0x4 + SX16(d0)));                 /* 009E32  move.w  $4(a0,d0.w),d0 */
    wr8(a4 + 0x4E, (d0 & 0xFF));                         /* 009E36  move.b  d0,$4e(a4) */
    wr8(a4 + 0x19, 0);                                   /* 009E3A  clr.b  $19(a4) */
    /* 009E3E  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 009E42  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9DCCU: goto L_009DCC;
        case 0x9DF6U: goto L_009DF6;
        case 0x9E0EU: goto L_009E0E;
        }
    }
}

/* 009E46  game routine */
void sub_009E46(void)
{
    /* 009E46  ori.b  #$f,d1 */
    {
        uint32_t s = 0xF;
        uint32_t d = (d1 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d1, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x9E4AU);                                       /* 009E4A  soft */
}

/* 009F04  game routine */
void sub_009F04(void)
{
    /* 009F04  ori.b  #$60,$0(a0) */
    {
        uint32_t s = 0x60;
        uint32_t d = rd8(a0);
        uint32_t r = (d | s) & 0xFF;
        wr8(a0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x9F0AU);                                       /* 009F0A  soft */
}

/* 009FA4  game routine */
void sub_009FA4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 009FA4  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    SETW(d7, 0x13);                                      /* 009FA8  move.w  #$13,d7 */
    a3 = V_1614;                                         /* 009FAC  lea  $1614(a5),a3 */
    do {
        BUDGET_CHECK(0x9FB0U);
L_009FB0:
        /* 009FB0  tst.w  (a3) */
        /* 009FB2  beq  $9fd2 */
        if (rd16(a3) != 0) {
L_009FB6:
            SETW(d0, rd16(a3 + 0xA));                            /* 009FB6  move.w  $a(a3),d0 */
            wr16(a3 + 0x2, rd16(a3 + 0x2) + (d0 & 0xFFFF));      /* 009FBA  add.w  d0,$2(a3) */
            SETW(d0, rd16(a3 + 0xC));                            /* 009FBE  move.w  $c(a3),d0 */
            wr16(a3 + 0x4, rd16(a3 + 0x4) + (d0 & 0xFFFF));      /* 009FC2  add.w  d0,$4(a3) */
            /* 009FC6  subi.w  #$1,$8(a3) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a3 + 0x8);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a3 + 0x8, r);
                fz = (r == 0);
                fx = (s > d);
            }
            /* 009FCC  bne  $9fd2 */
            if (COND(7)) {
L_009FD0:
                wr16(a3, 0);                                         /* 009FD0  clr.w  (a3) */
            }
        }
L_009FD2:
        a3 = a3 + 0x10;                                      /* 009FD2  lea  $10(a3),a3 */
        /* 009FD6  dbra  d7,$9fb0 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_009FDA:
    /* 009FDA  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 009FDE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9FB0U: goto L_009FB0;
        case 0x9FB6U: goto L_009FB6;
        case 0x9FD0U: goto L_009FD0;
        case 0x9FD2U: goto L_009FD2;
        case 0x9FDAU: goto L_009FDA;
        }
    }
}

/* 009FE0  game routine */
void sub_009FE0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 009FE0  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
L_009FE4:
    d0 = 0;                                              /* 009FE4  moveq  #0,d0 */
L_009FE6:
    /* 009FE6  move.w  $52(a4),d0 */
    {
        uint32_t v = rd16(a4 + 0x52);
        SETW(d0, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
L_009FEA:
    /* 009FEA  bmi  $a07c */
    if (COND(10)) {
        /* 009FEE  bne  $9ffc */
        if (COND(7)) {
            fz = !(rd8(V_0273) & 0x1);                           /* 009FF0  btst  #$0,$273(a5) */
            /* 009FF6  bne  $a014 */
            if (COND(6)) {
                goto L_00A014;
            }
            SETW(d0, d0 + 1);                                    /* 009FF8  addq.w  #1,d0 */
            /* 009FFA  bra  $a014 */
        } else {
            /* 009FFC  cmpi.w  #$4,$c(a4) */
            /* 00A002  beq  $a00c */
            if (rd16(a4 + 0xC) != 4) {
                /* 00A004  cmpi.w  #$3,$c(a4) */
                /* 00A00A  bne  $a014 */
                if (rd16(a4 + 0xC) != 3) {
                    goto L_00A014;
                }
            }
            fz = !(rd8(V_0273) & 0x1);                           /* 00A00C  btst  #$0,$273(a5) */
            /* 00A012  bne  $a07c */
            if (COND(6)) {
                goto L_00A07C;
            }
        }
L_00A014:
        d0 = (d0 & 0xFFFF) * 0xC0;                           /* 00A014  mulu.w  #$c0,d0 */
        a0 = 0xA082;                                         /* 00A018  lea  $a082(pc),a0 */
        a0 += d0;                                            /* 00A01C  adda.l  d0,a0 */
        d0 = 0;                                              /* 00A01E  moveq  #0,d0 */
        SETB(d0, rd8(a4 + 0x36));                            /* 00A020  move.b  $36(a4),d0 */
        SETB(d0, d0 & 0xF);                                  /* 00A024  andi.b  #$f,d0 */
        d0 = (d0 & 0xFFFF) * 0xC;                            /* 00A028  mulu.w  #$c,d0 */
        a0 += d0;                                            /* 00A02C  adda.l  d0,a0 */
        SETW(d7, 0x13);                                      /* 00A02E  move.w  #$13,d7 */
        a3 = V_1614;                                         /* 00A032  lea  $1614(a5),a3 */
        do {
            BUDGET_CHECK(0xA036U);
L_00A036:
            /* 00A036  tst.w  (a3) */
            /* 00A038  bne  $a074 */
            if (rd16(a3) == 0) {
                /* 00A03C  move.w  (a0)+,d0 */
                SETW(d0, rd16(a0));
                a0 += 2;
                /* 00A03E  tst.w  d0 */
                /* 00A040  bmi  $a07c */
                if ((int16_t)(d0 & 0xFFFF) < 0) {
                    goto L_00A07C;
                }
                SETW(d1, rd16(a4 + 0x2));                            /* 00A042  move.w  $2(a4),d1 */
                SETW(d2, rd16(a4 + 0x4));                            /* 00A046  move.w  $4(a4),d2 */
                wr16(a3 + 0x6, (d0 & 0xFFFF));                       /* 00A04A  move.w  d0,$6(a3) */
                /* 00A04E  add.w  (a0)+,d1 */
                {
                    uint32_t s = rd16(a0);
                    a0 += 2;
                    uint32_t d = (d1 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d1, r);
                }
                wr16(a3 + 0x2, (d1 & 0xFFFF));                       /* 00A050  move.w  d1,$2(a3) */
                /* 00A054  add.w  (a0)+,d2 */
                {
                    uint32_t s = rd16(a0);
                    a0 += 2;
                    uint32_t d = (d2 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d2, r);
                }
                wr16(a3 + 0x4, (d2 & 0xFFFF));                       /* 00A056  move.w  d2,$4(a3) */
                /* 00A05A  move.w  (a0)+,$a(a3) */
                {
                    uint32_t v = rd16(a0);
                    a0 += 2;
                    wr16(a3 + 0xA, v);
                }
                /* 00A05E  move.w  (a0)+,$c(a3) */
                {
                    uint32_t v = rd16(a0);
                    a0 += 2;
                    wr16(a3 + 0xC, v);
                }
                /* 00A062  move.w  (a0)+,$8(a3) */
                {
                    uint32_t v = rd16(a0);
                    a0 += 2;
                    wr16(a3 + 0x8, v);
                }
                wr16(a3 + 0xE, rd16(V_027A));                        /* 00A066  move.w  $27a(a5),$e(a3) */
                wr16(a3, 0xFFFF);                                    /* 00A06C  move.w  #$ffff,(a3) */
                /* 00A070  bra  $a07c */
                goto L_00A07C;
            }
            a3 = a3 + 0x10;                                      /* 00A074  lea  $10(a3),a3 */
            /* 00A078  dbra  d7,$a036 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
    }
L_00A07C:
    /* 00A07C  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 00A080  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x9FE4U: goto L_009FE4;
        case 0x9FE6U: goto L_009FE6;
        case 0x9FEAU: goto L_009FEA;
        case 0xA036U: goto L_00A036;
        case 0xA07CU: goto L_00A07C;
        }
    }
}

