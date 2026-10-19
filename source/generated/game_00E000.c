/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 00E2E8  game routine */
void sub_00E2E8(void)
{
    /* 00E2E8  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00E2EA  addq.w  #8,(a2) */
    {
        uint32_t s = 8;
        uint32_t d = rd16(a2);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(a2, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    MISS(0xE2ECU);
}

/* 00E660  Timer A (vector $134), sample playback: empty version */
void timer_a_isr_idle(void)
{
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 00E660  bclr  #$5,$fffa0f.l */
    RTE();                                               /* 00E668  rte */
}

/* 00E66A  Timer A: sends a sample nibble to YM channel A */
void timer_a_isr_sample(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00E66A  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    wr8(HW_YM_SELECT, 8);                                /* 00E66C  move.b  #$8,$ffff8800.l */
    a0 = rd32(V_0144);                                   /* 00E674  movea.l  $144(a5),a0 */
L_00E678:
    /* 00E678  move.b  (a0)+,$ffff8802.l */
    HOOK(35, 0xE678U);
    wr8(HW_YM_DATA, rd8(a0));
    a0 += 1;
    wr32(V_0144, a0);                                    /* 00E67E  move.l  a0,$144(a5) */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 00E682  bclr  #$5,$fffa0f.l */
    /* 00E68A  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTE();                                               /* 00E68C  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE678U: goto L_00E678;
        }
    }
}

/* 00E68E  game routine */
void sub_00E68E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00E68E  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    wr8(HW_YM_SELECT, 9);                                /* 00E690  move.b  #$9,$ffff8800.l */
    a0 = rd32(V_014C);                                   /* 00E698  movea.l  $14c(a5),a0 */
L_00E69C:
    /* 00E69C  move.b  (a0)+,$ffff8802.l */
    HOOK(36, 0xE69CU);
    wr8(HW_YM_DATA, rd8(a0));
    a0 += 1;
    wr32(V_014C, a0);                                    /* 00E6A2  move.l  a0,$14c(a5) */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 00E6A6  bclr  #$5,$fffa0f.l */
    /* 00E6AE  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTE();                                               /* 00E6B0  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE69CU: goto L_00E69C;
        }
    }
}

/* 00E6B2  game routine */
void sub_00E6B2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00E6B2  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    wr8(HW_YM_SELECT, 8);                                /* 00E6B4  move.b  #$8,$ffff8800.l */
    a0 = rd32(V_0144);                                   /* 00E6BC  movea.l  $144(a5),a0 */
L_00E6C0:
    /* 00E6C0  move.b  (a0)+,$ffff8802.l */
    HOOK(37, 0xE6C0U);
    wr8(HW_YM_DATA, rd8(a0));
    a0 += 1;
    wr32(V_0144, a0);                                    /* 00E6C6  move.l  a0,$144(a5) */
    wr8(HW_YM_SELECT, 9);                                /* 00E6CA  move.b  #$9,$ffff8800.l */
    a0 = rd32(V_014C);                                   /* 00E6D2  movea.l  $14c(a5),a0 */
L_00E6D6:
    /* 00E6D6  move.b  (a0)+,$ffff8802.l */
    HOOK(38, 0xE6D6U);
    wr8(HW_YM_DATA, rd8(a0));
    a0 += 1;
    wr32(V_014C, a0);                                    /* 00E6DC  move.l  a0,$14c(a5) */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 00E6E0  bclr  #$5,$fffa0f.l */
    /* 00E6E8  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTE();                                               /* 00E6EA  rte */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE6C0U: goto L_00E6C0;
        case 0xE6D6U: goto L_00E6D6;
        }
    }
}

/* 00E6EC  game routine */
void sub_00E6EC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00E6EC  movem.l  a1/a0,-(a7) */
    HOOK(39, 0xE6ECU);
    PUSH32(a1);
    PUSH32(a0);
    /* 00E6F0  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    /* 00E6F2  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_00E6F6:
    wr8(HW_YM_SELECT, 7);                                /* 00E6F6  move.b  #$7,$ffff8800.l */
    SETB(d1, rd8(HW_YM_SELECT));                         /* 00E6FE  move.b  $ffff8800.l,d1 */
    SETB(d1, d1 | 9);                                    /* 00E704  ori.b  #$9,d1 */
L_00E708:
    /* 00E708  move.b  d1,$ffff8802.l */
    HOOK(40, 0xE708U);
    wr8(HW_YM_DATA, (d1 & 0xFF));
    wr8(0x424FC, 0xFF);                                  /* 00E70E  st  $424fc.l */
    /* 00E714  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_00E716:
    wr8(V_0158, 0xFF);                                   /* 00E716  st  $158(a5) */
    a0 = 0xE86A;                                         /* 00E71A  lea  $e86a(pc),a0 */
    SETW(d0, (d0 & 0xFFFF) << 3);                        /* 00E71E  lsl.w  #3,d0 */
    a1 = rd32(a0 + 0x4 + SX16(d0));                      /* 00E720  movea.l  $4(a0,d0.w),a1 */
    a0 = rd32(a0 + SX16(d0));                            /* 00E724  movea.l  $0(a0,d0.w),a0 */
    wr32(V_0144, a0);                                    /* 00E728  move.l  a0,$144(a5) */
    wr32(V_0148, a1);                                    /* 00E72C  move.l  a1,$148(a5) */
    /* 00E730  tst.b  $159(a5) */
    /* 00E734  beq  $e742 */
    if (rd8(V_0159) != 0) {
L_00E736:
        a0 = 0xE6B2;                                         /* 00E736  lea  $e6b2(pc),a0 */
        wr32(V_0134, a0);                                    /* 00E73A  move.l  a0,$134.l */
        /* 00E740  bra  $e74c */
    } else {
L_00E742:
        a0 = 0xE66A;                                         /* 00E742  lea  $e66a(pc),a0 */
        wr32(V_0134, a0);                                    /* 00E746  move.l  a0,$134.l */
    }
L_00E74C:
    CALL(sub_00782C, 0xE750U);                           /* 00E74C  bsr  $782c */
