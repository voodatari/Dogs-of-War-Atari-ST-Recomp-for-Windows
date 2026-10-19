/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 006026  game routine */
void sub_006026(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 006026  movem.l  a6/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
    PUSH32(a6);
    PUSH32(d7);
    PUSH32(d6);
    PUSH32(d5);
    PUSH32(d4);
    PUSH32(d3);
    PUSH32(d2);
    PUSH32(d1);
    PUSH32(d0);
    a6 = V_VEHICLES;                                     /* 00602A  lea  $1a76(a5),a6 */
    SETW(d7, 4);                                         /* 00602E  move.w  #$4,d7 */
    do {
        BUDGET_CHECK(0x6032U);
L_006032:
        /* 006032  tst.w  (a6) */
        /* 006034  bne  $609c */
        if (rd16(a6) == 0) {
            /* 006038  subi.b  #$f6,d3 */
            {
                uint32_t s = 0xF6;
                uint32_t d = (d3 & 0xFF);
                uint32_t r = (d - s) & 0xFF;
                SETB(d3, r);
                fx = (s > d);
            }
            wr32(V_0198, d3);                                    /* 00603C  move.l  d3,$198(a5) */
            wr16(a6 + 0x2, (d0 & 0xFFFF));                       /* 006040  move.w  d0,$2(a6) */
            wr16(a6 + 0x4, (d1 & 0xFFFF));                       /* 006044  move.w  d1,$4(a6) */
            wr16(a6 + 0x6, (d2 & 0xFFFF));                       /* 006048  move.w  d2,$6(a6) */
            wr16(a6 + 0x10, (d3 & 0xFFFF));                      /* 00604C  move.w  d3,$10(a6) */
            wr16(a6 + 0xA, (d4 & 0xFFFF));                       /* 006050  move.w  d4,$a(a6) */
            wr16(a6 + 0xE, (d5 & 0xFFFF));                       /* 006054  move.w  d5,$e(a6) */
            wr16(a6 + 0x8, 0);                                   /* 006058  clr.w  $8(a6) */
            wr16(a6, 0xFFFF);                                    /* 00605C  move.w  #$ffff,(a6) */
            wr16(a6 + 0x2A, 0);                                  /* 006060  clr.w  $2a(a6) */
            wr16(a6 + 0x2C, 0);                                  /* 006064  clr.w  $2c(a6) */
            wr16(a6 + 0xC, 0);                                   /* 006068  clr.w  $c(a6) */
            wr32(a6 + 0x12, 0);                                  /* 00606C  clr.l  $12(a6) */
            wr32(a6 + 0x16, 0);                                  /* 006070  clr.l  $16(a6) */
            wr32(a6 + 0x1A, 0);                                  /* 006074  clr.l  $1a(a6) */
            wr32(a6 + 0x1E, 0);                                  /* 006078  clr.l  $1e(a6) */
            wr32(a6 + 0x22, 0);                                  /* 00607C  clr.l  $22(a6) */
            wr32(a6 + 0x26, 0);                                  /* 006080  clr.l  $26(a6) */
            a4 = a6 + 0x12;                                      /* 006084  lea  $12(a6),a4 */
            for (;;) {
                BUDGET_CHECK(0x6088U);
L_006088:
                /* 006088  move.b  (a0)+,d0 */
                {
                    uint32_t v = rd8(a0);
                    a0 += 1;
                    SETB(d0, v);
                    FL_LOGIC(v, 0xFF, 0x80);
                }
                /* 00608A  bmi  $60a4 */
                if (COND(11)) {
                    goto L_0060A4;
                }
                /* 00608C  move.b  (a0)+,d1 */
                SETB(d1, rd8(a0));
                a0 += 1;
                /* 00608E  move.b  (a0)+,d2 */
                SETB(d2, rd8(a0));
                a0 += 1;
                /* 006090  move.b  (a0)+,d3 */
                SETB(d3, rd8(a0));
                a0 += 1;
                CALL(sub_00B424, 0x6096U);                           /* 006092  bsr  $b424 */
L_006096:
                /* 006096  move.l  $2d6(a5),(a4)+ */
                {
                    uint32_t v = rd32(V_02D6);
                    wr32(a4, v);
                    a4 += 4;
                }
                /* 00609A  bra  $6088 */
            }
        }
        a6 = a6 + 0x2E;                                      /* 00609C  lea  $2e(a6),a6 */
        /* 0060A0  dbra  d7,$6032 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_0060A4:
    /* 0060A4  movem.l  (a7)+,d0-d7/a6 */
    d0 = POP32();
    d1 = POP32();
    d2 = POP32();
    d3 = POP32();
    d4 = POP32();
    d5 = POP32();
    d6 = POP32();
    d7 = POP32();
    a6 = POP32();
    RTS();                                               /* 0060A8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6032U: goto L_006032;
        case 0x6088U: goto L_006088;
        case 0x6096U: goto L_006096;
        }
    }
}

/* 0060AA  game routine */
void sub_0060AA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0060AA  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    d5 = 0;                                              /* 0060AE  moveq  #0,d5 */
    a6 = V_VEHICLES;                                     /* 0060B0  lea  $1a76(a5),a6 */
    SETW(d7, 4);                                         /* 0060B4  move.w  #$4,d7 */
    do {
        BUDGET_CHECK(0x60B8U);
L_0060B8:
        PUSH16(d7);                                          /* 0060B8  movem.w  d7,-(a7) */
        /* 0060BC  tst.w  (a6) */
        /* 0060BE  beq  $60de */
        if (rd16(a6) != 0) {
L_0060C0:
            /* 0060C0  cmpi.w  #$4,$10(a6) */
            /* 0060C6  bne  $60ce */
            if (rd16(a6 + 0x10) == 4) {
                CALL(draw_vehicle_type4, 0x60CCU);                   /* 0060C8  bsr  $62c0 */
L_0060CC:
                /* 0060CC  bra  $60de */
                HOOK(14, 0x60CCU);
            } else {
L_0060CE:
                /* 0060CE  cmpi.w  #$6,$10(a6) */
                /* 0060D4  bne  $60dc */
                if (rd16(a6 + 0x10) == 6) {
L_0060D6:
                    CALL(draw_vehicle_type6, 0x60DAU);                   /* 0060D6  bsr  $61fa */
L_0060DA:
                    /* 0060DA  bra  $60de */
                    HOOK(15, 0x60DAU);
                } else {
                    CALL(draw_vehicle, 0x60DEU);                         /* 0060DC  bsr  $60f0 */
                }
            }
        }
L_0060DE:
        /* 0060DE  movem.w  (a7)+,d7 */
        HOOK(16, 0x60DEU);
        d7 = SX16(POP16());
        a6 = a6 + 0x2E;                                      /* 0060E2  lea  $2e(a6),a6 */
        /* 0060E6  dbra  d7,$60b8 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_0060EA:
    /* 0060EA  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 0060EE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x60B8U: goto L_0060B8;
        case 0x60C0U: goto L_0060C0;
        case 0x60CCU: goto L_0060CC;
        case 0x60CEU: goto L_0060CE;
        case 0x60D6U: goto L_0060D6;
        case 0x60DAU: goto L_0060DA;
        case 0x60DEU: goto L_0060DE;
        case 0x60EAU: goto L_0060EA;
        }
    }
}

/* 0060F0  Draws a vehicle (general type) */
void draw_vehicle(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0060F0  moveq  #0,d5 */
    HOOK(17, 0x60F0U);
    d5 = 0;
    /* 0060F2  tst.w  (a6) */
    /* 0060F4  beq  $61f8 */
    if (rd16(a6) != 0) {
        a0 = 0x3D3FC;                                        /* 0060F8  lea  $3d3fc.l,a0 */
        SETW(d0, rd16(a6 + 0x2));                            /* 0060FE  move.w  $2(a6),d0 */
        SETW(d1, rd16(a6 + 0x4));                            /* 006102  move.w  $4(a6),d1 */
        wr16(V_02D4, rd16(a6 + 0xA));                        /* 006106  move.w  $a(a6),$2d4(a5) */
        SETW(d0, d0 - 0x24);                                 /* 00610C  subi.w  #$24,d0 */
        SETW(d4, 0x64);                                      /* 006110  move.w  #$64,d4 */
        /* 006114  sub.w  $2d4(a5),d4 */
        {
            uint32_t s = rd16(V_02D4);
            uint32_t d = (d4 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d4, r);
            fx = (s > d);
        }
        /* 006118  cmp.w  d4,d1 */
        /* 00611A  bls  $61f8 */
        if ((d1 & 0xFFFF) > (d4 & 0xFFFF)) {
            /* 00611E  cmp.w  #$64,d1 */
            /* 006122  bcc  $613c */
            if ((d1 & 0xFFFF) < 0x64) {
                SETW(d6, 0x64);                                      /* 006124  move.w  #$64,d6 */
                SETW(d6, d6 - (d1 & 0xFFFF));                        /* 006128  sub.w  d1,d6 */
                wr16(V_02D4, rd16(V_02D4) - (d6 & 0xFFFF));          /* 00612A  sub.w  d6,$2d4(a5) */
                SETW(d5, (d6 & 0xFFFF));                             /* 00612E  move.w  d6,d5 */
                d5 = (d5 & 0xFFFF) * 0x20;                           /* 006130  mulu.w  #$20,d5 */
                SETW(d1, 0x64);                                      /* 006134  move.w  #$64,d1 */
                /* 006138  bra  $6164 */
            } else {
                SETW(d4, 0x12C);                                     /* 00613C  move.w  #$12c,d4 */
                /* 006140  sub.w  $2d4(a5),d4 */
                {
                    uint32_t s = rd16(V_02D4);
                    uint32_t d = (d4 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d4, r);
                    fx = (s > d);
                }
                /* 006144  cmp.w  d4,d1 */
                /* 006146  bls  $6164 */
                if ((d1 & 0xFFFF) > (d4 & 0xFFFF)) {
                    /* 006148  cmp.w  #$12b,d1 */
                    /* 00614C  bls  $6154 */
                    if ((d1 & 0xFFFF) > 0x12B) {
                        wr16(a6, 0);                                         /* 00614E  clr.w  (a6) */
                        /* 006150  bra  $61f8 */
                        goto L_0061F8;
                    }
                    SETW(d2, (d1 & 0xFFFF));                             /* 006154  move.w  d1,d2 */
                    SETW(d4, 0x12C);                                     /* 006156  move.w  #$12c,d4 */
                    SETW(d4, d4 - rd16(V_02D4));                         /* 00615A  sub.w  $2d4(a5),d4 */
                    SETW(d2, d2 - (d4 & 0xFFFF));                        /* 00615E  sub.w  d4,d2 */
                    wr16(V_02D4, rd16(V_02D4) - (d2 & 0xFFFF));          /* 006160  sub.w  d2,$2d4(a5) */
                }
            }
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006164  lsr.w  #1,d0 */
            a1 = rd32(V_DRAW_BUFFER);                            /* 006166  movea.l  $28e(a5),a1 */
            a1 += SX16((d0 & 0xFFFF));                           /* 00616A  adda.w  d0,a1 */
            SETW(d1, d1 - 0x64);                                 /* 00616C  subi.w  #$64,d1 */
            SETW(d1, (d1 & 0xFFFF) << 5);                        /* 006170  lsl.w  #5,d1 */
            a1 = a1 + SX16(d1);                                  /* 006172  lea  $0(a1,d1.w),a1 */
            SETW(d1, (d1 & 0xFFFF) << 2);                        /* 006176  lsl.w  #2,d1 */
            a1 = a1 + SX16(d1);                                  /* 006178  lea  $0(a1,d1.w),a1 */
            /* 00617C  cmpi.w  #$2,$10(a6) */
            /* 006182  bcc  $6198 */
            if (rd16(a6 + 0x10) < 2) {
                a0 = 0x3D3FC;                                        /* 006184  lea  $3d3fc.l,a0 */
                SETW(d0, rd16(a6 + 0x8));                            /* 00618A  move.w  $8(a6),d0 */
                d0 = (d0 & 0xFFFF) * 0xB40;                          /* 00618E  mulu.w  #$b40,d0 */
                a0 += d0;                                            /* 006192  adda.l  d0,a0 */
                a0 += d5;                                            /* 006194  adda.l  d5,a0 */
                /* 006196  bra  $61aa */
            } else {
                a0 = 0x3F07C;                                        /* 006198  lea  $3f07c.l,a0 */
                SETW(d0, rd16(a6 + 0x8));                            /* 00619E  move.w  $8(a6),d0 */
                d0 = (d0 & 0xFFFF) * 0xB40;                          /* 0061A2  mulu.w  #$b40,d0 */
                a0 += d0;                                            /* 0061A6  adda.l  d0,a0 */
                a0 += d5;                                            /* 0061A8  adda.l  d5,a0 */
            }
            do {
                BUDGET_CHECK(0x61AAU);
L_0061AA:
                /* 0061AA  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 0061AE  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 0061B0  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0061B2  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0061B4  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 0061B6  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061B8  and.w  d4,(a1) */
                /* 0061BA  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061BC  and.w  d4,(a1) */
                /* 0061BE  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061C0  and.w  d4,(a1) */
                /* 0061C2  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061C4  and.w  d4,(a1) */
                /* 0061C6  or.w  d3,(a1)+ */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                /* 0061C8  move.l  (a0)+,(a1)+ */
                {
                    uint32_t v = rd32(a0);
                    a0 += 4;
                    wr32(a1, v);
                    a1 += 4;
                }
                /* 0061CA  move.l  (a0)+,(a1)+ */
                {
                    uint32_t v = rd32(a0);
                    a0 += 4;
                    wr32(a1, v);
                    a1 += 4;
                }
                /* 0061CC  move.l  (a0)+,(a1)+ */
                {
                    uint32_t v = rd32(a0);
                    a0 += 4;
                    wr32(a1, v);
                    a1 += 4;
                }
                /* 0061CE  move.l  (a0)+,(a1)+ */
                {
                    uint32_t v = rd32(a0);
                    a0 += 4;
                    wr32(a1, v);
                    a1 += 4;
                }
                /* 0061D0  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 0061D4  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 0061D6  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0061D8  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0061DA  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 0061DC  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061DE  and.w  d4,(a1) */
                /* 0061E0  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061E2  and.w  d4,(a1) */
                /* 0061E4  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061E6  and.w  d4,(a1) */
                /* 0061E8  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0061EA  and.w  d4,(a1) */
                /* 0061EC  or.w  d3,(a1)+ */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                a1 = a1 + 0x80;                                      /* 0061EE  lea  $80(a1),a1 */
                /* 0061F2  subq.w  #1,$2d4(a5) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(V_02D4);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(V_02D4, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 0061F6  bne  $61aa */
            } while (COND(6));
        }
    }
L_0061F8:
    RTS();                                               /* 0061F8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x61AAU: goto L_0061AA;
        }
    }
}

/* 0061FA  Draws a type 6 vehicle */
void draw_vehicle_type6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0061FA  move.w  #$2f,d7 */
    HOOK(18, 0x61FAU);
    SETW(d7, 0x2F);
    d5 = 0;                                              /* 0061FE  moveq  #0,d5 */
    /* 006200  tst.w  (a6) */
    /* 006202  beq  $62be */
    if (rd16(a6) != 0) {
L_006206:
        SETW(d0, rd16(a6 + 0x2));                            /* 006206  move.w  $2(a6),d0 */
        SETW(d1, rd16(a6 + 0x4));                            /* 00620A  move.w  $4(a6),d1 */
        /* 00620E  subi.w  #$24,d0 */
        {
            uint32_t s = 0x24;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d0, r);
            fx = (s > d);
        }
        /* 006212  cmp.w  #$34,d1 */
        /* 006216  bls  $62be */
        if ((d1 & 0xFFFF) > 0x34) {
L_00621A:
            /* 00621A  cmp.w  #$64,d1 */
            /* 00621E  bcc  $6236 */
            if ((d1 & 0xFFFF) < 0x64) {
L_006220:
                SETW(d6, 0x64);                                      /* 006220  move.w  #$64,d6 */
                SETW(d6, d6 - (d1 & 0xFFFF));                        /* 006224  sub.w  d1,d6 */
                SETW(d7, d7 - (d6 & 0xFFFF));                        /* 006226  sub.w  d6,d7 */
                SETW(d5, (d6 & 0xFFFF));                             /* 006228  move.w  d6,d5 */
                d5 = (d5 & 0xFFFF) * 0x10;                           /* 00622A  mulu.w  #$10,d5 */
                SETW(d1, 0x64);                                      /* 00622E  move.w  #$64,d1 */
                /* 006232  bra  $6250 */
            } else {
L_006236:
                /* 006236  cmp.w  #$fc,d1 */
                /* 00623A  bls  $6250 */
                if ((d1 & 0xFFFF) > 0xFC) {
L_00623C:
                    /* 00623C  cmp.w  #$12b,d1 */
                    /* 006240  bls  $6248 */
                    if ((d1 & 0xFFFF) > 0x12B) {
L_006242:
                        wr16(a6, 0);                                         /* 006242  clr.w  (a6) */
                        /* 006244  bra  $62be */
                        goto L_0062BE;
                    }
L_006248:
                    SETW(d2, (d1 & 0xFFFF));                             /* 006248  move.w  d1,d2 */
                    SETW(d2, d2 - 0xFC);                                 /* 00624A  subi.w  #$fc,d2 */
                    SETW(d7, d7 - (d2 & 0xFFFF));                        /* 00624E  sub.w  d2,d7 */
                }
            }
L_006250:
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006250  lsr.w  #1,d0 */
            a1 = rd32(V_DRAW_BUFFER);                            /* 006252  movea.l  $28e(a5),a1 */
            a1 += SX16((d0 & 0xFFFF));                           /* 006256  adda.w  d0,a1 */
            SETW(d1, d1 - 0x64);                                 /* 006258  subi.w  #$64,d1 */
            SETW(d1, (d1 & 0xFFFF) << 5);                        /* 00625C  lsl.w  #5,d1 */
            a1 = a1 + SX16(d1);                                  /* 00625E  lea  $0(a1,d1.w),a1 */
            SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 2, 16));      /* 006262  lsl.w  #2,d1 */
            a1 = a1 + SX16(d1);                                  /* 006264  lea  $0(a1,d1.w),a1 */
            a0 = 0x3EA7C;                                        /* 006268  lea  $3ea7c.l,a0 */
            SETW(d0, rd16(a6 + 0x8));                            /* 00626E  move.w  $8(a6),d0 */
            d0 = (d0 & 0xFFFF) * 0x300;                          /* 006272  mulu.w  #$300,d0 */
            a0 += d0;                                            /* 006276  adda.l  d0,a0 */
            a0 += d5;                                            /* 006278  adda.l  d5,a0 */
            do {
                BUDGET_CHECK(0x627AU);
L_00627A:
                /* 00627A  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 00627E  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006280  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006282  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 006284  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 006286  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006288  and.w  d4,(a1) */
                /* 00628A  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00628C  and.w  d4,(a1) */
                /* 00628E  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006290  and.w  d4,(a1) */
                /* 006292  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006294  and.w  d4,(a1) */
                /* 006296  or.w  d3,(a1)+ */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                /* 006298  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 00629C  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00629E  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0062A0  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0062A2  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 0062A4  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0062A6  and.w  d4,(a1) */
                /* 0062A8  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0062AA  and.w  d4,(a1) */
                /* 0062AC  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0062AE  and.w  d4,(a1) */
                /* 0062B0  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0062B2  and.w  d4,(a1) */
                wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 0062B4  or.w  d3,(a1) */
                a1 = a1 + 0x92;                                      /* 0062B6  lea  $92(a1),a1 */
                /* 0062BA  dbra  d7,$627a */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
    }
L_0062BE:
    RTS();                                               /* 0062BE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6206U: goto L_006206;
        case 0x621AU: goto L_00621A;
        case 0x6220U: goto L_006220;
        case 0x6236U: goto L_006236;
        case 0x623CU: goto L_00623C;
        case 0x6242U: goto L_006242;
        case 0x6248U: goto L_006248;
        case 0x6250U: goto L_006250;
        case 0x627AU: goto L_00627A;
        case 0x62BEU: goto L_0062BE;
        }
    }
}

/* 0062C0  Draws a type 4 vehicle */
void draw_vehicle_type4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0062C0  move.w  #$3e,d7 */
    HOOK(19, 0x62C0U);
    SETW(d7, 0x3E);
    d5 = 0;                                              /* 0062C4  moveq  #0,d5 */
    /* 0062C6  tst.w  (a6) */
    /* 0062C8  beq  $63a2 */
    if (rd16(a6) != 0) {
        SETW(d0, rd16(a6 + 0x2));                            /* 0062CC  move.w  $2(a6),d0 */
        SETW(d1, rd16(a6 + 0x4));                            /* 0062D0  move.w  $4(a6),d1 */
        /* 0062D4  subi.w  #$24,d0 */
        {
            uint32_t s = 0x24;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d0, r);
            fx = (s > d);
        }
        /* 0062D8  cmp.w  #$25,d1 */
        /* 0062DC  bls  $63a2 */
        if ((d1 & 0xFFFF) > 0x25) {
            /* 0062E0  cmp.w  #$64,d1 */
            /* 0062E4  bcc  $62fc */
            if ((d1 & 0xFFFF) < 0x64) {
                SETW(d6, 0x64);                                      /* 0062E6  move.w  #$64,d6 */
                SETW(d6, d6 - (d1 & 0xFFFF));                        /* 0062EA  sub.w  d1,d6 */
                SETW(d7, d7 - (d6 & 0xFFFF));                        /* 0062EC  sub.w  d6,d7 */
                SETW(d5, (d6 & 0xFFFF));                             /* 0062EE  move.w  d6,d5 */
                d5 = (d5 & 0xFFFF) * 0x18;                           /* 0062F0  mulu.w  #$18,d5 */
                SETW(d1, 0x64);                                      /* 0062F4  move.w  #$64,d1 */
                /* 0062F8  bra  $6316 */
            } else {
                /* 0062FC  cmp.w  #$ed,d1 */
                /* 006300  bls  $6316 */
                if ((d1 & 0xFFFF) > 0xED) {
                    /* 006302  cmp.w  #$12b,d1 */
                    /* 006306  bls  $630e */
                    if ((d1 & 0xFFFF) > 0x12B) {
                        wr16(a6, 0);                                         /* 006308  clr.w  (a6) */
                        /* 00630A  bra  $63a2 */
                        goto L_0063A2;
                    }
                    SETW(d2, (d1 & 0xFFFF));                             /* 00630E  move.w  d1,d2 */
                    SETW(d2, d2 - 0xED);                                 /* 006310  subi.w  #$ed,d2 */
                    SETW(d7, d7 - (d2 & 0xFFFF));                        /* 006314  sub.w  d2,d7 */
                }
            }
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006316  lsr.w  #1,d0 */
            a1 = rd32(V_DRAW_BUFFER);                            /* 006318  movea.l  $28e(a5),a1 */
            a1 += SX16((d0 & 0xFFFF));                           /* 00631C  adda.w  d0,a1 */
            SETW(d1, d1 - 0x64);                                 /* 00631E  subi.w  #$64,d1 */
            SETW(d1, (d1 & 0xFFFF) << 5);                        /* 006322  lsl.w  #5,d1 */
            a1 = a1 + SX16(d1);                                  /* 006324  lea  $0(a1,d1.w),a1 */
            SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 2, 16));      /* 006328  lsl.w  #2,d1 */
            a1 = a1 + SX16(d1);                                  /* 00632A  lea  $0(a1,d1.w),a1 */
            a0 = 0x406FC;                                        /* 00632E  lea  $406fc.l,a0 */
            SETW(d0, rd16(a6 + 0x8));                            /* 006334  move.w  $8(a6),d0 */
            d0 = (d0 & 0xFFFF) * 0x5E8;                          /* 006338  mulu.w  #$5e8,d0 */
            a0 += d0;                                            /* 00633C  adda.l  d0,a0 */
            a0 += d5;                                            /* 00633E  adda.l  d5,a0 */
            do {
                BUDGET_CHECK(0x6340U);
L_006340:
                /* 006340  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 006344  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006346  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006348  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 00634A  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 00634C  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00634E  and.w  d4,(a1) */
                /* 006350  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006352  and.w  d4,(a1) */
                /* 006354  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006356  and.w  d4,(a1) */
                /* 006358  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00635A  and.w  d4,(a1) */
                /* 00635C  or.w  d3,(a1)+ */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                /* 00635E  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 006362  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006364  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006366  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 006368  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 00636A  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00636C  and.w  d4,(a1) */
                /* 00636E  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006370  and.w  d4,(a1) */
                /* 006372  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006374  and.w  d4,(a1) */
                /* 006376  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006378  and.w  d4,(a1) */
                /* 00637A  or.w  d3,(a1)+ */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                /* 00637C  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 006380  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006382  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006384  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 006386  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 006388  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00638A  and.w  d4,(a1) */
                /* 00638C  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00638E  and.w  d4,(a1) */
                /* 006390  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006392  and.w  d4,(a1) */
                /* 006394  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006396  and.w  d4,(a1) */
                wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 006398  or.w  d3,(a1) */
                a1 = a1 + 0x8A;                                      /* 00639A  lea  $8a(a1),a1 */
                /* 00639E  dbra  d7,$6340 */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
    }
L_0063A2:
    RTS();                                               /* 0063A2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6340U: goto L_006340;
        }
    }
}

/* 0063A4  game routine */
void sub_0063A4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0063A4  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a6 = V_1560;                                         /* 0063A8  lea  $1560(a5),a6 */
    SETW(d7, 0xE);                                       /* 0063AC  move.w  #$e,d7 */
    do {
        BUDGET_CHECK(0x63B0U);
L_0063B0:
        /* 0063B0  tst.w  (a6) */
        /* 0063B2  bne  $63d0 */
        if (rd16(a6) == 0) {
            wr16(a6 + 0x2, (d0 & 0xFFFF));                       /* 0063B4  move.w  d0,$2(a6) */
            wr16(a6 + 0x4, (d1 & 0xFFFF));                       /* 0063B8  move.w  d1,$4(a6) */
            wr16(a6 + 0xA, (d2 & 0xFFFF));                       /* 0063BC  move.w  d2,$a(a6) */
            wr16(a6 + 0x6, 0);                                   /* 0063C0  clr.w  $6(a6) */
            wr16(a6 + 0x8, 3);                                   /* 0063C4  move.w  #$3,$8(a6) */
            wr16(a6, 0xFFFF);                                    /* 0063CA  move.w  #$ffff,(a6) */
            /* 0063CE  bra  $63d8 */
            goto L_0063D8;
        }
        a6 = a6 + 0xC;                                       /* 0063D0  lea  $c(a6),a6 */
        /* 0063D4  dbra  d7,$63b0 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_0063D8:
    /* 0063D8  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 0063DC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x63B0U: goto L_0063B0;
        }
    }
}

/* 0063DE  game routine */
void sub_0063DE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0063DE  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    d5 = 0;                                              /* 0063E2  moveq  #0,d5 */
    a6 = V_1560;                                         /* 0063E4  lea  $1560(a5),a6 */
    SETW(d7, 0xE);                                       /* 0063E8  move.w  #$e,d7 */
    do {
        BUDGET_CHECK(0x63ECU);
L_0063EC:
        /* 0063EC  tst.w  (a6) */
        /* 0063EE  beq  $63fa */
        if (rd16(a6) != 0) {
            PUSH16(d7);                                          /* 0063F0  movem.w  d7,-(a7) */
            CALL(sub_006408, 0x63F6U);                           /* 0063F4  bsr  $6408 */
L_0063F6:
            d7 = SX16(POP16());                                  /* 0063F6  movem.w  (a7)+,d7 */
        }
L_0063FA:
        a6 = a6 + 0xC;                                       /* 0063FA  lea  $c(a6),a6 */
        /* 0063FE  dbra  d7,$63ec */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006402:
    /* 006402  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 006406  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x63ECU: goto L_0063EC;
        case 0x63F6U: goto L_0063F6;
        case 0x63FAU: goto L_0063FA;
        case 0x6402U: goto L_006402;
        }
    }
}

/* 006408  game routine */
void sub_006408(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d5 = 0;                                              /* 006408  moveq  #0,d5 */
    /* 00640A  tst.w  $a(a6) */
    /* 00640E  beq  $6418 */
    if (rd16(a6 + 0xA) != 0) {
        /* 006410  subq.w  #1,$a(a6) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(a6 + 0xA);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(a6 + 0xA, r);
            fx = (s > d);
        }
        /* 006414  bra  $652c */
    } else {
        SETW(d6, 0x27);                                      /* 006418  move.w  #$27,d6 */
        SETW(d0, rd16(a6 + 0x2));                            /* 00641C  move.w  $2(a6),d0 */
        SETW(d1, rd16(a6 + 0x4));                            /* 006420  move.w  $4(a6),d1 */
        /* 006424  cmp.w  #$64,d0 */
        /* 006428  bcc  $6430 */
        if ((d0 & 0xFFFF) < 0x64) {
            SETW(d0, 0x64);                                      /* 00642A  move.w  #$64,d0 */
            /* 00642E  bra  $643a */
        } else {
            /* 006430  cmp.w  #$f4,d0 */
            /* 006434  bls  $643a */
            if ((d0 & 0xFFFF) > 0xF4) {
                SETW(d0, 0xF4);                                      /* 006436  move.w  #$f4,d0 */
            }
        }
        /* 00643A  cmp.w  #$64,d1 */
        /* 00643E  bcc  $645c */
        if ((d1 & 0xFFFF) < 0x64) {
            /* 006440  cmp.w  #$3c,d1 */
            /* 006444  bls  $6510 */
            if ((d1 & 0xFFFF) <= 0x3C) {
                goto L_006510;
            }
            SETW(d5, 0x64);                                      /* 006448  move.w  #$64,d5 */
            SETW(d5, d5 - (d1 & 0xFFFF));                        /* 00644C  sub.w  d1,d5 */
            SETW(d6, d6 - (d5 & 0xFFFF));                        /* 00644E  sub.w  d5,d6 */
            d5 = (d5 & 0xFFFF) * 0x18;                           /* 006450  mulu.w  #$18,d5 */
            SETW(d1, 0x64);                                      /* 006454  move.w  #$64,d1 */
            /* 006458  bra  $647a */
        } else {
            /* 00645C  cmp.w  #$104,d1 */
            /* 006460  bls  $647a */
            if ((d1 & 0xFFFF) > 0x104) {
                /* 006464  cmp.w  #$12d,d1 */
                /* 006468  bcs  $6472 */
                if ((d1 & 0xFFFF) >= 0x12D) {
                    wr16(a6, 0);                                         /* 00646C  clr.w  (a6) */
                    /* 00646E  bra  $652c */
                    goto L_00652C;
                }
                SETW(d4, (d1 & 0xFFFF));                             /* 006472  move.w  d1,d4 */
                SETW(d4, d4 - 0x105);                                /* 006474  subi.w  #$105,d4 */
                SETW(d6, d6 - (d4 & 0xFFFF));                        /* 006478  sub.w  d4,d6 */
            }
        }
        SETW(d0, d0 - 0x24);                                 /* 00647A  subi.w  #$24,d0 */
        SETW(d1, d1 - 0x64);                                 /* 00647E  subi.w  #$64,d1 */
        a1 = rd32(V_DRAW_BUFFER);                            /* 006482  movea.l  $28e(a5),a1 */
        SETW(d1, (d1 & 0xFFFF) << 5);                        /* 006486  lsl.w  #5,d1 */
        a1 = a1 + SX16(d1);                                  /* 006488  lea  $0(a1,d1.w),a1 */
        SETW(d1, (d1 & 0xFFFF) << 2);                        /* 00648C  lsl.w  #2,d1 */
        a1 = a1 + SX16(d1);                                  /* 00648E  lea  $0(a1,d1.w),a1 */
        SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006492  lsr.w  #1,d0 */
        SETW(d0, d0 & 0xFFF8);                               /* 006494  andi.w  #$fff8,d0 */
        a1 = a1 + SX16(d0);                                  /* 006498  lea  $0(a1,d0.w),a1 */
        a0 = 0x3C13C;                                        /* 00649C  lea  $3c13c.l,a0 */
        SETW(d0, rd16(a6 + 0x6));                            /* 0064A2  move.w  $6(a6),d0 */
        d0 = (d0 & 0xFFFF) * 0x3C0;                          /* 0064A6  mulu.w  #$3c0,d0 */
        a0 += d0;                                            /* 0064AA  adda.l  d0,a0 */
        a0 += d5;                                            /* 0064AC  adda.l  d5,a0 */
        do {
            BUDGET_CHECK(0x64AEU);
L_0064AE:
            /* 0064AE  movem.w  (a0)+,d0-d3 */
            { uint32_t ea;
            ea = a0;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            d3 = SX16(rd16(ea));
            ea += 2;
            a0 = ea;
            }
            SETW(d4, (d0 & 0xFFFF));                             /* 0064B2  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 0064B4  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0064B6  or.w  d2,d4 */
            SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0064B8  or.w  d3,d4 */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 0064BA  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064BC  and.w  d4,(a1) */
            /* 0064BE  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064C0  and.w  d4,(a1) */
            /* 0064C2  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064C4  and.w  d4,(a1) */
            /* 0064C6  or.w  d2,(a1)+ */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064C8  and.w  d4,(a1) */
            /* 0064CA  or.w  d3,(a1)+ */
            {
                uint32_t s = (d3 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 0064CC  movem.w  (a0)+,d0-d3 */
            { uint32_t ea;
            ea = a0;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            d3 = SX16(rd16(ea));
            ea += 2;
            a0 = ea;
            }
            SETW(d4, (d0 & 0xFFFF));                             /* 0064D0  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 0064D2  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0064D4  or.w  d2,d4 */
            SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0064D6  or.w  d3,d4 */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 0064D8  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064DA  and.w  d4,(a1) */
            /* 0064DC  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064DE  and.w  d4,(a1) */
            /* 0064E0  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064E2  and.w  d4,(a1) */
            /* 0064E4  or.w  d2,(a1)+ */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064E6  and.w  d4,(a1) */
            /* 0064E8  or.w  d3,(a1)+ */
            {
                uint32_t s = (d3 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 0064EA  movem.w  (a0)+,d0-d3 */
            { uint32_t ea;
            ea = a0;
            d0 = SX16(rd16(ea));
            ea += 2;
            d1 = SX16(rd16(ea));
            ea += 2;
            d2 = SX16(rd16(ea));
            ea += 2;
            d3 = SX16(rd16(ea));
            ea += 2;
            a0 = ea;
            }
            SETW(d4, (d0 & 0xFFFF));                             /* 0064EE  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 0064F0  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0064F2  or.w  d2,d4 */
            SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0064F4  or.w  d3,d4 */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 0064F6  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064F8  and.w  d4,(a1) */
            /* 0064FA  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0064FC  and.w  d4,(a1) */
            /* 0064FE  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006500  and.w  d4,(a1) */
            /* 006502  or.w  d2,(a1)+ */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006504  and.w  d4,(a1) */
            /* 006506  or.w  d3,(a1)+ */
            {
                uint32_t s = (d3 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            a1 = a1 + 0x88;                                      /* 006508  lea  $88(a1),a1 */
            /* 00650C  dbra  d6,$64ae */
            SETW(d6, d6 - 1);
        } while ((d6 & 0xFFFF) != 0xFFFF);
L_006510:
        /* 006510  subq.w  #1,$8(a6) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(a6 + 0x8);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(a6 + 0x8, r);
            fz = (r == 0);
            fx = (s > d);
        }
        /* 006514  bne  $652c */
        if (COND(7)) {
            wr16(a6 + 0x8, 3);                                   /* 006516  move.w  #$3,$8(a6) */
            /* 00651C  addi.w  #$1,$6(a6) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a6 + 0x6);
                uint32_t r = (d + s) & 0xFFFF;
                wr16(a6 + 0x6, r);
                fx = (r < s);
            }
            /* 006522  cmpi.w  #$5,$6(a6) */
            /* 006528  bne  $652c */
            if (rd16(a6 + 0x6) == 5) {
                wr16(a6, 0);                                         /* 00652A  clr.w  (a6) */
            }
        }
    }
L_00652C:
    RTS();                                               /* 00652C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x64AEU: goto L_0064AE;
        }
    }
}

/* 00652E  game routine */
void sub_00652E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00652E  move.l  a4,-(a7) */
    {
        uint32_t v = a4;
        a7 -= 4;
        wr32(a7, v);
    }
    a4 = V_1C84;                                         /* 006530  lea  $1c84(a5),a4 */
    SETW(d7, 5);                                         /* 006534  move.w  #$5,d7 */
    do {
        BUDGET_CHECK(0x6538U);
L_006538:
        /* 006538  tst.w  (a4) */
        /* 00653A  beq  $6544 */
        if (rd16(a4) != 0) {
            /* 00653C  move.w  d7,-(a7) */
            {
                uint32_t v = (d7 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
            }
            CALL(sub_006550, 0x6542U);                           /* 00653E  bsr  $6550 */
L_006542:
            /* 006542  move.w  (a7)+,d7 */
            SETW(d7, rd16(a7));
            a7 += 2;
        }
L_006544:
        a4 = a4 + 0x12;                                      /* 006544  lea  $12(a4),a4 */
        /* 006548  dbra  d7,$6538 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00654C:
    /* 00654C  movea.l  (a7)+,a4 */
    a4 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 00654E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6538U: goto L_006538;
        case 0x6542U: goto L_006542;
        case 0x6544U: goto L_006544;
        case 0x654CU: goto L_00654C;
        }
    }
}

/* 006550  game routine */
void sub_006550(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d7, 0xF);                                       /* 006550  move.w  #$f,d7 */
    d5 = 0;                                              /* 006554  moveq  #0,d5 */
    /* 006556  tst.w  (a4) */
    /* 006558  beq  $6602 */
    if (rd16(a4) != 0) {
        SETW(d0, rd16(a4 + 0x4));                            /* 00655C  move.w  $4(a4),d0 */
        SETW(d1, rd16(a4 + 0x6));                            /* 006560  move.w  $6(a4),d1 */
        SETW(d0, d0 - 0x24);                                 /* 006564  subi.w  #$24,d0 */
        /* 006568  cmp.w  #$54,d1 */
        /* 00656C  bls  $6602 */
        if ((d1 & 0xFFFF) > 0x54) {
            /* 006570  cmp.w  #$64,d1 */
            /* 006574  bcc  $658c */
            if ((d1 & 0xFFFF) < 0x64) {
                SETW(d6, 0x64);                                      /* 006576  move.w  #$64,d6 */
                SETW(d6, d6 - (d1 & 0xFFFF));                        /* 00657A  sub.w  d1,d6 */
                SETW(d7, d7 - (d6 & 0xFFFF));                        /* 00657C  sub.w  d6,d7 */
                SETW(d5, (d6 & 0xFFFF));                             /* 00657E  move.w  d6,d5 */
                d5 = (d5 & 0xFFFF) * 8;                              /* 006580  mulu.w  #$8,d5 */
                SETW(d1, 0x64);                                      /* 006584  move.w  #$64,d1 */
                /* 006588  bra  $65a6 */
            } else {
                /* 00658C  cmp.w  #$11c,d1 */
                /* 006590  bls  $65a6 */
                if ((d1 & 0xFFFF) > 0x11C) {
                    /* 006592  cmp.w  #$12b,d1 */
                    /* 006596  bls  $659e */
                    if ((d1 & 0xFFFF) > 0x12B) {
                        wr16(a4, 0);                                         /* 006598  clr.w  (a4) */
                        /* 00659A  bra  $6602 */
                        goto L_006602;
                    }
                    SETW(d2, (d1 & 0xFFFF));                             /* 00659E  move.w  d1,d2 */
                    SETW(d2, d2 - 0x11C);                                /* 0065A0  subi.w  #$11c,d2 */
                    SETW(d7, d7 - (d2 & 0xFFFF));                        /* 0065A4  sub.w  d2,d7 */
                }
            }
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 0065A6  lsr.w  #1,d0 */
            a1 = rd32(V_DRAW_BUFFER);                            /* 0065A8  movea.l  $28e(a5),a1 */
            a1 += SX16((d0 & 0xFFFF));                           /* 0065AC  adda.w  d0,a1 */
            SETW(d1, d1 - 0x64);                                 /* 0065AE  subi.w  #$64,d1 */
            SETW(d1, (d1 & 0xFFFF) << 5);                        /* 0065B2  lsl.w  #5,d1 */
            a1 = a1 + SX16(d1);                                  /* 0065B4  lea  $0(a1,d1.w),a1 */
            SETW(d1, (d1 & 0xFFFF) << 2);                        /* 0065B8  lsl.w  #2,d1 */
            a1 = a1 + SX16(d1);                                  /* 0065BA  lea  $0(a1,d1.w),a1 */
            a0 = 0x412CC;                                        /* 0065BE  lea  $412cc.l,a0 */
            d0 = 0;                                              /* 0065C4  moveq  #0,d0 */
            SETW(d0, rd16(a4 + 0x2));                            /* 0065C6  move.w  $2(a4),d0 */
            d0 = (d0 & 0xFFFF) * 0x480;                          /* 0065CA  mulu.w  #$480,d0 */
            a0 = a0 + SX16(d0);                                  /* 0065CE  lea  $0(a0,d0.w),a0 */
            SETW(d0, rd16(a4 + 0xA));                            /* 0065D2  move.w  $a(a4),d0 */
            SETW(d0, (d0 & 0xFFFF) << 7);                        /* 0065D6  lsl.w  #7,d0 */
            a0 += d0;                                            /* 0065D8  adda.l  d0,a0 */
            a0 += d5;                                            /* 0065DA  adda.l  d5,a0 */
            do {
                BUDGET_CHECK(0x65DCU);
L_0065DC:
                /* 0065DC  movem.w  (a0)+,d0-d3 */
                { uint32_t ea;
                ea = a0;
                d0 = SX16(rd16(ea));
                ea += 2;
                d1 = SX16(rd16(ea));
                ea += 2;
                d2 = SX16(rd16(ea));
                ea += 2;
                d3 = SX16(rd16(ea));
                ea += 2;
                a0 = ea;
                }
                SETW(d4, (d0 & 0xFFFF));                             /* 0065E0  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 0065E2  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 0065E4  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0065E6  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 0065E8  not.w  d4 */
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0065EA  and.w  d4,(a1) */
                /* 0065EC  or.w  d0,(a1)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0065EE  and.w  d4,(a1) */
                /* 0065F0  or.w  d1,(a1)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0065F2  and.w  d4,(a1) */
                /* 0065F4  or.w  d2,(a1)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a1;
                    a1 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0065F6  and.w  d4,(a1) */
                wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 0065F8  or.w  d3,(a1) */
                a1 = a1 + 0x9A;                                      /* 0065FA  lea  $9a(a1),a1 */
                /* 0065FE  dbra  d7,$65dc */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
    }
L_006602:
    SETW(d0, rd16(a4 + 0x8));                            /* 006602  move.w  $8(a4),d0 */
    /* 006606  add.w  d0,$6(a4) */
    {
        uint32_t s = (d0 & 0xFFFF);
        uint32_t d = rd16(a4 + 0x6);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(a4 + 0x6, r);
        fx = (r < s);
    }
    RTS();                                               /* 00660A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x65DCU: goto L_0065DC;
        }
    }
}

/* 00660C  game routine */
void sub_00660C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_135A;                                         /* 00660C  lea  $135a(a5),a6 */
    SETW(d6, 0x13);                                      /* 006610  move.w  #$13,d6 */
    do {
        BUDGET_CHECK(0x6614U);
L_006614:
        /* 006614  tst.w  $4(a6) */
        /* 006618  beq  $66ea */
        if (rd16(a6 + 0x4) != 0) {
            SETW(d0, rd16(a6 + 0x2));                            /* 00661C  move.w  $2(a6),d0 */
            /* 006620  subi.w  #$64,d0 */
            {
                uint32_t s = 0x64;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 006624  cmp.w  #$c0,d0 */
            /* 006628  bls  $6632 */
            if ((d0 & 0xFFFF) <= 0xC0) {
                goto L_006632;
            }
L_00662A:
            wr16(a6 + 0x4, 0);                                   /* 00662A  clr.w  $4(a6) */
            /* 00662E  bra  $66ea */
            goto L_0066EA;
L_006632:
            d0 = (d0 & 0xFFFF) * 0xA0;                           /* 006632  mulu.w  #$a0,d0 */
            a0 = rd32(V_DRAW_BUFFER);                            /* 006636  movea.l  $28e(a5),a0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 00663A  adda.w  d0,a0 */
            SETW(d0, rd16(a6));                                  /* 00663C  move.w  $0(a6),d0 */
            /* 006640  subi.w  #$24,d0 */
            {
                uint32_t s = 0x24;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 006644  cmp.w  #$42,d0 */
            /* 006648  bls  $662a */
            if ((d0 & 0xFFFF) <= 0x42) {
                BUDGET_CHECK(0x662AU);
                goto L_00662A;
            }
            /* 00664A  cmp.w  #$f8,d0 */
            /* 00664E  bhi  $662a */
            if ((d0 & 0xFFFF) > 0xF8) {
                BUDGET_CHECK(0x662AU);
                goto L_00662A;
            }
            SETW(d1, (d0 & 0xFFFF));                             /* 006650  move.w  d0,d1 */
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006652  lsr.w  #1,d0 */
            SETW(d0, d0 & 0xFFF8);                               /* 006654  andi.w  #$fff8,d0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 006658  adda.w  d0,a0 */
            d7 = 8;                                              /* 00665A  moveq  #8,d7 */
            a1 = a0;                                             /* 00665C  movea.l  a0,a1 */
            a1 += 1;                                             /* 00665E  addq.w  #1,a1 */
            d1 &= 0xF;                                           /* 006660  andi.l  #$f,d1 */
            /* 006666  bclr  #$3,d1 */
            fz = !(d1 & 0x8);
            d1 &= ~0x8;
            /* 00666A  beq  $6670 */
            if (COND(6)) {
                a0 += 1;                                             /* 00666C  addq.w  #1,a0 */
                a1 += 7;                                             /* 00666E  addq.w  #7,a1 */
            }
            d2 = 0;                                              /* 006670  moveq  #0,d2 */
            SETW(d2, 0x18);                                      /* 006672  move.w  #$18,d2 */
            d2 = (d2 & 0xFFFF) * 0x36;                           /* 006676  mulu.w  #$36,d2 */
            d1 = (d1 & 0xFFFF) * 0x61E;                          /* 00667A  mulu.w  #$61e,d1 */
            /* 00667E  add.l  d1,d2 */
            {
                uint32_t s = d1;
                uint32_t d = d2;
                uint32_t r = (d + s) & 0xFFFFFFFF;
                d2 = r;
                fx = (r < s);
            }
            a2 = 0x4694A;                                        /* 006680  lea  $4694a.l,a2 */
            a2 += d2;                                            /* 006686  adda.l  d2,a2 */
            do {
                BUDGET_CHECK(0x6688U);
L_006688:
                /* 006688  move.b  (a2)+,d0 */
                SETB(d0, rd8(a2));
                a2 += 1;
                SETB(d1, rd8(a2 + 0x1));                             /* 00668A  move.b  $1(a2),d1 */
                SETB(d2, rd8(a2 + 0x3));                             /* 00668E  move.b  $3(a2),d2 */
                SETB(d4, (d0 & 0xFF));                               /* 006692  move.b  d0,d4 */
                SETB(d4, d4 | (d1 & 0xFF));                          /* 006694  or.b  d1,d4 */
                SETB(d4, d4 | (d2 & 0xFF));                          /* 006696  or.b  d2,d4 */
                wr8(a0 + 0x6, rd8(a0 + 0x6) | (d4 & 0xFF));          /* 006698  or.b  d4,$6(a0) */
                SETB(d4, ~(d4 & 0xFF));                              /* 00669C  not.b  d4 */
                wr8(a0, rd8(a0) & (d4 & 0xFF));                      /* 00669E  and.b  d4,(a0) */
                wr8(a0 + 0x2, rd8(a0 + 0x2) & (d4 & 0xFF));          /* 0066A0  and.b  d4,$2(a0) */
                wr8(a0 + 0x4, rd8(a0 + 0x4) & (d4 & 0xFF));          /* 0066A4  and.b  d4,$4(a0) */
                wr8(a0, rd8(a0) | (d0 & 0xFF));                      /* 0066A8  or.b  d0,(a0) */
                wr8(a0 + 0x2, rd8(a0 + 0x2) | (d1 & 0xFF));          /* 0066AA  or.b  d1,$2(a0) */
                wr8(a0 + 0x4, rd8(a0 + 0x4) | (d2 & 0xFF));          /* 0066AE  or.b  d2,$4(a0) */
                SETB(d0, rd8(a2));                                   /* 0066B2  move.b  (a2),d0 */
                SETB(d1, rd8(a2 + 0x2));                             /* 0066B4  move.b  $2(a2),d1 */
                SETB(d2, rd8(a2 + 0x4));                             /* 0066B8  move.b  $4(a2),d2 */
                SETB(d4, (d0 & 0xFF));                               /* 0066BC  move.b  d0,d4 */
                SETB(d4, d4 | (d1 & 0xFF));                          /* 0066BE  or.b  d1,d4 */
                SETB(d4, d4 | (d2 & 0xFF));                          /* 0066C0  or.b  d2,d4 */
                wr8(a1 + 0x6, rd8(a1 + 0x6) | (d4 & 0xFF));          /* 0066C2  or.b  d4,$6(a1) */
                SETB(d4, ~(d4 & 0xFF));                              /* 0066C6  not.b  d4 */
                wr8(a1, rd8(a1) & (d4 & 0xFF));                      /* 0066C8  and.b  d4,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) & (d4 & 0xFF));          /* 0066CA  and.b  d4,$2(a1) */
                wr8(a1 + 0x4, rd8(a1 + 0x4) & (d4 & 0xFF));          /* 0066CE  and.b  d4,$4(a1) */
                wr8(a1, rd8(a1) | (d0 & 0xFF));                      /* 0066D2  or.b  d0,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) | (d1 & 0xFF));          /* 0066D4  or.b  d1,$2(a1) */
                wr8(a1 + 0x4, rd8(a1 + 0x4) | (d2 & 0xFF));          /* 0066D8  or.b  d2,$4(a1) */
                a0 += SX16(0xA0);                                    /* 0066DC  adda.w  #$a0,a0 */
                a1 += SX16(0xA0);                                    /* 0066E0  adda.w  #$a0,a1 */
                a2 += 5;                                             /* 0066E4  addq.l  #5,a2 */
                /* 0066E6  dbra  d7,$6688 */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
L_0066EA:
        a6 = a6 + 0xE;                                       /* 0066EA  lea  $e(a6),a6 */
        /* 0066EE  dbra  d6,$6614 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_0066F2:
    RTS();                                               /* 0066F2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6614U: goto L_006614;
        case 0x662AU: goto L_00662A;
        case 0x6688U: goto L_006688;
        case 0x66EAU: goto L_0066EA;
        case 0x66F2U: goto L_0066F2;
        }
    }
}

/* 0066F4  game routine */
void sub_0066F4(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1CF0;                                         /* 0066F4  lea  $1cf0(a5),a6 */
    SETW(d5, 0);                                         /* 0066F8  move.w  #$0,d5 */
L_0066FC:
    /* 0066FC  tst.w  (a6) */
    /* 0066FE  bne  $670c */
    if (rd16(a6) != 0) {
        goto L_00670C;
    }
L_006700:
    a6 += 8;                                             /* 006700  adda.l  #$8,a6 */
    /* 006706  dbra  d5,$66fc */
    SETW(d5, d5 - 1);
    if ((d5 & 0xFFFF) != 0xFFFF) {
        BUDGET_CHECK(0x66FCU);
        goto L_0066FC;
    }
L_00670A:
    RTS();                                               /* 00670A  rts */
L_00670C:
    SETW(d0, rd16(a6 + 0x2));                            /* 00670C  move.w  $2(a6),d0 */
    SETW(d2, rd16(a6 + 0x4));                            /* 006710  move.w  $4(a6),d2 */
    SETW(d3, rd16(a6 + 0x6));                            /* 006714  move.w  $6(a6),d3 */
    /* 006718  movem.l  a6/d5,-(a7) */
    PUSH32(a6);
    PUSH32(d5);
    CALL(sub_006724, 0x671EU);                           /* 00671C  bsr  $6724 */
L_00671E:
    /* 00671E  movem.l  (a7)+,d5/a6 */
    d5 = POP32();
    a6 = POP32();
    /* 006722  bra  $6700 */
    BUDGET_CHECK(0x6700U);
    goto L_006700;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x66FCU: goto L_0066FC;
        case 0x6700U: goto L_006700;
        case 0x670AU: goto L_00670A;
        case 0x671EU: goto L_00671E;
        }
    }
}

/* 006724  game routine */
void sub_006724(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d7, 0x18);                                      /* 006724  move.w  #$18,d7 */
    d5 = 0;                                              /* 006728  moveq  #0,d5 */
    /* 00672A  cmp.w  #$64,d2 */
    /* 00672E  bcc  $6742 */
    if ((d2 & 0xFFFF) < 0x64) {
        SETW(d6, 0x64);                                      /* 006732  move.w  #$64,d6 */
        SETW(d6, d6 - (d2 & 0xFFFF));                        /* 006736  sub.w  d2,d6 */
        SETW(d7, d7 - (d6 & 0xFFFF));                        /* 006738  sub.w  d6,d7 */
        SETW(d5, d5 + (d6 & 0xFFFF));                        /* 00673A  add.w  d6,d5 */
        SETW(d5, (d5 & 0xFFFF) << 4);                        /* 00673C  lsl.w  #4,d5 */
        SETW(d2, 0x64);                                      /* 00673E  move.w  #$64,d2 */
    }
    SETW(d2, d2 - 0x64);                                 /* 006742  subi.w  #$64,d2 */
    a1 = rd32(V_DRAW_BUFFER);                            /* 006746  movea.l  $28e(a5),a1 */
    SETW(d2, (d2 & 0xFFFF) << 5);                        /* 00674A  lsl.w  #5,d2 */
    a1 = a1 + SX16(d2);                                  /* 00674C  lea  $0(a1,d2.w),a1 */
    SETW(d2, (d2 & 0xFFFF) << 2);                        /* 006750  lsl.w  #2,d2 */
    a1 = a1 + SX16(d2);                                  /* 006752  lea  $0(a1,d2.w),a1 */
    SETW(d0, d0 - 0x64);                                 /* 006756  subi.w  #$64,d0 */
    a1 += 0x20;                                          /* 00675A  adda.l  #$20,a1 */
    SETW(d0, (d0 & 0xFFFF) >> 4);                        /* 006760  lsr.w  #4,d0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 006762  lsl.w  #3,d0 */
    a1 = a1 + SX16(d0);                                  /* 006764  lea  $0(a1,d0.w),a1 */
    a2 = 0x434B6;                                        /* 006768  lea  $434b6.l,a2 */
    d3 = (d3 & 0xFFFF) * 0x190;                          /* 00676E  mulu.w  #$190,d3 */
    a2 = a2 + SX16(d3);                                  /* 006772  lea  $0(a2,d3.w),a2 */
    a2 += d5;                                            /* 006776  adda.l  d5,a2 */
    do {
        BUDGET_CHECK(0x6778U);
L_006778:
        /* 006778  movem.w  (a2)+,d0-d3 */
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
        SETW(d4, (d0 & 0xFFFF));                             /* 00677C  move.w  d0,d4 */
        SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00677E  or.w  d1,d4 */
        SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006780  or.w  d2,d4 */
        SETW(d4, d4 | (d3 & 0xFFFF));                        /* 006782  or.w  d3,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 006784  not.w  d4 */
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006786  and.w  d4,(a1) */
        /* 006788  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00678A  and.w  d4,(a1) */
        /* 00678C  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00678E  and.w  d4,(a1) */
        /* 006790  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 006792  and.w  d4,(a1) */
        /* 006794  or.w  d3,(a1)+ */
        {
            uint32_t s = (d3 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 006796  movem.w  (a2)+,d0-d3 */
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
        SETW(d4, (d0 & 0xFFFF));                             /* 00679A  move.w  d0,d4 */
        SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00679C  or.w  d1,d4 */
        SETW(d4, d4 | (d2 & 0xFFFF));                        /* 00679E  or.w  d2,d4 */
        SETW(d4, d4 | (d3 & 0xFFFF));                        /* 0067A0  or.w  d3,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 0067A2  not.w  d4 */
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0067A4  and.w  d4,(a1) */
        /* 0067A6  or.w  d0,(a1)+ */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0067A8  and.w  d4,(a1) */
        /* 0067AA  or.w  d1,(a1)+ */
        {
            uint32_t s = (d1 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0067AC  and.w  d4,(a1) */
        /* 0067AE  or.w  d2,(a1)+ */
        {
            uint32_t s = (d2 & 0xFFFF);
            uint32_t ea = a1;
            a1 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d | s) & 0xFFFF;
            wr16(ea, r);
        }
        wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 0067B0  and.w  d4,(a1) */
        wr16(a1, rd16(a1) | (d3 & 0xFFFF));                  /* 0067B2  or.w  d3,(a1) */
        a1 = a1 + 0x92;                                      /* 0067B4  lea  $92(a1),a1 */
        /* 0067B8  dbra  d7,$6778 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 0067BC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6778U: goto L_006778;
        }
    }
}

/* 0067BE  game routine */
void sub_0067BE(void)
{
    /* 0067BE  btst  d0,d1 */
    /* 0067C0  btst  d0,d1 */
    /* 0067C2  btst  d0,d1 */
    /* 0067C4  btst  d0,d1 */
    /* 0067C6  btst  d0,d1 */
    /* 0067C8  btst  d0,d1 */
    /* 0067CA  btst  d0,d1 */
    /* 0067CC  btst  d0,d1 */
    /* 0067CE  btst  d0,d1 */
    /* 0067D0  btst  d0,d1 */
    /* 0067D2  btst  d0,d1 */
    /* 0067D4  btst  d0,d1 */
    /* 0067D6  btst  d0,d1 */
    /* 0067D8  btst  d0,d1 */
    /* 0067DA  btst  d0,d1 */
    /* 0067DC  btst  d0,d1 */
    /* 0067DE  btst  d0,d1 */
    /* 0067E0  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 0067E2  ori.b  #$1,d0 */
    SETB(d0, d0 | 1);                                    /* 0067E6  ori.b  #$1,d0 */
    /* 0067EA  btst  d0,d1 */
    /* 0067EC  btst  d0,d1 */
    /* 0067EE  btst  d0,d0 */
    SETB(d0, d0 | 0);                                    /* 0067F0  ori.b  #$0,d0 */
    SETB(d0, d0 | 1);                                    /* 0067F4  ori.b  #$1,d0 */
    /* 0067F8  btst  d0,d1 */
    /* 0067FA  btst  d0,d1 */
    /* 0067FC  btst  d0,d1 */
    /* 0067FE  btst  d0,d1 */
    /* 006800  btst  d0,d1 */
    /* 006802  btst  d0,d1 */
    SETB(d0, d0 | 1);                                    /* 006804  ori.b  #$1,d0 */
    /* 006808  btst  d0,d0 */
    SETB(d1, d1 | 1);                                    /* 00680A  ori.b  #$1,d1 */
    /* 00680E  btst  d0,d1 */
    SETB(d1, d1 | 0x2D);                                 /* 006810  ori.b  #$2d,d1 */
    /* 006814  andi.b  #$fc,(a4)+ */
    {
        uint32_t s = 0xFC;
        uint32_t ea = a4;
        a4 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d & s) & 0xFF;
        wr8(ea, r);
    }
    SETB(d7, d7 | 0x6D);                                 /* 006818  ori.b  #$6d,d7 */
    /* 00681C  andi.w  #$41fa,d6 */
    {
        uint32_t s = 0x41FA;
        uint32_t d = (d6 & 0xFFFF);
        uint32_t r = (d & s) & 0xFFFF;
        SETW(d6, r);
        FL_LOGIC(r, 0xFFFF, 0x8000);
    }
    SOFT(0x6820U);                                       /* 006820  soft */
}

/* 006812  Draws the bullets (two loops: the player's and the enemies') */
void draw_bullets(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, rd16(V_021C));                              /* 006812  move.w  $21c(a5),d0 */
    d0 = (d0 & 0xFFFF) * 7;                              /* 006816  mulu.w  #$7,d0 */
    /* 00681A  add.w  $246(a5),d0 */
    {
        uint32_t s = rd16(V_0246);
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
        fx = (r < s);
    }
    a0 = 0x67BE;                                         /* 00681E  lea  $67be(pc),a0 */
L_006822:
    /* 006822  tst.b  $0(a0,d0.w) */
    /* 006826  beq  $68be */
    if (rd8(a0 + SX16(d0)) != 0) {
L_00682A:
        a0 = V_BULLETS;                                      /* 00682A  lea  $1012(a5),a0 */
        d7 = 0x1D;                                           /* 00682E  moveq  #29,d7 */
        SETW(d5, 0xFFFC);                                    /* 006830  move.w  #$fffc,d5 */
        SETW(d4, 0xFF3F);                                    /* 006834  move.w  #$ff3f,d4 */
        SETW(d2, 0x64);                                      /* 006838  move.w  #$64,d2 */
        SETW(d6, 0xC6);                                      /* 00683C  move.w  #$c6,d6 */
        SETW(d3, 0x24);                                      /* 006840  move.w  #$24,d3 */
        a4 = rd32(V_DRAW_BUFFER);                            /* 006844  movea.l  $28e(a5),a4 */
        do {
            BUDGET_CHECK(0x6848U);
L_006848:
            /* 006848  tst.w  $8(a0) */
            /* 00684C  beq  $68b4 */
            HOOK(20, 0x6848U);
            if (rd16(a0 + 0x8) != 0) {
L_00684E:
                SETW(d0, rd16(a0));                                  /* 00684E  move.w  (a0),d0 */
                /* 006850  sub.w  d3,d0 */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d0, r);
                    fx = (s > d);
                }
                /* 006852  cmp.w  #$fe,d0 */
                /* 006856  bls  $685e */
                if ((d0 & 0xFFFF) <= 0xFE) {
                    goto L_00685E;
                }
L_006858:
                wr16(a0 + 0x8, 0);                                   /* 006858  clr.w  $8(a0) */
                /* 00685C  bra  $68b4 */
                goto L_0068B4;
L_00685E:
                /* 00685E  cmp.w  #$40,d0 */
                /* 006862  bcs  $6858 */
                if ((d0 & 0xFFFF) < 0x40) {
                    BUDGET_CHECK(0x6858U);
                    goto L_006858;
                }
L_006864:
                SETW(d1, rd16(a0 + 0x2));                            /* 006864  move.w  $2(a0),d1 */
                /* 006868  sub.w  d2,d1 */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t d = (d1 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d1, r);
                    fx = (s > d);
                }
                /* 00686A  cmp.w  d6,d1 */
                /* 00686C  bhi  $6858 */
                if ((d1 & 0xFFFF) > (d6 & 0xFFFF)) {
                    BUDGET_CHECK(0x6858U);
                    goto L_006858;
                }
L_00686E:
                a1 = a4;                                             /* 00686E  movea.l  a4,a1 */
                SETW(d1, (d1 & 0xFFFF) << 5);                        /* 006870  lsl.w  #5,d1 */
                a1 = a1 + SX16(d1);                                  /* 006872  lea  $0(a1,d1.w),a1 */
                SETW(d1, (d1 & 0xFFFF) << 2);                        /* 006876  lsl.w  #2,d1 */
                a1 = a1 + SX16(d1);                                  /* 006878  lea  $0(a1,d1.w),a1 */
                SETW(d1, (d0 & 0xFFFF));                             /* 00687C  move.w  d0,d1 */
                SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 00687E  lsr.w  #1,d0 */
                SETW(d0, d0 & (d5 & 0xFFFF));                        /* 006880  and.w  d5,d0 */
                /* 006882  bclr  #$2,d0 */
                fz = !(d0 & 0x4);
                d0 &= ~0x4;
                /* 006886  beq  $688a */
                if (COND(6)) {
L_006888:
                    a1 += 1;                                             /* 006888  addq.w  #1,a1 */
                }
L_00688A:
                a1 += SX16((d0 & 0xFFFF));                           /* 00688A  adda.w  d0,a1 */
                SETW(d1, d1 & 7);                                    /* 00688C  andi.w  #$7,d1 */
                SETW(d0, (d4 & 0xFFFF));                             /* 006890  move.w  d4,d0 */
                SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), (d1 & 63), 16)); /* 006892  lsr.w  d1,d0 */
                wr8(a1, rd8(a1) & (d0 & 0xFF));                      /* 006894  and.b  d0,(a1) */
                wr8(a1 + 0x2, rd8(a1 + 0x2) & (d0 & 0xFF));          /* 006896  and.b  d0,$2(a1) */
                wr8(a1 + 0x4, rd8(a1 + 0x4) & (d0 & 0xFF));          /* 00689A  and.b  d0,$4(a1) */
                wr8(a1 + 0xA0, rd8(a1 + 0xA0) & (d0 & 0xFF));        /* 00689E  and.b  d0,$a0(a1) */
                wr8(a1 + 0xA2, rd8(a1 + 0xA2) & (d0 & 0xFF));        /* 0068A2  and.b  d0,$a2(a1) */
                wr8(a1 + 0xA4, rd8(a1 + 0xA4) & (d0 & 0xFF));        /* 0068A6  and.b  d0,$a4(a1) */
                SETW(d0, ~(d0 & 0xFFFF));                            /* 0068AA  not.w  d0 */
                wr8(a1 + 0x6, rd8(a1 + 0x6) | (d0 & 0xFF));          /* 0068AC  or.b  d0,$6(a1) */
                wr8(a1 + 0xA6, rd8(a1 + 0xA6) | (d0 & 0xFF));        /* 0068B0  or.b  d0,$a6(a1) */
            }
L_0068B4:
            /* 0068B4  lea  $e(a0),a0 */
            HOOK(21, 0x68B4U);
            a0 = a0 + 0xE;
            /* 0068B8  dbra  d7,$6848 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
L_0068BC:
        RTS();                                               /* 0068BC  rts */
    }
L_0068BE:
    a0 = V_BULLETS;                                      /* 0068BE  lea  $1012(a5),a0 */
    d7 = 0x1D;                                           /* 0068C2  moveq  #29,d7 */
    SETW(d5, 0xFFFC);                                    /* 0068C4  move.w  #$fffc,d5 */
    SETW(d4, 0xFF3F);                                    /* 0068C8  move.w  #$ff3f,d4 */
    SETW(d2, 0x64);                                      /* 0068CC  move.w  #$64,d2 */
    SETW(d6, 0xC6);                                      /* 0068D0  move.w  #$c6,d6 */
    SETW(d3, 0x24);                                      /* 0068D4  move.w  #$24,d3 */
    a4 = rd32(V_DRAW_BUFFER);                            /* 0068D8  movea.l  $28e(a5),a4 */
    do {
        BUDGET_CHECK(0x68DCU);
L_0068DC:
        /* 0068DC  tst.w  $8(a0) */
        /* 0068E0  beq  $6946 */
        HOOK(22, 0x68DCU);
        if (rd16(a0 + 0x8) != 0) {
L_0068E2:
            SETW(d0, rd16(a0));                                  /* 0068E2  move.w  (a0),d0 */
            /* 0068E4  sub.w  d3,d0 */
            {
                uint32_t s = (d3 & 0xFFFF);
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d0, r);
                fx = (s > d);
            }
            /* 0068E6  cmp.w  #$fe,d0 */
            /* 0068EA  bls  $68f2 */
            if ((d0 & 0xFFFF) <= 0xFE) {
                goto L_0068F2;
            }
L_0068EC:
            wr16(a0 + 0x8, 0);                                   /* 0068EC  clr.w  $8(a0) */
            /* 0068F0  bra  $6946 */
            goto L_006946;
L_0068F2:
            /* 0068F2  cmp.w  #$40,d0 */
            /* 0068F6  bcs  $68ec */
            if ((d0 & 0xFFFF) < 0x40) {
                BUDGET_CHECK(0x68ECU);
                goto L_0068EC;
            }
L_0068F8:
            SETW(d1, rd16(a0 + 0x2));                            /* 0068F8  move.w  $2(a0),d1 */
            /* 0068FC  sub.w  d2,d1 */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t d = (d1 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d1, r);
                fx = (s > d);
            }
            /* 0068FE  cmp.w  d6,d1 */
            /* 006900  bhi  $68ec */
            if ((d1 & 0xFFFF) > (d6 & 0xFFFF)) {
                BUDGET_CHECK(0x68ECU);
                goto L_0068EC;
            }
L_006902:
            a1 = a4;                                             /* 006902  movea.l  a4,a1 */
            SETW(d1, (d1 & 0xFFFF) << 5);                        /* 006904  lsl.w  #5,d1 */
            a1 = a1 + SX16(d1);                                  /* 006906  lea  $0(a1,d1.w),a1 */
            SETW(d1, (d1 & 0xFFFF) << 2);                        /* 00690A  lsl.w  #2,d1 */
            a1 = a1 + SX16(d1);                                  /* 00690C  lea  $0(a1,d1.w),a1 */
            SETW(d1, (d0 & 0xFFFF));                             /* 006910  move.w  d0,d1 */
            SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 006912  lsr.w  #1,d0 */
            SETW(d0, d0 & (d5 & 0xFFFF));                        /* 006914  and.w  d5,d0 */
            /* 006916  bclr  #$2,d0 */
            fz = !(d0 & 0x4);
            d0 &= ~0x4;
            /* 00691A  beq  $691e */
            if (COND(6)) {
L_00691C:
                a1 += 1;                                             /* 00691C  addq.w  #1,a1 */
            }
L_00691E:
            a1 += SX16((d0 & 0xFFFF));                           /* 00691E  adda.w  d0,a1 */
            SETW(d1, d1 & 7);                                    /* 006920  andi.w  #$7,d1 */
            SETW(d0, (d4 & 0xFFFF));                             /* 006924  move.w  d4,d0 */
            SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), (d1 & 63), 16)); /* 006926  lsr.w  d1,d0 */
            wr8(a1, rd8(a1) & (d0 & 0xFF));                      /* 006928  and.b  d0,(a1) */
            wr8(a1 + 0x2, rd8(a1 + 0x2) & (d0 & 0xFF));          /* 00692A  and.b  d0,$2(a1) */
            wr8(a1 + 0x4, rd8(a1 + 0x4) & (d0 & 0xFF));          /* 00692E  and.b  d0,$4(a1) */
            wr8(a1 + 0x6, rd8(a1 + 0x6) & (d0 & 0xFF));          /* 006932  and.b  d0,$6(a1) */
            wr8(a1 + 0xA0, rd8(a1 + 0xA0) & (d0 & 0xFF));        /* 006936  and.b  d0,$a0(a1) */
            wr8(a1 + 0xA2, rd8(a1 + 0xA2) & (d0 & 0xFF));        /* 00693A  and.b  d0,$a2(a1) */
            wr8(a1 + 0xA4, rd8(a1 + 0xA4) & (d0 & 0xFF));        /* 00693E  and.b  d0,$a4(a1) */
            wr8(a1 + 0xA6, rd8(a1 + 0xA6) & (d0 & 0xFF));        /* 006942  and.b  d0,$a6(a1) */
        }
L_006946:
        /* 006946  lea  $e(a0),a0 */
        HOOK(23, 0x6946U);
        a0 = a0 + 0xE;
        /* 00694A  dbra  d7,$68dc */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00694E:
    RTS();                                               /* 00694E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6822U: goto L_006822;
        case 0x682AU: goto L_00682A;
        case 0x6848U: goto L_006848;
        case 0x684EU: goto L_00684E;
        case 0x6858U: goto L_006858;
        case 0x685EU: goto L_00685E;
        case 0x6864U: goto L_006864;
        case 0x686EU: goto L_00686E;
        case 0x6888U: goto L_006888;
        case 0x688AU: goto L_00688A;
        case 0x68B4U: goto L_0068B4;
        case 0x68BCU: goto L_0068BC;
        case 0x68BEU: goto L_0068BE;
        case 0x68DCU: goto L_0068DC;
        case 0x68E2U: goto L_0068E2;
        case 0x68ECU: goto L_0068EC;
        case 0x68F2U: goto L_0068F2;
        case 0x68F8U: goto L_0068F8;
        case 0x6902U: goto L_006902;
        case 0x691CU: goto L_00691C;
        case 0x691EU: goto L_00691E;
        case 0x6946U: goto L_006946;
        case 0x694EU: goto L_00694E;
        }
    }
}

/* 006950  game routine */
void sub_006950(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = V_BULLETS;                                      /* 006950  lea  $1012(a5),a0 */
    d7 = 0x1D;                                           /* 006954  moveq  #29,d7 */
    do {
        BUDGET_CHECK(0x6956U);
L_006956:
        /* 006956  tst.w  $8(a0) */
        /* 00695A  beq  $6b6a */
        if (rd16(a0 + 0x8) != 0) {
L_00695E:
            SETW(d3, rd16(a0));                                  /* 00695E  move.w  (a0),d3 */
            SETW(d4, rd16(a0 + 0x2));                            /* 006960  move.w  $2(a0),d4 */
            SETW(d5, 0x64);                                      /* 006964  move.w  #$64,d5 */
            /* 006968  cmpi.w  #$fffc,$a(a0) */
            /* 00696E  beq  $69c6 */
            if (rd16(a0 + 0xA) != 0xFFFC) {
L_006972:
                a4 = 0x2400;                                         /* 006972  lea  $2400.l,a4 */
                SETW(d2, rd16(V_SCROLL));                            /* 006978  move.w  $280(a5),d2 */
                SETW(d2, d2 - (d4 & 0xFFFF));                        /* 00697C  sub.w  d4,d2 */
                SETW(d2, d2 + (d5 & 0xFFFF));                        /* 00697E  add.w  d5,d2 */
                SETW(d2, d2 & 0xFFF0);                               /* 006980  andi.w  #$fff0,d2 */
                SETW(d2, d2 & 0x3FF);                                /* 006984  andi.w  #$3ff,d2 */
                SETW(d0, (d3 & 0xFFFF));                             /* 006988  move.w  d3,d0 */
                SETW(d0, d0 - (d5 & 0xFFFF));                        /* 00698A  sub.w  d5,d0 */
                SETW(d0, (d0 & 0xFFFF) >> 4);                        /* 00698C  lsr.w  #4,d0 */
                /* 00698E  add.w  d0,d2 */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t d = (d2 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d2, r);
                    fx = (r < s);
                }
                /* 006990  cmp.w  #$20,d2 */
                /* 006994  bcc  $699a */
                if ((d2 & 0xFFFF) < 0x20) {
L_006996:
                    /* 006996  addi.w  #$400,d2 */
                    {
                        uint32_t s = 0x400;
                        uint32_t d = (d2 & 0xFFFF);
                        uint32_t r = (d + s) & 0xFFFF;
                        SETW(d2, r);
                        fx = (r < s);
                    }
                }
L_00699A:
                /* 00699A  tst.b  $0(a4,d2.w) */
                /* 00699E  beq  $69c6 */
                if (rd8(a4 + SX16(d2)) == 0) {
                    goto L_0069C6;
                }
L_0069A0:
                wr16(a0 + 0x8, 0);                                   /* 0069A0  clr.w  $8(a0) */
                /* 0069A4  tst.w  $a(a0) */
                /* 0069A8  bmi  $6b6a */
                if ((int16_t)rd16(a0 + 0xA) < 0) {
                    goto L_006B6A;
                }
L_0069AC:
                /* 0069AC  movem.l  a0/d7,-(a7) */
                PUSH32(a0);
                PUSH32(d7);
                SETW(d0, (d3 & 0xFFFF));                             /* 0069B0  move.w  d3,d0 */
                SETW(d0, d0 - 2);                                    /* 0069B2  subq.w  #2,d0 */
                SETW(d1, (d4 & 0xFFFF));                             /* 0069B4  move.w  d4,d1 */
                /* 0069B6  subq.w  #3,d1 */
                {
                    uint32_t s = 3;
                    uint32_t d = (d1 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d1, r);
                    fx = (s > d);
                }
                d2 = 0x1C;                                           /* 0069B8  moveq  #28,d2 */
                CALL(sub_00A85E, 0x69BEU);                           /* 0069BA  bsr  $a85e */
L_0069BE:
                /* 0069BE  movem.l  (a7)+,d7/a0 */
                d7 = POP32();
                a0 = POP32();
                /* 0069C2  bra  $6b6a */
            } else {
L_0069C6:
                /* 0069C6  tst.w  $a(a0) */
                /* 0069CA  bmi  $6a86 */
                if ((int16_t)rd16(a0 + 0xA) >= 0) {
L_0069CE:
                    a4 = V_ENEMIES;                                      /* 0069CE  lea  $412(a5),a4 */
                    SETW(d0, (d3 & 0xFFFF));                             /* 0069D2  move.w  d3,d0 */
                    SETW(d1, (d4 & 0xFFFF));                             /* 0069D4  move.w  d4,d1 */
                    d6 = 0x18;                                           /* 0069D6  moveq  #24,d6 */
                    do {
                        BUDGET_CHECK(0x69D8U);
L_0069D8:
                        /* 0069D8  tst.w  $4(a4) */
                        /* 0069DC  beq  $6a7e */
                        if (rd16(a4 + 0x4) != 0) {
L_0069E0:
                            /* 0069E0  tst.w  $26(a4) */
                            /* 0069E4  bne  $6a7e */
                            if (rd16(a4 + 0x26) == 0) {
L_0069E8:
                                SETW(d2, (d0 & 0xFFFF));                             /* 0069E8  move.w  d0,d2 */
                                SETW(d2, d2 - rd16(a4));                             /* 0069EA  sub.w  $0(a4),d2 */
                                /* 0069EE  cmp.w  #$10,d2 */
                                /* 0069F2  bhi  $6a7e */
                                if ((d2 & 0xFFFF) <= 0x10) {
L_0069F6:
                                    SETW(d2, (d1 & 0xFFFF));                             /* 0069F6  move.w  d1,d2 */
                                    SETW(d2, d2 - rd16(a4 + 0x2));                       /* 0069F8  sub.w  $2(a4),d2 */
                                    /* 0069FC  cmp.w  #$15,d2 */
                                    /* 006A00  bhi  $6a7e */
                                    if ((d2 & 0xFFFF) <= 0x15) {
L_006A04:
                                        wr16(a0 + 0x8, 0);                                   /* 006A04  clr.w  $8(a0) */
                                        wr16(a4 + 0x26, 0xFFFF);                             /* 006A08  move.w  #$ffff,$26(a4) */
                                        /* 006A0E  cmpi.w  #$11,$8(a4) */
                                        /* 006A14  bcc  $6a1a */
                                        if (rd16(a4 + 0x8) < 0x11) {
                                            d0 = 3;                                              /* 006A16  moveq  #3,d0 */
                                            /* 006A18  bra  $6a1c */
                                        } else {
L_006A1A:
                                            d0 = 0xA;                                            /* 006A1A  moveq  #10,d0 */
                                        }
                                        a6 = a4;                                             /* 006A1C  movea.l  a4,a6 */
                                        CALL(sub_00B1E4, 0x6A22U);                           /* 006A1E  bsr  $b1e4 */
L_006A22:
                                        wr16(a4 + 0xC, 0);                                   /* 006A22  clr.w  $c(a4) */
                                        wr16(a4 + 0xE, 0);                                   /* 006A26  clr.w  $e(a4) */
                                        SETW(d0, rd16(a4));                                  /* 006A2A  move.w  $0(a4),d0 */
                                        SETW(d1, rd16(a4 + 0x2));                            /* 006A2E  move.w  $2(a4),d1 */
                                        SETW(d0, d0 + 4);                                    /* 006A32  addq.w  #4,d0 */
                                        /* 006A34  addq.w  #8,d1 */
                                        {
                                            uint32_t s = 8;
                                            uint32_t d = (d1 & 0xFFFF);
                                            uint32_t r = (d + s) & 0xFFFF;
                                            SETW(d1, r);
                                            fx = (r < s);
                                        }
                                        d2 = 0x1A;                                           /* 006A36  moveq  #26,d2 */
                                        CALL(sub_00A85E, 0x6A3CU);                           /* 006A38  bsr  $a85e */
L_006A3C:
                                        CALL(sub_007576, 0x6A40U);                           /* 006A3C  bsr  $7576 */
L_006A40:
                                        SETW(d0, d0 & 1);                                    /* 006A40  andi.w  #$1,d0 */
                                        SETW(d0, d0 + 4);                                    /* 006A44  addq.w  #4,d0 */
                                        CALL(sub_00E756, 0x6A4AU);                           /* 006A46  bsr  $e756 */
L_006A4A:
                                        wr32(V_02E2, rd32(V_02E2) + 0x32);                   /* 006A4A  addi.l  #$32,$2e2(a5) */
                                        /* 006A52  addi.w  #$1,$370(a5) */
                                        {
                                            uint32_t s = 1;
                                            uint32_t d = rd16(V_0370);
                                            uint32_t r = (d + s) & 0xFFFF;
                                            wr16(V_0370, r);
                                            fx = (r < s);
                                        }
                                        /* 006A58  tst.w  $a(a0) */
                                        /* 006A5C  beq  $6a7a */
                                        if (rd16(a0 + 0xA) != 0) {
                                            wr32(V_02E2, rd32(V_02E2) - 0x32);                   /* 006A5E  subi.l  #$32,$2e2(a5) */
                                            wr16(V_0370, rd16(V_0370) - 1);                      /* 006A66  subi.w  #$1,$370(a5) */
                                            wr32(V_037E, rd32(V_037E) + 0x32);                   /* 006A6C  addi.l  #$32,$37e(a5) */
                                            /* 006A74  addi.w  #$1,$40c(a5) */
                                            {
                                                uint32_t s = 1;
                                                uint32_t d = rd16(V_040C);
                                                uint32_t r = (d + s) & 0xFFFF;
                                                wr16(V_040C, r);
                                                fx = (r < s);
                                            }
                                        }
L_006A7A:
                                        /* 006A7A  bra  $6b6a */
                                        goto L_006B6A;
                                    }
                                }
                            }
                        }
L_006A7E:
                        a4 = a4 + 0x40;                                      /* 006A7E  lea  $40(a4),a4 */
                        /* 006A82  dbra  d6,$69d8 */
                        SETW(d6, d6 - 1);
                    } while ((d6 & 0xFFFF) != 0xFFFF);
                }
L_006A86:
                /* 006A86  tst.w  $254(a5) */
                /* 006A8A  bne  $6aec */
                if (rd16(V_0254) == 0) {
L_006A8C:
                    /* 006A8C  tst.w  $a(a0) */
                    /* 006A90  bpl  $6aec */
                    if ((int16_t)rd16(a0 + 0xA) < 0) {
L_006A92:
                        a4 = V_02DA;                                         /* 006A92  lea  $2da(a5),a4 */
                        wr16(V_027A, 0);                                     /* 006A96  clr.w  $27a(a5) */
                        SETW(d0, (d3 & 0xFFFF));                             /* 006A9A  move.w  d3,d0 */
                        SETW(d1, (d4 & 0xFFFF));                             /* 006A9C  move.w  d4,d1 */
                        d6 = 1;                                              /* 006A9E  moveq  #1,d6 */
                        do {
                            BUDGET_CHECK(0x6AA0U);
L_006AA0:
                            /* 006AA0  tst.w  (a4) */
                            /* 006AA2  beq  $6ae0 */
                            if (rd16(a4) != 0) {
L_006AA4:
                                /* 006AA4  tst.w  $50(a4) */
                                /* 006AA8  bne  $6ae0 */
                                if (rd16(a4 + 0x50) == 0) {
L_006AAA:
                                    /* 006AAA  tst.w  $2a(a4) */
                                    /* 006AAE  bne  $6ae0 */
                                    if (rd16(a4 + 0x2A) == 0) {
L_006AB0:
                                        SETW(d2, (d0 & 0xFFFF));                             /* 006AB0  move.w  d0,d2 */
                                        SETW(d2, d2 - rd16(a4 + 0x2));                       /* 006AB2  sub.w  $2(a4),d2 */
                                        SETW(d2, d2 - 0x24);                                 /* 006AB6  subi.w  #$24,d2 */
                                        /* 006ABA  cmp.w  #$12,d2 */
                                        /* 006ABE  bhi  $6ae0 */
                                        if ((d2 & 0xFFFF) <= 0x12) {
L_006AC0:
                                            /* 006AC0  cmp.w  #$6,d2 */
                                            /* 006AC4  bcs  $6ae0 */
                                            if ((d2 & 0xFFFF) >= 6) {
L_006AC6:
                                                SETW(d2, (d1 & 0xFFFF));                             /* 006AC6  move.w  d1,d2 */
                                                SETW(d2, d2 - rd16(a4 + 0x4));                       /* 006AC8  sub.w  $4(a4),d2 */
                                                /* 006ACC  sub.w  d5,d2 */
                                                {
                                                    uint32_t s = (d5 & 0xFFFF);
                                                    uint32_t d = (d2 & 0xFFFF);
                                                    uint32_t r = (d - s) & 0xFFFF;
                                                    SETW(d2, r);
                                                    fx = (s > d);
                                                }
                                                /* 006ACE  cmp.w  #$15,d2 */
                                                /* 006AD2  bhi  $6ae0 */
                                                if ((d2 & 0xFFFF) <= 0x15) {
L_006AD4:
                                                    wr16(a0 + 0x8, 0);                                   /* 006AD4  clr.w  $8(a0) */
                                                    CALL(player_start_death, 0x6ADCU);                   /* 006AD8  bsr  $aa42 */
L_006ADC:
                                                    /* 006ADC  bra  $6b6a */
                                                    goto L_006B6A;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
L_006AE0:
                            wr16(V_027A, rd16(V_027A) + 1);                      /* 006AE0  addq.w  #1,$27a(a5) */
                            a4 = a4 + 0x9C;                                      /* 006AE4  lea  $9c(a4),a4 */
                            /* 006AE8  dbra  d6,$6aa0 */
                            SETW(d6, d6 - 1);
                        } while ((d6 & 0xFFFF) != 0xFFFF);
                    }
                }
L_006AEC:
                a4 = V_VEHICLES;                                     /* 006AEC  lea  $1a76(a5),a4 */
                SETW(d0, (d3 & 0xFFFF));                             /* 006AF0  move.w  d3,d0 */
                SETW(d1, (d4 & 0xFFFF));                             /* 006AF2  move.w  d4,d1 */
                d6 = 4;                                              /* 006AF4  moveq  #4,d6 */
                do {
                    BUDGET_CHECK(0x6AF6U);
L_006AF6:
                    /* 006AF6  tst.w  (a4) */
                    /* 006AF8  beq  $6b42 */
                    if (rd16(a4) != 0) {
L_006AFA:
                        /* 006AFA  cmpi.w  #$fffe,$a(a0) */
                        /* 006B00  beq  $6b42 */
                        if (rd16(a0 + 0xA) != 0xFFFE) {
L_006B02:
                            SETW(d2, (d0 & 0xFFFF));                             /* 006B02  move.w  d0,d2 */
                            SETW(d2, d2 - rd16(a4 + 0x2));                       /* 006B04  sub.w  $2(a4),d2 */
                            /* 006B08  cmp.w  $e(a4),d2 */
                            /* 006B0C  bhi  $6b42 */
                            if ((d2 & 0xFFFF) <= rd16(a4 + 0xE)) {
                                SETW(d2, (d1 & 0xFFFF));                             /* 006B0E  move.w  d1,d2 */
                                SETW(d2, d2 - rd16(a4 + 0x4));                       /* 006B10  sub.w  $4(a4),d2 */
                                /* 006B14  cmp.w  $a(a4),d2 */
                                /* 006B18  bhi  $6b42 */
                                if ((d2 & 0xFFFF) <= rd16(a4 + 0xA)) {
                                    wr16(a0 + 0x8, 0);                                   /* 006B1A  clr.w  $8(a0) */
                                    /* 006B1E  movem.l  d2/d1/d0,-(a7) */
                                    PUSH32(d2);
                                    PUSH32(d1);
                                    PUSH32(d0);
                                    SETW(d0, (d3 & 0xFFFF));                             /* 006B22  move.w  d3,d0 */
                                    SETW(d1, (d4 & 0xFFFF));                             /* 006B24  move.w  d4,d1 */
                                    /* 006B26  tst.w  $a(a0) */
                                    /* 006B2A  bmi  $6b3c */
                                    if ((int16_t)rd16(a0 + 0xA) >= 0) {
                                        d2 = 0x1B;                                           /* 006B2C  moveq  #27,d2 */
                                        SETW(d0, d0 - 2);                                    /* 006B2E  subq.w  #2,d0 */
                                        /* 006B30  subq.w  #3,d1 */
                                        {
                                            uint32_t s = 3;
                                            uint32_t d = (d1 & 0xFFFF);
                                            uint32_t r = (d - s) & 0xFFFF;
                                            SETW(d1, r);
                                            fx = (s > d);
                                        }
                                        CALL(sub_00A85E, 0x6B36U);                           /* 006B32  bsr  $a85e */
L_006B36:
                                        d0 = 0;                                              /* 006B36  moveq  #0,d0 */
                                        CALL(sub_00E756, 0x6B3CU);                           /* 006B38  bsr  $e756 */
                                    }
L_006B3C:
                                    /* 006B3C  movem.l  (a7)+,d0-d2 */
                                    d0 = POP32();
                                    d1 = POP32();
                                    d2 = POP32();
                                    /* 006B40  bra  $6b4a */
                                    goto L_006B4A;
                                }
                            }
                        }
                    }
L_006B42:
                    a4 = a4 + 0x2E;                                      /* 006B42  lea  $2e(a4),a4 */
                    /* 006B46  dbra  d6,$6af6 */
                    SETW(d6, d6 - 1);
                } while ((d6 & 0xFFFF) != 0xFFFF);
L_006B4A:
                SETW(d0, rd16(a0 + 0x4));                            /* 006B4A  move.w  $4(a0),d0 */
                wr16(a0, rd16(a0) + (d0 & 0xFFFF));                  /* 006B4E  add.w  d0,(a0) */
                SETW(d0, rd16(a0 + 0x6));                            /* 006B50  move.w  $6(a0),d0 */
                /* 006B54  add.w  d0,$2(a0) */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t d = rd16(a0 + 0x2);
                    uint32_t r = (d + s) & 0xFFFF;
                    wr16(a0 + 0x2, r);
                    fx = (r < s);
                }
                /* 006B58  tst.w  $c(a0) */
                /* 006B5C  bne  $6b64 */
                if (rd16(a0 + 0xC) == 0) {
L_006B5E:
                    wr16(a0 + 0x8, 0);                                   /* 006B5E  clr.w  $8(a0) */
                    /* 006B62  bra  $6b6a */
                } else {
L_006B64:
                    /* 006B64  subi.w  #$1,$c(a0) */
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
L_006B6A:
        a0 = a0 + 0xE;                                       /* 006B6A  lea  $e(a0),a0 */
        /* 006B6E  dbra  d7,$6956 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006B72:
    RTS();                                               /* 006B72  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6956U: goto L_006956;
        case 0x695EU: goto L_00695E;
        case 0x6972U: goto L_006972;
        case 0x6996U: goto L_006996;
        case 0x699AU: goto L_00699A;
        case 0x69A0U: goto L_0069A0;
        case 0x69ACU: goto L_0069AC;
        case 0x69BEU: goto L_0069BE;
        case 0x69C6U: goto L_0069C6;
        case 0x69CEU: goto L_0069CE;
        case 0x69D8U: goto L_0069D8;
        case 0x69E0U: goto L_0069E0;
        case 0x69E8U: goto L_0069E8;
        case 0x69F6U: goto L_0069F6;
        case 0x6A04U: goto L_006A04;
        case 0x6A1AU: goto L_006A1A;
        case 0x6A22U: goto L_006A22;
        case 0x6A3CU: goto L_006A3C;
        case 0x6A40U: goto L_006A40;
        case 0x6A4AU: goto L_006A4A;
        case 0x6A7AU: goto L_006A7A;
        case 0x6A7EU: goto L_006A7E;
        case 0x6A86U: goto L_006A86;
        case 0x6A8CU: goto L_006A8C;
        case 0x6A92U: goto L_006A92;
        case 0x6AA0U: goto L_006AA0;
        case 0x6AA4U: goto L_006AA4;
        case 0x6AAAU: goto L_006AAA;
        case 0x6AB0U: goto L_006AB0;
        case 0x6AC0U: goto L_006AC0;
        case 0x6AC6U: goto L_006AC6;
        case 0x6AD4U: goto L_006AD4;
        case 0x6ADCU: goto L_006ADC;
        case 0x6AE0U: goto L_006AE0;
        case 0x6AECU: goto L_006AEC;
        case 0x6AF6U: goto L_006AF6;
        case 0x6AFAU: goto L_006AFA;
        case 0x6B02U: goto L_006B02;
        case 0x6B36U: goto L_006B36;
        case 0x6B3CU: goto L_006B3C;
        case 0x6B42U: goto L_006B42;
        case 0x6B4AU: goto L_006B4A;
        case 0x6B5EU: goto L_006B5E;
        case 0x6B64U: goto L_006B64;
        case 0x6B6AU: goto L_006B6A;
        case 0x6B72U: goto L_006B72;
        }
    }
}

/* 006B74  game routine */
void sub_006B74(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 006B74  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a0 = V_BULLETS;                                      /* 006B78  lea  $1012(a5),a0 */
    SETW(d7, 0x1D);                                      /* 006B7C  move.w  #$1d,d7 */
    do {
        BUDGET_CHECK(0x6B80U);
L_006B80:
        /* 006B80  tst.w  $8(a0) */
        /* 006B84  bne  $6bda */
        if (rd16(a0 + 0x8) == 0) {
L_006B86:
            wr16(a0, (d0 & 0xFFFF));                             /* 006B86  move.w  d0,(a0) */
            wr16(a0 + 0x2, (d1 & 0xFFFF));                       /* 006B88  move.w  d1,$2(a0) */
            wr16(a0 + 0x4, (d3 & 0xFFFF));                       /* 006B8C  move.w  d3,$4(a0) */
            wr16(a0 + 0x6, (d4 & 0xFFFF));                       /* 006B90  move.w  d4,$6(a0) */
            wr16(a0 + 0xA, (d5 & 0xFFFF));                       /* 006B94  move.w  d5,$a(a0) */
            wr16(a0 + 0xC, (d2 & 0xFFFF));                       /* 006B98  move.w  d2,$c(a0) */
            wr16(a0 + 0x8, 0xFFFF);                              /* 006B9C  move.w  #$ffff,$8(a0) */
            fz = !(rd8(V_0273) & 0x2);                           /* 006BA2  btst  #$1,$273(a5) */
            /* 006BA8  beq  $6bbc */
            if (COND(6)) {
L_006BAA:
                SETW(d0, rd16(a0 + 0x4));                            /* 006BAA  move.w  $4(a0),d0 */
                SETW(d0, (uint32_t)((int32_t)SX16((d0 & 0xFFFF)) >> 2)); /* 006BAE  asr.w  #2,d0 */
                wr16(a0, rd16(a0) + (d0 & 0xFFFF));                  /* 006BB0  add.w  d0,(a0) */
                SETW(d0, rd16(a0 + 0x6));                            /* 006BB2  move.w  $6(a0),d0 */
                SETW(d0, (uint32_t)((int32_t)SX16((d0 & 0xFFFF)) >> 2)); /* 006BB6  asr.w  #2,d0 */
                /* 006BB8  add.w  d0,$2(a0) */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t d = rd16(a0 + 0x2);
                    uint32_t r = (d + s) & 0xFFFF;
                    wr16(a0 + 0x2, r);
                    fx = (r < s);
                }
            }
L_006BBC:
            fz = !(rd8(V_0273) & 0x1);                           /* 006BBC  btst  #$0,$273(a5) */
            /* 006BC2  beq  $6bd6 */
            if (COND(6)) {
L_006BC4:
                SETW(d0, rd16(a0 + 0x4));                            /* 006BC4  move.w  $4(a0),d0 */
                SETW(d0, (uint32_t)((int32_t)SX16((d0 & 0xFFFF)) >> 1)); /* 006BC8  asr.w  #1,d0 */
                wr16(a0, rd16(a0) + (d0 & 0xFFFF));                  /* 006BCA  add.w  d0,(a0) */
                SETW(d0, rd16(a0 + 0x6));                            /* 006BCC  move.w  $6(a0),d0 */
                SETW(d0, (uint32_t)((int32_t)SX16((d0 & 0xFFFF)) >> 1)); /* 006BD0  asr.w  #1,d0 */
                /* 006BD2  add.w  d0,$2(a0) */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t d = rd16(a0 + 0x2);
                    uint32_t r = (d + s) & 0xFFFF;
                    wr16(a0 + 0x2, r);
                    fx = (r < s);
                }
            }
L_006BD6:
            /* 006BD6  bra  $6be2 */
            goto L_006BE2;
        }
L_006BDA:
        a0 = a0 + 0xE;                                       /* 006BDA  lea  $e(a0),a0 */
        /* 006BDE  dbra  d7,$6b80 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006BE2:
    /* 006BE2  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 006BE6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6B80U: goto L_006B80;
        case 0x6B86U: goto L_006B86;
        case 0x6BAAU: goto L_006BAA;
        case 0x6BBCU: goto L_006BBC;
        case 0x6BC4U: goto L_006BC4;
        case 0x6BD6U: goto L_006BD6;
        case 0x6BDAU: goto L_006BDA;
        case 0x6BE2U: goto L_006BE2;
        }
    }
}

/* 006BEA  game routine */
void sub_006BEA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = V_1206;                                         /* 006BEA  lea  $1206(a5),a0 */
    SETW(d7, 0x10);                                      /* 006BEE  move.w  #$10,d7 */
    do {
        BUDGET_CHECK(0x6BF2U);
L_006BF2:
        /* 006BF2  tst.w  $4(a0) */
        /* 006BF6  bne  $6c1e */
        if (rd16(a0 + 0x4) == 0) {
L_006BF8:
            wr16(a0 + 0x4, 0xFFFF);                              /* 006BF8  move.w  #$ffff,$4(a0) */
            wr16(a0, (d0 & 0xFFFF));                             /* 006BFE  move.w  d0,(a0) */
            wr16(a0 + 0x2, (d1 & 0xFFFF));                       /* 006C00  move.w  d1,$2(a0) */
            wr16(a0 + 0xC, (d5 & 0xFFFF));                       /* 006C04  move.w  d5,$c(a0) */
            wr16(a0 + 0xA, 0x18);                                /* 006C08  move.w  #$18,$a(a0) */
            wr16(a0 + 0x8, 0x24);                                /* 006C0E  move.w  #$24,$8(a0) */
            wr16(a0 + 0x10, (d2 & 0xFFFF));                      /* 006C14  move.w  d2,$10(a0) */
            wr16(a0 + 0x12, (d3 & 0xFFFF));                      /* 006C18  move.w  d3,$12(a0) */
            /* 006C1C  bra  $6c26 */
            goto L_006C26;
        }
L_006C1E:
        a0 = a0 + 0x14;                                      /* 006C1E  lea  $14(a0),a0 */
        /* 006C22  dbra  d7,$6bf2 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006C26:
    RTS();                                               /* 006C26  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6BF2U: goto L_006BF2;
        case 0x6BF8U: goto L_006BF8;
        case 0x6C1EU: goto L_006C1E;
        case 0x6C26U: goto L_006C26;
        }
    }
}

/* 006C28  game routine */
void sub_006C28(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1206;                                         /* 006C28  lea  $1206(a5),a4 */
    SETW(d7, 0x10);                                      /* 006C2C  move.w  #$10,d7 */
    do {
        BUDGET_CHECK(0x6C30U);
L_006C30:
        /* 006C30  tst.w  $4(a4) */
        /* 006C34  beq  $6c9e */
        if (rd16(a4 + 0x4) != 0) {
L_006C36:
            SETW(d0, rd16(a4 + 0x10));                           /* 006C36  move.w  $10(a4),d0 */
            wr16(a4, rd16(a4) + (d0 & 0xFFFF));                  /* 006C3A  add.w  d0,(a4) */
            SETW(d0, rd16(a4 + 0x12));                           /* 006C3C  move.w  $12(a4),d0 */
            wr16(a4 + 0x2, rd16(a4 + 0x2) + (d0 & 0xFFFF));      /* 006C40  add.w  d0,$2(a4) */
            /* 006C44  subq.w  #1,$8(a4) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a4 + 0x8);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a4 + 0x8, r);
                fx = (s > d);
            }
            /* 006C48  cmpi.w  #$20,$8(a4) */
            /* 006C4E  bcs  $6c58 */
            if (rd16(a4 + 0x8) >= 0x20) {
L_006C50:
                wr16(a4 + 0xA, 0x18);                                /* 006C50  move.w  #$18,$a(a4) */
                /* 006C56  bra  $6c76 */
            } else {
L_006C58:
                /* 006C58  cmpi.w  #$14,$8(a4) */
                /* 006C5E  bcs  $6c68 */
                if (rd16(a4 + 0x8) >= 0x14) {
L_006C60:
                    wr16(a4 + 0xA, 0x19);                                /* 006C60  move.w  #$19,$a(a4) */
                    /* 006C66  bra  $6c76 */
                } else {
L_006C68:
                    /* 006C68  cmpi.w  #$10,$8(a4) */
                    /* 006C6E  bcs  $6c76 */
                    if (rd16(a4 + 0x8) >= 0x10) {
L_006C70:
                        wr16(a4 + 0xA, 0x18);                                /* 006C70  move.w  #$18,$a(a4) */
                    }
                }
            }
L_006C76:
            /* 006C76  cmpi.w  #$10,$8(a4) */
            /* 006C7C  bne  $6c9e */
            if (rd16(a4 + 0x8) == 0x10) {
L_006C7E:
                SETW(d0, rd16(a4));                                  /* 006C7E  move.w  (a4),d0 */
                SETW(d1, rd16(a4 + 0x2));                            /* 006C80  move.w  $2(a4),d1 */
                SETW(d1, d1 - 7);                                    /* 006C84  subi.w  #$7,d1 */
                /* 006C88  subi.w  #$5,d0 */
                {
                    uint32_t s = 5;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d0, r);
                    fx = (s > d);
                }
                SETW(d2, rd16(a4 + 0xC));                            /* 006C8C  move.w  $c(a4),d2 */
                wr16(a4 + 0x4, 0);                                   /* 006C90  clr.w  $4(a4) */
                /* 006C94  movem.l  a4/d7,-(a7) */
                PUSH32(a4);
                PUSH32(d7);
                CALL(sub_006CA8, 0x6C9AU);                           /* 006C98  bsr  $6ca8 */
L_006C9A:
                /* 006C9A  movem.l  (a7)+,d7/a4 */
                d7 = POP32();
                a4 = POP32();
            }
        }
L_006C9E:
        a4 = a4 + 0x14;                                      /* 006C9E  lea  $14(a4),a4 */
        /* 006CA2  dbra  d7,$6c30 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006CA6:
    RTS();                                               /* 006CA6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6C30U: goto L_006C30;
        case 0x6C36U: goto L_006C36;
        case 0x6C50U: goto L_006C50;
        case 0x6C58U: goto L_006C58;
        case 0x6C60U: goto L_006C60;
        case 0x6C68U: goto L_006C68;
        case 0x6C70U: goto L_006C70;
        case 0x6C76U: goto L_006C76;
        case 0x6C7EU: goto L_006C7E;
        case 0x6C9AU: goto L_006C9A;
        case 0x6C9EU: goto L_006C9E;
        case 0x6CA6U: goto L_006CA6;
        }
    }
}

/* 006CA8  game routine */
void sub_006CA8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 006CA8  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a4 = V_1472;                                         /* 006CAC  lea  $1472(a5),a4 */
    SETW(d7, 0x10);                                      /* 006CB0  move.w  #$10,d7 */
    do {
        BUDGET_CHECK(0x6CB4U);
L_006CB4:
        /* 006CB4  tst.w  $4(a4) */
        /* 006CB8  bne  $6cec */
        if (rd16(a4 + 0x4) == 0) {
L_006CBA:
            /* 006CBA  addi.w  #$7,d0 */
            {
                uint32_t s = 7;
                uint32_t d = (d0 & 0xFFFF);
                uint32_t r = (d + s) & 0xFFFF;
                SETW(d0, r);
                fx = (r < s);
            }
            SETW(d0, d0 & 0xFFF0);                               /* 006CBE  andi.w  #$fff0,d0 */
            wr16(a4, (d0 & 0xFFFF));                             /* 006CC2  move.w  d0,(a4) */
            wr16(a4 + 0x2, (d1 & 0xFFFF));                       /* 006CC4  move.w  d1,$2(a4) */
            wr16(a4 + 0xA, (d2 & 0xFFFF));                       /* 006CC8  move.w  d2,$a(a4) */
            wr16(a4 + 0x6, 0);                                   /* 006CCC  clr.w  $6(a4) */
            wr16(a4 + 0xC, 2);                                   /* 006CD0  move.w  #$2,$c(a4) */
            wr16(a4 + 0x8, 0x14);                                /* 006CD6  move.w  #$14,$8(a4) */
            wr16(a4 + 0x4, 0xFFFF);                              /* 006CDC  move.w  #$ffff,$4(a4) */
            SETW(d0, 6);                                         /* 006CE2  move.w  #$6,d0 */
            CALL(sub_00E756, 0x6CEAU);                           /* 006CE6  bsr  $e756 */
L_006CEA:
            /* 006CEA  bra  $6cf4 */
            goto L_006CF4;
        }
L_006CEC:
        a4 = a4 + 0xE;                                       /* 006CEC  lea  $e(a4),a4 */
        /* 006CF0  dbra  d7,$6cb4 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006CF4:
    /* 006CF4  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 006CF8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6CB4U: goto L_006CB4;
        case 0x6CBAU: goto L_006CBA;
        case 0x6CEAU: goto L_006CEA;
        case 0x6CECU: goto L_006CEC;
        case 0x6CF4U: goto L_006CF4;
        }
    }
}

/* 006CFA  game routine */
void sub_006CFA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1472;                                         /* 006CFA  lea  $1472(a5),a4 */
    SETW(d7, 0x10);                                      /* 006CFE  move.w  #$10,d7 */
    do {
        BUDGET_CHECK(0x6D02U);
L_006D02:
        /* 006D02  subq.w  #1,$8(a4) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(a4 + 0x8);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(a4 + 0x8, r);
            fx = (s > d);
        }
        a4 = a4 + 0xE;                                       /* 006D06  lea  $e(a4),a4 */
        /* 006D0A  dbra  d7,$6d02 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006D0E:
    RTS();                                               /* 006D0E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6D02U: goto L_006D02;
        case 0x6D0EU: goto L_006D0E;
        }
    }
}

/* 006D12  Draws explosions */
void draw_explosions(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1472;                                         /* 006D12  lea  $1472(a5),a4 */
    SETW(d7, 0x10);                                      /* 006D16  move.w  #$10,d7 */
    do {
        BUDGET_CHECK(0x6D1AU);
L_006D1A:
        /* 006D1A  tst.w  $4(a4) */
        /* 006D1E  beq  $6d5a */
        HOOK(24, 0x6D1AU);
        if (rd16(a4 + 0x4) != 0) {
L_006D20:
            SETW(d0, rd16(a4));                                  /* 006D20  move.w  (a4),d0 */
            SETW(d1, rd16(a4 + 0x2));                            /* 006D22  move.w  $2(a4),d1 */
            SETW(d6, rd16(a4 + 0x6));                            /* 006D26  move.w  $6(a4),d6 */
            /* 006D2A  tst.w  $c(a4) */
            /* 006D2E  beq  $6d36 */
            if (rd16(a4 + 0xC) != 0) {
L_006D30:
                /* 006D30  subq.w  #1,$c(a4) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a4 + 0xC);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(a4 + 0xC, r);
                    fx = (s > d);
                }
                /* 006D34  bra  $6d50 */
            } else {
L_006D36:
                wr16(a4 + 0xC, 1);                                   /* 006D36  move.w  #$1,$c(a4) */
                /* 006D3C  addi.w  #$1,$6(a4) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a4 + 0x6);
                    uint32_t r = (d + s) & 0xFFFF;
                    wr16(a4 + 0x6, r);
                    fx = (r < s);
                }
                /* 006D42  cmpi.w  #$b,$6(a4) */
                /* 006D48  bcs  $6d50 */
                if (rd16(a4 + 0x6) >= 0xB) {
L_006D4A:
                    wr16(a4 + 0x4, 0);                                   /* 006D4A  clr.w  $4(a4) */
                    /* 006D4E  bra  $6d5a */
                    goto L_006D5A;
                }
            }
L_006D50:
            /* 006D50  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
            CALL(sub_006D64, 0x6D56U);                           /* 006D54  bsr  $6d64 */
L_006D56:
            /* 006D56  movem.l  (a7)+,d0-d7/a0-a6 */
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
L_006D5A:
        /* 006D5A  lea  $e(a4),a4 */
        HOOK(25, 0x6D5AU);
        a4 = a4 + 0xE;
        /* 006D5E  dbra  d7,$6d1a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006D62:
    RTS();                                               /* 006D62  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6D1AU: goto L_006D1A;
        case 0x6D20U: goto L_006D20;
        case 0x6D30U: goto L_006D30;
        case 0x6D36U: goto L_006D36;
        case 0x6D4AU: goto L_006D4A;
        case 0x6D50U: goto L_006D50;
        case 0x6D56U: goto L_006D56;
        case 0x6D5AU: goto L_006D5A;
        case 0x6D62U: goto L_006D62;
        }
    }
}

/* 006D64  game routine */
void sub_006D64(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, d0 - 0x64);                                 /* 006D64  subi.w  #$64,d0 */
    SETW(d0, d0 & 0xFFF0);                               /* 006D68  andi.w  #$fff0,d0 */
    SETW(d0, d0 + 0x40);                                 /* 006D6C  addi.w  #$40,d0 */
    /* 006D70  subi.w  #$64,d1 */
    {
        uint32_t s = 0x64;
        uint32_t d = (d1 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d1, r);
        fx = (s > d);
    }
    /* 006D74  tst.w  d1 */
    /* 006D76  bpl  $6d7a */
    if ((int16_t)(d1 & 0xFFFF) < 0) {
        SETW(d1, 0);                                         /* 006D78  clr.w  d1 */
    }
L_006D7A:
    d1 = (d1 & 0xFFFF) * 0xA0;                           /* 006D7A  mulu.w  #$a0,d1 */
    a0 = rd32(V_DRAW_BUFFER);                            /* 006D7E  movea.l  $28e(a5),a0 */
    a0 += SX16((d1 & 0xFFFF));                           /* 006D82  adda.w  d1,a0 */
    SETW(d5, 0);                                         /* 006D84  clr.w  d5 */
    wr16(V_0278, 0);                                     /* 006D86  clr.w  $278(a5) */
    /* 006D8A  cmp.w  #$e0,d0 */
    /* 006D8E  bls  $6d98 */
    if ((d0 & 0xFFFF) > 0xE0) {
L_006D90:
        SETW(d5, ~(d5 & 0xFFFF));                            /* 006D90  not.w  d5 */
        /* 006D92  cmp.w  #$f0,d0 */
        /* 006D96  bhi  $6da8 */
        if ((d0 & 0xFFFF) > 0xF0) {
            goto L_006DA8;
        }
    }
L_006D98:
    /* 006D98  cmp.w  #$40,d0 */
    /* 006D9C  bcc  $6db0 */
    if ((d0 & 0xFFFF) >= 0x40) {
        goto L_006DB0;
    }
L_006D9E:
    wr8(V_0278, 0xFF);                                   /* 006D9E  st  $278(a5) */
    /* 006DA2  cmp.w  #$30,d0 */
    /* 006DA6  bcc  $6db0 */
    if ((d0 & 0xFFFF) >= 0x30) {
        goto L_006DB0;
    }
L_006DA8:
    wr16(a4 + 0x4, 0);                                   /* 006DA8  clr.w  $4(a4) */
    /* 006DAC  bra  $6e4a */
    goto L_006E4A;
L_006DB0:
    SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006DB0  lsr.w  #1,d0 */
    SETW(d0, d0 & 0xFFF8);                               /* 006DB2  andi.w  #$fff8,d0 */
    a0 += SX16((d0 & 0xFFFF));                           /* 006DB6  adda.w  d0,a0 */
    d7 = 0x15;                                           /* 006DB8  moveq  #21,d7 */
    d6 = (d6 & 0xFFFF) * 0x108;                          /* 006DBA  mulu.w  #$108,d6 */
    a2 = 0x388DC;                                        /* 006DBE  lea  $388dc.l,a2 */
    a2 += d6;                                            /* 006DC4  adda.l  d6,a2 */
    SETW(d0, rd16(a4 + 0x2));                            /* 006DC6  move.w  $2(a4),d0 */
    SETW(d0, d0 - 0x64);                                 /* 006DCA  subi.w  #$64,d0 */
    /* 006DCE  tst.w  d0 */
    /* 006DD0  bpl  $6de4 */
    if ((int16_t)(d0 & 0xFFFF) < 0) {
        /* 006DD2  add.w  d0,d7 */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t d = (d7 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d7, r);
            fn = !!(r & 0x8000);
            fx = (r < s);
        }
        /* 006DD4  bmi  $6e4a */
        if (COND(11)) {
            goto L_006E4A;
        }
        /* 006DD8  neg.w  d0 */
        { uint32_t d, r, ea; (void)ea;
        d = (d0 & 0xFFFFU);
        r = (0 - d) & 0xFFFFU;
        d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
        FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
        d0 = SX16(d0);                                       /* 006DDA  ext.l  d0 */
        d0 = (d0 & 0xFFFF) * 0xC;                            /* 006DDC  mulu.w  #$c,d0 */
        a2 += d0;                                            /* 006DE0  adda.l  d0,a2 */
        /* 006DE2  bra  $6df0 */
    } else {
L_006DE4:
        /* 006DE4  subi.w  #$b2,d0 */
        {
            uint32_t s = 0xB2;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d0, r);
            fn = !!(r & 0x8000);
            fx = (s > d);
        }
        /* 006DE8  bmi  $6df0 */
        if (COND(10)) {
L_006DEA:
            /* 006DEA  sub.w  d0,d7 */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t d = (d7 & 0xFFFF);
                uint32_t r = (d - s) & 0xFFFF;
                SETW(d7, r);
                fn = !!(r & 0x8000);
                fx = (s > d);
            }
            /* 006DEC  bmi  $6e4a */
            if (COND(11)) {
                goto L_006E4A;
            }
        }
    }
    do {
        BUDGET_CHECK(0x6DF0U);
L_006DF0:
        /* 006DF0  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        /* 006DF2  move.w  (a2)+,d1 */
        SETW(d1, rd16(a2));
        a2 += 2;
        /* 006DF4  move.w  (a2)+,d2 */
        SETW(d2, rd16(a2));
        a2 += 2;
        /* 006DF6  tst.w  $278(a5) */
        /* 006DFA  beq  $6e02 */
        if (rd16(V_0278) != 0) {
L_006DFC:
            a0 += SX16(8);                                       /* 006DFC  adda.w  #$8,a0 */
            /* 006E00  bra  $6e18 */
        } else {
L_006E02:
            SETW(d4, (d0 & 0xFFFF));                             /* 006E02  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006E04  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006E06  or.w  d2,d4 */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 006E08  not.w  d4 */
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E0A  and.w  d4,(a0) */
            /* 006E0C  or.w  d0,(a0)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E0E  and.w  d4,(a0) */
            /* 006E10  or.w  d1,(a0)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E12  and.w  d4,(a0) */
            /* 006E14  or.w  d2,(a0)+ */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 006E16  and.w  d4,(a0)+ */
            {
                uint32_t s = (d4 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(ea, r);
            }
        }
L_006E18:
        /* 006E18  move.w  (a2)+,d0 */
        SETW(d0, rd16(a2));
        a2 += 2;
        /* 006E1A  move.w  (a2)+,d1 */
        SETW(d1, rd16(a2));
        a2 += 2;
        /* 006E1C  move.w  (a2)+,d2 */
        SETW(d2, rd16(a2));
        a2 += 2;
        /* 006E1E  tst.w  d5 */
        /* 006E20  beq  $6e2a */
        if ((d5 & 0xFFFF) != 0) {
L_006E22:
            a0 += 0x98;                                          /* 006E22  adda.l  #$98,a0 */
            /* 006E28  bra  $6e46 */
        } else {
L_006E2A:
            SETW(d4, (d0 & 0xFFFF));                             /* 006E2A  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006E2C  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006E2E  or.w  d2,d4 */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 006E30  not.w  d4 */
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E32  and.w  d4,(a0) */
            /* 006E34  or.w  d0,(a0)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E36  and.w  d4,(a0) */
            /* 006E38  or.w  d1,(a0)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E3A  and.w  d4,(a0) */
            /* 006E3C  or.w  d2,(a0)+ */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t ea = a0;
                a0 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006E3E  and.w  d4,(a0) */
            a0 += 0x92;                                          /* 006E40  adda.l  #$92,a0 */
        }
L_006E46:
        /* 006E46  dbra  d7,$6df0 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_006E4A:
    RTS();                                               /* 006E4A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6D7AU: goto L_006D7A;
        case 0x6D90U: goto L_006D90;
        case 0x6D98U: goto L_006D98;
        case 0x6D9EU: goto L_006D9E;
        case 0x6DB0U: goto L_006DB0;
        case 0x6DE4U: goto L_006DE4;
        case 0x6DEAU: goto L_006DEA;
        case 0x6DF0U: goto L_006DF0;
        case 0x6DFCU: goto L_006DFC;
        case 0x6E02U: goto L_006E02;
        case 0x6E18U: goto L_006E18;
        case 0x6E22U: goto L_006E22;
        case 0x6E2AU: goto L_006E2A;
        case 0x6E46U: goto L_006E46;
        case 0x6E4AU: goto L_006E4A;
        }
    }
}

/* 006E4C  game routine */
void sub_006E4C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_18E6;                                         /* 006E4C  lea  $18e6(a5),a6 */
    SETW(d6, 0x17C);                                     /* 006E50  move.w  #$17c,d6 */
    do {
        BUDGET_CHECK(0x6E54U);
L_006E54:
        /* 006E54  tst.w  $4(a6,d6.w) */
        /* 006E58  beq  $6f18 */
        HOOK(26, 0x6E54U);
        if (rd16(a6 + 0x4 + SX16(d6)) != 0) {
            /* 006E5C  move.w  $2(a6,d6.w),d0 */
            /* 006E60  bpl  $6e64 */
            SETW(d0, rd16(a6 + 0x2 + SX16(d6)));
            if ((int16_t)(d0 & 0xFFFF) < 0) {
                SETW(d0, 0);                                         /* 006E62  clr.w  d0 */
            }
            d0 = (d0 & 0xFFFF) * 0xA0;                           /* 006E64  mulu.w  #$a0,d0 */
            a0 = rd32(V_DRAW_BUFFER);                            /* 006E68  movea.l  $28e(a5),a0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 006E6C  adda.w  d0,a0 */
            SETW(d0, rd16(a6 + SX16(d6)));                       /* 006E6E  move.w  $0(a6,d6.w),d0 */
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 006E72  lsr.w  #1,d0 */
            SETW(d0, d0 & 0xFFF8);                               /* 006E74  andi.w  #$fff8,d0 */
            a0 += SX16((d0 & 0xFFFF));                           /* 006E78  adda.w  d0,a0 */
            d7 = 0x1F;                                           /* 006E7A  moveq  #31,d7 */
            SETW(d0, 0);                                         /* 006E7C  clr.w  d0 */
            SETW(d0, rd16(a6 + 0xA + SX16(d6)));                 /* 006E7E  move.w  $a(a6,d6.w),d0 */
            SETW(d0, d0 - 0xF0);                                 /* 006E82  subi.w  #$f0,d0 */
            wr16(V_0198, (d0 & 0xFFFF));                         /* 006E86  move.w  d0,$198(a5) */
            d0 = (d0 & 0xFFFF) * 0x200;                          /* 006E8A  mulu.w  #$200,d0 */
            a2 = 0x45736;                                        /* 006E8E  lea  $45736.l,a2 */
            a2 += d0;                                            /* 006E94  adda.l  d0,a2 */
            /* 006E96  move.w  $2(a6,d6.w),d0 */
            /* 006E9A  bpl  $6eac */
            SETW(d0, rd16(a6 + 0x2 + SX16(d6)));
            if ((int16_t)(d0 & 0xFFFF) < 0) {
                /* 006E9C  add.w  d0,d7 */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t d = (d7 & 0xFFFF);
                    uint32_t r = (d + s) & 0xFFFF;
                    SETW(d7, r);
                    fn = !!(r & 0x8000);
                }
                /* 006E9E  bmi  $6f18 */
                if (COND(11)) {
                    goto L_006F18;
                }
                /* 006EA2  neg.w  d0 */
                { uint32_t d, r, ea; (void)ea;
                d = (d0 & 0xFFFFU);
                r = (0 - d) & 0xFFFFU;
                d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
                FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
                d0 = SX16(d0);                                       /* 006EA4  ext.l  d0 */
                SETW(d0, (d0 & 0xFFFF) << 4);                        /* 006EA6  lsl.w  #4,d0 */
                a2 += d0;                                            /* 006EA8  adda.l  d0,a2 */
                /* 006EAA  bra  $6eb8 */
            } else {
                /* 006EAC  subi.w  #$ac,d0 */
                {
                    uint32_t s = 0xAC;
                    uint32_t d = (d0 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d0, r);
                    fn = !!(r & 0x8000);
                }
                /* 006EB0  bmi  $6eb8 */
                if (COND(10)) {
                    /* 006EB2  sub.w  d0,d7 */
                    {
                        uint32_t s = (d0 & 0xFFFF);
                        uint32_t d = (d7 & 0xFFFF);
                        uint32_t r = (d - s) & 0xFFFF;
                        SETW(d7, r);
                        fn = !!(r & 0x8000);
                    }
                    /* 006EB4  bmi  $6f18 */
                    if (COND(11)) {
                        goto L_006F18;
                    }
                }
            }
            do {
                BUDGET_CHECK(0x6EB8U);
L_006EB8:
                SETW(d0, rd16(a2));                                  /* 006EB8  move.w  (a2),d0 */
                SETW(d1, rd16(a2 + 0x4));                            /* 006EBA  move.w  $4(a2),d1 */
                SETW(d2, rd16(a2 + 0x8));                            /* 006EBE  move.w  $8(a2),d2 */
                SETW(d3, rd16(a2 + 0xC));                            /* 006EC2  move.w  $c(a2),d3 */
                SETW(d4, (d0 & 0xFFFF));                             /* 006EC6  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006EC8  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006ECA  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 006ECC  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 006ECE  not.w  d4 */
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006ED0  and.w  d4,(a0) */
                /* 006ED2  or.w  d0,(a0)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006ED4  and.w  d4,(a0) */
                /* 006ED6  or.w  d1,(a0)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006ED8  and.w  d4,(a0) */
                /* 006EDA  or.w  d2,(a0)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006EDC  and.w  d4,(a0) */
                /* 006EDE  or.w  d3,(a0)+ */
                {
                    uint32_t s = (d3 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                SETW(d0, rd16(a2 + 0x2));                            /* 006EE0  move.w  $2(a2),d0 */
                SETW(d1, rd16(a2 + 0x6));                            /* 006EE4  move.w  $6(a2),d1 */
                SETW(d2, rd16(a2 + 0xA));                            /* 006EE8  move.w  $a(a2),d2 */
                SETW(d3, rd16(a2 + 0xE));                            /* 006EEC  move.w  $e(a2),d3 */
                SETW(d4, (d0 & 0xFFFF));                             /* 006EF0  move.w  d0,d4 */
                SETW(d4, d4 | (d1 & 0xFFFF));                        /* 006EF2  or.w  d1,d4 */
                SETW(d4, d4 | (d2 & 0xFFFF));                        /* 006EF4  or.w  d2,d4 */
                SETW(d4, d4 | (d3 & 0xFFFF));                        /* 006EF6  or.w  d3,d4 */
                SETW(d4, ~(d4 & 0xFFFF));                            /* 006EF8  not.w  d4 */
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006EFA  and.w  d4,(a0) */
                /* 006EFC  or.w  d0,(a0)+ */
                {
                    uint32_t s = (d0 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006EFE  and.w  d4,(a0) */
                /* 006F00  or.w  d1,(a0)+ */
                {
                    uint32_t s = (d1 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006F02  and.w  d4,(a0) */
                /* 006F04  or.w  d2,(a0)+ */
                {
                    uint32_t s = (d2 & 0xFFFF);
                    uint32_t ea = a0;
                    a0 += 2;
                    uint32_t d = rd16(ea);
                    uint32_t r = (d | s) & 0xFFFF;
                    wr16(ea, r);
                }
                wr16(a0, rd16(a0) & (d4 & 0xFFFF));                  /* 006F06  and.w  d4,(a0) */
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 006F08  or.w  d3,(a0) */
                a0 += 0x92;                                          /* 006F0A  adda.l  #$92,a0 */
                a2 = a2 + 0x10;                                      /* 006F10  lea  $10(a2),a2 */
                /* 006F14  dbra  d7,$6eb8 */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
        }
L_006F18:
        /* 006F18  subi.w  #$14,d6 */
        HOOK(27, 0x6F18U);
        {
            uint32_t s = 0x14;
            uint32_t d = (d6 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d6, r);
            fn = !!(r & 0x8000);
            fx = (s > d);
        }
        /* 006F1C  bpl  $6e54 */
    } while (COND(10));
L_006F20:
    RTS();                                               /* 006F20  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6E54U: goto L_006E54;
        case 0x6EB8U: goto L_006EB8;
        case 0x6F18U: goto L_006F18;
        case 0x6F20U: goto L_006F20;
        }
    }
}

/* 006F22  game routine */
void sub_006F22(void)
{
    wr16(V_0274, 0xF2);                                  /* 006F22  move.w  #$f2,$274(a5) */
    /* 006F28  bra  $6f3e */
    { sub_006F3E(); return; }
}

/* 006F2A  game routine */
void sub_006F2A(void)
{
    wr16(V_0274, 0xF1);                                  /* 006F2A  move.w  #$f1,$274(a5) */
    /* 006F30  bra  $6f3e */
    { sub_006F3E(); return; }
}

/* 006F32  game routine */
void sub_006F32(void)
{
    wr16(V_0274, 0xF4);                                  /* 006F32  move.w  #$f4,$274(a5) */
    /* 006F38  bra  $6f3e */
    { sub_006F3E(); return; }
}

/* 006F3A  game routine */
void sub_006F3A(void)
{
    /* 006F3A  clr.w  $274(a5) */
    wr16(V_0274, 0);
    { sub_006F3E(); return; }
}

/* 006F3E  game routine */
void sub_006F3E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d1, 0);                                         /* 006F3E  clr.w  d1 */
    SETW(d2, (d1 & 0xFFFF));                             /* 006F40  move.w  d1,d2 */
    /* 006F42  move.b  (a2)+,d1 */
    SETB(d1, rd8(a2));
    a2 += 1;
    SETW(d1, d1 + 0x40);                                 /* 006F44  addi.w  #$40,d1 */
    d1 &= ~0x1;                                          /* 006F48  bclr  #$0,d1 */
    d1 &= ~0x2;                                          /* 006F4C  bclr  #$1,d1 */
    d1 &= ~0x4;                                          /* 006F50  bclr  #$2,d1 */
    d1 &= ~0x8;                                          /* 006F54  bclr  #$3,d1 */
    /* 006F58  move.b  (a2)+,d2 */
    SETB(d2, rd8(a2));
    a2 += 1;
    a6 = V_18E6;                                         /* 006F5A  lea  $18e6(a5),a6 */
    wr16(V_0276, 0x14);                                  /* 006F5E  move.w  #$14,$276(a5) */
    do {
        BUDGET_CHECK(0x6F64U);
L_006F64:
        /* 006F64  tst.w  $4(a6) */
        /* 006F68  bne  $6fa2 */
        if (rd16(a6 + 0x4) == 0) {
            wr16(a6, (d1 & 0xFFFF));                             /* 006F6A  move.w  d1,(a6) */
            SETW(d2, d2 - 0x40);                                 /* 006F6C  subi.w  #$40,d2 */
            wr16(a6 + 0x2, (d2 & 0xFFFF));                       /* 006F70  move.w  d2,$2(a6) */
            wr8(a6 + 0x4, 0xFF);                                 /* 006F74  st  $4(a6) */
            wr16(a6 + 0x8, 0);                                   /* 006F78  clr.w  $8(a6) */
            wr16(a6 + 0xC, 0);                                   /* 006F7C  clr.w  $c(a6) */
            wr16(a6 + 0x6, 5);                                   /* 006F80  move.w  #$5,$6(a6) */
            wr16(a6 + 0xA, rd16(V_0274));                        /* 006F86  move.w  $274(a5),$a(a6) */
            PUSH32(d0);                                          /* 006F8C  movem.l  d0,-(a7) */
            CALL(sub_007576, 0x6F94U);                           /* 006F90  bsr  $7576 */
L_006F94:
            SETW(d0, d0 & 1);                                    /* 006F94  andi.w  #$1,d0 */
            wr16(a6 + 0xE, (d0 & 0xFFFF));                       /* 006F98  move.w  d0,$e(a6) */
            d0 = POP32();                                        /* 006F9C  movem.l  (a7)+,d0 */
            RTS();                                               /* 006FA0  rts */
        }
        a6 += 0x14;                                          /* 006FA2  adda.l  #$14,a6 */
        /* 006FA8  subq.w  #1,$276(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0276);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0276, r);
            fz = (r == 0);
        }
        /* 006FAC  bne  $6f64 */
    } while (COND(6));
    RTS();                                               /* 006FAE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6F64U: goto L_006F64;
        case 0x6F94U: goto L_006F94;
        }
    }
}

/* 006FB0  game routine */
void sub_006FB0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_18E6;                                         /* 006FB0  lea  $18e6(a5),a6 */
    wr16(V_0276, 0x14);                                  /* 006FB4  move.w  #$14,$276(a5) */
    do {
        BUDGET_CHECK(0x6FBAU);
L_006FBA:
        /* 006FBA  tst.w  $4(a6) */
        {
            uint32_t v = rd16(a6 + 0x4);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
        /* 006FBE  beq  $7074 */
        if (COND(6)) {
            /* 006FC2  bpl  $6fd0 */
            if (COND(11)) {
                /* 006FC4  tst.w  $2(a6) */
                /* 006FC8  bne  $6fe0 */
                if (rd16(a6 + 0x2) != 0) {
                    goto L_006FE0;
                }
                wr16(a6 + 0x4, 1);                                   /* 006FCA  move.w  #$1,$4(a6) */
            }
            /* 006FD0  cmpi.w  #$c7,$2(a6) */
            /* 006FD6  bls  $6fe0 */
            if (rd16(a6 + 0x2) > 0xC7) {
                wr16(a6 + 0x4, 0);                                   /* 006FD8  clr.w  $4(a6) */
                /* 006FDC  bra  $7074 */
                goto L_007074;
            }
L_006FE0:
            /* 006FE0  tst.w  $c(a6) */
            /* 006FE4  bne  $7074 */
            if (rd16(a6 + 0xC) == 0) {
                /* 006FE8  subq.w  #1,$6(a6) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(a6 + 0x6);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(a6 + 0x6, r);
                    fz = (r == 0);
                }
                /* 006FEC  bne  $7074 */
                if (COND(7)) {
                    SETW(d0, rd16(a6 + 0xA));                            /* 006FF0  move.w  $a(a6),d0 */
                    /* 006FF4  cmp.w  #$f4,d0 */
                    /* 006FF8  bne  $701e */
                    if ((d0 & 0xFFFF) == 0xF4) {
                        wr16(a6 + 0x6, 0x1E);                                /* 006FFA  move.w  #$1e,$6(a6) */
                        /* 007000  move.l  a6,-(a7) */
                        {
                            uint32_t v = a6;
                            a7 -= 4;
                            wr32(a7, v);
                        }
                        SETW(d0, rd16(a6));                                  /* 007002  move.w  (a6),d0 */
                        SETW(d0, d0 + 0xC);                                  /* 007004  addi.w  #$c,d0 */
                        SETW(d1, rd16(a6 + 0x2));                            /* 007008  move.w  $2(a6),d1 */
                        /* 00700C  addi.w  #$c,d1 */
                        {
                            uint32_t s = 0xC;
                            uint32_t d = (d1 & 0xFFFF);
                            uint32_t r = (d + s) & 0xFFFF;
                            SETW(d1, r);
                            fx = (r < s);
                        }
                        SETW(d2, rd16(a6 + 0xA));                            /* 007010  move.w  $a(a6),d2 */
                        CALL(sub_006BEA, 0x7018U);                           /* 007014  bsr  $6bea */
L_007018:
                        /* 007018  movea.l  (a7)+,a6 */
                        a6 = rd32(a7);
                        a7 += 4;
                        /* 00701A  bra  $7074 */
                    } else {
                        wr16(a6 + 0x6, 0x19);                                /* 00701E  move.w  #$19,$6(a6) */
                        SETW(d2, 5);                                         /* 007024  move.w  #$5,d2 */
                        SETW(d3, 5);                                         /* 007028  move.w  #$5,d3 */
                        /* 00702C  cmp.w  #$f2,d0 */
                        /* 007030  beq  $7062 */
                        if ((d0 & 0xFFFF) != 0xF2) {
                            /* 007032  neg.w  d2 */
                            { uint32_t d, r, ea; (void)ea;
                            d = (d2 & 0xFFFFU);
                            r = (0 - d) & 0xFFFFU;
                            d2 = (d2 & 0xFFFF0000U) | ((r) & 0xFFFFU);
                            FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
                            /* 007034  cmp.w  #$f1,d0 */
                            /* 007038  beq  $7062 */
                            if ((d0 & 0xFFFF) != 0xF1) {
                                wr16(a6 + 0x6, 5);                                   /* 00703A  move.w  #$5,$6(a6) */
                                wr16(a6 + 0x8, rd16(a6 + 0x8) + 1);                  /* 007040  addq.w  #1,$8(a6) */
                                /* 007044  cmpi.w  #$13,$8(a6) */
                                /* 00704A  bls  $7050 */
                                if (rd16(a6 + 0x8) > 0x13) {
                                    wr16(a6 + 0x8, 0);                                   /* 00704C  clr.w  $8(a6) */
                                }
                                SETW(d0, rd16(a6 + 0x8));                            /* 007050  move.w  $8(a6),d0 */
                                a0 = 0x7084;                                         /* 007054  lea  $7084(pc),a0 */
                                SETW(d0, (d0 & 0xFFFF) << 2);                        /* 007058  lsl.w  #2,d0 */
                                SETW(d2, rd16(a0 + SX16(d0)));                       /* 00705A  move.w  $0(a0,d0.w),d2 */
                                SETW(d3, rd16(a0 + 0x2 + SX16(d0)));                 /* 00705E  move.w  $2(a0,d0.w),d3 */
                            }
                        }
                        /* 007062  move.l  a6,-(a7) */
                        {
                            uint32_t v = a6;
                            a7 -= 4;
                            wr32(a7, v);
                        }
                        SETW(d0, rd16(a6));                                  /* 007064  move.w  (a6),d0 */
                        SETW(d0, d0 + 0xC);                                  /* 007066  addi.w  #$c,d0 */
                        SETW(d1, rd16(a6 + 0x2));                            /* 00706A  move.w  $2(a6),d1 */
                        SETW(d1, d1 + 0xC);                                  /* 00706E  addi.w  #$c,d1 */
                        /* 007072  movea.l  (a7)+,a6 */
                        a6 = rd32(a7);
                        a7 += 4;
                    }
                }
            }
        }
L_007074:
        a6 += 0x14;                                          /* 007074  adda.l  #$14,a6 */
        /* 00707A  subq.w  #1,$276(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0276);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0276, r);
            fz = (r == 0);
            fx = (s > d);
        }
        /* 00707E  bne  $6fba */
    } while (COND(6));
L_007082:
    RTS();                                               /* 007082  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x6FBAU: goto L_006FBA;
        case 0x7018U: goto L_007018;
        case 0x7074U: goto L_007074;
        case 0x7082U: goto L_007082;
        }
    }
}

