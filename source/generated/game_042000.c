/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 042060  game routine */
void sub_042060(void)
{
    /* 042060  move.b  (a1)+,$fd0(a3) */
    wr8(a3 + 0xFD0, rd8(a1));
    a1 += 1;
    wr8(a3 + 0xFD1, rd8(0x424FA));                       /* 042064  move.b  $424fa(pc),$fd1(a3) */
    /* 04206A  bra  $42190 */
    { sub_042190(); return; }
}

/* 04206E  game routine */
void sub_04206E(void)
{
    /* 04206E  move.b  (a1)+,$813(a3) */
    {
        uint32_t v = rd8(a1);
        a1 += 1;
        wr8(a3 + 0x813, v);
    }
    wr8(a3 + 0x812, rd8(0x41D3D));                       /* 042072  move.b  $41d3d(pc),$812(a3) */
    /* 042078  bra  $42190 */
    { sub_042190(); return; }
}

/* 04207C  game routine */
void sub_04207C(void)
{
    /* 04207C  move.b  (a1)+,$6(a0) */
    wr8(a0 + 0x6, rd8(a1));
    a1 += 1;
    /* 042080  move.b  (a1)+,$7(a0) */
    wr8(a0 + 0x7, rd8(a1));
    a1 += 1;
    wr16(a0 + 0xA, 0);                                   /* 042084  move.w  #$0,$a(a0) */
    /* 04208A  bra  $42190 */
    { sub_042190(); return; }
}

/* 04208E  game routine */
void sub_04208E(void)
{
    /* 04208E  addq.l  #4,a7 */
    a7 += 4;
    { sub_042090(); return; }
}

/* 042090  game routine */
void sub_042090(void)
{
    wr8(a3 + 0xFCE, 0);                                  /* 042090  sf  $fce(a3) */
    /* 042094  bra  $41be8 */
    { sub_041BE8(); return; }
}

/* 042098  game routine */
void sub_042098(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, rd16(a0 + 0xA));                            /* 042098  move.w  $a(a0),d0 */
    a2 = SX16(rd16(a0 + 0x6));                           /* 04209C  movea.w  $6(a0),a2 */
    a2 += SX16((d0 & 0xFFFF));                           /* 0420A0  adda.w  d0,a2 */
    SETW(d0, d0 + 2);                                    /* 0420A2  addq.w  #2,d0 */
    /* 0420A4  tst.w  $0(a3,a2.w) */
    /* 0420A8  bne  $420b0 */
    if (rd16(a3 + SX16(a2)) == 0) {
L_0420AA:
        a2 = SX16(rd16(a0 + 0x6));                           /* 0420AA  movea.w  $6(a0),a2 */
        d0 = 2;                                              /* 0420AE  moveq  #2,d0 */
    }
L_0420B0:
    a1 = SX16(rd16(a3 + SX16(a2)));                      /* 0420B0  movea.w  $0(a3,a2.w),a1 */
    a1 += a3;                                            /* 0420B4  adda.l  a3,a1 */
    wr16(a0 + 0xA, (d0 & 0xFFFF));                       /* 0420B6  move.w  d0,$a(a0) */
    /* 0420BA  bra  $42190 */
    { sub_042190(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x420AAU: goto L_0420AA;
        case 0x420B0U: goto L_0420B0;
        }
    }
}

/* 0420BE  game routine */
void sub_0420BE(void)
{
    SETB(d0, rd8(a0 + 0x2F));                            /* 0420BE  move.b  $2f(a0),d0 */
    SETB(d1, (d0 & 0xFF));                               /* 0420C2  move.b  d0,d1 */
    SETB(d0, d0 & 7);                                    /* 0420C4  andi.b  #$7,d0 */
    SETB(d2, rd8(0x41D41));                              /* 0420C8  move.b  $41d41(pc),d2 */
    SETB(d0, d0 ^ (d2 & 0xFF));                          /* 0420CC  eor.b  d2,d0 */
    SETB(d0, d0 & (d1 & 0xFF));                          /* 0420CE  and.b  d1,d0 */
    SETB(d0, d0 ^ (d2 & 0xFF));                          /* 0420D0  eor.b  d2,d0 */
    wr8(a3 + 0x817, (d0 & 0xFF));                        /* 0420D2  move.b  d0,$817(a3) */
    wr8(a0 + 0x1, 0xFF);                                 /* 0420D6  st  $1(a0) */
    /* 0420DA  bra  $42190 */
    { sub_042190(); return; }
}

/* 0420DE  game routine */
void sub_0420DE(void)
{
    SETB(d0, rd8(a0 + 0x2F));                            /* 0420DE  move.b  $2f(a0),d0 */
    SETB(d1, (d0 & 0xFF));                               /* 0420E2  move.b  d0,d1 */
    SETB(d0, d0 & 0x38);                                 /* 0420E4  andi.b  #$38,d0 */
    SETB(d2, rd8(0x41D41));                              /* 0420E8  move.b  $41d41(pc),d2 */
    SETB(d0, d0 ^ (d2 & 0xFF));                          /* 0420EC  eor.b  d2,d0 */
    SETB(d0, d0 & (d1 & 0xFF));                          /* 0420EE  and.b  d1,d0 */
    SETB(d0, d0 ^ (d2 & 0xFF));                          /* 0420F0  eor.b  d2,d0 */
    wr8(a3 + 0x817, (d0 & 0xFF));                        /* 0420F2  move.b  d0,$817(a3) */
    wr8(a0 + 0x1, 0);                                    /* 0420F6  sf  $1(a0) */
    /* 0420FA  bra  $42190 */
    { sub_042190(); return; }
}

/* 0420FE  game routine */
void sub_0420FE(void)
{
    SETB(d0, rd8(a0 + 0x2F));                            /* 0420FE  move.b  $2f(a0),d0 */
    SETB(d0, d0 ^ 0xFF);                                 /* 042102  eori.b  #$ff,d0 */
    SETB(d2, rd8(0x41D41));                              /* 042106  move.b  $41d41(pc),d2 */
    SETB(d0, d0 & (d2 & 0xFF));                          /* 04210A  and.b  d2,d0 */
    wr8(a3 + 0x817, rd8(a3 + 0x817) & (d0 & 0xFF));      /* 04210C  and.b  d0,$817(a3) */
    wr8(a0 + 0x1, 0xFF);                                 /* 042110  st  $1(a0) */
    /* 042114  bra  $42190 */
    { sub_042190(); return; }
}

/* 042116  game routine */
void sub_042116(void)
{
    wr16(a0 + 0xE, 0);                                   /* 042116  clr.w  $e(a0) */
    wr8(a0, rd8(a0) | 0x4);                              /* 04211A  bset  #$2,$0(a0) */
    /* 042120  move.b  (a1)+,$18(a0) */
    {
        uint32_t v = rd8(a1);
        a1 += 1;
        wr8(a0 + 0x18, v);
    }
    /* 042124  move.b  (a1)+,$19(a0) */
    {
        uint32_t v = rd8(a1);
        a1 += 1;
        wr8(a0 + 0x19, v);
    }
    /* 042128  bra  $42190 */
    { sub_042190(); return; }
}

/* 04212A  game routine */
void sub_04212A(void)
{
    /* 04212A  move.b  (a1)+,$fcf(a3) */
    wr8(a3 + 0xFCF, rd8(a1));
    a1 += 1;
    /* 04212E  bra  $42190 */
    { sub_042190(); return; }
}

/* 042130  game routine */
void sub_042130(void)
{
    /* 042130  move.b  (a1)+,$2e(a0) */
    wr8(a0 + 0x2E, rd8(a1));
    a1 += 1;
    /* 042134  bra  $42190 */
    { sub_042190(); return; }
}

/* 042136  game routine */
void sub_042136(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr8(a0, rd8(a0) | 0x80);                             /* 042136  bset  #$7,$0(a0) */
L_04213C:
    wr8(a0, rd8(a0) | 0x8);                              /* 04213C  bset  #$3,$0(a0) */
    /* 042142  bra  $42190 */
    { sub_042190(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4213CU: goto L_04213C;
        }
    }
}

