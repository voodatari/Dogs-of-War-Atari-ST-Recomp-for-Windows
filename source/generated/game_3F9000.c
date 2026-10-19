/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 3F914E  game routine */
void sub_3F914E(void)
{
    /* 3F914E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_3F9152(); return; }
}

/* 3F9152  game routine */
void sub_3F9152(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x3FB37A;                                       /* 3F9152  lea  $3fb37a(pc),a1 */
    /* 3F9156  movea.l  (a1)+,a2 */
    a2 = rd32(a1);
    a1 += 4;
    other_sp = a2;                                       /* 3F9158  move  a2,usp */
    /* 3F915A  movem.l  (a1)+,d2-d7/a2-a7 */
    { uint32_t ea;
    ea = a1;
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
    a5 = rd32(ea);
    ea += 4;
    a6 = rd32(ea);
    ea += 4;
    a7 = rd32(ea);
    ea += 4;
    a1 = ea;
    }
    for (;;) {
        BUDGET_CHECK(0x3F915EU);
L_3F915E:
        fz = !(rd8(HW_IKBD_CTRL) & 0x1);                     /* 3F915E  btst  #$0,$fc00.w */
        /* 3F9164  beq  $3f916c */
        if (COND(7)) {
            goto L_3F916C;
        }
        (void)rd8(HW_IKBD_DATA);                             /* 3F9166  tst.b  $fc02.w */
        /* 3F916A  bra  $3f915e */
    }
L_3F916C:
    /* 3F916C  move  (a1)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a1; a1 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_3F916E:
    RTS();                                               /* 3F916E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F915EU: goto L_3F915E;
        case 0x3F916CU: goto L_3F916C;
        case 0x3F916EU: goto L_3F916E;
        }
    }
}

/* 3F9170  game routine */
void sub_3F9170(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a2 = 0x3F9F9A;                                       /* 3F9170  lea  $3f9f9a(pc),a2 */
    /* 3F9174  move.w  $8240.w,(a2)+ */
    {
        uint32_t v = rd16(HW_PALETTE);
        wr16(a2, v);
        a2 += 2;
    }
    /* 3F9178  move.w  $825e.w,(a2)+ */
    {
        uint32_t v = rd16(0xFF825E);
        wr16(a2, v);
        a2 += 2;
    }
    wr8(a2, 0xFF);                                       /* 3F917C  st  (a2) */
    SETW(d5, 0x700);                                     /* 3F917E  move.w  #$700,d5 */
    SETW(d6, 0x70);                                      /* 3F9182  move.w  #$70,d6 */
    SETW(d7, 7);                                         /* 3F9186  move.w  #$7,d7 */
    a1 = 0x3F94B8;                                       /* 3F918A  lea  $3f94b8(pc),a1 */
    /* 3F918E  clr.l  (a1)+ */
    wr32(a1, 0);
    a1 += 4;
    wr16(a1, 0);                                         /* 3F9190  clr.w  (a1) */
    a2 = 0xFFFF8240;                                     /* 3F9192  lea  $8240.w,a2 */
    d3 = 0xF;                                            /* 3F9196  moveq  #15,d3 */
    do {
        BUDGET_CHECK(0x3F9198U);
L_3F9198:
        /* 3F9198  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        SETW(d1, (d0 & 0xFFFF));                             /* 3F919A  move.w  d0,d1 */
        SETW(d2, (d0 & 0xFFFF));                             /* 3F919C  move.w  d0,d2 */
        SETW(d0, d0 & (d5 & 0xFFFF));                        /* 3F919E  and.w  d5,d0 */
        SETW(d1, d1 & (d6 & 0xFFFF));                        /* 3F91A0  and.w  d6,d1 */
        SETW(d2, d2 & (d7 & 0xFFFF));                        /* 3F91A2  and.w  d7,d2 */
        SETW(d0, (d0 & 0xFFFF) >> 8);                        /* 3F91A4  lsr.w  #8,d0 */
        SETW(d1, (d1 & 0xFFFF) >> 4);                        /* 3F91A6  lsr.w  #4,d1 */
        a1 = 0x3F94B8;                                       /* 3F91A8  lea  $3f94b8(pc),a1 */
        /* 3F91AC  add.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 3F91AE  add.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) + (d2 & 0xFFFF));                  /* 3F91B0  add.w  d2,(a1) */
        /* 3F91B2  dbra  d3,$3f9198 */
        SETW(d3, d3 - 1);
    } while ((d3 & 0xFFFF) != 0xFFFF);
L_3F91B6:
    a3 = 0x3F94B8;                                       /* 3F91B6  lea  $3f94b8(pc),a3 */
    /* 3F91BA  move.w  (a3)+,d0 */
    SETW(d0, rd16(a3));
    a3 += 2;
    SETW(d1, (d0 & 0xFFFF));                             /* 3F91BC  move.w  d0,d1 */
    SETW(d0, d0 + (d0 & 0xFFFF));                        /* 3F91BE  add.w  d0,d0 */
    SETW(d0, d0 + (d1 & 0xFFFF));                        /* 3F91C0  add.w  d1,d0 */
    /* 3F91C2  move.w  (a3)+,d1 */
    SETW(d1, rd16(a3));
    a3 += 2;
    SETW(d1, d1 + (d1 & 0xFFFF));                        /* 3F91C4  add.w  d1,d1 */
    /* 3F91C6  add.w  (a3)+,d0 */
    {
        uint32_t s = rd16(a3);
        a3 += 2;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
    }
    SETW(d0, d0 + (d1 & 0xFFFF));                        /* 3F91C8  add.w  d1,d0 */
    d0 = SX16(d0);                                       /* 3F91CA  ext.l  d0 */
    /* 3F91CC  divu.w  #$c0,d0 */
    {
        { uint32_t s, ea1; (void)ea1;
        s = 0xC0U;
        if (s == 0) SOFT(0x3F91CCU);
        uint32_t dd = d0, q = dd / s, rem = dd % s;
        if (q > 0xFFFFU) { fv = 1; fc = 0; fz = 0; } else { d0 = (rem << 16) | q; fn = !!(q & 0x8000U); fz = (q == 0); fv = 0; fc = 0; } }
    }
    SETW(d1, (d0 & 0xFFFF));                             /* 3F91D0  move.w  d0,d1 */
    SETW(d7, (d0 & 0xFFFF));                             /* 3F91D2  move.w  d0,d7 */
    SETW(d1, (d1 & 0xFFFF) << 4);                        /* 3F91D4  lsl.w  #4,d1 */
    SETW(d6, (d1 & 0xFFFF));                             /* 3F91D6  move.w  d1,d6 */
    SETW(d0, d0 + (d1 & 0xFFFF));                        /* 3F91D8  add.w  d1,d0 */
    SETW(d1, (d1 & 0xFFFF) << 4);                        /* 3F91DA  lsl.w  #4,d1 */
    SETW(d5, (d1 & 0xFFFF));                             /* 3F91DC  move.w  d1,d5 */
    SETW(d0, d0 + (d1 & 0xFFFF));                        /* 3F91DE  add.w  d1,d0 */
    wr16(a3, (d0 & 0xFFFF));                             /* 3F91E0  move.w  d0,(a3) */
    a1 = 0x3F94B2;                                       /* 3F91E2  lea  $3f94b2(pc),a1 */
    wr32(a1, rd32(0x70));                                /* 3F91E6  move.l  $70.w,(a1) */
    a2 = 0x3F92AC;                                       /* 3F91EA  lea  $3f92ac(pc),a2 */
    wr32(0x70, a2);                                      /* 3F91EE  move.l  a2,$70.w */
    /* 3F91F2  move  #$2300,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2300U;
    set_sr(s); }
L_3F91F6:
    d3 = 6;                                              /* 3F91F6  moveq  #6,d3 */
    a1 = 0x3F94C2;                                       /* 3F91F8  lea  $3f94c2(pc),a1 */
    do {
        BUDGET_CHECK(0x3F91FCU);
L_3F91FC:
        /* 3F91FC  stop  #$2300 */
        set_sr(0x2300);
        STOP_WAIT(0x3F9200U);
L_3F9200:
        /* 3F9200  stop  #$2300 */
        set_sr(0x2300);
        STOP_WAIT(0x3F9204U);
L_3F9204:
        a2 = 0xFFFF8240;                                     /* 3F9204  lea  $8240.w,a2 */
        d4 = 0xF;                                            /* 3F9208  moveq  #15,d4 */
        wr16(a3 + 0x2, 0);                                   /* 3F920A  clr.w  $2(a3) */
        do {
            BUDGET_CHECK(0x3F920EU);
L_3F920E:
            /* 3F920E  move.w  (a2),(a1)+ */
            {
                uint32_t v = rd16(a2);
                wr16(a1, v);
                a1 += 2;
            }
            SETW(d0, rd16(a2));                                  /* 3F9210  move.w  (a2),d0 */
            SETW(d0, d0 & 0x700);                                /* 3F9212  andi.w  #$700,d0 */
            /* 3F9216  cmp.w  d5,d0 */
            {
                uint32_t s = (d5 & 0xFFFF);
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                fn = !!(r & 0x8000);
                fz = (r == 0);
                fv = !!(((s ^ d) & (d ^ r)) & 0x8000);
            }
            /* 3F9218  beq  $3f9226 */
            if (COND(6)) {
L_3F921A:
                /* 3F921A  bgt  $3f9222 */
                if (COND(15)) {
L_3F921C:
                    SETW(d0, d0 + 0x100);                                /* 3F921C  addi.w  #$100,d0 */
                    /* 3F9220  bra  $3f9226 */
                } else {
L_3F9222:
                    SETW(d0, d0 - 0x100);                                /* 3F9222  subi.w  #$100,d0 */
                }
            }
L_3F9226:
            SETW(d1, rd16(a2));                                  /* 3F9226  move.w  (a2),d1 */
            SETW(d1, d1 & 0x70);                                 /* 3F9228  andi.w  #$70,d1 */
            /* 3F922C  cmp.w  d6,d1 */
            {
                uint32_t s = (d6 & 0xFFFF);
                uint32_t d = (d1 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                fn = !!(r & 0x8000);
                fz = (r == 0);
                fv = !!(((s ^ d) & (d ^ r)) & 0x8000);
            }
            /* 3F922E  beq  $3f923c */
            if (COND(6)) {
L_3F9230:
                /* 3F9230  bgt  $3f9238 */
                if (COND(15)) {
L_3F9232:
                    SETW(d1, d1 + 0x10);                                 /* 3F9232  addi.w  #$10,d1 */
                    /* 3F9236  bra  $3f923c */
                } else {
L_3F9238:
                    SETW(d1, d1 - 0x10);                                 /* 3F9238  subi.w  #$10,d1 */
                }
            }
L_3F923C:
            SETW(d2, rd16(a2));                                  /* 3F923C  move.w  (a2),d2 */
            SETW(d2, d2 & 7);                                    /* 3F923E  andi.w  #$7,d2 */
            /* 3F9242  cmp.w  d7,d2 */
            {
                uint32_t s = (d7 & 0xFFFF);
                uint32_t d = (d2 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                fn = !!(r & 0x8000);
                fz = (r == 0);
                fv = !!(((s ^ d) & (d ^ r)) & 0x8000);
            }
            /* 3F9244  beq  $3f9250 */
            if (COND(6)) {
L_3F9246:
                /* 3F9246  bgt  $3f924c */
                if (COND(15)) {
L_3F9248:
                    SETW(d2, d2 + 1);                                    /* 3F9248  addq.w  #1,d2 */
                    /* 3F924A  bra  $3f9250 */
                } else {
L_3F924C:
                    SETW(d2, d2 - 1);                                    /* 3F924C  subi.w  #$1,d2 */
                }
            }
L_3F9250:
            SETW(d0, d0 | (d1 & 0xFFFF));                        /* 3F9250  or.w  d1,d0 */
            SETW(d0, d0 | (d2 & 0xFFFF));                        /* 3F9252  or.w  d2,d0 */
            /* 3F9254  move.w  d0,(a2)+ */
            {
                uint32_t v = (d0 & 0xFFFF);
                wr16(a2, v);
                a2 += 2;
            }
            /* 3F9256  cmp.w  (a3),d0 */
            /* 3F9258  bne  $3f925e */
            if ((d0 & 0xFFFF) == rd16(a3)) {
L_3F925A:
                wr16(a3 + 0x2, rd16(a3 + 0x2) + 1);                  /* 3F925A  addq.w  #1,$2(a3) */
            }
L_3F925E:
            /* 3F925E  dbra  d4,$3f920e */
            SETW(d4, d4 - 1);
        } while ((d4 & 0xFFFF) != 0xFFFF);
L_3F9262:
        /* 3F9262  cmpi.w  #$10,$2(a3) */
        /* 3F9268  beq  $3f926e */
        if (rd16(a3 + 0x2) == 0x10) {
            goto L_3F926E;
        }
L_3F926A:
        /* 3F926A  dbra  d3,$3f91fc */
        SETW(d3, d3 - 1);
    } while ((d3 & 0xFFFF) != 0xFFFF);
L_3F926E:
    d4 = 6;                                              /* 3F926E  moveq  #6,d4 */
    SETW(d4, d4 - (d3 & 0xFFFF));                        /* 3F9270  sub.w  d3,d4 */
    a4 = 0x3F94B6;                                       /* 3F9272  lea  $3f94b6(pc),a4 */
    wr16(a4, (d4 & 0xFFFF));                             /* 3F9276  move.w  d4,(a4) */
    /* 3F9278  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_3F927C:
    wr32(0x70, rd32(0x3F94B2));                          /* 3F927C  move.l  $3f94b2(pc),$70.w */
    SETB(d1, rd8(0x3F8083));                             /* 3F9282  move.b  $3f8083(pc),d1 */
    /* 3F9286  cmp.b  #$5,d1 */
    /* 3F928A  beq  $3f92ae */
    if ((d1 & 0xFF) != 5) {
L_3F928C:
        /* 3F928C  cmp.b  #$4,d1 */
        /* 3F9290  bne  $3f92e4 */
        if ((d1 & 0xFF) != 4) {
            goto L_3F92E4;
        }
        a2 = rd32(0x3F8866);                                 /* 3F9294  movea.l  $3f8866(pc),a2 */
        a2 = a2 + 0xC0;                                      /* 3F9298  lea  $c0(a2),a2 */
        wr32(a2, 0x200009);                                  /* 3F929C  move.l  #$200009,(a2) */
        /* 3F92A2  move.l  #$19,$8(a2) */
        {
            uint32_t v = 0x19;
            wr32(a2 + 0x8, v);
            FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
        }
        /* 3F92AA  bra  $3f92da */
    } else {
        a2 = rd32(0x3F885A);                                 /* 3F92AE  movea.l  $3f885a(pc),a2 */
        SETB(d1, rd8(0x3F8087));                             /* 3F92B2  move.b  $3f8087(pc),d1 */
        /* 3F92B6  cmp.b  #$4,d1 */
        /* 3F92BA  bne  $3f92cc */
        if ((d1 & 0xFF) == 4) {
            wr32(a2, 0x100001);                                  /* 3F92BC  move.l  #$100001,(a2) */
            /* 3F92C2  move.l  #$1,$8(a2) */
            {
                uint32_t v = 1;
                wr32(a2 + 0x8, v);
                FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
            }
            /* 3F92CA  bra  $3f92da */
        } else {
            wr32(a2, 0x200001);                                  /* 3F92CC  move.l  #$200001,(a2) */
            /* 3F92D2  move.l  #$1,$8(a2) */
            {
                uint32_t v = 1;
                wr32(a2 + 0x8, v);
                FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
            }
        }
    }
    /* 3F92DA  nop */
    SOFT(0x3F92DCU);                                     /* 3F92DC  soft */
L_3F92E4:
    a5 = 0x8;                                            /* 3F92E4  lea  $8.w,a5 */
    SETW(d0, rd16(0x3F8098));                            /* 3F92E8  move.w  $3f8098(pc),d0 */
    /* 3F92EC  move.l  $3f809c(pc),d1 */
    /* 3F92F0  beq  $3f92f8 */
    d1 = rd32(0x3F809C);
    if (d1 != 0) {
L_3F92F2:
        a6 = d1;                                             /* 3F92F2  movea.l  d1,a6 */
        a6 += 8;                                             /* 3F92F4  addq.l  #8,a6 */
        /* 3F92F6  bra  $3f934a */
    } else {
        a6 = rd32(0x3F8094);                                 /* 3F92F8  movea.l  $3f8094(pc),a6 */
        do {
            BUDGET_CHECK(0x3F92FCU);
L_3F92FC:
            d1 = rd32(a5);                                       /* 3F92FC  move.l  (a5),d1 */
            /* 3F92FE  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9300  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9302  move.l  (a5),d1 */
            /* 3F9304  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9306  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9308  move.l  (a5),d1 */
            /* 3F930A  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F930C  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F930E  move.l  (a5),d1 */
            /* 3F9310  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9312  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9314  move.l  (a5),d1 */
            /* 3F9316  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9318  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F931A  move.l  (a5),d1 */
            /* 3F931C  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F931E  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9320  move.l  (a5),d1 */
            /* 3F9322  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9324  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9326  move.l  (a5),d1 */
            /* 3F9328  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F932A  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F932C  move.l  (a5),d1 */
            /* 3F932E  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9330  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9332  move.l  (a5),d1 */
            /* 3F9334  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9336  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9338  move.l  (a5),d1 */
            /* 3F933A  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F933C  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F933E  move.l  (a5),d1 */
            /* 3F9340  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9342  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            /* 3F9344  dbra  d0,$3f92fc */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
        /* 3F9348  bra  $3f9376 */
        goto L_3F9376;
    }
    do {
        BUDGET_CHECK(0x3F934AU);
L_3F934A:
        /* 3F934A  movem.l  (a5)+,d1-d7/a0-a4 */
        { uint32_t ea;
        ea = a5;
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
        a0 = rd32(ea);
        ea += 4;
        a1 = rd32(ea);
        ea += 4;
        a2 = rd32(ea);
        ea += 4;
        a3 = rd32(ea);
        ea += 4;
        a4 = rd32(ea);
        ea += 4;
        a5 = ea;
        }
        /* 3F934E  movem.l  d1-d7/a0-a4,(a6) */
        { uint32_t ea;
        ea = a6;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        wr32(ea, a0); ea += 4;
        wr32(ea, a1); ea += 4;
        wr32(ea, a2); ea += 4;
        wr32(ea, a3); ea += 4;
        wr32(ea, a4); ea += 4;
        }
        a6 = a6 + 0x30;                                      /* 3F9352  lea  $30(a6),a6 */
        /* 3F9356  dbra  d0,$3f934a */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_3F935A:
    a6 = 0x8;                                            /* 3F935A  lea  $8.w,a6 */
    a5 = rd32(0x3F8094);                                 /* 3F935E  movea.l  $3f8094(pc),a5 */
    SETW(d0, rd16(0x3F8098));                            /* 3F9362  move.w  $3f8098(pc),d0 */
    do {
        BUDGET_CHECK(0x3F9366U);
L_3F9366:
        /* 3F9366  movem.l  (a5)+,d1-d7/a0-a4 */
        { uint32_t ea;
        ea = a5;
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
        a0 = rd32(ea);
        ea += 4;
        a1 = rd32(ea);
        ea += 4;
        a2 = rd32(ea);
        ea += 4;
        a3 = rd32(ea);
        ea += 4;
        a4 = rd32(ea);
        ea += 4;
        a5 = ea;
        }
        /* 3F936A  movem.l  d1-d7/a0-a4,(a6) */
        { uint32_t ea;
        ea = a6;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        wr32(ea, a0); ea += 4;
        wr32(ea, a1); ea += 4;
        wr32(ea, a2); ea += 4;
        wr32(ea, a3); ea += 4;
        wr32(ea, a4); ea += 4;
        }
        a6 = a6 + 0x30;                                      /* 3F936E  lea  $30(a6),a6 */
        /* 3F9372  dbra  d0,$3f9366 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_3F9376:
    /* 3F9376  move  #$2300,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2300U;
    set_sr(s); }
L_3F937A:
    RTS();                                               /* 3F937A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F9198U: goto L_3F9198;
        case 0x3F91B6U: goto L_3F91B6;
        case 0x3F91F6U: goto L_3F91F6;
        case 0x3F91FCU: goto L_3F91FC;
        case 0x3F9200U: goto L_3F9200;
        case 0x3F9204U: goto L_3F9204;
        case 0x3F920EU: goto L_3F920E;
        case 0x3F921AU: goto L_3F921A;
        case 0x3F921CU: goto L_3F921C;
        case 0x3F9222U: goto L_3F9222;
        case 0x3F9226U: goto L_3F9226;
        case 0x3F9230U: goto L_3F9230;
        case 0x3F9232U: goto L_3F9232;
        case 0x3F9238U: goto L_3F9238;
        case 0x3F923CU: goto L_3F923C;
        case 0x3F9246U: goto L_3F9246;
        case 0x3F9248U: goto L_3F9248;
        case 0x3F924CU: goto L_3F924C;
        case 0x3F9250U: goto L_3F9250;
        case 0x3F925AU: goto L_3F925A;
        case 0x3F925EU: goto L_3F925E;
        case 0x3F9262U: goto L_3F9262;
        case 0x3F926AU: goto L_3F926A;
        case 0x3F926EU: goto L_3F926E;
        case 0x3F927CU: goto L_3F927C;
        case 0x3F928CU: goto L_3F928C;
        case 0x3F92E4U: goto L_3F92E4;
        case 0x3F92F2U: goto L_3F92F2;
        case 0x3F92FCU: goto L_3F92FC;
        case 0x3F934AU: goto L_3F934A;
        case 0x3F935AU: goto L_3F935A;
        case 0x3F9366U: goto L_3F9366;
        case 0x3F9376U: goto L_3F9376;
        case 0x3F937AU: goto L_3F937A;
        }
    }
}

/* 3F92AC  game routine */
void sub_3F92AC(void)
{
    RTE();                                               /* 3F92AC  rte */
}

/* 3F937C  game routine */
void sub_3F937C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETB(d1, rd8(0x3F8083));                             /* 3F937C  move.b  $3f8083(pc),d1 */
    /* 3F9380  cmp.b  #$5,d1 */
    /* 3F9384  beq  $3f93a6 */
    if ((d1 & 0xFF) != 5) {
L_3F9386:
        /* 3F9386  cmp.b  #$4,d1 */
        /* 3F938A  bne  $3f93dc */
        if ((d1 & 0xFF) != 4) {
            goto L_3F93DC;
        }
        a2 = rd32(0x3F8866);                                 /* 3F938E  movea.l  $3f8866(pc),a2 */
        a2 = a2 + 0xC0;                                      /* 3F9392  lea  $c0(a2),a2 */
        wr32(a2, 0x19);                                      /* 3F9396  move.l  #$19,(a2) */
        /* 3F939C  move.l  #$200009,$8(a2) */
        {
            uint32_t v = 0x200009;
            wr32(a2 + 0x8, v);
            FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
        }
        /* 3F93A4  bra  $3f93d2 */
    } else {
        a2 = rd32(0x3F885A);                                 /* 3F93A6  movea.l  $3f885a(pc),a2 */
        SETB(d1, rd8(0x3F8087));                             /* 3F93AA  move.b  $3f8087(pc),d1 */
        /* 3F93AE  cmp.b  #$4,d1 */
        /* 3F93B2  bne  $3f93c4 */
        if ((d1 & 0xFF) == 4) {
            wr32(a2, 1);                                         /* 3F93B4  move.l  #$1,(a2) */
            /* 3F93BA  move.l  #$100001,$8(a2) */
            {
                uint32_t v = 0x100001;
                wr32(a2 + 0x8, v);
                FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
            }
            /* 3F93C2  bra  $3f93d2 */
        } else {
            wr32(a2, 1);                                         /* 3F93C4  move.l  #$1,(a2) */
            /* 3F93CA  move.l  #$200001,$8(a2) */
            {
                uint32_t v = 0x200001;
                wr32(a2 + 0x8, v);
                FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
            }
        }
    }
    /* 3F93D2  nop */
    SOFT(0x3F93D4U);                                     /* 3F93D4  soft */
L_3F93DC:
    a5 = 0x8;                                            /* 3F93DC  lea  $8.w,a5 */
    a6 = rd32(0x3F8094);                                 /* 3F93E0  movea.l  $3f8094(pc),a6 */
    SETW(d0, rd16(0x3F8098));                            /* 3F93E4  move.w  $3f8098(pc),d0 */
    /* 3F93E8  move.l  $3f809c(pc),d1 */
    /* 3F93EC  bne  $3f943c */
    d1 = rd32(0x3F809C);
    if (d1 == 0) {
        do {
            BUDGET_CHECK(0x3F93EEU);
L_3F93EE:
            d1 = rd32(a5);                                       /* 3F93EE  move.l  (a5),d1 */
            /* 3F93F0  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F93F2  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F93F4  move.l  (a5),d1 */
            /* 3F93F6  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F93F8  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F93FA  move.l  (a5),d1 */
            /* 3F93FC  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F93FE  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9400  move.l  (a5),d1 */
            /* 3F9402  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9404  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9406  move.l  (a5),d1 */
            /* 3F9408  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F940A  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F940C  move.l  (a5),d1 */
            /* 3F940E  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9410  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9412  move.l  (a5),d1 */
            /* 3F9414  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9416  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9418  move.l  (a5),d1 */
            /* 3F941A  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F941C  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F941E  move.l  (a5),d1 */
            /* 3F9420  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9422  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9424  move.l  (a5),d1 */
            /* 3F9426  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9428  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F942A  move.l  (a5),d1 */
            /* 3F942C  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F942E  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            d1 = rd32(a5);                                       /* 3F9430  move.l  (a5),d1 */
            /* 3F9432  move.l  (a6),(a5)+ */
            {
                uint32_t v = rd32(a6);
                wr32(a5, v);
                a5 += 4;
            }
            /* 3F9434  move.l  d1,(a6)+ */
            {
                uint32_t v = d1;
                wr32(a6, v);
                a6 += 4;
            }
            /* 3F9436  dbra  d0,$3f93ee */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
        /* 3F943A  bra  $3f946a */
    } else {
        do {
            BUDGET_CHECK(0x3F943CU);
L_3F943C:
            /* 3F943C  movem.l  (a5)+,d1-d7/a0-a4 */
            { uint32_t ea;
            ea = a5;
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
            a0 = rd32(ea);
            ea += 4;
            a1 = rd32(ea);
            ea += 4;
            a2 = rd32(ea);
            ea += 4;
            a3 = rd32(ea);
            ea += 4;
            a4 = rd32(ea);
            ea += 4;
            a5 = ea;
            }
            /* 3F9440  movem.l  d1-d7/a0-a4,(a6) */
            { uint32_t ea;
            ea = a6;
            wr32(ea, d1); ea += 4;
            wr32(ea, d2); ea += 4;
            wr32(ea, d3); ea += 4;
            wr32(ea, d4); ea += 4;
            wr32(ea, d5); ea += 4;
            wr32(ea, d6); ea += 4;
            wr32(ea, d7); ea += 4;
            wr32(ea, a0); ea += 4;
            wr32(ea, a1); ea += 4;
            wr32(ea, a2); ea += 4;
            wr32(ea, a3); ea += 4;
            wr32(ea, a4); ea += 4;
            }
            a6 = a6 + 0x30;                                      /* 3F9444  lea  $30(a6),a6 */
            /* 3F9448  dbra  d0,$3f943c */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
L_3F944C:
        a6 = 0x8;                                            /* 3F944C  lea  $8.w,a6 */
        a5 = rd32(0x3F809C);                                 /* 3F9450  movea.l  $3f809c(pc),a5 */
        a5 += 8;                                             /* 3F9454  addq.l  #8,a5 */
        SETW(d0, rd16(0x3F8098));                            /* 3F9456  move.w  $3f8098(pc),d0 */
        do {
            BUDGET_CHECK(0x3F945AU);
L_3F945A:
            /* 3F945A  movem.l  (a5)+,d1-d7/a0-a4 */
            { uint32_t ea;
            ea = a5;
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
            a0 = rd32(ea);
            ea += 4;
            a1 = rd32(ea);
            ea += 4;
            a2 = rd32(ea);
            ea += 4;
            a3 = rd32(ea);
            ea += 4;
            a4 = rd32(ea);
            ea += 4;
            a5 = ea;
            }
            /* 3F945E  movem.l  d1-d7/a0-a4,(a6) */
            { uint32_t ea;
            ea = a6;
            wr32(ea, d1); ea += 4;
            wr32(ea, d2); ea += 4;
            wr32(ea, d3); ea += 4;
            wr32(ea, d4); ea += 4;
            wr32(ea, d5); ea += 4;
            wr32(ea, d6); ea += 4;
            wr32(ea, d7); ea += 4;
            wr32(ea, a0); ea += 4;
            wr32(ea, a1); ea += 4;
            wr32(ea, a2); ea += 4;
            wr32(ea, a3); ea += 4;
            wr32(ea, a4); ea += 4;
            }
            a6 = a6 + 0x30;                                      /* 3F9462  lea  $30(a6),a6 */
            /* 3F9466  dbra  d0,$3f945a */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
    }
L_3F946A:
    a1 = 0x3F94B2;                                       /* 3F946A  lea  $3f94b2(pc),a1 */
    wr32(a1, rd32(0x70));                                /* 3F946E  move.l  $70.w,(a1) */
    a2 = 0x3F92AC;                                       /* 3F9472  lea  $3f92ac(pc),a2 */
    wr32(0x70, a2);                                      /* 3F9476  move.l  a2,$70.w */
    /* 3F947A  move  #$2300,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2300U;
    set_sr(s); }
L_3F947E:
    SETW(d1, rd16(0x3F94B6));                            /* 3F947E  move.w  $3f94b6(pc),d1 */
    a1 = 0x3F94C2;                                       /* 3F9482  lea  $3f94c2(pc),a1 */
    SETW(d0, (d1 & 0xFFFF));                             /* 3F9486  move.w  d1,d0 */
    SETW(d0, d0 + 1);                                    /* 3F9488  addq.w  #1,d0 */
    SETW(d0, (d0 & 0xFFFF) << 5);                        /* 3F948A  lsl.w  #5,d0 */
    a1 += SX16((d0 & 0xFFFF));                           /* 3F948C  adda.w  d0,a1 */
    do {
        BUDGET_CHECK(0x3F948EU);
L_3F948E:
        /* 3F948E  stop  #$2300 */
        set_sr(0x2300);
        STOP_WAIT(0x3F9492U);
L_3F9492:
        /* 3F9492  stop  #$2300 */
        set_sr(0x2300);
        STOP_WAIT(0x3F9496U);
L_3F9496:
        a2 = 0xFFFF8260;                                     /* 3F9496  lea  $8260.w,a2 */
        d2 = 0xF;                                            /* 3F949A  moveq  #15,d2 */
        do {
            BUDGET_CHECK(0x3F949CU);
L_3F949C:
            /* 3F949C  move.w  -(a1),-(a2) */
            a1 -= 2;
            {
                uint32_t v = rd16(a1);
                a2 -= 2;
                wr16(a2, v);
            }
            /* 3F949E  dbra  d2,$3f949c */
            SETW(d2, d2 - 1);
        } while ((d2 & 0xFFFF) != 0xFFFF);
L_3F94A2:
        /* 3F94A2  dbra  d1,$3f948e */
        SETW(d1, d1 - 1);
    } while ((d1 & 0xFFFF) != 0xFFFF);
L_3F94A6:
    /* 3F94A6  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_3F94AA:
    wr32(0x70, rd32(0x3F94B2));                          /* 3F94AA  move.l  $3f94b2(pc),$70.w */
    RTS();                                               /* 3F94B0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F9386U: goto L_3F9386;
        case 0x3F93DCU: goto L_3F93DC;
        case 0x3F93EEU: goto L_3F93EE;
        case 0x3F943CU: goto L_3F943C;
        case 0x3F944CU: goto L_3F944C;
        case 0x3F945AU: goto L_3F945A;
        case 0x3F946AU: goto L_3F946A;
        case 0x3F947EU: goto L_3F947E;
        case 0x3F948EU: goto L_3F948E;
        case 0x3F9492U: goto L_3F9492;
        case 0x3F9496U: goto L_3F9496;
        case 0x3F949CU: goto L_3F949C;
        case 0x3F94A2U: goto L_3F94A2;
        case 0x3F94A6U: goto L_3F94A6;
        case 0x3F94AAU: goto L_3F94AA;
        }
    }
}

/* 3F94B2  game routine */
void sub_3F94B2(void)
{
    /* 3F94B2  ori.b  #$a8,d0 */
    SETB(d0, d0 | 0xA8);
    { sub_3F94B6(); return; }
}

/* 3F94B6  game routine */
void sub_3F94B6(void)
{
    /* 3F94B6  ori.b  #$2e,d0 */
    {
        uint32_t s = 0x2E;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x3F94BAU);
}

/* 3F94B8  game routine */
void sub_3F94B8(void)
{
    /* 3F94B8  ori.b  #$33,$23(a6) */
    {
        uint32_t s = 0x33;
        uint32_t d = rd8(a6 + 0x23);
        uint32_t r = (d | s) & 0xFF;
        wr8(a6 + 0x23, r);
        fn = !!(r & 0x80);
        fv = 0;
        fc = 0;
    }
    /* 3F94BE  btst  d0,(a1) */
    fz = !(rd8(a1) & (1u << (d0 & 7)));
    MISS(0x3F94C0U);
}

/* 3F94C2  game routine */
void sub_3F94C2(void)
{
    SETB(d0, d0 | 0x62);                                 /* 3F94C2  ori.b  #$62,d0 */
    wr8(a5, rd8(a5) ^ (1u << (d2 & 7)));                 /* 3F94C6  bchg  d2,(a5) */
    /* 3F94C8  subi.w  #$233,d3 */
    {
        uint32_t s = 0x233;
        uint32_t d = (d3 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d3, r);
        fx = (s > d);
    }
    wr8(a2, rd8(a2) ^ (1u << (d0 & 7)));                 /* 3F94CC  bchg  d0,(a2) */
    a0 -= 1;                                             /* 3F94CE  btst  d2,-(a0) */
    /* 3F94D0  ori.b  #$76,-(a1) */
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
    /* 3F94D4  btst  d0,-(a2) */
    a2 -= 1;
    fz = !(rd8(a2) & (1u << (d0 & 7)));
    MISS(0x3F94D6U);
}

/* 3F95A2  game routine */
void sub_3F95A2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 3F95A2  move.w  -$cc(a6),-(a7) */
    {
        uint32_t v = rd16(a6 - 0xCC);
        a7 -= 2;
        wr16(a7, v);
    }
    /* 3F95A6  move.w  #$e,-(a7) */
    {
        uint32_t v = 0xE;
        a7 -= 2;
        wr16(a7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    a1 = rd32(a6 + 0x14);                                /* 3F95AA  movea.l  $14(a6),a1 */
    /* 3F95AE  jsr  (a1) */
    { uint32_t ea;
    ea = a1;
    CALL_DYN(ea, 0x3F95B0U); }
L_3F95B0:
    a7 += 4;                                             /* 3F95B0  addq.l  #4,a7 */
    PUSH32(a6 - 0xA0);                                   /* 3F95B2  pea  -$a0(a6) */
    /* 3F95B6  move.w  #$3b,-(a7) */
    {
        uint32_t v = 0x3B;
        a7 -= 2;
        wr16(a7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    a1 = rd32(a6 + 0x14);                                /* 3F95BA  movea.l  $14(a6),a1 */
    /* 3F95BE  jsr  (a1) */
    { uint32_t ea;
    ea = a1;
    CALL_DYN(ea, 0x3F95C0U); }
L_3F95C0:
    a7 += 6;                                             /* 3F95C0  addq.l  #6,a7 */
    RTS();                                               /* 3F95C2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F95B0U: goto L_3F95B0;
        case 0x3F95C0U: goto L_3F95C0;
        }
    }
}

/* 3F9674  game routine */
void sub_3F9674(void)
{
    /* 3F9674  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_3F9678(); return; }
}

/* 3F9678  game routine */
void sub_3F9678(void)
{
    RTS();                                               /* 3F9678  rts */
}

/* 3F967E  game routine */
void sub_3F967E(void)
{
    SETB(d0, d0 | 0);                                    /* 3F967E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9682  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9686  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F968A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F968E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9692  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9696  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F969A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F969E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F96A2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F96A6  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F96AA  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F96AE  ori.b  #$0,d0 */
    /* 3F96B2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_3F96B6(); return; }
}

/* 3F96B6  game routine */
void sub_3F96B6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x3F9788;                                       /* 3F96B6  lea  $3f9788(pc),a1 */
    d1 = 0xF;                                            /* 3F96BA  moveq  #15,d1 */
    do {
        BUDGET_CHECK(0x3F96BCU);
L_3F96BC:
        /* 3F96BC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
            FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
        }
        /* 3F96BE  dbra  d1,$3f96bc */
        SETW(d1, d1 - 1);
    } while ((d1 & 0xFFFF) != 0xFFFF);
    a1 = 0x3FB3B0;                                       /* 3F96C2  lea  $3fb3b0(pc),a1 */
    /* 3F96C6  move  sr,-(a1) */
    { uint32_t ea; (void)ea;
    a1 -= 2; ea = a1;
    wr16(ea, get_sr());
    }
    /* 3F96C8  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    /* 3F96CC  movem.l  a7/a6/a5/a4/a3/a2/d7/d6/d5/d4/d3/d2,-(a1) */
    { uint32_t ea;
    ea = a1;
    ea -= 4; wr32(ea, a7);
    ea -= 4; wr32(ea, a6);
    ea -= 4; wr32(ea, a5);
    ea -= 4; wr32(ea, a4);
    ea -= 4; wr32(ea, a3);
    ea -= 4; wr32(ea, a2);
    ea -= 4; wr32(ea, d7);
    ea -= 4; wr32(ea, d6);
    ea -= 4; wr32(ea, d5);
    ea -= 4; wr32(ea, d4);
    ea -= 4; wr32(ea, d3);
    ea -= 4; wr32(ea, d2);
    a1 = ea;
    }
    a2 = other_sp;                                       /* 3F96D0  move  usp,a2 */
    /* 3F96D2  move.l  a2,-(a1) */
    {
        uint32_t v = a2;
        a1 -= 4;
        wr32(a1, v);
    }
    a2 = 0x3FB7A2;                                       /* 3F96D4  lea  $3fb7a2(pc),a2 */
    other_sp = a2;                                       /* 3F96D8  move  a2,usp */
    a7 = 0x3FB612;                                       /* 3F96DA  lea  $3fb612(pc),a7 */
    CALL(sub_3F97C8, 0x3F96E2U);                         /* 3F96DE  bsr  $3f97c8 */
L_3F96E2:
    CALL(sub_3F9170, 0x3F96E6U);                         /* 3F96E2  bsr  $3f9170 */
L_3F96E6:
    /* 3F96E6  move.w  #$2f,-(a7) */
    {
        uint32_t v = 0x2F;
        a7 -= 2;
        wr16(a7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    SOFT(0x3F96EAU);                                     /* 3F96EA  trap  #1 */
L_3F96EC:
    a7 += 2;                                             /* 3F96EC  addq.l  #2,a7 */
    a1 = 0x3F9724;                                       /* 3F96EE  lea  $3f9724(pc),a1 */
    wr32(a1, d0);                                        /* 3F96F2  move.l  d0,(a1) */
    a6 = 0x3F9788;                                       /* 3F96F4  lea  $3f9788(pc),a6 */
    /* 3F96F8  move.w  $6(a6),-(a7) */
    {
        uint32_t v = rd16(a6 + 0x6);
        a7 -= 2;
        wr16(a7, v);
    }
    PUSH32(a6 + 0x8);                                    /* 3F96FC  pea  $8(a6) */
    /* 3F9700  move.w  #$4e,-(a7) */
    {
        uint32_t v = 0x4E;
        a7 -= 2;
        wr16(a7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    SOFT(0x3F9704U);                                     /* 3F9704  trap  #1 */
L_3F9706:
    a7 += 8;                                             /* 3F9706  addq.l  #8,a7 */
    a1 = 0x3F9766;                                       /* 3F9708  lea  $3f9766(pc),a1 */
    /* 3F970C  move.l  d0,(a1) */
    {
        uint32_t v = d0;
        wr32(a1, v);
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
    /* 3F970E  bpl  $3f971c */
    if (COND(11)) {
        a1 = rd32(a6 + 0x2);                                 /* 3F9710  movea.l  $2(a6),a1 */
        wr16(a1, 0xFFCF);                                    /* 3F9714  move.w  #$ffcf,(a1) */
        /* 3F9718  bra  $3f9744 */
        { sub_3F9744(); return; }
    }
    d7 = 0;                                              /* 3F971C  moveq  #0,d7 */
    /* 3F971E  movea.l  $2(a6),a5 */
    a5 = rd32(a6 + 0x2);
    { sub_3F9722(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F96BCU: goto L_3F96BC;
        case 0x3F96E2U: goto L_3F96E2;
        case 0x3F96E6U: goto L_3F96E6;
        case 0x3F96ECU: goto L_3F96EC;
        case 0x3F9706U: goto L_3F9706;
        }
    }
}

/* 3F9722  game routine */
void sub_3F9722(void)
{
    /* 3F9722  lea  $0.l,a1 */
    a1 = 0x0;
    { sub_3F9728(); return; }
}

/* 3F9724  game routine */
void sub_3F9724(void)
{
    /* 3F9724  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_3F9728(); return; }
}

/* 3F9728  game routine */
void sub_3F9728(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 3F9728  move.w  d0,(a5)+ */
    {
        uint32_t v = (d0 & 0xFFFF);
        wr16(a5, v);
        a5 += 2;
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    /* 3F972A  bmi  $3f9744 */
    if (COND(11)) { sub_3F9744(); return; }
    d1 = 0x3D;                                           /* 3F972C  moveq  #61,d1 */
    do {
        BUDGET_CHECK(0x3F972EU);
L_3F972E:
        /* 3F972E  move.b  (a1)+,(a5)+ */
        {
            uint32_t v = rd8(a1);
            a1 += 1;
            wr8(a5, v);
            a5 += 1;
        }
        /* 3F9730  dbra  d1,$3f972e */
        SETW(d1, d1 - 1);
    } while ((d1 & 0xFFFF) != 0xFFFF);
    /* 3F9734  cmp.w  (a6),d7 */
    /* 3F9736  beq  $3f9744 */
    if ((d7 & 0xFFFF) == rd16(a6)) {
        { sub_3F9744(); return; }
    }
    /* 3F9738  move.w  #$4f,-(a7) */
    {
        uint32_t v = 0x4F;
        a7 -= 2;
        wr16(a7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    SOFT(0x3F973CU);                                     /* 3F973C  trap  #1 */
L_3F973E:
    a7 += 2;                                             /* 3F973E  addq.l  #2,a7 */
    /* 3F9740  addq.w  #1,d7 */
    {
        uint32_t s = 1;
        uint32_t d = (d7 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d7, r);
        fx = (r < s);
    }
    /* 3F9742  bra  $3f9722 */
    { sub_3F9722(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F972EU: goto L_3F972E;
        case 0x3F973EU: goto L_3F973E;
        }
    }
}

/* 3F9744  game routine */
void sub_3F9744(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 3F9744  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    CALL(sub_3F937C, 0x3F974CU);                         /* 3F9748  bsr  $3f937c */
L_3F974C:
    a6 = 0x3FB3C2;                                       /* 3F974C  lea  $3fb3c2(pc),a6 */
    a1 = 0xFFFFFA07;                                     /* 3F9750  lea  $fa07.w,a1 */
    /* 3F9754  move.b  (a6)+,(a1) */
    wr8(a1, rd8(a6));
    a6 += 1;
    a1 += 2;                                             /* 3F9756  addq.l  #2,a1 */
    /* 3F9758  move.b  (a6)+,(a1) */
    wr8(a1, rd8(a6));
    a6 += 1;
    a1 = a1 + 0xA;                                       /* 3F975A  lea  $a(a1),a1 */
    /* 3F975E  move.b  (a6)+,(a1) */
    wr8(a1, rd8(a6));
    a6 += 1;
    a1 += 2;                                             /* 3F9760  addq.l  #2,a1 */
    wr8(a1, rd8(a6));                                    /* 3F9762  move.b  (a6),(a1) */
    /* 3F9764  move.l  #$0,d0 */
    d0 = 0;
    { sub_3F976A(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F974CU: goto L_3F974C;
        }
    }
}

/* 3F9766  game routine */
void sub_3F9766(void)
{
    /* 3F9766  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_3F976A(); return; }
}

/* 3F976A  game routine */
void sub_3F976A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x3FB37A;                                       /* 3F976A  lea  $3fb37a(pc),a1 */
    /* 3F976E  movea.l  (a1)+,a2 */
    a2 = rd32(a1);
    a1 += 4;
    other_sp = a2;                                       /* 3F9770  move  a2,usp */
    /* 3F9772  movem.l  (a1)+,d2-d7/a2-a7 */
    { uint32_t ea;
    ea = a1;
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
    a5 = rd32(ea);
    ea += 4;
    a6 = rd32(ea);
    ea += 4;
    a7 = rd32(ea);
    ea += 4;
    a1 = ea;
    }
    for (;;) {
        BUDGET_CHECK(0x3F9776U);
L_3F9776:
        fz = !(rd8(HW_IKBD_CTRL) & 0x1);                     /* 3F9776  btst  #$0,$fc00.w */
        /* 3F977C  beq  $3f9784 */
        if (COND(7)) {
            goto L_3F9784;
        }
        (void)rd8(HW_IKBD_DATA);                             /* 3F977E  tst.b  $fc02.w */
        /* 3F9782  bra  $3f9776 */
    }
L_3F9784:
    /* 3F9784  move  (a1)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a1; a1 += 2;
    s = rd16(ea1);
    set_sr(s); }
    RTS();                                               /* 3F9786  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F9776U: goto L_3F9776;
        }
    }
}

/* 3F9788  game routine */
void sub_3F9788(void)
{
    SETB(d0, d0 | 0);                                    /* 3F9788  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F978C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9790  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9794  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F9798  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F979C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97A0  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97A4  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97A8  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97AC  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97B0  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97B4  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97B8  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97BC  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 3F97C0  ori.b  #$0,d0 */
    /* 3F97C4  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_3F97C8(); return; }
}

/* 3F97C8  game routine */
void sub_3F97C8(void)
{
    a6 = 0x3FB3C2;                                       /* 3F97C8  lea  $3fb3c2(pc),a6 */
    a1 = 0xFFFFFA07;                                     /* 3F97CC  lea  $fa07.w,a1 */
    SETB(d0, rd8(a1));                                   /* 3F97D0  move.b  (a1),d0 */
    SETB(d0, d0 & 0xA1);                                 /* 3F97D2  andi.b  #$a1,d0 */
    /* 3F97D6  move.b  d0,(a6)+ */
    {
        uint32_t v = (d0 & 0xFF);
        wr8(a6, v);
        a6 += 1;
    }
    wr8(a1, 0);                                          /* 3F97D8  clr.b  (a1) */
    a1 += 2;                                             /* 3F97DA  addq.l  #2,a1 */
    SETB(d0, rd8(a1));                                   /* 3F97DC  move.b  (a1),d0 */
    SETB(d0, d0 & 0x78);                                 /* 3F97DE  andi.b  #$78,d0 */
    /* 3F97E2  move.b  d0,(a6)+ */
    {
        uint32_t v = (d0 & 0xFF);
        wr8(a6, v);
        a6 += 1;
    }
    wr8(a1, 0);                                          /* 3F97E4  clr.b  (a1) */
    a1 += 2;                                             /* 3F97E6  addq.l  #2,a1 */
    wr8(a1, 0);                                          /* 3F97E8  clr.b  (a1) */
    a1 += 2;                                             /* 3F97EA  addq.l  #2,a1 */
    wr8(a1, 0);                                          /* 3F97EC  clr.b  (a1) */
    a1 += 2;                                             /* 3F97EE  addq.l  #2,a1 */
    wr8(a1, 0);                                          /* 3F97F0  clr.b  (a1) */
    a1 += 2;                                             /* 3F97F2  addq.l  #2,a1 */
    wr8(a1, 0);                                          /* 3F97F4  clr.b  (a1) */
    a1 += 2;                                             /* 3F97F6  addq.l  #2,a1 */
    SETB(d0, rd8(a1));                                   /* 3F97F8  move.b  (a1),d0 */
    SETB(d0, d0 & 0xA1);                                 /* 3F97FA  andi.b  #$a1,d0 */
    /* 3F97FE  move.b  d0,(a6)+ */
    {
        uint32_t v = (d0 & 0xFF);
        wr8(a6, v);
        a6 += 1;
    }
    wr8(a1, 0);                                          /* 3F9800  clr.b  (a1) */
    a1 += 2;                                             /* 3F9802  addq.l  #2,a1 */
    SETB(d0, rd8(a1));                                   /* 3F9804  move.b  (a1),d0 */
    SETB(d0, d0 & 0x78);                                 /* 3F9806  andi.b  #$78,d0 */
    wr8(a6, (d0 & 0xFF));                                /* 3F980A  move.b  d0,(a6) */
    wr8(a1, 0);                                          /* 3F980C  clr.b  (a1) */
    RTS();                                               /* 3F980E  rts */
}

/* 3F9F9A  game routine */
void sub_3F9F9A(void)
{
    /* 3F9F9A  ori.b  #$42,d0 */
    {
        uint32_t s = 0x42;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    SOFT(0x3F9F9EU);                                     /* 3F9F9E  soft */
}

