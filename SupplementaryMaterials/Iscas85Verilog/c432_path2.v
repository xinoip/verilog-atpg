module singlepath (N4, N329);
input N4;
output N329;
wire N380, N260, N157, N319, N309, N296, N158, N154, N330, N119, Vcc, gnd;
not NOT1_2(N119, N4);
nand NAND2_19(N154, N4, Vcc);
nor NOR2_20(N157, N119, gnd);
nor NOR2_21(N158, N119, gnd);
nand NAND2_68(N260, N157, Vcc);
and AND9_86(N296, N260, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc);
not NOT1_96(N309, N296);
not NOT1_97(N319, N296);
not NOT1_98(N329, N296);
xor XOR2_99(N330, N260, Vcc);
nand NAND4_138(N380, N4, Vcc, Vcc, Vcc);

endmodule
