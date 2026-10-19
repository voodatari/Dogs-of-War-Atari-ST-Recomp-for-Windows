/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 008038  game routine */
void sub_008038(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = rd32(a4 + 0x8);                                 /* 008038  move.l  $8(a4),d0 */
    /* 00803C  tst.b  $13(a4) */
    /* 008040  bne  $8050 */
    if (rd8(a4 + 0x13) == 0) {
L_008042:
        /* 008042  cmp.l  #$270f,d0 */
        /* 008048  bls  $8050 */
        if (d0 > 0x270F) {
            /* 00804A  addq.w  #1,(a4) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a4);
                uint32_t r = (d + s) & 0xFFFF;
                wr16(a4, r);
                fx = (r < s);
            }
            wr8(a4 + 0x13, 0xFF);                                /* 00804C  st  $13(a4) */
        }
    }
L_008050:
    CALL(sub_007D3E, 0x8054U);                           /* 008050  bsr  $7d3e */
L_008054:
    /* 008054  lea  $25f(a5),a0 */
    a0 = V_025F;
    { sub_008058(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8042U: goto L_008042;
        case 0x8050U: goto L_008050;
        case 0x8054U: goto L_008054;
        }
    }
}

/* 008058  game routine */
void sub_008058(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    for (;;) {
        BUDGET_CHECK(0x8058U);
        d0 = 0;                                              /* 008058  moveq  #0,d0 */
        /* 00805A  move.b  (a0)+,d0 */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 00805C  beq  $80a8 */
        if (COND(7)) {
            goto L_0080A8;
        }
L_008060:
        SETB(d0, d0 - 0x20);                                 /* 008060  subi.b  #$20,d0 */
        SETW(d0, (d0 & 0xFFFF) << 3);                        /* 008064  lsl.w  #3,d0 */
        a1 = 0x8700;                                         /* 008066  lea  $8700(pc),a1 */
        a1 += d0;                                            /* 00806A  adda.l  d0,a1 */
        a2 = rd32(V_DRAW_BUFFER);                            /* 00806C  movea.l  $28e(a5),a2 */
        a2 += SX16(rd16(V_0266));                            /* 008070  adda.w  $266(a5),a2 */
        SETW(d0, rd16(V_0268));                              /* 008074  move.w  $268(a5),d0 */
        /* 008078  bclr  #$0,d0 */
        fz = !(d0 & 0x1);
        d0 &= ~0x1;
        /* 00807C  beq  $8080 */
        if (COND(6)) {
L_00807E:
            a2 += 1;                                             /* 00807E  addq.w  #1,a2 */
        }
L_008080:
        SETW(d0, (d0 & 0xFFFF) << 2);                        /* 008080  lsl.w  #2,d0 */
        a2 += SX16((d0 & 0xFFFF));                           /* 008082  adda.w  d0,a2 */
        /* 008084  move.b  (a1)+,(a2) */
        wr8(a2, rd8(a1));
        a1 += 1;
        /* 008086  move.b  (a1)+,$a0(a2) */
        wr8(a2 + 0xA0, rd8(a1));
        a1 += 1;
        /* 00808A  move.b  (a1)+,$140(a2) */
        {
            uint32_t v = rd8(a1);
            a1 += 1;
            wr8(a2 + 0x140, v);
        }
        /* 00808E  move.b  (a1)+,$1e0(a2) */
        {
            uint32_t v = rd8(a1);
            a1 += 1;
            wr8(a2 + 0x1E0, v);
        }
        /* 008092  move.b  (a1)+,$280(a2) */
        wr8(a2 + 0x280, rd8(a1));
        a1 += 1;
        /* 008096  move.b  (a1)+,$320(a2) */
        wr8(a2 + 0x320, rd8(a1));
        a1 += 1;
        /* 00809A  move.b  (a1)+,$3c0(a2) */
        wr8(a2 + 0x3C0, rd8(a1));
        a1 += 1;
        /* 00809E  move.b  (a1)+,$460(a2) */
        wr8(a2 + 0x460, rd8(a1));
        a1 += 1;
        /* 0080A2  addq.w  #1,$268(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0268);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_0268, r);
            fx = (r < s);
        }
        /* 0080A6  bra  $8058 */
    }
L_0080A8:
    RTS();                                               /* 0080A8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8060U: goto L_008060;
        case 0x807EU: goto L_00807E;
        case 0x8080U: goto L_008080;
        case 0x80A8U: goto L_0080A8;
        }
    }
}

/* 0080AA  game routine */
void sub_0080AA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0080AA  tst.w  (a4) */
    /* 0080AC  beq  $823c */
    if (rd16(a4) == 0) {
        { sub_00823C(); return; }
    }
L_0080B0:
    SETW(d0, rd16(a4));                                  /* 0080B0  move.w  (a4),d0 */
    SETW(d7, 0xF);                                       /* 0080B2  move.w  #$f,d7 */
    a6 = a0;                                             /* 0080B6  movea.l  a0,a6 */
    wr32(V_0194, a0);                                    /* 0080B8  move.l  a0,$194(a5) */
    a6 -= 1;                                             /* 0080BC  suba.l  #$1,a6 */
    wr32(V_0198, a6);                                    /* 0080C2  move.l  a6,$198(a5) */
    do {
        BUDGET_CHECK(0x80C6U);
L_0080C6:
        wr32(a6, rd32(a6) & 0xFF00FF00);                     /* 0080C6  andi.l  #$ff00ff00,(a6) */
        wr32(a6 + 0x4, rd32(a6 + 0x4) & 0xFF00FF00);         /* 0080CC  andi.l  #$ff00ff00,$4(a6) */
        wr32(a6 + 0x8, 0);                                   /* 0080D4  clr.l  $8(a6) */
        wr32(a6 + 0xC, 0);                                   /* 0080D8  clr.l  $c(a6) */
        wr32(a6 + 0x10, 0);                                  /* 0080DC  clr.l  $10(a6) */
        wr32(a6 + 0x14, 0);                                  /* 0080E0  clr.l  $14(a6) */
        a6 = a6 + 0xA0;                                      /* 0080E4  lea  $a0(a6),a6 */
        /* 0080E8  dbra  d7,$80c6 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_0080EC:
    /* 0080EC  cmp.w  #$5,d0 */
    /* 0080F0  bls  $80f8 */
    if ((d0 & 0xFFFF) > 5) {
        SETW(d0, 5);                                         /* 0080F4  move.w  #$5,d0 */
    }
L_0080F8:
    SETW(d0, d0 - 1);                                    /* 0080F8  subi.w  #$1,d0 */
    do {
        BUDGET_CHECK(0x80FCU);
L_0080FC:
        CALL(sub_008106, 0x80FEU);                           /* 0080FC  bsr  $8106 */
L_0080FE:
        /* 0080FE  dbra  d0,$80fc */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008102:
    /* 008102  bra  $823c */
    { sub_00823C(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x80B0U: goto L_0080B0;
        case 0x80C6U: goto L_0080C6;
        case 0x80ECU: goto L_0080EC;
        case 0x80F8U: goto L_0080F8;
        case 0x80FCU: goto L_0080FC;
        case 0x80FEU: goto L_0080FE;
        case 0x8102U: goto L_008102;
        }
    }
}

/* 008106  game routine */
void sub_008106(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a3 = a0;                                             /* 008106  movea.l  a0,a3 */
    a1 = 0xE2E8;                                         /* 008108  lea  $e2e8(pc),a1 */
    a1 = a1 + 0x1F40;                                    /* 00810C  lea  $1f40(a1),a1 */
    /* 008110  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 008112  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008116  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 00811A  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 00811E  lea  $a0(a3),a3 */
    /* 008122  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 008124  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008128  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 00812C  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 008130  lea  $a0(a3),a3 */
    /* 008134  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 008136  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 00813A  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 00813E  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 008142  lea  $a0(a3),a3 */
    /* 008146  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 008148  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 00814C  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008150  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 008154  lea  $a0(a3),a3 */
    /* 008158  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 00815A  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 00815E  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008162  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 008166  lea  $a0(a3),a3 */
    /* 00816A  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 00816C  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008170  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008174  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 008178  lea  $a0(a3),a3 */
    /* 00817C  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 00817E  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008182  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008186  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 00818A  lea  $a0(a3),a3 */
    /* 00818E  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 008190  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008194  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008198  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 00819C  lea  $a0(a3),a3 */
    /* 0081A0  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 0081A2  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 0081A6  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 0081AA  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 0081AE  lea  $a0(a3),a3 */
    /* 0081B2  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 0081B4  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 0081B8  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 0081BC  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 0081C0  lea  $a0(a3),a3 */
    /* 0081C4  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 0081C6  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 0081CA  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 0081CE  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 0081D2  lea  $a0(a3),a3 */
    /* 0081D6  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 0081D8  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 0081DC  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 0081E0  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 0081E4  lea  $a0(a3),a3 */
    /* 0081E8  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 0081EA  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 0081EE  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 0081F2  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 0081F6  lea  $a0(a3),a3 */
    /* 0081FA  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 0081FC  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008200  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008204  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 008208  lea  $a0(a3),a3 */
    /* 00820C  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 00820E  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008212  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008216  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 00821A  lea  $a0(a3),a3 */
    /* 00821E  move.b  (a1)+,(a3) */
    wr8(a3, rd8(a1));
    a1 += 1;
    /* 008220  move.b  (a1)+,$2(a3) */
    wr8(a3 + 0x2, rd8(a1));
    a1 += 1;
    /* 008224  move.b  (a1)+,$4(a3) */
    wr8(a3 + 0x4, rd8(a1));
    a1 += 1;
    /* 008228  move.b  (a1)+,$6(a3) */
    wr8(a3 + 0x6, rd8(a1));
    a1 += 1;
    a3 = a3 + 0xA0;                                      /* 00822C  lea  $a0(a3),a3 */
    d2 = a0;                                             /* 008230  move.l  a0,d2 */
    /* 008232  bchg  #$0,d2 */
    fz = !(d2 & 0x1);
    d2 ^= 0x1;
    /* 008236  beq  $823a */
    if (COND(6)) {
L_008238:
        SETW(d2, d2 + 8);                                    /* 008238  addq.w  #8,d2 */
    }
L_00823A:
    /* 00823A  movea.l  d2,a0 */
    a0 = d2;
    { sub_00823C(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8238U: goto L_008238;
        case 0x823AU: goto L_00823A;
        }
    }
}

/* 00823C  game routine */
void sub_00823C(void)
{
    RTS();                                               /* 00823C  rts */
}

/* 008258  game routine */
void sub_008258(void)
{
    SETB(d2, rd8(a7));                                   /* 008258  move.b  (a7),d2 */
    /* 00825A  move.w  $0(a0,d1.l),d1 */
    SETW(d1, rd16(a0 + d1));
    { sub_00825E(); return; }
}

/* 00825E  game routine */
void sub_00825E(void)
{
    /* 00825E  addq.w  #1,d5 */
    {
        uint32_t s = 1;
        uint32_t d = (d5 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d5, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    MISS(0x8260U);
}

/* 0083B2  game routine */
void sub_0083B2(void)
{
    wr8(HW_YM_SELECT, 0);                                /* 0083B2  move.b  #$0,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 0083BA  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 1);                                /* 0083C2  move.b  #$1,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 0083CA  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 2);                                /* 0083D2  move.b  #$2,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 0083DA  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 3);                                /* 0083E2  move.b  #$3,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 0083EA  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 4);                                /* 0083F2  move.b  #$4,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 0083FA  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 5);                                /* 008402  move.b  #$5,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00840A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 6);                                /* 008412  move.b  #$6,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00841A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 7);                                /* 008422  move.b  #$7,$ffff8800.l */
    wr8(HW_YM_DATA, 0xFF);                               /* 00842A  move.b  #$ff,$ffff8802.l */
    wr8(HW_YM_SELECT, 8);                                /* 008432  move.b  #$8,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00843A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 9);                                /* 008442  move.b  #$9,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00844A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 0xA);                              /* 008452  move.b  #$a,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00845A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 0xB);                              /* 008462  move.b  #$b,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00846A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 0xC);                              /* 008472  move.b  #$c,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00847A  move.b  #$0,$ffff8802.l */
    wr8(HW_YM_SELECT, 0xD);                              /* 008482  move.b  #$d,$ffff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 00848A  move.b  #$0,$ffff8802.l */
    RTS();                                               /* 008492  rts */
}

/* 008494  game routine */
void sub_008494(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008494  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a0 = a4;                                             /* 008498  movea.l  a4,a0 */
    a1 = a3;                                             /* 00849A  movea.l  a3,a1 */
    CALL(sub_0084A4, 0x849EU);                           /* 00849C  bsr  $84a4 */
L_00849E:
    /* 00849E  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 0084A2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x849EU: goto L_00849E;
        }
    }
}

/* 0084A4  game routine */
void sub_0084A4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a3 = a0;                                             /* 0084A4  movea.l  a0,a3 */
    a3 += 0x7D00;                                        /* 0084A6  adda.l  #$7d00,a3 */
    /* 0084AC  move.w  (a1)+,d0 */
    SETW(d0, rd16(a1));
    a1 += 2;
    a2 = a1 + SX16(d0);                                  /* 0084AE  lea  $0(a1,d0.w),a2 */
    SETW(d3, 0x3E80);                                    /* 0084B2  move.w  #$3e80,d3 */
L_0084B6:
    /* 0084B6  tst.w  d3 */
    /* 0084B8  beq  $8526 */
    if ((d3 & 0xFFFF) != 0) {
L_0084BC:
        /* 0084BC  move.b  (a1)+,d1 */
        SETB(d1, rd8(a1));
        a1 += 1;
        /* 0084BE  cmp.b  #$2,d1 */
        /* 0084C2  bcc  $84d0 */
        if ((d1 & 0xFF) < 2) {
L_0084C6:
            /* 0084C6  move.b  (a1)+,d2 */
            SETB(d2, rd8(a1));
            a1 += 1;
            SETW(d2, (d2 & 0xFFFF) << 8);                        /* 0084C8  lsl.w  #8,d2 */
            /* 0084CA  move.b  (a1)+,d2 */
            SETB(d2, rd8(a1));
            a1 += 1;
            /* 0084CC  bra  $84dc */
        } else {
L_0084D0:
            /* 0084D0  move.b  d1,d2 */
            /* 0084D2  bpl  $84d8 */
            SETB(d2, (d1 & 0xFF));
            if ((int8_t)(d2 & 0xFF) < 0) {
L_0084D6:
                /* 0084D6  neg.b  d2 */
                { uint32_t d, r, ea; (void)ea;
                d = (d2 & 0xFFU);
                r = (0 - d) & 0xFFU;
                d2 = (d2 & 0xFFFFFF00U) | ((r) & 0xFFU);
                FL_SUB(d, 0u, r, 0xFFU, 0x80U, 1); }
            }
L_0084D8:
            SETW(d2, d2 & 0xFF);                                 /* 0084D8  andi.w  #$ff,d2 */
        }
L_0084DC:
        SETW(d2, d2 - 1);                                    /* 0084DC  subq.w  #1,d2 */
        /* 0084DE  tst.b  d1 */
        /* 0084E0  bmi  $850a */
        if ((int8_t)(d1 & 0xFF) >= 0) {
L_0084E4:
            /* 0084E4  cmp.b  #$1,d1 */
            /* 0084E8  beq  $850a */
            if ((d1 & 0xFF) != 1) {
L_0084EC:
                /* 0084EC  move.w  (a2)+,d1 */
                SETW(d1, rd16(a2));
                a2 += 2;
                do {
                    BUDGET_CHECK(0x84EEU);
L_0084EE:
                    wr16(a0, (d1 & 0xFFFF));                             /* 0084EE  move.w  d1,(a0) */
                    /* 0084F0  subq.w  #1,d3 */
                    {
                        uint32_t s = 1;
                        uint32_t d = (d3 & 0xFFFF);
                        uint32_t r = (d - s) & 0xFFFF;
                        SETW(d3, r);
                        fx = (s > d);
                    }
                    a0 += 0xA0;                                          /* 0084F2  adda.l  #$a0,a0 */
                    /* 0084F8  cmpa.l  a3,a0 */
                    /* 0084FA  bcs  $8504 */
                    if (a0 >= a3) {
L_0084FE:
                        a0 -= 0x7CFE;                                        /* 0084FE  suba.l  #$7cfe,a0 */
                    }
L_008504:
                    /* 008504  dbra  d2,$84ee */
                    SETW(d2, d2 - 1);
                } while ((d2 & 0xFFFF) != 0xFFFF);
L_008508:
                /* 008508  bra  $84b6 */
                BUDGET_CHECK(0x84B6U);
                goto L_0084B6;
            }
        }
        do {
            BUDGET_CHECK(0x850AU);
L_00850A:
            /* 00850A  move.w  (a2)+,(a0) */
            wr16(a0, rd16(a2));
            a2 += 2;
            /* 00850C  subq.w  #1,d3 */
            {
                uint32_t s = 1;
                uint32_t d = (d3 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d3, r);
                fx = (s > d);
            }
            a0 += 0xA0;                                          /* 00850E  adda.l  #$a0,a0 */
            /* 008514  cmpa.l  a3,a0 */
            /* 008516  bcs  $8520 */
            if (a0 >= a3) {
L_00851A:
                a0 -= 0x7CFE;                                        /* 00851A  suba.l  #$7cfe,a0 */
            }
L_008520:
            /* 008520  dbra  d2,$850a */
            SETW(d2, d2 - 1);
        } while ((d2 & 0xFFFF) != 0xFFFF);
L_008524:
        /* 008524  bra  $84b6 */
        BUDGET_CHECK(0x84B6U);
        goto L_0084B6;
    }
L_008526:
    RTS();                                               /* 008526  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x84B6U: goto L_0084B6;
        case 0x84BCU: goto L_0084BC;
        case 0x84C6U: goto L_0084C6;
        case 0x84D0U: goto L_0084D0;
        case 0x84D6U: goto L_0084D6;
        case 0x84D8U: goto L_0084D8;
        case 0x84DCU: goto L_0084DC;
        case 0x84E4U: goto L_0084E4;
        case 0x84ECU: goto L_0084EC;
        case 0x84EEU: goto L_0084EE;
        case 0x84FEU: goto L_0084FE;
        case 0x8504U: goto L_008504;
        case 0x8508U: goto L_008508;
        case 0x850AU: goto L_00850A;
        case 0x851AU: goto L_00851A;
        case 0x8520U: goto L_008520;
        case 0x8524U: goto L_008524;
        case 0x8526U: goto L_008526;
        }
    }
}

/* 008528  game routine */
void sub_008528(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0208, 4);                                     /* 008528  move.w  #$4,$208(a5) */
    d7 = 7;                                              /* 00852E  moveq  #7,d7 */
    do {
        BUDGET_CHECK(0x8530U);
L_008530:
        CALL(sub_008538, 0x8532U);                           /* 008530  bsr  $8538 */
L_008532:
        /* 008532  dbra  d7,$8530 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 008536  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8530U: goto L_008530;
        case 0x8532U: goto L_008532;
        }
    }
}

/* 008538  game routine */
void sub_008538(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    for (;;) {
        BUDGET_CHECK(0x8538U);
        CALL(wait_vbl, 0x853CU);                             /* 008538  bsr  $79dc */
L_00853C:
        /* 00853C  tst.w  $208(a5) */
        /* 008540  beq  $8548 */
        if (rd16(V_0208) == 0) {
            goto L_008548;
        }
        /* 008542  subq.w  #1,$208(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0208);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0208, r);
            fx = (s > d);
        }
        /* 008546  bra  $8538 */
    }
L_008548:
    wr16(V_0208, 4);                                     /* 008548  move.w  #$4,$208(a5) */
    a0 = HW_PALETTE;                                     /* 00854E  lea  $ff8240.l,a0 */
    d0 = 0xF;                                            /* 008554  moveq  #15,d0 */
    do {
        BUDGET_CHECK(0x8556U);
L_008556:
        SETW(d1, rd16(a0));                                  /* 008556  move.w  (a0),d1 */
        /* 008558  andi.w  #$700,d1 */
        {
            uint32_t s = 0x700;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d1, r);
            fz = (r == 0);
        }
        /* 00855C  beq  $8562 */
        if (COND(6)) {
            /* 00855E  subi.w  #$100,(a0) */
            {
                uint32_t s = 0x100;
                uint32_t d = rd16(a0);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a0, r);
                fx = (s > d);
            }
        }
        SETW(d1, rd16(a0));                                  /* 008562  move.w  (a0),d1 */
        /* 008564  andi.w  #$70,d1 */
        {
            uint32_t s = 0x70;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d1, r);
            fz = (r == 0);
        }
        /* 008568  beq  $856e */
        if (COND(6)) {
            /* 00856A  subi.w  #$10,(a0) */
            {
                uint32_t s = 0x10;
                uint32_t d = rd16(a0);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a0, r);
                fx = (s > d);
            }
        }
        SETW(d1, rd16(a0));                                  /* 00856E  move.w  (a0),d1 */
        /* 008570  andi.w  #$7,d1 */
        {
            uint32_t s = 7;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d1, r);
            fz = (r == 0);
        }
        /* 008574  beq  $857a */
        if (COND(6)) {
            /* 008576  subi.w  #$1,(a0) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a0);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a0, r);
                fx = (s > d);
            }
        }
        a0 = a0 + 0x2;                                       /* 00857A  lea  $2(a0),a0 */
        /* 00857E  dbra  d0,$8556 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 008582  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x853CU: goto L_00853C;
        case 0x8556U: goto L_008556;
        }
    }
}

/* 008584  game routine */
void sub_008584(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    for (;;) {
        BUDGET_CHECK(0x8584U);
        d0 = 0;                                              /* 008584  moveq  #0,d0 */
        /* 008586  move.b  (a0)+,d0 */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 008588  beq  $85b0 */
        if (COND(7)) { sub_0085B0(); return; }
L_00858A:
        CALL(sub_00858E, 0x858CU);                           /* 00858A  bsr  $858e */
L_00858C:
        /* 00858C  bra  $8584 */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x858AU: goto L_00858A;
        case 0x858CU: goto L_00858C;
        }
    }
}

/* 00858E  game routine */
void sub_00858E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00858E  bclr  #$7,d0 */
    fz = !(d0 & 0x80);
    d0 &= ~0x80;
    /* 008592  beq  $859c */
    if (COND(6)) {
L_008594:
        SETW(d0, SX8(d0));                                   /* 008594  ext.w  d0 */
        wr16(V_026C, (d0 & 0xFFFF));                         /* 008596  move.w  d0,$26c(a5) */
        RTS();                                               /* 00859A  rts */
    }
L_00859C:
    /* 00859C  cmp.b  #$d,d0 */
    /* 0085A0  bne  $85b2 */
    if ((d0 & 0xFF) == 0xD) {
L_0085A2:
        SETW(d0, rd16(V_026A));                              /* 0085A2  move.w  $26a(a5),d0 */
        wr16(V_0268, (d0 & 0xFFFF));                         /* 0085A6  move.w  d0,$268(a5) */
        /* 0085AA  addi.w  #$5a0,$266(a5) */
        wr16(V_0266, rd16(V_0266) + 0x5A0);
        { sub_0085B0(); return; }
    }
L_0085B2:
    /* 0085B2  subi.b  #$20,d0 */
    {
        uint32_t s = 0x20;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d - s) & 0xFF;
        SETB(d0, r);
        fn = !!(r & 0x80);
        fz = (r == 0);
    }
    /* 0085B6  beq  $85c4 */
    if (COND(6)) {
L_0085B8:
        /* 0085B8  bpl  $85c4 */
        if (COND(11)) {
L_0085BA:
            SETB(d0, d0 + 0x20);                                 /* 0085BA  addi.b  #$20,d0 */
            /* 0085BE  add.w  d0,$268(a5) */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t d = rd16(V_0268);
                uint32_t r = (d + s) & 0xFFFF;
                wr16(V_0268, r);
                fx = (r < s);
            }
            RTS();                                               /* 0085C2  rts */
        }
    }
L_0085C4:
    d0 = (d0 & 0xFFFF) * 8;                              /* 0085C4  mulu.w  #$8,d0 */
    a1 = 0x8700;                                         /* 0085C8  lea  $8700(pc),a1 */
    a1 += d0;                                            /* 0085CC  adda.l  d0,a1 */
    a2 = rd32(V_DRAW_BUFFER);                            /* 0085CE  movea.l  $28e(a5),a2 */
    d0 = 0;                                              /* 0085D2  clr.l  d0 */
    SETW(d0, rd16(V_0266));                              /* 0085D4  move.w  $266(a5),d0 */
    a2 += SX16((d0 & 0xFFFF));                           /* 0085D8  adda.w  d0,a2 */
    SETW(d0, rd16(V_0268));                              /* 0085DA  move.w  $268(a5),d0 */
    SETW(d1, 8);                                         /* 0085DE  move.w  #$8,d1 */
    SETW(d2, (d0 & 0xFFFF));                             /* 0085E2  move.w  d0,d2 */
    SETW(d0, d0 & 0xFE);                                 /* 0085E4  andi.w  #$fe,d0 */
    d0 = (d0 & 0xFFFF) * 4;                              /* 0085E8  mulu.w  #$4,d0 */
    a2 += SX16((d0 & 0xFFFF));                           /* 0085EC  adda.w  d0,a2 */
    fz = !(d2 & 0x1);                                    /* 0085EE  btst  #$0,d2 */
    /* 0085F2  beq  $85f8 */
    if (COND(6)) {
L_0085F4:
        a2 += SX16(1);                                       /* 0085F4  adda.w  #$1,a2 */
    }
    do {
        BUDGET_CHECK(0x85F8U);
L_0085F8:
        SETW(d0, 0);                                         /* 0085F8  clr.w  d0 */
        /* 0085FA  move.b  (a1)+,d0 */
        SETB(d0, rd8(a1));
        a1 += 1;
        wr8(a2, 0);                                          /* 0085FC  clr.b  (a2) */
        wr8(a2 + 0x2, 0);                                    /* 0085FE  clr.b  $2(a2) */
        wr8(a2 + 0x4, 0);                                    /* 008602  clr.b  $4(a2) */
        wr8(a2 + 0x6, 0);                                    /* 008606  clr.b  $6(a2) */
        SETW(d7, rd16(V_026C));                              /* 00860A  move.w  $26c(a5),d7 */
        fz = !(d7 & 0x1);                                    /* 00860E  btst  #$0,d7 */
        /* 008612  beq  $8616 */
        if (COND(6)) {
L_008614:
            wr8(a2, (d0 & 0xFF));                                /* 008614  move.b  d0,(a2) */
        }
L_008616:
        fz = !(d7 & 0x2);                                    /* 008616  btst  #$1,d7 */
        /* 00861A  beq  $8620 */
        if (COND(6)) {
L_00861C:
            wr8(a2 + 0x2, (d0 & 0xFF));                          /* 00861C  move.b  d0,$2(a2) */
        }
L_008620:
        fz = !(d7 & 0x4);                                    /* 008620  btst  #$2,d7 */
        /* 008624  beq  $862a */
        if (COND(6)) {
L_008626:
            wr8(a2 + 0x4, (d0 & 0xFF));                          /* 008626  move.b  d0,$4(a2) */
        }
L_00862A:
        fz = !(d7 & 0x8);                                    /* 00862A  btst  #$3,d7 */
        /* 00862E  beq  $8634 */
        if (COND(6)) {
L_008630:
            wr8(a2 + 0x6, (d0 & 0xFF));                          /* 008630  move.b  d0,$6(a2) */
        }
L_008634:
        a2 += 0xA0;                                          /* 008634  adda.l  #$a0,a2 */
        /* 00863A  subi.b  #$1,d1 */
        {
            uint32_t s = 1;
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d1, r);
            fz = (r == 0);
        }
        /* 00863E  bne  $85f8 */
    } while (COND(6));