/* 042144  game routine */
void sub_042144(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 042144  move.b  (a1)+,$2a(a0) */
    wr8(a0 + 0x2A, rd8(a1));
    a1 += 1;
    wr8(a0 + 0x29, rd8(a1));                             /* 042148  move.b  (a1),$29(a0) */
    /* 04214C  move.b  (a1)+,$2b(a0) */
    wr8(a0 + 0x2B, rd8(a1));
    a1 += 1;
L_042150:
    wr8(a0 + 0x2C, 0x40);                                /* 042150  move.b  #$40,$2c(a0) */
    /* 042156  bra  $42190 */
    { sub_042190(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x42150U: goto L_042150;
        }
    }
}

/* 042158  game routine */
void sub_042158(void)
{
    wr8(a0 + 0x2C, 0);                                   /* 042158  clr.b  $2c(a0) */
    /* 04215C  bra  $42190 */
    { sub_042190(); return; }
}

/* 04215E  game routine */
void sub_04215E(void)
{
    wr8(a0, rd8(a0) | 0x2);                              /* 04215E  bset  #$1,$0(a0) */
    /* 042164  bra  $42190 */
    { sub_042190(); return; }
}

/* 042166  game routine */
void sub_042166(void)
{
    wr8(a0 + 0x1E, 0);                                   /* 042166  clr.b  $1e(a0) */
    /* 04216A  bra  $421c2 */
    { sub_0421C2(); return; }
}

/* 04216C  game routine */
void sub_04216C(void)
{
    wr8(a0, rd8(a0) | 0x20);                             /* 04216C  bset  #$5,$0(a0) */
    /* 042172  bra  $421c2 */
    { sub_0421C2(); return; }
}

/* 042174  game routine */
void sub_042174(void)
{
    wr8(a0 + 0x2D, 0xFF);                                /* 042174  st  $2d(a0) */
    /* 042178  bra  $42190 */
    { sub_042190(); return; }
}

/* 04217A  game routine */
void sub_04217A(void)
{
    wr8(a0 + 0x2D, 0);                                   /* 04217A  sf  $2d(a0) */
    /* 04217E  bra  $42190 */
    { sub_042190(); return; }
}

/* 042180  game routine */
void sub_042180(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 042180  subq.b  #1,$1b(a0) */
    {
        uint32_t s = 1;
        uint32_t d = rd8(a0 + 0x1B);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0 + 0x1B, r);
        fz = (r == 0);
    }
    /* 042184  bne  $42202 */
    if (COND(7)) {
L_042188:
        wr8(a0, 0);                                          /* 042188  clr.b  $0(a0) */
        /* 04218C  movea.l  $2(a0),a1 */
        a1 = rd32(a0 + 0x2);
        { sub_042190(); return; }
    }