L_00E750:
    /* 00E750  movem.l  (a7)+,a0-a1 */
    a0 = POP32();
    a1 = POP32();
    RTS();                                               /* 00E754  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE6F6U: goto L_00E6F6;
        case 0xE708U: goto L_00E708;
        case 0xE716U: goto L_00E716;
        case 0xE736U: goto L_00E736;
        case 0xE742U: goto L_00E742;
        case 0xE74CU: goto L_00E74C;
        case 0xE750U: goto L_00E750;
        }
    }
}

/* 00E756  game routine */
void sub_00E756(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00E756  movem.l  a1/a0,-(a7) */
    HOOK(41, 0xE756U);
    PUSH32(a1);
    PUSH32(a0);
    /* 00E75A  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    /* 00E75C  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_00E760:
    wr8(HW_YM_SELECT, 7);                                /* 00E760  move.b  #$7,$ffff8800.l */
    SETB(d1, rd8(HW_YM_SELECT));                         /* 00E768  move.b  $ffff8800.l,d1 */
    SETB(d1, d1 | 0x12);                                 /* 00E76E  ori.b  #$12,d1 */
L_00E772:
    /* 00E772  move.b  d1,$ffff8802.l */
    HOOK(42, 0xE772U);
    wr8(HW_YM_DATA, (d1 & 0xFF));
    wr8(0x424FD, 0xFF);                                  /* 00E778  st  $424fd.l */
    /* 00E77E  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_00E780:
    wr8(V_0159, 0xFF);                                   /* 00E780  st  $159(a5) */
    a0 = 0xE86A;                                         /* 00E784  lea  $e86a(pc),a0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 3, 16));      /* 00E788  lsl.w  #3,d0 */
    a1 = rd32(a0 + 0x4 + SX16(d0));                      /* 00E78A  movea.l  $4(a0,d0.w),a1 */
    a0 = rd32(a0 + SX16(d0));                            /* 00E78E  movea.l  $0(a0,d0.w),a0 */
    wr32(V_014C, a0);                                    /* 00E792  move.l  a0,$14c(a5) */
    wr32(V_0150, a1);                                    /* 00E796  move.l  a1,$150(a5) */
    /* 00E79A  tst.b  $158(a5) */
    /* 00E79E  beq  $e7ac */
    if (rd8(V_0158) != 0) {
L_00E7A0:
        a0 = 0xE6B2;                                         /* 00E7A0  lea  $e6b2(pc),a0 */
        wr32(V_0134, a0);                                    /* 00E7A4  move.l  a0,$134.l */
        /* 00E7AA  bra  $e7b6 */
    } else {
L_00E7AC:
        a0 = 0xE68E;                                         /* 00E7AC  lea  $e68e(pc),a0 */
        wr32(V_0134, a0);                                    /* 00E7B0  move.l  a0,$134.l */
    }
L_00E7B6:
    CALL(sub_00782C, 0xE7BAU);                           /* 00E7B6  bsr  $782c */
L_00E7BA:
    /* 00E7BA  movem.l  (a7)+,a0-a1 */
    a0 = POP32();
    a1 = POP32();
    RTS();                                               /* 00E7BE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE760U: goto L_00E760;
        case 0xE772U: goto L_00E772;
        case 0xE780U: goto L_00E780;
        case 0xE7A0U: goto L_00E7A0;
        case 0xE7ACU: goto L_00E7AC;
        case 0xE7B6U: goto L_00E7B6;
        case 0xE7BAU: goto L_00E7BA;
        }
    }
}

/* 00E7C0  game routine */
void sub_00E7C0(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00E7C0  tst.w  $202(a5) */
    /* 00E7C4  bne  $e868 */
    if (rd16(V_0202) == 0) {
L_00E7C8:
        /* 00E7C8  tst.w  $158(a5) */
        /* 00E7CC  bne  $e7dc */
        if (rd16(V_0158) == 0) {
L_00E7CE:
            a0 = 0xE660;                                         /* 00E7CE  lea  $e660(pc),a0 */
            wr32(V_0134, a0);                                    /* 00E7D2  move.l  a0,$134.l */
            /* 00E7D8  bra  $e868 */
        } else {
L_00E7DC:
            wr16(HW_PALETTE, 0);                                 /* 00E7DC  clr.w  $ff8240.l */
            /* 00E7E2  tst.b  $158(a5) */
            /* 00E7E6  beq  $e800 */
            if (rd8(V_0158) != 0) {
L_00E7E8:
                d0 = rd32(V_0144);                                   /* 00E7E8  move.l  $144(a5),d0 */
                d1 = rd32(V_0148);                                   /* 00E7EC  move.l  $148(a5),d1 */
                /* 00E7F0  cmp.l  d1,d0 */
                /* 00E7F2  bls  $e800 */
                if (d0 > d1) {
L_00E7F4:
                    wr8(0x424FC, 0);                                     /* 00E7F4  sf  $424fc.l */
                    wr8(V_0158, 0);                                      /* 00E7FA  sf  $158.l */
                }
            }
L_00E800:
            /* 00E800  tst.b  $159(a5) */
            /* 00E804  beq  $e81e */
            if (rd8(V_0159) != 0) {
L_00E806:
                d0 = rd32(V_014C);                                   /* 00E806  move.l  $14c(a5),d0 */
                d1 = rd32(V_0150);                                   /* 00E80A  move.l  $150(a5),d1 */
                /* 00E80E  cmp.l  d1,d0 */
                /* 00E810  bls  $e81e */
                if (d0 > d1) {
L_00E812:
                    wr8(0x424FD, 0);                                     /* 00E812  sf  $424fd.l */
                    wr8(V_0159, 0);                                      /* 00E818  sf  $159.l */
                }
            }
L_00E81E:
            /* 00E81E  tst.w  $158(a5) */
            /* 00E822  bne  $e832 */
            if (rd16(V_0158) == 0) {
L_00E824:
                a0 = 0xE660;                                         /* 00E824  lea  $e660(pc),a0 */
                wr32(V_0134, a0);                                    /* 00E828  move.l  a0,$134.l */
                /* 00E82E  bra  $e868 */
            } else {
L_00E832:
                /* 00E832  cmpi.w  #$ffff,$158(a5) */
                /* 00E838  bne  $e848 */
                if (rd16(V_0158) == 0xFFFF) {
L_00E83A:
                    a0 = 0xE6B2;                                         /* 00E83A  lea  $e6b2(pc),a0 */
                    wr32(V_0134, a0);                                    /* 00E83E  move.l  a0,$134.l */
                    /* 00E844  bra  $e868 */
                } else {
L_00E848:
                    /* 00E848  tst.b  $158.l */
                    /* 00E84E  beq  $e85e */
                    if (rd8(V_0158) != 0) {
L_00E850:
                        a0 = 0xE66A;                                         /* 00E850  lea  $e66a(pc),a0 */
                        wr32(V_0134, a0);                                    /* 00E854  move.l  a0,$134.l */
                        /* 00E85A  bra  $e868 */
                    } else {
L_00E85E:
                        a0 = 0xE68E;                                         /* 00E85E  lea  $e68e(pc),a0 */
                        wr32(V_0134, a0);                                    /* 00E862  move.l  a0,$134.l */
                    }
                }
            }
        }
    }
L_00E868:
    RTS();                                               /* 00E868  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE7C8U: goto L_00E7C8;
        case 0xE7CEU: goto L_00E7CE;
        case 0xE7DCU: goto L_00E7DC;
        case 0xE7E8U: goto L_00E7E8;
        case 0xE7F4U: goto L_00E7F4;
        case 0xE800U: goto L_00E800;
        case 0xE806U: goto L_00E806;
        case 0xE812U: goto L_00E812;
        case 0xE81EU: goto L_00E81E;
        case 0xE824U: goto L_00E824;
        case 0xE832U: goto L_00E832;
        case 0xE83AU: goto L_00E83A;
        case 0xE848U: goto L_00E848;
        case 0xE850U: goto L_00E850;
        case 0xE85EU: goto L_00E85E;
        case 0xE868U: goto L_00E868;
        }
    }
}

/* 00E86A  game routine */
void sub_00E86A(void)
{
    SETB(d5, d5 | 0x56);                                 /* 00E86A  ori.b  #$56,d5 */
    SETB(d5, d5 | 0xFA);                                 /* 00E86E  ori.b  #$fa,d5 */
    SETB(d5, d5 | 0xFA);                                 /* 00E872  ori.b  #$fa,d5 */
    SETB(d5, d5 | 8);                                    /* 00E876  ori.b  #$8,d5 */
    SETB(d5, d5 | 8);                                    /* 00E87A  ori.b  #$8,d5 */
    SETB(d5, d5 | 0x4A);                                 /* 00E87E  ori.b  #$4a,d5 */
    SETB(d5, d5 | 0x4A);                                 /* 00E882  ori.b  #$4a,d5 */
    SETB(d5, d5 | 0x66);                                 /* 00E886  ori.b  #$66,d5 */
    SETB(d5, d5 | 0x66);                                 /* 00E88A  ori.b  #$66,d5 */
    SETB(d5, d5 | 0x78);                                 /* 00E88E  ori.b  #$78,d5 */
    SETB(d5, d5 | 0x78);                                 /* 00E892  ori.b  #$78,d5 */
    SETB(d5, d5 | 0x1A);                                 /* 00E896  ori.b  #$1a,d5 */
    SETB(d5, d5 | 0x1A);                                 /* 00E89A  ori.b  #$1a,d5 */
    SETB(d5, d5 | 0x80);                                 /* 00E89E  ori.b  #$80,d5 */
    SETB(d5, d5 | 0x80);                                 /* 00E8A2  ori.b  #$80,d5 */
    SETB(d5, d5 | 0x96);                                 /* 00E8A6  ori.b  #$96,d5 */
    SETB(d5, d5 | 0x96);                                 /* 00E8AA  ori.b  #$96,d5 */
    /* 00E8AE  ori.b  #$28,d5 */
    SETB(d5, d5 | 0x28);
    { sub_00E8B2(); return; }
}

/* 00E8B2  game routine */
void sub_00E8B2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00EA5C, 0xE8B6U);                           /* 00E8B2  bsr  $ea5c */
    for (;;) {
        BUDGET_CHECK(0xE8B6U);
L_00E8B6:
        CALL(sub_00EA4A, 0xE8BAU);                           /* 00E8B6  bsr  $ea4a */
L_00E8BA:
        CALL(sub_00EA76, 0xE8BEU);                           /* 00E8BA  bsr  $ea76 */
L_00E8BE:
        /* 00E8BE  cmp.w  #$ffff,d0 */
        /* 00E8C2  beq  $e902 */
        if ((d0 & 0xFFFF) == 0xFFFF) {
            goto L_00E902;
        }
L_00E8C6:
        wr16(V_01BC, 2);                                     /* 00E8C6  move.w  #$2,$1bc(a5) */
        wr16(V_01BA, 0x15E);                                 /* 00E8CC  move.w  #$15e,$1ba(a5) */
        wr16(a6, 0x84);                                      /* 00E8D2  move.w  #$84,(a6) */
        SETW(d0, rd16(V_016A));                              /* 00E8D6  move.w  $16a(a5),d0 */
        CALL(sub_00EA06, 0xE8DEU);                           /* 00E8DA  bsr  $ea06 */
        do {
            BUDGET_CHECK(0xE8DEU);
L_00E8DE:
            CALL(sub_00E9EA, 0xE8E2U);                           /* 00E8DE  bsr  $e9ea */
L_00E8E2:
            CALL(sub_00EA5C, 0xE8E6U);                           /* 00E8E2  bsr  $ea5c */
L_00E8E6:
            CALL(sub_00E9AE, 0xE8EAU);                           /* 00E8E6  bsr  $e9ae */
L_00E8EA:
            CALL(sub_00EA76, 0xE8EEU);                           /* 00E8EA  bsr  $ea76 */
L_00E8EE:
            fz = !(d0 & 0x40);                                   /* 00E8EE  btst  #$6,d0 */
            /* 00E8F2  bne  $e902 */
            if (COND(6)) {
                goto L_00E902;
            }
            /* 00E8F6  andi.b  #$5c,d0 */
            {
                uint32_t s = 0x5C;
                uint32_t d = (d0 & 0xFF);
                uint32_t r = (d & s) & 0xFF;
                SETB(d0, r);
                fz = (r == 0);
            }
            /* 00E8FA  beq  $e902 */
            if (COND(7)) {
                goto L_00E902;
            }
            /* 00E8FC  subq.w  #1,$1bc(a5) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(V_01BC);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(V_01BC, r);
                fz = (r == 0);
                fx = (s > d);
            }
            /* 00E900  bne  $e8de */
        } while (COND(6));
L_00E902:
        /* 00E902  subq.w  #1,$276(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0276);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0276, r);
            fz = (r == 0);
        }
        /* 00E906  beq  $e928 */
        if (COND(7)) {
            goto L_00E928;
        }
        wr32(V_016C, rd32(V_016C) + 0x200);                  /* 00E908  addi.l  #$200,$16c(a5) */
        /* 00E910  addq.w  #1,$16a(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_016A);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_016A, r);
            fx = (r < s);
        }
        /* 00E914  cmpi.w  #$9,$16a(a5) */
        /* 00E91A  bls  $e8c6 */
        if (rd16(V_016A) <= 9) {
            BUDGET_CHECK(0xE8C6U);
            goto L_00E8C6;
        }
        wr16(V_016A, 1);                                     /* 00E91C  move.w  #$1,$16a(a5) */
        /* 00E922  addq.w  #1,$168(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0168);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_0168, r);
            fx = (r < s);
        }
        /* 00E926  bra  $e8b6 */
    }
L_00E928:
    RTS();                                               /* 00E928  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE8B6U: goto L_00E8B6;
        case 0xE8BAU: goto L_00E8BA;
        case 0xE8BEU: goto L_00E8BE;
        case 0xE8C6U: goto L_00E8C6;
        case 0xE8DEU: goto L_00E8DE;
        case 0xE8E2U: goto L_00E8E2;
        case 0xE8E6U: goto L_00E8E6;
        case 0xE8EAU: goto L_00E8EA;
        case 0xE8EEU: goto L_00E8EE;
        }
    }
}

/* 00E92A  game routine */
void sub_00E92A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_00E92A;
    for (;;) {
        BUDGET_CHECK(0x2000U);
L_002000:
        /* 002000  movem.l  a4/a3/a2/a1/a0/d4/d3/d2/d1,-(a7) */
        PUSH32(a4);
        PUSH32(a3);
        PUSH32(a2);
        PUSH32(a1);
        PUSH32(a0);
        PUSH32(d4);
        PUSH32(d3);
        PUSH32(d2);
        PUSH32(d1);
        a4 = rd32(V_016C);                                   /* 002004  movea.l  $16c.w,a4 */
        d4 = 0;                                              /* 002008  moveq  #0,d4 */
        SETW(d4, rd16(V_0168));                              /* 00200A  move.w  $168.w,d4 */
        d4 = (d4 & 0xFFFF) * 0x1400;                         /* 00200E  mulu.w  #$1400,d4 */
        d3 = 0;                                              /* 002012  moveq  #0,d3 */
        SETW(d3, rd16(V_016A));                              /* 002014  move.w  $16a.w,d3 */
        SETW(d3, d3 - 1);                                    /* 002018  subq.w  #1,d3 */
        SETW(d3, (d3 & 0xFFFF) << 4);                        /* 00201A  lsl.w  #4,d3 */
        d3 = d3 << 5;                                        /* 00201C  lsl.l  #5,d3 */
        d4 += d3;                                            /* 00201E  add.l  d3,d4 */
        SETW(d3, rd16(V_0276));                              /* 002020  move.w  $276.w,d3 */
        SETW(d3, (d3 & 0xFFFF) << 4);                        /* 002024  lsl.w  #4,d3 */
        d3 = d3 << 5;                                        /* 002026  lsl.l  #5,d3 */
        /* 002028  tst.w  $166.w */
        /* 00202C  beq  $2034 */
        if (rd16(V_0166) != 0) {
L_00202E:
            d4 += 0x64000;                                       /* 00202E  addi.l  #$64000,d4 */
        }
L_002034:
        d3 += d4;                                            /* 002034  add.l  d4,d3 */
        a2 = 0x20EE;                                         /* 002036  lea  $20ee(pc),a2 */
        /* 00203A  tst.l  (a2) */
        /* 00203C  bmi  $206e */
        if ((int32_t)rd32(a2) < 0) {
            goto L_00206E;
        }
L_002040:
        /* 002040  cmp.l  (a2),d4 */
        /* 002042  blt  $206e */
        if ((int32_t)d4 < (int32_t)rd32(a2)) {
            goto L_00206E;
        }
L_002046:
        /* 002046  cmp.l  $4(a2),d4 */
        /* 00204A  bge  $206e */
        if ((int32_t)d4 >= (int32_t)rd32(a2 + 0x4)) {
            goto L_00206E;
        }
L_00204E:
        /* 00204E  cmp.l  $4(a2),d3 */
        /* 002052  bgt  $206e */
        if ((int32_t)d3 > (int32_t)rd32(a2 + 0x4)) {
            goto L_00206E;
        }
L_002056:
        d3 -= d4;                                            /* 002056  sub.l  d4,d3 */
        d4 -= rd32(a2);                                      /* 002058  sub.l  (a2),d4 */
L_00205A:
        a1 = d4;                                             /* 00205A  movea.l  d4,a1 */
        a1 += rd32(0x20FA);                                  /* 00205C  adda.l  $20fa(pc),a1 */
        do {
            BUDGET_CHECK(0x2060U);
L_002060:
            /* 002060  move.b  (a1)+,(a4)+ */
            {
                uint32_t v = rd8(a1);
                a1 += 1;
                wr8(a4, v);
                a4 += 1;
            }
            /* 002062  subq.l  #1,d3 */
            {
                uint32_t s = 1;
                uint32_t d = d3;
                uint32_t r = (d - s) & 0xFFFFFFFF;
                d3 = r;
                fz = (r == 0);
                fx = (s > d);
            }
            /* 002064  bne  $2060 */
        } while (COND(6));
L_002066:
        /* 002066  movem.l  (a7)+,d1-d4/a0-a4 */
        d1 = POP32();
        d2 = POP32();
        d3 = POP32();
        d4 = POP32();
        a0 = POP32();
        a1 = POP32();
        a2 = POP32();
        a3 = POP32();
        a4 = POP32();
        d0 = 0;                                              /* 00206A  moveq  #0,d0 */
        RTS();                                               /* 00206C  rts */
L_00206E:
        /* 00206E  sub.l  d4,d3 */
        {
            uint32_t s = d4;
            uint32_t d = d3;
            uint32_t r = (d - s) & 0xFFFFFFFF;
            d3 = r;
            fx = (s > d);
        }
        a0 = 0x217E;                                         /* 002070  lea  $217e(pc),a0 */
        wr32(a0 + 0xE, rd32(0x20EA));                        /* 002074  move.l  $20ea(pc),$e(a0) */
        /* 00207A  move.b  $2102(pc),d1 */
        /* 00207E  beq  $20a4 */
        SETB(d1, rd8(0x2102));
        if ((d1 & 0xFF) != 0) {
L_002080:
            wr32(a2, 0);                                         /* 002080  clr.l  (a2) */
            wr32(a2 + 0x4, rd32(0x20F6));                        /* 002082  move.l  $20f6(pc),$4(a2) */
            wr32(a0, rd32(0x20FA));                              /* 002088  move.l  $20fa(pc),(a0) */
            wr32(a0 + 0x4, 0);                                   /* 00208C  clr.l  $4(a0) */
            wr32(a0 + 0x8, rd32(0x20F6));                        /* 002090  move.l  $20f6(pc),$8(a0) */
            a1 = rd32(0x217A);                                   /* 002096  movea.l  $217a(pc),a1 */
            /* 00209A  move.w  $8(a1),d1 */
            {
                uint32_t v = rd16(a1 + 0x8);
                SETW(d1, v);
                FL_LOGIC(v, 0xFFFF, 0x8000);
            }
            /* 00209E  jsr  $0(a1,d1.w) */
            { uint32_t ea;
            ea = a1 + 0x0U + SX16(d1);
            CALL_DYN(ea, 0x20A2U); }
L_0020A2:
            /* 0020A2  bra  $20e6 */
        } else {
            wr32(a0 + 0x4, d4);                                  /* 0020A4  move.l  d4,$4(a0) */
            wr32(a2, d4);                                        /* 0020A8  move.l  d4,(a2) */
            wr32(a0 + 0x8, rd32(0x20FE));                        /* 0020AA  move.l  $20fe(pc),$8(a0) */
            d2 = d4;                                             /* 0020B0  move.l  d4,d2 */
            /* 0020B2  add.l  $20fe(pc),d2 */
            {
                uint32_t s = rd32(0x20FE);
                uint32_t d = d2;
                uint32_t r = (d + s) & 0xFFFFFFFF;
                d2 = r;
                fx = (r < s);
            }
            /* 0020B6  cmp.l  $20f6(pc),d2 */
            /* 0020BA  ble  $20d0 */
            if ((int32_t)d2 > (int32_t)rd32(0x20F6)) {
                d2 = rd32(0x20F6);                                   /* 0020BC  move.l  $20f6(pc),d2 */
                d0 = d2;                                             /* 0020C0  move.l  d2,d0 */
                d0 -= rd32(0x20FE);                                  /* 0020C2  sub.l  $20fe(pc),d0 */
                wr32(a0 + 0x4, d0);                                  /* 0020C6  move.l  d0,$4(a0) */
                wr32(a2, d0);                                        /* 0020CA  move.l  d0,(a2) */
                /* 0020CC  sub.l  d0,d4 */
                {
                    uint32_t s = d0;
                    uint32_t d = d4;
                    uint32_t r = (d - s) & 0xFFFFFFFF;
                    d4 = r;
                    fx = (s > d);
                }
                /* 0020CE  bra  $20d2 */
            } else {
                d4 = 0;                                              /* 0020D0  moveq  #0,d4 */
            }
            wr32(a2 + 0x4, d2);                                  /* 0020D2  move.l  d2,$4(a2) */
            wr32(a0, rd32(0x20FA));                              /* 0020D6  move.l  $20fa(pc),(a0) */
            a1 = rd32(0x217A);                                   /* 0020DA  movea.l  $217a(pc),a1 */
            /* 0020DE  move.w  $8(a1),d1 */
            {
                uint32_t v = rd16(a1 + 0x8);
                SETW(d1, v);
                FL_LOGIC(v, 0xFFFF, 0x8000);
            }
            /* 0020E2  jsr  $0(a1,d1.w) */
            { uint32_t ea;
            ea = a1 + 0x0U + SX16(d1);
            CALL_DYN(ea, 0x20E6U); }
        }
L_0020E6:
        /* 0020E6  bra  $205a */
        BUDGET_CHECK(0x205AU);
        goto L_00205A;
L_00E92A:
        /* 00E92A  jmp  $2000.w */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2000U: goto L_002000;
        case 0x202EU: goto L_00202E;
        case 0x2034U: goto L_002034;
        case 0x2040U: goto L_002040;
        case 0x2046U: goto L_002046;
        case 0x204EU: goto L_00204E;
        case 0x2056U: goto L_002056;
        case 0x205AU: goto L_00205A;
        case 0x2060U: goto L_002060;
        case 0x2066U: goto L_002066;
        case 0x206EU: goto L_00206E;
        case 0x2080U: goto L_002080;
        case 0x20A2U: goto L_0020A2;
        case 0x20E6U: goto L_0020E6;
        }
    }
}