L_008640:
    /* 008640  addi.w  #$1,$268(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_0268);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_0268, r);
        fx = (r < s);
    }
    RTS();                                               /* 008646  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8594U: goto L_008594;
        case 0x859CU: goto L_00859C;
        case 0x85A2U: goto L_0085A2;
        case 0x85B2U: goto L_0085B2;
        case 0x85B8U: goto L_0085B8;
        case 0x85BAU: goto L_0085BA;
        case 0x85C4U: goto L_0085C4;
        case 0x85F4U: goto L_0085F4;
        case 0x85F8U: goto L_0085F8;
        case 0x8614U: goto L_008614;
        case 0x8616U: goto L_008616;
        case 0x861CU: goto L_00861C;
        case 0x8620U: goto L_008620;
        case 0x8626U: goto L_008626;
        case 0x862AU: goto L_00862A;
        case 0x8630U: goto L_008630;
        case 0x8634U: goto L_008634;
        case 0x8640U: goto L_008640;
        }
    }
}

/* 0085B0  game routine */
void sub_0085B0(void)
{
    RTS();                                               /* 0085B0  rts */
}

/* 008648  game routine */
void sub_008648(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008648  tst.w  $232(a5) */
    /* 00864C  beq  $8688 */
    if (rd16(V_0232) != 0) {
        /* 00864E  cmpi.w  #$2,$238(a5) */
        /* 008654  bcc  $865c */
        if (rd16(V_0238) < 2) {
            wr16(V_0238, rd16(V_0238) + 1);                      /* 008656  addq.w  #1,$238(a5) */
            /* 00865A  bra  $8688 */
        } else {
            wr16(V_0238, 0);                                     /* 00865C  clr.w  $238(a5) */
            SETW(d0, rd16(V_0230));                              /* 008660  move.w  $230(a5),d0 */
            SETW(d0, (d0 & 0xFFFF) << 1);                        /* 008664  lsl.w  #1,d0 */
            a0 = HW_PALETTE;                                     /* 008666  lea  $ff8240.l,a0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 00866C  adda.w  d0,a0 */
            for (;;) {
                BUDGET_CHECK(0x866EU);
L_00866E:
                a1 = rd32(V_0234);                                   /* 00866E  movea.l  $234(a5),a1 */
                /* 008672  move.w  (a1)+,d0 */
                SETW(d0, rd16(a1));
                a1 += 2;
                /* 008674  tst.w  d0 */
                /* 008676  bpl  $8682 */
                if ((int16_t)(d0 & 0xFFFF) >= 0) {
                    goto L_008682;
                }
                a2 = 0x35EE;                                         /* 008678  lea  $35ee(pc),a2 */
                wr32(V_0234, a2);                                    /* 00867C  move.l  a2,$234(a5) */
                /* 008680  bra  $866e */
            }
L_008682:
            wr32(V_0234, a1);                                    /* 008682  move.l  a1,$234(a5) */
            wr16(a0, (d0 & 0xFFFF));                             /* 008686  move.w  d0,(a0) */
        }
    }
L_008688:
    RTS();                                               /* 008688  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x866EU: goto L_00866E;
        case 0x8688U: goto L_008688;
        }
    }
}

/* 00868A  game routine */
void sub_00868A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_02DA;                                         /* 00868A  lea  $2da(a5),a4 */
    CALL(sub_008696, 0x8692U);                           /* 00868E  bsr  $8696 */
L_008692:
    /* 008692  lea  $376(a5),a4 */
    a4 = V_0376;
    { sub_008696(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8692U: goto L_008692;
        }
    }
}

/* 008696  game routine */
void sub_008696(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = a4 + 0x66;                                      /* 008696  lea  $66(a4),a0 */
    d0 = 0;                                              /* 00869A  moveq  #0,d0 */
    for (;;) {
        BUDGET_CHECK(0x869CU);
L_00869C:
        /* 00869C  tst.b  $0(a0,d0.w) */
        /* 0086A0  bne  $86a6 */
        if (rd8(a0 + SX16(d0)) != 0) {
            goto L_0086A6;
        }
L_0086A2:
        SETW(d0, d0 + 1);                                    /* 0086A2  addq.w  #1,d0 */
        /* 0086A4  bra  $869c */
    }
L_0086A6:
    CALL(sub_00AAF8, 0x86AAU);                           /* 0086A6  bsr  $aaf8 */
L_0086AA:
    wr16(a4 + 0x62, (d0 & 0xFFFF));                      /* 0086AA  move.w  d0,$62(a4) */
    RTS();                                               /* 0086AE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x869CU: goto L_00869C;
        case 0x86A2U: goto L_0086A2;
        case 0x86A6U: goto L_0086A6;
        case 0x86AAU: goto L_0086AA;
        }
    }
}

/* 0086B0  game routine */
void sub_0086B0(void)
{
    /* 0086B0  bra  $e92a */
    { sub_00E92A(); return; }
}

/* 0086B4  game routine */
void sub_0086B4(void)
{
    /* 0086B4  bra  $eb26 */
    { sub_00EB26(); return; }
}

/* 0086B8  game routine */
void sub_0086B8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0086B8  bra  $fe32 */
    goto L_00FE32;
L_008930:
    /* 008930  bra  $fd84 */
L_00FD84:
    a0 = 0x5647E;                                        /* 00FD84  lea  $5647e.l,a0 */
    a1 = 0x5063A;                                        /* 00FD8A  lea  $5063a.l,a1 */
    a6 = a1;                                             /* 00FD90  movea.l  a1,a6 */
    a6 += 0x5E48;                                        /* 00FD92  adda.l  #$5e48,a6 */
    SETB(d4, 0xFF);                                      /* 00FD98  move.b  #$ff,d4 */
L_00FD9C:
    /* 00FD9C  move.b  (a0)+,d0 */
    SETB(d0, rd8(a0));
    a0 += 1;
    /* 00FD9E  cmpa.l  a6,a1 */
    /* 00FDA0  bge  $fdaa */
    if ((int32_t)a1 < (int32_t)a6) {
L_00FDA2:
        /* 00FDA2  cmp.b  d4,d0 */
        /* 00FDA4  beq  $fdac */
        if ((d0 & 0xFF) == (d4 & 0xFF)) {
            goto L_00FDAC;
        }
L_00FDA6:
        /* 00FDA6  move.b  d0,(a1)+ */
        {
            uint32_t v = (d0 & 0xFF);
            wr8(a1, v);
            a1 += 1;
        }
        /* 00FDA8  bra  $fd9c */
        BUDGET_CHECK(0xFD9CU);
        goto L_00FD9C;
    }
L_00FDAA:
    RTS();                                               /* 00FDAA  rts */
L_00FDAC:
    /* 00FDAC  cmpi.b  #$ff,(a0) */
    /* 00FDB0  bne  $fdb8 */
    if (rd8(a0) == 0xFF) {
        /* 00FDB2  move.b  d4,(a1)+ */
        {
            uint32_t v = (d4 & 0xFF);
            wr8(a1, v);
            a1 += 1;
        }
        a0 += 1;                                             /* 00FDB4  addq.l  #1,a0 */
        /* 00FDB6  bra  $fd9c */
        BUDGET_CHECK(0xFD9CU);
        goto L_00FD9C;
    }
L_00FDB8:
    d0 = 0;                                              /* 00FDB8  moveq  #0,d0 */
    d1 = 0;                                              /* 00FDBA  moveq  #0,d1 */
    /* 00FDBC  move.b  (a0)+,d1 */
    SETB(d1, rd8(a0));
    a0 += 1;
    /* 00FDBE  move.b  (a0)+,d0 */
    SETB(d0, rd8(a0));
    a0 += 1;
    do {
        BUDGET_CHECK(0xFDC0U);
L_00FDC0:
        /* 00FDC0  move.b  d0,(a1)+ */
        {
            uint32_t v = (d0 & 0xFF);
            wr8(a1, v);
            a1 += 1;
        }
        /* 00FDC2  dbra  d1,$fdc0 */
        SETW(d1, d1 - 1);
    } while ((d1 & 0xFFFF) != 0xFFFF);
L_00FDC6:
    /* 00FDC6  bra  $fd9c */
    BUDGET_CHECK(0xFD9CU);
    goto L_00FD9C;
L_00FE32:
    wr16(V_01BE, 1);                                     /* 00FE32  move.w  #$1,$1be(a5) */
    wr16(V_015A, 0);                                     /* 00FE38  clr.w  $15a(a5) */
    wr16(V_0168, 1);                                     /* 00FE3C  move.w  #$1,$168(a5) */
    wr16(V_0166, 1);                                     /* 00FE42  move.w  #$1,$166(a5) */
    wr16(V_016A, 1);                                     /* 00FE48  move.w  #$1,$16a(a5) */
    wr16(V_0276, 1);                                     /* 00FE4E  move.w  #$1,$276(a5) */
    CALL(sub_0086B4, 0xFE58U);                           /* 00FE54  bsr  $86b4 */
L_00FE58:
    wr32(V_016C, 0x5647E);                               /* 00FE58  move.l  #$5647e,$16c(a5) */
    CALL(sub_0086B0, 0xFE64U);                           /* 00FE60  bsr  $86b0 */
L_00FE64:
    /* 00FE64  tst.w  d0 */
    {
        uint32_t v = (d0 & 0xFFFF);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 00FE66  bne  $fe6e */
    if (COND(7)) {
L_00FE68:
        /* 00FE68  move.w  #$ffff,$15a(a5) */
        {
            uint32_t v = 0xFFFF;
            wr16(V_015A, v);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
    }
    CALL(sub_008904, 0xFE72U);                           /* 00FE6E  bsr  $8904 */
L_00FE72:
    wr16(V_01BE, 0xC);                                   /* 00FE72  move.w  #$c,$1be(a5) */
    wr16(V_0166, 0);                                     /* 00FE78  clr.w  $166(a5) */
    wr16(V_0164, 0);                                     /* 00FE7C  clr.w  $164(a5) */
    CALL(sub_0086B4, 0xFE84U);                           /* 00FE80  bsr  $86b4 */
L_00FE84:
    a0 = 0xFF14;                                         /* 00FE84  lea  $ff14(pc),a0 */
    CALL(sub_00FF3C, 0xFE8CU);                           /* 00FE88  bsr  $ff3c */
L_00FE8C:
    a4 = 0x3561F;                                        /* 00FE8C  lea  $3561f.l,a4 */
    a6 = 0x2E000;                                        /* 00FE92  lea  $2e000.l,a6 */
    a1 = 0x4754C;                                        /* 00FE98  lea  $4754c.l,a1 */
    a3 = 0x14000;                                        /* 00FE9E  lea  $14000.l,a3 */
    CALL(sub_00FC88, 0xFEA8U);                           /* 00FEA4  bsr  $fc88 */
L_00FEA8:
    a1 = 0x2F91C;                                        /* 00FEA8  lea  $2f91c.l,a1 */
    wr16(V_0276, 0x8FC);                                 /* 00FEAE  move.w  #$8fc,$276(a5) */
    CALL(sub_00FF62, 0xFEB8U);                           /* 00FEB4  bsr  $ff62 */
L_00FEB8:
    a1 = 0x45736;                                        /* 00FEB8  lea  $45736.l,a1 */
    wr16(V_0276, 0xC0);                                  /* 00FEBE  move.w  #$c0,$276(a5) */
    CALL(sub_00FF62, 0xFEC8U);                           /* 00FEC4  bsr  $ff62 */
L_00FEC8:
    a0 = 0x46F68;                                        /* 00FEC8  lea  $46f68.l,a0 */
    a1 = 0x49A3A;                                        /* 00FECE  lea  $49a3a.l,a1 */
    SETW(d0, 0x5FF);                                     /* 00FED4  move.w  #$5ff,d0 */
    do {
        BUDGET_CHECK(0xFED8U);
L_00FED8:
        /* 00FED8  move.b  (a0)+,(a1)+ */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            wr8(a1, v);
            a1 += 1;
        }
        /* 00FEDA  dbra  d0,$fed8 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_00FEDE:
    CALL(sub_008900, 0xFEE2U);                           /* 00FEDE  bsr  $8900 */
L_00FEE2:
    wr16(V_0164, 0);                                     /* 00FEE2  clr.w  $164(a5) */
    wr16(V_0166, 0);                                     /* 00FEE6  move.w  #$0,$166(a5) */
    wr16(V_0168, 0x14);                                  /* 00FEEC  move.w  #$14,$168(a5) */
    wr16(V_016A, 3);                                     /* 00FEF2  move.w  #$3,$16a(a5) */
    wr16(V_0276, 0x26);                                  /* 00FEF8  move.w  #$26,$276(a5) */
    wr32(V_016C, 0x5647E);                               /* 00FEFE  move.l  #$5647e,$16c(a5) */
    CALL(sub_00EB26, 0xFF0AU);                           /* 00FF06  bsr  $eb26 */
L_00FF0A:
    CALL(sub_00E92A, 0xFF0EU);                           /* 00FF0A  bsr  $e92a */
L_00FF0E:
    /* 00FF0E  bra  $8930 */
    BUDGET_CHECK(0x8930U);
    goto L_008930;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8930U: goto L_008930;
        case 0xFD84U: goto L_00FD84;
        case 0xFD9CU: goto L_00FD9C;
        case 0xFDA2U: goto L_00FDA2;
        case 0xFDA6U: goto L_00FDA6;
        case 0xFDAAU: goto L_00FDAA;
        case 0xFDACU: goto L_00FDAC;
        case 0xFDB8U: goto L_00FDB8;
        case 0xFDC0U: goto L_00FDC0;
        case 0xFDC6U: goto L_00FDC6;
        case 0xFE32U: goto L_00FE32;
        case 0xFE58U: goto L_00FE58;
        case 0xFE64U: goto L_00FE64;
        case 0xFE68U: goto L_00FE68;
        case 0xFE72U: goto L_00FE72;
        case 0xFE84U: goto L_00FE84;
        case 0xFE8CU: goto L_00FE8C;
        case 0xFEA8U: goto L_00FEA8;
        case 0xFEB8U: goto L_00FEB8;
        case 0xFEC8U: goto L_00FEC8;
        case 0xFED8U: goto L_00FED8;
        case 0xFEDEU: goto L_00FEDE;
        case 0xFEE2U: goto L_00FEE2;
        case 0xFF0AU: goto L_00FF0A;
        case 0xFF0EU: goto L_00FF0E;
        }
    }
}