L_042202:
    fz = !(rd8(a0) & 0x8);                               /* 042202  btst  #$3,$0(a0) */
    /* 042208  bne  $4220c */
    if (COND(7)) {
L_04220A:
        RTS();                                               /* 04220A  rts */
    }
    fz = !(rd8(a0) & 0x80);                              /* 04220C  btst  #$7,$0(a0) */
    /* 042212  beq  $4221a */
    if (COND(6)) {
        wr8(a0 + 0x1D, rd8(a0 + 0x1D) + 1);                  /* 042214  addq.b  #1,$1d(a0) */
        RTS();                                               /* 042218  rts */
    }
    wr8(a0 + 0x1D, rd8(a0 + 0x1D) - 1);                  /* 04221A  subq.b  #1,$1d(a0) */
    RTS();                                               /* 04221E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x42188U: goto L_042188;
        case 0x42202U: goto L_042202;
        case 0x4220AU: goto L_04220A;
        }
    }
}

/* 042190  game routine */
void sub_042190(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
L_042190:
    d0 = 0;                                              /* 042190  moveq  #0,d0 */
    /* 042192  move.b  (a1)+,d0 */
    {
        uint32_t v = rd8(a1);
        a1 += 1;
        SETB(d0, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 042194  bmi  $42220 */
    if (COND(10)) {
L_042198:
        wr8(a0 + 0x1D, (d0 & 0xFF));                         /* 042198  move.b  d0,$1d(a0) */
        /* 04219C  tst.b  $1(a0) */
        /* 0421A0  beq  $421a6 */
        if (rd8(a0 + 0x1) != 0) {
L_0421A2:
            wr8(a3 + 0x815, (d0 & 0xFF));                        /* 0421A2  move.b  d0,$815(a3) */
        }
L_0421A6:
        a2 = rd32(a0 + 0x24);                                /* 0421A6  movea.l  $24(a0),a2 */
        wr32(a0 + 0x20, a2);                                 /* 0421AA  move.l  a2,$20(a0) */
        wr8(a0 + 0x28, rd8(a2));                             /* 0421AE  move.b  (a2),$28(a0) */
        wr8(a0 + 0x1E, rd8(a2));                             /* 0421B2  move.b  (a2),$1e(a0) */
        wr8(a0 + 0x1F, rd8(a0 + 0x1A));                      /* 0421B6  move.b  $1a(a0),$1f(a0) */
        /* 0421BC  bset  #$5,$0(a0) */
        wr8(a0, rd8(a0) | 0x20);
        { sub_0421C2(); return; }
    }
L_042220:
    /* 042220  cmp.b  #$b8,d0 */
    /* 042224  bcs  $42270 */
    if ((d0 & 0xFF) >= 0xB8) {
L_042226:
        /* 042226  addi.b  #$20,d0 */
        {
            uint32_t s = 0x20;
            uint32_t d = (d0 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d0, r);
            fc = (r < s);
        }
        /* 04222A  bcs  $4224e */
        if (COND(4)) {
L_04222C:
            /* 04222C  addi.b  #$10,d0 */
            {
                uint32_t s = 0x10;
                uint32_t d = (d0 & 0xFF);
                uint32_t r = (d + s) & 0xFF;
                SETB(d0, r);
                fc = (r < s);
            }
            /* 042230  bcs  $42258 */
            if (COND(5)) {
                goto L_042258;
            }
L_042232:
            SETB(d0, d0 + 0x10);                                 /* 042232  addi.b  #$10,d0 */
            SETW(d0, d0 + (d0 & 0xFFFF));                        /* 042236  add.w  d0,d0 */
            a2 = 0x424A8;                                        /* 042238  lea  $424a8(pc),a2 */
            a2 = SX16(rd16(a2 + SX16(d0)));                      /* 04223C  movea.w  $0(a2,d0.w),a2 */
            a2 += a3;                                            /* 042240  adda.l  a3,a2 */
            wr32(a0 + 0x14, a2);                                 /* 042242  move.l  a2,$14(a0) */
            wr32(a0 + 0x10, a2);                                 /* 042246  move.l  a2,$10(a0) */
            /* 04224A  bra  $42190 */
            BUDGET_CHECK(0x42190U);
            goto L_042190;
        }
L_04224E:
        SETB(d0, d0 + 1);                                    /* 04224E  addq.b  #1,d0 */
        wr8(a0 + 0x1C, (d0 & 0xFF));                         /* 042250  move.b  d0,$1c(a0) */
        /* 042254  bra  $42190 */
        BUDGET_CHECK(0x42190U);
        goto L_042190;
L_042258:
        SETW(d0, d0 + (d0 & 0xFFFF));                        /* 042258  add.w  d0,d0 */
        a2 = 0x43382;                                        /* 04225A  lea  $43382(pc),a2 */
        a2 = SX16(rd16(a2 + SX16(d0)));                      /* 04225E  movea.w  $0(a2,d0.w),a2 */
        a2 += a3;                                            /* 042262  adda.l  a3,a2 */
        wr32(a0 + 0x24, a2);                                 /* 042264  move.l  a2,$24(a0) */
        /* 042268  move.b  -(a2),$1a(a0) */
        a2 -= 1;
        wr8(a0 + 0x1A, rd8(a2));
        /* 04226C  bra  $42190 */
        BUDGET_CHECK(0x42190U);
        goto L_042190;
    }
L_042270:
    SETW(d0, d0 & 0x7F);                                 /* 042270  andi.w  #$7f,d0 */
    /* 042274  add.w  d0,d0 */
    {
        uint32_t s = (d0 & 0xFFFF);
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    a2 = 0x42034;                                        /* 042276  lea  $42034(pc),a2 */
    a2 = SX16(rd16(a2 + SX16(d0)));                      /* 04227A  movea.w  $0(a2,d0.w),a2 */
    /* 04227E  jmp  $0(a3,a2.w) */
    { uint32_t ea;
    ea = a3 + 0x0U + SX16(a2);
    JUMP_DYN(ea);
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x42198U: goto L_042198;
        case 0x421A2U: goto L_0421A2;
        case 0x421A6U: goto L_0421A6;
        case 0x42220U: goto L_042220;
        case 0x42226U: goto L_042226;
        case 0x4222CU: goto L_04222C;
        case 0x42232U: goto L_042232;
        case 0x4224EU: goto L_04224E;
        case 0x42258U: goto L_042258;
        case 0x42270U: goto L_042270;
        }
    }
}

/* 0421C2  game routine */
void sub_0421C2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr8(a0 + 0x1B, rd8(a0 + 0x1C));                      /* 0421C2  move.b  $1c(a0),$1b(a0) */
    wr32(a0 + 0x2, a1);                                  /* 0421C8  move.l  a1,$2(a0) */
    /* 0421CC  tst.b  $2d(a0) */
    /* 0421D0  beq  $42200 */
    if (rd8(a0 + 0x2D) != 0) {
L_0421D2:
        /* 0421D2  tst.b  $fca(a3) */
        /* 0421D6  beq  $421e0 */
        if (rd8(a3 + 0xFCA) != 0) {
L_0421D8:
            fz = !(rd8(a3 + 0x1DD6) & 0x8);                      /* 0421D8  btst  #$3,$1dd6(a3) */
            /* 0421DE  beq  $42200 */
            if (COND(7)) {
                goto L_042200;
            }
        }
L_0421E0:
        /* 0421E0  tst.b  $fcb(a3) */
        /* 0421E4  beq  $421ee */
        if (rd8(a3 + 0xFCB) != 0) {
            fz = !(rd8(a3 + 0x1DF0) & 0x8);                      /* 0421E6  btst  #$3,$1df0(a3) */
            /* 0421EC  beq  $42200 */
            if (COND(7)) {
                goto L_042200;
            }
        }
L_0421EE:
        /* 0421EE  tst.b  $fcc(a3) */
        /* 0421F2  beq  $421fc */
        if (rd8(a3 + 0xFCC) != 0) {
            fz = !(rd8(a3 + 0x1E0A) & 0x8);                      /* 0421F4  btst  #$3,$1e0a(a3) */
            /* 0421FA  beq  $42200 */
            if (COND(7)) {
                goto L_042200;
            }
        }
L_0421FC:
        wr8(a0 + 0x2D, 0xFF);                                /* 0421FC  st  $2d(a0) */
    }
L_042200:
    RTS();                                               /* 042200  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x421D2U: goto L_0421D2;
        case 0x421D8U: goto L_0421D8;
        case 0x421E0U: goto L_0421E0;
        case 0x421EEU: goto L_0421EE;
        case 0x421FCU: goto L_0421FC;
        case 0x42200U: goto L_042200;
        }
    }
}

/* 042282  game routine */
void sub_042282(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d7 = 0;                                              /* 042282  moveq  #0,d7 */
    SETB(d7, rd8(a0));                                   /* 042284  move.b  $0(a0),d7 */
    fz = !(d7 & 0x20);                                   /* 042288  btst  #$5,d7 */
    /* 04228C  beq  $422b2 */
    if (COND(6)) {
L_04228E:
        /* 04228E  subq.b  #1,$1f(a0) */
        {
            uint32_t s = 1;
            uint32_t d = rd8(a0 + 0x1F);
            uint32_t r = (d - s) & 0xFF;
            wr8(a0 + 0x1F, r);
            fc = (s > d);
        }
        /* 042292  bcc  $422ac */
        if (COND(5)) {
L_042294:
            wr8(a0 + 0x1F, rd8(a0 + 0x1A));                      /* 042294  move.b  $1a(a0),$1f(a0) */
            a2 = rd32(a0 + 0x20);                                /* 04229A  movea.l  $20(a0),a2 */
            /* 04229E  move.b  $1(a2),d0 */
            /* 0422A2  bmi  $422ac */
            SETB(d0, rd8(a2 + 0x1));
            if ((int8_t)(d0 & 0xFF) >= 0) {
L_0422A4:
                wr32(a0 + 0x20, rd32(a0 + 0x20) + 1);                /* 0422A4  addq.l  #1,$20(a0) */
                wr8(a0 + 0x28, (d0 & 0xFF));                         /* 0422A8  move.b  d0,$28(a0) */
            }
        }
L_0422AC:
        wr8(a0 + 0x1E, rd8(a0 + 0x28));                      /* 0422AC  move.b  $28(a0),$1e(a0) */
    }
    SETB(d0, rd8(a0 + 0x1D));                            /* 0422B2  move.b  $1d(a0),d0 */
    SETB(d0, d0 + rd8(0x424F9));                         /* 0422B6  add.b  $424f9(pc),d0 */
    SETB(d0, d0 + rd8(a0 + 0x2E));                       /* 0422BA  add.b  $2e(a0),d0 */
    a1 = rd32(a0 + 0x14);                                /* 0422BE  movea.l  $14(a0),a1 */
    /* 0422C2  move.b  (a1)+,d1 */
    SETB(d1, rd8(a1));
    a1 += 1;
    /* 0422C4  bclr  #$7,d1 */
    fz = !(d1 & 0x80);
    d1 &= ~0x80;
    /* 0422C8  beq  $422ce */
    if (COND(6)) {
L_0422CA:
        a1 = rd32(a0 + 0x10);                                /* 0422CA  movea.l  $10(a0),a1 */
    }
L_0422CE:
    wr32(a0 + 0x14, a1);                                 /* 0422CE  move.l  a1,$14(a0) */
    SETB(d0, d0 + (d1 & 0xFF));                          /* 0422D2  add.b  d1,d0 */
    a1 = 0x423E8;                                        /* 0422D4  lea  $423e8(pc),a1 */
    SETB(d0, d0 + (d0 & 0xFF));                          /* 0422D8  add.b  d0,d0 */
    d1 = 0;                                              /* 0422DA  moveq  #0,d1 */
    SETB(d1, (d0 & 0xFF));                               /* 0422DC  move.b  d0,d1 */
    a1 += SX16((d1 & 0xFFFF));                           /* 0422DE  adda.w  d1,a1 */
    SETW(d0, rd16(a1));                                  /* 0422E0  move.w  (a1),d0 */
    SETB(d6, rd8(a0 + 0x2C));                            /* 0422E2  move.b  $2c(a0),d6 */
    fz = !(d6 & 0x40);                                   /* 0422E6  btst  #$6,d6 */
    /* 0422EA  beq  $42348 */
    if (COND(6)) {
L_0422EC:
        SETB(d5, (d1 & 0xFF));                               /* 0422EC  move.b  d1,d5 */
        SETB(d4, rd8(a0 + 0x29));                            /* 0422EE  move.b  $29(a0),d4 */
        SETB(d4, (d4 & 0xFF) << 1);                          /* 0422F2  lsl.b  #1,d4 */
        SETB(d1, rd8(a0 + 0x2B));                            /* 0422F4  move.b  $2b(a0),d1 */
        fz = !(d6 & 0x80);                                   /* 0422F8  btst  #$7,d6 */
        /* 0422FC  beq  $42304 */
        if (COND(6)) {
            fz = !(d7 & 0x1);                                    /* 0422FE  btst  #$0,d7 */
            /* 042302  bne  $4232e */
            if (COND(6)) {
                goto L_04232E;
            }
        }
L_042304:
        fz = !(d6 & 0x20);                                   /* 042304  btst  #$5,d6 */
        /* 042308  bne  $4231a */
        if (COND(7)) {
L_04230A:
            /* 04230A  sub.b  $2a(a0),d1 */
            {
                uint32_t s = rd8(a0 + 0x2A);
                uint32_t d = (d1 & 0xFF);
                uint32_t r = (d - s) & 0xFF;
                SETB(d1, r);
                fc = (s > d);
            }
            /* 04230E  bcc  $4232a */
            if (COND(4)) {
                goto L_04232A;
            }
L_042310:
            wr8(a0 + 0x2C, rd8(a0 + 0x2C) | 0x20);               /* 042310  bset  #$5,$2c(a0) */
            d1 = 0;                                              /* 042316  moveq  #0,d1 */
            /* 042318  bra  $4232a */
        } else {
L_04231A:
            SETB(d1, d1 + rd8(a0 + 0x2A));                       /* 04231A  add.b  $2a(a0),d1 */
            /* 04231E  cmp.b  d4,d1 */
            /* 042320  bcs  $4232a */
            if ((d1 & 0xFF) >= (d4 & 0xFF)) {
L_042322:
                wr8(a0 + 0x2C, rd8(a0 + 0x2C) & ~0x20);              /* 042322  bclr  #$5,$2c(a0) */
                SETB(d1, (d4 & 0xFF));                               /* 042328  move.b  d4,d1 */
            }
        }
L_04232A:
        wr8(a0 + 0x2B, (d1 & 0xFF));                         /* 04232A  move.b  d1,$2b(a0) */
L_04232E:
        SETB(d4, (d4 & 0xFF) >> 1);                          /* 04232E  lsr.b  #1,d4 */
        /* 042330  sub.b  d4,d1 */
        {
            uint32_t s = (d4 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d1, r);
            fc = (s > d);
        }
        /* 042332  bcc  $42338 */
        if (COND(5)) {
L_042334:
            SETW(d1, d1 - 0x100);                                /* 042334  subi.w  #$100,d1 */
        }
L_042338:
        /* 042338  addi.b  #$a0,d5 */
        {
            uint32_t s = 0xA0;
            uint32_t d = (d5 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d5, r);
            fc = (r < s);
        }
        /* 04233C  bcs  $42346 */
        if (COND(4)) {
            do {
                BUDGET_CHECK(0x4233EU);
L_04233E:
                SETW(d1, (d1 & 0xFFFF) << 1);                        /* 04233E  lsl.w  #1,d1 */
                /* 042340  addi.b  #$18,d5 */
                {
                    uint32_t s = 0x18;
                    uint32_t d = (d5 & 0xFF);
                    uint32_t r = (d + s) & 0xFF;
                    SETB(d5, r);
                    fc = (r < s);
                }
                /* 042344  bcc  $4233e */
            } while (COND(4));
        }
L_042346:
        SETW(d0, d0 + (d1 & 0xFFFF));                        /* 042346  add.w  d1,d0 */
    }
L_042348:
    SETB(d7, d7 ^ 1);                                    /* 042348  eori.b  #$1,d7 */
    wr8(a0, (d7 & 0xFF));                                /* 04234C  move.b  d7,$0(a0) */
    fz = !(d7 & 0x4);                                    /* 042350  btst  #$2,d7 */
    /* 042354  beq  $4237a */
    if (COND(6)) {
L_042356:
        SETB(d4, rd8(a0 + 0x19));                            /* 042356  move.b  $19(a0),d4 */
        /* 04235A  subq.b  #1,d4 */
        {
            uint32_t s = 1;
            uint32_t d = (d4 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d4, r);
            fz = (r == 0);
        }
        /* 04235C  bne  $42376 */
        if (COND(7)) {
L_04235E:
            SETW(d6, 0);                                         /* 04235E  clr.w  d6 */
            /* 042360  move.b  $18(a0),d6 */
            /* 042364  bpl  $4236a */
            SETB(d6, rd8(a0 + 0x18));
            if ((int8_t)(d6 & 0xFF) < 0) {
L_042366:
                SETW(d6, d6 + 0xFF00);                               /* 042366  addi.w  #$ff00,d6 */
            }
L_04236A:
            SETW(d6, d6 + rd16(a0 + 0xE));                       /* 04236A  add.w  $e(a0),d6 */
            wr16(a0 + 0xE, (d6 & 0xFFFF));                       /* 04236E  move.w  d6,$e(a0) */
            SETW(d0, d0 + (d6 & 0xFFFF));                        /* 042372  add.w  d6,d0 */
            /* 042374  bra  $4237a */
        } else {
            wr8(a0 + 0x19, (d4 & 0xFF));                         /* 042376  move.b  d4,$19(a0) */
        }
    }
L_04237A:
    SETB(d7, d7 ^ 0xFF);                                 /* 04237A  eori.b  #$ff,d7 */
    SETB(d3, rd8(0x41D41));                              /* 04237E  move.b  $41d41(pc),d3 */
    /* 042382  andi.b  #$3,d7 */
    {
        uint32_t s = 3;
        uint32_t d = (d7 & 0xFF);
        uint32_t r = (d & s) & 0xFF;
        SETB(d7, r);
        fz = (r == 0);
    }
    /* 042386  bne  $42398 */
    if (COND(7)) {
L_042388:
        SETB(d3, rd8(0x41D3F));                              /* 042388  move.b  $41d3f(pc),d3 */
        SETB(d3, d3 ^ 8);                                    /* 04238C  eori.b  #$8,d3 */
        wr8(a3 + 0x816, (d3 & 0xFF));                        /* 042390  move.b  d3,$816(a3) */
        SETB(d3, 7);                                         /* 042394  move.b  #$7,d3 */
    }
L_042398:
    SETB(d2, rd8(0x423D3));                              /* 042398  move.b  $423d3(pc),d2 */
    SETB(d3, d3 ^ (d2 & 0xFF));                          /* 04239C  eor.b  d2,d3 */
    SETB(d3, d3 & rd8(a0 + 0x2F));                       /* 04239E  and.b  $2f(a0),d3 */
    SETB(d3, d3 ^ (d2 & 0xFF));                          /* 0423A2  eor.b  d2,d3 */
    /* 0423A4  move.b  $2d(a0),d1 */
    /* 0423A8  bpl  $423c2 */
    SETB(d1, rd8(a0 + 0x2D));
    if ((int8_t)(d1 & 0xFF) < 0) {
L_0423AA:
        wr8(a0 + 0x2D, rd8(a0 + 0x2D) & 0x7F);               /* 0423AA  andi.b  #$7f,$2d(a0) */
        SETB(d1, rd8(a0 + 0x2F));                            /* 0423B0  move.b  $2f(a0),d1 */
        SETB(d1, d1 & 0x38);                                 /* 0423B4  andi.b  #$38,d1 */
        SETB(d1, ~(d1 & 0xFF));                              /* 0423B8  not.b  d1 */
        SETB(d3, d3 & (d1 & 0xFF));                          /* 0423BA  and.b  d1,d3 */
        wr8(a3 + 0x816, 1);                                  /* 0423BC  move.b  #$1,$816(a3) */
    }
L_0423C2:
    wr8(a3 + 0xEA9, (d3 & 0xFF));                        /* 0423C2  move.b  d3,$ea9(a3) */
    SETB(d1, rd8(a0 + 0x1E));                            /* 0423C6  move.b  $1e(a0),d1 */
    RTS();                                               /* 0423CA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4228EU: goto L_04228E;
        case 0x42294U: goto L_042294;
        case 0x422A4U: goto L_0422A4;
        case 0x422ACU: goto L_0422AC;
        case 0x422CAU: goto L_0422CA;
        case 0x422CEU: goto L_0422CE;
        case 0x422ECU: goto L_0422EC;
        case 0x42304U: goto L_042304;
        case 0x4230AU: goto L_04230A;
        case 0x42310U: goto L_042310;
        case 0x4231AU: goto L_04231A;
        case 0x42322U: goto L_042322;
        case 0x4232AU: goto L_04232A;
        case 0x42334U: goto L_042334;
        case 0x42338U: goto L_042338;
        case 0x4233EU: goto L_04233E;
        case 0x42346U: goto L_042346;
        case 0x42348U: goto L_042348;
        case 0x42356U: goto L_042356;
        case 0x4235EU: goto L_04235E;
        case 0x42366U: goto L_042366;
        case 0x4236AU: goto L_04236A;
        case 0x4237AU: goto L_04237A;
        case 0x42388U: goto L_042388;
        case 0x42398U: goto L_042398;
        case 0x423AAU: goto L_0423AA;
        case 0x423C2U: goto L_0423C2;
        }
    }
}

/* 0424A8  game routine */
void sub_0424A8(void)
{
    /* 0424A8  bclr  d7,(a4)+ */
    {
        uint32_t ea = a4;
        a4 += 1;
        wr8(ea, rd8(ea) & ~(1u << (d7 & 7)));
    }
    /* 0424AA  bclr  d7,(a5)+ */
    {
        uint32_t ea = a5;
        a5 += 1;
        wr8(ea, rd8(ea) & ~(1u << (d7 & 7)));
    }
    /* 0424AC  bclr  d7,-(a0) */
    a0 -= 1;
    wr8(a0, rd8(a0) & ~(1u << (d7 & 7)));
    /* 0424AE  bclr  d7,-(a3) */
    a3 -= 1;
    wr8(a3, rd8(a3) & ~(1u << (d7 & 7)));
    /* 0424B0  bclr  d7,-(a6) */
    a6 -= 1;
    wr8(a6, rd8(a6) & ~(1u << (d7 & 7)));
    wr8(a1 + 0xFAC, rd8(a1 + 0xFAC) & ~(1u << (d7 & 7))); /* 0424B2  bclr  d7,$fac(a1) */
    /* 0424B6  bclr  d7,$fb2(a7) */
    fz = !(rd8(a7 + 0xFB2) & (1u << (d7 & 7)));
    wr8(a7 + 0xFB2, rd8(a7 + 0xFB2) & ~(1u << (d7 & 7)));
    MISS(0x424BAU);
}

/* 0424C6  game routine */
void sub_0424C6(void)
{
    SETB(d0, d0 | (d0 & 0xFF));                          /* 0424C6  or.b  d0,d0 */
    d7 &= ~(1u << (d1 & 31));                            /* 0424C8  bclr  d1,d7 */
    SETB(d4, d4 | 4);                                    /* 0424CA  ori.b  #$4,d4 */
    /* 0424CE  movep.w  d3,4(a2) */
    wr8(a2 + 4 + 0, d3 >> 8);
    wr8(a2 + 4 + 2, d3 >> 0);
    /* 0424D2  or.b  d7,d5 */
    {
        uint32_t s = (d7 & 0xFF);
        uint32_t d = (d5 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d5, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x424D4U);
}

/* 0424F8  game routine */
void sub_0424F8(void)
{
    SOFT(0x424F8U);                                      /* 0424F8  soft */
}

/* 0424FA  game routine */
void sub_0424FA(void)
{
    SETB(d0, d0 | 0);                                    /* 0424FA  ori.b  #$0,d0 */
    /* 0424FE  ori.b  #$95,d0 */
    {
        uint32_t s = 0x95;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        fn = !!(r & 0x80);
        fv = 0;
        fc = 0;
    }
    /* 042502  btst  d6,d0 */
    fz = !(d0 & (1u << (d6 & 31)));
    { sub_042504(); return; }
}

/* 042504  game routine */
void sub_042504(void)
{
    set_ccr((get_ccr() | 0x9AU) & 0x1FU);                /* 042504  ori  #$9a,ccr */
    /* 042508  move.b  (a2)+,d0 */
    {
        uint32_t v = rd8(a2);
        a2 += 1;
        SETB(d0, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    MISS(0x4250AU);
}

/* 042964  game routine */
void sub_042964(void)
{
    /* 042964  move.b  -(a5),(a2)+ */
    a5 -= 1;
    {
        uint32_t v = rd8(a5);
        wr8(a2, v);
        a2 += 1;
    }
    /* 042966  move.b  $43e65(pc),(a2)+ */
    {
        uint32_t v = rd8(0x43E65);
        wr8(a2, v);
        a2 += 1;
        FL_LOGIC(v, 0xFF, 0x80);
    }
    MISS(0x4296AU);
}

/* 042CA4  game routine */
void sub_042CA4(void)
{
    SETB(d3, shift_op(1, 1, (d3 & 0xFF), (d0 & 63), 8)); /* 042CA4  lsl.b  d0,d3 */
    /* 042CA6  roxl.b  d1,d0 */
    SETB(d0, shift_op(2, 1, (d0 & 0xFF), (d1 & 63), 8));
    MISS(0x42CA8U);
}

/* 042D32  game routine */
void sub_042D32(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a3 = 0x4152A;                                        /* 042D32  lea  $4152a(pc),a3 */
    /* 042D36  cmp.b  #$0,d1 */
    /* 042D3A  bne  $42da6 */
    if ((d1 & 0xFF) == 0) {
L_042D3C:
        wr8(a3 + 0xFCA, 0);                                  /* 042D3C  sf  $fca(a3) */
        SETW(d0, SX8(d0));                                   /* 042D40  ext.w  d0 */
        SETW(d0, d0 + (d0 & 0xFFFF));                        /* 042D42  add.w  d0,d0 */
        a0 = 0x430D8;                                        /* 042D44  lea  $430d8(pc),a0 */
        a0 = SX16(rd16(a0 + SX16(d0)));                      /* 042D48  movea.w  $0(a0,d0.w),a0 */
        a0 += a3;                                            /* 042D4C  adda.l  a3,a0 */
        d0 = 0xD;                                            /* 042D4E  moveq  #13,d0 */
        a1 = 0x432FA;                                        /* 042D50  lea  $432fa(pc),a1 */
        do {
            BUDGET_CHECK(0x42D54U);
L_042D54:
            /* 042D54  move.b  (a0)+,(a1)+ */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                wr8(a1, v);
                a1 += 1;
            }
            /* 042D56  dbra  d0,$42d54 */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
L_042D5A:
        wr8(a3 + 0x1DDE, rd8(0x432FB));                      /* 042D5A  move.b  $432fb(pc),$1dde(a3) */
        wr16(a3 + 0xEB0, rd16(0x432FC));                     /* 042D60  move.w  $432fc(pc),$eb0(a3) */
        fz = !(rd8(a3 + 0x1DD6) & 0x8);                      /* 042D66  btst  #$3,$1dd6(a3) */
        /* 042D6C  bne  $42d74 */
        if (COND(7)) {
            wr8(a3 + 0xEB6, rd8(0x432FD));                       /* 042D6E  move.b  $432fd(pc),$eb6(a3) */
        }
L_042D74:
        wr8(a3 + 0x1DDF, rd8(0x43305));                      /* 042D74  move.b  $43305(pc),$1ddf(a3) */
        wr8(a3 + 0x1DE0, rd8(0x43307));                      /* 042D7A  move.b  $43307(pc),$1de0(a3) */
        SETB(d0, rd8(0x43304));                              /* 042D80  move.b  $43304(pc),d0 */
        SETW(d0, SX8(d0));                                   /* 042D84  ext.w  d0 */
        /* 042D86  add.w  d0,d0 */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        a2 = 0x43278;                                        /* 042D88  lea  $43278(pc),a2 */
        a2 = SX16(rd16(a2 + SX16(d0)));                      /* 042D8C  movea.w  $0(a2,d0.w),a2 */
        a2 += a3;                                            /* 042D90  adda.l  a3,a2 */
        wr32(a3 + 0x1DE2, a2);                               /* 042D92  move.l  a2,$1de2(a3) */
        wr32(a3 + 0x1DE6, a2);                               /* 042D96  move.l  a2,$1de6(a3) */
        wr8(a3 + 0xEB8, rd8(a2));                            /* 042D9A  move.b  (a2),$eb8(a3) */
        wr8(a3 + 0xFCA, 1);                                  /* 042D9E  move.b  #$1,$fca(a3) */
        RTS();                                               /* 042DA4  rts */
    }
    /* 042DA6  cmp.b  #$1,d1 */
    /* 042DAA  bne  $42e16 */
    if ((d1 & 0xFF) == 1) {
        wr8(a3 + 0xFCB, 0);                                  /* 042DAC  sf  $fcb(a3) */
        SETW(d0, SX8(d0));                                   /* 042DB0  ext.w  d0 */
        SETW(d0, d0 + (d0 & 0xFFFF));                        /* 042DB2  add.w  d0,d0 */
        a0 = 0x430D8;                                        /* 042DB4  lea  $430d8(pc),a0 */
        a0 = SX16(rd16(a0 + SX16(d0)));                      /* 042DB8  movea.w  $0(a0,d0.w),a0 */
        a0 += a3;                                            /* 042DBC  adda.l  a3,a0 */
        d0 = 0xD;                                            /* 042DBE  moveq  #13,d0 */
        a1 = 0x43314;                                        /* 042DC0  lea  $43314(pc),a1 */
        do {
            BUDGET_CHECK(0x42DC4U);
L_042DC4:
            /* 042DC4  move.b  (a0)+,(a1)+ */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                wr8(a1, v);
                a1 += 1;
            }
            /* 042DC6  dbra  d0,$42dc4 */
            SETW(d0, d0 - 1);
        } while ((d0 & 0xFFFF) != 0xFFFF);
        wr8(a3 + 0x1DF8, rd8(0x43315));                      /* 042DCA  move.b  $43315(pc),$1df8(a3) */
        wr16(a3 + 0xEB2, rd16(0x43316));                     /* 042DD0  move.w  $43316(pc),$eb2(a3) */
        fz = !(rd8(a3 + 0x1DF0) & 0x8);                      /* 042DD6  btst  #$3,$1df0(a3) */
        /* 042DDC  bne  $42de4 */
        if (COND(7)) {
            wr8(a3 + 0xEB6, rd8(0x43317));                       /* 042DDE  move.b  $43317(pc),$eb6(a3) */
        }
        wr8(a3 + 0x1DF9, rd8(0x4331F));                      /* 042DE4  move.b  $4331f(pc),$1df9(a3) */
        wr8(a3 + 0x1DFA, rd8(0x43321));                      /* 042DEA  move.b  $43321(pc),$1dfa(a3) */
        SETB(d0, rd8(0x4331E));                              /* 042DF0  move.b  $4331e(pc),d0 */
        SETW(d0, SX8(d0));                                   /* 042DF4  ext.w  d0 */
        /* 042DF6  add.w  d0,d0 */
        {
            uint32_t s = (d0 & 0xFFFF);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        a2 = 0x43278;                                        /* 042DF8  lea  $43278(pc),a2 */
        a2 = SX16(rd16(a2 + SX16(d0)));                      /* 042DFC  movea.w  $0(a2,d0.w),a2 */
        a2 += a3;                                            /* 042E00  adda.l  a3,a2 */
        wr32(a3 + 0x1DFC, a2);                               /* 042E02  move.l  a2,$1dfc(a3) */
        wr32(a3 + 0x1E00, a2);                               /* 042E06  move.l  a2,$1e00(a3) */
        wr8(a3 + 0xEB9, rd8(a2));                            /* 042E0A  move.b  (a2),$eb9(a3) */
        wr8(a3 + 0xFCB, 1);                                  /* 042E0E  move.b  #$1,$fcb(a3) */
        RTS();                                               /* 042E14  rts */
    }
    wr8(a3 + 0xFCC, 0);                                  /* 042E16  sf  $fcc(a3) */
    SETW(d0, SX8(d0));                                   /* 042E1A  ext.w  d0 */
    SETW(d0, d0 + (d0 & 0xFFFF));                        /* 042E1C  add.w  d0,d0 */
    a0 = 0x430D8;                                        /* 042E1E  lea  $430d8(pc),a0 */
    a0 = SX16(rd16(a0 + SX16(d0)));                      /* 042E22  movea.w  $0(a0,d0.w),a0 */
    a0 += a3;                                            /* 042E26  adda.l  a3,a0 */
    d0 = 0xD;                                            /* 042E28  moveq  #13,d0 */
    a1 = 0x4332E;                                        /* 042E2A  lea  $4332e(pc),a1 */
    do {
        BUDGET_CHECK(0x42E2EU);
L_042E2E:
        /* 042E2E  move.b  (a0)+,(a1)+ */
        {
            uint32_t v = rd8(a0);
            a0 += 1;
            wr8(a1, v);
            a1 += 1;
        }
        /* 042E30  dbra  d0,$42e2e */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
    wr8(a3 + 0x1E12, rd8(0x4332F));                      /* 042E34  move.b  $4332f(pc),$1e12(a3) */
    wr16(a3 + 0xEB4, rd16(0x43330));                     /* 042E3A  move.w  $43330(pc),$eb4(a3) */
    fz = !(rd8(a3 + 0x1E0A) & 0x8);                      /* 042E40  btst  #$3,$1e0a(a3) */
    /* 042E46  bne  $42e4e */
    if (COND(7)) {
        wr8(a3 + 0xEB6, rd8(0x43331));                       /* 042E48  move.b  $43331(pc),$eb6(a3) */
    }
    wr8(a3 + 0x1E13, rd8(0x43339));                      /* 042E4E  move.b  $43339(pc),$1e13(a3) */
    wr8(a3 + 0x1E14, rd8(0x4333B));                      /* 042E54  move.b  $4333b(pc),$1e14(a3) */
    SETB(d0, rd8(0x43338));                              /* 042E5A  move.b  $43338(pc),d0 */
    SETW(d0, SX8(d0));                                   /* 042E5E  ext.w  d0 */
    /* 042E60  add.w  d0,d0 */
    {
        uint32_t s = (d0 & 0xFFFF);
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
        fx = (r < s);
    }
    a2 = 0x43278;                                        /* 042E62  lea  $43278(pc),a2 */
    a2 = SX16(rd16(a2 + SX16(d0)));                      /* 042E66  movea.w  $0(a2,d0.w),a2 */
    a2 += a3;                                            /* 042E6A  adda.l  a3,a2 */
    wr32(a3 + 0x1E16, a2);                               /* 042E6C  move.l  a2,$1e16(a3) */
    wr32(a3 + 0x1E1A, a2);                               /* 042E70  move.l  a2,$1e1a(a3) */
    wr8(a3 + 0xEBA, rd8(a2));                            /* 042E74  move.b  (a2),$eba(a3) */
    wr8(a3 + 0xFCC, 1);                                  /* 042E78  move.b  #$1,$fcc(a3) */
    RTS();                                               /* 042E7E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x42D3CU: goto L_042D3C;
        case 0x42D54U: goto L_042D54;
        case 0x42D5AU: goto L_042D5A;
        case 0x42D74U: goto L_042D74;
        case 0x42DC4U: goto L_042DC4;
        case 0x42E2EU: goto L_042E2E;
        }
    }
}

/* 042E82  game routine */
void sub_042E82(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_042E82;
L_042E80:
    RTS();                                               /* 042E80  rts */
L_042E82:
    a3 = 0x4152A;                                        /* 042E82  lea  $4152a(pc),a3 */
    CALL(sub_043348, 0x42E8AU);                          /* 042E86  bsr  $43348 */
L_042E8A:
    /* 042E8A  tst.b  $fca(a3) */
    {
        uint32_t v = rd8(a3 + 0xFCA);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 042E8E  bmi  $42e80 */
    if (COND(11)) {
        BUDGET_CHECK(0x42E80U);
        goto L_042E80;
    }
L_042E90:
    /* 042E90  beq  $42e94 */
    if (COND(6)) {
L_042E92:
        CALL(sub_042EAA, 0x42E94U);                          /* 042E92  bsr  $42eaa */
    }
L_042E94:
    /* 042E94  tst.b  $fcb(a3) */
    /* 042E98  beq  $42e9e */
    if (rd8(a3 + 0xFCB) != 0) {
        CALL(sub_042F64, 0x42E9EU);                          /* 042E9A  bsr  $42f64 */
    }
L_042E9E:
    /* 042E9E  tst.b  $fcc(a3) */
    /* 042EA2  beq  $42ea8 */
    if (rd8(a3 + 0xFCC) != 0) {
        CALL(sub_04301E, 0x42EA8U);                          /* 042EA4  bsr  $4301e */
    }
L_042EA8:
    /* 042EA8  bra  $42e80 */
    BUDGET_CHECK(0x42E80U);
    goto L_042E80;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x42E80U: goto L_042E80;
        case 0x42E8AU: goto L_042E8A;
        case 0x42E90U: goto L_042E90;
        case 0x42E92U: goto L_042E92;
        case 0x42E94U: goto L_042E94;
        case 0x42E9EU: goto L_042E9E;
        case 0x42EA8U: goto L_042EA8;
        }
    }
}

/* 042EAA  game routine */
void sub_042EAA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 042EAA  tst.b  $1dd0(a3) */
    /* 042EAE  bne  $42ec0 */
    if (rd8(a3 + 0x1DD0) == 0) {
L_042EB0:
        /* 042EB0  tst.b  $1ddc(a3) */
        /* 042EB4  bne  $42ec0 */
        if (rd8(a3 + 0x1DDC) == 0) {
L_042EB6:
            wr8(a3 + 0xFCA, 0);                                  /* 042EB6  sf  $fca(a3) */
            wr8(a3 + 0xEAA, 0);                                  /* 042EBA  clr.b  $eaa(a3) */
            RTS();                                               /* 042EBE  rts */
        }
    }
L_042EC0:
    wr8(a3 + 0x1DD0, rd8(a3 + 0x1DD0) - 1);              /* 042EC0  subq.b  #1,$1dd0(a3) */
    /* 042EC4  tst.b  $1dde(a3) */
    /* 042EC8  bne  $42efe */
    if (rd8(a3 + 0x1DDE) == 0) {
L_042ECA:
        /* 042ECA  tst.b  $1ddc(a3) */
        /* 042ECE  bne  $42ed6 */
        if (rd8(a3 + 0x1DDC) == 0) {
L_042ED0:
            /* 042ED0  tst.b  $1dd9(a3) */
            /* 042ED4  beq  $42f3a */
            if (rd8(a3 + 0x1DD9) == 0) {
                goto L_042F3A;
            }
        }
L_042ED6:
        wr8(a3 + 0x1DD9, rd8(a3 + 0x1DD9) - 1);              /* 042ED6  subq.b  #1,$1dd9(a3) */
        wr8(a3 + 0x1DDE, rd8(0x432FB));                      /* 042EDA  move.b  $432fb(pc),$1dde(a3) */
        /* 042EE0  move.b  $43301(pc),d0 */
        /* 042EE4  beq  $42eea */
        SETB(d0, rd8(0x43301));
        if ((d0 & 0xFF) != 0) {
            SETB(d0, rd8(0x43364));                              /* 042EE6  move.b  $43364(pc),d0 */
        }
L_042EEA:
        SETB(d1, rd8(0x432FD));                              /* 042EEA  move.b  $432fd(pc),d1 */
        /* 042EEE  add.b  d0,d1 */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d1, r);
            fx = (r < s);
        }
        wr8(a3 + 0xEB1, (d1 & 0xFF));                        /* 042EF0  move.b  d1,$eb1(a3) */
        /* 042EF4  move.b  $432fc(pc),d1 */
        {
            uint32_t v = rd8(0x432FC);
            SETB(d1, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 042EF8  addx.b  d0,d1 */
        { uint32_t s, d, r, x = fx, ea1, ea2; (void)ea1; (void)ea2;
        s = d0 & 0xFFU; d = d1 & 0xFFU;
        r = (s + d + x) & 0xFFU;
        fv = !!((~(s ^ d) & (s ^ r)) & 0x80U); fc = fx = (((uint64_t)s + d + x) >> 8) & 1;
        fn = !!(r & 0x80U); if (r) fz = 0;
        d1 = (d1 & 0xFFFFFF00U) | ((r) & 0xFFU);
        }
        wr8(a3 + 0xEB0, (d1 & 0xFF));                        /* 042EFA  move.b  d1,$eb0(a3) */
    }
L_042EFE:
    wr8(a3 + 0x1DDE, rd8(a3 + 0x1DDE) - 1);              /* 042EFE  subq.b  #1,$1dde(a3) */
    /* 042F02  move.b  $43307(pc),d0 */
    /* 042F06  beq  $42f12 */
    SETB(d0, rd8(0x43307));
    if ((d0 & 0xFF) != 0) {
        /* 042F08  subq.b  #1,$1de0(a3) */
        {
            uint32_t s = 1;
            uint32_t d = rd8(a3 + 0x1DE0);
            uint32_t r = (d - s) & 0xFF;
            wr8(a3 + 0x1DE0, r);
            fz = (r == 0);
        }
        /* 042F0C  bne  $42f2c */
        if (COND(6)) {
            goto L_042F2C;
        }
        wr8(a3 + 0x1DE0, (d0 & 0xFF));                       /* 042F0E  move.b  d0,$1de0(a3) */
    }
L_042F12:
    /* 042F12  tst.b  $1dd8(a3) */
    {
        uint32_t v = rd8(a3 + 0x1DD8);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 042F16  beq  $42f2c */
    if (COND(6)) {
L_042F18:
        /* 042F18  bpl  $42f24 */
        if (COND(11)) {
            SETW(d0, rd16(0x432FE));                             /* 042F1A  move.w  $432fe(pc),d0 */
            wr16(a3 + 0xEB0, rd16(a3 + 0xEB0) + (d0 & 0xFFFF));  /* 042F1E  add.w  d0,$eb0(a3) */
            /* 042F22  bra  $42f2c */
        } else {
L_042F24:
            SETW(d0, rd16(0x432FE));                             /* 042F24  move.w  $432fe(pc),d0 */
            wr16(a3 + 0xEB0, rd16(a3 + 0xEB0) - (d0 & 0xFFFF));  /* 042F28  sub.w  d0,$eb0(a3) */
        }
    }
L_042F2C:
    fz = !(rd8(a3 + 0x1DD6) & 0x8);                      /* 042F2C  btst  #$3,$1dd6(a3) */
    /* 042F32  bne  $42f3a */
    if (COND(7)) {
        wr8(a3 + 0xEB6, rd8(0x423DB));                       /* 042F34  move.b  $423db(pc),$eb6(a3) */
    }
L_042F3A:
    /* 042F3A  subq.b  #1,$1ddf(a3) */
    {
        uint32_t s = 1;
        uint32_t d = rd8(a3 + 0x1DDF);
        uint32_t r = (d - s) & 0xFF;
        wr8(a3 + 0x1DDF, r);
        fz = (r == 0);
        fx = (s > d);
    }
    /* 042F3E  bne  $42f62 */
    if (COND(7)) {
L_042F40:
        wr8(a3 + 0x1DDF, rd8(0x43305));                      /* 042F40  move.b  $43305(pc),$1ddf(a3) */
        a2 = rd32(0x43310);                                  /* 042F46  movea.l  $43310(pc),a2 */
        /* 042F4A  move.b  (a2)+,d0 */
        {
            uint32_t v = rd8(a2);
            a2 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 042F4C  bpl  $42f5a */
        if (COND(11)) {
            /* 042F4E  cmp.b  #$80,d0 */
            /* 042F52  bne  $42f62 */
            if ((d0 & 0xFF) != 0x80) {
                goto L_042F62;
            }
            a2 = rd32(0x4330C);                                  /* 042F54  movea.l  $4330c(pc),a2 */
            /* 042F58  move.b  (a2)+,d0 */
            SETB(d0, rd8(a2));
            a2 += 1;
        }
L_042F5A:
        wr32(a3 + 0x1DE6, a2);                               /* 042F5A  move.l  a2,$1de6(a3) */
        wr8(a3 + 0xEB8, (d0 & 0xFF));                        /* 042F5E  move.b  d0,$eb8(a3) */
    }
L_042F62:
    RTS();                                               /* 042F62  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x42EB0U: goto L_042EB0;
        case 0x42EB6U: goto L_042EB6;
        case 0x42EC0U: goto L_042EC0;
        case 0x42ECAU: goto L_042ECA;
        case 0x42ED0U: goto L_042ED0;
        case 0x42ED6U: goto L_042ED6;
        case 0x42EEAU: goto L_042EEA;
        case 0x42EFEU: goto L_042EFE;
        case 0x42F12U: goto L_042F12;
        case 0x42F18U: goto L_042F18;
        case 0x42F24U: goto L_042F24;
        case 0x42F3AU: goto L_042F3A;
        case 0x42F40U: goto L_042F40;
        case 0x42F5AU: goto L_042F5A;
        case 0x42F62U: goto L_042F62;
        }
    }
}

/* 042F64  game routine */
void sub_042F64(void)
{
    /* 042F64  tst.b  $1dea(a3) */
    /* 042F68  bne  $42f7a */
    if (rd8(a3 + 0x1DEA) == 0) {
        /* 042F6A  tst.b  $1df6(a3) */
        /* 042F6E  bne  $42f7a */
        if (rd8(a3 + 0x1DF6) == 0) {
            wr8(a3 + 0xFCB, 0);                                  /* 042F70  sf  $fcb(a3) */
            wr8(a3 + 0xEAB, 0);                                  /* 042F74  clr.b  $eab(a3) */
            RTS();                                               /* 042F78  rts */
        }
    }
    wr8(a3 + 0x1DEA, rd8(a3 + 0x1DEA) - 1);              /* 042F7A  subq.b  #1,$1dea(a3) */
    /* 042F7E  tst.b  $1df8(a3) */
    /* 042F82  bne  $42fb8 */
    if (rd8(a3 + 0x1DF8) == 0) {
        /* 042F84  tst.b  $1df6(a3) */
        /* 042F88  bne  $42f90 */
        if (rd8(a3 + 0x1DF6) == 0) {
            /* 042F8A  tst.b  $1df3(a3) */
            /* 042F8E  beq  $42ff4 */
            if (rd8(a3 + 0x1DF3) == 0) {
                goto L_042FF4;
            }
        }
        wr8(a3 + 0x1DF3, rd8(a3 + 0x1DF3) - 1);              /* 042F90  subq.b  #1,$1df3(a3) */
        wr8(a3 + 0x1DF8, rd8(0x43315));                      /* 042F94  move.b  $43315(pc),$1df8(a3) */
        /* 042F9A  move.b  $4331b(pc),d0 */
        /* 042F9E  beq  $42fa4 */
        SETB(d0, rd8(0x4331B));
        if ((d0 & 0xFF) != 0) {
            SETB(d0, rd8(0x43365));                              /* 042FA0  move.b  $43365(pc),d0 */
        }
        SETB(d1, rd8(0x43317));                              /* 042FA4  move.b  $43317(pc),d1 */
        /* 042FA8  add.b  d0,d1 */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d1, r);
            fx = (r < s);
        }
        wr8(a3 + 0xEB3, (d1 & 0xFF));                        /* 042FAA  move.b  d1,$eb3(a3) */
        /* 042FAE  move.b  $43316(pc),d1 */
        {
            uint32_t v = rd8(0x43316);
            SETB(d1, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 042FB2  addx.b  d0,d1 */
        { uint32_t s, d, r, x = fx, ea1, ea2; (void)ea1; (void)ea2;
        s = d0 & 0xFFU; d = d1 & 0xFFU;
        r = (s + d + x) & 0xFFU;
        fv = !!((~(s ^ d) & (s ^ r)) & 0x80U); fc = fx = (((uint64_t)s + d + x) >> 8) & 1;
        fn = !!(r & 0x80U); if (r) fz = 0;
        d1 = (d1 & 0xFFFFFF00U) | ((r) & 0xFFU);
        }
        wr8(a3 + 0xEB2, (d1 & 0xFF));                        /* 042FB4  move.b  d1,$eb2(a3) */
    }
    wr8(a3 + 0x1DF8, rd8(a3 + 0x1DF8) - 1);              /* 042FB8  subq.b  #1,$1df8(a3) */
    /* 042FBC  move.b  $43321(pc),d0 */
    /* 042FC0  beq  $42fcc */
    SETB(d0, rd8(0x43321));
    if ((d0 & 0xFF) != 0) {
        /* 042FC2  subq.b  #1,$1dfa(a3) */
        {
            uint32_t s = 1;
            uint32_t d = rd8(a3 + 0x1DFA);
            uint32_t r = (d - s) & 0xFF;
            wr8(a3 + 0x1DFA, r);
            fz = (r == 0);
        }
        /* 042FC6  bne  $42fe6 */
        if (COND(6)) {
            goto L_042FE6;
        }
        wr8(a3 + 0x1DFA, (d0 & 0xFF));                       /* 042FC8  move.b  d0,$1dfa(a3) */
    }
    /* 042FCC  tst.b  $1df2(a3) */
    {
        uint32_t v = rd8(a3 + 0x1DF2);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 042FD0  beq  $42fe6 */
    if (COND(6)) {
        /* 042FD2  bpl  $42fde */
        if (COND(11)) {
            SETW(d0, rd16(0x43318));                             /* 042FD4  move.w  $43318(pc),d0 */
            wr16(a3 + 0xEB2, rd16(a3 + 0xEB2) + (d0 & 0xFFFF));  /* 042FD8  add.w  d0,$eb2(a3) */
            /* 042FDC  bra  $42fe6 */
        } else {
            SETW(d0, rd16(0x43318));                             /* 042FDE  move.w  $43318(pc),d0 */
            wr16(a3 + 0xEB2, rd16(a3 + 0xEB2) - (d0 & 0xFFFF));  /* 042FE2  sub.w  d0,$eb2(a3) */
        }
    }
L_042FE6:
    fz = !(rd8(a3 + 0x1DF0) & 0x8);                      /* 042FE6  btst  #$3,$1df0(a3) */
    /* 042FEC  bne  $42ff4 */
    if (COND(7)) {
        wr8(a3 + 0xEB6, rd8(0x423DD));                       /* 042FEE  move.b  $423dd(pc),$eb6(a3) */
    }
L_042FF4:
    /* 042FF4  subq.b  #1,$1df9(a3) */
    {
        uint32_t s = 1;
        uint32_t d = rd8(a3 + 0x1DF9);
        uint32_t r = (d - s) & 0xFF;
        wr8(a3 + 0x1DF9, r);
        fz = (r == 0);
        fx = (s > d);
    }
    /* 042FF8  bne  $4301c */
    if (COND(7)) {
        wr8(a3 + 0x1DF9, rd8(0x4331F));                      /* 042FFA  move.b  $4331f(pc),$1df9(a3) */
        a2 = rd32(0x4332A);                                  /* 043000  movea.l  $4332a(pc),a2 */
        /* 043004  move.b  (a2)+,d0 */
        {
            uint32_t v = rd8(a2);
            a2 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 043006  bpl  $43014 */
        if (COND(11)) {
            /* 043008  cmp.b  #$80,d0 */
            /* 04300C  bne  $4301c */
            if ((d0 & 0xFF) != 0x80) {
                goto L_04301C;
            }
            a2 = rd32(0x43326);                                  /* 04300E  movea.l  $43326(pc),a2 */
            /* 043012  move.b  (a2)+,d0 */
            SETB(d0, rd8(a2));
            a2 += 1;
        }
        wr32(a3 + 0x1E00, a2);                               /* 043014  move.l  a2,$1e00(a3) */
        wr8(a3 + 0xEB9, (d0 & 0xFF));                        /* 043018  move.b  d0,$eb9(a3) */
    }
L_04301C:
    RTS();                                               /* 04301C  rts */
}

