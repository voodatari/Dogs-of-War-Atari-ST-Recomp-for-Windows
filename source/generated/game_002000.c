/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 0020EE  game routine */
void sub_0020EE(void)
{
    /* 0020EE  ori.b  #$0,d0 */
    {
        uint32_t s = 0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d0, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x20F2U);
}

/* 002110  game routine */
void sub_002110(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 002110  cmp.b  #$3b,d0 */
    {
        uint32_t s = 0x3B;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d - s) & 0xFF;
        fz = (r == 0);
    }
    /* 002114  jmp  $7a6a.w */
L_007A6A:
    /* 007A6A  beq  $373a */
    if (COND(7)) { sub_00373A(); return; }
L_007A6E:
    /* 007A6E  tst.w  $252(a5) */
    /* 007A72  beq  $7aba */
    if (rd16(V_0252) != 0) {
        /* 007A76  cmp.b  #$44,d0 */
        /* 007A7A  bne  $7a80 */
        if ((d0 & 0xFF) == 0x44) {
            wr8(V_026E, 0xFF);                                   /* 007A7C  st  $26e(a5) */
        }
        /* 007A80  cmp.b  #$3f,d0 */
        /* 007A84  bne  $7a8c */
        if ((d0 & 0xFF) == 0x3F) {
            wr8(V_0254, rd8(V_0254) ^ 0xFF);                     /* 007A86  eori.b  #$ff,$254(a5) */
        }
        /* 007A8C  cmp.b  #$3e,d0 */
        /* 007A90  bne  $7a9a */
        if ((d0 & 0xFF) == 0x3E) {
            wr8(0xFF820A, rd8(0xFF820A) ^ 0x2);                  /* 007A92  bchg  #$1,$ff820a.l */
        }
        /* 007A9A  cmp.b  #$c4,d0 */
        /* 007A9E  bne  $7aa4 */
        if ((d0 & 0xFF) == 0xC4) {
            wr16(V_026E, 0);                                     /* 007AA0  clr.w  $26e(a5) */
        }
        /* 007AA4  cmp.b  #$41,d0 */
        /* 007AA8  bne  $7ab8 */
        if ((d0 & 0xFF) == 0x41) {
            a7 += 4;                                             /* 007AAC  addq.l  #4,a7 */
            /* 007AAE  movem.l  (a7)+,d0-d1/a0/a4 */
            d0 = POP32();
            d1 = POP32();
            a0 = POP32();
            a4 = POP32();
            wr16(a0 + 0x2, rd16(a0 + 0x1));                      /* 007AB2  move.w  $1(a0),$2(a0) */
        }
        RTS();                                               /* 007AB8  rts */
    }
