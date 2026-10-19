/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 04152A  game routine */
void sub_04152A(void)
{
    /* 04152A  bchg  d3,-(a0) */
    a0 -= 1;
    wr8(a0, rd8(a0) ^ (1u << (d3 & 7)));
    /* 04152C  andi.w  #$0,d0 */
    {
        uint32_t s = 0;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d & s) & 0xFFFF;
        SETW(d0, r);
        FL_LOGIC(r, 0xFFFF, 0x8000);
    }
    MISS(0x41530U);
}

/* 041BCC  game routine */
void sub_041BCC(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 041BCC  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
    HOOK(43, 0x41BCCU);
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
    CALL(sub_041C20, 0x41BD4U);                          /* 041BD0  bsr  $41c20 */
L_041BD4:
    /* 041BD4  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 041BD8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41BD4U: goto L_041BD4;
        }
    }
}

/* 041BDA  game routine */
void sub_041BDA(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 041BDA  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    CALL(sub_041D48, 0x41BE2U);                          /* 041BDE  bsr  $41d48 */
L_041BE2:
    /* 041BE2  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 041BE6  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41BE2U: goto L_041BE2;
        }
    }
}

/* 041BE8  game routine */
void sub_041BE8(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 041BE8  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
    HOOK(44, 0x41BE8U);
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
    CALL(sub_041FC6, 0x41BF0U);                          /* 041BEC  bsr  $41fc6 */
L_041BF0:
    /* 041BF0  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 041BF4  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41BF0U: goto L_041BF0;
        }
    }
}

/* 041C04  game routine */
void sub_041C04(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 041C04  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    CALL(sub_042D32, 0x41C0CU);                          /* 041C08  bsr  $42d32 */
L_041C0C:
    /* 041C0C  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 041C10  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41C0CU: goto L_041C0C;
        }
    }
}

/* 041C20  game routine */
void sub_041C20(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a3 = 0x4152A;                                        /* 041C20  lea  $4152a(pc),a3 */
    CALL(sub_041BE8, 0x41C28U);                          /* 041C24  bsr  $41be8 */
L_041C28:
    wr8(a3 + 0xFCF, 0);                                  /* 041C28  clr.b  $fcf(a3) */
    SETW(d0, SX8(d0));                                   /* 041C2C  ext.w  d0 */
    d0 = (d0 & 0xFFFF) * 8;                              /* 041C2E  mulu.w  #$8,d0 */
    a0 = 0x42504;                                        /* 041C32  lea  $42504(pc),a0 */
    wr8(a3 + 0x813, rd8(a0 + 0x1 + SX16(d0)));           /* 041C36  move.b  $1(a0,d0.w),$813(a3) */
    wr8(a3 + 0x812, rd8(0x41D3D));                       /* 041C3C  move.b  $41d3d(pc),$812(a3) */
    a1 = 0x41CAC;                                        /* 041C42  lea  $41cac(pc),a1 */
    d7 = 2;                                              /* 041C46  moveq  #2,d7 */
    do {
        BUDGET_CHECK(0x41C48U);
L_041C48:
        wr8(a1 + 0x1B, 1);                                   /* 041C48  move.b  #$1,$1b(a1) */
        wr8(a1, 0);                                          /* 041C4E  clr.b  $0(a1) */
        wr8(a1 + 0x2C, 0);                                   /* 041C52  clr.b  $2c(a1) */
        wr8(a1 + 0x2D, 0);                                   /* 041C56  sf  $2d(a1) */
        wr8(a1 + 0x2E, 0);                                   /* 041C5A  sf  $2e(a1) */
        a0 = 0x424C6;                                        /* 041C5E  lea  $424c6(pc),a0 */
        wr32(a1 + 0x10, a0);                                 /* 041C62  move.l  a0,$10(a1) */
        wr32(a1 + 0x14, a0);                                 /* 041C66  move.l  a0,$14(a1) */
        a0 = 0x42504;                                        /* 041C6A  lea  $42504(pc),a0 */
        a0 = SX16(rd16(a0 + 0x2 + SX16(d0)));                /* 041C6E  movea.w  $2(a0,d0.w),a0 */
        wr16(a1 + 0x6, (a0 & 0xFFFF));                       /* 041C72  move.w  a0,$6(a1) */
        wr16(a1 + 0xA, 2);                                   /* 041C76  move.w  #$2,$a(a1) */
        a0 = SX16(rd16(a3 + SX16(a0)));                      /* 041C7C  movea.w  $0(a3,a0.w),a0 */
        a0 += a3;                                            /* 041C80  adda.l  a3,a0 */
        wr32(a1 + 0x2, a0);                                  /* 041C82  move.l  a0,$2(a1) */
        a1 += SX16(0x30);                                    /* 041C86  adda.w  #$30,a1 */
        /* 041C8A  addq.w  #2,d0 */
        {
            uint32_t s = 2;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fx = (r < s);
        }
        /* 041C8C  dbra  d7,$41c48 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_041C90:
    wr32(a3 + 0xFD2, 0);                                 /* 041C90  clr.l  $fd2(a3) */
    wr8(a3 + 0xFD8, 0xF);                                /* 041C94  move.b  #$f,$fd8(a3) */
    wr8(a3 + 0xFD0, 0);                                  /* 041C9A  sf  $fd0(a3) */
    wr8(a3 + 0x814, 0xFF);                               /* 041C9E  st  $814(a3) */
    wr8(a3 + 0xFCE, 0xFF);                               /* 041CA2  st  $fce(a3) */
    wr8(a3 + 0xFC8, 0xFF);                               /* 041CA6  st  $fc8(a3) */
    RTS();                                               /* 041CAA  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41C28U: goto L_041C28;
        case 0x41C48U: goto L_041C48;
        case 0x41C90U: goto L_041C90;
        }
    }
}

/* 041CAC  game routine */
void sub_041CAC(void)
{
    /* 041CAC  move.l  d0,-(a0) */
    {
        uint32_t v = d0;
        a0 -= 4;
        wr32(a0, v);
    }
    SETB(d4, d4 | 0xBC);                                 /* 041CAE  ori.b  #$bc,d4 */
    /* 041CB2  move.b  (a2)+,(a0) */
    wr8(a0, rd8(a2));
    a2 += 1;
    SETB(d0, d0 | 0xA);                                  /* 041CB4  ori.b  #$a,d0 */
    SETB(d0, d0 | 0);                                    /* 041CB8  ori.b  #$0,d0 */
    SETB(d4, d4 | 0xE1);                                 /* 041CBC  ori.b  #$e1,d4 */
    SETB(d4, d4 | 0xE2);                                 /* 041CC0  ori.b  #$e2,d4 */
    SETB(d0, d0 | 1);                                    /* 041CC4  ori.b  #$1,d0 */
    /* 041CC8  andi.b  #$0,$4(a3,d0.w) */
    {
        uint32_t s = 0;
        uint32_t d = rd8(a3 + 0x4 + SX16(d0));
        uint32_t r = (d & s) & 0xFF;
        wr8(a3 + 0x4 + SX16(d0), r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x41CCEU);
}

/* 041CDC  game routine */
void sub_041CDC(void)
{
    /* 041CDC  move.l  d0,-(a0) */
    {
        uint32_t v = d0;
        a0 -= 4;
        wr32(a0, v);
    }
    SETB(d4, d4 | 0xAA);                                 /* 041CDE  ori.b  #$aa,d4 */
    /* 041CE2  move.b  (a2)+,d0 */
    SETB(d0, rd8(a2));
    a2 += 1;
    SETB(d0, d0 | 0x1E);                                 /* 041CE4  ori.b  #$1e,d0 */
    SETB(d0, d0 | 0x90);                                 /* 041CE8  ori.b  #$90,d0 */
    SETB(d4, d4 | 0xC6);                                 /* 041CEC  ori.b  #$c6,d4 */
    SETB(d4, d4 | 0xC6);                                 /* 041CF0  ori.b  #$c6,d4 */
    SETW(d1, (d1 & 0xFFFF));                             /* 041CF4  move.w  d1,d1 */
    SETB(d1, d1 & 0xF);                                  /* 041CF6  andi.b  #$f,d1 */
    /* 041CFA  btst  d6,d1 */
    SETB(d4, d4 | 0xF2);                                 /* 041CFC  ori.b  #$f2,d4 */
    SETB(d4, d4 | 0xF0);                                 /* 041D00  ori.b  #$f0,d4 */
    /* 041D04  btst  d6,d2 */
    SETB(d0, d0 & 0x7F);                                 /* 041D06  andi.b  #$7f,d0 */
    /* 041D0A  ori.b  #$0,(a2) */
    wr8(a2, rd8(a2) | 0);
    { sub_041D0E(); return; }
}

/* 041D0C  game routine */
void sub_041D0C(void)
{
    /* 041D0C  move.l  d0,-(a0) */
    {
        uint32_t v = d0;
        a0 -= 4;
        wr32(a0, v);
    }
    { sub_041D0E(); return; }
}

/* 041D0E  game routine */
void sub_041D0E(void)
{
    /* 041D0E  ori.b  #$c3,d4 */
    {
        uint32_t s = 0xC3;
        uint32_t d = (d4 & 0xFF);
        uint32_t r = (d | s) & 0xFF;
        SETB(d4, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x41D12U);
}

/* 041D3C  game routine */
void sub_041D3C(void)
{
    /* 041D3C  move.w  #$8f1f,d6 */
    {
        uint32_t v = 0x8F1F;
        SETW(d6, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    MISS(0x41D40U);
}

/* 041D48  game routine */
void sub_041D48(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    goto L_041D48;
L_041D46:
    RTS();                                               /* 041D46  rts */
L_041D48:
    a3 = 0x4152A;                                        /* 041D48  lea  $4152a(pc),a3 */
    /* 041D4C  tst.b  $fc8(a3) */
    /* 041D50  bne  $41d58 */
    if (rd8(a3 + 0xFC8) == 0) {
        /* 041D52  tst.l  $fca(a3) */
        /* 041D56  beq  $41d46 */
        if (rd32(a3 + 0xFCA) == 0) {
            BUDGET_CHECK(0x41D46U);
            goto L_041D46;
        }
    }
L_041D58:
    CALL(sub_042E82, 0x41D5CU);                          /* 041D58  bsr  $42e82 */
L_041D5C:
    /* 041D5C  tst.b  $fc8(a3) */
    /* 041D60  beq  $41e32 */
    if (rd8(a3 + 0xFC8) != 0) {
L_041D64:
        /* 041D64  move.b  $424fa(pc),d0 */
        /* 041D68  beq  $41d86 */
        SETB(d0, rd8(0x424FA));
        if ((d0 & 0xFF) != 0) {
            /* 041D6A  tst.b  $fd8(a3) */
            /* 041D6E  beq  $42090 */
            if (rd8(a3 + 0xFD8) == 0) {
                { sub_042090(); return; }
            }
            /* 041D72  subq.b  #1,$fd1(a3) */
            {
                uint32_t s = 1;
                uint32_t d = rd8(a3 + 0xFD1);
                uint32_t r = (d - s) & 0xFF;
                wr8(a3 + 0xFD1, r);
                fz = (r == 0);
            }
            /* 041D76  bne  $41d86 */
            if (COND(7)) {
                /* 041D78  subq.b  #1,$fd8(a3) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd8(a3 + 0xFD8);
                    uint32_t r = (d - s) & 0xFF;
                    wr8(a3 + 0xFD8, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 041D7C  beq  $42090 */
                if (COND(7)) { sub_042090(); return; }
                wr8(a3 + 0xFD1, rd8(0x424FA));                       /* 041D80  move.b  $424fa(pc),$fd1(a3) */
            }
        }
L_041D86:
        wr8(a3 + 0x816, rd8(0x41D3F));                       /* 041D86  move.b  $41d3f(pc),$816(a3) */
        SETB(d0, rd8(0x41D3D));                              /* 041D8C  move.b  $41d3d(pc),d0 */
        /* 041D90  add.b  d0,$814(a3) */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = rd8(a3 + 0x814);
            uint32_t r = (d + s) & 0xFF;
            wr8(a3 + 0x814, r);
            fc = (r < s);
        }
        /* 041D94  bcc  $41dae */
        if (COND(5)) {
L_041D96:
            a0 = 0x41CAC;                                        /* 041D96  lea  $41cac(pc),a0 */
            CALL(sub_042180, 0x41D9EU);                          /* 041D9A  bsr  $42180 */
L_041D9E:
            a0 = 0x41CDC;                                        /* 041D9E  lea  $41cdc(pc),a0 */
            CALL(sub_042180, 0x41DA6U);                          /* 041DA2  bsr  $42180 */
L_041DA6:
            a0 = 0x41D0C;                                        /* 041DA6  lea  $41d0c(pc),a0 */
            CALL(sub_042180, 0x41DAEU);                          /* 041DAA  bsr  $42180 */
        }
L_041DAE:
        wr8(a3 + 0xFD8, rd8(a3 + 0xFD8) & 0xF);              /* 041DAE  andi.b  #$f,$fd8(a3) */
        a0 = 0x41CAC;                                        /* 041DB4  lea  $41cac(pc),a0 */
        CALL(sub_042282, 0x41DBCU);                          /* 041DB8  bsr  $42282 */
L_041DBC:
        wr16(a3 + 0xFD6, (d0 & 0xFFFF));                     /* 041DBC  move.w  d0,$fd6(a3) */
        wr8(a3 + 0xEA2, (d0 & 0xFF));                        /* 041DC0  move.b  d0,$ea2(a3) */
        wr8(a3 + 0xEA3, rd8(0x42500));                       /* 041DC4  move.b  $42500(pc),$ea3(a3) */
        SETB(d0, rd8(0x42502));                              /* 041DCA  move.b  $42502(pc),d0 */
        SETB(d0, d0 ^ 0xF);                                  /* 041DCE  eori.b  #$f,d0 */
        /* 041DD2  sub.b  d0,d1 */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d1, r);
            fc = (s > d);
        }
        /* 041DD4  bcc  $41dd8 */
        if (COND(5)) {
L_041DD6:
            d1 = 0;                                              /* 041DD6  moveq  #0,d1 */
        }
L_041DD8:
        /* 041DD8  move.b  d1,$eaa(a3) */
        HOOK(45, 0x41DD8U);
        wr8(a3 + 0xEAA, (d1 & 0xFF));
L_041DDC:
        /* 041DDC  lea  $41cdc(pc),a0 */
        HOOK(46, 0x41DDCU);
        a0 = 0x41CDC;
        CALL(sub_042282, 0x41DE4U);                          /* 041DE0  bsr  $42282 */
L_041DE4:
        wr16(a3 + 0xFD6, (d0 & 0xFFFF));                     /* 041DE4  move.w  d0,$fd6(a3) */
        wr8(a3 + 0xEA4, (d0 & 0xFF));                        /* 041DE8  move.b  d0,$ea4(a3) */
        wr8(a3 + 0xEA5, rd8(0x42500));                       /* 041DEC  move.b  $42500(pc),$ea5(a3) */
        SETB(d0, rd8(0x42502));                              /* 041DF2  move.b  $42502(pc),d0 */
        SETB(d0, d0 ^ 0xF);                                  /* 041DF6  eori.b  #$f,d0 */
        /* 041DFA  sub.b  d0,d1 */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d1, r);
            fc = (s > d);
        }
        /* 041DFC  bcc  $41e00 */
        if (COND(5)) {
L_041DFE:
            d1 = 0;                                              /* 041DFE  moveq  #0,d1 */
        }
L_041E00:
        /* 041E00  move.b  d1,$eab(a3) */
        HOOK(47, 0x41E00U);
        wr8(a3 + 0xEAB, (d1 & 0xFF));
L_041E04:
        /* 041E04  lea  $41d0c(pc),a0 */
        HOOK(48, 0x41E04U);
        a0 = 0x41D0C;
        CALL(sub_042282, 0x41E0CU);                          /* 041E08  bsr  $42282 */
L_041E0C:
        wr16(a3 + 0xFD6, (d0 & 0xFFFF));                     /* 041E0C  move.w  d0,$fd6(a3) */
        wr8(a3 + 0xEA6, (d0 & 0xFF));                        /* 041E10  move.b  d0,$ea6(a3) */
        wr8(a3 + 0xEA7, rd8(0x42500));                       /* 041E14  move.b  $42500(pc),$ea7(a3) */
        SETB(d0, rd8(0x42502));                              /* 041E1A  move.b  $42502(pc),d0 */
        SETB(d0, d0 ^ 0xF);                                  /* 041E1E  eori.b  #$f,d0 */
        /* 041E22  sub.b  d0,d1 */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d - s) & 0xFF;
            SETB(d1, r);
            fc = (s > d);
            fx = fc;
        }
        /* 041E24  bcc  $41e28 */
        if (COND(5)) {
L_041E26:
            d1 = 0;                                              /* 041E26  moveq  #0,d1 */
        }
L_041E28:
        /* 041E28  move.b  d1,$eac(a3) */
        HOOK(49, 0x41E28U);
        wr8(a3 + 0xEAC, (d1 & 0xFF));
L_041E2C:
        /* 041E2C  move.b  $41d40(pc),$ea8(a3) */
        HOOK(50, 0x41E2CU);
        wr8(a3 + 0xEA8, rd8(0x41D40));
    }
    /* 041E32  tst.b  $fca(a3) */
    {
        uint32_t v = rd8(a3 + 0xFCA);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 041E36  beq  $41e68 */
    if (COND(6)) {
L_041E38:
        /* 041E38  bmi  $41d46 */
        if (COND(11)) {
            BUDGET_CHECK(0x41D46U);
            goto L_041D46;
        }
L_041E3C:
        wr8(a3 + 0xEA2, rd8(0x423DB));                       /* 041E3C  move.b  $423db(pc),$ea2(a3) */
        wr8(a3 + 0xEA3, rd8(0x423DA));                       /* 041E42  move.b  $423da(pc),$ea3(a3) */
        SETB(d0, rd8(0x43300));                              /* 041E48  move.b  $43300(pc),d0 */
        fz = !(d0 & 0x8);                                    /* 041E4C  btst  #$3,d0 */
        /* 041E50  bne  $41e58 */
        if (COND(7)) {
            wr8(a3 + 0xEA8, rd8(0x423E0));                       /* 041E52  move.b  $423e0(pc),$ea8(a3) */
        }
L_041E58:
        wr8(a3 + 0xEA9, rd8(a3 + 0xEA9) | 9);                /* 041E58  ori.b  #$9,$ea9(a3) */
        wr8(a3 + 0xEA9, rd8(a3 + 0xEA9) & (d0 & 0xFF));      /* 041E5E  and.b  d0,$ea9(a3) */
        wr8(a3 + 0xEAA, rd8(0x423E2));                       /* 041E62  move.b  $423e2(pc),$eaa(a3) */
    }
L_041E68:
    /* 041E68  tst.b  $fcb(a3) */
    /* 041E6C  beq  $41e9c */
    HOOK(51, 0x41E68U);
    if (rd8(a3 + 0xFCB) != 0) {
        wr8(a3 + 0xEA4, rd8(0x423DD));                       /* 041E6E  move.b  $423dd(pc),$ea4(a3) */
        wr8(a3 + 0xEA5, rd8(0x423DC));                       /* 041E74  move.b  $423dc(pc),$ea5(a3) */
        SETB(d0, rd8(0x4331A));                              /* 041E7A  move.b  $4331a(pc),d0 */
        fz = !(d0 & 0x8);                                    /* 041E7E  btst  #$3,d0 */
        /* 041E82  bne  $41e8a */
        if (COND(7)) {
            wr8(a3 + 0xEA8, rd8(0x423E0));                       /* 041E84  move.b  $423e0(pc),$ea8(a3) */
        }
        wr8(a3 + 0xEA9, rd8(a3 + 0xEA9) | 0x12);             /* 041E8A  ori.b  #$12,$ea9(a3) */
        SETB(d0, shift_op(3, 1, (d0 & 0xFF), 1, 8));         /* 041E90  rol.b  #1,d0 */
        wr8(a3 + 0xEA9, rd8(a3 + 0xEA9) & (d0 & 0xFF));      /* 041E92  and.b  d0,$ea9(a3) */
        wr8(a3 + 0xEAB, rd8(0x423E3));                       /* 041E96  move.b  $423e3(pc),$eab(a3) */
    }
L_041E9C:
    /* 041E9C  tst.b  $fcc(a3) */
    /* 041EA0  beq  $41ed0 */
    HOOK(52, 0x41E9CU);
    if (rd8(a3 + 0xFCC) != 0) {
        wr8(a3 + 0xEA6, rd8(0x423DF));                       /* 041EA2  move.b  $423df(pc),$ea6(a3) */
        wr8(a3 + 0xEA7, rd8(0x423DE));                       /* 041EA8  move.b  $423de(pc),$ea7(a3) */
        SETB(d0, rd8(0x43334));                              /* 041EAE  move.b  $43334(pc),d0 */
        fz = !(d0 & 0x8);                                    /* 041EB2  btst  #$3,d0 */
        /* 041EB6  bne  $41ebe */
        if (COND(7)) {
            wr8(a3 + 0xEA8, rd8(0x423E0));                       /* 041EB8  move.b  $423e0(pc),$ea8(a3) */
        }
        wr8(a3 + 0xEA9, rd8(a3 + 0xEA9) | 0x24);             /* 041EBE  ori.b  #$24,$ea9(a3) */
        SETB(d0, shift_op(3, 1, (d0 & 0xFF), 2, 8));         /* 041EC4  rol.b  #2,d0 */
        wr8(a3 + 0xEA9, rd8(a3 + 0xEA9) & (d0 & 0xFF));      /* 041EC6  and.b  d0,$ea9(a3) */
        wr8(a3 + 0xEAC, rd8(0x423E4));                       /* 041ECA  move.b  $423e4(pc),$eac(a3) */
    }
L_041ED0:
    /* 041ED0  andi.b  #$3f,$ea9(a3) */
    HOOK(53, 0x41ED0U);
    {
        uint32_t s = 0x3F;
        uint32_t d = rd8(a3 + 0xEA9);
        uint32_t r = (d & s) & 0xFF;
        wr8(a3 + 0xEA9, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    /* 041ED6  move  sr,d1 */
    { uint32_t ea; (void)ea;
    d1 = (d1 & 0xFFFF0000U) | ((get_sr()) & 0xFFFFU);
    }
    /* 041ED8  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_041EDC:
    SETB(d2, 0);                                         /* 041EDC  clr.b  d2 */
    /* 041EDE  tst.l  $fd2(a3) */
    {
        uint32_t v = rd32(a3 + 0xFD2);
        FL_LOGIC(v, 0xFFFFFFFF, 0x80000000);
    }
L_041EE2:
    /* 041EE2  bne  $41ef4 */
    HOOK(54, 0x41EE2U);
    if (COND(7)) {
L_041EE4:
        wr8(HW_YM_SELECT, 6);                                /* 041EE4  move.b  #$6,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423D2));                       /* 041EEC  move.b  $423d2(pc),$ff8802.l */
    }
L_041EF4:
    /* 041EF4  tst.b  $fd2(a3) */
    {
        uint32_t v = rd8(a3 + 0xFD2);
        FL_LOGIC(v, 0xFF, 0x80);
    }
L_041EF8:
    /* 041EF8  bne  $41f2e */
    HOOK(55, 0x41EF8U);
    if (COND(7)) {
L_041EFA:
        wr8(HW_YM_SELECT, 0);                                /* 041EFA  move.b  #$0,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423CC));                       /* 041F02  move.b  $423cc(pc),$ff8802.l */
        wr8(HW_YM_SELECT, 1);                                /* 041F0A  move.b  #$1,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423CD));                       /* 041F12  move.b  $423cd(pc),$ff8802.l */
        wr8(HW_YM_SELECT, 8);                                /* 041F1A  move.b  #$8,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423D4));                       /* 041F22  move.b  $423d4(pc),$ff8802.l */
L_041F2A:
        SETB(d2, d2 | 9);                                    /* 041F2A  ori.b  #$9,d2 */
    }
L_041F2E:
    /* 041F2E  tst.b  $fd3(a3) */
    {
        uint32_t v = rd8(a3 + 0xFD3);
        FL_LOGIC(v, 0xFF, 0x80);
    }
L_041F32:
    /* 041F32  bne  $41f68 */
    HOOK(56, 0x41F32U);
    if (COND(7)) {
L_041F34:
        wr8(HW_YM_SELECT, 2);                                /* 041F34  move.b  #$2,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423CE));                       /* 041F3C  move.b  $423ce(pc),$ff8802.l */
        wr8(HW_YM_SELECT, 3);                                /* 041F44  move.b  #$3,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423CF));                       /* 041F4C  move.b  $423cf(pc),$ff8802.l */
        wr8(HW_YM_SELECT, 9);                                /* 041F54  move.b  #$9,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423D5));                       /* 041F5C  move.b  $423d5(pc),$ff8802.l */
        SETB(d2, d2 | 0x12);                                 /* 041F64  ori.b  #$12,d2 */
    }
L_041F68:
    /* 041F68  tst.b  $fd4(a3) */
    /* 041F6C  bne  $41fa2 */
    if (rd8(a3 + 0xFD4) == 0) {
L_041F6E:
        wr8(HW_YM_SELECT, 4);                                /* 041F6E  move.b  #$4,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423D0));                       /* 041F76  move.b  $423d0(pc),$ff8802.l */
        wr8(HW_YM_SELECT, 5);                                /* 041F7E  move.b  #$5,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423D1));                       /* 041F86  move.b  $423d1(pc),$ff8802.l */
        wr8(HW_YM_SELECT, 0xA);                              /* 041F8E  move.b  #$a,$ff8800.l */
        wr8(HW_YM_DATA, rd8(0x423D6));                       /* 041F96  move.b  $423d6(pc),$ff8802.l */
        SETB(d2, d2 | 0x24);                                 /* 041F9E  ori.b  #$24,d2 */
    }
    wr8(HW_YM_SELECT, 7);                                /* 041FA2  move.b  #$7,$ff8800.l */
    SETB(d0, rd8(HW_YM_SELECT));                         /* 041FAA  move.b  $ff8800.l,d0 */
    SETB(d3, rd8(0x423D3));                              /* 041FB0  move.b  $423d3(pc),d3 */
    SETB(d3, d3 ^ (d0 & 0xFF));                          /* 041FB4  eor.b  d0,d3 */
    SETB(d3, d3 & (d2 & 0xFF));                          /* 041FB6  and.b  d2,d3 */
    SETB(d3, d3 ^ (d0 & 0xFF));                          /* 041FB8  eor.b  d0,d3 */
    wr8(HW_YM_DATA, (d3 & 0xFF));                        /* 041FBA  move.b  d3,$ff8802.l */
    /* 041FC0  move  d1,sr */
    { uint32_t s, ea1; (void)ea1;
    s = (d1 & 0xFFFFU);
    set_sr(s); }
L_041FC2:
    /* 041FC2  bra  $41d46 */
    BUDGET_CHECK(0x41D46U);
    goto L_041D46;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41D46U: goto L_041D46;
        case 0x41D58U: goto L_041D58;
        case 0x41D5CU: goto L_041D5C;
        case 0x41D64U: goto L_041D64;
        case 0x41D86U: goto L_041D86;
        case 0x41D96U: goto L_041D96;
        case 0x41D9EU: goto L_041D9E;
        case 0x41DA6U: goto L_041DA6;
        case 0x41DAEU: goto L_041DAE;
        case 0x41DBCU: goto L_041DBC;
        case 0x41DD6U: goto L_041DD6;
        case 0x41DD8U: goto L_041DD8;
        case 0x41DDCU: goto L_041DDC;
        case 0x41DE4U: goto L_041DE4;
        case 0x41DFEU: goto L_041DFE;
        case 0x41E00U: goto L_041E00;
        case 0x41E04U: goto L_041E04;
        case 0x41E0CU: goto L_041E0C;
        case 0x41E26U: goto L_041E26;
        case 0x41E28U: goto L_041E28;
        case 0x41E2CU: goto L_041E2C;
        case 0x41E38U: goto L_041E38;
        case 0x41E3CU: goto L_041E3C;
        case 0x41E58U: goto L_041E58;
        case 0x41E68U: goto L_041E68;
        case 0x41E9CU: goto L_041E9C;
        case 0x41ED0U: goto L_041ED0;
        case 0x41EDCU: goto L_041EDC;
        case 0x41EE2U: goto L_041EE2;
        case 0x41EE4U: goto L_041EE4;
        case 0x41EF4U: goto L_041EF4;
        case 0x41EF8U: goto L_041EF8;
        case 0x41EFAU: goto L_041EFA;
        case 0x41F2AU: goto L_041F2A;
        case 0x41F2EU: goto L_041F2E;
        case 0x41F32U: goto L_041F32;
        case 0x41F34U: goto L_041F34;
        case 0x41F68U: goto L_041F68;
        case 0x41F6EU: goto L_041F6E;
        case 0x41FC2U: goto L_041FC2;
        }
    }
}

/* 041FC6  game routine */
void sub_041FC6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a3 = 0x4152A;                                        /* 041FC6  lea  $4152a(pc),a3 */
    wr8(a3 + 0xFC8, 0);                                  /* 041FCA  sf  $fc8(a3) */
    /* 041FCE  bra  $43368 */
    goto L_043368;
L_041FD2:
    /* 041FD2  move  sr,d2 */
    { uint32_t ea; (void)ea;
    d2 = (d2 & 0xFFFF0000U) | ((get_sr()) & 0xFFFFU);
    }
    /* 041FD4  move  #$2700,sr */
    { uint32_t s, ea1; (void)ea1;
    s = 0x2700U;
    set_sr(s); }
L_041FD8:
    wr8(HW_YM_SELECT, 7);                                /* 041FD8  move.b  #$7,$ff8800.l */
L_041FE0:
    SETB(d1, rd8(HW_YM_SELECT));                         /* 041FE0  move.b  $ff8800.l,d1 */
L_041FE6:
    SETB(d1, d1 | 0x3F);                                 /* 041FE6  ori.b  #$3f,d1 */
L_041FEA:
    wr8(HW_YM_DATA, (d1 & 0xFF));                        /* 041FEA  move.b  d1,$ff8802.l */
L_041FF0:
    wr8(HW_YM_SELECT, 8);                                /* 041FF0  move.b  #$8,$ff8800.l */
L_041FF8:
    wr8(HW_YM_DATA, 0);                                  /* 041FF8  move.b  #$0,$ff8802.l */
L_042000:
    wr8(HW_YM_SELECT, 9);                                /* 042000  move.b  #$9,$ff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 042008  move.b  #$0,$ff8802.l */
    wr8(HW_YM_SELECT, 0xA);                              /* 042010  move.b  #$a,$ff8800.l */
    wr8(HW_YM_DATA, 0);                                  /* 042018  move.b  #$0,$ff8802.l */
    /* 042020  move  d2,sr */
    { uint32_t s, ea1; (void)ea1;
    s = (d2 & 0xFFFFU);
    set_sr(s); }
L_042022:
    RTS();                                               /* 042022  rts */
L_043368:
    a3 = 0x4152A;                                        /* 043368  lea  $4152a(pc),a3 */
    wr32(a3 + 0xFCA, 0);                                 /* 04336C  clr.l  $fca(a3) */
    wr16(a3 + 0xEAA, 0);                                 /* 043370  clr.w  $eaa(a3) */
    wr8(a3 + 0xEAC, 0);                                  /* 043374  clr.b  $eac(a3) */
    /* 043378  move.b  #$3f,$ea9(a3) */
    {
        uint32_t v = 0x3F;
        wr8(a3 + 0xEA9, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 04337E  bra  $41fd2 */
    BUDGET_CHECK(0x41FD2U);
    goto L_041FD2;

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x41FD2U: goto L_041FD2;
        case 0x41FD8U: goto L_041FD8;
        case 0x41FE0U: goto L_041FE0;
        case 0x41FE6U: goto L_041FE6;
        case 0x41FEAU: goto L_041FEA;
        case 0x41FF0U: goto L_041FF0;
        case 0x41FF8U: goto L_041FF8;
        case 0x42000U: goto L_042000;
        case 0x42022U: goto L_042022;
        case 0x43368U: goto L_043368;
        }
    }
}

