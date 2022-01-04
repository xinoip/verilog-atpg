// Verilog
// false_path
// Ninputs 3
// Noutputs 1
// NtotalGates 2
// NAND2 1
// OR2 1

module falsepath (A,B,G);

input A,B;

output G;

wire F;

nand NAND2_1 (F, A, B);
nor OR2_1 (G, F, B);

endmodule
