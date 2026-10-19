/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 3F8FCE  game routine */
void sub_3F8FCE(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a1 = 0x3FB35E;                                       /* 3F8FCE  lea  $3fb35e(pc),a1 */
    d1 = 6;                                              /* 3F8FD2  moveq  #6,d1 */
    do {
        BUDGET_CHECK(0x3F8FD4U);
L_3F8FD4:
        /* 3F8FD4  move.l  (a0)+,(a1)+ */
        {
            uint32_t v = rd32(a0);
            a0 += 4;
            wr32(a1, v);
            a1 += 4;
        }
        /* 3F8FD6  dbra  d1,$3f8fd4 */
        SETW(d1, d1 - 1);
    } while ((d1 & 0xFFFF) != 0xFFFF);
L_3F8FDA:
    wr16(a0 - 0x10, 0);                                  /* 3F8FDA  clr.w  -$10(a0) */
    /* 3F8FDE  move.b  $3f80af(pc),d1 */
    {
        uint32_t v = rd8(0x3F80AF);
        SETB(d1, v);
        FL_LOGIC(v, 0xFF, 0x80);
    }
L_3F8FE2:
    /* 3F8FE2  bne  $3f95c4 */
    if (COND(7)) {
L_3F8FE6:
        a1 = a1 + 0x36;                                      /* 3F8FE6  lea  $36(a1),a1 */
L_3F8FEA:
        /* 3F8FEA  move  sr,-(a1) */
        { uint32_t ea; (void)ea;
        a1 -= 2; ea = a1;
        wr16(ea, get_sr());
        }
L_3F8FEC:
        /* 3F8FEC  move  #$2700,sr */
        { uint32_t s, ea1; (void)ea1;
        s = 0x2700U;
        set_sr(s); }
L_3F8FF0:
        /* 3F8FF0  movem.l  a7/a6/a5/a4/a3/a2/d7/d6/d5/d4/d3/d2,-(a1) */
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
L_3F8FF4:
        a2 = other_sp;                                       /* 3F8FF4  move  usp,a2 */
L_3F8FF6:
        /* 3F8FF6  move.l  a2,-(a1) */
        {
            uint32_t v = a2;
            a1 -= 4;
            wr32(a1, v);
        }
L_3F8FF8:
        a2 = 0x3FB7A2;                                       /* 3F8FF8  lea  $3fb7a2(pc),a2 */
L_3F8FFC:
        other_sp = a2;                                       /* 3F8FFC  move  a2,usp */
L_3F8FFE:
        a7 = 0x3FB612;                                       /* 3F8FFE  lea  $3fb612(pc),a7 */
L_3F9002:
        CALL(sub_3F97C8, 0x3F9006U);                         /* 3F9002  bsr  $3f97c8 */
L_3F9006:
        if (hold_loader_text()) { g_cpu.pc = 0x3F9006U; yield_to_host(RC_STOP); }          /* (added by hand) the loader's text stays on screen at full brightness before its fades */
        CALL(sub_3F9170, 0x3F900AU);                         /* 3F9006  bsr  $3f9170 */
L_3F900A:
        a6 = 0x3FB35E;                                       /* 3F900A  lea  $3fb35e(pc),a6 */
        /* 3F900E  clr.w  -(a7) */
        a7 -= 2;
        wr16(a7, 0);
        a0 = a6 + 0xE;                                       /* 3F9010  lea  $e(a6),a0 */
        PUSH32(a0);                                          /* 3F9014  pea  (a0) */
        d0 = 0x3D;                                           /* 3F9016  moveq  #61,d0 */
        /* 3F9018  cmpi.w  #$6352,$c(a6) */
        /* 3F901E  bne  $3f9028 */
        if (rd16(a6 + 0xC) == 0x6352) {
            d0 = 0x3C;                                           /* 3F9020  moveq  #60,d0 */
            wr16(a6 + 0xC, 0x5772);                              /* 3F9022  move.w  #$5772,$c(a6) */
        }
L_3F9028:
        /* 3F9028  move.w  d0,-(a7) */
        {
            uint32_t v = (d0 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
        SOFT(0x3F902AU);                                     /* 3F902A  trap  #1 */
L_3F902C:
        a7 += 8;                                             /* 3F902C  addq.l  #8,a7 */
        /* 3F902E  move.w  d0,d7 */
        /* 3F9030  bpl  $3f903c */
        SETW(d7, (d0 & 0xFFFF));
        if ((int16_t)(d7 & 0xFFFF) < 0) {
            a1 = 0x3F914E;                                       /* 3F9032  lea  $3f914e(pc),a1 */
            wr32(a1, d0);                                        /* 3F9036  move.l  d0,(a1) */
            /* 3F9038  bra  $3f912c */
        } else {
L_3F903C:
            /* 3F903C  tst.l  $4(a6) */
            /* 3F9040  beq  $3f9054 */
            if (rd32(a6 + 0x4) != 0) {
                /* 3F9042  clr.w  -(a7) */
                a7 -= 2;
                wr16(a7, 0);
                /* 3F9044  move.w  d7,-(a7) */
                {
                    uint32_t v = (d7 & 0xFFFF);
                    a7 -= 2;
                    wr16(a7, v);
                }
                /* 3F9046  move.l  $4(a6),-(a7) */
                {
                    uint32_t v = rd32(a6 + 0x4);
                    a7 -= 4;
                    wr32(a7, v);
                }
                /* 3F904A  move.w  #$42,-(a7) */
                {
                    uint32_t v = 0x42;
                    a7 -= 2;
                    wr16(a7, v);
                    FL_LOGIC(v, 0xFFFF, 0x8000);
                }
                SOFT(0x3F904EU);                                     /* 3F904E  trap  #1 */
L_3F9050:
                a7 = a7 + 0xA;                                       /* 3F9050  lea  $a(a7),a7 */
            }
L_3F9054:
            d0 = rd32(a6);                                       /* 3F9054  move.l  (a6),d0 */
            /* 3F9056  move.b  $3f809a(pc),d2 */
            /* 3F905A  beq  $3f90f2 */
            SETB(d2, rd8(0x3F809A));
            if ((d2 & 0xFF) != 0) {
L_3F905E:
                d2 = rd32(0x3F80A0);                                 /* 3F905E  move.l  $3f80a0(pc),d2 */
                /* 3F9062  move.l  $3f809c(pc),d1 */
                /* 3F9066  beq  $3f9078 */
                d1 = rd32(0x3F809C);
                if (d1 != 0) {
L_3F9068:
                    d2 -= rd32(0x3F8094);                                /* 3F9068  sub.l  $3f8094(pc),d2 */
                    d2 += rd32(0x3F809C);                                /* 3F906C  add.l  $3f809c(pc),d2 */
                    d2 += 8;                                             /* 3F9070  addq.l  #8,d2 */
                    /* 3F9072  add.l  $3f809c(pc),d0 */
                    {
                        uint32_t s = rd32(0x3F809C);
                        uint32_t d = d0;
                        uint32_t r = (d + s) & 0xFFFFFFFF;
                        d0 = r;
                        fx = (r < s);
                    }
                    /* 3F9076  bra  $3f9082 */
                } else {
                    d2 = rd32(0x3F80A0);                                 /* 3F9078  move.l  $3f80a0(pc),d2 */
                    d0 += rd32(0x3F8094);                                /* 3F907C  add.l  $3f8094(pc),d0 */
                    /* 3F9080  subq.l  #8,d0 */
                    {
                        uint32_t s = 8;
                        uint32_t d = d0;
                        uint32_t r = (d - s) & 0xFFFFFFFF;
                        d0 = r;
                        fx = (s > d);
                    }
                }
L_3F9082:
                /* 3F9082  cmp.l  d0,d2 */
                /* 3F9084  ble  $3f90e8 */
                if ((int32_t)d2 > (int32_t)d0) {
                    d0 += rd32(a6 + 0x8);                                /* 3F9086  add.l  $8(a6),d0 */
                    /* 3F908A  cmp.l  d0,d2 */
                    /* 3F908C  bge  $3f90ec */
                    if ((int32_t)d2 >= (int32_t)d0) {
                        goto L_3F90EC;
                    }
                    d3 = rd32(a6 + 0x8);                                 /* 3F908E  move.l  $8(a6),d3 */
                    d1 = d0;                                             /* 3F9092  move.l  d0,d1 */
                    d1 -= rd32(a6 + 0x8);                                /* 3F9094  sub.l  $8(a6),d1 */
                    d0 -= d2;                                            /* 3F9098  sub.l  d2,d0 */
                    /* 3F909A  sub.l  d0,d3 */
                    {
                        uint32_t s = d0;
                        uint32_t d = d3;
                        uint32_t r = (d - s) & 0xFFFFFFFF;
                        d3 = r;
                        fx = (s > d);
                    }
                    /* 3F909C  move.l  d0,-(a7) */
                    {
                        uint32_t v = d0;
                        a7 -= 4;
                        wr32(a7, v);
                    }
                    /* 3F909E  move.l  d3,-(a7) */
                    {
                        uint32_t v = d3;
                        a7 -= 4;
                        wr32(a7, v);
                    }
                    /* 3F90A0  move.l  d1,-(a7) */
                    {
                        uint32_t v = d1;
                        a7 -= 4;
                        wr32(a7, v);
                    }
                    /* 3F90A2  move.l  d3,-(a7) */
                    {
                        uint32_t v = d3;
                        a7 -= 4;
                        wr32(a7, v);
                    }
                    /* 3F90A4  move.w  d7,-(a7) */
                    {
                        uint32_t v = (d7 & 0xFFFF);
                        a7 -= 2;
                        wr16(a7, v);
                    }
                    d0 = 0x3F;                                           /* 3F90A6  moveq  #63,d0 */
                    /* 3F90A8  cmpi.w  #$5772,$c(a6) */
                    /* 3F90AE  bne  $3f90b2 */
                    if (rd16(a6 + 0xC) == 0x5772) {
                        d0 = 0x40;                                           /* 3F90B0  moveq  #64,d0 */
                    }
                    /* 3F90B2  move.w  d0,-(a7) */
                    {
                        uint32_t v = (d0 & 0xFFFF);
                        a7 -= 2;
                        wr16(a7, v);
                        FL_LOGIC(v, 0xFFFF, 0x8000);
                    }
                    SOFT(0x3F90B4U);                                     /* 3F90B4  trap  #1 */
L_3F90B6:
                    a7 = a7 + 0xC;                                       /* 3F90B6  lea  $c(a7),a7 */
                    /* 3F90BA  move.l  (a7)+,d3 */
                    d3 = rd32(a7);
                    a7 += 4;
                    d6 = d0;                                             /* 3F90BC  move.l  d0,d6 */
                    /* 3F90BE  cmp.l  d0,d3 */
                    /* 3F90C0  bgt  $3f911c */
                    if ((int32_t)d3 > (int32_t)d0) {
                        goto L_3F911C;
                    }
                    d0 = rd32(a6);                                       /* 3F90C4  move.l  (a6),d0 */
                    /* 3F90C6  add.l  d6,d0 */
                    {
                        uint32_t s = d6;
                        uint32_t d = d0;
                        uint32_t r = (d + s) & 0xFFFFFFFF;
                        d0 = r;
                        fx = (r < s);
                    }
                    /* 3F90C8  move.l  (a7)+,d1 */
                    d1 = rd32(a7);
                    a7 += 4;
                    /* 3F90CA  move.l  d0,-(a7) */
                    {
                        uint32_t v = d0;
                        a7 -= 4;
                        wr32(a7, v);
                    }
                    /* 3F90CC  move.l  d1,-(a7) */
                    {
                        uint32_t v = d1;
                        a7 -= 4;
                        wr32(a7, v);
                    }
                    /* 3F90CE  move.w  d7,-(a7) */
                    {
                        uint32_t v = (d7 & 0xFFFF);
                        a7 -= 2;
                        wr16(a7, v);
                    }
                    d0 = 0x3F;                                           /* 3F90D0  moveq  #63,d0 */
                    /* 3F90D2  cmpi.w  #$5772,$c(a6) */
                    /* 3F90D8  bne  $3f90dc */
                    if (rd16(a6 + 0xC) == 0x5772) {
                        d0 = 0x40;                                           /* 3F90DA  moveq  #64,d0 */
                    }
                    /* 3F90DC  move.w  d0,-(a7) */
                    {
                        uint32_t v = (d0 & 0xFFFF);
                        a7 -= 2;
                        wr16(a7, v);
                        FL_LOGIC(v, 0xFFFF, 0x8000);
                    }
                    SOFT(0x3F90DEU);                                     /* 3F90DE  trap  #1 */
L_3F90E0:
                    a7 = a7 + 0xC;                                       /* 3F90E0  lea  $c(a7),a7 */
                    /* 3F90E4  add.l  d6,d0 */
                    {
                        uint32_t s = d6;
                        uint32_t d = d0;
                        uint32_t r = (d + s) & 0xFFFFFFFF;
                        d0 = r;
                        fx = (r < s);
                    }
                    /* 3F90E6  bra  $3f911c */
                    goto L_3F911C;
                }
L_3F90E8:
                d0 = rd32(a6);                                       /* 3F90E8  move.l  (a6),d0 */
                /* 3F90EA  bra  $3f9100 */
                goto L_3F9100;
L_3F90EC:
                /* 3F90EC  sub.l  $8(a6),d0 */
                {
                    uint32_t s = rd32(a6 + 0x8);
                    uint32_t d = d0;
                    uint32_t r = (d - s) & 0xFFFFFFFF;
                    d0 = r;
                    fx = (s > d);
                }
                /* 3F90F0  bra  $3f9100 */
            } else {
                /* 3F90F2  cmp.l  #$300000,d0 */
                /* 3F90F8  bcc  $3f9100 */
                if (d0 < 0x300000) {
                    /* 3F90FA  addi.l  #$200000,d0 */
                    {
                        uint32_t s = 0x200000;
                        uint32_t d = d0;
                        uint32_t r = (d + s) & 0xFFFFFFFF;
                        d0 = r;
                        fx = (r < s);
                    }
                }
            }
L_3F9100:
            /* 3F9100  move.l  d0,-(a7) */
            {
                uint32_t v = d0;
                a7 -= 4;
                wr32(a7, v);
            }
            /* 3F9102  move.l  $8(a6),-(a7) */
            {
                uint32_t v = rd32(a6 + 0x8);
                a7 -= 4;
                wr32(a7, v);
            }
            /* 3F9106  move.w  d7,-(a7) */
            {
                uint32_t v = (d7 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
            }
            d0 = 0x3F;                                           /* 3F9108  moveq  #63,d0 */
            /* 3F910A  cmpi.w  #$5772,$c(a6) */
            /* 3F9110  bne  $3f9114 */
            if (rd16(a6 + 0xC) == 0x5772) {
                d0 = 0x40;                                           /* 3F9112  moveq  #64,d0 */
            }
L_3F9114:
            /* 3F9114  move.w  d0,-(a7) */
            {
                uint32_t v = (d0 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
                FL_LOGIC(v, 0xFFFF, 0x8000);
            }
            SOFT(0x3F9116U);                                     /* 3F9116  trap  #1 */
L_3F9118:
            a7 = a7 + 0xC;                                       /* 3F9118  lea  $c(a7),a7 */
L_3F911C:
            a1 = 0x3F914E;                                       /* 3F911C  lea  $3f914e(pc),a1 */
            wr32(a1, d0);                                        /* 3F9120  move.l  d0,(a1) */
            /* 3F9122  move.w  d7,-(a7) */
            {
                uint32_t v = (d7 & 0xFFFF);
                a7 -= 2;
                wr16(a7, v);
            }
            /* 3F9124  move.w  #$3e,-(a7) */
            {
                uint32_t v = 0x3E;
                a7 -= 2;
                wr16(a7, v);
                FL_LOGIC(v, 0xFFFF, 0x8000);
            }
            SOFT(0x3F9128U);                                     /* 3F9128  trap  #1 */
L_3F912A:
            a7 += 4;                                             /* 3F912A  addq.l  #4,a7 */
        }
        /* 3F912C  move  #$2700,sr */
        { uint32_t s, ea1; (void)ea1;
        s = 0x2700U;
        set_sr(s); }
L_3F9130:
        CALL(sub_3F937C, 0x3F9134U);                         /* 3F9130  bsr  $3f937c */
L_3F9134:
        a6 = 0x3FB3C2;                                       /* 3F9134  lea  $3fb3c2(pc),a6 */
        a1 = 0xFFFFFA07;                                     /* 3F9138  lea  $fa07.w,a1 */
        /* 3F913C  move.b  (a6)+,(a1) */
        wr8(a1, rd8(a6));
        a6 += 1;
        a1 += 2;                                             /* 3F913E  addq.l  #2,a1 */
        /* 3F9140  move.b  (a6)+,(a1) */
        wr8(a1, rd8(a6));
        a6 += 1;
        a1 = a1 + 0xA;                                       /* 3F9142  lea  $a(a1),a1 */
        /* 3F9146  move.b  (a6)+,(a1) */
        wr8(a1, rd8(a6));
        a6 += 1;
        a1 += 2;                                             /* 3F9148  addq.l  #2,a1 */
        wr8(a1, rd8(a6));                                    /* 3F914A  move.b  (a6),(a1) */
        /* 3F914C  move.l  #$0,d0 */
        d0 = 0;
        { sub_3F9152(); return; }
    }
    a0 = 0x3F96B6;                                       /* 3F95C4  lea  $3f96b6(pc),a0 */
    /* 3F95C8  movem.l  a7/a6/a5/a4/a3/a2/a1/d7/d6/d5/d4/d3/d2/d1,-(a0) */
    { uint32_t ea;
    ea = a0;
    ea -= 4; wr32(ea, a7);
    ea -= 4; wr32(ea, a6);
    ea -= 4; wr32(ea, a5);
    ea -= 4; wr32(ea, a4);
    ea -= 4; wr32(ea, a3);
    ea -= 4; wr32(ea, a2);
    ea -= 4; wr32(ea, a1);
    ea -= 4; wr32(ea, d7);
    ea -= 4; wr32(ea, d6);
    ea -= 4; wr32(ea, d5);
    ea -= 4; wr32(ea, d4);
    ea -= 4; wr32(ea, d3);
    ea -= 4; wr32(ea, d2);
    ea -= 4; wr32(ea, d1);
    a0 = ea;
    }
    a6 = rd32(0x3F80A6);                                 /* 3F95CC  movea.l  $3f80a6(pc),a6 */
    /* 3F95D0  tst.w  -$cc(a6) */
    /* 3F95D4  beq  $3f95dc */
    if (rd16(a6 - 0xCC) != 0) {
        CALL(sub_3F95A2, 0x3F95D8U);                         /* 3F95D6  bsr  $3f95a2 */
L_3F95D8:
        wr16(a6 - 0xCC, 0);                                  /* 3F95D8  clr.w  -$cc(a6) */
    }
    d0 = 0;                                              /* 3F95DC  moveq  #0,d0 */
    a3 = 0x3FB35E;                                       /* 3F95DE  lea  $3fb35e(pc),a3 */
    /* 3F95E2  cmpi.w  #$5772,$c(a3) */
    /* 3F95E8  bne  $3f95ec */
    if (rd16(a3 + 0xC) == 0x5772) {
        d0 = 1;                                              /* 3F95EA  moveq  #1,d0 */
    }
    /* 3F95EC  move.w  d0,-(a7) */
    {
        uint32_t v = (d0 & 0xFFFF);
        a7 -= 2;
        wr16(a7, v);
    }
    PUSH32(a3 + 0xE);                                    /* 3F95EE  pea  $e(a3) */
    /* 3F95F2  move.w  #$3d,-(a7) */
    {
        uint32_t v = 0x3D;
        a7 -= 2;
        wr16(a7, v);
        FL_LOGIC(v, 0xFFFF, 0x8000);
    }
    a1 = rd32(a6 + 0x14);                                /* 3F95F6  movea.l  $14(a6),a1 */
    /* 3F95FA  jsr  (a1) */
    { uint32_t ea;
    ea = a1;
    CALL_DYN(ea, 0x3F95FCU); }
L_3F95FC:
    a7 += 8;                                             /* 3F95FC  addq.l  #8,a7 */
    /* 3F95FE  move.w  d0,d7 */
    /* 3F9600  bpl  $3f960c */
    SETW(d7, (d0 & 0xFFFF));
    if ((int16_t)(d7 & 0xFFFF) < 0) {
        a1 = 0x3F9674;                                       /* 3F9602  lea  $3f9674(pc),a1 */
        wr32(a1, d0);                                        /* 3F9606  move.l  d0,(a1) */
        /* 3F9608  bra  $3f965c */
    } else {
        /* 3F960C  clr.w  -(a7) */
        a7 -= 2;
        wr16(a7, 0);
        /* 3F960E  move.w  d7,-(a7) */
        {
            uint32_t v = (d7 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
        }
        a3 = 0x3FB35E;                                       /* 3F9610  lea  $3fb35e(pc),a3 */
        /* 3F9614  move.l  $4(a3),-(a7) */
        {
            uint32_t v = rd32(a3 + 0x4);
            a7 -= 4;
            wr32(a7, v);
        }
        /* 3F9618  move.w  #$42,-(a7) */
        {
            uint32_t v = 0x42;
            a7 -= 2;
            wr16(a7, v);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
        a1 = rd32(a6 + 0x14);                                /* 3F961C  movea.l  $14(a6),a1 */
        /* 3F9620  jsr  (a1) */
        { uint32_t ea;
        ea = a1;
        CALL_DYN(ea, 0x3F9622U); }
L_3F9622:
        a7 = a7 + 0xA;                                       /* 3F9622  lea  $a(a7),a7 */
        a3 = 0x3FB35E;                                       /* 3F9626  lea  $3fb35e(pc),a3 */
        /* 3F962A  move.l  (a3),-(a7) */
        {
            uint32_t v = rd32(a3);
            a7 -= 4;
            wr32(a7, v);
        }
        /* 3F962C  move.l  $8(a3),-(a7) */
        {
            uint32_t v = rd32(a3 + 0x8);
            a7 -= 4;
            wr32(a7, v);
        }
        /* 3F9630  move.w  d7,-(a7) */
        {
            uint32_t v = (d7 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
        }
        d0 = 0x3F;                                           /* 3F9632  moveq  #63,d0 */
        /* 3F9634  cmpi.w  #$5772,$c(a3) */
        /* 3F963A  bne  $3f963e */
        if (rd16(a3 + 0xC) == 0x5772) {
            d0 = 0x40;                                           /* 3F963C  moveq  #64,d0 */
        }
        /* 3F963E  move.w  d0,-(a7) */
        {
            uint32_t v = (d0 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
        a1 = rd32(a6 + 0x14);                                /* 3F9640  movea.l  $14(a6),a1 */
        /* 3F9644  jsr  (a1) */
        { uint32_t ea;
        ea = a1;
        CALL_DYN(ea, 0x3F9646U); }
L_3F9646:
        a7 = a7 + 0xC;                                       /* 3F9646  lea  $c(a7),a7 */
        a1 = 0x3F9674;                                       /* 3F964A  lea  $3f9674(pc),a1 */
        wr32(a1, d0);                                        /* 3F964E  move.l  d0,(a1) */
        /* 3F9650  move.w  d7,-(a7) */
        {
            uint32_t v = (d7 & 0xFFFF);
            a7 -= 2;
            wr16(a7, v);
        }
        /* 3F9652  move.w  #$3e,-(a7) */
        {
            uint32_t v = 0x3E;
            a7 -= 2;
            wr16(a7, v);
            FL_LOGIC(v, 0xFFFF, 0x8000);
        }
        a1 = rd32(a6 + 0x14);                                /* 3F9656  movea.l  $14(a6),a1 */
        /* 3F965A  jsr  (a1) */
        { uint32_t ea;
        ea = a1;
        CALL_DYN(ea, 0x3F965CU); }
    }
L_3F965C:
    a0 = 0x3F967E;                                       /* 3F965C  lea  $3f967e(pc),a0 */
    /* 3F9660  movem.l  (a0)+,d1-d7/a1-a7 */
    { uint32_t ea;
    ea = a0;
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
    a1 = rd32(ea);
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
    a0 = ea;
    }
    for (;;) {
        BUDGET_CHECK(0x3F9664U);
L_3F9664:
        fz = !(rd8(HW_IKBD_CTRL) & 0x1);                     /* 3F9664  btst  #$0,$fc00.w */
        /* 3F966A  beq  $3f9672 */
        if (COND(7)) {
            goto L_3F9672;
        }
        (void)rd8(HW_IKBD_DATA);                             /* 3F966C  tst.b  $fc02.w */
        /* 3F9670  bra  $3f9664 */
    }
L_3F9672:
    /* 3F9672  move.l  #$0,d0 */
    d0 = 0;
    { sub_3F9678(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x3F8FD4U: goto L_3F8FD4;
        case 0x3F8FDAU: goto L_3F8FDA;
        case 0x3F8FE2U: goto L_3F8FE2;
        case 0x3F8FE6U: goto L_3F8FE6;
        case 0x3F8FEAU: goto L_3F8FEA;
        case 0x3F8FECU: goto L_3F8FEC;
        case 0x3F8FF0U: goto L_3F8FF0;
        case 0x3F8FF4U: goto L_3F8FF4;
        case 0x3F8FF6U: goto L_3F8FF6;
        case 0x3F8FF8U: goto L_3F8FF8;
        case 0x3F8FFCU: goto L_3F8FFC;
        case 0x3F8FFEU: goto L_3F8FFE;
        case 0x3F9002U: goto L_3F9002;
        case 0x3F9006U: goto L_3F9006;
        case 0x3F900AU: goto L_3F900A;
        case 0x3F9028U: goto L_3F9028;
        case 0x3F902CU: goto L_3F902C;
        case 0x3F903CU: goto L_3F903C;
        case 0x3F9050U: goto L_3F9050;
        case 0x3F9054U: goto L_3F9054;
        case 0x3F905EU: goto L_3F905E;
        case 0x3F9068U: goto L_3F9068;
        case 0x3F9082U: goto L_3F9082;
        case 0x3F90B6U: goto L_3F90B6;
        case 0x3F90E0U: goto L_3F90E0;
        case 0x3F90E8U: goto L_3F90E8;
        case 0x3F9100U: goto L_3F9100;
        case 0x3F9114U: goto L_3F9114;
        case 0x3F9118U: goto L_3F9118;
        case 0x3F912AU: goto L_3F912A;
        case 0x3F9130U: goto L_3F9130;
        case 0x3F9134U: goto L_3F9134;
        case 0x3F95D8U: goto L_3F95D8;
        case 0x3F95FCU: goto L_3F95FC;
        case 0x3F9622U: goto L_3F9622;
        case 0x3F9646U: goto L_3F9646;
        case 0x3F965CU: goto L_3F965C;
        case 0x3F9664U: goto L_3F9664;
        }
    }
}

