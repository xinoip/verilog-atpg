//a0 w00 c1 w12 c2 w22 c3 w32 c4 w42 c5 w52 c6 w62 c7 w72 c8 w82 c9 w92 c10 w102 c11 w112 c12 w122 c13 w132 c14 w142 c15 w152 c16 w162 c17 w172 c18 w182 c19 w192 c20 w202 c21 w212 c22 w222 c23 w232 c24 w242 c25 w252 c26 w262 c27 w272 c28 w282 c29 w292 c30 w302 c31 s31
module adder_noh(a31,a30,a29,a28,a27,a26,a25,a24,a23,a22,a21,a20,a19,a18,a17,a16,
						a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0,
						b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,
						b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0,c0,
						cout,s31,s30,s29,s28,s27,s26,s25,s24,s23,s22,s21,s20,s19,s18,s17,s16,
						s15,s14,s13,s12,s11,s10,s9,s8,s7,s6,s5,s4,s3,s2,s1,s0);

input a31,a30,a29,a28,a27,a26,a25,a24,a23,a22,a21,a20,a19,a18,a17,a16,
		a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0,
		b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,
		b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0,c0;

output cout,s31,s30,s29,s28,s27,s26,s25,s24,s23,s22,s21,s20,s19,s18,s17,s16,
		s15,s14,s13,s12,s11,s10,s9,s8,s7,s6,s5,s4,s3,s2,s1,s0;

wire c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16, c17, c18, c19, c20, c21, c22, c23, c24, c25, c26, c27, c28, c29, c30, c31,
    w00, w01, w02, w03,
    w10, w11, w12, w13,
    w20, w21, w22, w23,
    w30, w31, w32, w33,
    w40, w41, w42, w43,
    w50, w51, w52, w53,
    w60, w61, w62, w63,
    w70, w71, w72, w73,
    w80, w81, w82, w83,
    w90, w91, w92, w93,
    w100, w101, w102, w103,
    w110, w111, w112, w113,
    w120, w121, w122, w123,
    w130, w131, w132, w133,
    w140, w141, w142, w143,
    w150, w151, w152, w153,
    w160, w161, w162, w163,
    w170, w171, w172, w173,
    w180, w181, w182, w183,
    w190, w191, w192, w193,
    w200, w201, w202, w203,
    w210, w211, w212, w213,
    w220, w221, w222, w223,
    w230, w231, w232, w233,
    w240, w241, w242, w243,
    w250, w251, w252, w253,
    w260, w261, w262, w263,
    w270, w271, w272, w273,
    w280, w281, w282, w283,
    w290, w291, w292, w293,
    w300, w301, w302, w303,
    w310, w311, w312, w313;
    
    
    

and g0(w00, a0, b0);
and g1(w01, c0, b0);
and g2(w02, a0, c0);
or  g3(c1, w00, w01, w02);
xor g4(w03, a0, b0);
xor g5(s0, w03, c0);

and ag0(w10, a1, b1);
and ag1(w11, c1, b1);
and ag2(w12, a1, c1);
or  ag3(c2, w10, w11, w12);
xor ag4(w13, a1, b1);
xor ag5(s1, w13, c1);

and bg0(w20, a2, b2);
and bg1(w21, c2, b2);
and bg2(w22, a2, c2);
or  bg3(c3, w20, w21, w22);
xor bg4(w23, a2, b2);
xor bg5(s2, w23, c2);

and cg0(w30, a3, b3);
and cg1(w31, c3, b3);
and cg2(w32, a3, c3);
or  cg3(c4, w30, w31, w32);
xor cg4(w33, a3, b3);
xor cg5(s3, w33, c3);



and dg0(w40, a4, b4);
and dg1(w41, c4, b4);
and dg2(w42, a4, c4);
or  dg3(c5, w40, w41, w42);
xor dg4(w43, a4, b4);
xor dg5(s4, w43, c4);

and eg0(w50, a5, b5);
and eg1(w51, c5, b5);
and eg2(w52, a5, c5);
or  eg3(c6, w50, w51, w52);
xor eg4(w53, a5, b5);
xor eg5(s5, w53, c5);

and fg0(w60, a6, b6);
and fg1(w61, c6, b6);
and fg2(w62, a6, c6);
or  fg3(c7, w60, w61, w62);
xor fg4(w63, a6, b6);
xor fg5(s6, w63, c6);

and gg0(w70, a7, b7);
and gg1(w71, c7, b7);
and gg2(w72, a7, c7);
or  gg3(c8, w70, w71, w72);
xor gg4(w73, a7, b7);
xor gg5(s7, w73, c7);



and hg0(w80, a8, b8);
and hg1(w81, c8, b8);
and hg2(w82, a8, c8);
or  hg3(c9, w80, w81, w82);
xor hg4(w83, a8, b8);
xor hg5(s8, w83, c8);

and ig0(w90, a9, b9);
and ig1(w91, c9, b9);
and ig2(w92, a9, c9);
or  ig3(c10, w90, w91, w92);
xor ig4(w93, a9, b9);
xor ig5(s9, w93, c9);

