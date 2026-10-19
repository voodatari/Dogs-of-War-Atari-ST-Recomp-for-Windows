/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 01026C  game routine */
void sub_01026C(void)
{
    SETW(d2, (d0 & 0xFFFF));                             /* 01026C  move.w  d0,d2 */
    /* 01026E  subi.b  #$0,d0 */
    {
        uint32_t s = 0;
        uint32_t d = (d0 & 0xFF);
        uint32_t r = (d - s) & 0xFF;
        SETB(d0, r);
        FL_SUB(s, d, r, 0xFF, 0x80, 1);
    }
    MISS(0x10272U);
}

/* 0104AC  game routine */
void sub_0104AC(void)
{
    /* 0104AC  ble  $180ae */
    if (COND(14)) {
        /* 0104B0  btst  d0,d0 */
        fz = !(d0 & (1u << (d0 & 31)));
        MISS(0x104B2U);
    }
    /* 0180AE  bra  $18090 */
    { MISS(0x18090U); return; }
}

/* 010730  game routine */
void sub_010730(void)
{
    SETB(d0, d0 | 0);                                    /* 010730  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 010734  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 010738  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 01073C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 010740  ori.b  #$0,d0 */
    /* 010744  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_010748(); return; }
}

/* 010748  game routine */
void sub_010748(void)
{
    SETB(d1, d1 | 2);                                    /* 010748  ori.b  #$2,d1 */
    SETB(d5, d5 | 0x25);                                 /* 01074C  ori.b  #$25,d5 */
    SETB(d1, d1 | 1);                                    /* 010750  ori.b  #$1,d1 */
    SETB(d6, d6 | 0x33);                                 /* 010754  ori.b  #$33,d6 */
    /* 010758  ori.b  #$1,d2 */
    {
        uint32_t s = 1;
        uint32_t d = (d2 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d2, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x1075CU);
}

/* 0107A8  game routine */
void sub_0107A8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0107A8  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    a0 = 0x10808;                                        /* 0107AA  lea  $10808(pc),a0 */
    d0 = (d0 & 0xFFFF) * 0xA;                            /* 0107AE  mulu.w  #$a,d0 */
    a0 = a0 + SX16(d0);                                  /* 0107B2  lea  $0(a0,d0.w),a0 */
    wr16(V_0164, 0);                                     /* 0107B6  clr.w  $164(a5) */
    wr16(V_0166, 0);                                     /* 0107BA  move.w  #$0,$166(a5) */
    /* 0107C0  move.w  (a0)+,$168(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0168, v);
    }
    /* 0107C4  move.w  (a0)+,$16a(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_016A, v);
    }
    /* 0107C8  move.w  (a0)+,$276(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0276, v);
    }
    /* 0107CC  move.l  (a0)+,$1d0(a5) */
    {
        uint32_t v = rd32(a0);
        a0 += 4;
        wr32(V_01D0, v);
    }
    wr32(V_016C, 0x5E652);                               /* 0107D0  move.l  #$5e652,$16c(a5) */
    CALL(sub_00EB26, 0x107DCU);                          /* 0107D8  bsr  $eb26 */
L_0107DC:
    CALL(sub_00E92A, 0x107E0U);                          /* 0107DC  bsr  $e92a */
L_0107E0:
    a4 = 0x5E66F;                                        /* 0107E0  lea  $5e66f.l,a4 */
    a6 = 0x5647E;                                        /* 0107E6  lea  $5647e.l,a6 */
    a1 = 0x5E652;                                        /* 0107EC  lea  $5e652.l,a1 */
    d0 = rd32(V_01D0);                                   /* 0107F2  move.l  $1d0(a5),d0 */
    a1 = a1 + SX16(d0);                                  /* 0107F6  lea  $0(a1,d0.w),a1 */
    a3 = 0x14000;                                        /* 0107FA  lea  $14000.l,a3 */
    CALL(sub_00FC88, 0x10804U);                          /* 010800  bsr  $fc88 */
L_010804:
    /* 010804  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 010806  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x107DCU: goto L_0107DC;
        case 0x107E0U: goto L_0107E0;
        case 0x10804U: goto L_010804;
        }
    }
}

/* 010808  game routine */
void sub_010808(void)
{
    /* 010808  ori.b  #$5,(a5)+ */
    {
        uint32_t s = 5;
        uint32_t ea = a5;
        a5 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x1080CU);
}

/* 010826  game routine */
void sub_010826(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 010826  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    a0 = 0x1087C;                                        /* 010828  lea  $1087c(pc),a0 */
    d0 = (d0 & 0xFFFF) * 6;                              /* 01082C  mulu.w  #$6,d0 */
    a0 = a0 + SX16(d0);                                  /* 010830  lea  $0(a0,d0.w),a0 */
    wr16(V_0164, 0);                                     /* 010834  clr.w  $164(a5) */
    wr16(V_0166, 0);                                     /* 010838  move.w  #$0,$166(a5) */
    /* 01083E  move.w  (a0)+,$168(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0168, v);
    }
    /* 010842  move.w  (a0)+,$16a(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_016A, v);
    }
    /* 010846  move.w  (a0)+,$276(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0276, v);
    }
    wr32(V_016C, 0x5E5EE);                               /* 01084A  move.l  #$5e5ee,$16c(a5) */
    CALL(sub_00EB26, 0x10856U);                          /* 010852  bsr  $eb26 */
L_010856:
    CALL(sub_00E92A, 0x1085AU);                          /* 010856  bsr  $e92a */
L_01085A:
    a0 = 0x466B6;                                        /* 01085A  lea  $466b6.l,a0 */
    a1 = 0x5F792;                                        /* 010860  lea  $5f792.l,a1 */
    SETW(d7, 0x6D);                                      /* 010866  move.w  #$6d,d7 */
    do {
        BUDGET_CHECK(0x1086AU);
L_01086A:
        /* 01086A  move.w  (a0)+,(a1)+ */
        {
            uint32_t v = rd16(a0);
            a0 += 2;
            wr16(a1, v);
            a1 += 2;
        }
        /* 01086C  move.w  (a0)+,(a1)+ */
        {
            uint32_t v = rd16(a0);
            a0 += 2;
            wr16(a1, v);
            a1 += 2;
        }
        /* 01086E  move.w  (a0)+,(a1)+ */
        {
            uint32_t v = rd16(a0);
            a0 += 2;
            wr16(a1, v);
            a1 += 2;
        }
        /* 010870  dbra  d7,$1086a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_010874:
    CALL(sub_00891C, 0x10878U);                          /* 010874  bsr  $891c */
L_010878:
    /* 010878  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 01087A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x10856U: goto L_010856;
        case 0x1085AU: goto L_01085A;
        case 0x1086AU: goto L_01086A;
        case 0x10874U: goto L_010874;
        case 0x10878U: goto L_010878;
        }
    }
}

/* 01087C  game routine */
void sub_01087C(void)
{
    wr8(a4 + 0x9, rd8(a4 + 0x9) | 9);                    /* 01087C  ori.b  #$9,$9(a4) */
    wr8(0x9, rd8(0x9) | 8);                              /* 010882  ori.b  #$8,$9(a5) */
    /* 010888  ori.b  #$7,$9(a6) */
    wr8(a6 + 0x9, rd8(a6 + 0x9) | 7);
    { sub_01088E(); return; }
}

/* 01088E  game routine */
void sub_01088E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_01088E;
    for (;;) {
        BUDGET_CHECK(0x2C10U);
        a0 = 0x79A0;                                         /* 002C10  lea  $79a0(pc),a0 */
        a2 = 0x8;                                            /* 002C14  lea  $8(a5),a2 */
        a1 = 0x8;                                            /* 002C18  lea  $8.l,a1 */
        do {
            BUDGET_CHECK(0x2C1EU);
L_002C1E:
            /* 002C1E  move.l  a0,(a1)+ */
            {
                uint32_t v = a0;
                wr32(a1, v);
                a1 += 4;
            }
            /* 002C20  cmpa.l  a2,a1 */
            /* 002C22  bcs  $2c1e */
        } while (a1 < a2);
        a0 = 0x8;                                            /* 002C24  lea  $8(a5),a0 */
        a1 = V_1E0C;                                         /* 002C28  lea  $1e0c(a5),a1 */
        CALL(sub_00759C, 0x2C30U);                           /* 002C2C  bsr  $759c */
L_002C30:
        wr32(V_1E12, 0x12345678);                            /* 002C30  move.l  #$12345678,$1e12(a5) */
        wr16(HW_PALETTE, 0);                                 /* 002C38  clr.w  $ff8240.l */
        CALL(sub_007626, 0x2C42U);                           /* 002C3E  bsr  $7626 */
L_002C42:
        CALL(sub_00765A, 0x2C46U);                           /* 002C42  bsr  $765a */
L_002C46:
        /* 002C46  move  #$2300,sr */
        { uint32_t s, ea1; (void)ea1;
        s = 0x2300U;
        set_sr(s); }
L_002C4A:
        a1 = 0x8C24;                                         /* 002C4A  lea  $8c24(pc),a1 */
        wr16(a1 + 0x1C, 0x25);                               /* 002C4E  move.w  #$25,$1c(a1) */
        CALL(sub_0075E2, 0x2C58U);                           /* 002C54  bsr  $75e2 */
L_002C58:
        wr16(V_0266, 0x1900);                                /* 002C58  move.w  #$1900,$266(a5) */
        wr16(V_0268, 0);                                     /* 002C5E  move.w  #$0,$268(a5) */
        wr16(V_026A, 0);                                     /* 002C64  move.w  #$0,$26a(a5) */
        wr16(V_026C, 2);                                     /* 002C6A  move.w  #$2,$26c(a5) */
        a0 = 0x37C6;                                         /* 002C70  lea  $37c6(pc),a0 */
        CALL(sub_008934, 0x2C78U);                           /* 002C74  bsr  $8934 */
L_002C78:
        CALL(sub_0086B8, 0x2C7CU);                           /* 002C78  bsr  $86b8 */
L_002C7C:
        /* 002C7C  bra  $373a */
        { sub_00373A(); return; }
L_01088E:
        a1 = 0xAC00;                                         /* 01088E  lea  $ac00(pc),a1 */
        a1 -= 0x8000;                                        /* 010892  suba.l  #$8000,a1 */
        a0 = 0x2C00;                                         /* 010898  lea  $2c00.l,a0 */
        SETW(d7, 0x3A97);                                    /* 01089E  move.w  #$3a97,d7 */
        do {
            BUDGET_CHECK(0x108A2U);
L_0108A2:
            /* 0108A2  move.l  (a1)+,(a0)+ */
            {
                uint32_t v = rd32(a1);
                a1 += 4;
                wr32(a0, v);
                a0 += 4;
            }
            /* 0108A4  dbra  d7,$108a2 */
            SETW(d7, d7 - 1);
        } while ((d7 & 0xFFFF) != 0xFFFF);
        /* 0108A8  jmp  $2c10.l */
    }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x2C1EU: goto L_002C1E;
        case 0x2C30U: goto L_002C30;
        case 0x2C42U: goto L_002C42;
        case 0x2C46U: goto L_002C46;
        case 0x2C4AU: goto L_002C4A;
        case 0x2C58U: goto L_002C58;
        case 0x2C78U: goto L_002C78;
        case 0x2C7CU: goto L_002C7C;
        case 0x108A2U: goto L_0108A2;
        }
    }
}

/* 0108AE  game routine */
void sub_0108AE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 0108AE  move.l  a0,-(a7) */
    {
        uint32_t v = a0;
        a7 -= 4;
        wr32(a7, v);
    }
    /* 0108B0  subq.w  #1,d0 */
    {
        uint32_t s = 1;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d - s) & 0xFFFF;
        SETW(d0, r);
        fx = (s > d);
    }
    d0 = (d0 & 0xFFFF) * 6;                              /* 0108B2  mulu.w  #$6,d0 */
    wr16(V_019E, (d0 & 0xFFFF));                         /* 0108B6  move.w  d0,$19e(a5) */
    a0 = 0x108EC;                                        /* 0108BA  lea  $108ec(pc),a0 */
    a0 = a0 + SX16(d0);                                  /* 0108BE  lea  $0(a0,d0.w),a0 */
    wr16(V_0164, 0);                                     /* 0108C2  clr.w  $164(a5) */
    wr16(V_0166, 0);                                     /* 0108C6  move.w  #$0,$166(a5) */
    /* 0108CC  move.w  (a0)+,$168(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0168, v);
    }
    /* 0108D0  move.w  (a0)+,$16a(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_016A, v);
    }
    /* 0108D4  move.w  (a0)+,$276(a5) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(V_0276, v);
    }
    wr32(V_016C, 0x73B92);                               /* 0108D8  move.l  #$73b92,$16c(a5) */
    CALL(sub_00EB26, 0x108E4U);                          /* 0108E0  bsr  $eb26 */