/* 00E9AE  game routine */
void sub_00E9AE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(a6, 0x190);                                     /* 00E9AE  move.w  #$190,(a6) */
    wr16(a6, 0x90);                                      /* 00E9B2  move.w  #$90,(a6) */
    wr16(a6, 0x190);                                     /* 00E9B6  move.w  #$190,(a6) */
    d0 = 1;                                              /* 00E9BA  moveq  #1,d0 */
    CALL(sub_00EA06, 0xE9C0U);                           /* 00E9BC  bsr  $ea06 */
L_00E9C0:
    wr16(a6, 0x180);                                     /* 00E9C0  move.w  #$180,(a6) */
    SETW(d0, 0xA0);                                      /* 00E9C4  move.w  #$a0,d0 */
    /* 00E9C8  bra  $ea06 */
    { sub_00EA06(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xE9C0U: goto L_00E9C0;
        }
    }
}

/* 00E9EA  game routine */
void sub_00E9EA(void)
{
    d0 = rd32(V_016C);                                   /* 00E9EA  move.l  $16c(a5),d0 */
    wr8(0xFF860D, (d0 & 0xFF));                          /* 00E9EE  move.b  d0,$ffff860d.l */
    d0 = d0 >> 8;                                        /* 00E9F4  lsr.l  #8,d0 */
    wr8(0xFF860B, (d0 & 0xFF));                          /* 00E9F6  move.b  d0,$ffff860b.l */
    d0 = shift_op(1, 0, d0, 8, 32);                      /* 00E9FC  lsr.l  #8,d0 */
    wr8(0xFF8609, (d0 & 0xFF));                          /* 00E9FE  move.b  d0,$ffff8609.l */
    RTS();                                               /* 00EA04  rts */
}

/* 00EA06  game routine */
void sub_00EA06(void)
{
    RTS();                                               /* 00EA06  rts */
}

