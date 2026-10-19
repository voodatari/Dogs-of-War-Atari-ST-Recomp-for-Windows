/* C translation of the 68000 code of the game (one function per routine). It is now the source: edit it by hand. */
#include "cpu.h"
#include "game.h"
#include "globals.h"

/* 0045D6  game routine */
void sub_0045D6(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = V_18E4;                                         /* 0045D6  lea  $18e4(a5),a0 */
    a1 = V_1DB8;                                         /* 0045DA  lea  $1db8(a5),a1 */
    CALL(sub_00759C, 0x45E2U);                           /* 0045DE  bsr  $759c */
L_0045E2:
    CALL(sub_0090AA, 0x45E6U);                           /* 0045E2  bsr  $90aa */
L_0045E6:
    wr16(V_SCROLL, 0);                                   /* 0045E6  clr.w  $280(a5) */
    a0 = 0x73BAE;                                        /* 0045EA  lea  $73bae.l,a0 */
    wr16(V_0246, rd16(V_0246) + 1);                      /* 0045F0  addq.w  #1,$246(a5) */
    SETW(d1, rd16(V_0246));                              /* 0045F4  move.w  $246(a5),d1 */
    SETW(d1, (d1 & 0xFFFF) << 1);                        /* 0045F8  lsl.w  #1,d1 */
    SETW(d0, rd16(a0 + 0x8 + SX16(d1)));                 /* 0045FA  move.w  $8(a0,d1.w),d0 */
    a0 = a0 + SX16(d0);                                  /* 0045FE  lea  $0(a0,d0.w),a0 */
    wr32(V_1DF6, a0);                                    /* 004602  move.l  a0,$1df6(a5) */
    wr8(V_1DF4, 0xB8);                                   /* 004606  move.b  #$b8,$1df4(a5) */
    wr16(V_HSCROLL_PHASE, 0);                            /* 00460C  clr.w  $24a(a5) */
    wr16(V_024C, rd16(V_024C) + 1);                      /* 004610  addq.w  #1,$24c(a5) */
    wr16(V_0248, 2);                                     /* 004614  move.w  #$2,$248(a5) */
    fz = !(rd8(V_024D) & 0x1);                           /* 00461A  btst  #$0,$24d(a5) */
    /* 004620  beq  $4626 */
    if (COND(6)) {
        /* 004622  neg.w  $248(a5) */
        { uint32_t d, r, ea; (void)ea;
        ea = a5 + 0x248U;
        d = rd16(ea);
        r = (0 - d) & 0xFFFFU;
        wr16(ea, r);
        FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
    }
    a1 = 0x5649A;                                        /* 004626  lea  $5649a.l,a1 */
    a0 = 0x28000;                                        /* 00462C  lea  $28000.l,a0 */
    SETW(d0, rd16(V_0282));                              /* 004632  move.w  $282(a5),d0 */
    d0 = (d0 & 0xFFFF) * 0x400;                          /* 004636  mulu.w  #$400,d0 */
    a1 = a1 + SX16(d0);                                  /* 00463A  lea  $0(a1,d0.w),a1 */
    /* 00463E  moveq  #31,d7 */
    d7 = 0x1F;
    { sub_004640(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x45E2U: goto L_0045E2;
        case 0x45E6U: goto L_0045E6;
        }
    }
}

/* 004640  game routine */
void sub_004640(void)
{
    /* 004640  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 004642  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 004644  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 004646  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 004648  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00464A  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00464C  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    /* 00464E  move.l  (a1)+,(a0)+ */
    {
        uint32_t v = rd32(a1);
        a1 += 4;
        wr32(a0, v);
        a0 += 4;
    }
    { sub_004650(); return; }
}

/* 004650  game routine */
void sub_004650(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 004650  dbra  d7,$4640 */
    SETW(d7, d7 - 1);
    if ((d7 & 0xFFFF) != 0xFFFF) { sub_004640(); return; }
    a1 = 0x28000;                                        /* 004654  lea  $28000.l,a1 */
    a0 = a1 + 0x400;                                     /* 00465A  lea  $400(a1),a0 */
    SETW(d7, 0xE);                                       /* 00465E  move.w  #$e,d7 */
    do {
        BUDGET_CHECK(0x4662U);
L_004662:
        SETW(d6, 0x1F);                                      /* 004662  move.w  #$1f,d6 */
        d5 = 3;                                              /* 004666  moveq  #3,d5 */
        do {
            BUDGET_CHECK(0x4668U);
L_004668:
            /* 004668  move.w  (a1)+,d0 */
            SETW(d0, rd16(a1));
            a1 += 2;
            SETW(d1, rd16(a1 + 0x6));                            /* 00466A  move.w  $6(a1),d1 */
            SETW(d2, rd16(a1 + 0xE));                            /* 00466E  move.w  $e(a1),d2 */
            SETW(d3, rd16(a1 + 0x16));                           /* 004672  move.w  $16(a1),d3 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 004676  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 004678  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 00467A  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 00467C  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 00467E  roxl.w  #1,d4 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 004680  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 004682  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 004684  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 004686  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 004688  roxl.w  #1,d4 */
            SETW(d4, d4 & (d5 & 0xFFFF));                        /* 00468A  and.w  d5,d4 */
            SETW(d3, d3 | (d4 & 0xFFFF));                        /* 00468C  or.w  d4,d3 */
            /* 00468E  move.w  d0,(a0)+ */
            {
                uint32_t v = (d0 & 0xFFFF);
                wr16(a0, v);
                a0 += 2;
            }
            wr16(a0 + 0x6, (d1 & 0xFFFF));                       /* 004690  move.w  d1,$6(a0) */
            wr16(a0 + 0xE, (d2 & 0xFFFF));                       /* 004694  move.w  d2,$e(a0) */
            wr16(a0 + 0x16, (d3 & 0xFFFF));                      /* 004698  move.w  d3,$16(a0) */
            /* 00469C  move.w  (a1)+,d0 */
            SETW(d0, rd16(a1));
            a1 += 2;
            SETW(d1, rd16(a1 + 0x6));                            /* 00469E  move.w  $6(a1),d1 */
            SETW(d2, rd16(a1 + 0xE));                            /* 0046A2  move.w  $e(a1),d2 */
            SETW(d3, rd16(a1 + 0x16));                           /* 0046A6  move.w  $16(a1),d3 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 0046AA  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 0046AC  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 0046AE  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0046B0  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 0046B2  roxl.w  #1,d4 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 0046B4  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 0046B6  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 0046B8  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0046BA  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 0046BC  roxl.w  #1,d4 */
            SETW(d4, d4 & (d5 & 0xFFFF));                        /* 0046BE  and.w  d5,d4 */
            SETW(d3, d3 | (d4 & 0xFFFF));                        /* 0046C0  or.w  d4,d3 */
            /* 0046C2  move.w  d0,(a0)+ */
            {
                uint32_t v = (d0 & 0xFFFF);
                wr16(a0, v);
                a0 += 2;
            }
            wr16(a0 + 0x6, (d1 & 0xFFFF));                       /* 0046C4  move.w  d1,$6(a0) */
            wr16(a0 + 0xE, (d2 & 0xFFFF));                       /* 0046C8  move.w  d2,$e(a0) */
            wr16(a0 + 0x16, (d3 & 0xFFFF));                      /* 0046CC  move.w  d3,$16(a0) */
            /* 0046D0  move.w  (a1)+,d0 */
            SETW(d0, rd16(a1));
            a1 += 2;
            SETW(d1, rd16(a1 + 0x6));                            /* 0046D2  move.w  $6(a1),d1 */
            SETW(d2, rd16(a1 + 0xE));                            /* 0046D6  move.w  $e(a1),d2 */
            SETW(d3, rd16(a1 + 0x16));                           /* 0046DA  move.w  $16(a1),d3 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 0046DE  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 0046E0  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 0046E2  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0046E4  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 0046E6  roxl.w  #1,d4 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 0046E8  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 0046EA  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 0046EC  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 0046EE  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 0046F0  roxl.w  #1,d4 */
            SETW(d4, d4 & (d5 & 0xFFFF));                        /* 0046F2  and.w  d5,d4 */
            SETW(d3, d3 | (d4 & 0xFFFF));                        /* 0046F4  or.w  d4,d3 */
            /* 0046F6  move.w  d0,(a0)+ */
            {
                uint32_t v = (d0 & 0xFFFF);
                wr16(a0, v);
                a0 += 2;
            }
            wr16(a0 + 0x6, (d1 & 0xFFFF));                       /* 0046F8  move.w  d1,$6(a0) */
            wr16(a0 + 0xE, (d2 & 0xFFFF));                       /* 0046FC  move.w  d2,$e(a0) */
            wr16(a0 + 0x16, (d3 & 0xFFFF));                      /* 004700  move.w  d3,$16(a0) */
            /* 004704  move.w  (a1)+,d0 */
            SETW(d0, rd16(a1));
            a1 += 2;
            SETW(d1, rd16(a1 + 0x6));                            /* 004706  move.w  $6(a1),d1 */
            SETW(d2, rd16(a1 + 0xE));                            /* 00470A  move.w  $e(a1),d2 */
            SETW(d3, rd16(a1 + 0x16));                           /* 00470E  move.w  $16(a1),d3 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 004712  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 004714  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 004716  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 004718  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 00471A  roxl.w  #1,d4 */
            SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), 1, 16));      /* 00471C  lsl.w  #1,d3 */
            SETW(d2, shift_op(2, 1, (d2 & 0xFFFF), 1, 16));      /* 00471E  roxl.w  #1,d2 */
            SETW(d1, shift_op(2, 1, (d1 & 0xFFFF), 1, 16));      /* 004720  roxl.w  #1,d1 */
            SETW(d0, shift_op(2, 1, (d0 & 0xFFFF), 1, 16));      /* 004722  roxl.w  #1,d0 */
            SETW(d4, shift_op(2, 1, (d4 & 0xFFFF), 1, 16));      /* 004724  roxl.w  #1,d4 */
            SETW(d4, d4 & (d5 & 0xFFFF));                        /* 004726  and.w  d5,d4 */
            SETW(d3, d3 | (d4 & 0xFFFF));                        /* 004728  or.w  d4,d3 */
            /* 00472A  move.w  d0,(a0)+ */
            {
                uint32_t v = (d0 & 0xFFFF);
                wr16(a0, v);
                a0 += 2;
            }
            wr16(a0 + 0x6, (d1 & 0xFFFF));                       /* 00472C  move.w  d1,$6(a0) */
            wr16(a0 + 0xE, (d2 & 0xFFFF));                       /* 004730  move.w  d2,$e(a0) */
            wr16(a0 + 0x16, (d3 & 0xFFFF));                      /* 004734  move.w  d3,$16(a0) */
            a0 = a0 + 0x18;                                      /* 004738  lea  $18(a0),a0 */
            a1 = a1 + 0x18;                                      /* 00473C  lea  $18(a1),a1 */
            /* 004740  dbra  d6,$4668 */
            SETW(d6, d6 - 1);
        } while ((d6 & 0xFFFF) != 0xFFFF);
        /* 004744  dbra  d7,$4662 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    RTS();                                               /* 004748  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4662U: goto L_004662;
        case 0x4668U: goto L_004668;
        }
    }
}

/* 00474A  One logic frame in the horizontal-scroll stretches */
void frame_horizontal(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    do {
        BUDGET_CHECK(0x474AU);
        /* 00474A  bsr  $91e0 */
        HOOK(0, 0x474AU);
        CALL(wait_two_vbl, 0x474EU);
L_00474E:
        CALL(sub_007E06, 0x4752U);                           /* 00474E  bsr  $7e06 */
L_004752:
        /* 004752  bsr  $a544 */
        HOOK(1, 0x4752U);
        CALL(sub_00A544, 0x4756U);
L_004756:
        CALL(sub_00A5FC, 0x475AU);                           /* 004756  bsr  $a5fc */
L_00475A:
        CALL(sub_004A52, 0x475EU);                           /* 00475A  bsr  $4a52 */
L_00475E:
        CALL(sub_006C28, 0x4762U);                           /* 00475E  bsr  $6c28 */
L_004762:
        CALL(sub_006CFA, 0x4766U);                           /* 004762  bsr  $6cfa */
L_004766:
        CALL(sub_006950, 0x476AU);                           /* 004766  bsr  $6950 */
L_00476A:
        CALL(sub_004C74, 0x476EU);                           /* 00476A  bsr  $4c74 */
L_00476E:
        CALL(sub_005336, 0x4772U);                           /* 00476E  bsr  $5336 */
L_004772:
        CALL(sub_009FA4, 0x4776U);                           /* 004772  bsr  $9fa4 */
L_004776:
        CALL(sub_00B944, 0x477AU);                           /* 004776  bsr  $b944 */
L_00477A:
        CALL(sub_00AC5E, 0x477EU);                           /* 00477A  bsr  $ac5e */
L_00477E:
        CALL(sub_00922E, 0x4782U);                           /* 00477E  bsr  $922e */
L_004782:
        /* 004782  tst.w  $26e(a5) */
        /* 004786  beq  $478e */
        if (rd16(V_026E) != 0) {
            /* 004788  move.w  #$417,$ff8240.l */
            wr16(HW_PALETTE, 0x417);
        } else {
L_00478E:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 00478E  or.w  d0,d1 */
        }
        CALL(sub_00B09A, 0x4794U);                           /* 004790  bsr  $b09a */
L_004794:
        CALL(sub_00AFA2, 0x4798U);                           /* 004794  bsr  $afa2 */
L_004798:
        CALL(sub_00B386, 0x479CU);                           /* 004798  bsr  $b386 */
L_00479C:
        /* 00479C  tst.w  $26e(a5) */
        /* 0047A0  beq  $47a8 */
        if (rd16(V_026E) != 0) {
            /* 0047A2  move.w  #$444,$ff8240.l */
            wr16(HW_PALETTE, 0x444);
        } else {
L_0047A8:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 0047A8  or.w  d0,d1 */
        }
        CALL(sub_004954, 0x47AEU);                           /* 0047AA  bsr  $4954 */
L_0047AE:
        /* 0047AE  tst.w  $26e(a5) */
        /* 0047B2  beq  $47ba */
        if (rd16(V_026E) != 0) {
            /* 0047B4  move.w  #$50,$ff8240.l */
            wr16(HW_PALETTE, 0x50);
        } else {
L_0047BA:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 0047BA  or.w  d0,d1 */
        }
        CALL(horizontal_background, 0x47C0U);                /* 0047BC  bsr  $488c */
L_0047C0:
        /* 0047C0  tst.w  $26e(a5) */
        /* 0047C4  beq  $47cc */
        if (rd16(V_026E) != 0) {
            /* 0047C6  move.w  #$770,$ff8240.l */
            wr16(HW_PALETTE, 0x770);
        } else {
L_0047CC:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 0047CC  or.w  d0,d1 */
        }
        CALL(sub_004F32, 0x47D2U);                           /* 0047CE  bsr  $4f32 */
L_0047D2:
        CALL(sub_0054AA, 0x47D6U);                           /* 0047D2  bsr  $54aa */
L_0047D6:
        /* 0047D6  tst.w  $26e(a5) */
        /* 0047DA  beq  $47e2 */
        if (rd16(V_026E) != 0) {
            /* 0047DC  move.w  #$0,$ff8240.l */
            wr16(HW_PALETTE, 0);
        } else {
L_0047E2:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 0047E2  or.w  d0,d1 */
        }
        CALL(sub_005B32, 0x47E8U);                           /* 0047E4  bsr  $5b32 */
L_0047E8:
        /* 0047E8  tst.w  $26e(a5) */
        /* 0047EC  beq  $47f4 */
        if (rd16(V_026E) != 0) {
            /* 0047EE  move.w  #$741,$ff8240.l */
            wr16(HW_PALETTE, 0x741);
        } else {
L_0047F4:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 0047F4  or.w  d0,d1 */
        }
        CALL(sub_005946, 0x47FAU);                           /* 0047F6  bsr  $5946 */
L_0047FA:
        /* 0047FA  tst.w  $26e(a5) */
        /* 0047FE  beq  $4806 */
        HOOK(2, 0x47FAU);
        if (rd16(V_026E) != 0) {
            /* 004800  move.w  #$400,$ff8240.l */
            wr16(HW_PALETTE, 0x400);
        } else {
L_004806:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 004806  or.w  d0,d1 */
        }
        CALL(draw_objects_a, 0x480CU);                       /* 004808  bsr  $5e32 */
L_00480C:
        CALL(sub_00660C, 0x4810U);                           /* 00480C  bsr  $660c */
L_004810:
        CALL(draw_explosions, 0x4814U);                      /* 004810  bsr  $6d12 */
L_004814:
        CALL(sub_0063DE, 0x4818U);                           /* 004814  bsr  $63de */
L_004818:
        CALL(draw_objects_b, 0x481CU);                       /* 004818  bsr  $5f1a */
L_00481C:
        /* 00481C  tst.w  $26e(a5) */
        /* 004820  beq  $4828 */
        if (rd16(V_026E) != 0) {
            /* 004822  move.w  #$60,$ff8240.l */
            wr16(HW_PALETTE, 0x60);
        } else {
L_004828:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 004828  or.w  d0,d1 */
        }
        /* 00482A  tst.w  $26e(a5) */
        /* 00482E  beq  $4836 */
        if (rd16(V_026E) != 0) {
            /* 004830  move.w  #$600,$ff8240.l */
            wr16(HW_PALETTE, 0x600);
        } else {
L_004836:
            SETW(d1, d1 | (d0 & 0xFFFF));                        /* 004836  or.w  d0,d1 */
        }
        CALL(draw_bullets, 0x483CU);                         /* 004838  bsr  $6812 */
L_00483C:
        CALL(draw_effects, 0x4840U);                         /* 00483C  bsr  $b6ea */
L_004840:
        CALL(sub_008E82, 0x4844U);                           /* 004840  bsr  $8e82 */
L_004844:
        CALL(sub_008E96, 0x4848U);                           /* 004844  bsr  $8e96 */
L_004848:
        /* 004848  addq.w  #1,$272(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0272);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(V_0272, r);
            fx = (r < s);
        }
        /* 00484C  cmpi.w  #$180,$280(a5) */
        /* 004852  bne  $487e */
        if (rd16(V_SCROLL) == 0x180) {
            wr16(V_0248, 0);                                     /* 004854  clr.w  $248(a5) */
            a4 = V_1C3C;                                         /* 004858  lea  $1c3c(a5),a4 */
            d7 = 5;                                              /* 00485C  moveq  #5,d7 */
            do {
                BUDGET_CHECK(0x485EU);
L_00485E:
                wr16(a4, 0);                                         /* 00485E  clr.w  (a4) */
                a4 = a4 + 0xC;                                       /* 004860  lea  $c(a4),a4 */
                /* 004864  dbra  d7,$485e */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
            d7 = 0xF;                                            /* 004868  moveq  #15,d7 */
            do {
                BUDGET_CHECK(0x486AU);
L_00486A:
                wr16(a4, 0);                                         /* 00486A  clr.w  (a4) */
                a4 = a4 + 0xE;                                       /* 00486C  lea  $e(a4),a4 */
                /* 004870  dbra  d7,$486a */
                SETW(d7, d7 - 1);
            } while ((d7 & 0xFFFF) != 0xFFFF);
            CALL(sub_009062, 0x4878U);                           /* 004874  bsr  $9062 */
L_004878:
            CALL(level_map_prefill, 0x487CU);                    /* 004878  bsr  $8d1e */
L_00487C:
            RTS();                                               /* 00487C  rts */
        }
L_00487E:
        SETW(d0, rd16(V_02DA));                              /* 00487E  move.w  $2da(a5),d0 */
        /* 004882  add.w  $376(a5),d0 */
        {
            uint32_t s = rd16(V_0376);
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
            fz = (r == 0);
            fx = (r < s);
        }
        /* 004886  bne  $474a */
    } while (COND(6));
    RTS();                                               /* 00488A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x474EU: goto L_00474E;
        case 0x4752U: goto L_004752;
        case 0x4756U: goto L_004756;
        case 0x475AU: goto L_00475A;
        case 0x475EU: goto L_00475E;
        case 0x4762U: goto L_004762;
        case 0x4766U: goto L_004766;
        case 0x476AU: goto L_00476A;
        case 0x476EU: goto L_00476E;
        case 0x4772U: goto L_004772;
        case 0x4776U: goto L_004776;
        case 0x477AU: goto L_00477A;
        case 0x477EU: goto L_00477E;
        case 0x4782U: goto L_004782;
        case 0x478EU: goto L_00478E;
        case 0x4794U: goto L_004794;
        case 0x4798U: goto L_004798;
        case 0x479CU: goto L_00479C;
        case 0x47A8U: goto L_0047A8;
        case 0x47AEU: goto L_0047AE;
        case 0x47BAU: goto L_0047BA;
        case 0x47C0U: goto L_0047C0;
        case 0x47CCU: goto L_0047CC;
        case 0x47D2U: goto L_0047D2;
        case 0x47D6U: goto L_0047D6;
        case 0x47E2U: goto L_0047E2;
        case 0x47E8U: goto L_0047E8;
        case 0x47F4U: goto L_0047F4;
        case 0x47FAU: goto L_0047FA;
        case 0x4806U: goto L_004806;
        case 0x480CU: goto L_00480C;
        case 0x4810U: goto L_004810;
        case 0x4814U: goto L_004814;
        case 0x4818U: goto L_004818;
        case 0x481CU: goto L_00481C;
        case 0x4828U: goto L_004828;
        case 0x4836U: goto L_004836;
        case 0x483CU: goto L_00483C;
        case 0x4840U: goto L_004840;
        case 0x4844U: goto L_004844;
        case 0x4848U: goto L_004848;
        case 0x485EU: goto L_00485E;
        case 0x486AU: goto L_00486A;
        case 0x4878U: goto L_004878;
        case 0x487CU: goto L_00487C;
        case 0x487EU: goto L_00487E;
        }
    }
}

/* 00488C  Background of the horizontal stretches (repeating 32x32 texture) */
void horizontal_background(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 00488C  move.w  $24a(a5),d0 */
    HOOK(3, 0x488CU);
    SETW(d0, rd16(V_HSCROLL_PHASE));
    SETW(d0, d0 & 0x1E);                                 /* 004890  andi.w  #$1e,d0 */
    SETW(d0, (d0 & 0xFFFF) << 4);                        /* 004894  lsl.w  #4,d0 */
    SETW(d0, (d0 & 0xFFFF) << 5);                        /* 004896  lsl.w  #5,d0 */
    a0 = 0x28000;                                        /* 004898  lea  $28000.l,a0 */
    a0 = a0 + SX16(d0);                                  /* 00489E  lea  $0(a0,d0.w),a0 */
    a1 = rd32(V_DRAW_BUFFER);                            /* 0048A2  movea.l  $28e(a5),a1 */
    a1 = a1 + 0x20;                                      /* 0048A6  lea  $20(a1),a1 */
    wr16(V_0276, 0x20);                                  /* 0048AA  move.w  #$20,$276(a5) */
    /* 0048B0  move.l  a1,-(a7) */
    {
        uint32_t v = a1;
        a7 -= 4;
        wr32(a7, v);
    }
    do {
        BUDGET_CHECK(0x48B2U);
L_0048B2:
        /* 0048B2  movem.l  (a0)+,d0-d7 */
        { uint32_t ea;
        ea = a0;
        d0 = rd32(ea);
        ea += 4;
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
        a0 = ea;
        }
        /* 0048B6  movem.l  d0-d7,(a1) */
        { uint32_t ea;
        ea = a1;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048BA  movem.l  d0-d7,$1400(a1) */
        { uint32_t ea;
        ea = a1 + 0x1400U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048C0  movem.l  d0-d7,$2800(a1) */
        { uint32_t ea;
        ea = a1 + 0x2800U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048C6  movem.l  d0-d7,$3c00(a1) */
        { uint32_t ea;
        ea = a1 + 0x3C00U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048CC  movem.l  d0-d7,$5000(a1) */
        { uint32_t ea;
        ea = a1 + 0x5000U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048D2  movem.l  d0-d7,$6400(a1) */
        { uint32_t ea;
        ea = a1 + 0x6400U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048D8  movem.l  d0-d7,$20(a1) */
        { uint32_t ea;
        ea = a1 + 0x20U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048DE  movem.l  d0-d7,$1420(a1) */
        { uint32_t ea;
        ea = a1 + 0x1420U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048E4  movem.l  d0-d7,$2820(a1) */
        { uint32_t ea;
        ea = a1 + 0x2820U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048EA  movem.l  d0-d7,$3c20(a1) */
        { uint32_t ea;
        ea = a1 + 0x3C20U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048F0  movem.l  d0-d7,$5020(a1) */
        { uint32_t ea;
        ea = a1 + 0x5020U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048F6  movem.l  d0-d7,$6420(a1) */
        { uint32_t ea;
        ea = a1 + 0x6420U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 0048FC  movem.l  d0-d7,$40(a1) */
        { uint32_t ea;
        ea = a1 + 0x40U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 004902  movem.l  d0-d7,$1440(a1) */
        { uint32_t ea;
        ea = a1 + 0x1440U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 004908  movem.l  d0-d7,$2840(a1) */
        { uint32_t ea;
        ea = a1 + 0x2840U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 00490E  movem.l  d0-d7,$3c40(a1) */
        { uint32_t ea;
        ea = a1 + 0x3C40U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 004914  movem.l  d0-d7,$5040(a1) */
        { uint32_t ea;
        ea = a1 + 0x5040U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        /* 00491A  movem.l  d0-d7,$6440(a1) */
        { uint32_t ea;
        ea = a1 + 0x6440U;
        wr32(ea, d0); ea += 4;
        wr32(ea, d1); ea += 4;
        wr32(ea, d2); ea += 4;
        wr32(ea, d3); ea += 4;
        wr32(ea, d4); ea += 4;
        wr32(ea, d5); ea += 4;
        wr32(ea, d6); ea += 4;
        wr32(ea, d7); ea += 4;
        }
        a1 = a1 + 0xA0;                                      /* 004920  lea  $a0(a1),a1 */
        /* 004924  subq.w  #1,$276(a5) */
        {
            uint32_t s = 1;
            uint32_t d = rd16(V_0276);
            uint32_t r = (d - s) & 0xFFFF;
            wr16(V_0276, r);
            fz = (r == 0);
        }
        /* 004928  bne  $48b2 */
    } while (COND(6));
L_00492A:
    /* 00492A  movea.l  (a7)+,a1 */
    a1 = rd32(a7);
    a7 += 4;
    wr16(V_0276, 8);                                     /* 00492C  move.w  #$8,$276(a5) */
L_004932:
    /* 004932  movem.l  (a1),d0-d7 */
    { uint32_t ea;
    ea = a1;
    d0 = rd32(ea);
    ea += 4;
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
    }
    /* 004936  movem.l  d0-d7,$7800(a1) */
    { uint32_t ea;
    ea = a1 + 0x7800U;
    wr32(ea, d0); ea += 4;
    wr32(ea, d1); ea += 4;
    wr32(ea, d2); ea += 4;
    wr32(ea, d3); ea += 4;
    wr32(ea, d4); ea += 4;
    wr32(ea, d5); ea += 4;
    wr32(ea, d6); ea += 4;
    wr32(ea, d7); ea += 4;
    }
    /* 00493C  movem.l  d0-d7,$7820(a1) */
    { uint32_t ea;
    ea = a1 + 0x7820U;
    wr32(ea, d0); ea += 4;
    wr32(ea, d1); ea += 4;
    wr32(ea, d2); ea += 4;
    wr32(ea, d3); ea += 4;
    wr32(ea, d4); ea += 4;
    wr32(ea, d5); ea += 4;
    wr32(ea, d6); ea += 4;
    wr32(ea, d7); ea += 4;
    }
    /* 004942  movem.l  d0-d7,$7840(a1) */
    { uint32_t ea;
    ea = a1 + 0x7840U;
    wr32(ea, d0); ea += 4;
    wr32(ea, d1); ea += 4;
    wr32(ea, d2); ea += 4;
    wr32(ea, d3); ea += 4;
    wr32(ea, d4); ea += 4;
    wr32(ea, d5); ea += 4;
    wr32(ea, d6); ea += 4;
    wr32(ea, d7); ea += 4;
    }
    a1 = a1 + 0xA0;                                      /* 004948  lea  $a0(a1),a1 */
    /* 00494C  subq.w  #1,$276(a5) */
    {
        uint32_t s = 1;
        uint32_t d = rd16(V_0276);
        uint32_t r = (d - s) & 0xFFFF;
        wr16(V_0276, r);
        fz = (r == 0);
    }
    /* 004950  bne  $4932 */
    if (COND(6)) { BUDGET_CHECK(0x4932U); goto L_004932; }
    { sub_004952(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x48B2U: goto L_0048B2;
        case 0x492AU: goto L_00492A;
        case 0x4932U: goto L_004932;
        }
    }
}

/* 004952  game routine */
void sub_004952(void)
{
    RTS();                                               /* 004952  rts */
}

/* 004954  game routine */
void sub_004954(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    SETW(d0, 0x90);                                      /* 004954  move.w  #$90,d0 */
    /* 004958  tst.w  $2da(a5) */
    /* 00495C  beq  $4962 */
    if (rd16(V_02DA) != 0) {
L_00495E:
        SETW(d0, rd16(V_02DC));                              /* 00495E  move.w  $2dc(a5),d0 */
    }
    SETW(d1, 0x90);                                      /* 004962  move.w  #$90,d1 */
    /* 004966  tst.w  $376(a5) */
    /* 00496A  beq  $4970 */
    if (rd16(V_0376) != 0) {
        SETW(d1, rd16(V_0378));                              /* 00496C  move.w  $378(a5),d1 */
    }
L_004970:
    /* 004970  tst.w  $248(a5) */
    /* 004974  bmi  $498c */
    if ((int16_t)rd16(V_0248) >= 0) {
L_004976:
        /* 004976  cmp.w  #$46,d0 */
        /* 00497A  bcs  $4952 */
        if ((d0 & 0xFFFF) < 0x46) {
            { sub_004952(); return; }
        }
L_00497C:
        /* 00497C  cmp.w  #$46,d1 */
        /* 004980  bcs  $4952 */
        if ((d1 & 0xFFFF) < 0x46) {
            { sub_004952(); return; }
        }
L_004982:
        SETW(d0, d0 + (d1 & 0xFFFF));                        /* 004982  add.w  d1,d0 */
        /* 004984  cmp.w  #$102,d0 */
        /* 004988  bcs  $4952 */
        if ((d0 & 0xFFFF) < 0x102) {
            { sub_004952(); return; }
        }
L_00498A:
        /* 00498A  bra  $49a0 */
    } else {
L_00498C:
        /* 00498C  cmp.w  #$da,d0 */
        /* 004990  bhi  $4952 */
        if ((d0 & 0xFFFF) > 0xDA) {
            { sub_004952(); return; }
        }
L_004992:
        /* 004992  cmp.w  #$da,d1 */
        /* 004996  bhi  $4952 */
        if ((d1 & 0xFFFF) > 0xDA) {
            { sub_004952(); return; }
        }
L_004998:
        SETW(d0, d0 + (d1 & 0xFFFF));                        /* 004998  add.w  d1,d0 */
        /* 00499A  cmp.w  #$13e,d0 */
        /* 00499E  bhi  $4952 */
        if ((d0 & 0xFFFF) > 0x13E) {
            { sub_004952(); return; }
        }
    }
L_0049A0:
    wr16(V_SCROLL, rd16(V_SCROLL) + 1);                  /* 0049A0  addq.w  #1,$280(a5) */
    wr8(V_1DF4, rd8(V_1DF4) + 1);                        /* 0049A4  addq.b  #1,$1df4(a5) */
    SETW(d5, rd16(V_0248));                              /* 0049A8  move.w  $248(a5),d5 */
    wr16(V_HSCROLL_PHASE, rd16(V_HSCROLL_PHASE) + (d5 & 0xFFFF)); /* 0049AC  add.w  d5,$24a(a5) */
    /* 0049B0  neg.w  d5 */
    { uint32_t d, r, ea; (void)ea;
    d = (d5 & 0xFFFFU);
    r = (0 - d) & 0xFFFFU;
    d5 = (d5 & 0xFFFF0000U) | ((r) & 0xFFFFU);
    FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
    wr16(V_02DC, rd16(V_02DC) + (d5 & 0xFFFF));          /* 0049B2  add.w  d5,$2dc(a5) */
    wr16(V_0378, rd16(V_0378) + (d5 & 0xFFFF));          /* 0049B6  add.w  d5,$378(a5) */
    a6 = V_ENEMIES;                                      /* 0049BA  lea  $412(a5),a6 */
    SETW(d6, 0xBC0);                                     /* 0049BE  move.w  #$bc0,d6 */
    do {
        BUDGET_CHECK(0x49C2U);
L_0049C2:
        wr16(a6 + SX16(d6), rd16(a6 + SX16(d6)) + (d5 & 0xFFFF)); /* 0049C2  add.w  d5,$0(a6,d6.w) */
        /* 0049C6  subi.w  #$40,d6 */
        {
            uint32_t s = 0x40;
            uint32_t d = (d6 & 0xFFFF);
            uint32_t r = (d - s) & 0xFFFF;
            SETW(d6, r);
            fn = !!(r & 0x8000);
        }
        /* 0049CA  bpl  $49c2 */
    } while (COND(10));
L_0049CC:
    a6 = V_1206;                                         /* 0049CC  lea  $1206(a5),a6 */
    SETW(d6, 0x10);                                      /* 0049D0  move.w  #$10,d6 */
    do {
        BUDGET_CHECK(0x49D4U);
L_0049D4:
        wr16(a6, rd16(a6) + (d5 & 0xFFFF));                  /* 0049D4  add.w  d5,$0(a6) */
        a6 = a6 + 0x14;                                      /* 0049D8  lea  $14(a6),a6 */
        /* 0049DC  dbra  d6,$49d4 */
        SETW(d6, d6 - 1);
    } while ((d6 & 0xFFFF) != 0xFFFF);
L_0049E0:
    a0 = V_BULLETS;                                      /* 0049E0  lea  $1012(a5),a0 */
    SETW(d0, 0x1D);                                      /* 0049E4  move.w  #$1d,d0 */
    do {
        BUDGET_CHECK(0x49E8U);
L_0049E8:
        wr16(a0, rd16(a0) + (d5 & 0xFFFF));                  /* 0049E8  add.w  d5,(a0) */
        a0 = a0 + 0xE;                                       /* 0049EA  lea  $e(a0),a0 */
        /* 0049EE  dbra  d0,$49e8 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_0049F2:
    a0 = V_1614;                                         /* 0049F2  lea  $1614(a5),a0 */
    SETW(d0, 0x13);                                      /* 0049F6  move.w  #$13,d0 */
    do {
        BUDGET_CHECK(0x49FAU);
L_0049FA:
        wr16(a0 + 0x2, rd16(a0 + 0x2) + (d5 & 0xFFFF));      /* 0049FA  add.w  d5,$2(a0) */
        a0 = a0 + 0x10;                                      /* 0049FE  lea  $10(a0),a0 */
        /* 004A02  dbra  d0,$49fa */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_004A06:
    a0 = V_1754;                                         /* 004A06  lea  $1754(a5),a0 */
    SETW(d0, 0x18);                                      /* 004A0A  move.w  #$18,d0 */
    do {
        BUDGET_CHECK(0x4A0EU);
L_004A0E:
        wr16(a0, rd16(a0) + (d5 & 0xFFFF));                  /* 004A0E  add.w  d5,(a0) */
        a0 = a0 + 0x10;                                      /* 004A10  lea  $10(a0),a0 */
        /* 004A14  dbra  d0,$4a0e */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_004A18:
    a0 = V_1C3C;                                         /* 004A18  lea  $1c3c(a5),a0 */
    d0 = 5;                                              /* 004A1C  moveq  #5,d0 */
    do {
        BUDGET_CHECK(0x4A1EU);
L_004A1E:
        wr16(a0 + 0x2, rd16(a0 + 0x2) + (d5 & 0xFFFF));      /* 004A1E  add.w  d5,$2(a0) */
        a0 = a0 + 0xC;                                       /* 004A22  lea  $c(a0),a0 */
        /* 004A26  dbra  d0,$4a1e */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_004A2A:
    a0 = V_1B5C;                                         /* 004A2A  lea  $1b5c(a5),a0 */
    d0 = 0xF;                                            /* 004A2E  moveq  #15,d0 */
    do {
        BUDGET_CHECK(0x4A30U);
L_004A30:
        wr16(a0 + 0x2, rd16(a0 + 0x2) + (d5 & 0xFFFF));      /* 004A30  add.w  d5,$2(a0) */
        a0 = a0 + 0xE;                                       /* 004A34  lea  $e(a0),a0 */
        /* 004A38  dbra  d0,$4a30 */
        SETW(d0, d0 - 1);
    } while ((d0 & 0xFFFF) != 0xFFFF);
L_004A3C:
    SETW(d7, 0x13);                                      /* 004A3C  move.w  #$13,d7 */
    a6 = V_135A;                                         /* 004A40  lea  $135a(a5),a6 */
    do {
        BUDGET_CHECK(0x4A44U);
L_004A44:
        /* 004A44  add.w  d5,$0(a6) */
        {
            uint32_t s = (d5 & 0xFFFF);
            uint32_t d = rd16(a6);
            uint32_t r = (d + s) & 0xFFFF;
            wr16(a6, r);
            fx = (r < s);
        }
        a6 = a6 + 0xE;                                       /* 004A48  lea  $e(a6),a6 */
        /* 004A4C  dbra  d7,$4a44 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_004A50:
    /* 004A50  rts */
    HOOK(4, 0x4A50U);
    RTS();

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x495EU: goto L_00495E;
        case 0x4970U: goto L_004970;
        case 0x4976U: goto L_004976;
        case 0x497CU: goto L_00497C;
        case 0x4982U: goto L_004982;
        case 0x498AU: goto L_00498A;
        case 0x498CU: goto L_00498C;
        case 0x4992U: goto L_004992;
        case 0x4998U: goto L_004998;
        case 0x49A0U: goto L_0049A0;
        case 0x49C2U: goto L_0049C2;
        case 0x49CCU: goto L_0049CC;
        case 0x49D4U: goto L_0049D4;
        case 0x49E0U: goto L_0049E0;
        case 0x49E8U: goto L_0049E8;
        case 0x49F2U: goto L_0049F2;
        case 0x49FAU: goto L_0049FA;
        case 0x4A06U: goto L_004A06;
        case 0x4A0EU: goto L_004A0E;
        case 0x4A18U: goto L_004A18;
        case 0x4A1EU: goto L_004A1E;
        case 0x4A2AU: goto L_004A2A;
        case 0x4A30U: goto L_004A30;
        case 0x4A3CU: goto L_004A3C;
        case 0x4A44U: goto L_004A44;
        case 0x4A50U: goto L_004A50;
        }
    }
}

/* 004A52  game routine */
void sub_004A52(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a6 = V_1614;                                         /* 004A52  lea  $1614(a5),a6 */
    SETW(d7, 0x13);                                      /* 004A56  move.w  #$13,d7 */
    do {
        BUDGET_CHECK(0x4A5AU);
L_004A5A:
        /* 004A5A  tst.w  (a6) */
        /* 004A5C  beq  $4b82 */
        if (rd16(a6) != 0) {
            /* 004A60  cmpi.w  #$10,$6(a6) */
            /* 004A66  bcs  $4b82 */
            if (rd16(a6 + 0x6) >= 0x10) {
                /* 004A6A  cmpi.w  #$17,$6(a6) */
                /* 004A70  bhi  $4b82 */
                if (rd16(a6 + 0x6) <= 0x17) {
                    a4 = V_1C3C;                                         /* 004A74  lea  $1c3c(a5),a4 */
                    SETW(d6, 5);                                         /* 004A78  move.w  #$5,d6 */
                    do {
                        BUDGET_CHECK(0x4A7CU);
L_004A7C:
                        /* 004A7C  tst.w  (a4) */
                        /* 004A7E  beq  $4b7a */
                        if (rd16(a4) != 0) {
                            /* 004A82  cmpi.w  #$ef,$6(a4) */
                            /* 004A88  beq  $4b7a */
                            if (rd16(a4 + 0x6) != 0xEF) {
                                SETW(d0, rd16(a6 + 0x2));                            /* 004A8C  move.w  $2(a6),d0 */
                                SETW(d1, rd16(a6 + 0x4));                            /* 004A90  move.w  $4(a6),d1 */
                                SETW(d2, rd16(a4 + 0x2));                            /* 004A94  move.w  $2(a4),d2 */
                                SETW(d3, rd16(a4 + 0x4));                            /* 004A98  move.w  $4(a4),d3 */
                                SETW(d3, d3 + 0x60);                                 /* 004A9C  addi.w  #$60,d3 */
                                SETW(d2, d2 + 0x20);                                 /* 004AA0  addi.w  #$20,d2 */
                                /* 004AA4  sub.w  d3,d1 */
                                {
                                    uint32_t s = (d3 & 0xFFFF);
                                    uint32_t d = (d1 & 0xFFFF);
                                    uint32_t r = (d - s) & 0xFFFF;
                                    SETW(d1, r);
                                    fx = (s > d);
                                }
                                /* 004AA6  cmp.w  #$1b,d1 */
                                /* 004AAA  bhi  $4b7a */
                                if ((d1 & 0xFFFF) <= 0x1B) {
                                    /* 004AAE  cmp.w  #$0,d1 */
                                    /* 004AB2  bcs  $4b7a */
                                    if ((d1 & 0xFFFF) >= 0) {
                                        /* 004AB6  sub.w  d2,d0 */
                                        {
                                            uint32_t s = (d2 & 0xFFFF);
                                            uint32_t d = (d0 & 0xFFFF);
                                            uint32_t r = (d - s) & 0xFFFF;
                                            SETW(d0, r);
                                            fx = (s > d);
                                        }
                                        /* 004AB8  cmp.w  #$20,d0 */
                                        /* 004ABC  bhi  $4b7a */
                                        if ((d0 & 0xFFFF) <= 0x20) {
                                            /* 004AC0  cmp.w  #$0,d0 */
                                            /* 004AC4  bcs  $4b7a */
                                            if ((d0 & 0xFFFF) >= 0) {
                                                wr32(V_02E2, rd32(V_02E2) + 0x3E8);                  /* 004AC8  addi.l  #$3e8,$2e2(a5) */
                                                /* 004AD0  tst.w  $e(a6) */
                                                /* 004AD4  beq  $4ae6 */
                                                if (rd16(a6 + 0xE) != 0) {
                                                    wr32(V_02E2, rd32(V_02E2) - 0x3E8);                  /* 004AD6  subi.l  #$3e8,$2e2(a5) */
                                                    wr32(V_037E, rd32(V_037E) + 0x3E8);                  /* 004ADE  addi.l  #$3e8,$37e(a5) */
                                                }
                                                wr16(a4 + 0x6, 0xEF);                                /* 004AE6  move.w  #$ef,$6(a4) */
                                                wr16(a6, 0);                                         /* 004AEC  clr.w  (a6) */
                                                SETW(d0, rd16(a4 + 0x2));                            /* 004AEE  move.w  $2(a4),d0 */
                                                SETW(d1, rd16(a4 + 0x4));                            /* 004AF2  move.w  $4(a4),d1 */
                                                fz = !(rd8(V_1DC7) & 0x10);                          /* 004AF6  btst  #$4,$1dc7(a5) */
                                                /* 004AFC  bne  $4b3c */
                                                if (COND(7)) {
                                                    SETW(d1, d1 + 0x56);                                 /* 004AFE  addi.w  #$56,d1 */
                                                    /* 004B02  addi.w  #$23,d0 */
                                                    {
                                                        uint32_t s = 0x23;
                                                        uint32_t d = (d0 & 0xFFFF);
                                                        uint32_t r = (d + s) & 0xFFFF;
                                                        SETW(d0, r);
                                                        fx = (r < s);
                                                    }
                                                    d2 = 0;                                              /* 004B06  moveq  #0,d2 */
                                                    CALL(sub_0063A4, 0x4B0CU);                           /* 004B08  bsr  $63a4 */
L_004B0C:
                                                    SETW(d0, d0 + 0x10);                                 /* 004B0C  addi.w  #$10,d0 */
                                                    /* 004B10  addi.w  #$10,d1 */
                                                    {
                                                        uint32_t s = 0x10;
                                                        uint32_t d = (d1 & 0xFFFF);
                                                        uint32_t r = (d + s) & 0xFFFF;
                                                        SETW(d1, r);
                                                        fx = (r < s);
                                                    }
                                                    d2 = 0;                                              /* 004B14  moveq  #0,d2 */
                                                    CALL(sub_0063A4, 0x4B1AU);                           /* 004B16  bsr  $63a4 */
L_004B1A:
                                                    SETW(d0, d0 + 0x10);                                 /* 004B1A  addi.w  #$10,d0 */
                                                    /* 004B1E  subi.w  #$5,d1 */
                                                    {
                                                        uint32_t s = 5;
                                                        uint32_t d = (d1 & 0xFFFF);
                                                        uint32_t r = (d - s) & 0xFFFF;
                                                        SETW(d1, r);
                                                        fx = (s > d);
                                                    }
                                                    SETW(d2, 0xFFFF);                                    /* 004B22  move.w  #$ffff,d2 */
                                                    CALL(sub_006CA8, 0x4B2AU);                           /* 004B26  bsr  $6ca8 */
L_004B2A:
                                                    SETW(d1, d1 + 0x10);                                 /* 004B2A  addi.w  #$10,d1 */
                                                    /* 004B2E  subi.w  #$20,d0 */
                                                    {
                                                        uint32_t s = 0x20;
                                                        uint32_t d = (d0 & 0xFFFF);
                                                        uint32_t r = (d - s) & 0xFFFF;
                                                        SETW(d0, r);
                                                        fx = (s > d);
                                                    }
                                                    SETW(d2, 0xFFFF);                                    /* 004B32  move.w  #$ffff,d2 */
                                                    CALL(sub_006CA8, 0x4B3AU);                           /* 004B36  bsr  $6ca8 */
L_004B3A:
                                                    /* 004B3A  bra  $4b82 */
                                                    goto L_004B82;
                                                }
                                                SETW(d1, d1 + 0x5E);                                 /* 004B3C  addi.w  #$5e,d1 */
                                                /* 004B40  addi.w  #$20,d0 */
                                                {
                                                    uint32_t s = 0x20;
                                                    uint32_t d = (d0 & 0xFFFF);
                                                    uint32_t r = (d + s) & 0xFFFF;
                                                    SETW(d0, r);
                                                    fx = (r < s);
                                                }
                                                SETW(d2, 0xFFFF);                                    /* 004B44  move.w  #$ffff,d2 */
                                                CALL(sub_006CA8, 0x4B4CU);                           /* 004B48  bsr  $6ca8 */
L_004B4C:
                                                SETW(d0, d0 + 0x20);                                 /* 004B4C  addi.w  #$20,d0 */
                                                /* 004B50  addi.w  #$10,d1 */
                                                {
                                                    uint32_t s = 0x10;
                                                    uint32_t d = (d1 & 0xFFFF);
                                                    uint32_t r = (d + s) & 0xFFFF;
                                                    SETW(d1, r);
                                                    fx = (r < s);
                                                }
                                                SETW(d2, 0xFFFF);                                    /* 004B54  move.w  #$ffff,d2 */
                                                CALL(sub_006CA8, 0x4B5CU);                           /* 004B58  bsr  $6ca8 */
L_004B5C:
                                                SETW(d0, d0 - 0x10);                                 /* 004B5C  subi.w  #$10,d0 */
                                                /* 004B60  subi.w  #$19,d1 */
                                                {
                                                    uint32_t s = 0x19;
                                                    uint32_t d = (d1 & 0xFFFF);
                                                    uint32_t r = (d - s) & 0xFFFF;
                                                    SETW(d1, r);
                                                    fx = (s > d);
                                                }
                                                d2 = 0;                                              /* 004B64  moveq  #0,d2 */
                                                CALL(sub_0063A4, 0x4B6AU);                           /* 004B66  bsr  $63a4 */
L_004B6A:
                                                SETW(d1, d1 + 0x10);                                 /* 004B6A  addi.w  #$10,d1 */
                                                /* 004B6E  subi.w  #$10,d0 */
                                                {
                                                    uint32_t s = 0x10;
                                                    uint32_t d = (d0 & 0xFFFF);
                                                    uint32_t r = (d - s) & 0xFFFF;
                                                    SETW(d0, r);
                                                    fx = (s > d);
                                                }
                                                d2 = 0;                                              /* 004B72  moveq  #0,d2 */
                                                CALL(sub_0063A4, 0x4B78U);                           /* 004B74  bsr  $63a4 */
L_004B78:
                                                /* 004B78  bra  $4b82 */
                                                goto L_004B82;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        a4 = a4 + 0xC;                                       /* 004B7A  lea  $c(a4),a4 */
                        /* 004B7E  dbra  d6,$4a7c */
                        SETW(d6, d6 - 1);
                    } while ((d6 & 0xFFFF) != 0xFFFF);
                }
            }
        }
L_004B82:
        a6 = a6 + 0x10;                                      /* 004B82  lea  $10(a6),a6 */
        /* 004B86  dbra  d7,$4a5a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_004B8A:
    RTS();                                               /* 004B8A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4A5AU: goto L_004A5A;
        case 0x4A7CU: goto L_004A7C;
        case 0x4B0CU: goto L_004B0C;
        case 0x4B1AU: goto L_004B1A;
        case 0x4B2AU: goto L_004B2A;
        case 0x4B3AU: goto L_004B3A;
        case 0x4B4CU: goto L_004B4C;
        case 0x4B5CU: goto L_004B5C;
        case 0x4B6AU: goto L_004B6A;
        case 0x4B78U: goto L_004B78;
        case 0x4B82U: goto L_004B82;
        case 0x4B8AU: goto L_004B8A;
        }
    }
}

/* 004BBE  game routine */
void sub_004BBE(void)
{
    a0 = 0x1986F;                                        /* 004BBE  lea  $1986f.l,a0 */
    RTS();                                               /* 004BC4  rts */
}

/* 004C06  game routine */
void sub_004C06(void)
{
    RTS();                                               /* 004C06  rts */
}

/* 004C2E  game routine */
void sub_004C2E(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 004C2E  movem.l  a6/a5/a4/a3/a2/a1/a0/d7/d6/d5/d4/d3/d2/d1/d0,-(a7) */
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
    a4 = V_1C3C;                                         /* 004C32  lea  $1c3c(a5),a4 */
    SETW(d7, 5);                                         /* 004C36  move.w  #$5,d7 */
    do {
        BUDGET_CHECK(0x4C3AU);
L_004C3A:
        /* 004C3A  tst.w  (a4) */
        /* 004C3C  bne  $4c60 */
        if (rd16(a4) == 0) {
            wr16(a4 + 0x6, (d0 & 0xFFFF));                       /* 004C40  move.w  d0,$6(a4) */
            wr8(a4 + 0x2, 0);                                    /* 004C44  clr.b  $2(a4) */
            wr8(a4 + 0x4, 0);                                    /* 004C48  clr.b  $4(a4) */
            /* 004C4C  move.b  (a0)+,$3(a4) */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                wr8(a4 + 0x3, v);
            }
            /* 004C50  move.b  (a0)+,$5(a4) */
            {
                uint32_t v = rd8(a0);
                a0 += 1;
                wr8(a4 + 0x5, v);
            }
            wr8(a4, 0xFF);                                       /* 004C54  st  (a4) */
            wr16(a4 + 0x8, 0);                                   /* 004C56  clr.w  $8(a4) */
            wr16(a4 + 0xA, 0);                                   /* 004C5A  clr.w  $a(a4) */
            /* 004C5E  bra  $4c6a */
            goto L_004C6A;
        }
        a4 = a4 + 0xC;                                       /* 004C60  lea  $c(a4),a4 */
        /* 004C64  dbra  d7,$4c3a */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
    a0 += 2;                                             /* 004C68  addq.l  #2,a0 */
L_004C6A:
    wr32(V_1DF6, a0);                                    /* 004C6A  move.l  a0,$1df6(a5) */
    /* 004C6E  movem.l  (a7)+,d0-d7/a0-a6 */
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
    RTS();                                               /* 004C72  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4C3AU: goto L_004C3A;
        }
    }
}

/* 004C74  game routine */
void sub_004C74(void)
{
    a4 = V_1C3C;                                         /* 004C74  lea  $1c3c(a5),a4 */
    /* 004C78  moveq  #5,d7 */
    d7 = 5;
    { sub_004C7A(); return; }
}

/* 004C7A  game routine */
void sub_004C7A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    /* 004C7A  tst.w  (a4) */
    /* 004C7C  beq  $4e9a */
    if (rd16(a4) == 0) {
        { sub_004E9A(); return; }
    }
    /* 004C80  cmpi.w  #$16,$2(a4) */
    /* 004C86  bcs  $4c90 */
    if (rd16(a4 + 0x2) >= 0x16) {
        /* 004C88  cmpi.w  #$10a,$2(a4) */
        /* 004C8E  bcs  $4c96 */
        if (rd16(a4 + 0x2) < 0x10A) {
            goto L_004C96;
        }
    }
    wr16(a4, 0);                                         /* 004C90  clr.w  (a4) */
    /* 004C92  bra  $4e9a */
    { sub_004E9A(); return; }
L_004C96:
    /* 004C96  cmpi.w  #$ea,$6(a4) */
    /* 004C9C  bne  $4cfc */
    if (rd16(a4 + 0x6) == 0xEA) {
        /* 004C9E  tst.w  $8(a4) */
        /* 004CA2  beq  $4cac */
        if (rd16(a4 + 0x8) != 0) {
            wr16(a4 + 0x8, rd16(a4 + 0x8) - 1);                  /* 004CA4  subq.w  #1,$8(a4) */
            /* 004CA8  bra  $4e9a */
            { sub_004E9A(); return; }
        }
L_004CAC:
        SETW(d0, rd16(a4 + 0xA));                            /* 004CAC  move.w  $a(a4),d0 */
        d0 = (d0 & 0xFFFF) * 0xA;                            /* 004CB0  mulu.w  #$a,d0 */
        a0 = 0x5704;                                         /* 004CB4  lea  $5704(pc),a0 */
        a0 = a0 + SX16(d0);                                  /* 004CB8  lea  $0(a0,d0.w),a0 */
        /* 004CBC  cmpi.w  #$ffff,(a0) */
        /* 004CC0  bne  $4cc8 */
        if (rd16(a0) == 0xFFFF) {
            wr16(a4 + 0xA, 0);                                   /* 004CC2  clr.w  $a(a4) */
            /* 004CC6  bra  $4cac */
            BUDGET_CHECK(0x4CACU);
            goto L_004CAC;
        }
        SETW(d0, rd16(a4 + 0x2));                            /* 004CC8  move.w  $2(a4),d0 */
        /* 004CCC  add.w  (a0)+,d0 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
        }
        SETW(d1, rd16(a4 + 0x4));                            /* 004CCE  move.w  $4(a4),d1 */
        /* 004CD2  add.w  (a0)+,d1 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
        }
        SETW(d0, d0 + 0x24);                                 /* 004CD4  addi.w  #$24,d0 */
        SETW(d1, d1 + 0x64);                                 /* 004CD8  addi.w  #$64,d1 */
        /* 004CDC  move.w  (a0)+,d2 */
        SETW(d2, rd16(a0));
        a0 += 2;
        /* 004CDE  move.w  (a0)+,d3 */
        SETW(d3, rd16(a0));
        a0 += 2;
        /* 004CE0  move.w  (a0)+,$8(a4) */
        {
            uint32_t v = rd16(a0);
            a0 += 2;
            wr16(a4 + 0x8, v);
        }
        SETW(d4, 0xF);                                       /* 004CE4  move.w  #$f,d4 */
        CALL(sub_005904, 0x4CECU);                           /* 004CE8  bsr  $5904 */
L_004CEC:
        wr16(a4 + 0xA, rd16(a4 + 0xA) + 1);                  /* 004CEC  addi.w  #$1,$a(a4) */
        /* 004CF2  tst.w  $8(a4) */
        /* 004CF6  beq  $4cac */
        if (rd16(a4 + 0x8) == 0) {
            BUDGET_CHECK(0x4CACU);
            goto L_004CAC;
        }
        /* 004CF8  bra  $4e9a */
        { sub_004E9A(); return; }
    }
    /* 004CFC  cmpi.w  #$eb,$6(a4) */
    /* 004D02  bne  $4d60 */
    if (rd16(a4 + 0x6) == 0xEB) {
        /* 004D04  tst.w  $8(a4) */
        /* 004D08  beq  $4d12 */
        if (rd16(a4 + 0x8) != 0) {
            wr16(a4 + 0x8, rd16(a4 + 0x8) - 1);                  /* 004D0A  subq.w  #1,$8(a4) */
            /* 004D0E  bra  $4e9a */
            { sub_004E9A(); return; }
        }
        for (;;) {
            BUDGET_CHECK(0x4D12U);
L_004D12:
            SETW(d0, rd16(a4 + 0xA));                            /* 004D12  move.w  $a(a4),d0 */
            d0 = (d0 & 0xFFFF) * 0xA;                            /* 004D16  mulu.w  #$a,d0 */
            a0 = 0x576A;                                         /* 004D1A  lea  $576a(pc),a0 */
            a0 = a0 + SX16(d0);                                  /* 004D1E  lea  $0(a0,d0.w),a0 */
            /* 004D22  cmpi.w  #$ffff,(a0) */
            /* 004D26  bne  $4d2e */
            if (rd16(a0) != 0xFFFF) {
                goto L_004D2E;
            }
            wr16(a4 + 0xA, 0);                                   /* 004D28  clr.w  $a(a4) */
            /* 004D2C  bra  $4d12 */
        }
L_004D2E:
        SETW(d0, rd16(a4 + 0x2));                            /* 004D2E  move.w  $2(a4),d0 */
        /* 004D32  add.w  (a0)+,d0 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
        }
        SETW(d1, rd16(a4 + 0x4));                            /* 004D34  move.w  $4(a4),d1 */
        /* 004D38  add.w  (a0)+,d1 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
        }
        SETW(d0, d0 + 0x24);                                 /* 004D3A  addi.w  #$24,d0 */
        /* 004D3E  addi.w  #$64,d1 */
        {
            uint32_t s = 0x64;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
            fx = (r < s);
        }
        /* 004D42  move.w  (a0)+,d3 */
        SETW(d3, rd16(a0));
        a0 += 2;
        /* 004D44  move.w  (a0)+,d4 */
        SETW(d4, rd16(a0));
        a0 += 2;
        /* 004D46  move.w  (a0)+,$8(a4) */
        {
            uint32_t v = rd16(a0);
            a0 += 2;
            wr16(a4 + 0x8, v);
        }
        SETW(d5, 0xFFFF);                                    /* 004D4A  move.w  #$ffff,d5 */
        SETW(d2, 0x12C);                                     /* 004D4E  move.w  #$12c,d2 */
        CALL(sub_006B74, 0x4D56U);                           /* 004D52  bsr  $6b74 */
L_004D56:
        wr16(a4 + 0xA, rd16(a4 + 0xA) + 1);                  /* 004D56  addi.w  #$1,$a(a4) */
        /* 004D5C  bra  $4e9a */
        { sub_004E9A(); return; }
    }
    /* 004D60  cmpi.w  #$ec,$6(a4) */
    /* 004D66  bne  $4dca */
    if (rd16(a4 + 0x6) == 0xEC) {
        /* 004D68  tst.w  $8(a4) */
        /* 004D6C  beq  $4d76 */
        if (rd16(a4 + 0x8) != 0) {
            wr16(a4 + 0x8, rd16(a4 + 0x8) - 1);                  /* 004D6E  subq.w  #1,$8(a4) */
            /* 004D72  bra  $4e9a */
            { sub_004E9A(); return; }
        }
L_004D76:
        SETW(d0, rd16(a4 + 0xA));                            /* 004D76  move.w  $a(a4),d0 */
        d0 = (d0 & 0xFFFF) * 0xA;                            /* 004D7A  mulu.w  #$a,d0 */
        a0 = 0x57BC;                                         /* 004D7E  lea  $57bc(pc),a0 */
        a0 = a0 + SX16(d0);                                  /* 004D82  lea  $0(a0,d0.w),a0 */
        /* 004D86  cmpi.w  #$ffff,(a0) */
        /* 004D8A  bne  $4d92 */
        if (rd16(a0) == 0xFFFF) {
            wr16(a4 + 0xA, 0);                                   /* 004D8C  clr.w  $a(a4) */
            /* 004D90  bra  $4d76 */
            BUDGET_CHECK(0x4D76U);
            goto L_004D76;
        }
        SETW(d0, rd16(a4 + 0x2));                            /* 004D92  move.w  $2(a4),d0 */
        /* 004D96  add.w  (a0)+,d0 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d0, r);
        }
        SETW(d1, rd16(a4 + 0x4));                            /* 004D98  move.w  $4(a4),d1 */
        /* 004D9C  add.w  (a0)+,d1 */
        {
            uint32_t s = rd16(a0);
            a0 += 2;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
        }
        SETW(d0, d0 + 0x24);                                 /* 004D9E  addi.w  #$24,d0 */
        /* 004DA2  addi.w  #$64,d1 */
        {
            uint32_t s = 0x64;
            uint32_t d = (d1 & 0xFFFF);
            uint32_t r = (d + s) & 0xFFFF;
            SETW(d1, r);
            fx = (r < s);
        }
        /* 004DA6  move.w  (a0)+,d3 */
        SETW(d3, rd16(a0));
        a0 += 2;
        /* 004DA8  move.w  (a0)+,d4 */
        SETW(d4, rd16(a0));
        a0 += 2;
        /* 004DAA  move.w  (a0)+,$8(a4) */
        {
            uint32_t v = rd16(a0);
            a0 += 2;
            wr16(a4 + 0x8, v);
        }
        SETW(d5, 0xFFFF);                                    /* 004DAE  move.w  #$ffff,d5 */
        SETW(d2, 0x12C);                                     /* 004DB2  move.w  #$12c,d2 */
        CALL(sub_006B74, 0x4DBAU);                           /* 004DB6  bsr  $6b74 */
L_004DBA:
        wr16(a4 + 0xA, rd16(a4 + 0xA) + 1);                  /* 004DBA  addi.w  #$1,$a(a4) */
        /* 004DC0  tst.w  $8(a4) */
        /* 004DC4  beq  $4d76 */
        if (rd16(a4 + 0x8) == 0) {
            BUDGET_CHECK(0x4D76U);
            goto L_004D76;
        }
        /* 004DC6  bra  $4e9a */
        { sub_004E9A(); return; }
    }
    /* 004DCA  cmpi.w  #$ed,$6(a4) */
    /* 004DD0  bne  $4e30 */
    if (rd16(a4 + 0x6) == 0xED) {
        /* 004DD2  tst.w  $8(a4) */
        /* 004DD6  beq  $4de0 */
        if (rd16(a4 + 0x8) == 0) {
            { sub_004DE0(); return; }
        }
        wr16(a4 + 0x8, rd16(a4 + 0x8) - 1);                  /* 004DD8  subq.w  #1,$8(a4) */
        /* 004DDC  bra  $4e9a */
        { sub_004E9A(); return; }
    }
    /* 004E30  cmpi.w  #$ee,$6(a4) */
    /* 004E36  bne  $4e9a */
    if (rd16(a4 + 0x6) != 0xEE) {
        { sub_004E9A(); return; }
    }
    /* 004E38  tst.w  $8(a4) */
    /* 004E3C  beq  $4e46 */
    if (rd16(a4 + 0x8) != 0) {
        wr16(a4 + 0x8, rd16(a4 + 0x8) - 1);                  /* 004E3E  subq.w  #1,$8(a4) */
        /* 004E42  bra  $4e9a */
        { sub_004E9A(); return; }
    }
L_004E46:
    SETW(d0, rd16(a4 + 0xA));                            /* 004E46  move.w  $a(a4),d0 */
    d0 = (d0 & 0xFFFF) * 0xA;                            /* 004E4A  mulu.w  #$a,d0 */
    a0 = 0x58B2;                                         /* 004E4E  lea  $58b2(pc),a0 */
    a0 = a0 + SX16(d0);                                  /* 004E52  lea  $0(a0,d0.w),a0 */
    /* 004E56  cmpi.w  #$ffff,(a0) */
    /* 004E5A  bne  $4e62 */
    if (rd16(a0) == 0xFFFF) {
        wr16(a4 + 0xA, 0);                                   /* 004E5C  clr.w  $a(a4) */
        /* 004E60  bra  $4e46 */
        BUDGET_CHECK(0x4E46U);
        goto L_004E46;
    }
    SETW(d0, rd16(a4 + 0x2));                            /* 004E62  move.w  $2(a4),d0 */
    /* 004E66  add.w  (a0)+,d0 */
    {
        uint32_t s = rd16(a0);
        a0 += 2;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
    }
    SETW(d1, rd16(a4 + 0x4));                            /* 004E68  move.w  $4(a4),d1 */
    /* 004E6C  add.w  (a0)+,d1 */
    {
        uint32_t s = rd16(a0);
        a0 += 2;
        uint32_t d = (d1 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d1, r);
    }
    SETW(d0, d0 + 0x24);                                 /* 004E6E  addi.w  #$24,d0 */
    /* 004E72  addi.w  #$64,d1 */
    {
        uint32_t s = 0x64;
        uint32_t d = (d1 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d1, r);
        fx = (r < s);
    }
    SETW(d2, 0x1E);                                      /* 004E76  move.w  #$1e,d2 */
    /* 004E7A  move.w  (a0)+,d3 */
    SETW(d3, rd16(a0));
    a0 += 2;
    /* 004E7C  move.w  (a0)+,d4 */
    SETW(d4, rd16(a0));
    a0 += 2;
    /* 004E7E  move.w  (a0)+,$8(a4) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(a4 + 0x8, v);
    }
    SETW(d5, 0xFFFF);                                    /* 004E82  move.w  #$ffff,d5 */
    CALL(sub_00B6A6, 0x4E8AU);                           /* 004E86  bsr  $b6a6 */
L_004E8A:
    wr16(a4 + 0xA, rd16(a4 + 0xA) + 1);                  /* 004E8A  addi.w  #$1,$a(a4) */
    /* 004E90  tst.w  $8(a4) */
    /* 004E94  beq  $4e46 */
    if (rd16(a4 + 0x8) == 0) {
        BUDGET_CHECK(0x4E46U);
        goto L_004E46;
    }
    /* 004E96  bra  $4e9a */
    { sub_004E9A(); return; }

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4CACU: goto L_004CAC;
        case 0x4CECU: goto L_004CEC;
        case 0x4D12U: goto L_004D12;
        case 0x4D56U: goto L_004D56;
        case 0x4D76U: goto L_004D76;
        case 0x4DBAU: goto L_004DBA;
        case 0x4E46U: goto L_004E46;
        case 0x4E8AU: goto L_004E8A;
        }
    }
}

/* 004DE0  game routine */
void sub_004DE0(void)
{
    for (;;) {
        BUDGET_CHECK(0x4DE0U);
        SETW(d0, rd16(a4 + 0xA));                            /* 004DE0  move.w  $a(a4),d0 */
        d0 = (d0 & 0xFFFF) * 0xA;                            /* 004DE4  mulu.w  #$a,d0 */
        a0 = 0x5860;                                         /* 004DE8  lea  $5860(pc),a0 */
        a0 = a0 + SX16(d0);                                  /* 004DEC  lea  $0(a0,d0.w),a0 */
        /* 004DF0  cmpi.w  #$ffff,(a0) */
        /* 004DF4  bne  $4dfc */
        if (rd16(a0) != 0xFFFF) {
            goto L_004DFC;
        }
        wr16(a4 + 0xA, 0);                                   /* 004DF6  clr.w  $a(a4) */
        /* 004DFA  bra  $4de0 */
    }
L_004DFC:
    SETW(d0, rd16(a4 + 0x2));                            /* 004DFC  move.w  $2(a4),d0 */
    /* 004E00  add.w  (a0)+,d0 */
    {
        uint32_t s = rd16(a0);
        a0 += 2;
        uint32_t d = (d0 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d0, r);
    }
    SETW(d1, rd16(a4 + 0x4));                            /* 004E02  move.w  $4(a4),d1 */
    /* 004E06  add.w  (a0)+,d1 */
    {
        uint32_t s = rd16(a0);
        a0 += 2;
        uint32_t d = (d1 & 0xFFFF);
        uint32_t r = (d + s) & 0xFFFF;
        SETW(d1, r);
    }
    SETW(d0, d0 + 0x24);                                 /* 004E08  addi.w  #$24,d0 */
    SETW(d1, d1 + 0x64);                                 /* 004E0C  addi.w  #$64,d1 */
    /* 004E10  move.w  (a0)+,d2 */
    SETW(d2, rd16(a0));
    a0 += 2;
    /* 004E12  move.w  (a0)+,d3 */
    SETW(d3, rd16(a0));
    a0 += 2;
    /* 004E14  move.w  (a0)+,$8(a4) */
    {
        uint32_t v = rd16(a0);
        a0 += 2;
        wr16(a4 + 0x8, v);
    }
    SETW(d4, 0xF);                                       /* 004E18  move.w  #$f,d4 */
    /* 004E1C  bsr  $5904 */
    CALL(sub_005904, 0x4E20U);
    { sub_004E20(); return; }
}

/* 004E20  game routine */
void sub_004E20(void)
{
    wr16(a4 + 0xA, rd16(a4 + 0xA) + 1);                  /* 004E20  addi.w  #$1,$a(a4) */
    /* 004E26  tst.w  $8(a4) */
    /* 004E2A  beq  $4de0 */
    if (rd16(a4 + 0x8) == 0) {
        { sub_004DE0(); return; }
    }
    /* 004E2C  bra  $4e9a */
    { sub_004E9A(); return; }
}

/* 004E9A  game routine */
void sub_004E9A(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = a4 + 0xC;                                       /* 004E9A  lea  $c(a4),a4 */
    /* 004E9E  dbra  d7,$4c7a */
    SETW(d7, d7 - 1);
    if ((d7 & 0xFFFF) != 0xFFFF) { sub_004C7A(); return; }
L_004EA2:
    RTS();                                               /* 004EA2  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4EA2U: goto L_004EA2;
        }
    }
}

/* 004F32  game routine */
void sub_004F32(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a4 = V_1C3C;                                         /* 004F32  lea  $1c3c(a5),a4 */
    d7 = 5;                                              /* 004F36  moveq  #5,d7 */
    do {
        BUDGET_CHECK(0x4F38U);
L_004F38:
        /* 004F38  tst.w  (a4) */
        /* 004F3A  beq  $4f92 */
        if (rd16(a4) != 0) {
            a1 = 0x45736;                                        /* 004F3C  lea  $45736.l,a1 */
            SETW(d0, rd16(a4 + 0x6));                            /* 004F42  move.w  $6(a4),d0 */
            SETW(d0, d0 - 0xEA);                                 /* 004F46  subi.w  #$ea,d0 */
            d0 = (d0 & 0xFFFF) * 0x200;                          /* 004F4A  mulu.w  #$200,d0 */
            a1 = a1 + SX16(d0);                                  /* 004F4E  lea  $0(a1,d0.w),a1 */
            a0 = rd32(V_DRAW_BUFFER);                            /* 004F52  movea.l  $28e(a5),a0 */
            a0 = a0 + 0x20;                                      /* 004F56  lea  $20(a0),a0 */
            SETW(d0, rd16(a4 + 0x4));                            /* 004F5A  move.w  $4(a4),d0 */
            SETW(d0, (d0 & 0xFFFF) << 5);                        /* 004F5E  lsl.w  #5,d0 */
            a0 = a0 + SX16(d0);                                  /* 004F60  lea  $0(a0,d0.w),a0 */
            SETW(d0, (d0 & 0xFFFF) << 2);                        /* 004F64  lsl.w  #2,d0 */
            a0 = a0 + SX16(d0);                                  /* 004F66  lea  $0(a0,d0.w),a0 */
            SETW(d0, rd16(a4 + 0x2));                            /* 004F6A  move.w  $2(a4),d0 */
            SETW(d0, d0 - 0x40);                                 /* 004F6E  subi.w  #$40,d0 */
            SETW(d0, shift_op(0, 0, (d0 & 0xFFFF), 1, 16));      /* 004F72  asr.w  #1,d0 */
            SETW(d0, d0 & 0xFFF8);                               /* 004F74  andi.w  #$fff8,d0 */
            a0 = a0 + SX16(d0);                                  /* 004F78  lea  $0(a0,d0.w),a0 */
            wr16(V_0212, 0x20);                                  /* 004F7C  move.w  #$20,$212(a5) */
            SETW(d0, rd16(a4 + 0x2));                            /* 004F82  move.w  $2(a4),d0 */
            /* 004F86  movem.l  a4/d7,-(a7) */
            PUSH32(a4);
            PUSH32(d7);
            CALL(sub_004F9C, 0x4F8EU);                           /* 004F8A  bsr  $4f9c */
L_004F8E:
            /* 004F8E  movem.l  (a7)+,d7/a4 */
            d7 = POP32();
            a4 = POP32();
        }
L_004F92:
        a4 = a4 + 0xC;                                       /* 004F92  lea  $c(a4),a4 */
        /* 004F96  dbra  d7,$4f38 */
        SETW(d7, d7 - 1);
    } while ((d7 & 0xFFFF) != 0xFFFF);
L_004F9A:
    RTS();                                               /* 004F9A  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x4F38U: goto L_004F38;
        case 0x4F8EU: goto L_004F8E;
        case 0x4F92U: goto L_004F92;
        case 0x4F9AU: goto L_004F9A;
        }
    }
}

/* 004F9C  game routine */
void sub_004F9C(void)
{
    if (resume_pc) goto resume_dispatch;                 /* loaded state / dynamic jump: see the end of the routine */
    a0 = a0 + 0x16;                                      /* 004F9C  lea  $16(a0),a0 */
    wr8(V_020C, 0xFF);                                   /* 004FA0  st  $20c(a5) */
    wr8(V_020E, 0xFF);                                   /* 004FA4  st  $20e(a5) */
    wr8(V_0210, 0xFF);                                   /* 004FA8  st  $210(a5) */
    /* 004FAC  cmp.w  #$e0,d0 */
    /* 004FB0  bls  $4fce */
    if ((d0 & 0xFFFF) > 0xE0) {
        wr16(V_0210, 0);                                     /* 004FB2  clr.w  $210(a5) */
        /* 004FB6  cmp.w  #$f0,d0 */
        /* 004FBA  bcs  $5144 */
        if ((d0 & 0xFFFF) < 0xF0) {
            goto L_005144;
        }
        wr16(V_020E, 0);                                     /* 004FBE  clr.w  $20e(a5) */
        /* 004FC2  cmp.w  #$100,d0 */
        /* 004FC6  bcs  $5144 */
        if ((d0 & 0xFFFF) < 0x100) {
            goto L_005144;
        }
        /* 004FCA  bra  $52f8 */
    } else {
        /* 004FCE  cmp.w  #$40,d0 */
        /* 004FD2  bcc  $4ff0 */
        if ((d0 & 0xFFFF) < 0x40) {
            wr16(V_020C, 0);                                     /* 004FD4  clr.w  $20c(a5) */
            /* 004FD8  cmp.w  #$30,d0 */
            /* 004FDC  bcc  $5144 */
            if ((d0 & 0xFFFF) >= 0x30) {
                goto L_005144;
            }
            wr16(V_020E, 0);                                     /* 004FE0  clr.w  $20e(a5) */
            /* 004FE4  cmp.w  #$20,d0 */
            /* 004FE8  bcc  $5144 */
            if ((d0 & 0xFFFF) >= 0x20) {
                goto L_005144;
            }
            /* 004FEC  bra  $52f8 */
            goto L_0052F8;
        }
        /* 004FF0  andi.w  #$f,d0 */
        {
            uint32_t s = 0xF;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d0, r);
            fz = (r == 0);
        }
        /* 004FF4  beq  $50fa */
        if (COND(6)) {
            /* 004FF8  cmp.w  #$8,d0 */
            /* 004FFC  bls  $5080 */
            if ((d0 & 0xFFFF) > 8) {
                /* 005000  neg.w  d0 */
                { uint32_t d, r, ea; (void)ea;
                d = (d0 & 0xFFFFU);
                r = (0 - d) & 0xFFFFU;
                d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
                FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
                SETW(d0, d0 + 0x10);                                 /* 005002  addi.w  #$10,d0 */
                SETW(d6, (d0 & 0xFFFF));                             /* 005006  move.w  d0,d6 */
                d7 = 0x10;                                           /* 005008  moveq  #16,d7 */
                SETW(d7, d7 - (d6 & 0xFFFF));                        /* 00500A  sub.w  d6,d7 */
                do {
                    BUDGET_CHECK(0x500CU);
L_00500C:
                    /* 00500C  movem.l  (a1),d0-d3 */
                    { uint32_t ea;
                    ea = a1;
                    d0 = rd32(ea);
                    ea += 4;
                    d1 = rd32(ea);
                    ea += 4;
                    d2 = rd32(ea);
                    ea += 4;
                    d3 = rd32(ea);
                    ea += 4;
                    }
                    SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), (d6 & 63), 16)); /* 005010  lsl.w  d6,d0 */
                    SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), (d6 & 63), 16)); /* 005012  lsl.w  d6,d1 */
                    SETW(d2, shift_op(1, 1, (d2 & 0xFFFF), (d6 & 63), 16)); /* 005014  lsl.w  d6,d2 */
                    SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), (d6 & 63), 16)); /* 005016  lsl.w  d6,d3 */
                    SETW(d5, (d0 & 0xFFFF));                             /* 005018  move.w  d0,d5 */
                    SETW(d5, d5 | (d1 & 0xFFFF));                        /* 00501A  or.w  d1,d5 */
                    SETW(d5, d5 | (d2 & 0xFFFF));                        /* 00501C  or.w  d2,d5 */
                    SETW(d5, d5 | (d3 & 0xFFFF));                        /* 00501E  or.w  d3,d5 */
                    SETW(d5, ~(d5 & 0xFFFF));                            /* 005020  not.w  d5 */
                    wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 005022  and.w  d5,(a0) */
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005024  or.w  d3,(a0) */
                    /* 005026  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005028  or.w  d2,(a0) */
                    /* 00502A  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00502C  or.w  d1,(a0) */
                    /* 00502E  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005030  or.w  d0,(a0) */
                    /* 005032  movem.l  (a1)+,d0-d3 */
                    { uint32_t ea;
                    ea = a1;
                    d0 = rd32(ea);
                    ea += 4;
                    d1 = rd32(ea);
                    ea += 4;
                    d2 = rd32(ea);
                    ea += 4;
                    d3 = rd32(ea);
                    ea += 4;
                    a1 = ea;
                    }
                    d0 = shift_op(1, 0, d0, (d7 & 63), 32);              /* 005036  lsr.l  d7,d0 */
                    d1 = shift_op(1, 0, d1, (d7 & 63), 32);              /* 005038  lsr.l  d7,d1 */
                    d2 = shift_op(1, 0, d2, (d7 & 63), 32);              /* 00503A  lsr.l  d7,d2 */
                    d3 = shift_op(1, 0, d3, (d7 & 63), 32);              /* 00503C  lsr.l  d7,d3 */
                    d5 = d0;                                             /* 00503E  move.l  d0,d5 */
                    d5 |= d1;                                            /* 005040  or.l  d1,d5 */
                    d5 |= d2;                                            /* 005042  or.l  d2,d5 */
                    d5 |= d3;                                            /* 005044  or.l  d3,d5 */
                    d5 = ~d5;                                            /* 005046  not.l  d5 */
                    /* 005048  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 00504A  or.w  d3,(a0) */
                    /* 00504C  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 00504E  or.w  d2,(a0) */
                    /* 005050  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005052  or.w  d1,(a0) */
                    /* 005054  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005056  or.w  d0,(a0) */
                    d0 = (d0 << 16) | (d0 >> 16);                        /* 005058  swap  d0 */
                    d1 = (d1 << 16) | (d1 >> 16);                        /* 00505A  swap  d1 */
                    d2 = (d2 << 16) | (d2 >> 16);                        /* 00505C  swap  d2 */
                    d3 = (d3 << 16) | (d3 >> 16);                        /* 00505E  swap  d3 */
                    d5 = (d5 << 16) | (d5 >> 16);                        /* 005060  swap  d5 */
                    /* 005062  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005064  or.w  d3,(a0) */
                    /* 005066  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005068  or.w  d2,(a0) */
                    /* 00506A  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00506C  or.w  d1,(a0) */
                    /* 00506E  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005070  or.w  d0,(a0) */
                    a0 = a0 + 0xB6;                                      /* 005072  lea  $b6(a0),a0 */
                    /* 005076  subq.w  #1,$212(a5) */
                    {
                        uint32_t s = 1;
                        uint32_t d = rd16(V_0212);
                        uint32_t r = (d - s) & 0xFFFF;
                        wr16(V_0212, r);
                        fz = (r == 0);
                        fx = (s > d);
                    }
                    /* 00507A  bne  $500c */
                } while (COND(6));
                /* 00507C  bra  $52f8 */
                goto L_0052F8;
            }
            SETW(d6, (d0 & 0xFFFF));                             /* 005080  move.w  d0,d6 */
            d7 = 0x10;                                           /* 005082  moveq  #16,d7 */
            SETW(d7, d7 - (d6 & 0xFFFF));                        /* 005084  sub.w  d6,d7 */
            do {
                BUDGET_CHECK(0x5086U);
L_005086:
                /* 005086  movem.l  (a1),d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                }
                d0 = shift_op(1, 1, d0, (d7 & 63), 32);              /* 00508A  lsl.l  d7,d0 */
                d1 = shift_op(1, 1, d1, (d7 & 63), 32);              /* 00508C  lsl.l  d7,d1 */
                d2 = shift_op(1, 1, d2, (d7 & 63), 32);              /* 00508E  lsl.l  d7,d2 */
                d3 = shift_op(1, 1, d3, (d7 & 63), 32);              /* 005090  lsl.l  d7,d3 */
                SETW(d5, (d0 & 0xFFFF));                             /* 005092  move.w  d0,d5 */
                SETW(d5, d5 | (d1 & 0xFFFF));                        /* 005094  or.w  d1,d5 */
                SETW(d5, d5 | (d2 & 0xFFFF));                        /* 005096  or.w  d2,d5 */
                SETW(d5, d5 | (d3 & 0xFFFF));                        /* 005098  or.w  d3,d5 */
                SETW(d5, ~(d5 & 0xFFFF));                            /* 00509A  not.w  d5 */
                wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 00509C  and.w  d5,(a0) */
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 00509E  or.w  d3,(a0) */
                /* 0050A0  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0050A2  or.w  d2,(a0) */
                /* 0050A4  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0050A6  or.w  d1,(a0) */
                /* 0050A8  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0050AA  or.w  d0,(a0) */
                /* 0050AC  movem.l  (a1)+,d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                a1 = ea;
                }
                d0 = shift_op(1, 0, d0, (d6 & 63), 32);              /* 0050B0  lsr.l  d6,d0 */
                d1 = shift_op(1, 0, d1, (d6 & 63), 32);              /* 0050B2  lsr.l  d6,d1 */
                d2 = shift_op(1, 0, d2, (d6 & 63), 32);              /* 0050B4  lsr.l  d6,d2 */
                d3 = shift_op(1, 0, d3, (d6 & 63), 32);              /* 0050B6  lsr.l  d6,d3 */
                d5 = d0;                                             /* 0050B8  move.l  d0,d5 */
                d5 |= d1;                                            /* 0050BA  or.l  d1,d5 */
                d5 |= d2;                                            /* 0050BC  or.l  d2,d5 */
                d5 |= d3;                                            /* 0050BE  or.l  d3,d5 */
                d5 = ~d5;                                            /* 0050C0  not.l  d5 */
                /* 0050C2  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0050C4  or.w  d3,(a0) */
                /* 0050C6  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0050C8  or.w  d2,(a0) */
                /* 0050CA  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0050CC  or.w  d1,(a0) */
                /* 0050CE  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0050D0  or.w  d0,(a0) */
                d0 = (d0 << 16) | (d0 >> 16);                        /* 0050D2  swap  d0 */
                d1 = (d1 << 16) | (d1 >> 16);                        /* 0050D4  swap  d1 */
                d2 = (d2 << 16) | (d2 >> 16);                        /* 0050D6  swap  d2 */
                d3 = (d3 << 16) | (d3 >> 16);                        /* 0050D8  swap  d3 */
                d5 = (d5 << 16) | (d5 >> 16);                        /* 0050DA  swap  d5 */
                /* 0050DC  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0050DE  or.w  d3,(a0) */
                /* 0050E0  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0050E2  or.w  d2,(a0) */
                /* 0050E4  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0050E6  or.w  d1,(a0) */
                /* 0050E8  and.w  d5,-(a0) */
                {
                    uint32_t s = (d5 & 0xFFFF);
                    a0 -= 2;
                    uint32_t d = rd16(a0);
                    uint32_t r = (d & s) & 0xFFFF;
                    wr16(a0, r);
                }
                wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0050EA  or.w  d0,(a0) */
                a0 = a0 + 0xB6;                                      /* 0050EC  lea  $b6(a0),a0 */
                /* 0050F0  subq.w  #1,$212(a5) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(V_0212);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(V_0212, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 0050F4  bne  $5086 */
            } while (COND(6));
            /* 0050F6  bra  $52f8 */
            goto L_0052F8;
        }
        a0 = a0 - 0x8;                                       /* 0050FA  lea  -$8(a0),a0 */
        do {
            BUDGET_CHECK(0x50FEU);
L_0050FE:
            /* 0050FE  movem.l  (a1)+,d0-d3 */
            { uint32_t ea;
            ea = a1;
            d0 = rd32(ea);
            ea += 4;
            d1 = rd32(ea);
            ea += 4;
            d2 = rd32(ea);
            ea += 4;
            d3 = rd32(ea);
            ea += 4;
            a1 = ea;
            }
            d6 = d0;                                             /* 005102  move.l  d0,d6 */
            d6 |= d1;                                            /* 005104  or.l  d1,d6 */
            d6 |= d2;                                            /* 005106  or.l  d2,d6 */
            d6 |= d3;                                            /* 005108  or.l  d3,d6 */
            d6 = ~d6;                                            /* 00510A  not.l  d6 */
            wr16(a0, rd16(a0) & (d6 & 0xFFFF));                  /* 00510C  and.w  d6,(a0) */
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 00510E  or.w  d3,(a0) */
            /* 005110  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005112  or.w  d2,(a0) */
            /* 005114  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005116  or.w  d1,(a0) */
            /* 005118  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 00511A  or.w  d0,(a0) */
            d0 = (d0 << 16) | (d0 >> 16);                        /* 00511C  swap  d0 */
            d1 = (d1 << 16) | (d1 >> 16);                        /* 00511E  swap  d1 */
            d2 = (d2 << 16) | (d2 >> 16);                        /* 005120  swap  d2 */
            d3 = (d3 << 16) | (d3 >> 16);                        /* 005122  swap  d3 */
            d6 = (d6 << 16) | (d6 >> 16);                        /* 005124  swap  d6 */
            /* 005126  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005128  or.w  d3,(a0) */
            /* 00512A  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 00512C  or.w  d2,(a0) */
            /* 00512E  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005130  or.w  d1,(a0) */
            /* 005132  and.w  d6,-(a0) */
            {
                uint32_t s = (d6 & 0xFFFF);
                a0 -= 2;
                uint32_t d = rd16(a0);
                uint32_t r = (d & s) & 0xFFFF;
                wr16(a0, r);
            }
            wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005134  or.w  d0,(a0) */
            a0 = a0 + 0xAE;                                      /* 005136  lea  $ae(a0),a0 */
            /* 00513A  subq.w  #1,$212(a5) */
            {
                uint32_t s = 1;
                uint32_t d = rd16(V_0212);
                uint32_t r = (d - s) & 0xFFFF;
                wr16(V_0212, r);
                fz = (r == 0);
                fx = (s > d);
            }
            /* 00513E  bne  $50fe */
        } while (COND(6));
        /* 005140  bra  $52f8 */
        goto L_0052F8;
L_005144:
        /* 005144  andi.w  #$f,d0 */
        {
            uint32_t s = 0xF;
            uint32_t d = (d0 & 0xFFFF);
            uint32_t r = (d & s) & 0xFFFF;
            SETW(d0, r);
            fz = (r == 0);
        }
        /* 005148  beq  $529a */
        if (COND(6)) {
            /* 00514C  cmp.w  #$8,d0 */
            /* 005150  bls  $51fa */
            if ((d0 & 0xFFFF) > 8) {
                /* 005154  neg.w  d0 */
                { uint32_t d, r, ea; (void)ea;
                d = (d0 & 0xFFFFU);
                r = (0 - d) & 0xFFFFU;
                d0 = (d0 & 0xFFFF0000U) | ((r) & 0xFFFFU);
                FL_SUB(d, 0u, r, 0xFFFFU, 0x8000U, 1); }
                SETW(d0, d0 + 0x10);                                 /* 005156  addi.w  #$10,d0 */
                SETW(d6, (d0 & 0xFFFF));                             /* 00515A  move.w  d0,d6 */
                d7 = 0x10;                                           /* 00515C  moveq  #16,d7 */
                SETW(d7, d7 - (d6 & 0xFFFF));                        /* 00515E  sub.w  d6,d7 */
                do {
                    BUDGET_CHECK(0x5160U);
L_005160:
                    /* 005160  tst.w  $210(a5) */
                    /* 005164  bne  $516c */
                    if (rd16(V_0210) == 0) {
                        a0 = a0 - 0x6;                                       /* 005166  lea  -$6(a0),a0 */
                        /* 00516A  bra  $5192 */
                    } else {
                        /* 00516C  movem.l  (a1),d0-d3 */
                        { uint32_t ea;
                        ea = a1;
                        d0 = rd32(ea);
                        ea += 4;
                        d1 = rd32(ea);
                        ea += 4;
                        d2 = rd32(ea);
                        ea += 4;
                        d3 = rd32(ea);
                        ea += 4;
                        }
                        SETW(d0, shift_op(1, 1, (d0 & 0xFFFF), (d6 & 63), 16)); /* 005170  lsl.w  d6,d0 */
                        SETW(d1, shift_op(1, 1, (d1 & 0xFFFF), (d6 & 63), 16)); /* 005172  lsl.w  d6,d1 */
                        SETW(d2, shift_op(1, 1, (d2 & 0xFFFF), (d6 & 63), 16)); /* 005174  lsl.w  d6,d2 */
                        SETW(d3, shift_op(1, 1, (d3 & 0xFFFF), (d6 & 63), 16)); /* 005176  lsl.w  d6,d3 */
                        SETW(d5, (d0 & 0xFFFF));                             /* 005178  move.w  d0,d5 */
                        SETW(d5, d5 | (d1 & 0xFFFF));                        /* 00517A  or.w  d1,d5 */
                        SETW(d5, d5 | (d2 & 0xFFFF));                        /* 00517C  or.w  d2,d5 */
                        SETW(d5, d5 | (d3 & 0xFFFF));                        /* 00517E  or.w  d3,d5 */
                        SETW(d5, ~(d5 & 0xFFFF));                            /* 005180  not.w  d5 */
                        wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 005182  and.w  d5,(a0) */
                        wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005184  or.w  d3,(a0) */
                        /* 005186  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005188  or.w  d2,(a0) */
                        /* 00518A  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00518C  or.w  d1,(a0) */
                        /* 00518E  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005190  or.w  d0,(a0) */
                    }
                    /* 005192  movem.l  (a1)+,d0-d3 */
                    { uint32_t ea;
                    ea = a1;
                    d0 = rd32(ea);
                    ea += 4;
                    d1 = rd32(ea);
                    ea += 4;
                    d2 = rd32(ea);
                    ea += 4;
                    d3 = rd32(ea);
                    ea += 4;
                    a1 = ea;
                    }
                    d0 = shift_op(1, 0, d0, (d7 & 63), 32);              /* 005196  lsr.l  d7,d0 */
                    d1 = shift_op(1, 0, d1, (d7 & 63), 32);              /* 005198  lsr.l  d7,d1 */
                    d2 = shift_op(1, 0, d2, (d7 & 63), 32);              /* 00519A  lsr.l  d7,d2 */
                    d3 = shift_op(1, 0, d3, (d7 & 63), 32);              /* 00519C  lsr.l  d7,d3 */
                    d5 = d0;                                             /* 00519E  move.l  d0,d5 */
                    d5 |= d1;                                            /* 0051A0  or.l  d1,d5 */
                    d5 |= d2;                                            /* 0051A2  or.l  d2,d5 */
                    d5 |= d3;                                            /* 0051A4  or.l  d3,d5 */
                    d5 = ~d5;                                            /* 0051A6  not.l  d5 */
                    /* 0051A8  tst.w  $20e(a5) */
                    /* 0051AC  bne  $51b4 */
                    if (rd16(V_020E) == 0) {
                        a0 = a0 - 0x8;                                       /* 0051AE  lea  -$8(a0),a0 */
                        /* 0051B2  bra  $51c4 */
                    } else {
                        /* 0051B4  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0051B6  or.w  d3,(a0) */
                        /* 0051B8  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0051BA  or.w  d2,(a0) */
                        /* 0051BC  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0051BE  or.w  d1,(a0) */
                        /* 0051C0  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0051C2  or.w  d0,(a0) */
                    }
                    /* 0051C4  tst.w  $20c(a5) */
                    /* 0051C8  bne  $51d0 */
                    if (rd16(V_020C) == 0) {
                        a0 = a0 - 0x8;                                       /* 0051CA  lea  -$8(a0),a0 */
                        /* 0051CE  bra  $51ea */
                    } else {
                        d0 = (d0 << 16) | (d0 >> 16);                        /* 0051D0  swap  d0 */
                        d1 = (d1 << 16) | (d1 >> 16);                        /* 0051D2  swap  d1 */
                        d2 = (d2 << 16) | (d2 >> 16);                        /* 0051D4  swap  d2 */
                        d3 = (d3 << 16) | (d3 >> 16);                        /* 0051D6  swap  d3 */
                        d5 = (d5 << 16) | (d5 >> 16);                        /* 0051D8  swap  d5 */
                        /* 0051DA  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0051DC  or.w  d3,(a0) */
                        /* 0051DE  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0051E0  or.w  d2,(a0) */
                        /* 0051E2  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0051E4  or.w  d1,(a0) */
                        /* 0051E6  and.w  d5,-(a0) */
                        {
                            uint32_t s = (d5 & 0xFFFF);
                            a0 -= 2;
                            uint32_t d = rd16(a0);
                            uint32_t r = (d & s) & 0xFFFF;
                            wr16(a0, r);
                        }
                        wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0051E8  or.w  d0,(a0) */
                    }
                    a0 = a0 + 0xB6;                                      /* 0051EA  lea  $b6(a0),a0 */
                    /* 0051EE  subq.w  #1,$212(a5) */
                    {
                        uint32_t s = 1;
                        uint32_t d = rd16(V_0212);
                        uint32_t r = (d - s) & 0xFFFF;
                        wr16(V_0212, r);
                        fz = (r == 0);
                        fx = (s > d);
                    }
                    /* 0051F2  bne  $5160 */
                } while (COND(6));
                /* 0051F6  bra  $52f8 */
                goto L_0052F8;
            }
            SETW(d6, (d0 & 0xFFFF));                             /* 0051FA  move.w  d0,d6 */
            d7 = 0x10;                                           /* 0051FC  moveq  #16,d7 */
            SETW(d7, d7 - (d6 & 0xFFFF));                        /* 0051FE  sub.w  d6,d7 */
            do {
                BUDGET_CHECK(0x5200U);
L_005200:
                /* 005200  movem.l  (a1),d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                }
                d0 = shift_op(1, 1, d0, (d7 & 63), 32);              /* 005204  lsl.l  d7,d0 */
                d1 = shift_op(1, 1, d1, (d7 & 63), 32);              /* 005206  lsl.l  d7,d1 */
                d2 = shift_op(1, 1, d2, (d7 & 63), 32);              /* 005208  lsl.l  d7,d2 */
                d3 = shift_op(1, 1, d3, (d7 & 63), 32);              /* 00520A  lsl.l  d7,d3 */
                SETW(d5, (d0 & 0xFFFF));                             /* 00520C  move.w  d0,d5 */
                SETW(d5, d5 | (d1 & 0xFFFF));                        /* 00520E  or.w  d1,d5 */
                SETW(d5, d5 | (d2 & 0xFFFF));                        /* 005210  or.w  d2,d5 */
                SETW(d5, d5 | (d3 & 0xFFFF));                        /* 005212  or.w  d3,d5 */
                SETW(d5, ~(d5 & 0xFFFF));                            /* 005214  not.w  d5 */
                /* 005216  tst.w  $210(a5) */
                /* 00521A  bne  $5222 */
                if (rd16(V_0210) == 0) {
                    a0 = a0 - 0x6;                                       /* 00521C  lea  -$6(a0),a0 */
                    /* 005220  bra  $5232 */
                } else {
                    wr16(a0, rd16(a0) & (d5 & 0xFFFF));                  /* 005222  and.w  d5,(a0) */
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005224  or.w  d3,(a0) */
                    /* 005226  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005228  or.w  d2,(a0) */
                    /* 00522A  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00522C  or.w  d1,(a0) */
                    /* 00522E  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005230  or.w  d0,(a0) */
                }
                /* 005232  movem.l  (a1)+,d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                a1 = ea;
                }
                d0 = shift_op(1, 0, d0, (d6 & 63), 32);              /* 005236  lsr.l  d6,d0 */
                d1 = shift_op(1, 0, d1, (d6 & 63), 32);              /* 005238  lsr.l  d6,d1 */
                d2 = shift_op(1, 0, d2, (d6 & 63), 32);              /* 00523A  lsr.l  d6,d2 */
                d3 = shift_op(1, 0, d3, (d6 & 63), 32);              /* 00523C  lsr.l  d6,d3 */
                d5 = d0;                                             /* 00523E  move.l  d0,d5 */
                d5 |= d1;                                            /* 005240  or.l  d1,d5 */
                d5 |= d2;                                            /* 005242  or.l  d2,d5 */
                d5 |= d3;                                            /* 005244  or.l  d3,d5 */
                d5 = ~d5;                                            /* 005246  not.l  d5 */
                /* 005248  tst.w  $20e(a5) */
                /* 00524C  bne  $5254 */
                if (rd16(V_020E) == 0) {
                    a0 = a0 - 0x8;                                       /* 00524E  lea  -$8(a0),a0 */
                    /* 005252  bra  $5264 */
                } else {
                    /* 005254  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 005256  or.w  d3,(a0) */
                    /* 005258  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 00525A  or.w  d2,(a0) */
                    /* 00525C  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 00525E  or.w  d1,(a0) */
                    /* 005260  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005262  or.w  d0,(a0) */
                }
                /* 005264  tst.w  $20c(a5) */
                /* 005268  bne  $5270 */
                if (rd16(V_020C) == 0) {
                    a0 = a0 - 0x8;                                       /* 00526A  lea  -$8(a0),a0 */
                    /* 00526E  bra  $528a */
                } else {
                    d0 = (d0 << 16) | (d0 >> 16);                        /* 005270  swap  d0 */
                    d1 = (d1 << 16) | (d1 >> 16);                        /* 005272  swap  d1 */
                    d2 = (d2 << 16) | (d2 >> 16);                        /* 005274  swap  d2 */
                    d3 = (d3 << 16) | (d3 >> 16);                        /* 005276  swap  d3 */
                    d5 = (d5 << 16) | (d5 >> 16);                        /* 005278  swap  d5 */
                    /* 00527A  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 00527C  or.w  d3,(a0) */
                    /* 00527E  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 005280  or.w  d2,(a0) */
                    /* 005282  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 005284  or.w  d1,(a0) */
                    /* 005286  and.w  d5,-(a0) */
                    {
                        uint32_t s = (d5 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 005288  or.w  d0,(a0) */
                }
                a0 = a0 + 0xB6;                                      /* 00528A  lea  $b6(a0),a0 */
                /* 00528E  subq.w  #1,$212(a5) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(V_0212);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(V_0212, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 005292  bne  $5200 */
            } while (COND(6));
            /* 005296  bra  $52f8 */
        } else {
            a0 = a0 - 0x8;                                       /* 00529A  lea  -$8(a0),a0 */
            do {
                BUDGET_CHECK(0x529EU);
L_00529E:
                /* 00529E  movem.l  (a1)+,d0-d3 */
                { uint32_t ea;
                ea = a1;
                d0 = rd32(ea);
                ea += 4;
                d1 = rd32(ea);
                ea += 4;
                d2 = rd32(ea);
                ea += 4;
                d3 = rd32(ea);
                ea += 4;
                a1 = ea;
                }
                d6 = d0;                                             /* 0052A2  move.l  d0,d6 */
                d6 |= d1;                                            /* 0052A4  or.l  d1,d6 */
                d6 |= d2;                                            /* 0052A6  or.l  d2,d6 */
                d6 |= d3;                                            /* 0052A8  or.l  d3,d6 */
                d6 = ~d6;                                            /* 0052AA  not.l  d6 */
                /* 0052AC  tst.w  $20e(a5) */
                /* 0052B0  bne  $52b8 */
                if (rd16(V_020E) == 0) {
                    a0 = a0 - 0x6;                                       /* 0052B2  lea  -$6(a0),a0 */
                    /* 0052B6  bra  $52c8 */
                } else {
                    wr16(a0, rd16(a0) & (d6 & 0xFFFF));                  /* 0052B8  and.w  d6,(a0) */
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0052BA  or.w  d3,(a0) */
                    /* 0052BC  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0052BE  or.w  d2,(a0) */
                    /* 0052C0  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0052C2  or.w  d1,(a0) */
                    /* 0052C4  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0052C6  or.w  d0,(a0) */
                }
                /* 0052C8  tst.w  $20c(a5) */
                /* 0052CC  bne  $52d4 */
                if (rd16(V_020C) == 0) {
                    a0 = a0 - 0x8;                                       /* 0052CE  lea  -$8(a0),a0 */
                    /* 0052D2  bra  $52ee */
                } else {
                    d0 = (d0 << 16) | (d0 >> 16);                        /* 0052D4  swap  d0 */
                    d1 = (d1 << 16) | (d1 >> 16);                        /* 0052D6  swap  d1 */
                    d2 = (d2 << 16) | (d2 >> 16);                        /* 0052D8  swap  d2 */
                    d3 = (d3 << 16) | (d3 >> 16);                        /* 0052DA  swap  d3 */
                    d6 = (d6 << 16) | (d6 >> 16);                        /* 0052DC  swap  d6 */
                    /* 0052DE  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d3 & 0xFFFF));                  /* 0052E0  or.w  d3,(a0) */
                    /* 0052E2  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d2 & 0xFFFF));                  /* 0052E4  or.w  d2,(a0) */
                    /* 0052E6  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d1 & 0xFFFF));                  /* 0052E8  or.w  d1,(a0) */
                    /* 0052EA  and.w  d6,-(a0) */
                    {
                        uint32_t s = (d6 & 0xFFFF);
                        a0 -= 2;
                        uint32_t d = rd16(a0);
                        uint32_t r = (d & s) & 0xFFFF;
                        wr16(a0, r);
                    }
                    wr16(a0, rd16(a0) | (d0 & 0xFFFF));                  /* 0052EC  or.w  d0,(a0) */
                }
                a0 = a0 + 0xAE;                                      /* 0052EE  lea  $ae(a0),a0 */
                /* 0052F2  subq.w  #1,$212(a5) */
                {
                    uint32_t s = 1;
                    uint32_t d = rd16(V_0212);
                    uint32_t r = (d - s) & 0xFFFF;
                    wr16(V_0212, r);
                    fz = (r == 0);
                    fx = (s > d);
                }
                /* 0052F6  bne  $529e */
            } while (COND(6));
        }
    }
L_0052F8:
    RTS();                                               /* 0052F8  rts */

resume_dispatch:                                    /* resume in the middle of the routine (when loading a state or after a dynamic jump) */
    {
        uint32_t pc_ = resume_pc; resume_pc = 0;
        switch (pc_) {
        case 0x500CU: goto L_00500C;
        case 0x5086U: goto L_005086;
        case 0x50FEU: goto L_0050FE;
        case 0x5160U: goto L_005160;
        case 0x5200U: goto L_005200;
        case 0x529EU: goto L_00529E;
        }
    }
}

