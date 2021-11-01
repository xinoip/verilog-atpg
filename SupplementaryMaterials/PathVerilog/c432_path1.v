module singlepath (N1, N370);
input N1;
output N370;
wire N360, N357, N348, N300, N263, N242, N199, N154, N260, N224, N118, Vcc, gnd;
not NOT1_1(N118, N1);
nand NAND2_19(N154, N118, Vcc);
and AND9_46(N199, N154, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc);
xor XOR2_50(N224, N154, Vcc);
nand NAND2_56(N242, N1, Vcc);
nand NAND2_68(N260, N224, Vcc);
nand NAND2_69(N263, N224, Vcc);
not NOT1_87(N300, N263);
nand NAND2_117(N348, N300, Vcc);
and AND9_126(N357, N348, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc, Vcc);
not NOT1_127(N360, N357);
not NOT1_128(N370, N357);

endmodule