/* 00EA10  game routine */
void sub_00EA10(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00EA18, 0xEA12U);                           /* 00EA10  bsr  $ea18 */
L_00EA12:
    RTS();                                               /* 00EA12  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEA12U: goto L_00EA12;
        }
    }
}

/* 00EA18  game routine */
void sub_00EA18(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00EA18  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    /* 00EA1A  move.w  d7,-(a7) */
    {
        uint32_t v = (d7 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    d7 = 0x20;                                           /* 00EA1C  moveq  #32,d7 */
    do {
        BUDGET_CHECK(0xEA1EU);
L_00EA1E:
        /* 00EA1E  dbra  d7,$ea1e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00EA22:
    /* 00EA22  move.w  (a7)+,d7 */
    SETW(d7, rd16(a7));
    a7 += 2;
    /* 00EA24  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_00EA26:
    RTS();                                               /* 00EA26  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEA1EU: goto L_00EA1E;
        case 0xEA22U: goto L_00EA22;
        case 0xEA26U: goto L_00EA26;
        }
    }
}

/* 00EA28  game routine */
void sub_00EA28(void)
{
    RTS();                                               /* 00EA28  rts */
}

/* 00EA4A  game routine */
void sub_00EA4A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(a6, 0x86);                                      /* 00EA4A  move.w  #$86,(a6) */
    SETW(d0, rd16(V_0168));                              /* 00EA4E  move.w  $168(a5),d0 */
    CALL(sub_00EA06, 0xEA54U);                           /* 00EA52  bsr  $ea06 */
L_00EA54:
    wr16(a6, 0x80);                                      /* 00EA54  move.w  #$80,(a6) */
    d0 = 0x13;                                           /* 00EA58  moveq  #19,d0 */
    /* 00EA5A  bra  $ea06 */
    { sub_00EA06(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEA54U: goto L_00EA54;
        }
    }
}

/* 00EA5C  game routine */
void sub_00EA5C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(a6, 0x80);                                      /* 00EA5C  move.w  #$80,(a6) */
    CALL(sub_00EA10, 0xEA62U);                           /* 00EA60  bsr  $ea10 */
L_00EA62:
    d7 = 0x40000;                                        /* 00EA62  move.l  #$40000,d7 */
    fz = !(d0 & 0x80);                                   /* 00EA68  btst  #$7,d0 */
    /* 00EA6C  bne  $ea74 */
    if (COND(7)) {
        d7 = 0x60000;                                        /* 00EA6E  move.l  #$60000,d7 */
    }
    RTS();                                               /* 00EA74  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEA62U: goto L_00EA62;
        }
    }
}

/* 00EA76  game routine */
void sub_00EA76(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00EA76  subq.l  #1,d7 */
    {
        uint32_t s = 1;
        uint32_t d = d7;
        uint32_t r = (d - s) & 0xFFFFFFFF;
        d7 = r;
        fz = (r == 0);
        fx = (s > d);
    }
    /* 00EA78  beq  $ea86 */
    if (COND(6)) {
        /* 00EA7A  btst  #$5,$fffa01.l */
        /* 00EA82  nop */
        /* 00EA84  bra  $ea10 */
        { sub_00EA10(); return; }
    }
    CALL(sub_00EA28, 0xEA88U);                           /* 00EA86  bsr  $ea28 */
L_00EA88:
    d0 = 0xFFFFFFFF;                                     /* 00EA88  moveq  #-1,d0 */
    RTS();                                               /* 00EA8A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEA88U: goto L_00EA88;
        }
    }
}

/* 00EA8C  game routine */
void sub_00EA8C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00EA8C  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    set_sr(get_sr() | 0x700U);                           /* 00EA8E  ori  #$700,sr */
    wr8(HW_YM_SELECT, 0xE);                              /* 00EA92  move.b  #$e,$ffff8800.l */
    SETB(d1, rd8(HW_YM_SELECT));                         /* 00EA9A  move.b  $ffff8800.l,d1 */
    SETB(d2, (d1 & 0xFF));                               /* 00EAA0  move.b  d1,d2 */
    SETB(d1, d1 & 0xF8);                                 /* 00EAA2  andi.b  #$f8,d1 */
    SETB(d0, d0 & 7);                                    /* 00EAA6  andi.b  #$7,d0 */
    SETB(d0, d0 | (d1 & 0xFF));                          /* 00EAAA  or.b  d1,d0 */
    wr8(HW_YM_DATA, (d0 & 0xFF));                        /* 00EAAC  move.b  d0,$ffff8802.l */
    SETB(d0, (d2 & 0xFF));                               /* 00EAB2  move.b  d2,d0 */
    /* 00EAB4  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_00EAB6:
    RTS();                                               /* 00EAB6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEAB6U: goto L_00EAB6;
        }
    }
}

/* 00EAD8  game routine */
void sub_00EAD8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00EAD8  tst.w  $1ba(a5) */
    /* 00EADC  beq  $eb10 */
    if (rd16(V_01BA) != 0) {
        /* 00EADE  subq.w  #1,$1ba(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01BA);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_01BA, r);
            FL_SUB(s, d, r, 0xFFFF, 0x8000, 1);
        }
        /* 00EAE2  bne  $eb10 */
        if (COND(7)) {
            a6 = 0xFF8606;                                       /* 00EAE4  lea  $ff8606.l,a6 */
            /* 00EAEA  move  sr,-(a7) */
            { uint32_t ea; (void)ea;
            a7 -= 2; ea = a7;
            wr16(ea, get_sr());
            }
            set_sr(get_sr() | 0x700U);                           /* 00EAEC  ori  #$700,sr */
            wr16(a6, 0x80);                                      /* 00EAF0  move.w  #$80,(a6) */
            CALL(sub_00EA10, 0xEAF8U);                           /* 00EAF4  bsr  $ea10 */
L_00EAF8:
            fz = !(d0 & 0x80);                                   /* 00EAF8  btst  #$7,d0 */
            /* 00EAFC  beq  $eb06 */
            if (COND(6)) {
                wr16(V_01BA, 1);                                     /* 00EAFE  move.w  #$1,$1ba(a5) */
                /* 00EB04  bra  $eb0a */
            } else {
                /* 00EB06  moveq  #7,d0 */
                d0 = 7;
                FL_LOGIC(d0, 0xFFFFFFFF, 0x80000000);
                CALL(sub_00EA8C, 0xEB0AU);                           /* 00EB08  bsr  $ea8c */
            }
L_00EB0A:
            /* 00EB0A  move  (a7)+,sr */
            { uint32_t s, ea1; (void)ea1;
            ea1 = a7; a7 += 2;
            s = rd16(ea1);
            set_sr(s); }
            CALL(sub_0077F6, 0xEB10U);                           /* 00EB0C  bsr  $77f6 */
        }
    }
L_00EB10:
    RTS();                                               /* 00EB10  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEAF8U: goto L_00EAF8;
        case 0xEB0AU: goto L_00EB0A;
        case 0xEB10U: goto L_00EB10;
        }
    }
}

/* 00EB26  game routine */
void sub_00EB26(void)
{
    RTS();                                               /* 00EB26  rts */
}

/* 00EB76  game routine */
void sub_00EB76(void)
{
    SETB(d0, d0 | 0x57);                                 /* 00EB76  ori.b  #$57,d0 */
    /* 00EB7A  btst  d0,(a0) */
    /* 00EB7C  andi.b  #$33,-(a2) */
    {
        uint32_t s = 0x33;
        a2 -= 1;
        uint32_t d = rd8(a2);
        uint32_t r = (d & s) & 0xFF;
        wr8(a2, r);
    }
    SETW(d4, d4 - 0x777);                                /* 00EB80  subi.w  #$777,d4 */
    /* 00EB84  btst  d1,(a1) */
    /* 00EB86  subi.b  #$31,-(a0) */
    {
        uint32_t s = 0x31;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0, r);
    }
    wr8(a2, rd8(a2) ^ (1u << (d3 & 7)));                 /* 00EB8A  bchg  d3,(a2) */
    /* 00EB8C  bchg  d3,-(a5) */
    a5 -= 1;
    wr8(a5, rd8(a5) ^ (1u << (d3 & 7)));
    d1 ^= (1u << (d3 & 31));                             /* 00EB8E  bchg  d3,d1 */
    a0 -= 1;                                             /* 00EB90  btst  d0,-(a0) */
    wr8(a0 + SX16(d0), rd8(a0 + SX16(d0)) & 0x40);       /* 00EB92  andi.b  #$40,$0(a0,d0.w) */
    SETB(d0, d0 | 0);                                    /* 00EB98  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EB9C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBA0  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBA4  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBA8  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBAC  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBB0  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBB4  ori.b  #$0,d0 */
    /* 00EBB8  ori.b  #$10,-(a7) */
    {
        uint32_t s = 0x10;
        a7 -= 2;
        uint32_t d = rd8(a7);
        uint32_t r = (d | s) & 0xFF;
        wr8(a7, r);
    }
    { sub_00EBBC(); return; }
}

/* 00EB96  game routine */
void sub_00EB96(void)
{
    SETB(d0, d0 | 0);                                    /* 00EB96  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EB9A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EB9E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBA2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBA6  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBAA  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 00EBAE  ori.b  #$0,d0 */
    /* 00EBB2  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_00EBB6(); return; }
}

/* 00EBB6  game routine */
void sub_00EBB6(void)
{
    SETB(d0, d0 | 0x27);                                 /* 00EBB6  ori.b  #$27,d0 */
    /* 00EBBA  btst  d0,(a0) */
    { sub_00EBBC(); return; }
}

/* 00EBBC  game routine */
void sub_00EBBC(void)
{
    /* 00EBBC  andi.b  #$33,-(a2) */
    {
        uint32_t s = 0x33;
        a2 -= 1;
        uint32_t d = rd8(a2);
        uint32_t r = (d & s) & 0xFF;
        wr8(a2, r);
    }
    SETW(d4, d4 - 0x777);                                /* 00EBC0  subi.w  #$777,d4 */
    /* 00EBC4  btst  d1,(a1) */
    /* 00EBC6  subi.b  #$31,-(a0) */
    {
        uint32_t s = 0x31;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0, r);
    }
    wr8(a2, rd8(a2) ^ (1u << (d3 & 7)));                 /* 00EBCA  bchg  d3,(a2) */
    /* 00EBCC  bchg  d3,-(a5) */
    a5 -= 1;
    wr8(a5, rd8(a5) ^ (1u << (d3 & 7)));
    d1 ^= (1u << (d3 & 31));                             /* 00EBCE  bchg  d3,d1 */
    SETW(d7, d7 | 0x410);                                /* 00EBD0  ori.w  #$410,d7 */
    wr16(a1, rd16(a1) + 0);                              /* 00EBD4  addi.w  #$0,(a1) */
    /* 00EBD8  ori.w  #$110,(a7) */
    wr16(a7, rd16(a7) | 0x110);
    { sub_00EBDC(); return; }
}

/* 00EBD6  game routine */
void sub_00EBD6(void)
{
    SETB(d0, d0 | 0x57);                                 /* 00EBD6  ori.b  #$57,d0 */
    /* 00EBDA  btst  d0,(a0) */
    { sub_00EBDC(); return; }
}

/* 00EBDC  game routine */
void sub_00EBDC(void)
{
    /* 00EBDC  andi.b  #$33,-(a2) */
    {
        uint32_t s = 0x33;
        a2 -= 1;
        uint32_t d = rd8(a2);
        uint32_t r = (d & s) & 0xFF;
        wr8(a2, r);
    }
    SETW(d4, d4 - 0x777);                                /* 00EBE0  subi.w  #$777,d4 */
    /* 00EBE4  btst  d1,(a1) */
    /* 00EBE6  subi.b  #$31,-(a0) */
    {
        uint32_t s = 0x31;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d - s) & 0xFF;
        wr8(a0, r);
    }
    wr8(a2, rd8(a2) ^ (1u << (d3 & 7)));                 /* 00EBEA  bchg  d3,(a2) */
    /* 00EBEC  bchg  d3,-(a5) */
    a5 -= 1;
    wr8(a5, rd8(a5) ^ (1u << (d3 & 7)));
    d1 ^= (1u << (d3 & 31));                             /* 00EBEE  bchg  d3,d1 */
    /* 00EBF0  btst  d2,(a0) */
    /* 00EBF2  addi.b  #$60,-$13(a0,d5.w) */
    {
        uint32_t s = 0x60;
        uint32_t d = rd8(a0 - 0x13 + SX16(d5));
        uint32_t r = (d + s) & 0xFF;
        wr8(a0 - 0x13 + SX16(d5), r);
        fx = (r < s);
    }
    /* 00EBF8  andi.l  #$61000044,(a4)+ */
    {
        uint32_t s = 0x61000044;
        uint32_t ea = a4;
        a4 += 4;
        uint32_t d = rd32(ea);
        uint32_t r = (d & s) & 0xFFFFFFFF;
        wr32(ea, r);
    }
    { wait_fire(); return; }
}

/* 00EBFE  Waits for the fire button */
void wait_fire(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    d0 = 0;                                              /* 00EBFE  moveq  #0,d0 */
    CALL(sub_0074F4, 0xEC04U);                           /* 00EC00  bsr  $74f4 */
    for (;;) {
        BUDGET_CHECK(0xEC04U);
L_00EC04:
        /* 00EC04  tst.b  $2ec(a5) */
        /* 00EC08  bne  $ec16 */
        IDLE_EVERY(0xEC04U);
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00EC0C:
        /* 00EC0C  tst.b  $388(a5) */
        /* 00EC10  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00EC14:
        /* 00EC14  bra  $ec04 */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEC04U: goto L_00EC04;
        case 0xEC0CU: goto L_00EC0C;
        case 0xEC14U: goto L_00EC14;
        }
    }
}

/* 00EC16  game routine */
void sub_00EC16(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_01B4, 0);                                     /* 00EC16  clr.w  $1b4(a5) */
    wr16(V_029C, 0);                                     /* 00EC1A  clr.w  $29c(a5) */
    CALL(sub_007866, 0xEC22U);                           /* 00EC1E  bsr  $7866 */
L_00EC22:
    CALL(sub_0083B2, 0xEC26U);                           /* 00EC22  bsr  $83b2 */
L_00EC26:
    /* 00EC26  bra  $88fc */
    { sub_0088FC(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEC22U: goto L_00EC22;
        case 0xEC26U: goto L_00EC26;
        }
    }
}