L_007ABA:
    fz = !(d0 & 0x80);                                   /* 007ABA  btst  #$7,d0 */
    /* 007ABE  bne  $7ad8 */
    if (COND(7)) {
L_007AC0:
        a0 = rd32(V_0256);                                   /* 007AC0  movea.l  $256(a5),a0 */
        /* 007AC4  cmp.b  (a0)+,d0 */
        {
            uint32_t s = rd8(a0);
            a0 += 1;
            uint32_t d = (d0 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            fz = (r == 0);
        }
        /* 007AC6  beq  $7acc */
        if (COND(6)) {
L_007AC8:
            a0 = 0x8258;                                         /* 007AC8  lea  $8258(pc),a0 */
        }
        wr32(V_0256, a0);                                    /* 007ACC  move.l  a0,$256(a5) */
        /* 007AD0  tst.b  (a0) */
        /* 007AD2  bne  $7ad8 */
        if (rd8(a0) == 0) {
            wr8(V_0252, 0xFF);                                   /* 007AD4  st  $252(a5) */
        }
    }
L_007AD8:
    RTS();                                               /* 007AD8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x7A6AU: goto L_007A6A;
        case 0x7A6EU: goto L_007A6E;
        case 0x7ABAU: goto L_007ABA;
        case 0x7AC0U: goto L_007AC0;
        case 0x7AC8U: goto L_007AC8;
        case 0x7AD8U: goto L_007AD8;
        }
    }
}

/* 002120  game routine */
void sub_002120(void)
{
    /* 002120  movem.l  a1/a0,-(a7) */
    PUSH32(a1);
    PUSH32(a0);
    a0 = rd32(0x20FA);                                   /* 002124  movea.l  $20fa(pc),a0 */
    a0 -= 8;                                             /* 002128  subq.l  #8,a0 */
    /* 00212A  cmpi.l  #$6356616c,(a0)+ */
    {
        uint32_t s = 0x6356616C;
        uint32_t ea = a0;
        a0 += 4;
        uint32_t d = rd32(ea);
        uint32_t r = (d - s) & 0xFFFFFFFF;
        fz = (r == 0);
    }
    /* 002130  bne  $2138 */
    if (COND(7)) {
        a1 = 0x20EE;                                         /* 002132  lea  $20ee(pc),a1 */
        wr32(a1, rd32(a0));                                  /* 002136  move.l  (a0),(a1) */
    }
    /* 002138  movem.l  (a7)+,a0-a1 */
    a0 = POP32();
    a1 = POP32();
    /* 00213C  bra  $2110 */
    { sub_002110(); return; }
}

/* 0021D4  game routine */
void sub_0021D4(void)
{
    /* 0021D4  ori.w  #$808,d5 */
    {
        uint32_t s = 0x808;
        uint32_t d = (d5 & 0xFFFF);
        uint32_t r = (d | s) & 0xFFFF;
        SETW(d5, r);
        FL_LOGIC(r, 0xFFFF, 0x8000);
    }
    MISS(0x21D8U);
}

/* 002C00  game routine */
void sub_002C00(void)
{
    a5 -= a5;                                            /* 002C00  suba.l  a5,a5 */
    a7 = 0x2400;                                         /* 002C02  lea  $2400.l,a7 */
    /* 002C08  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
    /* 002C0C  bra  $890c */
    /* 00890C  bra  $1088e */
    { sub_01088E(); return; }
}

/* 002D9A  game routine */
void sub_002D9A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    CALL(sub_00360C, 0x2D9EU);                           /* 002D9A  bsr  $360c */
L_002D9E:
    wr32(V_01DC, a0);                                    /* 002D9E  move.l  a0,$1dc(a5) */
    a1 = 0x35CE;                                         /* 002DA2  lea  $35ce(pc),a1 */
    CALL(sub_0074F0, 0x2DAAU);                           /* 002DA6  bsr  $74f0 */
L_002DAA:
    d0 = 5;                                              /* 002DAA  moveq  #5,d0 */
    CALL(sub_0074F4, 0x2DB0U);                           /* 002DAC  bsr  $74f4 */
L_002DB0:
    wr16(V_027A, 0);                                     /* 002DB0  clr.w  $27a(a5) */
    CALL(sub_002DE6, 0x2DB6U);                           /* 002DB4  bsr  $2de6 */
L_002DB6:
    SETW(d0, rd16(V_01C6));                              /* 002DB6  move.w  $1c6(a5),d0 */
    /* 002DBA  cmp.w  #$8,d0 */
    /* 002DBE  bcc  $2dda */
    if ((d0 & 0xFFFF) < 8) {
        SETW(d1, rd16(V_0230));                              /* 002DC0  move.w  $230(a5),d1 */
        SETW(d1, (d1 & 0xFFFF) << 1);                        /* 002DC4  lsl.w  #1,d1 */
        a0 = HW_PALETTE;                                     /* 002DC6  lea  $ff8240.l,a0 */
        a0 += SX16((d1 & 0xFFFF));                           /* 002DCC  adda.w  d1,a0 */
        SETW(d0, (d0 & 0xFFFF) << 1);                        /* 002DCE  lsl.w  #1,d0 */
        a1 = 0x35D4;                                         /* 002DD0  lea  $35d4(pc),a1 */
        SETW(d0, rd16(a1 + SX16(d0)));                       /* 002DD4  move.w  $0(a1,d0.w),d0 */
        wr16(a0, (d0 & 0xFFFF));                             /* 002DD8  move.w  d0,(a0) */
    }
    /* 002DDA  addq.w  #1,$27a(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_027A);
        uint32_t r = (d + s) & 0xFFFF;
        wr16(V_027A, r);
        fx = (r < s);
    }
    CALL(sub_002DE6, 0x2DE0U);                           /* 002DDE  bsr  $2de6 */
L_002DE0:
    CALL(sub_002E44, 0x2DE4U);                           /* 002DE0  bsr  $2e44 */
L_002DE4:
    RTS();                                               /* 002DE4  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2D9EU: goto L_002D9E;
        case 0x2DAAU: goto L_002DAA;
        case 0x2DB0U: goto L_002DB0;
        case 0x2DB6U: goto L_002DB6;
        case 0x2DE0U: goto L_002DE0;
        case 0x2DE4U: goto L_002DE4;
        }
    }
}

/* 002DE6  game routine */
void sub_002DE6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_002DE6;
    for (;;) {
        BUDGET_CHECK(0x2C80U);
L_002C80:
        /* 002C80  cmpi.w  #$7,$1c6(a5) */
        /* 002C86  bhi  $7de2 */
        if (rd16(V_01C6) > 7) {
            { sub_007DE2(); return; }
        }
        a1 = 0x3094;                                         /* 002C8A  lea  $3094(pc),a1 */
        a2 = 0x3172;                                         /* 002C8E  lea  $3172(pc),a2 */
        a0 = 0x28000;                                        /* 002C92  lea  $28000.l,a0 */
        do {
            BUDGET_CHECK(0x2C98U);
L_002C98:
            /* 002C98  move.w  (a1)+,(a0)+ */
            {
                uint32_t v = rd16(a1);
                a1 += 2;
                wr16(a0, v);
                a0 += 2;
            }
            /* 002C9A  cmpa.l  a2,a1 */
            /* 002C9C  bcs  $2c98 */
        } while (a1 < a2);
        /* 002C9E  move.l  $170(a5),(a0)+ */
        {
            uint32_t v = rd32(V_0170);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CA2  move.l  $174(a5),(a0)+ */
        {
            uint32_t v = rd32(V_0174);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CA6  move.l  $178(a5),(a0)+ */
        {
            uint32_t v = rd32(V_0178);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CAA  move.l  $17c(a5),(a0)+ */
        {
            uint32_t v = rd32(V_017C);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CAE  move.l  $180(a5),(a0)+ */
        {
            uint32_t v = rd32(V_0180);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CB2  move.l  $184(a5),(a0)+ */
        {
            uint32_t v = rd32(V_0184);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CB6  move.l  $188(a5),(a0)+ */
        {
            uint32_t v = rd32(V_0188);
            wr32(a0, v);
            a0 += 4;
        }
        /* 002CBA  move.l  $18c(a5),(a0)+ */
        {
            uint32_t v = rd32(V_018C);
            wr32(a0, v);
            a0 += 4;
        }
        wr16(V_0168, 0);                                     /* 002CBE  move.w  #$0,$168(a5) */
        wr16(V_016A, 2);                                     /* 002CC4  move.w  #$2,$16a(a5) */
        wr16(V_0276, 1);                                     /* 002CCA  move.w  #$1,$276(a5) */
        wr32(V_016C, 0x28000);                               /* 002CD0  move.l  #$28000,$16c(a5) */
        CALL(sub_0086B4, 0x2CDCU);                           /* 002CD8  bsr  $86b4 */
L_002CDC:
        /* 002CDC  bra  $86dc */
        goto L_0086DC;
L_002DE6:
        SETW(d1, rd16(V_027A));                              /* 002DE6  move.w  $27a(a5),d1 */
        d1 = (d1 & 0xFFFF) * 0x9C;                           /* 002DEA  mulu.w  #$9c,d1 */
        a6 = V_02DA;                                         /* 002DEE  lea  $2da(a5),a6 */
        d0 = rd32(a6 + 0x8 + SX16(d1));                      /* 002DF2  move.l  $8(a6,d1.w),d0 */
        wr32(V_01E4, d0);                                    /* 002DF6  move.l  d0,$1e4(a5) */
        CALL(sub_003054, 0x2DFEU);                           /* 002DFA  bsr  $3054 */
L_002DFE:
        /* 002DFE  cmpi.w  #$7,$1c6(a5) */
        /* 002E04  bhi  $7de2 */
        if (rd16(V_01C6) > 7) {
            { sub_007DE2(); return; }
        }
        CALL(sub_002FAC, 0x2E0CU);                           /* 002E08  bsr  $2fac */
L_002E0C:
        CALL(sub_002E44, 0x2E10U);                           /* 002E0C  bsr  $2e44 */
L_002E10:
        wr16(V_0266, 0x7760);                                /* 002E10  move.w  #$7760,$266(a5) */
        wr16(V_0268, 8);                                     /* 002E16  move.w  #$8,$268(a5) */
        wr16(V_026A, 8);                                     /* 002E1C  move.w  #$8,$26a(a5) */
        wr16(V_026C, 0xC);                                   /* 002E22  move.w  #$c,$26c(a5) */
        a0 = 0x34D8;                                         /* 002E28  lea  $34d8(pc),a0 */
        CALL(sub_008584, 0x2E30U);                           /* 002E2C  bsr  $8584 */
L_002E30:
        SETW(d0, rd16(V_027A));                              /* 002E30  move.w  $27a(a5),d0 */
        /* 002E34  addi.w  #$31,d0 */
        {
            uint32_t s = 0x31;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        CALL(sub_00858E, 0x2E3CU);                           /* 002E38  bsr  $858e */
L_002E3C:
        CALL(sub_003000, 0x2E40U);                           /* 002E3C  bsr  $3000 */
L_002E40:
        /* 002E40  bra  $2c80 */
    }
L_0086DC:
    /* 0086DC  bra  $e8b2 */
    { sub_00E8B2(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2C80U: goto L_002C80;
        case 0x2C98U: goto L_002C98;
        case 0x2CDCU: goto L_002CDC;
        case 0x2DFEU: goto L_002DFE;
        case 0x2E0CU: goto L_002E0C;
        case 0x2E10U: goto L_002E10;
        case 0x2E30U: goto L_002E30;
        case 0x2E3CU: goto L_002E3C;
        case 0x2E40U: goto L_002E40;
        }
    }
}

/* 002E44  game routine */
void sub_002E44(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    wr16(V_0232, 0);                                     /* 002E44  clr.w  $232(a5) */
    CALL(sub_007626, 0x2E4CU);                           /* 002E48  bsr  $7626 */
L_002E4C:
    CALL(sub_007558, 0x2E50U);                           /* 002E4C  bsr  $7558 */
L_002E50:
    wr16(V_0266, 0x320);                                 /* 002E50  move.w  #$320,$266(a5) */
    wr16(V_0268, 0);                                     /* 002E56  move.w  #$0,$268(a5) */
    wr16(V_026A, 0);                                     /* 002E5C  move.w  #$0,$26a(a5) */
    wr16(V_026C, 0xC);                                   /* 002E62  move.w  #$c,$26c(a5) */
    a0 = rd32(V_01DC);                                   /* 002E68  movea.l  $1dc(a5),a0 */
    CALL(sub_008584, 0x2E70U);                           /* 002E6C  bsr  $8584 */
L_002E70:
    wr16(V_0266, 0x12C0);                                /* 002E70  move.w  #$12c0,$266(a5) */
    wr16(V_0268, 4);                                     /* 002E76  move.w  #$4,$268(a5) */
    wr16(V_026A, 4);                                     /* 002E7C  move.w  #$4,$26a(a5) */
    wr16(V_026C, 1);                                     /* 002E82  move.w  #$1,$26c(a5) */
    wr32(V_01C2, 0x310D0D00);                            /* 002E88  move.l  #$310d0d00,$1c2(a5) */
    do {
        BUDGET_CHECK(0x2E90U);
L_002E90:
        a0 = V_01C2;                                         /* 002E90  lea  $1c2(a5),a0 */
        CALL(sub_008584, 0x2E98U);                           /* 002E94  bsr  $8584 */
L_002E98:
        /* 002E98  addq.b  #1,$1c2(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd8(V_01C2);
            uint32_t r = (d + s) & 0xFF;
            wr8(V_01C2, r);
            fx = (r < s);
        }
        /* 002E9C  cmpi.b  #$39,$1c2(a5) */
        /* 002EA2  bne  $2e90 */
    } while (rd8(V_01C2) != 0x39);
    wr16(V_0266, 0x12C0);                                /* 002EA4  move.w  #$12c0,$266(a5) */
    wr16(V_0268, 7);                                     /* 002EAA  move.w  #$7,$268(a5) */
    wr16(V_026A, 7);                                     /* 002EB0  move.w  #$7,$26a(a5) */
    wr16(V_026C, 3);                                     /* 002EB6  move.w  #$3,$26c(a5) */
    a0 = 0x3094;                                         /* 002EBC  lea  $3094(pc),a0 */
    CALL(sub_008584, 0x2EC4U);                           /* 002EC0  bsr  $8584 */
L_002EC4:
    a0 = 0x35CC;                                         /* 002EC4  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2ECCU);                           /* 002EC8  bsr  $8584 */
L_002ECC:
    wr16(V_026C, 4);                                     /* 002ECC  move.w  #$4,$26c(a5) */
    a0 = 0x30AA;                                         /* 002ED2  lea  $30aa(pc),a0 */
    CALL(sub_008584, 0x2EDAU);                           /* 002ED6  bsr  $8584 */
L_002EDA:
    a0 = 0x35CC;                                         /* 002EDA  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2EE2U);                           /* 002EDE  bsr  $8584 */
L_002EE2:
    wr16(V_026C, 5);                                     /* 002EE2  move.w  #$5,$26c(a5) */
    a0 = 0x30C0;                                         /* 002EE8  lea  $30c0(pc),a0 */
    CALL(sub_008584, 0x2EF0U);                           /* 002EEC  bsr  $8584 */
L_002EF0:
    a0 = 0x35CC;                                         /* 002EF0  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2EF8U);                           /* 002EF4  bsr  $8584 */
L_002EF8:
    wr16(V_026C, 6);                                     /* 002EF8  move.w  #$6,$26c(a5) */
    a0 = 0x30D6;                                         /* 002EFE  lea  $30d6(pc),a0 */
    CALL(sub_008584, 0x2F06U);                           /* 002F02  bsr  $8584 */
L_002F06:
    a0 = 0x35CC;                                         /* 002F06  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2F0EU);                           /* 002F0A  bsr  $8584 */
L_002F0E:
    wr16(V_026C, 7);                                     /* 002F0E  move.w  #$7,$26c(a5) */
    a0 = 0x30EC;                                         /* 002F14  lea  $30ec(pc),a0 */
    CALL(sub_008584, 0x2F1CU);                           /* 002F18  bsr  $8584 */
L_002F1C:
    a0 = 0x35CC;                                         /* 002F1C  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2F24U);                           /* 002F20  bsr  $8584 */
L_002F24:
    wr16(V_026C, 8);                                     /* 002F24  move.w  #$8,$26c(a5) */
    a0 = 0x3102;                                         /* 002F2A  lea  $3102(pc),a0 */
    CALL(sub_008584, 0x2F32U);                           /* 002F2E  bsr  $8584 */
L_002F32:
    a0 = 0x35CC;                                         /* 002F32  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2F3AU);                           /* 002F36  bsr  $8584 */
L_002F3A:
    wr16(V_026C, 9);                                     /* 002F3A  move.w  #$9,$26c(a5) */
    a0 = 0x3118;                                         /* 002F40  lea  $3118(pc),a0 */
    CALL(sub_008584, 0x2F48U);                           /* 002F44  bsr  $8584 */
L_002F48:
    a0 = 0x35CC;                                         /* 002F48  lea  $35cc(pc),a0 */
    CALL(sub_008584, 0x2F50U);                           /* 002F4C  bsr  $8584 */
L_002F50:
    wr16(V_026C, 0xA);                                   /* 002F50  move.w  #$a,$26c(a5) */
    a0 = 0x312E;                                         /* 002F56  lea  $312e(pc),a0 */
    CALL(sub_008584, 0x2F5EU);                           /* 002F5A  bsr  $8584 */
L_002F5E:
    wr16(V_0266, 0x12C0);                                /* 002F5E  move.w  #$12c0,$266(a5) */
    wr16(V_0268, 0x1D);                                  /* 002F64  move.w  #$1d,$268(a5) */
    wr16(V_026A, 0x1D);                                  /* 002F6A  move.w  #$1d,$26a(a5) */
    wr16(V_026C, 1);                                     /* 002F70  move.w  #$1,$26c(a5) */
    wr16(V_01C2, 0);                                     /* 002F76  clr.w  $1c2(a5) */
    do {
        BUDGET_CHECK(0x2F7AU);
L_002F7A:
        SETW(d1, rd16(V_01C2));                              /* 002F7A  move.w  $1c2(a5),d1 */
        SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), 2, 16));      /* 002F7E  lsl.w  #2,d1 */
        a0 = V_0170;                                         /* 002F80  lea  $170(a5),a0 */
        d0 = rd32(a0 + SX16(d1));                            /* 002F84  move.l  $0(a0,d1.w),d0 */
        CALL(sub_007D38, 0x2F8CU);                           /* 002F88  bsr  $7d38 */
L_002F8C:
        SETW(d0, 0xD);                                       /* 002F8C  move.w  #$d,d0 */
        CALL(sub_00858E, 0x2F94U);                           /* 002F90  bsr  $858e */
L_002F94:
        SETW(d0, 0xD);                                       /* 002F94  move.w  #$d,d0 */
        CALL(sub_00858E, 0x2F9CU);                           /* 002F98  bsr  $858e */
L_002F9C:
        /* 002F9C  addi.w  #$1,$1c2(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_01C2);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_01C2, r);
            fx = (r < s);
        }
        /* 002FA2  cmpi.w  #$8,$1c2(a5) */
        /* 002FA8  bne  $2f7a */
    } while (rd16(V_01C2) != 8);
    RTS();                                               /* 002FAA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2E4CU: goto L_002E4C;
        case 0x2E50U: goto L_002E50;
        case 0x2E70U: goto L_002E70;
        case 0x2E90U: goto L_002E90;
        case 0x2E98U: goto L_002E98;
        case 0x2EC4U: goto L_002EC4;
        case 0x2ECCU: goto L_002ECC;
        case 0x2EDAU: goto L_002EDA;
        case 0x2EE2U: goto L_002EE2;
        case 0x2EF0U: goto L_002EF0;
        case 0x2EF8U: goto L_002EF8;
        case 0x2F06U: goto L_002F06;
        case 0x2F0EU: goto L_002F0E;
        case 0x2F1CU: goto L_002F1C;
        case 0x2F24U: goto L_002F24;
        case 0x2F32U: goto L_002F32;
        case 0x2F3AU: goto L_002F3A;
        case 0x2F48U: goto L_002F48;
        case 0x2F50U: goto L_002F50;
        case 0x2F5EU: goto L_002F5E;
        case 0x2F7AU: goto L_002F7A;
        case 0x2F8CU: goto L_002F8C;
        case 0x2F94U: goto L_002F94;
        case 0x2F9CU: goto L_002F9C;
        }
    }
}

/* 002FAC  game routine */
void sub_002FAC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = V_0170;                                         /* 002FAC  lea  $170(a5),a0 */
    SETW(d0, rd16(V_01C6));                              /* 002FB0  move.w  $1c6(a5),d0 */
    d0 = SX16(d0);                                       /* 002FB4  ext.l  d0 */
    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), 2, 16));      /* 002FB6  lsl.w  #2,d0 */
    a0 += d0;                                            /* 002FB8  adda.l  d0,a0 */
    a1 = V_0190;                                         /* 002FBA  lea  $190(a5),a1 */
    a2 = V_018C;                                         /* 002FBE  lea  $18c(a5),a2 */
    do {
        BUDGET_CHECK(0x2FC2U);
L_002FC2:
        /* 002FC2  move.l  -(a2),-(a1) */
        a2 -= 4;
        {
            uint32_t v = rd32(a2);
            a1 -= 4;
            wr32(a1, v);
        }
        /* 002FC4  cmpa.l  a0,a1 */
        /* 002FC6  bne  $2fc2 */
    } while (a1 != a0);
    d0 = rd32(V_01E4);                                   /* 002FC8  move.l  $1e4(a5),d0 */
    wr32(a0, d0);                                        /* 002FCC  move.l  d0,(a0) */
    a0 = 0x3094;                                         /* 002FCE  lea  $3094(pc),a0 */
    SETW(d0, rd16(V_01C6));                              /* 002FD2  move.w  $1c6(a5),d0 */
    d0 = (d0 & 0xFFFF) * 0x16;                           /* 002FD6  mulu.w  #$16,d0 */
    a0 += d0;                                            /* 002FDA  adda.l  d0,a0 */
    wr32(V_01CC, a0);                                    /* 002FDC  move.l  a0,$1cc(a5) */
    a1 = 0x3142;                                         /* 002FE0  lea  $3142(pc),a1 */
    a2 = 0x312C;                                         /* 002FE4  lea  $312c(pc),a2 */
    do {
        BUDGET_CHECK(0x2FE8U);
L_002FE8:
        /* 002FE8  move.b  -(a2),-(a1) */
        a2 -= 1;
        {
            uint32_t v = rd8(a2);
            a1 -= 1;
            wr8(a1, v);
        }
        /* 002FEA  cmpa.l  a0,a2 */
        /* 002FEC  bhi  $2fe8 */
    } while (a2 > a0);
    d0 = 0x20202020;                                     /* 002FEE  move.l  #$20202020,d0 */
    /* 002FF4  move.l  d0,(a0)+ */
    {
        uint32_t v = d0;
        wr32(a0, v);
        a0 += 4;
    }
    /* 002FF6  move.l  d0,(a0)+ */
    {
        uint32_t v = d0;
        wr32(a0, v);
        a0 += 4;
    }
    /* 002FF8  move.l  d0,(a0)+ */
    {
        uint32_t v = d0;
        wr32(a0, v);
        a0 += 4;
    }
    /* 002FFA  move.l  d0,(a0)+ */
    {
        uint32_t v = d0;
        wr32(a0, v);
        a0 += 4;
    }
    /* 002FFC  move.l  d0,(a0)+ */
    {
        uint32_t v = d0;
        wr32(a0, v);
        a0 += 4;
    }
    RTS();                                               /* 002FFE  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2FC2U: goto L_002FC2;
        case 0x2FE8U: goto L_002FE8;
        }
    }
}

