/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 00F56E  game routine */
void sub_00F56E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x57A4A;                                        /* 00F56E  lea  $57a4a.l,a0 */
    a1 = 0x22988;                                        /* 00F574  lea  $22988.l,a1 */
    SETW(d6, 0xA);                                       /* 00F57A  move.w  #$a,d6 */
    SETW(d7, 4);                                         /* 00F57E  move.w  #$4,d7 */
    CALL(sub_00F63A, 0xF586U);                           /* 00F582  bsr  $f63a */
L_00F586:
    a0 = 0x57C02;                                        /* 00F586  lea  $57c02.l,a0 */
    a1 = 0x22DE8;                                        /* 00F58C  lea  $22de8.l,a1 */
    SETW(d6, 0xA);                                       /* 00F592  move.w  #$a,d6 */
    SETW(d7, 4);                                         /* 00F596  move.w  #$4,d7 */
    CALL(sub_00F63A, 0xF59EU);                           /* 00F59A  bsr  $f63a */
L_00F59E:
    a0 = 0x57DBA;                                        /* 00F59E  lea  $57dba.l,a0 */
    a1 = 0x23248;                                        /* 00F5A4  lea  $23248.l,a1 */
    SETW(d6, 0xA);                                       /* 00F5AA  move.w  #$a,d6 */
    SETW(d7, 4);                                         /* 00F5AE  move.w  #$4,d7 */
    CALL(sub_00F63A, 0xF5B6U);                           /* 00F5B2  bsr  $f63a */
L_00F5B6:
    RTS();                                               /* 00F5B6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF586U: goto L_00F586;
        case 0xF59EU: goto L_00F59E;
        case 0xF5B6U: goto L_00F5B6;
        }
    }
}

/* 00F5B8  game routine */
void sub_00F5B8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5E26A;                                        /* 00F5B8  lea  $5e26a.l,a0 */
    a1 = 0x20CB0;                                        /* 00F5BE  lea  $20cb0.l,a1 */
    SETW(d6, 1);                                         /* 00F5C4  move.w  #$1,d6 */
    SETW(d7, 0x31);                                      /* 00F5C8  move.w  #$31,d7 */
    CALL(sub_00F63A, 0xF5D0U);                           /* 00F5CC  bsr  $f63a */
