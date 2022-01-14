module d_test_circ_2 (N0, N1, N2, N7);

input N0, N1, N2;

output N7;

wire N25, N26;

and AND2_4 (N25, N0, N1);
nand NAND2_5 (N26, N1, N2);
nor NOR2_6 (N7, N25, N26);

endmodule
