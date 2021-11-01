
module fa(a, b, cin, cout, s);

input a, b, cin;

output s, cout;

wire w0, w1, w2, w3;

and g0(w0, a, b);
and g1(w1, cin, b);
and g2(w2, a, cin);
or  g3(cout, w0, w1, w2);

xor g4(w3, a, b);
xor g5(s, w3, cin);


endmodule	