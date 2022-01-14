module d_test_circ_1 (N0, N1, N2, N3, N4, N5, N14, N15);

input N0, N1, N2, N3, N4, N5;

output N14, N15;

wire N28, N29, N30, N33;

and AND2_8 (N28, N0, N1);
nand NAND2_9 (N29, N1, N2);
or OR2_10 (N30, N28, N29);
or OR2_11 (N33, N3, N4);
and AND2_12 (N14, N33, N30);
and AND2_13 (N15, N30, N5);

endmodule
