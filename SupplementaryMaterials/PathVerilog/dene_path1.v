module singlepath (N1, N199);
input N1;
output N199;
wire N154, N118, Vcc, gnd;
not NOT1_1(N118, N1);
nand NAND2_19(N154, N118, Vcc);
and AND9_46(N199, N154, Vcc, Vcc);

endmodule