/* 00EC2A  game routine */
void sub_00EC2A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_01B4, 0xFFFF);                                /* 00EC2A  move.w  #$ffff,$1b4(a5) */
    wr16(V_029C, 0);                                     /* 00EC30  clr.w  $29c(a5) */
    CALL(sub_007866, 0xEC38U);                           /* 00EC34  bsr  $7866 */
L_00EC38:
    CALL(sub_0083B2, 0xEC3CU);                           /* 00EC38  bsr  $83b2 */
L_00EC3C:
    /* 00EC3C  bra  $88fc */
    { sub_0088FC(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEC38U: goto L_00EC38;
        case 0xEC3CU: goto L_00EC3C;
        }
    }
}

/* 00EC40  game routine */
void sub_00EC40(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00EC40  tst.w  $15a(a5) */
    /* 00EC44  bne  $ecc6 */
    if (rd16(V_015A) == 0) {
        wr16(V_0164, 0);                                     /* 00EC48  clr.w  $164(a5) */
        wr16(V_0166, 0);                                     /* 00EC4C  move.w  #$0,$166(a5) */
        wr16(V_0168, 0);                                     /* 00EC52  move.w  #$0,$168(a5) */
        wr16(V_016A, 0xA);                                   /* 00EC58  move.w  #$a,$16a(a5) */
        wr16(V_0276, 0x31);                                  /* 00EC5E  move.w  #$31,$276(a5) */
        wr32(V_016C, 0x68254);                               /* 00EC64  move.l  #$68254,$16c(a5) */
        CALL(sub_00EB26, 0xEC70U);                           /* 00EC6C  bsr  $eb26 */
L_00EC70:
        CALL(sub_00E92A, 0xEC74U);                           /* 00EC70  bsr  $e92a */
L_00EC74:
        a4 = 0x68271;                                        /* 00EC74  lea  $68271.l,a4 */
        a6 = 0x17F80;                                        /* 00EC7A  lea  $17f80.l,a6 */
        a1 = 0x6E24F;                                        /* 00EC80  lea  $6e24f.l,a1 */
        a3 = 0x14000;                                        /* 00EC86  lea  $14000.l,a3 */
        CALL(sub_00FC88, 0xEC90U);                           /* 00EC8C  bsr  $fc88 */
L_00EC90:
        a1 = 0xEB96;                                         /* 00EC90  lea  $eb96(pc),a1 */
        CALL(sub_00AF96, 0xEC98U);                           /* 00EC94  bsr  $af96 */
L_00EC98:
        CALL(wait_vbl, 0xEC9CU);                             /* 00EC98  bsr  $79dc */
L_00EC9C:
        a0 = 0x18000;                                        /* 00EC9C  lea  $18000.l,a0 */
        a1 = 0x20000;                                        /* 00ECA2  lea  $20000.l,a1 */
        SETW(d7, 0x1FFF);                                    /* 00ECA8  move.w  #$1fff,d7 */
        do {
            BUDGET_CHECK(0xECACU);
L_00ECAC:
            /* 00ECAC  move.l  (a0)+,(a1)+ */
            {
                uint32_t v = rd32(a0);
                a0 += 4;
                wr32(a1, v);
                a1 += 4;
            }
            /* 00ECAE  dbra  d7,$ecac */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
        a1 = 0xEB76;                                         /* 00ECB2  lea  $eb76(pc),a1 */
        CALL(sub_00AF96, 0xECBAU);                           /* 00ECB6  bsr  $af96 */
L_00ECBA:
        wr16(V_0202, 0);                                     /* 00ECBA  clr.w  $202(a5) */
        wr16(V_0200, 0xFFFF);                                /* 00ECBE  move.w  #$ffff,$200(a5) */
        RTS();                                               /* 00ECC4  rts */
    }
L_00ECC6:
    wr16(V_0164, 0);                                     /* 00ECC6  clr.w  $164(a5) */
    wr16(V_0166, 1);                                     /* 00ECCA  move.w  #$1,$166(a5) */
    wr16(V_0168, 1);                                     /* 00ECD0  move.w  #$1,$168(a5) */
    wr16(V_016A, 1);                                     /* 00ECD6  move.w  #$1,$16a(a5) */
    wr16(V_0276, 0x8F);                                  /* 00ECDC  move.w  #$8f,$276(a5) */
    wr32(V_016C, 0x5647E);                               /* 00ECE2  move.l  #$5647e,$16c(a5) */
    CALL(sub_00EB26, 0xECEEU);                           /* 00ECEA  bsr  $eb26 */
L_00ECEE:
    CALL(sub_00E92A, 0xECF2U);                           /* 00ECEE  bsr  $e92a */
L_00ECF2:
    a1 = 0xEB96;                                         /* 00ECF2  lea  $eb96(pc),a1 */
    CALL(sub_00AF96, 0xECFAU);                           /* 00ECF6  bsr  $af96 */
L_00ECFA:
    CALL(wait_vbl, 0xECFEU);                             /* 00ECFA  bsr  $79dc */
L_00ECFE:
    a0 = 0x5F22A;                                        /* 00ECFE  lea  $5f22a.l,a0 */
    a1 = 0x20000;                                        /* 00ED04  lea  $20000.l,a1 */
    SETW(d0, 0x1FFF);                                    /* 00ED0A  move.w  #$1fff,d0 */
    do {
        BUDGET_CHECK(0xED0EU);
L_00ED0E:
        /* 00ED0E  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00ED10  dbra  d0,$ed0e */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_00ED14:
    a1 = 0xEB76;                                         /* 00ED14  lea  $eb76(pc),a1 */
    CALL(sub_00AF96, 0xED1CU);                           /* 00ED18  bsr  $af96 */
L_00ED1C:
    SETW(d6, 0x64);                                      /* 00ED1C  move.w  #$64,d6 */
    do {
        BUDGET_CHECK(0xED20U);
L_00ED20:
        CALL(wait_vbl, 0xED24U);                             /* 00ED20  bsr  $79dc */
L_00ED24:
        /* 00ED24  tst.b  $2ec(a5) */
        /* 00ED28  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00ED2C:
        /* 00ED2C  tst.b  $388(a5) */
        /* 00ED30  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00ED34:
        /* 00ED34  dbra  d6,$ed20 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00ED38:
    CALL(sub_00EF0C, 0xED3CU);                           /* 00ED38  bsr  $ef0c */
L_00ED3C:
    SETW(d6, 0x46);                                      /* 00ED3C  move.w  #$46,d6 */
    do {
        BUDGET_CHECK(0xED40U);
L_00ED40:
        CALL(wait_vbl, 0xED44U);                             /* 00ED40  bsr  $79dc */
L_00ED44:
        /* 00ED44  tst.b  $2ec(a5) */
        /* 00ED48  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00ED4C:
        /* 00ED4C  tst.b  $388(a5) */
        /* 00ED50  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00ED54:
        /* 00ED54  dbra  d6,$ed40 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00ED58:
    CALL(sub_00F620, 0xED5CU);                           /* 00ED58  bsr  $f620 */
L_00ED5C:
    SETW(d6, 4);                                         /* 00ED5C  move.w  #$4,d6 */
    do {
        BUDGET_CHECK(0xED60U);
L_00ED60:
        CALL(wait_vbl, 0xED64U);                             /* 00ED60  bsr  $79dc */
L_00ED64:
        /* 00ED64  tst.b  $2ec(a5) */
        /* 00ED68  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00ED6C:
        /* 00ED6C  tst.b  $388(a5) */
        /* 00ED70  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00ED74:
        /* 00ED74  dbra  d6,$ed60 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00ED78:
    CALL(sub_00F5B8, 0xED7CU);                           /* 00ED78  bsr  $f5b8 */
L_00ED7C:
    SETW(d6, 0x24);                                      /* 00ED7C  move.w  #$24,d6 */
    do {
        BUDGET_CHECK(0xED80U);
L_00ED80:
        CALL(wait_vbl, 0xED84U);                             /* 00ED80  bsr  $79dc */
L_00ED84:
        /* 00ED84  tst.b  $2ec(a5) */
        /* 00ED88  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00ED8C:
        /* 00ED8C  tst.b  $388(a5) */
        /* 00ED90  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00ED94:
        /* 00ED94  dbra  d6,$ed80 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00ED98:
    CALL(sub_00F620, 0xED9CU);                           /* 00ED98  bsr  $f620 */
L_00ED9C:
    SETW(d6, 4);                                         /* 00ED9C  move.w  #$4,d6 */
    do {
        BUDGET_CHECK(0xEDA0U);
L_00EDA0:
        CALL(wait_vbl, 0xEDA4U);                             /* 00EDA0  bsr  $79dc */
L_00EDA4:
        /* 00EDA4  tst.b  $2ec(a5) */
        /* 00EDA8  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00EDAC:
        /* 00EDAC  tst.b  $388(a5) */
        /* 00EDB0  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00EDB4:
        /* 00EDB4  dbra  d6,$eda0 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00EDB8:
    CALL(sub_00F5B8, 0xEDBCU);                           /* 00EDB8  bsr  $f5b8 */
L_00EDBC:
    SETW(d6, 0x1A);                                      /* 00EDBC  move.w  #$1a,d6 */
    do {
        BUDGET_CHECK(0xEDC0U);
L_00EDC0:
        CALL(wait_vbl, 0xEDC4U);                             /* 00EDC0  bsr  $79dc */
L_00EDC4:
        /* 00EDC4  tst.b  $2ec(a5) */
        /* 00EDC8  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00EDCC:
        /* 00EDCC  tst.b  $388(a5) */
        /* 00EDD0  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00EDD4:
        /* 00EDD4  dbra  d6,$edc0 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00EDD8:
    CALL(sub_00F5EC, 0xEDDCU);                           /* 00EDD8  bsr  $f5ec */
L_00EDDC:
    SETW(d6, 0x21);                                      /* 00EDDC  move.w  #$21,d6 */
    do {
        BUDGET_CHECK(0xEDE0U);
L_00EDE0:
        CALL(wait_vbl, 0xEDE4U);                             /* 00EDE0  bsr  $79dc */
L_00EDE4:
        /* 00EDE4  tst.b  $2ec(a5) */
        /* 00EDE8  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00EDEC:
        /* 00EDEC  tst.b  $388(a5) */
        /* 00EDF0  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00EDF4:
        /* 00EDF4  dbra  d6,$ede0 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00EDF8:
    CALL(sub_00F606, 0xEDFCU);                           /* 00EDF8  bsr  $f606 */
L_00EDFC:
    SETW(d6, 0x35);                                      /* 00EDFC  move.w  #$35,d6 */
    do {
        BUDGET_CHECK(0xEE00U);
L_00EE00:
        CALL(wait_vbl, 0xEE04U);                             /* 00EE00  bsr  $79dc */
L_00EE04:
        /* 00EE04  tst.b  $2ec(a5) */
        /* 00EE08  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00EE0C:
        /* 00EE0C  tst.b  $388(a5) */
        /* 00EE10  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00EE14:
        /* 00EE14  dbra  d6,$ee00 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_00EE18:
    CALL(sub_00F5D2, 0xEE1CU);                           /* 00EE18  bsr  $f5d2 */
L_00EE1C:
    CALL(sub_007558, 0xEE20U);                           /* 00EE1C  bsr  $7558 */
L_00EE20:
    wr32(V_01E0, 0x66F38);                               /* 00EE20  move.l  #$66f38,$1e0(a5) */
    wr16(V_0202, 0xFFFF);                                /* 00EE28  move.w  #$ffff,$202(a5) */
    a0 = 0xF816;                                         /* 00EE2E  lea  $f816(pc),a0 */
    wr32(V_0134, a0);                                    /* 00EE32  move.l  a0,$134.l */
    /* 00EE38  move  sr,-(a7) */
    { uint32_t ea; (void)ea;
    a7 -= 2; ea = a7;
    wr16(ea, get_sr());
    }
    /* 00EE3A  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_00EE3E:
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);          /* 00EE3E  bclr  #$5,$fffa0f.l */
    wr8(0xFFFA0B, rd8(0xFFFA0B) & ~0x20);                /* 00EE46  bclr  #$5,$fffa0b.l */
    wr8(HW_MFP_IMRA, rd8(HW_MFP_IMRA) | 0x20);           /* 00EE4E  bset  #$5,$fffa13.l */
    wr8(HW_MFP_IERA, rd8(HW_MFP_IERA) | 0x20);           /* 00EE56  bset  #$5,$fffa07.l */
    wr8(HW_MFP_TACR, 2);                                 /* 00EE5E  move.b  #$2,$fffa19.l */
    wr8(HW_MFP_TADR, 0x32);                              /* 00EE66  move.b  #$32,$fffa1f.l */
    /* 00EE6E  move  (a7)+,sr */
    { uint32_t s, ea1; (void)ea1;
    ea1 = a7; a7 += 2;
    s = rd16(ea1);
    set_sr(s); }
L_00EE70:
    a3 = 0xF752;                                         /* 00EE70  lea  $f752(pc),a3 */
    for (;;) {
        BUDGET_CHECK(0xEE74U);
L_00EE74:
        /* 00EE74  tst.b  $2ec(a5) */
        /* 00EE78  bne  $ec16 */
        if (rd8(V_02EC) != 0) {
            { sub_00EC16(); return; }
        }
L_00EE7C:
        /* 00EE7C  tst.b  $388(a5) */
        /* 00EE80  bne  $ec2a */
        if (rd8(V_0388) != 0) {
            { sub_00EC2A(); return; }
        }
L_00EE84:
        CALL(wait_vbl, 0xEE88U);                             /* 00EE84  bsr  $79dc */
L_00EE88:
        d5 = 0;                                              /* 00EE88  moveq  #0,d5 */
        /* 00EE8A  move.b  (a3)+,d5 */
        {
            uint32_t v = rd8(a3);
            a3 += 1;
            SETB(d5, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 00EE8C  bmi  $ee94 */
        if (COND(11)) {
            goto L_00EE94;
        }
L_00EE8E:
        CALL(sub_00F688, 0xEE92U);                           /* 00EE8E  bsr  $f688 */
L_00EE92:
        /* 00EE92  bra  $ee74 */
    }
L_00EE94:
    a0 = 0x1DA68;                                        /* 00EE94  lea  $1da68.l,a0 */
    a1 = 0x25A68;                                        /* 00EE9A  lea  $25a68.l,a1 */
    SETW(d7, 0x24);                                      /* 00EEA0  move.w  #$24,d7 */
    do {
        BUDGET_CHECK(0xEEA4U);
L_00EEA4:
        /* 00EEA4  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEA6  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEA8  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEAA  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEAC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEAE  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEB0  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEB2  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEB4  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEB6  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEB8  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEBA  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEBC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00EEBE  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        a0 = a0 + 0x68;                                      /* 00EEC0  lea  $68(a0),a0 */
        a1 = a1 + 0x68;                                      /* 00EEC4  lea  $68(a1),a1 */
        /* 00EEC8  dbra  d7,$eea4 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00EECC:
    CALL(sub_00F5B8, 0xEED0U);                           /* 00EECC  bsr  $f5b8 */
L_00EED0:
    CALL(sub_00EEF2, 0xEED4U);                           /* 00EED0  bsr  $eef2 */
L_00EED4:
    CALL(sub_00EF3E, 0xEED8U);                           /* 00EED4  bsr  $ef3e */
L_00EED8:
    CALL(sub_00EF58, 0xEEDCU);                           /* 00EED8  bsr  $ef58 */
L_00EEDC:
    CALL(sub_00F56E, 0xEEE0U);                           /* 00EEDC  bsr  $f56e */
L_00EEE0:
    wr16(V_0202, 0);                                     /* 00EEE0  clr.w  $202(a5) */
    wr16(V_0200, 0xFFFF);                                /* 00EEE4  move.w  #$ffff,$200(a5) */
    RTS();                                               /* 00EEEA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEC70U: goto L_00EC70;
        case 0xEC74U: goto L_00EC74;
        case 0xEC90U: goto L_00EC90;
        case 0xEC98U: goto L_00EC98;
        case 0xEC9CU: goto L_00EC9C;
        case 0xECACU: goto L_00ECAC;
        case 0xECBAU: goto L_00ECBA;
        case 0xECC6U: goto L_00ECC6;
        case 0xECEEU: goto L_00ECEE;
        case 0xECF2U: goto L_00ECF2;
        case 0xECFAU: goto L_00ECFA;
        case 0xECFEU: goto L_00ECFE;
        case 0xED0EU: goto L_00ED0E;
        case 0xED14U: goto L_00ED14;
        case 0xED1CU: goto L_00ED1C;
        case 0xED20U: goto L_00ED20;
        case 0xED24U: goto L_00ED24;
        case 0xED2CU: goto L_00ED2C;
        case 0xED34U: goto L_00ED34;
        case 0xED38U: goto L_00ED38;
        case 0xED3CU: goto L_00ED3C;
        case 0xED40U: goto L_00ED40;
        case 0xED44U: goto L_00ED44;
        case 0xED4CU: goto L_00ED4C;
        case 0xED54U: goto L_00ED54;
        case 0xED58U: goto L_00ED58;
        case 0xED5CU: goto L_00ED5C;
        case 0xED60U: goto L_00ED60;
        case 0xED64U: goto L_00ED64;
        case 0xED6CU: goto L_00ED6C;
        case 0xED74U: goto L_00ED74;
        case 0xED78U: goto L_00ED78;
        case 0xED7CU: goto L_00ED7C;
        case 0xED80U: goto L_00ED80;
        case 0xED84U: goto L_00ED84;
        case 0xED8CU: goto L_00ED8C;
        case 0xED94U: goto L_00ED94;
        case 0xED98U: goto L_00ED98;
        case 0xED9CU: goto L_00ED9C;
        case 0xEDA0U: goto L_00EDA0;
        case 0xEDA4U: goto L_00EDA4;
        case 0xEDACU: goto L_00EDAC;
        case 0xEDB4U: goto L_00EDB4;
        case 0xEDB8U: goto L_00EDB8;
        case 0xEDBCU: goto L_00EDBC;
        case 0xEDC0U: goto L_00EDC0;
        case 0xEDC4U: goto L_00EDC4;
        case 0xEDCCU: goto L_00EDCC;
        case 0xEDD4U: goto L_00EDD4;
        case 0xEDD8U: goto L_00EDD8;
        case 0xEDDCU: goto L_00EDDC;
        case 0xEDE0U: goto L_00EDE0;
        case 0xEDE4U: goto L_00EDE4;
        case 0xEDECU: goto L_00EDEC;
        case 0xEDF4U: goto L_00EDF4;
        case 0xEDF8U: goto L_00EDF8;
        case 0xEDFCU: goto L_00EDFC;
        case 0xEE00U: goto L_00EE00;
        case 0xEE04U: goto L_00EE04;
        case 0xEE0CU: goto L_00EE0C;
        case 0xEE14U: goto L_00EE14;
        case 0xEE18U: goto L_00EE18;
        case 0xEE1CU: goto L_00EE1C;
        case 0xEE20U: goto L_00EE20;
        case 0xEE3EU: goto L_00EE3E;
        case 0xEE70U: goto L_00EE70;
        case 0xEE74U: goto L_00EE74;
        case 0xEE7CU: goto L_00EE7C;
        case 0xEE84U: goto L_00EE84;
        case 0xEE88U: goto L_00EE88;
        case 0xEE8EU: goto L_00EE8E;
        case 0xEE92U: goto L_00EE92;
        case 0xEE94U: goto L_00EE94;
        case 0xEEA4U: goto L_00EEA4;
        case 0xEECCU: goto L_00EECC;
        case 0xEED0U: goto L_00EED0;
        case 0xEED4U: goto L_00EED4;
        case 0xEED8U: goto L_00EED8;
        case 0xEEDCU: goto L_00EEDC;
        case 0xEEE0U: goto L_00EEE0;
        }
    }
}

/* 00EEF2  game routine */
void sub_00EEF2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5EB1A;                                        /* 00EEF2  lea  $5eb1a.l,a0 */
    a1 = 0x26260;                                        /* 00EEF8  lea  $26260.l,a1 */
    SETW(d6, 6);                                         /* 00EEFE  move.w  #$6,d6 */
    SETW(d7, 0x1D);                                      /* 00EF02  move.w  #$1d,d7 */
    CALL(sub_00F63A, 0xEF0AU);                           /* 00EF06  bsr  $f63a */
L_00EF0A:
    RTS();                                               /* 00EF0A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEF0AU: goto L_00EF0A;
        }
    }
}

/* 00EF0C  game routine */
void sub_00EF0C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5ABDA;                                        /* 00EF0C  lea  $5abda.l,a0 */
    a1 = 0x203D0;                                        /* 00EF12  lea  $203d0.l,a1 */
    SETW(d6, 8);                                         /* 00EF18  move.w  #$8,d6 */
    SETW(d7, 0xC1);                                      /* 00EF1C  move.w  #$c1,d7 */
    CALL(sub_00F63A, 0xEF24U);                           /* 00EF20  bsr  $f63a */
L_00EF24:
    a0 = 0x5EB1A;                                        /* 00EF24  lea  $5eb1a.l,a0 */
    a1 = 0x26260;                                        /* 00EF2A  lea  $26260.l,a1 */
    SETW(d6, 6);                                         /* 00EF30  move.w  #$6,d6 */
    SETW(d7, 0x1D);                                      /* 00EF34  move.w  #$1d,d7 */
    CALL(sub_00F63A, 0xEF3CU);                           /* 00EF38  bsr  $f63a */
L_00EF3C:
    RTS();                                               /* 00EF3C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEF24U: goto L_00EF24;
        case 0xEF3CU: goto L_00EF3C;
        }
    }
}

/* 00EF3E  game routine */
void sub_00EF3E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5649A;                                        /* 00EF3E  lea  $5649a.l,a0 */
    a1 = 0x20068;                                        /* 00EF44  lea  $20068.l,a1 */
    SETW(d6, 6);                                         /* 00EF4A  move.w  #$6,d6 */
    SETW(d7, 0x3C);                                      /* 00EF4E  move.w  #$3c,d7 */
    CALL(sub_00F63A, 0xEF56U);                           /* 00EF52  bsr  $f63a */
L_00EF56:
    RTS();                                               /* 00EF56  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEF56U: goto L_00EF56;
        }
    }
}

/* 00EF58  game routine */
void sub_00EF58(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x579CA;                                        /* 00EF58  lea  $579ca.l,a0 */
    a1 = 0x27440;                                        /* 00EF5E  lea  $27440.l,a1 */
    SETW(d6, 0);                                         /* 00EF64  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00EF68  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xEF70U);                           /* 00EF6C  bsr  $f63a */
L_00EF70:
    a0 = 0x579CA;                                        /* 00EF70  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00EF76  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00EF7C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00EF80  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xEF88U);                           /* 00EF84  bsr  $f63a */
L_00EF88:
    a0 = 0x579CA;                                        /* 00EF88  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00EF8E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00EF94  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00EF98  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xEFA0U);                           /* 00EF9C  bsr  $f63a */
L_00EFA0:
    a0 = 0x579CA;                                        /* 00EFA0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00EFA6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00EFAC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00EFB0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xEFB8U);                           /* 00EFB4  bsr  $f63a */
L_00EFB8:
    a0 = 0x579CA;                                        /* 00EFB8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00EFBE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00EFC4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00EFC8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xEFD0U);                           /* 00EFCC  bsr  $f63a */
L_00EFD0:
    a0 = 0x579CA;                                        /* 00EFD0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00EFD6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00EFDC  move.w  #$0,d6 */
L_00EFE0:
    SETW(d7, 0xF);                                       /* 00EFE0  move.w  #$f,d7 */
L_00EFE4:
    CALL(sub_00F63A, 0xEFE8U);                           /* 00EFE4  bsr  $f63a */
L_00EFE8:
    a0 = 0x579CA;                                        /* 00EFE8  lea  $579ca.l,a0 */
L_00EFEE:
    a1 = 0x27448;                                        /* 00EFEE  lea  $27448.l,a1 */
L_00EFF4:
    SETW(d6, 0);                                         /* 00EFF4  move.w  #$0,d6 */
L_00EFF8:
    SETW(d7, 0xF);                                       /* 00EFF8  move.w  #$f,d7 */
L_00EFFC:
    CALL(sub_00F63A, 0xF000U);                           /* 00EFFC  bsr  $f63a */
L_00F000:
    a0 = 0x579CA;                                        /* 00F000  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F006  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F00C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F010  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF018U);                           /* 00F014  bsr  $f63a */
L_00F018:
    a0 = 0x579CA;                                        /* 00F018  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F01E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F024  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F028  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF030U);                           /* 00F02C  bsr  $f63a */
L_00F030:
    a0 = 0x579CA;                                        /* 00F030  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F036  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F03C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F040  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF048U);                           /* 00F044  bsr  $f63a */
L_00F048:
    a0 = 0x579CA;                                        /* 00F048  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F04E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F054  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F058  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF060U);                           /* 00F05C  bsr  $f63a */
L_00F060:
    a0 = 0x579CA;                                        /* 00F060  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F066  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F06C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F070  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF078U);                           /* 00F074  bsr  $f63a */
L_00F078:
    a0 = 0x579CA;                                        /* 00F078  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F07E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F084  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F088  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF090U);                           /* 00F08C  bsr  $f63a */
L_00F090:
    a0 = 0x579CA;                                        /* 00F090  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F096  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F09C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F0A0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF0A8U);                           /* 00F0A4  bsr  $f63a */
L_00F0A8:
    a0 = 0x579CA;                                        /* 00F0A8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F0AE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F0B4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F0B8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF0C0U);                           /* 00F0BC  bsr  $f63a */
L_00F0C0:
    a0 = 0x579CA;                                        /* 00F0C0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F0C6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F0CC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F0D0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF0D8U);                           /* 00F0D4  bsr  $f63a */
L_00F0D8:
    a0 = 0x579CA;                                        /* 00F0D8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F0DE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F0E4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F0E8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF0F0U);                           /* 00F0EC  bsr  $f63a */
L_00F0F0:
    a0 = 0x579CA;                                        /* 00F0F0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F0F6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F0FC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F100  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF108U);                           /* 00F104  bsr  $f63a */
L_00F108:
    a0 = 0x579CA;                                        /* 00F108  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F10E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F114  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F118  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF120U);                           /* 00F11C  bsr  $f63a */
L_00F120:
    a0 = 0x579CA;                                        /* 00F120  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F126  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F12C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F130  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF138U);                           /* 00F134  bsr  $f63a */
L_00F138:
    a0 = 0x579CA;                                        /* 00F138  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F13E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F144  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F148  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF150U);                           /* 00F14C  bsr  $f63a */
L_00F150:
    a0 = 0x579CA;                                        /* 00F150  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F156  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F15C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F160  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF168U);                           /* 00F164  bsr  $f63a */
L_00F168:
    a0 = 0x579CA;                                        /* 00F168  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F16E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F174  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F178  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF180U);                           /* 00F17C  bsr  $f63a */
L_00F180:
    a0 = 0x579CA;                                        /* 00F180  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F186  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F18C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F190  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF198U);                           /* 00F194  bsr  $f63a */
L_00F198:
    a0 = 0x579CA;                                        /* 00F198  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F19E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F1A4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F1A8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF1B0U);                           /* 00F1AC  bsr  $f63a */
L_00F1B0:
    a0 = 0x579CA;                                        /* 00F1B0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F1B6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F1BC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F1C0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF1C8U);                           /* 00F1C4  bsr  $f63a */
L_00F1C8:
    a0 = 0x579CA;                                        /* 00F1C8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F1CE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F1D4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F1D8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF1E0U);                           /* 00F1DC  bsr  $f63a */
L_00F1E0:
    a0 = 0x579CA;                                        /* 00F1E0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F1E6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F1EC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F1F0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF1F8U);                           /* 00F1F4  bsr  $f63a */
L_00F1F8:
    a0 = 0x579CA;                                        /* 00F1F8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F1FE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F204  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F208  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF210U);                           /* 00F20C  bsr  $f63a */
L_00F210:
    a0 = 0x579CA;                                        /* 00F210  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F216  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F21C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F220  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF228U);                           /* 00F224  bsr  $f63a */
L_00F228:
    a0 = 0x579CA;                                        /* 00F228  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F22E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F234  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F238  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF240U);                           /* 00F23C  bsr  $f63a */
L_00F240:
    a0 = 0x579CA;                                        /* 00F240  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F246  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F24C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F250  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF258U);                           /* 00F254  bsr  $f63a */
L_00F258:
    a0 = 0x579CA;                                        /* 00F258  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F25E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F264  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F268  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF270U);                           /* 00F26C  bsr  $f63a */
L_00F270:
    a0 = 0x579CA;                                        /* 00F270  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F276  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F27C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F280  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF288U);                           /* 00F284  bsr  $f63a */
L_00F288:
    a0 = 0x579CA;                                        /* 00F288  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F28E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F294  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F298  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF2A0U);                           /* 00F29C  bsr  $f63a */
L_00F2A0:
    a0 = 0x579CA;                                        /* 00F2A0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F2A6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F2AC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F2B0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF2B8U);                           /* 00F2B4  bsr  $f63a */
L_00F2B8:
    a0 = 0x579CA;                                        /* 00F2B8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F2BE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F2C4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F2C8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF2D0U);                           /* 00F2CC  bsr  $f63a */
L_00F2D0:
    a0 = 0x579CA;                                        /* 00F2D0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F2D6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F2DC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F2E0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF2E8U);                           /* 00F2E4  bsr  $f63a */
L_00F2E8:
    a0 = 0x579CA;                                        /* 00F2E8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F2EE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F2F4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F2F8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF300U);                           /* 00F2FC  bsr  $f63a */
L_00F300:
    a0 = 0x579CA;                                        /* 00F300  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F306  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F30C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F310  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF318U);                           /* 00F314  bsr  $f63a */
L_00F318:
    a0 = 0x579CA;                                        /* 00F318  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F31E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F324  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F328  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF330U);                           /* 00F32C  bsr  $f63a */
L_00F330:
    a0 = 0x579CA;                                        /* 00F330  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F336  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F33C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F340  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF348U);                           /* 00F344  bsr  $f63a */
L_00F348:
    a0 = 0x579CA;                                        /* 00F348  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F34E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F354  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F358  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF360U);                           /* 00F35C  bsr  $f63a */
L_00F360:
    a0 = 0x579CA;                                        /* 00F360  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F366  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F36C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F370  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF378U);                           /* 00F374  bsr  $f63a */
L_00F378:
    a0 = 0x579CA;                                        /* 00F378  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F37E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F384  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F388  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF390U);                           /* 00F38C  bsr  $f63a */
L_00F390:
    a0 = 0x579CA;                                        /* 00F390  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F396  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F39C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F3A0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF3A8U);                           /* 00F3A4  bsr  $f63a */
L_00F3A8:
    a0 = 0x579CA;                                        /* 00F3A8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F3AE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F3B4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F3B8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF3C0U);                           /* 00F3BC  bsr  $f63a */
L_00F3C0:
    a0 = 0x579CA;                                        /* 00F3C0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F3C6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F3CC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F3D0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF3D8U);                           /* 00F3D4  bsr  $f63a */
L_00F3D8:
    a0 = 0x579CA;                                        /* 00F3D8  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F3DE  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F3E4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F3E8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF3F0U);                           /* 00F3EC  bsr  $f63a */
L_00F3F0:
    a0 = 0x579CA;                                        /* 00F3F0  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F3F6  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F3FC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F400  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF408U);                           /* 00F404  bsr  $f63a */
L_00F408:
    a0 = 0x579CA;                                        /* 00F408  lea  $579ca.l,a0 */
    a1 = 0x27448;                                        /* 00F40E  lea  $27448.l,a1 */
    SETW(d6, 0);                                         /* 00F414  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F418  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF420U);                           /* 00F41C  bsr  $f63a */
L_00F420:
    a0 = 0x579CA;                                        /* 00F420  lea  $579ca.l,a0 */
    a1 = 0x27450;                                        /* 00F426  lea  $27450.l,a1 */
    SETW(d6, 0);                                         /* 00F42C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F430  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF438U);                           /* 00F434  bsr  $f63a */
L_00F438:
    a0 = 0x579CA;                                        /* 00F438  lea  $579ca.l,a0 */
    a1 = 0x27458;                                        /* 00F43E  lea  $27458.l,a1 */
    SETW(d6, 0);                                         /* 00F444  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F448  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF450U);                           /* 00F44C  bsr  $f63a */
L_00F450:
    a0 = 0x579CA;                                        /* 00F450  lea  $579ca.l,a0 */
    a1 = 0x27460;                                        /* 00F456  lea  $27460.l,a1 */
    SETW(d6, 0);                                         /* 00F45C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F460  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF468U);                           /* 00F464  bsr  $f63a */
L_00F468:
    a0 = 0x579CA;                                        /* 00F468  lea  $579ca.l,a0 */
    a1 = 0x27468;                                        /* 00F46E  lea  $27468.l,a1 */
    SETW(d6, 0);                                         /* 00F474  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F478  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF480U);                           /* 00F47C  bsr  $f63a */
L_00F480:
    a0 = 0x579CA;                                        /* 00F480  lea  $579ca.l,a0 */
    a1 = 0x27470;                                        /* 00F486  lea  $27470.l,a1 */
    SETW(d6, 0);                                         /* 00F48C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F490  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF498U);                           /* 00F494  bsr  $f63a */
L_00F498:
    a0 = 0x579CA;                                        /* 00F498  lea  $579ca.l,a0 */
    a1 = 0x27478;                                        /* 00F49E  lea  $27478.l,a1 */
    SETW(d6, 0);                                         /* 00F4A4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F4A8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF4B0U);                           /* 00F4AC  bsr  $f63a */
L_00F4B0:
    a0 = 0x579CA;                                        /* 00F4B0  lea  $579ca.l,a0 */
    a1 = 0x27480;                                        /* 00F4B6  lea  $27480.l,a1 */
    SETW(d6, 0);                                         /* 00F4BC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F4C0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF4C8U);                           /* 00F4C4  bsr  $f63a */
L_00F4C8:
    a0 = 0x579CA;                                        /* 00F4C8  lea  $579ca.l,a0 */
    a1 = 0x27488;                                        /* 00F4CE  lea  $27488.l,a1 */
    SETW(d6, 0);                                         /* 00F4D4  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F4D8  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF4E0U);                           /* 00F4DC  bsr  $f63a */
L_00F4E0:
    a0 = 0x579CA;                                        /* 00F4E0  lea  $579ca.l,a0 */
    a1 = 0x27490;                                        /* 00F4E6  lea  $27490.l,a1 */
    SETW(d6, 0);                                         /* 00F4EC  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F4F0  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF4F8U);                           /* 00F4F4  bsr  $f63a */
L_00F4F8:
    a0 = 0x579CA;                                        /* 00F4F8  lea  $579ca.l,a0 */
    a1 = 0x27498;                                        /* 00F4FE  lea  $27498.l,a1 */
    SETW(d6, 0);                                         /* 00F504  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F508  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF510U);                           /* 00F50C  bsr  $f63a */
L_00F510:
    a0 = 0x579CA;                                        /* 00F510  lea  $579ca.l,a0 */
    a1 = 0x274A0;                                        /* 00F516  lea  $274a0.l,a1 */
    SETW(d6, 0);                                         /* 00F51C  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F520  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF528U);                           /* 00F524  bsr  $f63a */
L_00F528:
    a0 = 0x579CA;                                        /* 00F528  lea  $579ca.l,a0 */
    a1 = 0x274A8;                                        /* 00F52E  lea  $274a8.l,a1 */
    SETW(d6, 0);                                         /* 00F534  move.w  #$0,d6 */
    SETW(d7, 0xF);                                       /* 00F538  move.w  #$f,d7 */
    CALL(sub_00F63A, 0xF540U);                           /* 00F53C  bsr  $f63a */
L_00F540:
    a0 = 0x571BA;                                        /* 00F540  lea  $571ba.l,a0 */
    a1 = 0x26330;                                        /* 00F546  lea  $26330.l,a1 */
    SETW(d6, 5);                                         /* 00F54C  move.w  #$5,d6 */
    SETW(d7, 0x2A);                                      /* 00F550  move.w  #$2a,d7 */
    CALL(sub_00F63A, 0xF558U);                           /* 00F554  bsr  $f63a */
L_00F558:
    a1 = 0x278C8;                                        /* 00F558  lea  $278c8.l,a1 */
    a2 = 0x278C0;                                        /* 00F55E  lea  $278c0.l,a2 */
    /* 00F564  clr.l  (a1)+ */
    wr32(a1, 0);
    a1 += 4;
    /* 00F566  clr.l  (a1)+ */
    wr32(a1, 0);
    a1 += 4;
    /* 00F568  clr.l  (a2)+ */
    wr32(a2, 0);
    a2 += 4;
    /* 00F56A  clr.l  (a2)+ */
    wr32(a2, 0);
    a2 += 4;
    RTS();                                               /* 00F56C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xEF70U: goto L_00EF70;
        case 0xEF88U: goto L_00EF88;
        case 0xEFA0U: goto L_00EFA0;
        case 0xEFB8U: goto L_00EFB8;
        case 0xEFD0U: goto L_00EFD0;
        case 0xEFE0U: goto L_00EFE0;
        case 0xEFE4U: goto L_00EFE4;
        case 0xEFE8U: goto L_00EFE8;
        case 0xEFEEU: goto L_00EFEE;
        case 0xEFF4U: goto L_00EFF4;
        case 0xEFF8U: goto L_00EFF8;
        case 0xEFFCU: goto L_00EFFC;
        case 0xF000U: goto L_00F000;
        case 0xF018U: goto L_00F018;
        case 0xF030U: goto L_00F030;
        case 0xF048U: goto L_00F048;
        case 0xF060U: goto L_00F060;
        case 0xF078U: goto L_00F078;
        case 0xF090U: goto L_00F090;
        case 0xF0A8U: goto L_00F0A8;
        case 0xF0C0U: goto L_00F0C0;
        case 0xF0D8U: goto L_00F0D8;
        case 0xF0F0U: goto L_00F0F0;
        case 0xF108U: goto L_00F108;
        case 0xF120U: goto L_00F120;
        case 0xF138U: goto L_00F138;
        case 0xF150U: goto L_00F150;
        case 0xF168U: goto L_00F168;
        case 0xF180U: goto L_00F180;
        case 0xF198U: goto L_00F198;
        case 0xF1B0U: goto L_00F1B0;
        case 0xF1C8U: goto L_00F1C8;
        case 0xF1E0U: goto L_00F1E0;
        case 0xF1F8U: goto L_00F1F8;
        case 0xF210U: goto L_00F210;
        case 0xF228U: goto L_00F228;
        case 0xF240U: goto L_00F240;
        case 0xF258U: goto L_00F258;
        case 0xF270U: goto L_00F270;
        case 0xF288U: goto L_00F288;
        case 0xF2A0U: goto L_00F2A0;
        case 0xF2B8U: goto L_00F2B8;
        case 0xF2D0U: goto L_00F2D0;
        case 0xF2E8U: goto L_00F2E8;
        case 0xF300U: goto L_00F300;
        case 0xF318U: goto L_00F318;
        case 0xF330U: goto L_00F330;
        case 0xF348U: goto L_00F348;
        case 0xF360U: goto L_00F360;
        case 0xF378U: goto L_00F378;
        case 0xF390U: goto L_00F390;
        case 0xF3A8U: goto L_00F3A8;
        case 0xF3C0U: goto L_00F3C0;
        case 0xF3D8U: goto L_00F3D8;
        case 0xF3F0U: goto L_00F3F0;
        case 0xF408U: goto L_00F408;
        case 0xF420U: goto L_00F420;
        case 0xF438U: goto L_00F438;
        case 0xF450U: goto L_00F450;
        case 0xF468U: goto L_00F468;
        case 0xF480U: goto L_00F480;
        case 0xF498U: goto L_00F498;
        case 0xF4B0U: goto L_00F4B0;
        case 0xF4C8U: goto L_00F4C8;
        case 0xF4E0U: goto L_00F4E0;
        case 0xF4F8U: goto L_00F4F8;
        case 0xF510U: goto L_00F510;
        case 0xF528U: goto L_00F528;
        case 0xF540U: goto L_00F540;
        case 0xF558U: goto L_00F558;
        }
    }
}