/* 0086BC  game routine */
void sub_0086BC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0086BC  bra  $ca02 */
L_00CA02:
    a1 = 0x8C24;                                         /* 00CA02  lea  $8c24(pc),a1 */
    CALL(sub_0075E2, 0xCA0AU);                           /* 00CA06  bsr  $75e2 */
L_00CA0A:
    wr16(V_026C, 8);                                     /* 00CA0A  move.w  #$8,$26c(a5) */
    wr16(V_0266, 0x3C00);                                /* 00CA10  move.w  #$3c00,$266(a5) */
    wr16(V_0268, 6);                                     /* 00CA16  move.w  #$6,$268(a5) */
    wr16(V_026A, 6);                                     /* 00CA1C  move.w  #$6,$26a(a5) */
    /* 00CA22  tst.w  $2da(a5) */
    /* 00CA26  beq  $cab4 */
    if (rd16(V_02DA) != 0) {
L_00CA2A:
        a0 = 0xCE74;                                         /* 00CA2A  lea  $ce74(pc),a0 */
        CALL(sub_00A9D8, 0xCA32U);                           /* 00CA2E  bsr  $a9d8 */
L_00CA32:
        d7 = 0xC350;                                         /* 00CA32  move.l  #$c350,d7 */
        CALL(sub_0075A4, 0xCA3CU);                           /* 00CA38  bsr  $75a4 */
L_00CA3C:
        a3 = 0x5AC80;                                        /* 00CA3C  lea  $5ac80.l,a3 */
        a4 = 0x18000;                                        /* 00CA42  lea  $18000.l,a4 */
        CALL(sub_008494, 0xCA4CU);                           /* 00CA48  bsr  $8494 */
L_00CA4C:
        a0 = 0x18000;                                        /* 00CA4C  lea  $18000.l,a0 */
        a1 = 0x20000;                                        /* 00CA52  lea  $20000.l,a1 */
        CALL(sub_007564, 0xCA5CU);                           /* 00CA58  bsr  $7564 */
L_00CA5C:
        wr16(V_027A, 0);                                     /* 00CA5C  clr.w  $27a(a5) */
        wr32(V_01F6, 0x20000);                               /* 00CA60  move.l  #$20000,$1f6(a5) */
        wr16(V_01EE, 0x64);                                  /* 00CA68  move.w  #$64,$1ee(a5) */
        wr16(V_01EC, 0x64);                                  /* 00CA6E  move.w  #$64,$1ec(a5) */
        wr16(V_01F2, 0xC);                                   /* 00CA74  move.w  #$c,$1f2(a5) */
        wr8(V_01F0, 0xFF);                                   /* 00CA7A  st  $1f0(a5) */
        wr8(V_1DE2, 0);                                      /* 00CA7E  clr.b  $1de2(a5) */
        a4 = V_02DA;                                         /* 00CA82  lea  $2da(a5),a4 */
        d0 = rd32(a4 + 0x58);                                /* 00CA86  move.l  $58(a4),d0 */
        CALL(sub_007D3E, 0xCA8EU);                           /* 00CA8A  bsr  $7d3e */
L_00CA8E:
        a0 = V_025E;                                         /* 00CA8E  lea  $25e(a5),a0 */
        wr16(V_0266, 0x74E0);                                /* 00CA92  move.w  #$74e0,$266(a5) */
        wr16(V_0268, 0x1C);                                  /* 00CA98  move.w  #$1c,$268(a5) */
        wr16(V_026A, 0x1C);                                  /* 00CA9E  move.w  #$1c,$26a(a5) */
        CALL(sub_008058, 0xCAA8U);                           /* 00CAA4  bsr  $8058 */
L_00CAA8:
        CALL(sub_00CB68, 0xCAACU);                           /* 00CAA8  bsr  $cb68 */
L_00CAAC:
        /* 00CAAC  tst.w  $1b4(a5) */
        /* 00CAB0  beq  $cb5c */
        if (rd16(V_01B4) == 0) {
            goto L_00CB5C;
        }
    }
    CALL(sub_007626, 0xCAB8U);                           /* 00CAB4  bsr  $7626 */
L_00CAB8:
    wr16(V_026C, 8);                                     /* 00CAB8  move.w  #$8,$26c(a5) */
    wr16(V_0266, 0x3C00);                                /* 00CABE  move.w  #$3c00,$266(a5) */
    wr16(V_0268, 6);                                     /* 00CAC4  move.w  #$6,$268(a5) */
    wr16(V_026A, 6);                                     /* 00CACA  move.w  #$6,$26a(a5) */
    /* 00CAD0  tst.w  $376(a5) */
    /* 00CAD4  beq  $cb5c */
    if (rd16(V_0376) != 0) {
        a0 = 0xCE8E;                                         /* 00CAD8  lea  $ce8e(pc),a0 */
        CALL(sub_00A9D8, 0xCAE0U);                           /* 00CADC  bsr  $a9d8 */
L_00CAE0:
        d7 = 0xC350;                                         /* 00CAE0  move.l  #$c350,d7 */
        CALL(sub_0075A4, 0xCAEAU);                           /* 00CAE6  bsr  $75a4 */
L_00CAEA:
        a3 = 0x5AC80;                                        /* 00CAEA  lea  $5ac80.l,a3 */
        a4 = 0x18000;                                        /* 00CAF0  lea  $18000.l,a4 */
        CALL(sub_008494, 0xCAFAU);                           /* 00CAF6  bsr  $8494 */
L_00CAFA:
        a0 = 0x18000;                                        /* 00CAFA  lea  $18000.l,a0 */
        a1 = 0x20000;                                        /* 00CB00  lea  $20000.l,a1 */
        CALL(sub_007564, 0xCB0AU);                           /* 00CB06  bsr  $7564 */
L_00CB0A:
        wr16(V_027A, 1);                                     /* 00CB0A  move.w  #$1,$27a(a5) */
        wr32(V_01F6, 0x20000);                               /* 00CB10  move.l  #$20000,$1f6(a5) */
        wr16(V_01EE, 0x64);                                  /* 00CB18  move.w  #$64,$1ee(a5) */
        wr16(V_01EC, 0x64);                                  /* 00CB1E  move.w  #$64,$1ec(a5) */
        wr16(V_01F2, 0xC);                                   /* 00CB24  move.w  #$c,$1f2(a5) */
        wr8(V_01F0, 0xFF);                                   /* 00CB2A  st  $1f0(a5) */
        wr8(V_1DE2, 0);                                      /* 00CB2E  clr.b  $1de2(a5) */
        a4 = V_0376;                                         /* 00CB32  lea  $376(a5),a4 */
        d0 = rd32(a4 + 0x58);                                /* 00CB36  move.l  $58(a4),d0 */
        CALL(sub_007D3E, 0xCB3EU);                           /* 00CB3A  bsr  $7d3e */
L_00CB3E:
        a0 = V_025E;                                         /* 00CB3E  lea  $25e(a5),a0 */
        wr16(V_0266, 0x74E0);                                /* 00CB42  move.w  #$74e0,$266(a5) */
        wr16(V_0268, 0x1C);                                  /* 00CB48  move.w  #$1c,$268(a5) */
        wr16(V_026A, 0x1C);                                  /* 00CB4E  move.w  #$1c,$26a(a5) */
        CALL(sub_008058, 0xCB58U);                           /* 00CB54  bsr  $8058 */
L_00CB58:
        CALL(sub_00CB68, 0xCB5CU);                           /* 00CB58  bsr  $cb68 */
    }
L_00CB5C:
    wr16(0xFF825C, 0);                                   /* 00CB5C  clr.w  $ff825c.l */
    CALL(sub_00751A, 0xCB66U);                           /* 00CB62  bsr  $751a */
L_00CB66:
    RTS();                                               /* 00CB66  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xCA02U: goto L_00CA02;
        case 0xCA0AU: goto L_00CA0A;
        case 0xCA2AU: goto L_00CA2A;
        case 0xCA32U: goto L_00CA32;
        case 0xCA3CU: goto L_00CA3C;
        case 0xCA4CU: goto L_00CA4C;
        case 0xCA5CU: goto L_00CA5C;
        case 0xCA8EU: goto L_00CA8E;
        case 0xCAA8U: goto L_00CAA8;
        case 0xCAACU: goto L_00CAAC;
        case 0xCAB8U: goto L_00CAB8;
        case 0xCAE0U: goto L_00CAE0;
        case 0xCAEAU: goto L_00CAEA;
        case 0xCAFAU: goto L_00CAFA;
        case 0xCB0AU: goto L_00CB0A;
        case 0xCB3EU: goto L_00CB3E;
        case 0xCB58U: goto L_00CB58;
        case 0xCB5CU: goto L_00CB5C;
        case 0xCB66U: goto L_00CB66;
        }
    }
}

/* 0086D8  game routine */
void sub_0086D8(void)
{
    /* 0086D8  bra  $79dc */
    { wait_vbl(); return; }
}

