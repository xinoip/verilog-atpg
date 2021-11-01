
module adder_16bit(a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0,cin,
						cout,s15,s14,s13,s12,s11,s10,s9,s8,s7,s6,s5,s4,s3,s2,s1,s0);

input a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0,
		b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0,cin;

output cout,s15,s14,s13,s12,s11,s10,s9,s8,s7,s6,s5,s4,s3,s2,s1,s0;

wire c12,c8,c4;

adder_4bit g0(a3,a2,a1,a0,b3,b2,b1,b0,cin,c4,s3,s2,s1,s0);
adder_4bit g1(a7,a6,a5,a4,b7,b6,b5,b4,c4,c8,s7,s6,s5,s4);
adder_4bit g2(a11,a10,a9,a8,b11,b10,b9,b8,c8,c12,s11,s10,s9,s8);
adder_4bit g3(a15,a14,a13,a12,b15,b14,b13,b12,c12,cout,s15,s14,s13,s12);

endmodule