L_0108E4:
    CALL(sub_00E92A, 0x108E8U);                          /* 0108E4  bsr  $e92a */
L_0108E8:
    /* 0108E8  movea.l  (a7)+,a0 */
    a0 = rd32(a7);
    a7 += 4;
    RTS();                                               /* 0108EA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x108E4U: goto L_0108E4;
        case 0x108E8U: goto L_0108E8;
        }
    }
}

/* 0108EC  game routine */
void sub_0108EC(void)
{
    wr8(a7 + 0xF, rd8(a7 + 0xF) | 6);                    /* 0108EC  ori.b  #$6,$f(a7) */
    wr8(a0 + 0xF + SX16(d0), rd8(a0 + 0xF + SX16(d0)) | 9); /* 0108F2  ori.b  #$9,$f(a0,d0.w) */
    wr8(a2 + 0xF + SX16(d0), rd8(a2 + 0xF + SX16(d0)) | 2); /* 0108F8  ori.b  #$2,$f(a2,d0.w) */
    wr8(a3 + 0xF + SX16(d0), rd8(a3 + 0xF + SX16(d0)) | 3); /* 0108FE  ori.b  #$3,$f(a3,d0.w) */
    wr8(a4 + 0xF + SX16(d0), rd8(a4 + 0xF + SX16(d0)) | 8); /* 010904  ori.b  #$8,$f(a4,d0.w) */
    wr8(a6 + 0xF + SX16(d0), rd8(a6 + 0xF + SX16(d0)) | 1); /* 01090A  ori.b  #$1,$f(a6,d0.w) */
    wr8(a7 + 0xF + SX16(d0), rd8(a7 + 0xF + SX16(d0)) | 5); /* 010910  ori.b  #$5,$f(a7,d0.w) */
    /* 010916  ori.b  #$a,$f.w */
    {
        uint32_t s = 0xA;
        uint32_t d = rd8(0xF);
        uint32_t r = (d | s) & 0xFF;
        wr8(0xF, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x1091CU);
}

