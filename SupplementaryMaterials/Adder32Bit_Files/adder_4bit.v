
module adder_4bit(a3, a2, a1, a0, 
					b3, b2, b1, b0, cin, 
					cout, s3 ,s2 ,s1, s0);

input a3, a2, a1, a0, b3, b2, b1, b0, cin;

output cout, s3 ,s2 ,s1, s0;

wire c1, c2, c3;

fa g0(a0, b0, cin, c1, s0);
fa g1(a1, b1, c1, c2, s1);
fa g2(a2, b2, c2, c3, s2);
fa g3(a3, b3, c3, cout, s3);


endmodule
