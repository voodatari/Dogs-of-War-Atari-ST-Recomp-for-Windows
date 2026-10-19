/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 04301E  game routine */
void sub_04301E(void)
{
    /* 04301E  tst.b  $1e04(a3) */
    /* 043022  bne  $43034 */
    if (rd8(a3 + 0x1E04) == 0) {
        /* 043024  tst.b  $1e10(a3) */
        /* 043028  bne  $43034 */
        if (rd8(a3 + 0x1E10) == 0) {
            wr8(a3 + 0xFCC, 0);                                  /* 04302A  sf  $fcc(a3) */
            wr8(a3 + 0xEAC, 0);                                  /* 04302E  clr.b  $eac(a3) */
            RTS();                                               /* 043032  rts */
        }
    }
    wr8(a3 + 0x1E04, rd8(a3 + 0x1E04) - 1);              /* 043034  subq.b  #1,$1e04(a3) */
    /* 043038  tst.b  $1e12(a3) */
    /* 04303C  bne  $43072 */
    if (rd8(a3 + 0x1E12) == 0) {
        /* 04303E  tst.b  $1e10(a3) */
        /* 043042  bne  $4304a */
        if (rd8(a3 + 0x1E10) == 0) {
            /* 043044  tst.b  $1e0d(a3) */
            /* 043048  beq  $430ae */
            if (rd8(a3 + 0x1E0D) == 0) {
                goto L_0430AE;
            }
        }
        wr8(a3 + 0x1E0D, rd8(a3 + 0x1E0D) - 1);              /* 04304A  subq.b  #1,$1e0d(a3) */
        wr8(a3 + 0x1E12, rd8(0x4332F));                      /* 04304E  move.b  $4332f(pc),$1e12(a3) */
        /* 043054  move.b  $43335(pc),d0 */
        /* 043058  beq  $4305e */
        SETB(d0, rd8(0x43335));
        if ((d0 & 0xFF) != 0) {
            SETB(d0, rd8(0x43366));                              /* 04305A  move.b  $43366(pc),d0 */
        }
        SETB(d1, rd8(0x43331));                              /* 04305E  move.b  $43331(pc),d1 */
        /* 043062  add.b  d0,d1 */
        {
            uint32_t s = (d0 & 0xFF);
            uint32_t d = (d1 & 0xFF);
            uint32_t r = (d + s) & 0xFF;
            SETB(d1, r);
            fx = (r < s);
        }
        wr8(a3 + 0xEB5, (d1 & 0xFF));                        /* 043064  move.b  d1,$eb5(a3) */
        /* 043068  move.b  $43330(pc),d1 */
        {
            uint32_t v = rd8(0x43330);
            SETB(d1, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 04306C  addx.b  d0,d1 */
        { uint32_t s, d, r, x = fx, ea1, ea2; (void)ea1; (void)ea2;
        s = d0 & 0xFFU; d = d1 & 0xFFU;
        r = (s + d + x) & 0xFFU;
        fv = !!((~(s ^ d) & (s ^ r)) & 0x80U); fc = fx = (((uint64_t)s + d + x) >> 8) & 1;
        fn = !!(r & 0x80U); if (r) fz = 0;
        d1 = (d1 & 0xFFFFFF00U) | ((r) & 0xFFU);
        }
        wr8(a3 + 0xEB4, (d1 & 0xFF));                        /* 04306E  move.b  d1,$eb4(a3) */
    }
    wr8(a3 + 0x1E12, rd8(a3 + 0x1E12) - 1);              /* 043072  subq.b  #1,$1e12(a3) */
    /* 043076  move.b  $4333b(pc),d0 */
    /* 04307A  beq  $43086 */
    SETB(d0, rd8(0x4333B));
    if ((d0 & 0xFF) != 0) {
        /* 04307C  subq.b  #1,$1e14(a3) */
        {
            uint32_t s = 1;
            uint32_t d = rd8(a3 + 0x1E14);
            uint32_t r = (d - s) & 0xFF;
            wr8(a3 + 0x1E14, r);
            fz = (r == 0);
        }
        /* 043080  bne  $430a0 */
        if (COND(6)) {
            goto L_0430A0;
        }
        wr8(a3 + 0x1E14, (d0 & 0xFF));                       /* 043082  move.b  d0,$1e14(a3) */
    }
    /* 043086  tst.b  $1e0c(a3) */
    {
        uint32_t v = rd8(a3 + 0x1E0C);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    /* 04308A  beq  $430a0 */
    if (COND(6)) {
        /* 04308C  bpl  $43098 */
        if (COND(11)) {
            SETW(d0, rd16(0x43332));                             /* 04308E  move.w  $43332(pc),d0 */
            wr16(a3 + 0xEB4, rd16(a3 + 0xEB4) + (d0 & 0xFFFF));  /* 043092  add.w  d0,$eb4(a3) */
            /* 043096  bra  $430a0 */
        } else {
            SETW(d0, rd16(0x43332));                             /* 043098  move.w  $43332(pc),d0 */
            wr16(a3 + 0xEB4, rd16(a3 + 0xEB4) - (d0 & 0xFFFF));  /* 04309C  sub.w  d0,$eb4(a3) */
        }
    }
L_0430A0:
    fz = !(rd8(a3 + 0x1E0A) & 0x8);                      /* 0430A0  btst  #$3,$1e0a(a3) */
    /* 0430A6  bne  $430ae */
    if (COND(7)) {
        wr8(a3 + 0xEB6, rd8(0x423DF));                       /* 0430A8  move.b  $423df(pc),$eb6(a3) */
    }
L_0430AE:
    /* 0430AE  subq.b  #1,$1e13(a3) */
    {
        uint32_t s = 1;
        uint32_t d = rd8(a3 + 0x1E13);
        uint32_t r = (d - s) & 0xFF;
        wr8(a3 + 0x1E13, r);
        fz = (r == 0);
        fx = (s > d);
    }
    /* 0430B2  bne  $430d6 */
    if (COND(7)) {
        wr8(a3 + 0x1E13, rd8(0x43339));                      /* 0430B4  move.b  $43339(pc),$1e13(a3) */
        a2 = rd32(0x43344);                                  /* 0430BA  movea.l  $43344(pc),a2 */
        /* 0430BE  move.b  (a2)+,d0 */
        {
            uint32_t v = rd8(a2);
            a2 += 1;
            SETB(d0, v);
            FL_LOGIC(v, 0xFF, 0x80);
        }
        /* 0430C0  bpl  $430ce */
        if (COND(11)) {
            /* 0430C2  cmp.b  #$80,d0 */
            /* 0430C6  bne  $430d6 */
            if ((d0 & 0xFF) != 0x80) {
                goto L_0430D6;
            }
            a2 = rd32(0x43340);                                  /* 0430C8  movea.l  $43340(pc),a2 */
            /* 0430CC  move.b  (a2)+,d0 */
            SETB(d0, rd8(a2));
            a2 += 1;
        }
        wr32(a3 + 0x1E1A, a2);                               /* 0430CE  move.l  a2,$1e1a(a3) */
        wr8(a3 + 0xEBA, (d0 & 0xFF));                        /* 0430D2  move.b  d0,$eba(a3) */
    }
L_0430D6:
    RTS();                                               /* 0430D6  rts */
}

/* 043278  game routine */
void sub_043278(void)
{
    /* 043278  move.b  -(a0),$1d70(a6) */
    a0 -= 1;
    {
        uint32_t v = rd8(a0);
        wr8(a6 + 0x1D70, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
    MISS(0x4327CU);
}

/* 0432FA  game routine */
void sub_0432FA(void)
{
    SETB(d7, d7 | 0xC0);                                 /* 0432FA  ori.b  #$c0,d7 */
    /* 0432FE  ori.b  #$0,(a4)+ */
    {
        uint32_t s = 0;
        uint32_t ea = a4;
        a4 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
    }
    /* 043302  btst  d0,d1 */
    /* 043304  btst  #$0,d3 */
    SETB(d3, d3 | 0);                                    /* 043308  ori.b  #$0,d3 */
    SETB(d4, d4 | 0xEA);                                 /* 04330C  ori.b  #$ea,d4 */
    /* 043310  ori.b  #$f1,d4 */
    SETB(d4, d4 | 0xF1);
    { sub_043314(); return; }
}

/* 043314  game routine */
void sub_043314(void)
{
    SETB(d0, d0 | 0);                                    /* 043314  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043318  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 04331C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043320  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043324  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043328  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 04332C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043330  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043334  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043338  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 04333C  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043340  ori.b  #$0,d0 */
    /* 043344  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);
    { sub_043348(); return; }
}

/* 04332E  game routine */
void sub_04332E(void)
{
    SETB(d0, d0 | 0);                                    /* 04332E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043332  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043336  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 04333A  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 04333E  ori.b  #$0,d0 */
    SETB(d0, d0 | 0);                                    /* 043342  ori.b  #$0,d0 */
    SETB(d0, d0 | 0x3A);                                 /* 043346  ori.b  #$3a,d0 */
    /* 04334A  ori.b  #$0,(a2)+ */
    {
        uint32_t s = 0;
        uint32_t ea = a2;
        a2 += 1;
        uint32_t d = rd8(ea);
        uint32_t r = (d | s) & 0xFF;
        wr8(ea, r);
        FL_LOGIC(r, 0xFF, 0x80);
    }
    MISS(0x4334EU);
}

/* 043348  game routine */
void sub_043348(void)
{
    SETB(d0, rd8(0x43364));                              /* 043348  move.b  $43364(pc),d0 */
    SETB(d0, d0 & 0x48);                                 /* 04334C  andi.b  #$48,d0 */
    SETB(d0, d0 + 0x38);                                 /* 043350  addi.b  #$38,d0 */
    SETB(d0, shift_op(1, 1, (d0 & 0xFF), 2, 8));         /* 043354  lsl.b  #2,d0 */
    wr16(a3 + 0x1E3C, shift_op(2, 1, rd16(a3 + 0x1E3C), 1, 16)); /* 043356  roxl.w  $1e3c(a3) */
    wr16(a3 + 0x1E3A, shift_op(2, 1, rd16(a3 + 0x1E3A), 1, 16)); /* 04335A  roxl.w  $1e3a(a3) */
    SETB(d0, rd8(0x43364));                              /* 04335E  move.b  $43364(pc),d0 */
    RTS();                                               /* 043362  rts */
}