and jg0(w100, a10, b10);
and jg1(w101, c10, b10);
and jg2(w102, a10, c10);
or  jg3(c11, w100, w101, w102);
xor jg4(w103, a10, b10);
xor jg5(s10, w103, c10);

and kg0(w110, a11, b11);
and kg1(w111, c11, b11);
and kg2(w112, a11, c11);
or  kg3(c12, w110, w111, w112);
xor kg4(w113, a11, b11);
xor kg5(s11, w113, c11);



and lg0(w120, a12, b12);
and lg1(w121, c12, b12);
and lg2(w122, a12, c12);
or  lg3(c13, w120, w121, w122);
xor lg4(w123, a12, b12);
xor lg5(s12, w123, c12);

and mg0(w130, a13, b13);
and mg1(w131, c13, b13);
and mg2(w132, a13, c13);
or  mg3(c14, w130, w131, w132);
xor mg4(w133, a13, b13);
xor mg5(s13, w133, c13);

and ng0(w140, a14, b14);
and ng1(w141, c14, b14);
and ng2(w142, a14, c14);
or  ng3(c15, w140, w141, w142);
xor ng4(w143, a14, b14);
xor ng5(s14, w143, c14);

and og0(w150, a15, b15);
and og1(w151, c15, b15);
and og2(w152, a15, c15);
or  og3(c16, w150, w151, w152);
xor og4(w153, a15, b15);
xor og5(s15, w153, c15);



and pg0(w160, a16, b16);
and pg1(w161, c16, b16);
and pg2(w162, a16, c16);
or  pg3(c17, w160, w161, w162);
xor pg4(w163, a16, b16);
xor pg5(s16, w163, c16);

and qg0(w170, a17, b17);
and qg1(w171, c17, b17);
and qg2(w172, a17, c17);
or  qg3(c18, w170, w171, w172);
xor qg4(w173, a17, b17);
xor qg5(s17, w173, c17);

and rg0(w180, a18, b18);
and rg1(w181, c18, b18);
and rg2(w182, a18, c18);
or  rg3(c19, w180, w181, w182);
xor rg4(w183, a18, b18);
xor rg5(s18, w183, c18);

and sg0(w190, a19, b19);
and sg1(w191, c19, b19);
and sg2(w192, a19, c19);
or  sg3(c20, w190, w191, w192);
xor sg4(w193, a19, b19);
xor sg5(s19, w193, c19);



and tg0(w200, a20, b20);
and tg1(w201, c20, b20);
and tg2(w202, a20, c20);
or  tg3(c21, w200, w201, w202);
xor tg4(w203, a20, b20);
xor tg5(s20, w203, c20);

and ug0(w210, a21, b21);
and ug1(w211, c21, b21);
and ug2(w212, a21, c21);
or  ug3(c22, w210, w211, w212);
xor ug4(w213, a21, b21);
xor ug5(s21, w213, c21);

and vg0(w220, a22, b22);
and vg1(w221, c22, b22);
and vg2(w222, a22, c22);
or  vg3(c23, w220, w221, w222);
xor vg4(w223, a22, b22);
xor vg5(s22, w223, c22);

and wg0(w230, a23, b23);
and wg1(w231, c23, b23);
and wg2(w232, a23, c23);
or  wg3(c24, w230, w231, w232);
xor wg4(w233, a23, b23);
xor wg5(s23, w233, c23);



and yg0(w240, a24, b24);
and yg1(w241, c24, b24);
and yg2(w242, a24, c24);
or  yg3(c25, w240, w241, w242);
xor yg4(w243, a24, b24);
xor yg5(s24, w243, c24);

and zg0(w250, a25, b25);
and zg1(w251, c25, b25);
and zg2(w252, a25, c25);
or  zg3(c26, w250, w251, w252);
xor zg4(w253, a25, b25);
xor zg5(s25, w253, c25);

and g0a(w260, a26, b26);
and g1a(w261, c26, b26);
and g2a(w262, a26, c26);
or  g3a(c27, w260, w261, w262);
xor g4a(w263, a26, b26);
xor g5a(s26, w263, c26);

and g0b(w270, a27, b27);
and g1b(w271, c27, b27);
and g2b(w272, a27, c27);
or  g3b(c28, w270, w271, w272);
xor g4b(w273, a27, b27);
xor g5b(s27, w273, c27);



and g0c(w280, a28, b28);
and g1c(w281, c28, b28);
and g2c(w282, a28, c28);
or  g3c(c29, w280, w281, w282);
xor g4c(w283, a28, b28);
xor g5c(s28, w283, c28);

and g0d(w290, a29, b29);
and g1d(w291, c29, b29);
and g2d(w292, a29, c29);
or  g3d(c30, w290, w291, w292);
xor g4d(w293, a29, b29);
xor g5d(s29, w293, c29);

and g0e(w300, a30, b30);
and g1e(w301, c30, b30);
and g2e(w302, a30, c30);
or  g3e(c31, w300, w301, w302);
xor g4e(w303, a30, b30);
xor g5e(s30, w303, c30);

and g0f(w310, a31, b31);
and g1f(w311, c31, b31);
and g2f(w312, a31, c31);
or  g3f(cout, w310, w311, w312);
xor g4f(w313, a31, b31);
xor g5f(s31, w313, c31);

endmodule