L_00F5D0:
    RTS();                                               /* 00F5D0  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF5D0U: goto L_00F5D0;
        }
    }
}

/* 00F5D2  game routine */
void sub_00F5D2(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5E59A;                                        /* 00F5D2  lea  $5e59a.l,a0 */
    a1 = 0x20CB0;                                        /* 00F5D8  lea  $20cb0.l,a1 */
    SETW(d6, 1);                                         /* 00F5DE  move.w  #$1,d6 */
    SETW(d7, 0x31);                                      /* 00F5E2  move.w  #$31,d7 */
    CALL(sub_00F63A, 0xF5EAU);                           /* 00F5E6  bsr  $f63a */
L_00F5EA:
    RTS();                                               /* 00F5EA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF5EAU: goto L_00F5EA;
        }
    }
}

/* 00F5EC  game routine */
void sub_00F5EC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5E8CA;                                        /* 00F5EC  lea  $5e8ca.l,a0 */
    a1 = 0x212F0;                                        /* 00F5F2  lea  $212f0.l,a1 */
    SETW(d6, 1);                                         /* 00F5F8  move.w  #$1,d6 */
    SETW(d7, 9);                                         /* 00F5FC  move.w  #$9,d7 */
    CALL(sub_00F63A, 0xF604U);                           /* 00F600  bsr  $f63a */
L_00F604:
    RTS();                                               /* 00F604  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF604U: goto L_00F604;
        }
    }
}

/* 00F606  game routine */
void sub_00F606(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5E8CA;                                        /* 00F606  lea  $5e8ca.l,a0 */
    a1 = 0x212F0;                                        /* 00F60C  lea  $212f0.l,a1 */
    SETW(d6, 1);                                         /* 00F612  move.w  #$1,d6 */
    SETW(d7, 0x1F);                                      /* 00F616  move.w  #$1f,d7 */
    CALL(sub_00F63A, 0xF61EU);                           /* 00F61A  bsr  $f63a */
L_00F61E:
    RTS();                                               /* 00F61E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF61EU: goto L_00F61E;
        }
    }
}

/* 00F620  game routine */
void sub_00F620(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = 0x5EADA;                                        /* 00F620  lea  $5eada.l,a0 */
    a1 = 0x21570;                                        /* 00F626  lea  $21570.l,a1 */
    SETW(d6, 1);                                         /* 00F62C  move.w  #$1,d6 */
    SETW(d7, 3);                                         /* 00F630  move.w  #$3,d7 */
    CALL(sub_00F63A, 0xF638U);                           /* 00F634  bsr  $f63a */
L_00F638:
    RTS();                                               /* 00F638  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF638U: goto L_00F638;
        }
    }
}

/* 00F63A  game routine */
void sub_00F63A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00F63A  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    do {
        BUDGET_CHECK(0xF63EU);
L_00F63E:
        SETW(d5, (d6 & 0xFFFF));                             /* 00F63E  move.w  d6,d5 */
        do {
            BUDGET_CHECK(0xF640U);
L_00F640:
            /* 00F640  movem.w  (a0)+,d0-d3 */
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
            SETW(d4, (d0 & 0xFFFF));                             /* 00F644  move.w  d0,d4 */
            SETW(d4, d4 | (d1 & 0xFFFF));                        /* 00F646  or.w  d1,d4 */
            SETW(d4, d4 | (d2 & 0xFFFF));                        /* 00F648  or.w  d2,d4 */
            SETW(d4, d4 | (d3 & 0xFFFF));                        /* 00F64A  or.w  d3,d4 */
            SETW(d4, ~(d4 & 0xFFFF));                            /* 00F64C  not.w  d4 */
            wr16(a1, rd16(a1) & (d4 & 0xFFFF));                  /* 00F64E  and.w  d4,(a1) */
            wr16(a1 + 0x2, rd16(a1 + 0x2) & (d4 & 0xFFFF));      /* 00F650  and.w  d4,$2(a1) */
            wr16(a1 + 0x4, rd16(a1 + 0x4) & (d4 & 0xFFFF));      /* 00F654  and.w  d4,$4(a1) */
            wr16(a1 + 0x6, rd16(a1 + 0x6) & (d4 & 0xFFFF));      /* 00F658  and.w  d4,$6(a1) */
            /* 00F65C  or.w  d0,(a1)+ */
            {
                uint32_t s = (d0 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 00F65E  or.w  d1,(a1)+ */
            {
                uint32_t s = (d1 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 00F660  or.w  d2,(a1)+ */
            {
                uint32_t s = (d2 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 00F662  or.w  d3,(a1)+ */
            {
                uint32_t s = (d3 & 0xFFFF);
                uint32_t ea = a1;
                a1 += 2;
                uint32_t d = rd16(ea);
                uint32_t r = (d | s) & 0xFFFF;
                wr16(ea, r);
            }
            /* 00F664  dbra  d5,$f640 */
            SETW(d5, d5 - 1);
        } while ((d5 & 0xFFFF) != 0xFFFF);
L_00F668:
        SETW(d5, (d6 & 0xFFFF));                             /* 00F668  move.w  d6,d5 */
        SETW(d3, (d6 & 0xFFFF));                             /* 00F66A  move.w  d6,d3 */
        SETW(d3, (d3 & 0xFFFF) << 3);                        /* 00F66C  lsl.w  #3,d3 */
        d4 = 0xA0;                                           /* 00F66E  move.l  #$a0,d4 */
        /* 00F674  sub.w  d3,d4 */
        {
            uint32_t s = (d3 & 0xFFFF);
            uint32_t d = (d4 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d4, r);
            fx = (s > d);
        }
        a1 = a1 + SX16(d4);                                  /* 00F676  lea  $0(a1,d4.w),a1 */
        a1 = a1 - 0x8;                                       /* 00F67A  lea  -$8(a1),a1 */
        /* 00F67E  dbra  d7,$f63e */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00F682:
    /* 00F682  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 00F686  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF63EU: goto L_00F63E;
        case 0xF640U: goto L_00F640;
        case 0xF668U: goto L_00F668;
        case 0xF682U: goto L_00F682;
        }
    }
}

/* 00F688  game routine */
void sub_00F688(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00F688  tst.w  $26e(a5) */
    /* 00F68C  beq  $f694 */
    if (rd16(V_026E) != 0) {
        /* 00F68E  move.w  #$657,$ff8240.l */
        wr16(HW_PALETTE, 0x657);
    } else {
L_00F694:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 00F694  or.w  d0,d1 */
    }
    a0 = 0x1DA68;                                        /* 00F696  lea  $1da68.l,a0 */
    a1 = 0x25A68;                                        /* 00F69C  lea  $25a68.l,a1 */
    SETW(d7, 0x24);                                      /* 00F6A2  move.w  #$24,d7 */
    do {
        BUDGET_CHECK(0xF6A6U);
L_00F6A6:
        /* 00F6A6  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6A8  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6AA  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6AC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6AE  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6B0  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6B2  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6B4  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6B6  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6B8  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6BA  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6BC  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6BE  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 00F6C0  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        a0 = a0 + 0x68;                                      /* 00F6C2  lea  $68(a0),a0 */
        a1 = a1 + 0x68;                                      /* 00F6C6  lea  $68(a1),a1 */
        /* 00F6CA  dbra  d7,$f6a6 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_00F6CE:
    /* 00F6CE  tst.w  $26e(a5) */
    /* 00F6D2  beq  $f6da */
    if (rd16(V_026E) != 0) {
        /* 00F6D4  move.w  #$431,$ff8240.l */
        wr16(HW_PALETTE, 0x431);
    } else {
L_00F6DA:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 00F6DA  or.w  d0,d1 */
    }
    SETW(d3, rd16(V_0204));                              /* 00F6DC  move.w  $204(a5),d3 */
    d3 = (d3 & 0xFFFF) * 0x508;                          /* 00F6E0  mulu.w  #$508,d3 */
    a0 = 0x597BA;                                        /* 00F6E4  lea  $597ba.l,a0 */
    a0 = a0 + SX16(d3);                                  /* 00F6EA  lea  $0(a0,d3.w),a0 */
    a1 = 0x26260;                                        /* 00F6EE  lea  $26260.l,a1 */
    SETW(d6, 6);                                         /* 00F6F4  move.w  #$6,d6 */
    SETW(d7, 0x16);                                      /* 00F6F8  move.w  #$16,d7 */
    CALL(sub_00F63A, 0xF700U);                           /* 00F6FC  bsr  $f63a */
L_00F700:
    /* 00F700  addq.w  #1,$204(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_0204);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_0204, r);
        fx = (r < s);
    }
    /* 00F704  cmpi.w  #$4,$204(a5) */
    /* 00F70A  bcs  $f71e */
    if (rd16(V_0204) >= 4) {
L_00F70C:
        wr16(V_0204, 0);                                     /* 00F70C  clr.w  $204(a5) */
        /* 00F710  tst.w  $26e(a5) */
        /* 00F714  beq  $f71c */
        if (rd16(V_026E) != 0) {
            /* 00F716  move.w  #$727,$ff8240.l */
            wr16(HW_PALETTE, 0x727);
        } else {
L_00F71C:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 00F71C  or.w  d0,d1 */
        }
    }
L_00F71E:
    a0 = 0x57F72;                                        /* 00F71E  lea  $57f72.l,a0 */
    SETW(d5, d5 & 3);                                    /* 00F724  andi.w  #$3,d5 */
    d5 = (d5 & 0xFFFF) * 0x818;                          /* 00F728  mulu.w  #$818,d5 */
    a0 = a0 + SX16(d5);                                  /* 00F72C  lea  $0(a0,d5.w),a0 */
    a1 = 0x25A60;                                        /* 00F730  lea  $25a60.l,a1 */
    SETW(d6, 6);                                         /* 00F736  move.w  #$6,d6 */
    SETW(d7, 0x24);                                      /* 00F73A  move.w  #$24,d7 */
    CALL(sub_00F63A, 0xF742U);                           /* 00F73E  bsr  $f63a */
L_00F742:
    /* 00F742  tst.w  $26e(a5) */
    /* 00F746  beq  $f74e */
    if (rd16(V_026E) != 0) {
        /* 00F748  move.w  #$0,$ff8240.l */
        wr16(HW_PALETTE, 0);
    } else {
L_00F74E:
        SETW(d1, d1 | (d0 & 0xFFFF));                        /* 00F74E  or.w  d0,d1 */
    }
    RTS();                                               /* 00F750  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF694U: goto L_00F694;
        case 0xF6A6U: goto L_00F6A6;
        case 0xF6CEU: goto L_00F6CE;
        case 0xF6DAU: goto L_00F6DA;
        case 0xF700U: goto L_00F700;
        case 0xF70CU: goto L_00F70C;
        case 0xF71CU: goto L_00F71C;
        case 0xF71EU: goto L_00F71E;
        case 0xF742U: goto L_00F742;
        case 0xF74EU: goto L_00F74E;
        }
    }
}

/* 00F752  game routine */
void sub_00F752(void)
{
    SETB(d1, d1 | 2);                                    /* 00F752  ori.b  #$2,d1 */
    /* 00F756  btst  d0,d2 */
    SETB(d1, d1 & 1);                                    /* 00F758  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F75C  ori.b  #$2,d0 */
    /* 00F760  btst  d0,d1 */
    /* 00F762  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F764  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F768  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F76C  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F770  ori.b  #$2,d0 */
    /* 00F774  btst  d0,d1 */
    /* 00F776  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F778  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F77C  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F780  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F784  ori.b  #$2,d0 */
    /* 00F788  btst  d0,d1 */
    /* 00F78A  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F78C  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F790  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F794  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F798  ori.b  #$2,d0 */
    /* 00F79C  btst  d0,d1 */
    /* 00F79E  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F7A0  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F7A4  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F7A8  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F7AC  ori.b  #$2,d0 */
    /* 00F7B0  btst  d0,d1 */
    /* 00F7B2  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F7B4  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F7B8  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F7BC  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F7C0  ori.b  #$2,d0 */
    /* 00F7C4  btst  d0,d1 */
    /* 00F7C6  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F7C8  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F7CC  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F7D0  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F7D4  ori.b  #$2,d0 */
    /* 00F7D8  btst  d0,d1 */
    /* 00F7DA  btst  d0,d0 */
    SETB(d0, d0 | 1);                                    /* 00F7DC  ori.b  #$1,d0 */
    SETB(d2, d2 & 2);                                    /* 00F7E0  andi.b  #$2,d2 */
    SETB(d1, d1 & 1);                                    /* 00F7E4  andi.b  #$1,d1 */
    SETB(d0, d0 | 2);                                    /* 00F7E8  ori.b  #$2,d0 */
    /* 00F7EC  btst  d0,d1 */
    /* 00F7EE  btst  d0,d0 */
    /* 00F7F0  ori.b  #$ff,d0 */
    {
        uint32_t s = 0xFF;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    /* 00F7F4  bvs  $f864 */
    if (COND(9)) { sub_00F864(); return; }
    MISS(0xF7F6U);
}

/* 00F816  game routine */
void sub_00F816(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00F816  movem.l  a1/a0/d1/d0,-(a7) */
    PUSH32(a1);
    PUSH32(a0);
    PUSH32(d1);
    PUSH32(d0);
    d0 = 0;                                              /* 00F81A  moveq  #0,d0 */
    a0 = rd32(V_01E0);                                   /* 00F81C  movea.l  $1e0(a5),a0 */
    /* 00F820  move.b  (a0)+,d0 */
    SETB(d0, rd8(a0));
    a0 += 1;
    SETB(d0, d0 + 0x80);                                 /* 00F822  addi.b  #$80,d0 */
    SETW(d0, (d0 & 0xFFFF) << 3);                        /* 00F826  lsl.w  #3,d0 */
    wr32(V_01E0, a0);                                    /* 00F828  move.l  a0,$1e0(a5) */
    /* 00F82C  cmpa.l  #$67a0c,a0 */
    /* 00F832  bcs  $f83c */
    if (a0 >= 0x67A0C) {
L_00F834:
        wr32(V_01E0, 0x66F38);                               /* 00F834  move.l  #$66f38,$1e0(a5) */
    }
L_00F83C:
    a1 = 0x67A0C;                                        /* 00F83C  lea  $67a0c.l,a1 */
    d1 = rd32(a1 + SX16(d0));                            /* 00F842  move.l  $0(a1,d0.w),d1 */
    SETW(d0, rd16(a1 + 0x4 + SX16(d0)));                 /* 00F846  move.w  $4(a1,d0.w),d0 */
    a1 = 0xFFFF8800;                                     /* 00F84A  lea  $ffff8800.l,a1 */
    /* 00F850  movep.l  d1,0(a1) */
    wr8(a1 + 0 + 0, d1 >> 24);
    wr8(a1 + 0 + 2, d1 >> 16);
    wr8(a1 + 0 + 4, d1 >> 8);
    wr8(a1 + 0 + 6, d1 >> 0);
    /* 00F854  movep.w  d0,0(a1) */
    wr8(a1 + 0 + 0, d0 >> 8);
    wr8(a1 + 0 + 2, d0 >> 0);
    /* 00F858  movem.l  (a7)+,d0-d1/a0-a1 */
    d0 = POP32();
    d1 = POP32();
    a0 = POP32();
    a1 = POP32();
    /* 00F85C  bclr  #$5,$fffa0f.l */
    wr8(HW_MFP_ISRA, rd8(HW_MFP_ISRA) & ~0x20);
    { sub_00F864(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF834U: goto L_00F834;
        case 0xF83CU: goto L_00F83C;
        }
    }
}

/* 00F864  game routine */
void sub_00F864(void)
{
    RTE();                                               /* 00F864  rte */
}

/* 00F91E  game routine */
void sub_00F91E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0xF91EU);
        /* 00F91E  tst.b  $424f2.l */
        /* 00F924  bne  $f91e */
        IDLE_EVERY(0xF91EU);
    } while (rd8(0x424F2) != 0);
    CALL(sub_00FB4A, 0xF92AU);                           /* 00F926  bsr  $fb4a */
L_00F92A:
    /* 00F92A  tst.w  $2da(a5) */
    /* 00F92E  beq  $f938 */
    if (rd16(V_02DA) != 0) {
        CALL(sub_00FB66, 0xF934U);                           /* 00F930  bsr  $fb66 */
L_00F934:
        CALL(sub_00FBA8, 0xF938U);                           /* 00F934  bsr  $fba8 */
    }
L_00F938:
    /* 00F938  tst.w  $376(a5) */
    /* 00F93C  beq  $f946 */
    if (rd16(V_0376) != 0) {
        CALL(sub_00FBCC, 0xF942U);                           /* 00F93E  bsr  $fbcc */
L_00F942:
        CALL(sub_00FC0E, 0xF946U);                           /* 00F942  bsr  $fc0e */
    }
L_00F946:
    d0 = 0x61A80;                                        /* 00F946  move.l  #$61a80,d0 */
    CALL(sub_00FC60, 0xF950U);                           /* 00F94C  bsr  $fc60 */
L_00F950:
    SETW(d0, rd16(V_021C));                              /* 00F950  move.w  $21c(a5),d0 */
    SETW(d0, (d0 & 0xFFFF) << 2);                        /* 00F954  lsl.w  #2,d0 */
    a0 = 0xC7EA;                                         /* 00F956  lea  $c7ea(pc),a0 */
    d0 = rd32(a0 + SX16(d0));                            /* 00F95A  move.l  $0(a0,d0.w),d0 */
    /* 00F95E  tst.w  $2da(a5) */
    /* 00F962  beq  $f96c */
    if (rd16(V_02DA) != 0) {
        /* 00F964  tst.w  $376(a5) */
        /* 00F968  beq  $f96c */
        if (rd16(V_0376) != 0) {
            SETW(d0, (d0 & 0xFFFF) >> 1);                        /* 00F96A  lsr.w  #1,d0 */
        }
    }
    SETW(d0, shift_op(1, 0, (d0 & 0xFFFF), 1, 16));      /* 00F96C  lsr.w  #1,d0 */
    /* 00F96E  tst.w  $2da(a5) */
    /* 00F972  beq  $f978 */
    if (rd16(V_02DA) != 0) {
        /* 00F974  add.l  d0,$332(a5) */
        {
            uint32_t s = d0;
            uint32_t d = rd32(V_PLAYER1);
            uint32_t r = (d + s) & 0xFFFFFFFF;
            wr32(V_PLAYER1, r);
            fx = (r < s);
        }
    }
    /* 00F978  tst.w  $376(a5) */
    /* 00F97C  beq  $f982 */
    if (rd16(V_0376) != 0) {
        /* 00F97E  add.l  d0,$3ce(a5) */
        {
            uint32_t s = d0;
            uint32_t d = rd32(V_PLAYER2);
            uint32_t r = (d + s) & 0xFFFFFFFF;
            wr32(V_PLAYER2, r);
            fx = (r < s);
        }
    }
    /* 00F982  tst.w  $2da(a5) */
    /* 00F986  beq  $f9a4 */
    if (rd16(V_02DA) != 0) {
        d0 = 0x10;                                           /* 00F988  move.l  #$10,d0 */
        d1 = 0;                                              /* 00F98E  moveq  #0,d1 */
        CALL(sub_041C04, 0xF996U);                           /* 00F990  jsr  $41c04.l */
L_00F996:
        CALL(sub_00FBA8, 0xF99AU);                           /* 00F996  bsr  $fba8 */
L_00F99A:
        d0 = 0x61A80;                                        /* 00F99A  move.l  #$61a80,d0 */
        CALL(sub_00FC60, 0xF9A4U);                           /* 00F9A0  bsr  $fc60 */
    }
L_00F9A4:
    /* 00F9A4  tst.w  $376(a5) */
    /* 00F9A8  beq  $f9c6 */
    if (rd16(V_0376) != 0) {
        d0 = 0x10;                                           /* 00F9AA  move.l  #$10,d0 */
        d1 = 0;                                              /* 00F9B0  moveq  #0,d1 */
        CALL(sub_041C04, 0xF9B8U);                           /* 00F9B2  jsr  $41c04.l */
L_00F9B8:
        CALL(sub_00FC0E, 0xF9BCU);                           /* 00F9B8  bsr  $fc0e */
L_00F9BC:
        d0 = 0x61A80;                                        /* 00F9BC  move.l  #$61a80,d0 */
        CALL(sub_00FC60, 0xF9C6U);                           /* 00F9C2  bsr  $fc60 */
    }
L_00F9C6:
    RTS();                                               /* 00F9C6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF92AU: goto L_00F92A;
        case 0xF934U: goto L_00F934;
        case 0xF938U: goto L_00F938;
        case 0xF942U: goto L_00F942;
        case 0xF946U: goto L_00F946;
        case 0xF950U: goto L_00F950;
        case 0xF996U: goto L_00F996;
        case 0xF99AU: goto L_00F99A;
        case 0xF9A4U: goto L_00F9A4;
        case 0xF9B8U: goto L_00F9B8;
        case 0xF9BCU: goto L_00F9BC;
        case 0xF9C6U: goto L_00F9C6;
        }
    }
}

/* 00F9C8  game routine */
void sub_00F9C8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00FB2E, 0xF9CCU);                           /* 00F9C8  bsr  $fb2e */
L_00F9CC:
    /* 00F9CC  tst.w  $2da(a5) */
    /* 00F9D0  beq  $f9d6 */
    if (rd16(V_02DA) != 0) {
        CALL(sub_00FB82, 0xF9D6U);                           /* 00F9D2  bsr  $fb82 */
    }
L_00F9D6:
    /* 00F9D6  tst.w  $376(a5) */
    /* 00F9DA  beq  $f9e0 */
    if (rd16(V_0376) != 0) {
        CALL(sub_00FBE8, 0xF9E0U);                           /* 00F9DC  bsr  $fbe8 */
    }
L_00F9E0:
    wr16(V_01DA, 0x64);                                  /* 00F9E0  move.w  #$64,$1da(a5) */
    for (;;) {
        BUDGET_CHECK(0xF9E6U);
L_00F9E6:
        /* 00F9E6  tst.w  $1da(a5) */
        /* 00F9EA  beq  $f9f6 */
        if (rd16(V_01DA) == 0) {
            goto L_00F9F6;
        }
        CALL(sub_0086D8, 0xF9F0U);                           /* 00F9EC  bsr  $86d8 */
L_00F9F0:
        /* 00F9F0  subq.w  #1,$1da(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01DA);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_01DA, r);
            fx = (s > d);
        }
        /* 00F9F4  bra  $f9e6 */
    }
    for (;;) {
        BUDGET_CHECK(0xF9F6U);
L_00F9F6:
        CALL(sub_0086D8, 0xF9FAU);                           /* 00F9F6  bsr  $86d8 */
L_00F9FA:
        /* 00F9FA  tst.w  $2da(a5) */
        /* 00F9FE  beq  $fa1a */
        if (rd16(V_02DA) != 0) {
            /* 00FA00  tst.w  $370(a5) */
            /* 00FA04  beq  $fa1a */
            if (rd16(V_0370) != 0) {
                wr16(V_0370, rd16(V_0370) - 1);                      /* 00FA06  subq.w  #1,$370(a5) */
                /* 00FA0A  addi.l  #$a,$332(a5) */
                {
                    uint32_t s = 0xA;
                    uint32_t d = rd32(V_PLAYER1);
                    uint32_t r = (d + s) & 0xFFFFFFFF;
                    wr32(V_PLAYER1, r);
                    fx = (r < s);
                }
                CALL(sub_00FB82, 0xFA16U);                           /* 00FA12  bsr  $fb82 */
L_00FA16:
                CALL(sub_00FBA8, 0xFA1AU);                           /* 00FA16  bsr  $fba8 */
            }
        }
L_00FA1A:
        /* 00FA1A  tst.w  $376(a5) */
        /* 00FA1E  beq  $fa3a */
        if (rd16(V_0376) != 0) {
            /* 00FA20  tst.w  $40c(a5) */
            /* 00FA24  beq  $fa3a */
            if (rd16(V_040C) != 0) {
                wr16(V_040C, rd16(V_040C) - 1);                      /* 00FA26  subq.w  #1,$40c(a5) */
                /* 00FA2A  addi.l  #$a,$3ce(a5) */
                {
                    uint32_t s = 0xA;
                    uint32_t d = rd32(V_PLAYER2);
                    uint32_t r = (d + s) & 0xFFFFFFFF;
                    wr32(V_PLAYER2, r);
                    fx = (r < s);
                }
                CALL(sub_00FBE8, 0xFA36U);                           /* 00FA32  bsr  $fbe8 */
L_00FA36:
                CALL(sub_00FC0E, 0xFA3AU);                           /* 00FA36  bsr  $fc0e */
            }
        }
L_00FA3A:
        SETW(d0, rd16(V_0370));                              /* 00FA3A  move.w  $370(a5),d0 */
        /* 00FA3E  add.w  $40c(a5),d0 */
        {
            uint32_t s = rd16(V_040C);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fz = (r == 0);
            fx = (r < s);
        }
        /* 00FA42  beq  $fa60 */
        if (COND(7)) {
            goto L_00FA60;
        }
        /* 00FA44  tst.w  $1d8(a5) */
        /* 00FA48  bpl  $fa5a */
        if ((int16_t)rd16(V_01D8) < 0) {
            wr16(V_01D8, 4);                                     /* 00FA4A  move.w  #$4,$1d8(a5) */
            d0 = 1;                                              /* 00FA50  moveq  #1,d0 */
            d1 = 0;                                              /* 00FA52  moveq  #0,d1 */
            CALL(sub_041C04, 0xFA5AU);                           /* 00FA54  jsr  $41c04.l */
        }
L_00FA5A:
        /* 00FA5A  subq.w  #1,$1d8(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01D8);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_01D8, r);
            fx = (s > d);
        }
        /* 00FA5E  bra  $f9f6 */
    }
L_00FA60:
    RTS();                                               /* 00FA60  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xF9CCU: goto L_00F9CC;
        case 0xF9D6U: goto L_00F9D6;
        case 0xF9E0U: goto L_00F9E0;
        case 0xF9E6U: goto L_00F9E6;
        case 0xF9F0U: goto L_00F9F0;
        case 0xF9F6U: goto L_00F9F6;
        case 0xF9FAU: goto L_00F9FA;
        case 0xFA16U: goto L_00FA16;
        case 0xFA1AU: goto L_00FA1A;
        case 0xFA36U: goto L_00FA36;
        case 0xFA3AU: goto L_00FA3A;
        case 0xFA5AU: goto L_00FA5A;
        }
    }
}

/* 00FA62  game routine */
void sub_00FA62(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0208, 4);                                     /* 00FA62  move.w  #$4,$208(a5) */
    d7 = 1;                                              /* 00FA68  moveq  #1,d7 */
    do {
        BUDGET_CHECK(0xFA6AU);
L_00FA6A:
        CALL(sub_00FA72, 0xFA6CU);                           /* 00FA6A  bsr  $fa72 */
L_00FA6C:
        /* 00FA6C  dbra  d7,$fa6a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 00FA70  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFA6AU: goto L_00FA6A;
        case 0xFA6CU: goto L_00FA6C;
        }
    }
}

/* 00FA72  game routine */
void sub_00FA72(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    for (;;) {
        BUDGET_CHECK(0xFA72U);
        CALL(sub_0086D8, 0xFA76U);                           /* 00FA72  bsr  $86d8 */
L_00FA76:
        /* 00FA76  tst.w  $208(a5) */
        /* 00FA7A  beq  $fa82 */
        if (rd16(V_0208) == 0) {
            goto L_00FA82;
        }
        /* 00FA7C  subq.w  #1,$208(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0208);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0208, r);
            fx = (s > d);
        }
        /* 00FA80  bra  $fa72 */
    }
L_00FA82:
    wr16(V_0208, 4);                                     /* 00FA82  move.w  #$4,$208(a5) */
    a0 = HW_PALETTE;                                     /* 00FA88  lea  $ff8240.l,a0 */
    d0 = 0xF;                                            /* 00FA8E  moveq  #15,d0 */
    do {
        BUDGET_CHECK(0xFA90U);
L_00FA90:
        SETW(d1, rd16(a0));                                  /* 00FA90  move.w  (a0),d1 */
        /* 00FA92  andi.w  #$700,d1 */
        {
            uint32_t s = 0x700;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d1, r);
            fz = (r == 0);
        }
        /* 00FA96  beq  $fa9c */
        if (COND(6)) {
            /* 00FA98  subi.w  #$100,(a0) */
            {
                uint32_t s = 0x100;
                uint32_t d = rd16(a0);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a0, r);
                fx = (s > d);
            }
        }
        SETW(d1, rd16(a0));                                  /* 00FA9C  move.w  (a0),d1 */
        /* 00FA9E  andi.w  #$70,d1 */
        {
            uint32_t s = 0x70;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d1, r);
            fz = (r == 0);
        }
        /* 00FAA2  beq  $faa8 */
        if (COND(6)) {
            /* 00FAA4  subi.w  #$10,(a0) */
            {
                uint32_t s = 0x10;
                uint32_t d = rd16(a0);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a0, r);
                fx = (s > d);
            }
        }
        SETW(d1, rd16(a0));                                  /* 00FAA8  move.w  (a0),d1 */
        /* 00FAAA  andi.w  #$7,d1 */
        {
            uint32_t s = 7;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d1, r);
            fz = (r == 0);
        }
        /* 00FAAE  beq  $fab4 */
        if (COND(6)) {
            /* 00FAB0  subi.w  #$1,(a0) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(a0);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(a0, r);
                fx = (s > d);
            }
        }
        a0 = a0 + 0x2;                                       /* 00FAB4  lea  $2(a0),a0 */
        /* 00FAB8  dbra  d0,$fa90 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 00FABC  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFA76U: goto L_00FA76;
        case 0xFA90U: goto L_00FA90;
        }
    }
}

/* 00FABE  game routine */
void sub_00FABE(void)
{
    /* 00FABE  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00FAC0  move.l  -(a0),d0 */
    a0 -= 4;
    {
        uint32_t v = rd32(a0);
        d0 = v;
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
    /* 00FAC2  movea.l  d3,a0 */
    a0 = d3;
    MISS(0xFAC4U);
}

/* 00FAFC  game routine */
void sub_00FAFC(void)
{
    /* 00FAFC  move.w  $2020(a6),-(a0) */
    {
        uint32_t v = rd16(a6 + 0x2020);
        a0 -= 2;
        wr16(a0, v);
    }
    /* 00FB00  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00FB02  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00FB04  move.l  -(a4),d0 */
    a4 -= 4;
    d0 = rd32(a4);
    wr8(a2 + 0x20 + SX16(d2), rd8(a2 + 0x20 + SX16(d2)) | 0x20); /* 00FB06  ori.b  #$20,$20(a2,d2.w) */
    /* 00FB0C  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00FB0E  move.l  -(a0),d0 */
    a0 -= 4;
    d0 = rd32(a0);
    /* 00FB10  move.l  d0,d2 */
    d2 = d0;
    { sub_00FB12(); return; }
}

/* 00FB12  game routine */
void sub_00FB12(void)
{
    SETW(d5, d5 + 1);                                    /* 00FB12  addq.w  #1,d5 */
    /* 00FB14  addq.w  #3,d5 */
    {
        uint32_t s = 3;
        uint32_t d = (d5 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d5, r);
        FL_ADD(s, d, r, 0xFFFF, 0x8000, 1);
    }
    /* 00FB16  trap  #7 */
    SOFT(0xFB16U);
    MISS(0xFB18U);
}

/* 00FB2E  game routine */
void sub_00FB2E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x28A0);                                /* 00FB2E  move.w  #$28a0,$266(a5) */
    wr16(V_0268, 0xD);                                   /* 00FB34  move.w  #$d,$268(a5) */
    wr16(V_026A, 0xD);                                   /* 00FB3A  move.w  #$d,$26a(a5) */
    a0 = 0xFB12;                                         /* 00FB40  lea  $fb12(pc),a0 */
    CALL(sub_008928, 0xFB48U);                           /* 00FB44  bsr  $8928 */
L_00FB48:
    RTS();                                               /* 00FB48  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFB48U: goto L_00FB48;
        }
    }
}

/* 00FB4A  game routine */
void sub_00FB4A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x28A0);                                /* 00FB4A  move.w  #$28a0,$266(a5) */
    wr16(V_0268, 0xD);                                   /* 00FB50  move.w  #$d,$268(a5) */
    wr16(V_026A, 0xD);                                   /* 00FB56  move.w  #$d,$26a(a5) */
    a0 = 0xFB20;                                         /* 00FB5C  lea  $fb20(pc),a0 */
    CALL(sub_008928, 0xFB64U);                           /* 00FB60  bsr  $8928 */
L_00FB64:
    RTS();                                               /* 00FB64  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFB64U: goto L_00FB64;
        }
    }
}

/* 00FB66  game routine */
void sub_00FB66(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x37A0);                                /* 00FB66  move.w  #$37a0,$266(a5) */
    wr16(V_0268, 0xC);                                   /* 00FB6C  move.w  #$c,$268(a5) */
    wr16(V_026A, 0xC);                                   /* 00FB72  move.w  #$c,$26a(a5) */
    a0 = 0xFAFC;                                         /* 00FB78  lea  $fafc(pc),a0 */
    CALL(sub_008928, 0xFB80U);                           /* 00FB7C  bsr  $8928 */
L_00FB80:
    RTS();                                               /* 00FB80  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFB80U: goto L_00FB80;
        }
    }
}

/* 00FB82  game routine */
void sub_00FB82(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x37A0);                                /* 00FB82  move.w  #$37a0,$266(a5) */
    wr16(V_0268, 0x10);                                  /* 00FB88  move.w  #$10,$268(a5) */
    wr16(V_026A, 0x10);                                  /* 00FB8E  move.w  #$10,$26a(a5) */
    d0 = 0;                                              /* 00FB94  moveq  #0,d0 */
    SETW(d0, rd16(V_0370));                              /* 00FB96  move.w  $370(a5),d0 */
    CALL(sub_007D3E, 0xFB9EU);                           /* 00FB9A  bsr  $7d3e */
L_00FB9E:
    a0 = V_0261;                                         /* 00FB9E  lea  $261(a5),a0 */
    CALL(sub_008928, 0xFBA6U);                           /* 00FBA2  bsr  $8928 */
L_00FBA6:
    RTS();                                               /* 00FBA6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFB9EU: goto L_00FB9E;
        case 0xFBA6U: goto L_00FBA6;
        }
    }
}

/* 00FBA8  game routine */
void sub_00FBA8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x37A0);                                /* 00FBA8  move.w  #$37a0,$266(a5) */
    wr16(V_0268, 0x16);                                  /* 00FBAE  move.w  #$16,$268(a5) */
    wr16(V_026A, 0x16);                                  /* 00FBB4  move.w  #$16,$26a(a5) */
    d0 = rd32(V_PLAYER1);                                /* 00FBBA  move.l  $332(a5),d0 */
    CALL(sub_007D3E, 0xFBC2U);                           /* 00FBBE  bsr  $7d3e */
L_00FBC2:
    a0 = V_025F;                                         /* 00FBC2  lea  $25f(a5),a0 */
    CALL(sub_008928, 0xFBCAU);                           /* 00FBC6  bsr  $8928 */
L_00FBCA:
    RTS();                                               /* 00FBCA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFBC2U: goto L_00FBC2;
        case 0xFBCAU: goto L_00FBCA;
        }
    }
}

/* 00FBCC  game routine */
void sub_00FBCC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x4420);                                /* 00FBCC  move.w  #$4420,$266(a5) */
    wr16(V_0268, 0xC);                                   /* 00FBD2  move.w  #$c,$268(a5) */
    wr16(V_026A, 0xC);                                   /* 00FBD8  move.w  #$c,$26a(a5) */
    a0 = 0xFB07;                                         /* 00FBDE  lea  $fb07(pc),a0 */
    CALL(sub_008928, 0xFBE6U);                           /* 00FBE2  bsr  $8928 */
L_00FBE6:
    RTS();                                               /* 00FBE6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFBE6U: goto L_00FBE6;
        }
    }
}

/* 00FBE8  game routine */
void sub_00FBE8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x4420);                                /* 00FBE8  move.w  #$4420,$266(a5) */
    wr16(V_0268, 0x10);                                  /* 00FBEE  move.w  #$10,$268(a5) */
    wr16(V_026A, 0x10);                                  /* 00FBF4  move.w  #$10,$26a(a5) */
    d0 = 0;                                              /* 00FBFA  moveq  #0,d0 */
    SETW(d0, rd16(V_040C));                              /* 00FBFC  move.w  $40c(a5),d0 */
    CALL(sub_007D3E, 0xFC04U);                           /* 00FC00  bsr  $7d3e */
L_00FC04:
    a0 = V_0261;                                         /* 00FC04  lea  $261(a5),a0 */
    CALL(sub_008928, 0xFC0CU);                           /* 00FC08  bsr  $8928 */
L_00FC0C:
    RTS();                                               /* 00FC0C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFC04U: goto L_00FC04;
        case 0xFC0CU: goto L_00FC0C;
        }
    }
}

/* 00FC0E  game routine */
void sub_00FC0E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0266, 0x4420);                                /* 00FC0E  move.w  #$4420,$266(a5) */
    wr16(V_0268, 0x16);                                  /* 00FC14  move.w  #$16,$268(a5) */
    wr16(V_026A, 0x16);                                  /* 00FC1A  move.w  #$16,$26a(a5) */
    d0 = rd32(V_PLAYER2);                                /* 00FC20  move.l  $3ce(a5),d0 */
    CALL(sub_007D3E, 0xFC28U);                           /* 00FC24  bsr  $7d3e */
L_00FC28:
    a0 = V_025F;                                         /* 00FC28  lea  $25f(a5),a0 */
    CALL(sub_008928, 0xFC30U);                           /* 00FC2C  bsr  $8928 */
L_00FC30:
    RTS();                                               /* 00FC30  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFC28U: goto L_00FC28;
        case 0xFC30U: goto L_00FC30;
        }
    }
}

/* 00FC32  game routine */
void sub_00FC32(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00FB4A, 0xFC36U);                           /* 00FC32  bsr  $fb4a */
L_00FC36:
    /* 00FC36  tst.w  $2da(a5) */
    /* 00FC3A  beq  $fc44 */
    if (rd16(V_02DA) != 0) {
        CALL(sub_00FB66, 0xFC40U);                           /* 00FC3C  bsr  $fb66 */
L_00FC40:
        CALL(sub_00FBA8, 0xFC44U);                           /* 00FC40  bsr  $fba8 */
    }
L_00FC44:
    /* 00FC44  tst.w  $376(a5) */
    /* 00FC48  beq  $fc4e */
    if (rd16(V_0376) != 0) {
        CALL(sub_00FBCC, 0xFC4CU);                           /* 00FC4A  bsr  $fbcc */
L_00FC4C:
        CALL(sub_00FC0E, 0xFC4EU);                           /* 00FC4C  bsr  $fc0e */
    }
    for (;;) {
        BUDGET_CHECK(0xFC4EU);
L_00FC4E:
        /* 00FC4E  tst.w  $1da(a5) */
        /* 00FC52  beq  $fc5e */
        if (rd16(V_01DA) == 0) {
            goto L_00FC5E;
        }
        CALL(sub_0086D8, 0xFC58U);                           /* 00FC54  bsr  $86d8 */
L_00FC58:
        /* 00FC58  subq.w  #1,$1da(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01DA);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_01DA, r);
            fx = (s > d);
        }
        /* 00FC5C  bra  $fc4e */
    }
L_00FC5E:
    RTS();                                               /* 00FC5E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFC36U: goto L_00FC36;
        case 0xFC40U: goto L_00FC40;
        case 0xFC44U: goto L_00FC44;
        case 0xFC4CU: goto L_00FC4C;
        case 0xFC4EU: goto L_00FC4E;
        case 0xFC58U: goto L_00FC58;
        }
    }
}

/* 00FC60  game routine */
void sub_00FC60(void)
{
    do {
        BUDGET_CHECK(0xFC60U);
        /* 00FC60  subq.l  #1,d0 */
        {
            uint32_t s = 1;
            uint32_t d = d0;
            uint32_t r = (d - s) & 0xFFFFFFFF;
            d0 = r;
            fz = (r == 0);
            fx = (s > d);
        }
        /* 00FC62  bne  $fc60 */
    } while (COND(6));
    RTS();                                               /* 00FC64  rts */
}

/* 00FC66  game routine */
void sub_00FC66(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d7, 0xF9F);                                     /* 00FC66  move.w  #$f9f,d7 */
    do {
        BUDGET_CHECK(0xFC6AU);
L_00FC6A:
        /* 00FC6A  movem.w  (a0),d0-d3 */
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
        }
        SETW(d4, (d3 & 0xFFFF));                             /* 00FC6E  move.w  d3,d4 */
        SETW(d4, d4 & (d2 & 0xFFFF));                        /* 00FC70  and.w  d2,d4 */
        SETW(d4, d4 & (d1 & 0xFFFF));                        /* 00FC72  and.w  d1,d4 */
        SETW(d0, ~(d0 & 0xFFFF));                            /* 00FC74  not.w  d0 */
        SETW(d4, d4 & (d0 & 0xFFFF));                        /* 00FC76  and.w  d0,d4 */
        SETW(d4, ~(d4 & 0xFFFF));                            /* 00FC78  not.w  d4 */
        /* 00FC7A  and.w  d4,(a0)+ */
        {
            uint32_t s = (d4 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d & s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 00FC7C  and.w  d4,(a0)+ */
        {
            uint32_t s = (d4 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d & s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 00FC7E  and.w  d4,(a0)+ */
        {
            uint32_t s = (d4 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d & s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 00FC80  and.w  d4,(a0)+ */
        {
            uint32_t s = (d4 & 0xFFFF);
            uint32_t ea = a0;
            a0 += 2;
            uint32_t d = rd16(ea);
            uint32_t r = (d & s) & 0xFFFF;
            wr16(ea, r);
        }
        /* 00FC82  dbra  d7,$fc6a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 00FC86  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFC6AU: goto L_00FC6A;
        }
    }
}

/* 00FC88  game routine */
void sub_00FC88(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00FD20, 0xFC8CU);                           /* 00FC88  bsr  $fd20 */
L_00FC8C:
    /* 00FC8C  bra  $fcc2 */
    goto L_00FCC2;
L_00FC90:
    /* 00FC90  tst.w  d5 */
    /* 00FC92  bne  $fca8 */
    if ((d5 & 0xFFFF) == 0) {
L_00FC94:
        d0 = 0;                                              /* 00FC94  moveq  #0,d0 */
        SETB(d0, (d2 & 0xFF));                               /* 00FC96  move.b  d2,d0 */
        /* 00FC98  cmp.w  #$90,d0 */
        /* 00FC9C  bne  $fca2 */
        if ((d0 & 0xFFFF) == 0x90) {
L_00FC9E:
            d5 = 1;                                              /* 00FC9E  moveq  #1,d5 */
            /* 00FCA0  bra  $fcc2 */
            goto L_00FCC2;
        }
L_00FCA2:
        SETW(d6, (d0 & 0xFFFF));                             /* 00FCA2  move.w  d0,d6 */
        /* 00FCA4  move.b  d0,(a6)+ */
        {
            uint32_t v = (d0 & 0xFF);
            wr8(a6, v);
            a6 += 1;
        }
        /* 00FCA6  bra  $fcc2 */
    } else {
L_00FCA8:
        /* 00FCA8  cmp.w  #$1,d5 */
        /* 00FCAC  bne  $fcc2 */
        if ((d5 & 0xFFFF) == 1) {
L_00FCAE:
            /* 00FCAE  move.w  d2,d0 */
            /* 00FCB0  beq  $fcbc */
            SETW(d0, (d2 & 0xFFFF));
            if ((d0 & 0xFFFF) != 0) {
L_00FCB2:
                /* 00FCB2  bra  $fcb6 */
                goto L_00FCB6;
L_00FCB4:
                /* 00FCB4  move.b  d6,(a6)+ */
                {
                    uint32_t v = (d6 & 0xFF);
                    wr8(a6, v);
                    a6 += 1;
                }
L_00FCB6:
                /* 00FCB6  subq.w  #1,d2 */
                {
                    uint32_t s = 1;
                    uint32_t d = (d2 & 0xFFFF);
                    uint32_t r = (d - s) & 0xFFFF;
                    SETW(d2, r);
                    fz = (r == 0);
                }
                /* 00FCB8  bne  $fcb4 */
                if (COND(6)) {
                    BUDGET_CHECK(0xFCB4U);
                    goto L_00FCB4;
                }
L_00FCBA:
                /* 00FCBA  bra  $fcc0 */
            } else {
L_00FCBC:
                /* 00FCBC  move.b  #$90,(a6)+ */
                {
                    uint32_t v = 0x90;
                    wr8(a6, v);
                    a6 += 1;
                }
            }
L_00FCC0:
            SETW(d5, 0);                                         /* 00FCC0  clr.w  d5 */
        }
    }
L_00FCC2:
    d4 = 0;                                              /* 00FCC2  moveq  #0,d4 */
    /* 00FCC4  bra  $fcfe */
    goto L_00FCFE;
L_00FCC6:
    a2 += 1;                                             /* 00FCC6  addq.w  #1,a2 */
    SETW(d0, (a2 & 0xFFFF));                             /* 00FCC8  move.w  a2,d0 */
    /* 00FCCA  cmpi.w  #$7,d0 */
    /* 00FCCE  ble  $fcec */
    if ((int16_t)(d0 & 0xFFFF) > (int16_t)7) {
L_00FCD0:
        d0 = 0;                                              /* 00FCD0  moveq  #0,d0 */
L_00FCD2:
        /* 00FCD2  move.b  (a4)+,d0 */
        SETB(d0, rd8(a4));
        a4 += 1;
        SETW(d3, (d0 & 0xFFFF));                             /* 00FCD4  move.w  d0,d3 */
        a2 -= SX16((a2 & 0xFFFF));                           /* 00FCD6  suba.w  a2,a2 */
        SETW(d1, (d4 & 0xFFFF));                             /* 00FCD8  move.w  d4,d1 */
        SETW(d1, (d1 & 0xFFFF) << 2);                        /* 00FCDA  asl.w  #2,d1 */
        fz = !(d3 & 0x1);                                    /* 00FCDC  btst  #$0,d3 */
        /* 00FCE0  beq  $fce4 */
        if (COND(6)) {
L_00FCE2:
            SETW(d1, d1 + 2);                                    /* 00FCE2  addq.w  #2,d1 */
        }
L_00FCE4:
        a0 = a3;                                             /* 00FCE4  movea.l  a3,a0 */
        SETW(d4, rd16(a0 + SX16(d1)));                       /* 00FCE6  move.w  $0(a0,d1.w),d4 */
        /* 00FCEA  bra  $fcfe */
    } else {
L_00FCEC:
        SETW(d3, (uint32_t)((int32_t)SX16((d3 & 0xFFFF)) >> 1)); /* 00FCEC  asr.w  #1,d3 */
        SETW(d4, (d4 & 0xFFFF) << 2);                        /* 00FCEE  asl.w  #2,d4 */
        a0 = a3;                                             /* 00FCF0  movea.l  a3,a0 */
        fz = !(d3 & 0x1);                                    /* 00FCF2  btst  #$0,d3 */
        /* 00FCF6  beq  $fcfa */
        if (COND(6)) {
L_00FCF8:
            SETW(d4, d4 + 2);                                    /* 00FCF8  addq.w  #2,d4 */
        }
L_00FCFA:
        SETW(d4, rd16(a0 + SX16(d4)));                       /* 00FCFA  move.w  $0(a0,d4.w),d4 */
    }
L_00FCFE:
    /* 00FCFE  tst.w  d4 */
    /* 00FD00  bpl  $fcc6 */
    if ((int16_t)(d4 & 0xFFFF) >= 0) {
        BUDGET_CHECK(0xFCC6U);
        goto L_00FCC6;
    }
L_00FD02:
    SETW(d4, d4 + 1);                                    /* 00FD02  addq.w  #1,d4 */
    /* 00FD04  neg.w  d4 */
    { uint32_t d, r, ea; (void)ea;
    d = (d4 & 0xFFFFU);
    r = (0 - d) & 0xFFFFU;
    d4 = (d4 & 0xFFFF0000U) | ((r) & 0xFFFFU);
    FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
    /* 00FD06  cmpi.w  #$100,d4 */
    /* 00FD0A  bne  $fd10 */
    if ((d4 & 0xFFFF) == 0x100) {
L_00FD0C:
        d0 = 0xFFFFFFFF;                                     /* 00FD0C  moveq  #-1,d0 */
        /* 00FD0E  bra  $fd12 */
    } else {
L_00FD10:
        SETW(d0, (d4 & 0xFFFF));                             /* 00FD10  move.w  d4,d0 */
    }
L_00FD12:
    SETW(d4, (d0 & 0xFFFF));                             /* 00FD12  move.w  d0,d4 */
    SETW(d2, (d0 & 0xFFFF));                             /* 00FD14  move.w  d0,d2 */
    /* 00FD16  cmpi.w  #$ffff,d0 */
    /* 00FD1A  bne  $fc90 */
    if ((d0 & 0xFFFF) != 0xFFFF) {
        BUDGET_CHECK(0xFC90U);
        goto L_00FC90;
    }
L_00FD1E:
    RTS();                                               /* 00FD1E  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFC8CU: goto L_00FC8C;
        case 0xFC90U: goto L_00FC90;
        case 0xFC94U: goto L_00FC94;
        case 0xFC9EU: goto L_00FC9E;
        case 0xFCA2U: goto L_00FCA2;
        case 0xFCA8U: goto L_00FCA8;
        case 0xFCAEU: goto L_00FCAE;
        case 0xFCB2U: goto L_00FCB2;
        case 0xFCB4U: goto L_00FCB4;
        case 0xFCB6U: goto L_00FCB6;
        case 0xFCBAU: goto L_00FCBA;
        case 0xFCBCU: goto L_00FCBC;
        case 0xFCC0U: goto L_00FCC0;
        case 0xFCC2U: goto L_00FCC2;
        case 0xFCC6U: goto L_00FCC6;
        case 0xFCD0U: goto L_00FCD0;
        case 0xFCD2U: goto L_00FCD2;
        case 0xFCE2U: goto L_00FCE2;
        case 0xFCE4U: goto L_00FCE4;
        case 0xFCECU: goto L_00FCEC;
        case 0xFCF8U: goto L_00FCF8;
        case 0xFCFAU: goto L_00FCFA;
        case 0xFCFEU: goto L_00FCFE;
        case 0xFD02U: goto L_00FD02;
        case 0xFD0CU: goto L_00FD0C;
        case 0xFD10U: goto L_00FD10;
        case 0xFD12U: goto L_00FD12;
        case 0xFD1EU: goto L_00FD1E;
        }
    }
}

/* 00FD20  game routine */
void sub_00FD20(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a2 = SX16(0x63);                                     /* 00FD20  movea.w  #$63,a2 */
    SETW(d5, 0);                                         /* 00FD24  clr.w  d5 */
    CALL(sub_00FD72, 0xFD2AU);                           /* 00FD26  bsr  $fd72 */
L_00FD2A:
    wr16(V_01D6, (d0 & 0xFFFF));                         /* 00FD2A  move.w  d0,$1d6(a5) */
    wr16(a3, 0xFEFF);                                    /* 00FD2E  move.w  #$feff,(a3) */
    wr16(a3 + 0x2, 0xFEFF);                              /* 00FD32  move.w  #$feff,$2(a3) */
    wr16(V_01D4, 0);                                     /* 00FD38  clr.w  $1d4(a5) */
    /* 00FD3C  bra  $fd64 */
    goto L_00FD64;
L_00FD3E:
    CALL(sub_00FD72, 0xFD42U);                           /* 00FD3E  bsr  $fd72 */
L_00FD42:
    SETW(d1, rd16(V_01D4));                              /* 00FD42  move.w  $1d4(a5),d1 */
    d1 = d1 << 2;                                        /* 00FD46  asl.l  #2,d1 */
    a0 = a3;                                             /* 00FD48  movea.l  a3,a0 */
    a0 += SX16((d1 & 0xFFFF));                           /* 00FD4A  adda.w  d1,a0 */
    wr16(a0, (d0 & 0xFFFF));                             /* 00FD4C  move.w  d0,(a0) */
    CALL(sub_00FD72, 0xFD52U);                           /* 00FD4E  bsr  $fd72 */
L_00FD52:
    SETW(d1, rd16(V_01D4));                              /* 00FD52  move.w  $1d4(a5),d1 */
    d1 = d1 << 2;                                        /* 00FD56  asl.l  #2,d1 */
    a0 = a3;                                             /* 00FD58  movea.l  a3,a0 */
    a0 += SX16((d1 & 0xFFFF));                           /* 00FD5A  adda.w  d1,a0 */
    wr16(a0 + 0x2, (d0 & 0xFFFF));                       /* 00FD5C  move.w  d0,$2(a0) */
    wr16(V_01D4, rd16(V_01D4) + 1);                      /* 00FD60  addq.w  #1,$1d4(a5) */
L_00FD64:
    SETW(d0, rd16(V_01D4));                              /* 00FD64  move.w  $1d4(a5),d0 */
    /* 00FD68  cmp.w  $1d6(a5),d0 */
    /* 00FD6C  blt  $fd3e */
    if ((int16_t)(d0 & 0xFFFF) < (int16_t)rd16(V_01D6)) {
        BUDGET_CHECK(0xFD3EU);
        goto L_00FD3E;
    }
L_00FD6E:
    d0 = 0;                                              /* 00FD6E  moveq  #0,d0 */
    RTS();                                               /* 00FD70  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFD2AU: goto L_00FD2A;
        case 0xFD3EU: goto L_00FD3E;
        case 0xFD42U: goto L_00FD42;
        case 0xFD52U: goto L_00FD52;
        case 0xFD64U: goto L_00FD64;
        case 0xFD6EU: goto L_00FD6E;
        }
    }
}

/* 00FD72  game routine */
void sub_00FD72(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00FD72  cmpa.l  a1,a4 */
    /* 00FD74  bcs  $fd7a */
    if (a4 >= a1) {
        d0 = 0xFFFFFFFF;                                     /* 00FD76  moveq  #-1,d0 */
        RTS();                                               /* 00FD78  rts */
    }
L_00FD7A:
    /* 00FD7A  move.b  (a4)+,d1 */
    SETB(d1, rd8(a4));
    a4 += 1;
    /* 00FD7C  move.b  (a4)+,d0 */
    SETB(d0, rd8(a4));
    a4 += 1;
    SETW(d0, (d0 & 0xFFFF) << 8);                        /* 00FD7E  lsl.w  #8,d0 */
    SETB(d0, (d1 & 0xFF));                               /* 00FD80  move.b  d1,d0 */
    RTS();                                               /* 00FD82  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFD7AU: goto L_00FD7A;
        }
    }
}

/* 00FE1E  game routine */
void sub_00FE1E(void)
{
    /* 00FE1E  move.w  -(a0),-(a0) */
    a0 -= 2;
    {
        uint32_t v = rd16(a0);
        a0 -= 2;
        wr16(a0, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    MISS(0xFE20U);
}

/* 00FF14  game routine */
void sub_00FF14(void)
{
    SETB(d5, d5 | 9);                                    /* 00FF14  ori.b  #$9,d5 */
    wr32(a0, rd32(a0) | 0x35602);                        /* 00FF18  ori.l  #$35602,(a0) */
    wr8(a0 + 0x41, rd8(a0 + 0x41) | 1);                  /* 00FF1E  ori.b  #$1,$41(a0) */
    SETB(d5, d5 | 0x7E);                                 /* 00FF24  ori.b  #$7e,d5 */
    wr8(a7 + 0xA, rd8(a7 + 0xA) | 1);                    /* 00FF28  ori.b  #$1,$a(a7) */
    /* 00FF2E  ori.b  #$ee,d5 */
    {
        uint32_t s = 0xEE;
        uint32_t d = (d5 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d5, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xFF32U);
}

/* 00FF3C  game routine */
void sub_00FF3C(void)
{
    /* 00FF3C  move.w  (a0)+,$168(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0168, v);
    }
    /* 00FF40  move.w  (a0)+,$16a(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_016A, v);
    }
    /* 00FF44  move.w  (a0)+,$276(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0276, v);
    }
    /* 00FF48  move.l  (a0)+,$16c(a5) */
    {
        uint32_t v = rd32(a0);
        a0 += 4;
        wr32(V_016C, v);
    }
    /* 00FF4C  bra  $86b0 */
    { sub_0086B0(); return; }
}

/* 00FF62  game routine */
void sub_00FF62(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0xFF62U);
        /* 00FF62  movem.w  (a1),d0-d7 */
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
        d4 = SX16(rd16(ea));
        ea += 2;
        d5 = SX16(rd16(ea));
        ea += 2;
        d6 = SX16(rd16(ea));
        ea += 2;
        d7 = SX16(rd16(ea));
        ea += 2;
        }
        /* 00FF66  move.w  d0,(a1)+ */
        {
            uint32_t v = (d0 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF68  move.w  d4,(a1)+ */
        {
            uint32_t v = (d4 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF6A  move.w  d1,(a1)+ */
        {
            uint32_t v = (d1 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF6C  move.w  d5,(a1)+ */
        {
            uint32_t v = (d5 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF6E  move.w  d2,(a1)+ */
        {
            uint32_t v = (d2 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF70  move.w  d6,(a1)+ */
        {
            uint32_t v = (d6 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF72  move.w  d3,(a1)+ */
        {
            uint32_t v = (d3 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF74  move.w  d7,(a1)+ */
        {
            uint32_t v = (d7 & 0xFFFF);
            wr16(a1, v);
            a1 += 2;
        }
        /* 00FF76  subq.w  #1,$276(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0276);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0276, r);
            fz = (r == 0);
            fx = (s > d);
        }
        /* 00FF7A  bne  $ff62 */
    } while (COND(6));
L_00FF7C:
    RTS();                                               /* 00FF7C  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0xFF7CU: goto L_00FF7C;
        }
    }
}

/* 00FF8C  game routine */
void sub_00FF8C(void)
{
    SETB(d0, d0 | 0x41);                                 /* 00FF8C  ori.b  #$41,d0 */
    SETB(d0, d0 | 0xBE);                                 /* 00FF90  ori.b  #$be,d0 */
    /* 00FF94  ori.b  #$e0,-(a0) */
    {
        uint32_t s = 0xE0;
        a0 -= 1;
        uint32_t d = rd8(a0);
        uint32_t r = (d | s) & 0xFF;
        wr8(a0, r);
    }
    SETB(d0, d0 | 0);                                    /* 00FF98  ori.b  #$0,d0 */
    SETB(d0, d0 | 0x3F);                                 /* 00FF9C  ori.b  #$3f,d0 */
    SETB(d0, d0 | 0xC0);                                 /* 00FFA0  ori.b  #$c0,d0 */
    /* 00FFA4  ori.b  #$18,(a0)+ */
    {
        uint32_t s = 0x18;
        uint32_t ea = a0;
        a0 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    /* 00FFA8  ori.b  #$e0,d0 */
    {
        uint32_t s = 0xE0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0xFFACU);
}

