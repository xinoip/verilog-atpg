module singlepath (N6, N23);
input N6;
output N23;
wire N19, N16, N11, Vcc, gnd;
nand NAND2_2(N11, N6, Vcc);
nand NAND2_3(N16, N11, Vcc);
nand NAND2_4(N19, N11, Vcc);
nand NAND2_6(N23, N19, Vcc);

endmodule
