module singlepath (N1, N732);
input N1;
output N732;
wire N712, N703, N701, N700, N630, N702, N625, N716, N620, N724, N350, N274, N250, N580, N338, N572, N708, N290, N340, N406, N577, N574, N720, N570, N601, Vcc, gnd;
xor XOR2_1(N250, N1, Vcc);
xor XOR2_25(N274, N1, Vcc);
xor XOR2_41(N290, N250, Vcc);
xor XOR2_57(N338, N290, Vcc);
xor XOR2_59(N340, N290, Vcc);
xor XOR2_69(N350, N338, Vcc);
xor XOR2_77(N406, N350, Vcc);
not NOT1_97(N570, N406);
not NOT1_99(N572, N406);
not NOT1_101(N574, N406);
not NOT1_104(N577, N406);
not NOT1_107(N580, N406);
and AND4_128(N601, N406, Vcc, Vcc, Vcc);
and AND5_131(N620, N406, Vcc, Vcc, Vcc, Vcc);
and AND5_132(N625, N406, Vcc, Vcc, Vcc, Vcc);
and AND5_133(N630, N570, Vcc, Vcc, Vcc, Vcc);
and AND2_147(N700, N630, Vcc);
and AND2_148(N701, N630, Vcc);
and AND2_149(N702, N630, Vcc);
and AND2_150(N703, N630, Vcc);
and AND2_155(N708, N406, Vcc);
and AND2_159(N712, N406, Vcc);
and AND2_163(N716, N406, Vcc);
and AND2_167(N720, N406, Vcc);
xor XOR2_171(N724, N1, Vcc);
xor XOR2_179(N732, N700, Vcc);

endmodule