/* 008700  game routine */
void sub_008700(void)
{
    SETB(d0, d0 | 0);                                    /* 008700  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008704  ori.b  #$0,d0 */
    /* 008708  move.b  (a0)+,d4 */
    SETB(d4, rd8(a0));
    a0 += 1;
    /* 00870A  move.b  (a0)+,d4 */
    SETB(d4, rd8(a0));
    a0 += 1;
    /* 00870C  move.b  (a0)+,d4 */
    SETB(d4, rd8(a0));
    a0 += 1;
    /* 00870E  ori.b  #$36,(a0)+ */
    {
        uint32_t s = 0x36;
        uint32_t ea = a0;
        a0 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    SETB(d1, (d0 & 0xFF));                               /* 008712  move.b  d0,d1 */
    SETB(d0, d0 | 0);                                    /* 008714  ori.b  #$0,d0 */
    wr8(a4 + 0x2C7E, rd8(a4 + 0x2C7E) | 0x7E);           /* 008718  ori.b  #$7e,$2c7e(a4) */
    d6 = rd32(a4 + 0x10);                                /* 00871E  move.l  $10(a4),d6 */
    d6 = 0xFFFFFFE0;                                     /* 008722  moveq  #-32,d6 */
    d6 = 0xE;                                            /* 008724  moveq  #14,d6 */
    /* 008726  moveq  #16,d6 */
    d6 = 0x10;
    FL_LOGIC(d6, 0xFFFFFFFF, 0x80000000);
    MISS(0x8728U);
}

/* 0088FC  game routine */
void sub_0088FC(void)
{
    /* 0088FC  bra  $382e */
    { sub_00382E(); return; }
}

/* 008900  game routine */
void sub_008900(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_008900;
    for (;;) {
        BUDGET_CHECK(0x2CE0U);
L_002CE0:
        a0 = V_0170;                                         /* 002CE0  lea  $170(a5),a0 */
        /* 002CE4  move.l  #$4e20,(a0)+ */
        {
            uint32_t v = 0x4E20;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CEA  move.l  #$4650,(a0)+ */
        {
            uint32_t v = 0x4650;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CF0  move.l  #$3e80,(a0)+ */
        {
            uint32_t v = 0x3E80;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CF6  move.l  #$36b0,(a0)+ */
        {
            uint32_t v = 0x36B0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CFC  move.l  #$2ee0,(a0)+ */
        {
            uint32_t v = 0x2EE0;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002D02  move.l  #$2710,(a0)+ */
        {
            uint32_t v = 0x2710;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002D08  move.l  #$1f40,(a0)+ */
        {
            uint32_t v = 0x1F40;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002D0E  move.l  #$1770,(a0)+ */
        {
            uint32_t v = 0x1770;
            wr32(a0, v);
            a0 += 4;
        }
        /* 002D14  move.l  #$fa0,(a0)+ */
        {
            uint32_t v = 0xFA0;
            wr32(a0, v);
            a0 += 4;
        }
        wr16(V_01BE, 3);                                     /* 002D1A  move.w  #$3,$1be(a5) */
        wr16(V_01BC, 3);                                     /* 002D20  move.w  #$3,$1bc(a5) */
        wr16(V_0164, 0);                                     /* 002D26  clr.w  $164(a5) */
        wr16(V_0166, 0);                                     /* 002D2A  clr.w  $166(a5) */
        wr16(V_0168, 0);                                     /* 002D2E  move.w  #$0,$168(a5) */
        wr16(V_016A, 2);                                     /* 002D34  move.w  #$2,$16a(a5) */
        wr16(V_0276, 1);                                     /* 002D3A  move.w  #$1,$276(a5) */
        wr32(V_016C, 0x28000);                               /* 002D40  move.l  #$28000,$16c(a5) */
        CALL(sub_0086B4, 0x2D4CU);                           /* 002D48  bsr  $86b4 */
L_002D4C:
        CALL(sub_0086B0, 0x2D50U);                           /* 002D4C  bsr  $86b0 */
L_002D50:
        a1 = 0x28000;                                        /* 002D50  lea  $28000.l,a1 */
        SETW(d7, 0x9F);                                      /* 002D56  move.w  #$9f,d7 */
        do {
            BUDGET_CHECK(0x2D5AU);
L_002D5A:
            /* 002D5A  cmpi.b  #$6e,(a1)+ */
            {
                uint32_t s = 0x6E;
                uint32_t ea = a1;
                a1 += 1;
                uint32_t d = rd8(ea);
                uint32_t r = (d - s) & 0xFF;
                fz = (r == 0);
                fc = (s > d);
            }
            /* 002D5E  bhi  $2d98 */
            if (COND(2)) {
                goto L_002D98;
            }
L_002D60:
            /* 002D60  dbra  d7,$2d5a */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_002D64:
        a0 = 0x3094;                                         /* 002D64  lea  $3094(pc),a0 */
        a2 = 0x3172;                                         /* 002D68  lea  $3172(pc),a2 */
        a1 = 0x28000;                                        /* 002D6C  lea  $28000.l,a1 */
        do {
            BUDGET_CHECK(0x2D72U);
L_002D72:
            /* 002D72  move.w  (a1)+,(a0)+ */
            {
                uint32_t v = rd16(a1);
                a1 += 2;
                wr16(a0, v);
                a0 += 2;
            }
            /* 002D74  cmpa.l  a2,a0 */
            /* 002D76  bcs  $2d72 */
        } while (a0 < a2);
L_002D78:
        /* 002D78  move.l  (a1)+,$170(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_0170, v);
        }
        /* 002D7C  move.l  (a1)+,$174(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_0174, v);
        }
        /* 002D80  move.l  (a1)+,$178(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_0178, v);
        }
        /* 002D84  move.l  (a1)+,$17c(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_017C, v);
        }
        /* 002D88  move.l  (a1)+,$180(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_0180, v);
        }
        /* 002D8C  move.l  (a1)+,$184(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_0184, v);
        }
        /* 002D90  move.l  (a1)+,$188(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_0188, v);
        }
        /* 002D94  move.l  (a1)+,$18c(a5) */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(V_018C, v);
        }
L_002D98:
        RTS();                                               /* 002D98  rts */
L_008900:
        /* 008900  bra  $2ce0 */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2CE0U: goto L_002CE0;
        case 0x2D4CU: goto L_002D4C;
        case 0x2D50U: goto L_002D50;
        case 0x2D5AU: goto L_002D5A;
        case 0x2D60U: goto L_002D60;
        case 0x2D64U: goto L_002D64;
        case 0x2D72U: goto L_002D72;
        case 0x2D78U: goto L_002D78;
        }
    }
}

/* 008904  game routine */
void sub_008904(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008904  bra  $eab8 */
L_00EAB8:
    /* 00EAB8  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    set_sr(get_sr() | 0x700U);                           /* 00EABA  ori  #$700,sr */
    wr16(a6, 0x80);                                      /* 00EABE  move.w  #$80,(a6) */
    do {
        BUDGET_CHECK(0xEAC2U);
L_00EAC2:
        CALL(sub_00EA10, 0xEAC6U);                           /* 00EAC2  bsr  $ea10 */
L_00EAC6:
        fz = !(d0 & 0x80);                                   /* 00EAC6  btst  #$7,d0 */
        /* 00EACA  bne  $eac2 */
    } while (COND(6));
L_00EACC:
    /* 00EACC  moveq  #7,d0 */
    d0 = 7;
    FL_LOGIC(d0, 0xFFFFFFFF, 0x80000000);
    CALL(sub_00EA8C, 0xEAD0U);                           /* 00EACE  bsr  $ea8c */
L_00EAD0:
    /* 00EAD0  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_00EAD2:
    CALL(sub_0077F6, 0xEAD6U);                           /* 00EAD2  bsr  $77f6 */
L_00EAD6:
    RTS();                                               /* 00EAD6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEAB8U: goto L_00EAB8;
        case 0xEAC2U: goto L_00EAC2;
        case 0xEAC6U: goto L_00EAC6;
        case 0xEACCU: goto L_00EACC;
        case 0xEAD0U: goto L_00EAD0;
        case 0xEAD2U: goto L_00EAD2;
        case 0xEAD6U: goto L_00EAD6;
        }
    }
}

/* 008910  game routine */
void sub_008910(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008910  bra  $106ec */
L_0106EC:
    SETW(d0, rd16(V_021C));                              /* 0106EC  move.w  $21c(a5),d0 */
    a0 = 0x10748;                                        /* 0106F0  lea  $10748(pc),a0 */
    d0 = (d0 & 0xFFFF) * 8;                              /* 0106F4  mulu.w  #$8,d0 */
    a0 = a0 + SX16(d0);                                  /* 0106F8  lea  $0(a0,d0.w),a0 */
    /* 0106FC  move.w  (a0)+,d0 */
    SETW(d0, rd16(a0));
    a0 += 2;
    CALL(sub_0107A8, 0x10702U);                          /* 0106FE  bsr  $107a8 */
L_010702:
    /* 010702  move.w  (a0)+,d0 */
    SETW(d0, rd16(a0));
    a0 += 2;
    CALL(sub_010826, 0x10708U);                          /* 010704  bsr  $10826 */
L_010708:
    /* 010708  move.w  (a0)+,d0 */
    SETW(d0, rd16(a0));
    a0 += 2;
    CALL(sub_0108AE, 0x1070EU);                          /* 01070A  bsr  $108ae */
L_01070E:
    /* 01070E  move.w  (a0)+,d0 */
    SETW(d0, rd16(a0));
    a0 += 2;
    a0 = 0x8C24;                                         /* 010710  lea  $8c24(pc),a0 */
    wr16(a0 + 0x1C, (d0 & 0xFFFF));                      /* 010714  move.w  d0,$1c(a0) */
    CALL(sub_00AF9A, 0x1071CU);                          /* 010718  bsr  $af9a */
L_01071C:
    SETW(d0, rd16(V_021C));                              /* 01071C  move.w  $21c(a5),d0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 1, 16));      /* 010720  lsl.w  #1,d0 */
    a0 = 0x10730;                                        /* 010722  lea  $10730(pc),a0 */
    SETW(d0, rd16(a0 + SX16(d0)));                       /* 010726  move.w  $0(a0,d0.w),d0 */
    wr16(V_024C, (d0 & 0xFFFF));                         /* 01072A  move.w  d0,$24c(a5) */
    RTS();                                               /* 01072E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x106ECU: goto L_0106EC;
        case 0x10702U: goto L_010702;
        case 0x10708U: goto L_010708;
        case 0x1070EU: goto L_01070E;
        case 0x1071CU: goto L_01071C;
        }
    }
}

/* 008914  game routine */
void sub_008914(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008914  bra  $bb34 */
L_00BB34:
    wr16(V_0164, 0);                                     /* 00BB34  clr.w  $164(a5) */
    wr16(V_0166, 0);                                     /* 00BB38  move.w  #$0,$166(a5) */
    wr16(V_0168, 0x27);                                  /* 00BB3E  move.w  #$27,$168(a5) */
    wr16(V_016A, 1);                                     /* 00BB44  move.w  #$1,$16a(a5) */
    wr16(V_0276, 0x3A);                                  /* 00BB4A  move.w  #$3a,$276(a5) */
    wr32(V_016C, 0x5F4AE);                               /* 00BB50  move.l  #$5f4ae,$16c(a5) */
    CALL(sub_00EB26, 0xBB5CU);                           /* 00BB58  bsr  $eb26 */
L_00BB5C:
    CALL(sub_00E92A, 0xBB60U);                           /* 00BB5C  bsr  $e92a */
L_00BB60:
    CALL(sub_007626, 0xBB64U);                           /* 00BB60  bsr  $7626 */
L_00BB64:
    a1 = 0x8C24;                                         /* 00BB64  lea  $8c24(pc),a1 */
    CALL(sub_0075E2, 0xBB6CU);                           /* 00BB68  bsr  $75e2 */
L_00BB6C:
    a4 = 0x5F4CB;                                        /* 00BB6C  lea  $5f4cb.l,a4 */
    a6 = 0x5647E;                                        /* 00BB72  lea  $5647e.l,a6 */
    a1 = 0x667E8;                                        /* 00BB78  lea  $667e8.l,a1 */
    a3 = 0x14000;                                        /* 00BB7E  lea  $14000.l,a3 */
    CALL(sub_00FC88, 0xBB88U);                           /* 00BB84  bsr  $fc88 */
L_00BB88:
    wr16(V_01EC, 0xA0);                                  /* 00BB88  move.w  #$a0,$1ec(a5) */
    wr16(V_01EE, 0x7D);                                  /* 00BB8E  move.w  #$7d,$1ee(a5) */
    d0 = 1;                                              /* 00BB94  moveq  #1,d0 */
    CALL(sub_0074F4, 0xBB9AU);                           /* 00BB96  bsr  $74f4 */
L_00BB9A:
    CALL(sub_007626, 0xBB9EU);                           /* 00BB9A  bsr  $7626 */
L_00BB9E:
    a1 = 0x8C44;                                         /* 00BB9E  lea  $8c44(pc),a1 */
    CALL(sub_0075E2, 0xBBA6U);                           /* 00BBA2  bsr  $75e2 */
L_00BBA6:
    a3 = 0x57C9A;                                        /* 00BBA6  lea  $57c9a.l,a3 */
    a4 = 0x20000;                                        /* 00BBAC  lea  $20000.l,a4 */
    CALL(sub_008494, 0xBBB6U);                           /* 00BBB2  bsr  $8494 */
L_00BBB6:
    d0 = 0x20000;                                        /* 00BBB6  move.l  #$20000,d0 */
    CALL(sub_008920, 0xBBC0U);                           /* 00BBBC  bsr  $8920 */
L_00BBC0:
    CALL(sub_007558, 0xBBC4U);                           /* 00BBC0  bsr  $7558 */
L_00BBC4:
    CALL(sub_00C97C, 0xBBC8U);                           /* 00BBC4  bsr  $c97c */
L_00BBC8:
    wr16(V_026C, 8);                                     /* 00BBC8  move.w  #$8,$26c(a5) */
    a0 = 0xC9D0;                                         /* 00BBCE  lea  $c9d0(pc),a0 */
    wr16(V_0266, 0xB40);                                 /* 00BBD2  move.w  #$b40,$266(a5) */
    wr16(V_0268, 0xC);                                   /* 00BBD8  move.w  #$c,$268(a5) */
    wr16(V_026A, 0xC);                                   /* 00BBDE  move.w  #$c,$26a(a5) */
    CALL(sub_008584, 0xBBE8U);                           /* 00BBE4  bsr  $8584 */
L_00BBE8:
    a1 = 0xC70E;                                         /* 00BBE8  lea  $c70e(pc),a1 */
    CALL(sub_0075E2, 0xBBF0U);                           /* 00BBEC  bsr  $75e2 */
L_00BBF0:
    wr8(V_01FE, 0xFF);                                   /* 00BBF0  st  $1fe(a5) */
    wr16(V_0214, 0xFFFF);                                /* 00BBF4  move.w  #$ffff,$214(a5) */
    wr16(V_1DC2, 0);                                     /* 00BBFA  move.w  #$0,$1dc2(a5) */
    a0 = V_1CF8;                                         /* 00BC00  lea  $1cf8(a5),a0 */
    a1 = 0xC7BA;                                         /* 00BC04  lea  $c7ba(pc),a1 */
    SETW(d7, 0xB);                                       /* 00BC08  move.w  #$b,d7 */
    d6 = 0;                                              /* 00BC0C  moveq  #0,d6 */
    a2 = V_023A;                                         /* 00BC0E  lea  $23a(a5),a2 */
    do {
        BUDGET_CHECK(0xBC12U);
L_00BC12:
        wr16(a0 + 0x6, 0);                                   /* 00BC12  clr.w  $6(a0) */
        /* 00BC16  tst.b  $0(a2,d6.w) */
        /* 00BC1A  bne  $bc36 */
        if (rd8(a2 + SX16(d6)) == 0) {
L_00BC1C:
            wr16(a0, rd16(a1));                                  /* 00BC1C  move.w  (a1),(a0) */
            wr16(a0 + 0x2, rd16(a1 + 0x2));                      /* 00BC1E  move.w  $2(a1),$2(a0) */
            wr16(a0 + 0x4, 0);                                   /* 00BC24  move.w  #$0,$4(a0) */
            wr16(a0 + 0x6, 0xFFFF);                              /* 00BC2A  move.w  #$ffff,$6(a0) */
            wr16(a0 + 0xC, 6);                                   /* 00BC30  move.w  #$6,$c(a0) */
        }
        a0 = a0 + 0x10;                                      /* 00BC36  lea  $10(a0),a0 */
        a1 += 4;                                             /* 00BC3A  adda.l  #$4,a1 */
        /* 00BC40  addq.w  #1,d6 */
        {
            uint32_t s = 1;
            uint32_t d = (d6 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d6, r);
            fx = (r < s);
        }
        /* 00BC42  dbra  d7,$bc12 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BC46:
    /* 00BC46  tst.w  $2da(a5) */
    /* 00BC4A  bne  $bc52 */
    if (rd16(V_02DA) == 0) {
        SETB(d0, rd8(V_0388));                               /* 00BC4C  move.b  $388(a5),d0 */
        /* 00BC50  bra  $bc56 */
    } else {
L_00BC52:
        SETB(d0, rd8(V_02EC));                               /* 00BC52  move.b  $2ec(a5),d0 */
    }
    /* 00BC56  tst.b  d0 */
    /* 00BC58  beq  $bc64 */
    if ((d0 & 0xFF) != 0) {
L_00BC5A:
        /* 00BC5A  tst.w  $214(a5) */
        /* 00BC5E  bmi  $bc64 */
        if ((int16_t)rd16(V_0214) < 0) {
            goto L_00BC64;
        }
L_00BC60:
        /* 00BC60  bra  $bcec */
    } else {
L_00BC64:
        CALL(sub_00BDC8, 0xBC68U);                           /* 00BC64  bsr  $bdc8 */
L_00BC68:
        CALL(sub_00BE9A, 0xBC6CU);                           /* 00BC68  bsr  $be9a */
L_00BC6C:
        CALL(sub_00C81A, 0xBC70U);                           /* 00BC6C  bsr  $c81a */
L_00BC70:
        CALL(wait_vbl, 0xBC74U);                             /* 00BC70  bsr  $79dc */
L_00BC74:
        CALL(sub_00BE6A, 0xBC78U);                           /* 00BC74  bsr  $be6a */
L_00BC78:
        CALL(sub_00C906, 0xBC7CU);                           /* 00BC78  bsr  $c906 */
L_00BC7C:
        CALL(sub_00BF54, 0xBC80U);                           /* 00BC7C  bsr  $bf54 */
L_00BC80:
        /* 00BC80  tst.w  d7 */
        /* 00BC82  bmi  $bcae */
        if ((int16_t)(d7 & 0xFFFF) >= 0) {
L_00BC84:
            wr16(V_1DC2, 0);                                     /* 00BC84  clr.w  $1dc2(a5) */
            a0 = V_023A;                                         /* 00BC88  lea  $23a(a5),a0 */
            /* 00BC8C  tst.b  $0(a0,d7.w) */
            /* 00BC90  bne  $bc46 */
            if (rd8(a0 + SX16(d7)) != 0) {
                BUDGET_CHECK(0xBC46U);
                goto L_00BC46;
            }
L_00BC92:
            /* 00BC92  cmp.w  $214(a5),d7 */
            /* 00BC96  beq  $bc46 */
            if ((d7 & 0xFFFF) == rd16(V_0214)) {
                BUDGET_CHECK(0xBC46U);
                goto L_00BC46;
            }
L_00BC98:
            CALL(sub_00C9A2, 0xBC9CU);                           /* 00BC98  bsr  $c9a2 */
L_00BC9C:
            wr16(V_0214, (d7 & 0xFFFF));                         /* 00BC9C  move.w  d7,$214(a5) */
            SETW(d0, (d7 & 0xFFFF));                             /* 00BCA0  move.w  d7,d0 */
            CALL(sub_00BD22, 0xBCA6U);                           /* 00BCA2  bsr  $bd22 */
L_00BCA6:
            SETW(d0, (d7 & 0xFFFF));                             /* 00BCA6  move.w  d7,d0 */
            CALL(sub_00C94C, 0xBCACU);                           /* 00BCA8  bsr  $c94c */
L_00BCAC:
            /* 00BCAC  bra  $bc46 */
            BUDGET_CHECK(0xBC46U);
            goto L_00BC46;
        }
L_00BCAE:
        /* 00BCAE  tst.w  $1dc2(a5) */
        /* 00BCB2  bne  $bce2 */
        if (rd16(V_1DC2) == 0) {
L_00BCB4:
            wr16(V_1DC2, 0xFFFF);                                /* 00BCB4  move.w  #$ffff,$1dc2(a5) */
            CALL(sub_00C97C, 0xBCBEU);                           /* 00BCBA  bsr  $c97c */
L_00BCBE:
            CALL(sub_00BD4C, 0xBCC2U);                           /* 00BCBE  bsr  $bd4c */
L_00BCC2:
            a0 = 0xC9D0;                                         /* 00BCC2  lea  $c9d0(pc),a0 */
            wr16(V_026C, 8);                                     /* 00BCC6  move.w  #$8,$26c(a5) */
            wr16(V_0266, 0xB40);                                 /* 00BCCC  move.w  #$b40,$266(a5) */
            wr16(V_0268, 0xC);                                   /* 00BCD2  move.w  #$c,$268(a5) */
            wr16(V_026A, 0xC);                                   /* 00BCD8  move.w  #$c,$26a(a5) */
            CALL(sub_008584, 0xBCE2U);                           /* 00BCDE  bsr  $8584 */
        }
L_00BCE2:
        wr16(V_0214, 0xFFFF);                                /* 00BCE2  move.w  #$ffff,$214(a5) */
        /* 00BCE8  bra  $bc46 */
        BUDGET_CHECK(0xBC46U);
        goto L_00BC46;
    }
L_00BCEC:
    wr16(V_021C, rd16(V_0214));                          /* 00BCEC  move.w  $214(a5),$21c(a5) */
    SETW(d0, rd16(V_021C));                              /* 00BCF2  move.w  $21c(a5),d0 */
    a0 = 0xC7EA;                                         /* 00BCF6  lea  $c7ea(pc),a0 */
    SETW(d0, (d0 & 0xFFFF) << 2);                        /* 00BCFA  lsl.w  #2,d0 */
    d0 = rd32(a0 + SX16(d0));                            /* 00BCFC  move.l  $0(a0,d0.w),d0 */
    /* 00BD00  tst.w  $2da(a5) */
    /* 00BD04  beq  $bd0e */
    if (rd16(V_02DA) != 0) {
L_00BD06:
        /* 00BD06  tst.w  $376(a5) */
        /* 00BD0A  beq  $bd0e */
        if (rd16(V_0376) != 0) {
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 00BD0C  lsr.w  #1,d0 */
        }
    }
L_00BD0E:
    SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 00BD0E  lsr.w  #1,d0 */
    wr32(V_PLAYER1, rd32(V_PLAYER1) + d0);               /* 00BD10  add.l  d0,$332(a5) */
    /* 00BD14  add.l  d0,$3ce(a5) */
    {
        uint32_t s = d0;
        uint32_t d = rd32(V_PLAYER2);
        uint32_t r = (d + s) & 0xFFFFFFFF;
        wr32(V_PLAYER2, r);
        fx = (r < s);
    }
    CALL(sub_007626, 0xBD1CU);                           /* 00BD18  bsr  $7626 */
L_00BD1C:
    wr8(V_01FE, 0);                                      /* 00BD1C  sf  $1fe(a5) */
    RTS();                                               /* 00BD20  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBB34U: goto L_00BB34;
        case 0xBB5CU: goto L_00BB5C;
        case 0xBB60U: goto L_00BB60;
        case 0xBB64U: goto L_00BB64;
        case 0xBB6CU: goto L_00BB6C;
        case 0xBB88U: goto L_00BB88;
        case 0xBB9AU: goto L_00BB9A;
        case 0xBB9EU: goto L_00BB9E;
        case 0xBBA6U: goto L_00BBA6;
        case 0xBBB6U: goto L_00BBB6;
        case 0xBBC0U: goto L_00BBC0;
        case 0xBBC4U: goto L_00BBC4;
        case 0xBBC8U: goto L_00BBC8;
        case 0xBBE8U: goto L_00BBE8;
        case 0xBBF0U: goto L_00BBF0;
        case 0xBC12U: goto L_00BC12;
        case 0xBC1CU: goto L_00BC1C;
        case 0xBC46U: goto L_00BC46;
        case 0xBC52U: goto L_00BC52;
        case 0xBC5AU: goto L_00BC5A;
        case 0xBC60U: goto L_00BC60;
        case 0xBC64U: goto L_00BC64;
        case 0xBC68U: goto L_00BC68;
        case 0xBC6CU: goto L_00BC6C;
        case 0xBC70U: goto L_00BC70;
        case 0xBC74U: goto L_00BC74;
        case 0xBC78U: goto L_00BC78;
        case 0xBC7CU: goto L_00BC7C;
        case 0xBC80U: goto L_00BC80;
        case 0xBC84U: goto L_00BC84;
        case 0xBC92U: goto L_00BC92;
        case 0xBC98U: goto L_00BC98;
        case 0xBC9CU: goto L_00BC9C;
        case 0xBCA6U: goto L_00BCA6;
        case 0xBCACU: goto L_00BCAC;
        case 0xBCAEU: goto L_00BCAE;
        case 0xBCB4U: goto L_00BCB4;
        case 0xBCBEU: goto L_00BCBE;
        case 0xBCC2U: goto L_00BCC2;
        case 0xBCE2U: goto L_00BCE2;
        case 0xBCECU: goto L_00BCEC;
        case 0xBD06U: goto L_00BD06;
        case 0xBD0EU: goto L_00BD0E;
        case 0xBD1CU: goto L_00BD1C;
        }
    }
}

/* 00891C  game routine */
void sub_00891C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_00891C;
    for (;;) {
        BUDGET_CHECK(0x5DCCU);
L_005DCC:
        d0 = 0x26;                                           /* 005DCC  moveq  #38,d0 */
        a0 = 0x5E60A;                                        /* 005DCE  lea  $5e60a.l,a0 */
        a4 = a0;                                             /* 005DD4  movea.l  a0,a4 */
        a4 += 0x2838;                                        /* 005DD6  adda.l  #$2838,a4 */
        a1 = a4;                                             /* 005DDC  movea.l  a4,a1 */
        do {
            BUDGET_CHECK(0x5DDEU);
L_005DDE:
            d7 = 0x15;                                           /* 005DDE  moveq  #21,d7 */
            do {
                BUDGET_CHECK(0x5DE0U);
L_005DE0:
                d3 = 0;                                              /* 005DE0  moveq  #0,d3 */
                d4 = 0;                                              /* 005DE2  moveq  #0,d4 */
                d5 = 0;                                              /* 005DE4  moveq  #0,d5 */
                /* 005DE6  move.w  (a0)+,d3 */
                SETW(d3, rd16(a0));
                a0 += 2;
                /* 005DE8  move.w  (a0)+,d4 */
                SETW(d4, rd16(a0));
                a0 += 2;
                /* 005DEA  move.w  (a0)+,d5 */
                SETW(d5, rd16(a0));
                a0 += 2;
                SETW(d2, 6);                                         /* 005DEC  move.w  #$6,d2 */
                do {
                    BUDGET_CHECK(0x5DF0U);
L_005DF0:
                    d3 = shift_op(3, 0, d3, 2, 32);                      /* 005DF0  ror.l  #2,d3 */
                    d4 = shift_op(3, 0, d4, 2, 32);                      /* 005DF2  ror.l  #2,d4 */
                    d5 = shift_op(3, 0, d5, 2, 32);                      /* 005DF4  ror.l  #2,d5 */
                    wr16(a1, (d3 & 0xFFFF));                             /* 005DF6  move.w  d3,(a1) */
                    wr16(a1 + 0x2, (d4 & 0xFFFF));                       /* 005DF8  move.w  d4,$2(a1) */
                    wr16(a1 + 0x4, (d5 & 0xFFFF));                       /* 005DFC  move.w  d5,$4(a1) */
                    d3 = (d3 << 16) | (d3 >> 16);                        /* 005E00  swap  d3 */
                    d4 = (d4 << 16) | (d4 >> 16);                        /* 005E02  swap  d4 */
                    d5 = (d5 << 16) | (d5 >> 16);                        /* 005E04  swap  d5 */
                    wr16(a1 + 0x6, (d3 & 0xFFFF));                       /* 005E06  move.w  d3,$6(a1) */
                    wr16(a1 + 0x8, (d4 & 0xFFFF));                       /* 005E0A  move.w  d4,$8(a1) */
                    wr16(a1 + 0xA, (d5 & 0xFFFF));                       /* 005E0E  move.w  d5,$a(a1) */
                    d3 = (d3 << 16) | (d3 >> 16);                        /* 005E12  swap  d3 */
                    d4 = (d4 << 16) | (d4 >> 16);                        /* 005E14  swap  d4 */
                    d5 = (d5 << 16) | (d5 >> 16);                        /* 005E16  swap  d5 */
                    a1 += 0x2838;                                        /* 005E18  adda.l  #$2838,a1 */
                    /* 005E1E  dbra  d2,$5df0 */
                    SETW(d2, d2 - 1);
                } while ((d2 & 0xFFFF) != 0xFFFF);
L_005E22:
                a4 = a4 + 0xC;                                       /* 005E22  lea  $c(a4),a4 */
                a1 = a4;                                             /* 005E26  movea.l  a4,a1 */
                /* 005E28  dbra  d7,$5de0 */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
L_005E2C:
            /* 005E2C  dbra  d0,$5dde */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
L_005E30:
        RTS();                                               /* 005E30  rts */
L_00891C:
        /* 00891C  bra  $5dcc */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x5DCCU: goto L_005DCC;
        case 0x5DDEU: goto L_005DDE;
        case 0x5DE0U: goto L_005DE0;
        case 0x5DF0U: goto L_005DF0;
        case 0x5E22U: goto L_005E22;
        case 0x5E2CU: goto L_005E2C;
        case 0x5E30U: goto L_005E30;
        }
    }
}

/* 008920  game routine */
void sub_008920(void)
{
    /* 008920  bra  $7b2a */
    { sub_007B2A(); return; }
}

/* 008924  game routine */
void sub_008924(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008924  bra  $f866 */
    /* 00F866  tst.w  $2da(a5) */
    /* 00F86A  bne  $f872 */
    if (rd16(V_02DA) == 0) {
        wr16(V_0370, 0);                                     /* 00F86C  move.w  #$0,$370(a5) */
    }
    /* 00F872  tst.w  $376(a5) */
    /* 00F876  bne  $f87e */
    if (rd16(V_0376) == 0) {
        wr16(V_040C, 0);                                     /* 00F878  move.w  #$0,$40c(a5) */
    }
    d0 = 6;                                              /* 00F87E  moveq  #6,d0 */
    CALL(sub_041BCC, 0xF886U);                           /* 00F880  jsr  $41bcc.l */
L_00F886:
    d0 = 0x118C30;                                       /* 00F886  move.l  #$118c30,d0 */
    CALL(sub_00FC60, 0xF890U);                           /* 00F88C  bsr  $fc60 */
L_00F890:
    CALL(sub_00FA62, 0xF894U);                           /* 00F890  bsr  $fa62 */
L_00F894:
    a0 = 0x20000;                                        /* 00F894  lea  $20000.l,a0 */
    CALL(sub_00FC66, 0xF89EU);                           /* 00F89A  bsr  $fc66 */
L_00F89E:
    a0 = 0x18000;                                        /* 00F89E  lea  $18000.l,a0 */
    CALL(sub_00FC66, 0xF8A8U);                           /* 00F8A4  bsr  $fc66 */
L_00F8A8:
    wr16(V_026C, 0xE);                                   /* 00F8A8  move.w  #$e,$26c(a5) */
    wr16(0xFF825C, 0x777);                               /* 00F8AE  move.w  #$777,$ff825c.l */
    wr16(V_0266, 0x820);                                 /* 00F8B6  move.w  #$820,$266(a5) */
    wr16(V_0268, 8);                                     /* 00F8BC  move.w  #$8,$268(a5) */
    wr16(V_026A, 8);                                     /* 00F8C2  move.w  #$8,$26a(a5) */
    wr16(V_026C, 0xE);                                   /* 00F8C8  move.w  #$e,$26c(a5) */
    a0 = 0xFABE;                                         /* 00F8CE  lea  $fabe(pc),a0 */
    CALL(sub_008928, 0xF8D6U);                           /* 00F8D2  bsr  $8928 */
L_00F8D6:
    wr16(V_0266, 0xF00);                                 /* 00F8D6  move.w  #$f00,$266(a5) */
    wr16(V_0268, 8);                                     /* 00F8DC  move.w  #$8,$268(a5) */
    wr16(V_026A, 8);                                     /* 00F8E2  move.w  #$8,$26a(a5) */
    a0 = 0xFAD3;                                         /* 00F8E8  lea  $fad3(pc),a0 */
    CALL(sub_008928, 0xF8F0U);                           /* 00F8EC  bsr  $8928 */
L_00F8F0:
    wr16(V_0266, 0x15E0);                                /* 00F8F0  move.w  #$15e0,$266(a5) */
    wr16(V_0268, 8);                                     /* 00F8F6  move.w  #$8,$268(a5) */
    wr16(V_026A, 8);                                     /* 00F8FC  move.w  #$8,$26a(a5) */
    a0 = 0xFAE9;                                         /* 00F902  lea  $fae9(pc),a0 */
    CALL(sub_008928, 0xF90AU);                           /* 00F906  bsr  $8928 */
L_00F90A:
    CALL(sub_00F91E, 0xF90EU);                           /* 00F90A  bsr  $f91e */
L_00F90E:
    wr16(V_01DA, 0x64);                                  /* 00F90E  move.w  #$64,$1da(a5) */
    CALL(sub_00FC32, 0xF918U);                           /* 00F914  bsr  $fc32 */
L_00F918:
    CALL(sub_00F9C8, 0xF91CU);                           /* 00F918  bsr  $f9c8 */
L_00F91C:
    RTS();                                               /* 00F91C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF886U: goto L_00F886;
        case 0xF890U: goto L_00F890;
        case 0xF894U: goto L_00F894;
        case 0xF89EU: goto L_00F89E;
        case 0xF8A8U: goto L_00F8A8;
        case 0xF8D6U: goto L_00F8D6;
        case 0xF8F0U: goto L_00F8F0;
        case 0xF90AU: goto L_00F90A;
        case 0xF90EU: goto L_00F90E;
        case 0xF918U: goto L_00F918;
        case 0xF91CU: goto L_00F91C;
        }
    }
}

/* 008928  game routine */
void sub_008928(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_008928;
L_0034F0:
    SETW(d0, 0);                                         /* 0034F0  clr.w  d0 */
    /* 0034F2  move.b  (a0)+,d0 */
    {
        uint32_t v = rd8(a0);
        a0 += 1;
        SETB(d0, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 0034F4  beq  $351c */
    if (COND(7)) { sub_00351C(); return; }
    CALL(sub_0034FA, 0x34F8U);                           /* 0034F6  bsr  $34fa */
L_0034F8:
    /* 0034F8  bra  $34f0 */
    BUDGET_CHECK(0x34F0U);
    goto L_0034F0;
L_008928:
    /* 008928  bra  $34f0 */
    BUDGET_CHECK(0x34F0U);
    goto L_0034F0;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x34F0U: goto L_0034F0;
        case 0x34F8U: goto L_0034F8;
        }
    }
}

/* 008934  game routine */
void sub_008934(void)
{
    /* 008934  bra  $8584 */
    { sub_008584(); return; }
}

/* 008938  game routine */
void sub_008938(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008938  bra  $baba */
L_00BABA:
    SETW(d0, 0xB);                                       /* 00BABA  move.w  #$b,d0 */
    a0 = 0x49A3A;                                        /* 00BABE  lea  $49a3a.l,a0 */
    a4 = a0;                                             /* 00BAC4  movea.l  a0,a4 */
    a4 += 0xC00;                                         /* 00BAC6  adda.l  #$c00,a4 */
    a1 = a4;                                             /* 00BACC  movea.l  a4,a1 */
    do {
        BUDGET_CHECK(0xBACEU);
L_00BACE:
        d7 = 0xF;                                            /* 00BACE  moveq  #15,d7 */
        do {
            BUDGET_CHECK(0xBAD0U);
L_00BAD0:
            d3 = 0;                                              /* 00BAD0  moveq  #0,d3 */
            d4 = 0;                                              /* 00BAD2  moveq  #0,d4 */
            d5 = 0;                                              /* 00BAD4  moveq  #0,d5 */
            d6 = 0;                                              /* 00BAD6  moveq  #0,d6 */
            /* 00BAD8  move.w  (a0)+,d3 */
            SETW(d3, rd16(a0));
            a0 += 2;
            /* 00BADA  move.w  (a0)+,d4 */
            SETW(d4, rd16(a0));
            a0 += 2;
            /* 00BADC  move.w  (a0)+,d5 */
            SETW(d5, rd16(a0));
            a0 += 2;
            /* 00BADE  move.w  (a0)+,d6 */
            SETW(d6, rd16(a0));
            a0 += 2;
            SETW(d2, 6);                                         /* 00BAE0  move.w  #$6,d2 */
            do {
                BUDGET_CHECK(0xBAE4U);
L_00BAE4:
                d3 = shift_op(3, 0, d3, 2, 32);                      /* 00BAE4  ror.l  #2,d3 */
                d4 = shift_op(3, 0, d4, 2, 32);                      /* 00BAE6  ror.l  #2,d4 */
                d5 = shift_op(3, 0, d5, 2, 32);                      /* 00BAE8  ror.l  #2,d5 */
                d6 = shift_op(3, 0, d6, 2, 32);                      /* 00BAEA  ror.l  #2,d6 */
                wr16(a1, (d3 & 0xFFFF));                             /* 00BAEC  move.w  d3,(a1) */
                wr16(a1 + 0x2, (d4 & 0xFFFF));                       /* 00BAEE  move.w  d4,$2(a1) */
                wr16(a1 + 0x4, (d5 & 0xFFFF));                       /* 00BAF2  move.w  d5,$4(a1) */
                wr16(a1 + 0x6, (d6 & 0xFFFF));                       /* 00BAF6  move.w  d6,$6(a1) */
                d3 = (d3 << 16) | (d3 >> 16);                        /* 00BAFA  swap  d3 */
                d4 = (d4 << 16) | (d4 >> 16);                        /* 00BAFC  swap  d4 */
                d5 = (d5 << 16) | (d5 >> 16);                        /* 00BAFE  swap  d5 */
                d6 = (d6 << 16) | (d6 >> 16);                        /* 00BB00  swap  d6 */
                wr16(a1 + 0x8, (d3 & 0xFFFF));                       /* 00BB02  move.w  d3,$8(a1) */
                wr16(a1 + 0xA, (d4 & 0xFFFF));                       /* 00BB06  move.w  d4,$a(a1) */
                wr16(a1 + 0xC, (d5 & 0xFFFF));                       /* 00BB0A  move.w  d5,$c(a1) */
                wr16(a1 + 0xE, (d6 & 0xFFFF));                       /* 00BB0E  move.w  d6,$e(a1) */
                d3 = (d3 << 16) | (d3 >> 16);                        /* 00BB12  swap  d3 */
                d4 = (d4 << 16) | (d4 >> 16);                        /* 00BB14  swap  d4 */
                d5 = (d5 << 16) | (d5 >> 16);                        /* 00BB16  swap  d5 */
                d6 = (d6 << 16) | (d6 >> 16);                        /* 00BB18  swap  d6 */
                a1 += 0xC00;                                         /* 00BB1A  adda.l  #$c00,a1 */
                /* 00BB20  dbra  d2,$bae4 */
                SETW(d2, d2 - 1);
            } while ((d2 & 0xFFFF) != 0xFFFF);
L_00BB24:
            a4 = a4 + 0x10;                                      /* 00BB24  lea  $10(a4),a4 */
            a1 = a4;                                             /* 00BB28  movea.l  a4,a1 */
            /* 00BB2A  dbra  d7,$bad0 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_00BB2E:
        /* 00BB2E  dbra  d0,$bace */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_00BB32:
    RTS();                                               /* 00BB32  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xBABAU: goto L_00BABA;
        case 0xBACEU: goto L_00BACE;
        case 0xBAD0U: goto L_00BAD0;
        case 0xBAE4U: goto L_00BAE4;
        case 0xBB24U: goto L_00BB24;
        case 0xBB2EU: goto L_00BB2E;
        case 0xBB32U: goto L_00BB32;
        }
    }
}

/* 00893C  Draws/updates the background of the vertical stretch (256-line circular buffer) */
void vertical_background(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00893C  moveq  #0,d7 */
    HOOK(28, 0x893CU);
    d7 = 0;
    SETB(d7, rd8(V_0281));                               /* 00893E  move.b  $281(a5),d7 */
    SETW(d0, 0xFF);                                      /* 008942  move.w  #$ff,d0 */
    SETW(d0, d0 - (d7 & 0xFFFF));                        /* 008946  sub.w  d7,d0 */
    d0 = (d0 & 0xFFFF) * 0x60;                           /* 008948  mulu.w  #$60,d0 */
    a1 = 0x28000;                                        /* 00894C  lea  $28000.l,a1 */
    a1 += d0;                                            /* 008952  adda.l  d0,a1 */
    a0 = rd32(V_DRAW_BUFFER);                            /* 008954  movea.l  $28e(a5),a0 */
    a0 = a0 + 0x20;                                      /* 008958  lea  $20(a0),a0 */
    /* 00895C  cmp.w  #$c6,d7 */
    /* 008960  bls  $8968 */
    if ((d7 & 0xFFFF) > 0xC6) {
L_008962:
        SETW(d7, 0xC7);                                      /* 008962  move.w  #$c7,d7 */
        /* 008966  bra  $8978 */
        { sub_008978(); return; }
    }
L_008968:
    /* 008968  move.w  d7,-(a7) */
    {
        uint32_t v = (d7 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    CALL(sub_008978, 0x896CU);                           /* 00896A  bsr  $8978 */
L_00896C:
    a1 = 0x28000;                                        /* 00896C  lea  $28000.l,a1 */
    SETW(d7, 0xC6);                                      /* 008972  move.w  #$c6,d7 */
    /* 008976  sub.w  (a7)+,d7 */
    {
        uint32_t s = rd16(a7);
        a7 += 2;
        uint32_t d = (d7 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d7, r);
    }
    { sub_008978(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8962U: goto L_008962;
        case 0x8968U: goto L_008968;
        case 0x896CU: goto L_00896C;
        }
    }
}

/* 008978  game routine */
void sub_008978(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0276, (d7 & 0xFFFF));                         /* 008978  move.w  d7,$276(a5) */
L_00897C:
    /* 00897C  movem.l  (a1)+,d0-d7/a2-a4/a6 */
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
    d4 = rd32(ea);
    ea += 4;
    d5 = rd32(ea);
    ea += 4;
    d6 = rd32(ea);
    ea += 4;
    d7 = rd32(ea);
    ea += 4;
    a2 = rd32(ea);
    ea += 4;
    a3 = rd32(ea);
    ea += 4;
    a4 = rd32(ea);
    ea += 4;
    a6 = rd32(ea);
    ea += 4;
    a1 = ea;
    }
    /* 008980  movem.l  d0-d7/a2-a4/a6,(a0) */
    { uint32_t ea;
    ea = a0;
    wr32(ea, d0); ea += 4;
    wr32(ea, d1); ea += 4;
    wr32(ea, d2); ea += 4;
    wr32(ea, d3); ea += 4;
    wr32(ea, d4); ea += 4;
    wr32(ea, d5); ea += 4;
    wr32(ea, d6); ea += 4;
    wr32(ea, d7); ea += 4;
    wr32(ea, a2); ea += 4;
    wr32(ea, a3); ea += 4;
    wr32(ea, a4); ea += 4;
    wr32(ea, a6); ea += 4;
    }
    /* 008984  movem.l  (a1)+,d0-d7/a2-a4/a6 */
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
    d4 = rd32(ea);
    ea += 4;
    d5 = rd32(ea);
    ea += 4;
    d6 = rd32(ea);
    ea += 4;
    d7 = rd32(ea);
    ea += 4;
    a2 = rd32(ea);
    ea += 4;
    a3 = rd32(ea);
    ea += 4;
    a4 = rd32(ea);
    ea += 4;
    a6 = rd32(ea);
    ea += 4;
    a1 = ea;
    }
    /* 008988  movem.l  d0-d7/a2-a4/a6,$30(a0) */
    { uint32_t ea;
    ea = a0 + 0x30U;
    wr32(ea, d0); ea += 4;
    wr32(ea, d1); ea += 4;
    wr32(ea, d2); ea += 4;
    wr32(ea, d3); ea += 4;
    wr32(ea, d4); ea += 4;
    wr32(ea, d5); ea += 4;
    wr32(ea, d6); ea += 4;
    wr32(ea, d7); ea += 4;
    wr32(ea, a2); ea += 4;
    wr32(ea, a3); ea += 4;
    wr32(ea, a4); ea += 4;
    wr32(ea, a6); ea += 4;
    }
    a0 = a0 + 0xA0;                                      /* 00898E  lea  $a0(a0),a0 */
    /* 008992  subq.w  #1,$276(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_0276);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_0276, r);
        fn = !!(r & 0x8000);
    }
    /* 008996  bpl  $897c */
    if (COND(10)) { BUDGET_CHECK(0x897CU); goto L_00897C; }
    { sub_008998(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x897CU: goto L_00897C;
        }
    }
}

/* 008998  game routine */
void sub_008998(void)
{
    RTS();                                               /* 008998  rts */
}

/* 00899A  Reads from the level map the rows of blocks that come in from the top (format in host/level.c) */
void level_map_read_row(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, 0x78);                                      /* 00899A  move.w  #$78,d0 */
    /* 00899E  tst.w  $2da(a5) */
    /* 0089A2  beq  $89a8 */
    if (rd16(V_02DA) != 0) {
L_0089A4:
        SETW(d0, rd16(V_02DE));                              /* 0089A4  move.w  $2de(a5),d0 */
    }
    SETW(d1, 0x78);                                      /* 0089A8  move.w  #$78,d1 */
    /* 0089AC  tst.w  $376(a5) */
    /* 0089B0  beq  $89b6 */
    if (rd16(V_0376) != 0) {
        SETW(d1, rd16(V_037A));                              /* 0089B2  move.w  $37a(a5),d1 */
    }
L_0089B6:
    /* 0089B6  cmp.w  #$a2,d0 */
    /* 0089BA  bhi  $8998 */
    if ((d0 & 0xFFFF) > 0xA2) {
        { sub_008998(); return; }
    }
L_0089BC:
    /* 0089BC  cmp.w  #$a2,d1 */
    /* 0089C0  bhi  $8998 */
    if ((d1 & 0xFFFF) > 0xA2) {
        { sub_008998(); return; }
    }
L_0089C2:
    SETW(d0, d0 + (d1 & 0xFFFF));                        /* 0089C2  add.w  d1,d0 */
    /* 0089C4  cmp.w  #$f4,d0 */
    /* 0089C8  bhi  $8998 */
    if ((d0 & 0xFFFF) > 0xF4) {
        { sub_008998(); return; }
    }
L_0089CA:
    wr16(V_SCROLL, rd16(V_SCROLL) + 1);                  /* 0089CA  addq.w  #1,$280(a5) */
    wr8(V_1DF4, rd8(V_1DF4) + 1);                        /* 0089CE  addq.b  #1,$1df4(a5) */
    wr16(V_02DE, rd16(V_02DE) + 1);                      /* 0089D2  addq.w  #1,$2de(a5) */
    wr16(V_037A, rd16(V_037A) + 1);                      /* 0089D6  addq.w  #1,$37a(a5) */
    a6 = V_ENEMIES;                                      /* 0089DA  lea  $412(a5),a6 */
    SETW(d6, 0xBC0);                                     /* 0089DE  move.w  #$bc0,d6 */
    do {
        BUDGET_CHECK(0x89E2U);
L_0089E2:
        wr16(a6 + 0x2 + SX16(d6), rd16(a6 + 0x2 + SX16(d6)) + 1); /* 0089E2  addq.w  #1,$2(a6,d6.w) */
        /* 0089E6  subi.w  #$40,d6 */
        {
            uint32_t s = 0x40;
            uint32_t d = (d6 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d6, r);
            fn = !!(r & 0x8000);
        }
        /* 0089EA  bpl  $89e2 */
    } while (COND(10));
L_0089EC:
    a6 = V_1206;                                         /* 0089EC  lea  $1206(a5),a6 */
    SETW(d6, 0x10);                                      /* 0089F0  move.w  #$10,d6 */
    do {
        BUDGET_CHECK(0x89F4U);
L_0089F4:
        wr16(a6 + 0x2, rd16(a6 + 0x2) + 1);                  /* 0089F4  addq.w  #1,$2(a6) */
        a6 = a6 + 0x14;                                      /* 0089F8  lea  $14(a6),a6 */
        /* 0089FC  dbra  d6,$89f4 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_008A00:
    a6 = V_1472;                                         /* 008A00  lea  $1472(a5),a6 */
    SETW(d6, 0xE0);                                      /* 008A04  move.w  #$e0,d6 */
    do {
        BUDGET_CHECK(0x8A08U);
L_008A08:
        wr16(a6 + 0x2 + SX16(d6), rd16(a6 + 0x2 + SX16(d6)) + 1); /* 008A08  addq.w  #1,$2(a6,d6.w) */
        /* 008A0C  subi.w  #$e,d6 */
        {
            uint32_t s = 0xE;
            uint32_t d = (d6 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d6, r);
            fn = !!(r & 0x8000);
        }
        /* 008A10  bpl  $8a08 */
    } while (COND(10));
L_008A12:
    a6 = V_18E6;                                         /* 008A12  lea  $18e6(a5),a6 */
    SETW(d6, 0x17C);                                     /* 008A16  move.w  #$17c,d6 */
    do {
        BUDGET_CHECK(0x8A1AU);
L_008A1A:
        wr16(a6 + 0x2 + SX16(d6), rd16(a6 + 0x2 + SX16(d6)) + 1); /* 008A1A  addq.w  #1,$2(a6,d6.w) */
        /* 008A1E  subi.w  #$14,d6 */
        {
            uint32_t s = 0x14;
            uint32_t d = (d6 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d6, r);
            fn = !!(r & 0x8000);
        }
        /* 008A22  bpl  $8a1a */
    } while (COND(10));
L_008A24:
    a0 = V_BULLETS;                                      /* 008A24  lea  $1012(a5),a0 */
    SETW(d0, 0x1D);                                      /* 008A28  move.w  #$1d,d0 */
    do {
        BUDGET_CHECK(0x8A2CU);
L_008A2C:
        wr16(a0 + 0x2, rd16(a0 + 0x2) + 1);                  /* 008A2C  addi.w  #$1,$2(a0) */
        a0 = a0 + 0xE;                                       /* 008A32  lea  $e(a0),a0 */
        /* 008A36  dbra  d0,$8a2c */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008A3A:
    a0 = V_1C84;                                         /* 008A3A  lea  $1c84(a5),a0 */
    SETW(d0, 5);                                         /* 008A3E  move.w  #$5,d0 */
    do {
        BUDGET_CHECK(0x8A42U);
L_008A42:
        wr16(a0 + 0x6, rd16(a0 + 0x6) + 1);                  /* 008A42  addi.w  #$1,$6(a0) */
        a0 = a0 + 0x12;                                      /* 008A48  lea  $12(a0),a0 */
        /* 008A4C  dbra  d0,$8a42 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008A50:
    SETW(d7, 4);                                         /* 008A50  move.w  #$4,d7 */
    a0 = V_VEHICLES;                                     /* 008A54  lea  $1a76(a5),a0 */
    do {
        BUDGET_CHECK(0x8A58U);
L_008A58:
        wr16(a0 + 0x4, rd16(a0 + 0x4) + 1);                  /* 008A58  addi.w  #$1,$4(a0) */
        a0 = a0 + 0x2E;                                      /* 008A5E  lea  $2e(a0),a0 */
        /* 008A62  dbra  d7,$8a58 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_008A66:
    a0 = V_1614;                                         /* 008A66  lea  $1614(a5),a0 */
    SETW(d0, 0x13);                                      /* 008A6A  move.w  #$13,d0 */
    do {
        BUDGET_CHECK(0x8A6EU);
L_008A6E:
        wr16(a0 + 0x4, rd16(a0 + 0x4) + 1);                  /* 008A6E  addi.w  #$1,$4(a0) */
        a0 = a0 + 0x10;                                      /* 008A74  lea  $10(a0),a0 */
        /* 008A78  dbra  d0,$8a6e */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008A7C:
    a0 = V_1560;                                         /* 008A7C  lea  $1560(a5),a0 */
    SETW(d0, 0xE);                                       /* 008A80  move.w  #$e,d0 */
    do {
        BUDGET_CHECK(0x8A84U);
L_008A84:
        wr16(a0 + 0x4, rd16(a0 + 0x4) + 1);                  /* 008A84  addi.w  #$1,$4(a0) */
        a0 = a0 + 0xC;                                       /* 008A8A  lea  $c(a0),a0 */
        /* 008A8E  dbra  d0,$8a84 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008A92:
    a0 = V_1754;                                         /* 008A92  lea  $1754(a5),a0 */
    SETW(d0, 0x18);                                      /* 008A96  move.w  #$18,d0 */
    do {
        BUDGET_CHECK(0x8A9AU);
L_008A9A:
        wr16(a0 + 0x2, rd16(a0 + 0x2) + 1);                  /* 008A9A  addi.w  #$1,$2(a0) */
        a0 = a0 + 0x10;                                      /* 008AA0  lea  $10(a0),a0 */
        /* 008AA4  dbra  d0,$8a9a */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008AA8:
    a0 = V_1CF0;                                         /* 008AA8  lea  $1cf0(a5),a0 */
    SETW(d0, 0);                                         /* 008AAC  move.w  #$0,d0 */
    do {
        BUDGET_CHECK(0x8AB0U);
L_008AB0:
        wr16(a0 + 0x4, rd16(a0 + 0x4) + 1);                  /* 008AB0  addi.w  #$1,$4(a0) */
        a0 = a0 + 0x8;                                       /* 008AB6  lea  $8(a0),a0 */
        /* 008ABA  dbra  d0,$8ab0 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008ABE:
    SETW(d0, 0x13);                                      /* 008ABE  move.w  #$13,d0 */
    a0 = V_135A;                                         /* 008AC2  lea  $135a(a5),a0 */
    do {
        BUDGET_CHECK(0x8AC6U);
L_008AC6:
        wr16(a0, rd16(a0) + 1);                              /* 008AC6  addq.w  #1,(a0) */
        a0 = a0 + 0xE;                                       /* 008AC8  lea  $e(a0),a0 */
        /* 008ACC  dbra  d0,$8ac6 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008AD0:
    SETW(d1, 0xFF);                                      /* 008AD0  move.w  #$ff,d1 */
    SETW(d0, rd16(V_SCROLL));                            /* 008AD4  move.w  $280(a5),d0 */
    SETW(d0, d0 & 0xFF);                                 /* 008AD8  andi.w  #$ff,d0 */
    SETB(d1, d1 - (d0 & 0xFF));                          /* 008ADC  sub.b  d0,d1 */
    SETB(d1, d1 & 0xE0);                                 /* 008ADE  andi.b  #$e0,d1 */
    SETB(d1, d1 - 0x20);                                 /* 008AE2  subi.b  #$20,d1 */
    SETW(d1, d1 & 0xFF);                                 /* 008AE6  andi.w  #$ff,d1 */
    a0 = 0x28000;                                        /* 008AEA  lea  $28000.l,a0 */
    d1 = (d1 & 0xFFFF) * 0x60;                           /* 008AF0  mulu.w  #$60,d1 */
    a0 += SX16((d1 & 0xFFFF));                           /* 008AF4  adda.w  d1,a0 */
    SETW(d2, rd16(V_SCROLL));                            /* 008AF6  move.w  $280(a5),d2 */
    /* 008AFA  addi.w  #$20,d2 */
    {
        uint32_t s = 0x20;
        uint32_t d = (d2 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d2, r);
        fx = (r < s);
    }
    SETW(d2, d2 & 0x3E0);                                /* 008AFE  andi.w  #$3e0,d2 */
    a4 = 0x2400;                                         /* 008B02  lea  $2400.l,a4 */
    a4 += SX16((d2 & 0xFFFF));                           /* 008B08  adda.w  d2,a4 */
    SETB(d0, d0 & 0x1F);                                 /* 008B0A  andi.b  #$1f,d0 */
    /* 008B0E  cmp.b  #$9,d0 */
    /* 008B12  beq  $8b2e */
    if ((d0 & 0xFF) != 9) {
L_008B14:
        a0 += SX16(0x20);                                    /* 008B14  adda.w  #$20,a0 */
        a4 += 4;                                             /* 008B18  addq.w  #4,a4 */
        /* 008B1A  cmp.b  #$13,d0 */
        /* 008B1E  beq  $8b2e */
        if ((d0 & 0xFF) != 0x13) {
L_008B20:
            a0 += SX16(0x20);                                    /* 008B20  adda.w  #$20,a0 */
            a4 += 4;                                             /* 008B24  addq.w  #4,a4 */
            /* 008B26  cmp.b  #$1d,d0 */
            /* 008B2A  beq  $8b2e */
            if ((d0 & 0xFF) != 0x1D) {
L_008B2C:
                RTS();                                               /* 008B2C  rts */
            }
        }
    }
L_008B2E:
    wr32(a4, 0);                                         /* 008B2E  clr.l  (a4) */
    wr32(a4 + 0x10, 0);                                  /* 008B30  clr.l  $10(a4) */
    a2 = rd32(V_027C);                                   /* 008B34  movea.l  $27c(a5),a2 */
    SETW(d0, 0);                                         /* 008B38  clr.w  d0 */
    /* 008B3A  move.b  (a2)+,d0 */
    SETB(d0, rd8(a2));
    a2 += 1;
    wr16(V_0282, (d0 & 0xFFFF));                         /* 008B3C  move.w  d0,$282(a5) */
    /* 008B40  cmp.b  #$ff,d0 */
    /* 008B44  bne  $8b5a */
    if ((d0 & 0xFF) == 0xFF) {
        a2 -= 2;                                             /* 008B46  subq.l  #2,a2 */
        wr32(V_027C, a2);                                    /* 008B48  move.l  a2,$27c(a5) */
        d0 = 0xFFFFFFFE;                                     /* 008B4C  moveq  #-2,d0 */
        do {
            BUDGET_CHECK(0x8B4EU);
L_008B4E:
            wr16(HW_PALETTE, (d0 & 0xFFFF));                     /* 008B4E  move.w  d0,$ff8240.l */
            /* 008B54  dbra  d0,$8b4e */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
        /* 008B58  move.b  (a2)+,d0 */
        SETB(d0, rd8(a2));
        a2 += 1;
    }
L_008B5A:
    wr32(V_027C, a2);                                    /* 008B5A  move.l  a2,$27c(a5) */
    /* 008B5E  tst.b  d0 */
    /* 008B60  bpl  $8d4c */
    if ((int8_t)(d0 & 0xFF) >= 0) {
        { level_map_advance(); return; }
    }
L_008B64:
    /* 008B64  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    CALL(level_map_advance, 0x8B6AU);                    /* 008B66  bsr  $8d4c */
    do {
        BUDGET_CHECK(0x8B6AU);
L_008B6A:
        a0 = rd32(a7);                                       /* 008B6A  movea.l  (a7),a0 */
        d0 = 0;                                              /* 008B6C  clr.l  d0 */
        /* 008B6E  move.b  (a2)+,d0 */
        {
            uint32_t v = rd8(a2);
            a2 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 008B70  beq  $8bd6 */
        if (COND(7)) {
            goto L_008BD6;
        }
L_008B74:
        /* 008B74  cmp.b  #$f3,d0 */
        /* 008B78  bne  $8b82 */
        if ((d0 & 0xFF) == 0xF3) {
            CALL(sub_006F3A, 0x8B7EU);                           /* 008B7A  bsr  $6f3a */
L_008B7E:
            /* 008B7E  bra  $8bd0 */
        } else {
L_008B82:
            /* 008B82  cmp.b  #$f4,d0 */
            /* 008B86  bne  $8b90 */
            if ((d0 & 0xFF) == 0xF4) {
                CALL(sub_006F32, 0x8B8CU);                           /* 008B88  bsr  $6f32 */
L_008B8C:
                /* 008B8C  bra  $8bd0 */
            } else {
L_008B90:
                /* 008B90  cmp.b  #$f2,d0 */
                /* 008B94  bne  $8b9e */
                if ((d0 & 0xFF) == 0xF2) {
                    CALL(sub_006F22, 0x8B9AU);                           /* 008B96  bsr  $6f22 */
L_008B9A:
                    /* 008B9A  bra  $8bd0 */
                } else {
L_008B9E:
                    /* 008B9E  cmp.b  #$f1,d0 */
                    /* 008BA2  bne  $8bac */
                    if ((d0 & 0xFF) == 0xF1) {
                        CALL(sub_006F2A, 0x8BA8U);                           /* 008BA4  bsr  $6f2a */
L_008BA8:
                        /* 008BA8  bra  $8bd0 */
                    } else {
L_008BAC:
                        SETW(d2, 0);                                         /* 008BAC  clr.w  d2 */
                        /* 008BAE  move.b  (a2)+,d2 */
                        SETB(d2, rd8(a2));
                        a2 += 1;
                        a3 = 0x8D80;                                         /* 008BB0  lea  $8d80(pc),a3 */
                        SETB(d2, (d2 & 0xFF) << 2);                          /* 008BB4  lsl.b  #2,d2 */
                        SETW(d1, rd16(a3 + SX16(d2)));                       /* 008BB6  move.w  $0(a3,d2.w),d1 */
                        a0 += SX16((d1 & 0xFFFF));                           /* 008BBA  adda.w  d1,a0 */
                        /* 008BBC  subq.w  #1,d0 */
                        {
                            uint32_t s = 1;
                            uint32_t d = (d0 & 0xFFFF);
                            uint32_t r = (d - s) & 0xFFFF;
                            SETW(d0, r);
                            fx = (s > d);
                        }
                        /* 008BBE  cmp.w  #$7c,d0 */
                        /* 008BC2  bhi  $8bce */
                        if ((d0 & 0xFFFF) <= 0x7C) {
L_008BC4:
                            SETW(d1, rd16(a3 + 0x2 + SX16(d2)));                 /* 008BC4  move.w  $2(a3,d2.w),d1 */
                            wr8(a4 + SX16(d1), 0xFF);                            /* 008BC8  st  $0(a4,d1.w) */
                            /* 008BCC  addq.w  #1,d0 */
                            {
                                uint32_t s = 1;
                                uint32_t d = (d0 & 0xFFFF);
                                uint32_t r = (d + s) & 0xFFFF;
                                SETW(d0, r);
                                fx = (r < s);
                            }
                        }
L_008BCE:
                        CALL(level_map_read_objects, 0x8BD0U);               /* 008BCE  bsr  $8bde */
                    }
                }
            }
        }
L_008BD0:
        /* 008BD0  tst.b  (a2) */
        /* 008BD2  bne  $8b6a */
    } while (rd8(a2) != 0);
L_008BD4:
    a2 += 1;                                             /* 008BD4  addq.l  #1,a2 */
L_008BD6:
    wr32(V_027C, a2);                                    /* 008BD6  move.l  a2,$27c(a5) */
    /* 008BDA  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 008BDC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x89A4U: goto L_0089A4;
        case 0x89B6U: goto L_0089B6;
        case 0x89BCU: goto L_0089BC;
        case 0x89C2U: goto L_0089C2;
        case 0x89CAU: goto L_0089CA;
        case 0x89E2U: goto L_0089E2;
        case 0x89ECU: goto L_0089EC;
        case 0x89F4U: goto L_0089F4;
        case 0x8A00U: goto L_008A00;
        case 0x8A08U: goto L_008A08;
        case 0x8A12U: goto L_008A12;
        case 0x8A1AU: goto L_008A1A;
        case 0x8A24U: goto L_008A24;
        case 0x8A2CU: goto L_008A2C;
        case 0x8A3AU: goto L_008A3A;
        case 0x8A42U: goto L_008A42;
        case 0x8A50U: goto L_008A50;
        case 0x8A58U: goto L_008A58;
        case 0x8A66U: goto L_008A66;
        case 0x8A6EU: goto L_008A6E;
        case 0x8A7CU: goto L_008A7C;
        case 0x8A84U: goto L_008A84;
        case 0x8A92U: goto L_008A92;
        case 0x8A9AU: goto L_008A9A;
        case 0x8AA8U: goto L_008AA8;
        case 0x8AB0U: goto L_008AB0;
        case 0x8ABEU: goto L_008ABE;
        case 0x8AC6U: goto L_008AC6;
        case 0x8AD0U: goto L_008AD0;
        case 0x8B14U: goto L_008B14;
        case 0x8B20U: goto L_008B20;
        case 0x8B2CU: goto L_008B2C;
        case 0x8B2EU: goto L_008B2E;
        case 0x8B4EU: goto L_008B4E;
        case 0x8B5AU: goto L_008B5A;
        case 0x8B64U: goto L_008B64;
        case 0x8B6AU: goto L_008B6A;
        case 0x8B74U: goto L_008B74;
        case 0x8B7EU: goto L_008B7E;
        case 0x8B82U: goto L_008B82;
        case 0x8B8CU: goto L_008B8C;
        case 0x8B90U: goto L_008B90;
        case 0x8B9AU: goto L_008B9A;
        case 0x8B9EU: goto L_008B9E;
        case 0x8BA8U: goto L_008BA8;
        case 0x8BACU: goto L_008BAC;
        case 0x8BC4U: goto L_008BC4;
        case 0x8BCEU: goto L_008BCE;
        case 0x8BD0U: goto L_008BD0;
        case 0x8BD4U: goto L_008BD4;
        case 0x8BD6U: goto L_008BD6;
        }
    }
}

/* 008BDE  Reads the list of objects and events of a map block */
void level_map_read_objects(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008BDE  movem.l  d5/d4/d3/d2/d1/d0,-(a7) */
    PUSH32(d5);
    PUSH32(d4);
    PUSH32(d3);
    PUSH32(d2);
    PUSH32(d1);
    PUSH32(d0);
    a1 = 0x57C9A;                                        /* 008BE2  lea  $57c9a.l,a1 */
    d0 &= 0xFF;                                          /* 008BE8  andi.l  #$ff,d0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 7, 16));      /* 008BEE  lsl.w  #7,d0 */
    a1 += d0;                                            /* 008BF0  adda.l  d0,a1 */
    d5 = 0xF;                                            /* 008BF2  moveq  #15,d5 */
    do {
        BUDGET_CHECK(0x8BF4U);
L_008BF4:
        /* 008BF4  move.w  (a1)+,d0 */
        SETW(d0, rd16(a1));
        a1 += 2;
        /* 008BF6  move.w  (a1)+,d1 */
        SETW(d1, rd16(a1));
        a1 += 2;
        /* 008BF8  move.w  (a1)+,d2 */
        SETW(d2, rd16(a1));
        a1 += 2;
        /* 008BFA  move.w  (a1)+,d3 */
        SETW(d3, rd16(a1));
        a1 += 2;
        SETW(d4, (d0 & 0xFFFF));                             /* 008BFC  move.w  d0,d4 */
        SETW(d4, d4 | (d1 & 0xFFFF));                        /* 008BFE  or.w  d1,d4 */
        SETW(d4, d4 | (d2 & 0xFFFF));                        /* 008C00  or.w  d2,d4 */
        SETW(d4, d4 | (d3 & 0xFFFF));                        /* 008C02  or.w  d3,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 008C04  not.w  d4 */
        wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 008C06  and.w  d4,(a0) */
        /* 008C08  or.w  d0,(a0)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 008C0A  and.w  d4,(a0) */
        /* 008C0C  or.w  d1,(a0)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 008C0E  and.w  d4,(a0) */
        /* 008C10  or.w  d2,(a0)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 008C12  and.w  d4,(a0) */
        wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 008C14  or.w  d3,(a0) */
        a0 += SX16(0x5A);                                    /* 008C16  adda.w  #$5a,a0 */
        /* 008C1A  dbra  d5,$8bf4 */
        SETW(d5, d5 - 1);
    } while ((d5 & 0xFFFF) != 0xFFFF);
L_008C1E:
    /* 008C1E  movem.l  (a7)+,d0-d5 */
    d0 = POP32();
    d1 = POP32();
    d2 = POP32();
    d3 = POP32();
    d4 = POP32();
    d5 = POP32();
    RTS();                                               /* 008C22  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8BF4U: goto L_008BF4;
        case 0x8C1EU: goto L_008C1E;
        }
    }
}

/* 008C24  game routine */
void sub_008C24(void)
{
    SETB(d0, d0 | 0x62);                                 /* 008C24  ori.b  #$62,d0 */
    wr8(a5, rd8(a5) ^ (1u << (d2 & 7)));                 /* 008C28  bchg  d2,(a5) */
    /* 008C2A  subi.w  #$233,d3 */
    {
        uint32_t s = 0x233;
        uint32_t d = (d3 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d3, r);
        fx = (s > d);
    }
    wr8(a2, rd8(a2) ^ (1u << (d0 & 7)));                 /* 008C2E  bchg  d0,(a2) */
    a0 -= 1;                                             /* 008C30  btst  d2,-(a0) */
    /* 008C32  ori.b  #$76,-(a1) */
    {
        uint32_t s = 0x76;
        a1 -= 1;
        uint32_t d = rd8(a1);
        uint32_t r = (d | s) & 0xFF;
        wr8(a1, r);
        fn = !!(r & 0x80);
        fv = 0;
        fc = 0;
    }
    /* 008C36  btst  d0,-(a2) */
    a2 -= 1;
    fz = !(rd8(a2) & (1u << (d0 & 7)));
    MISS(0x8C38U);
}

/* 008C44  game routine */
void sub_008C44(void)
{
    SETB(d0, d0 | 0);                                    /* 008C44  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008C48  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008C4C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008C50  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008C54  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008C58  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 008C5C  ori.b  #$0,d0 */
    /* 008C60  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_008C64(); return; }
}

/* 008C64  game routine */
void sub_008C64(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x8C44;                                         /* 008C64  lea  $8c44(pc),a1 */
    CALL(sub_0075E2, 0x8C6CU);                           /* 008C68  bsr  $75e2 */
L_008C6C:
    CALL(sub_007134, 0x8C70U);                           /* 008C6C  bsr  $7134 */
L_008C70:
    CALL(sub_008DA0, 0x8C74U);                           /* 008C70  bsr  $8da0 */
L_008C74:
    CALL(sub_009D70, 0x8C78U);                           /* 008C74  bsr  $9d70 */
L_008C78:
    SETW(d0, rd16(V_01B4));                              /* 008C78  move.w  $1b4(a5),d0 */
    wr16(V_01AA, (d0 & 0xFFFF));                         /* 008C7C  move.w  d0,$1aa(a5) */
    /* 008C80  tst.w  $1b4(a5) */
    /* 008C84  bne  $8c8c */
    if (rd16(V_01B4) == 0) {
L_008C88:
        CALL(sub_008DC6, 0x8C8CU);                           /* 008C88  bsr  $8dc6 */
    }
L_008C8C:
    CALL(sub_008CA2, 0x8C90U);                           /* 008C8C  bsr  $8ca2 */
L_008C90:
    CALL(level_map_prefill, 0x8C94U);                    /* 008C90  bsr  $8d1e */
L_008C94:
    CALL(sub_007558, 0x8C98U);                           /* 008C94  bsr  $7558 */
L_008C98:
    a1 = 0x8C24;                                         /* 008C98  lea  $8c24(pc),a1 */
    CALL(sub_0075E2, 0x8CA0U);                           /* 008C9C  bsr  $75e2 */
L_008CA0:
    RTS();                                               /* 008CA0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8C6CU: goto L_008C6C;
        case 0x8C70U: goto L_008C70;
        case 0x8C74U: goto L_008C74;
        case 0x8C78U: goto L_008C78;
        case 0x8C88U: goto L_008C88;
        case 0x8C8CU: goto L_008C8C;
        case 0x8C90U: goto L_008C90;
        case 0x8C94U: goto L_008C94;
        case 0x8C98U: goto L_008C98;
        case 0x8CA0U: goto L_008CA0;
        }
    }
}

/* 008CA2  game routine */
void sub_008CA2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x44776;                                        /* 008CA2  lea  $44776.l,a0 */
    SETW(d0, rd16(V_021C));                              /* 008CA8  move.w  $21c(a5),d0 */
    d0 = (d0 & 0xFFFF) * 0x150;                          /* 008CAC  mulu.w  #$150,d0 */
    a0 = a0 + SX16(d0);                                  /* 008CB0  lea  $0(a0,d0.w),a0 */
    a2 = a0;                                             /* 008CB4  movea.l  a0,a2 */
    a1 = 0x25BE8;                                        /* 008CB6  lea  $25be8.l,a1 */
    a3 = 0x25C68;                                        /* 008CBC  lea  $25c68.l,a3 */
    /* 008CC2  tst.w  $1b4(a5) */
    /* 008CC6  bne  $8cf0 */
    if (rd16(V_01B4) == 0) {
        do {
            BUDGET_CHECK(0x8CC8U);
L_008CC8:
            CALL(sub_007576, 0x8CCCU);                           /* 008CC8  bsr  $7576 */
L_008CCC:
            d0 &= 0xF;                                           /* 008CCC  andi.l  #$f,d0 */
            /* 008CD2  cmp.w  #$b,d0 */
            /* 008CD6  bls  $8cda */
            if ((d0 & 0xFFFF) > 0xB) {
                d0 = 0xB;                                            /* 008CD8  moveq  #11,d0 */
            }
L_008CDA:
            SETW(d5, rd16(V_021C));                              /* 008CDA  move.w  $21c(a5),d5 */
            /* 008CDE  cmp.w  d5,d0 */
            /* 008CE0  beq  $8cc8 */
        } while ((d0 & 0xFFFF) == (d5 & 0xFFFF));
L_008CE2:
        d0 = (d0 & 0xFFFF) * 0x150;                          /* 008CE2  mulu.w  #$150,d0 */
        a2 = 0x44776;                                        /* 008CE6  lea  $44776.l,a2 */
        a2 = a2 + SX16(d0);                                  /* 008CEC  lea  $0(a2,d0.w),a2 */
    }
    SETW(d7, 0x14);                                      /* 008CF0  move.w  #$14,d7 */
    do {
        BUDGET_CHECK(0x8CF4U);
L_008CF4:
        /* 008CF4  move.l  (a0)+,(a1) */
        wr32(a1, rd32(a0));
        a0 += 4;
        /* 008CF6  move.l  (a2)+,(a3) */
        wr32(a3, rd32(a2));
        a2 += 4;
        /* 008CF8  move.l  (a0)+,$4(a1) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1 + 0x4, v);
        }
        /* 008CFC  move.l  (a2)+,$4(a3) */
        wr32(a3 + 0x4, rd32(a2));
        a2 += 4;
        /* 008D00  move.l  (a0)+,$8(a1) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1 + 0x8, v);
        }
        /* 008D04  move.l  (a2)+,$8(a3) */
        wr32(a3 + 0x8, rd32(a2));
        a2 += 4;
        /* 008D08  move.l  (a0)+,$c(a1) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1 + 0xC, v);
        }
        /* 008D0C  move.l  (a2)+,$c(a3) */
        wr32(a3 + 0xC, rd32(a2));
        a2 += 4;
        a1 = a1 + 0xA0;                                      /* 008D10  lea  $a0(a1),a1 */
        a3 = a3 + 0xA0;                                      /* 008D14  lea  $a0(a3),a3 */
        /* 008D18  dbra  d7,$8cf4 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_008D1C:
    RTS();                                               /* 008D1C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8CC8U: goto L_008CC8;
        case 0x8CCCU: goto L_008CCC;
        case 0x8CDAU: goto L_008CDA;
        case 0x8CE2U: goto L_008CE2;
        case 0x8CF4U: goto L_008CF4;
        case 0x8D1CU: goto L_008D1C;
        }
    }
}

/* 008D1E  Fills the first rows of the background buffer when a section starts */
void level_map_prefill(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = rd32(V_027C);                                   /* 008D1E  movea.l  $27c(a5),a0 */
    d0 = 0;                                              /* 008D22  moveq  #0,d0 */
    SETB(d0, rd8(a0));                                   /* 008D24  move.b  (a0),d0 */
    wr16(V_0282, (d0 & 0xFFFF));                         /* 008D26  move.w  d0,$282(a5) */
    a0 = 0x28000;                                        /* 008D2A  lea  $28000.l,a0 */
    d6 = 7;                                              /* 008D30  moveq  #7,d6 */
    do {
        BUDGET_CHECK(0x8D32U);
L_008D32:
        d5 = 2;                                              /* 008D32  moveq  #2,d5 */
        do {
            BUDGET_CHECK(0x8D34U);
L_008D34:
            /* 008D34  move.w  d0,-(a7) */
            {
                uint32_t v = (d0 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
            }
            CALL(level_map_advance, 0x8D38U);                    /* 008D36  bsr  $8d4c */
L_008D38:
            /* 008D38  move.w  (a7)+,d0 */
            SETW(d0, rd16(a7));
            a7 += 2;
            a0 = a0 + 0x20;                                      /* 008D3A  lea  $20(a0),a0 */
            /* 008D3E  dbra  d5,$8d34 */
            SETW(d5, d5 - 1);
        } while ((d5 & 0xFFFF) != 0xFFFF);
L_008D42:
        a0 = a0 + 0xBA0;                                     /* 008D42  lea  $ba0(a0),a0 */
        /* 008D46  dbra  d6,$8d32 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_008D4A:
    RTS();                                               /* 008D4A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8D32U: goto L_008D32;
        case 0x8D34U: goto L_008D34;
        case 0x8D38U: goto L_008D38;
        case 0x8D42U: goto L_008D42;
        case 0x8D4AU: goto L_008D4A;
        }
    }
}

/* 008D4C  Advances the map one row */
void level_map_advance(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, d0 & 0x7F);                                 /* 008D4C  andi.w  #$7f,d0 */
    a1 = 0x5649A;                                        /* 008D50  lea  $5649a.l,a1 */
    d0 = (d0 & 0xFFFF) * 0x400;                          /* 008D56  mulu.w  #$400,d0 */
    a1 += d0;                                            /* 008D5A  adda.l  d0,a1 */
    d7 = 0x1F;                                           /* 008D5C  moveq  #31,d7 */
    do {
        BUDGET_CHECK(0x8D5EU);
L_008D5E:
        /* 008D5E  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D60  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D62  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D64  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D66  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D68  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D6A  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        /* 008D6C  move.l  (a1)+,(a0)+ */
        {
            uint32_t v = rd32(a1);
            a1 += 4;
            wr32(a0, v);
            a0 += 4;
        }
        a0 += 0x40;                                          /* 008D6E  adda.l  #$40,a0 */
        /* 008D74  dbra  d7,$8d5e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_008D78:
    a0 -= 0xC00;                                         /* 008D78  suba.l  #$c00,a0 */
    RTS();                                               /* 008D7E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8D5EU: goto L_008D5E;
        case 0x8D78U: goto L_008D78;
        }
    }
}

/* 008D80  game routine */
void sub_008D80(void)
{
    /* 008D80  ori.b  #$10,d0 */
    {
        uint32_t s = 0x10;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x8D84U);
}

/* 008DA0  game routine */
void sub_008DA0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0xFF8C;                                         /* 008DA0  lea  $ff8c(pc),a0 */
    a1 = 0x20268;                                        /* 008DA4  lea  $20268.l,a1 */
    SETW(d0, 0x27);                                      /* 008DAA  move.w  #$27,d0 */
    do {
        BUDGET_CHECK(0x8DAEU);
L_008DAE:
        /* 008DAE  move.l  (a0)+,(a1) */
        wr32(a1, rd32(a0));
        a0 += 4;
        /* 008DB0  move.l  (a0)+,$4(a1) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1 + 0x4, v);
        }
        /* 008DB4  move.l  (a0)+,$8(a1) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1 + 0x8, v);
        }
        /* 008DB8  move.l  (a0)+,$c(a1) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1 + 0xC, v);
        }
        a1 = a1 + 0xA0;                                      /* 008DBC  lea  $a0(a1),a1 */
        /* 008DC0  dbra  d0,$8dae */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_008DC4:
    RTS();                                               /* 008DC4  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8DAEU: goto L_008DAE;
        case 0x8DC4U: goto L_008DC4;
        }
    }
}

/* 008DC6  game routine */
void sub_008DC6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x26FC0;                                        /* 008DC6  lea  $26fc0.l,a1 */
    a2 = 0x1EFC0;                                        /* 008DCC  lea  $1efc0.l,a2 */
    a0 = 0x1026C;                                        /* 008DD2  lea  $1026c(pc),a0 */
    SETW(d7, 0x11);                                      /* 008DD6  move.w  #$11,d7 */
    do {
        BUDGET_CHECK(0x8DDAU);
L_008DDA:
        wr32(a1, rd32(a0));                                  /* 008DDA  move.l  (a0),(a1) */
        /* 008DDC  move.l  (a0)+,(a2) */
        wr32(a2, rd32(a0));
        a0 += 4;
        wr32(a1 + 0x4, rd32(a0));                            /* 008DDE  move.l  (a0),$4(a1) */
        /* 008DE2  move.l  (a0)+,$4(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x4, v);
        }
        wr32(a1 + 0x8, rd32(a0));                            /* 008DE6  move.l  (a0),$8(a1) */
        /* 008DEA  move.l  (a0)+,$8(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x8, v);
        }
        wr32(a1 + 0xC, rd32(a0));                            /* 008DEE  move.l  (a0),$c(a1) */
        /* 008DF2  move.l  (a0)+,$c(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0xC, v);
        }
        wr32(a1 + 0x10, rd32(a0));                           /* 008DF6  move.l  (a0),$10(a1) */
        /* 008DFA  move.l  (a0)+,$10(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x10, v);
        }
        wr32(a1 + 0x14, rd32(a0));                           /* 008DFE  move.l  (a0),$14(a1) */
        /* 008E02  move.l  (a0)+,$14(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x14, v);
        }
        wr32(a1 + 0x18, rd32(a0));                           /* 008E06  move.l  (a0),$18(a1) */
        /* 008E0A  move.l  (a0)+,$18(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x18, v);
        }
        wr32(a1 + 0x1C, rd32(a0));                           /* 008E0E  move.l  (a0),$1c(a1) */
        /* 008E12  move.l  (a0)+,$1c(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x1C, v);
        }
        a1 = a1 + 0xA0;                                      /* 008E16  lea  $a0(a1),a1 */
        a2 = a2 + 0xA0;                                      /* 008E1A  lea  $a0(a2),a2 */
        /* 008E1E  dbra  d7,$8dda */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_008E22:
    RTS();                                               /* 008E22  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8DDAU: goto L_008DDA;
        case 0x8E22U: goto L_008E22;
        }
    }
}

/* 008E24  game routine */
void sub_008E24(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x26F40;                                        /* 008E24  lea  $26f40.l,a1 */
    a2 = 0x1EF40;                                        /* 008E2A  lea  $1ef40.l,a2 */
    a0 = 0x104AC;                                        /* 008E30  lea  $104ac(pc),a0 */
    SETW(d7, 0x11);                                      /* 008E34  move.w  #$11,d7 */
    do {
        BUDGET_CHECK(0x8E38U);
L_008E38:
        wr32(a1, rd32(a0));                                  /* 008E38  move.l  (a0),(a1) */
        /* 008E3A  move.l  (a0)+,(a2) */
        wr32(a2, rd32(a0));
        a0 += 4;
        wr32(a1 + 0x4, rd32(a0));                            /* 008E3C  move.l  (a0),$4(a1) */
        /* 008E40  move.l  (a0)+,$4(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x4, v);
        }
        wr32(a1 + 0x8, rd32(a0));                            /* 008E44  move.l  (a0),$8(a1) */
        /* 008E48  move.l  (a0)+,$8(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x8, v);
        }
        wr32(a1 + 0xC, rd32(a0));                            /* 008E4C  move.l  (a0),$c(a1) */
        /* 008E50  move.l  (a0)+,$c(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0xC, v);
        }
        wr32(a1 + 0x10, rd32(a0));                           /* 008E54  move.l  (a0),$10(a1) */
        /* 008E58  move.l  (a0)+,$10(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x10, v);
        }
        wr32(a1 + 0x14, rd32(a0));                           /* 008E5C  move.l  (a0),$14(a1) */
        /* 008E60  move.l  (a0)+,$14(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x14, v);
        }
        wr32(a1 + 0x18, rd32(a0));                           /* 008E64  move.l  (a0),$18(a1) */
        /* 008E68  move.l  (a0)+,$18(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x18, v);
        }
        wr32(a1 + 0x1C, rd32(a0));                           /* 008E6C  move.l  (a0),$1c(a1) */
        /* 008E70  move.l  (a0)+,$1c(a2) */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a2 + 0x1C, v);
        }
        a1 = a1 + 0xA0;                                      /* 008E74  lea  $a0(a1),a1 */
        a2 = a2 + 0xA0;                                      /* 008E78  lea  $a0(a2),a2 */
        /* 008E7C  dbra  d7,$8e38 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 008E80  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8E38U: goto L_008E38;
        }
    }
}

/* 008E82  game routine */
void sub_008E82(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008E82  tst.w  $2da(a5) */
    /* 008E86  bne  $8e94 */
    if (rd16(V_02DA) == 0) {
        /* 008E88  tst.w  $36e(a5) */
        /* 008E8C  bne  $8e94 */
        if (rd16(V_036E) == 0) {
            CALL(sub_008E24, 0x8E90U);                           /* 008E8E  bsr  $8e24 */
L_008E90:
            wr8(V_036E, 0xFF);                                   /* 008E90  st  $36e(a5) */
        }
    }
L_008E94:
    RTS();                                               /* 008E94  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8E90U: goto L_008E90;
        case 0x8E94U: goto L_008E94;
        }
    }
}

/* 008E96  game routine */
void sub_008E96(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 008E96  tst.w  $376(a5) */
    /* 008E9A  bne  $8eaa */
    if (rd16(V_0376) == 0) {
L_008E9C:
        /* 008E9C  tst.w  $40a(a5) */
        /* 008EA0  bne  $8eaa */
        if (rd16(V_040A) == 0) {
L_008EA2:
            CALL(sub_008DC6, 0x8EA6U);                           /* 008EA2  bsr  $8dc6 */
L_008EA6:
            wr8(V_040A, 0xFF);                                   /* 008EA6  st  $40a(a5) */
        }
    }
L_008EAA:
    RTS();                                               /* 008EAA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8E9CU: goto L_008E9C;
        case 0x8EA2U: goto L_008EA2;
        case 0x8EA6U: goto L_008EA6;
        case 0x8EAAU: goto L_008EAA;
        }
    }
}

/* 008EAC  One logic frame in the vertical-scroll stretches: updates, draws and waits */
void frame_vertical(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
L_008EAC:
    /* 008EAC  tst.w  $252(a5) */
    /* 008EB0  beq  $8ee2 */
    if (rd16(V_0252) != 0) {
        d0 = 0;                                              /* 008EB2  moveq  #0,d0 */
        /* 008EB4  move.b  $1de2(a5),d0 */
        /* 008EB8  beq  $8ec6 */
        SETB(d0, rd8(V_1DE2));
        if ((d0 & 0xFF) != 0) {
            /* 008EBA  cmp.b  #$9,d0 */
            /* 008EBE  bhi  $8ec6 */
            if ((d0 & 0xFF) <= 9) {
                /* 008EC0  subq.w  #1,d0 */
                {
                    uint32_t s = 1;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d0, r);
                    fx = (s > d);
                }
                CALL(sub_00E756, 0x8EC6U);                           /* 008EC2  bsr  $e756 */
            }
        }
L_008EC6:
        /* 008EC6  cmpi.b  #$19,$1de2(a5) */
        /* 008ECC  bne  $8ed4 */
        if (rd8(V_1DE2) == 0x19) {
            wr8(V_MISSION_END, 0xFF);                            /* 008ECE  st  $206(a5) */
            RTS();                                               /* 008ED2  rts */
        }
        /* 008ED4  tst.w  $254(a5) */
        /* 008ED8  beq  $8ee2 */
        if (rd16(V_0254) != 0) {
            /* 008EDA  cmpi.b  #$39,$1de2(a5) */
            /* 008EE0  beq  $8eea */
            if (rd8(V_1DE2) == 0x39) {
                goto L_008EEA;
            }
        }
    }
L_008EE2:
    /* 008EE2  bsr  $91e0 */
    HOOK(29, 0x8EE2U);
    CALL(wait_two_vbl, 0x8EE6U);
L_008EE6:
    CALL(sub_007E06, 0x8EEAU);                           /* 008EE6  bsr  $7e06 */
L_008EEA:
    /* 008EEA  bsr  $a2c2 */
    HOOK(30, 0x8EEAU);
    CALL(sub_00A2C2, 0x8EEEU);
L_008EEE:
    CALL(sub_0071E8, 0x8EF2U);                           /* 008EEE  bsr  $71e8 */
L_008EF2:
    CALL(sub_00A544, 0x8EF6U);                           /* 008EF2  bsr  $a544 */
L_008EF6:
    CALL(sub_00A5FC, 0x8EFAU);                           /* 008EF6  bsr  $a5fc */
L_008EFA:
    CALL(sub_00AE1A, 0x8EFEU);                           /* 008EFA  bsr  $ae1a */
L_008EFE:
    CALL(sub_006C28, 0x8F02U);                           /* 008EFE  bsr  $6c28 */
L_008F02:
    CALL(sub_006CFA, 0x8F06U);                           /* 008F02  bsr  $6cfa */
L_008F06:
    CALL(sub_006FB0, 0x8F0AU);                           /* 008F06  bsr  $6fb0 */
L_008F0A:
    CALL(sub_006950, 0x8F0EU);                           /* 008F0A  bsr  $6950 */
L_008F0E:
    CALL(sub_009FA4, 0x8F12U);                           /* 008F0E  bsr  $9fa4 */
L_008F12:
    CALL(sub_00A526, 0x8F16U);                           /* 008F12  bsr  $a526 */
L_008F16:
    CALL(sub_00B46E, 0x8F1AU);                           /* 008F16  bsr  $b46e */
L_008F1A:
    CALL(sub_00B944, 0x8F1EU);                           /* 008F1A  bsr  $b944 */
L_008F1E:
    CALL(sub_00AC5E, 0x8F22U);                           /* 008F1E  bsr  $ac5e */
L_008F22:
    CALL(sub_00A3FE, 0x8F26U);                           /* 008F22  bsr  $a3fe */
L_008F26:
    CALL(sub_00AE7A, 0x8F2AU);                           /* 008F26  bsr  $ae7a */
L_008F2A:
    CALL(sub_00922E, 0x8F2EU);                           /* 008F2A  bsr  $922e */
L_008F2E:
    /* 008F2E  tst.w  $26e(a5) */
    /* 008F32  beq  $8f3a */
    if (rd16(V_026E) != 0) {
        /* 008F34  move.w  #$417,$ff8240.l */
        wr16(HW_PALETTE, 0x417);
    } else {
L_008F3A:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008F3A  or.w  d0,d1 */
    }
    CALL(sub_00B09A, 0x8F40U);                           /* 008F3C  bsr  $b09a */
L_008F40:
    CALL(sub_00AFA2, 0x8F44U);                           /* 008F40  bsr  $afa2 */
L_008F44:
    CALL(sub_00B386, 0x8F48U);                           /* 008F44  bsr  $b386 */
L_008F48:
    /* 008F48  tst.w  $26e(a5) */
    /* 008F4C  beq  $8f54 */
    if (rd16(V_026E) != 0) {
        /* 008F4E  move.w  #$444,$ff8240.l */
        wr16(HW_PALETTE, 0x444);
    } else {
L_008F54:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008F54  or.w  d0,d1 */
    }
    CALL(level_map_read_row, 0x8F5AU);                   /* 008F56  bsr  $899a */
L_008F5A:
    /* 008F5A  tst.w  $26e(a5) */
    /* 008F5E  beq  $8f66 */
    if (rd16(V_026E) != 0) {
        /* 008F60  move.w  #$50,$ff8240.l */
        wr16(HW_PALETTE, 0x50);
    } else {
L_008F66:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008F66  or.w  d0,d1 */
    }
    /* 008F68  tst.w  $254(a5) */
    /* 008F6C  beq  $8f78 */
    if (rd16(V_0254) != 0) {
        /* 008F6E  cmpi.b  #$39,$1de2(a5) */
        /* 008F74  beq  $900c */
        if (rd8(V_1DE2) == 0x39) {
            goto L_00900C;
        }
    }
L_008F78:
    CALL(vertical_background, 0x8F7CU);                  /* 008F78  bsr  $893c */
L_008F7C:
    /* 008F7C  tst.w  $26e(a5) */
    /* 008F80  beq  $8f88 */
    if (rd16(V_026E) != 0) {
        /* 008F82  move.w  #$222,$ff8240.l */
        wr16(HW_PALETTE, 0x222);
    } else {
L_008F88:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008F88  or.w  d0,d1 */
    }
    /* 008F8A  tst.w  $26e(a5) */
    /* 008F8E  beq  $8f96 */
    if (rd16(V_026E) != 0) {
        /* 008F90  move.w  #$0,$ff8240.l */
        wr16(HW_PALETTE, 0);
    } else {
L_008F96:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008F96  or.w  d0,d1 */
    }
    CALL(sub_006E4C, 0x8F9CU);                           /* 008F98  bsr  $6e4c */
L_008F9C:
    /* 008F9C  tst.w  $26e(a5) */
    /* 008FA0  beq  $8fa8 */
    if (rd16(V_026E) != 0) {
        /* 008FA2  move.w  #$741,$ff8240.l */
        wr16(HW_PALETTE, 0x741);
    } else {
L_008FA8:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008FA8  or.w  d0,d1 */
    }
    CALL(sub_0060AA, 0x8FAEU);                           /* 008FAA  bsr  $60aa */
L_008FAE:
    CALL(draw_explosions, 0x8FB2U);                      /* 008FAE  bsr  $6d12 */
L_008FB2:
    CALL(sub_0063DE, 0x8FB6U);                           /* 008FB2  bsr  $63de */
L_008FB6:
    CALL(sub_00652E, 0x8FBAU);                           /* 008FB6  bsr  $652e */
L_008FBA:
    CALL(sub_0066F4, 0x8FBEU);                           /* 008FBA  bsr  $66f4 */
L_008FBE:
    CALL(sub_005B32, 0x8FC2U);                           /* 008FBE  bsr  $5b32 */
L_008FC2:
    CALL(sub_005946, 0x8FC6U);                           /* 008FC2  bsr  $5946 */
L_008FC6:
    /* 008FC6  tst.w  $26e(a5) */
    /* 008FCA  beq  $8fd2 */
    HOOK(31, 0x8FC6U);
    if (rd16(V_026E) != 0) {
        /* 008FCC  move.w  #$400,$ff8240.l */
        wr16(HW_PALETTE, 0x400);
    } else {
L_008FD2:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008FD2  or.w  d0,d1 */
    }
    CALL(draw_objects_a, 0x8FD8U);                       /* 008FD4  bsr  $5e32 */
L_008FD8:
    CALL(sub_00660C, 0x8FDCU);                           /* 008FD8  bsr  $660c */
L_008FDC:
    CALL(draw_bullets, 0x8FE0U);                         /* 008FDC  bsr  $6812 */
L_008FE0:
    CALL(draw_objects_b, 0x8FE4U);                       /* 008FE0  bsr  $5f1a */
L_008FE4:
    /* 008FE4  tst.w  $26e(a5) */
    {
        uint32_t v = rd16(V_026E);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
L_008FE8:
    /* 008FE8  beq  $8ff0 */
    if (COND(6)) {
        /* 008FEA  move.w  #$444,$ff8240.l */
        wr16(HW_PALETTE, 0x444);
    } else {
L_008FF0:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008FF0  or.w  d0,d1 */
    }
L_008FF2:
    /* 008FF2  tst.w  $26e(a5) */
    {
        uint32_t v = rd16(V_026E);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
L_008FF6:
    /* 008FF6  beq  $8ffe */
    if (COND(6)) {
        /* 008FF8  move.w  #$0,$ff8240.l */
        wr16(HW_PALETTE, 0);
    } else {
L_008FFE:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 008FFE  or.w  d0,d1 */
    }
L_009000:
    CALL(draw_effects, 0x9004U);                         /* 009000  bsr  $b6ea */
L_009004:
    CALL(sub_008E82, 0x9008U);                           /* 009004  bsr  $8e82 */
L_009008:
    CALL(sub_008E96, 0x900CU);                           /* 009008  bsr  $8e96 */
L_00900C:
    /* 00900C  addq.w  #1,$272(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_0272);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_0272, r);
        fx = (r < s);
    }
    /* 009010  cmpi.w  #$560,$280(a5) */
    /* 009016  bne  $902c */
    if (rd16(V_SCROLL) == 0x560) {
        CALL(sub_0045D6, 0x901CU);                           /* 009018  bsr  $45d6 */
L_00901C:
        CALL(frame_horizontal, 0x9020U);                     /* 00901C  bsr  $474a */
L_009020:
        SETW(d0, rd16(V_02DA));                              /* 009020  move.w  $2da(a5),d0 */
        /* 009024  add.w  $376(a5),d0 */
        {
            uint32_t s = rd16(V_0376);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fz = (r == 0);
            fx = (r < s);
        }
        /* 009028  beq  $9046 */
        if (COND(7)) {
            goto L_009046;
        }
    }
L_00902C:
    /* 00902C  tst.w  $206(a5) */
    /* 009030  bne  $9046 */
    if (rd16(V_MISSION_END) == 0) {
L_009032:
        SETW(d0, rd16(V_02DA));                              /* 009032  move.w  $2da(a5),d0 */
        /* 009036  add.w  $376(a5),d0 */
        {
            uint32_t s = rd16(V_0376);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fz = (r == 0);
            fx = (r < s);
        }
        /* 00903A  bne  $8eac */
        if (COND(6)) {
            BUDGET_CHECK(0x8EACU);
            goto L_008EAC;
        }
        /* 00903E  tst.w  $254(a5) */
        /* 009042  bne  $8eac */
        if (rd16(V_0254) != 0) {
            BUDGET_CHECK(0x8EACU);
            goto L_008EAC;
        }
    }
L_009046:
    RTS();                                               /* 009046  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x8EC6U: goto L_008EC6;
        case 0x8EE2U: goto L_008EE2;
        case 0x8EE6U: goto L_008EE6;
        case 0x8EEAU: goto L_008EEA;
        case 0x8EEEU: goto L_008EEE;
        case 0x8EF2U: goto L_008EF2;
        case 0x8EF6U: goto L_008EF6;
        case 0x8EFAU: goto L_008EFA;
        case 0x8EFEU: goto L_008EFE;
        case 0x8F02U: goto L_008F02;
        case 0x8F06U: goto L_008F06;
        case 0x8F0AU: goto L_008F0A;
        case 0x8F0EU: goto L_008F0E;
        case 0x8F12U: goto L_008F12;
        case 0x8F16U: goto L_008F16;
        case 0x8F1AU: goto L_008F1A;
        case 0x8F1EU: goto L_008F1E;
        case 0x8F22U: goto L_008F22;
        case 0x8F26U: goto L_008F26;
        case 0x8F2AU: goto L_008F2A;
        case 0x8F2EU: goto L_008F2E;
        case 0x8F3AU: goto L_008F3A;
        case 0x8F40U: goto L_008F40;
        case 0x8F44U: goto L_008F44;
        case 0x8F48U: goto L_008F48;
        case 0x8F54U: goto L_008F54;
        case 0x8F5AU: goto L_008F5A;
        case 0x8F66U: goto L_008F66;
        case 0x8F78U: goto L_008F78;
        case 0x8F7CU: goto L_008F7C;
        case 0x8F88U: goto L_008F88;
        case 0x8F96U: goto L_008F96;
        case 0x8F9CU: goto L_008F9C;
        case 0x8FA8U: goto L_008FA8;
        case 0x8FAEU: goto L_008FAE;
        case 0x8FB2U: goto L_008FB2;
        case 0x8FB6U: goto L_008FB6;
        case 0x8FBAU: goto L_008FBA;
        case 0x8FBEU: goto L_008FBE;
        case 0x8FC2U: goto L_008FC2;
        case 0x8FC6U: goto L_008FC6;
        case 0x8FD2U: goto L_008FD2;
        case 0x8FD8U: goto L_008FD8;
        case 0x8FDCU: goto L_008FDC;
        case 0x8FE0U: goto L_008FE0;
        case 0x8FE4U: goto L_008FE4;
        case 0x8FE8U: goto L_008FE8;
        case 0x8FF0U: goto L_008FF0;
        case 0x8FF2U: goto L_008FF2;
        case 0x8FF6U: goto L_008FF6;
        case 0x8FFEU: goto L_008FFE;
        case 0x9000U: goto L_009000;
        case 0x9004U: goto L_009004;
        case 0x9008U: goto L_009008;
        case 0x900CU: goto L_00900C;
        case 0x901CU: goto L_00901C;
        case 0x9020U: goto L_009020;
        case 0x902CU: goto L_00902C;
        case 0x9032U: goto L_009032;
        }
    }
}

