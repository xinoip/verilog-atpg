
module c6288_tb();

reg N1,N18,N35,N52,N69,N86,N103,N120,N137,N154,
      N171,N188,N205,N222,N239,N256,N273,N290,N307,N324,
      N341,N358,N375,N392,N409,N426,N443,N460,N477,N494,
      N511,N528;

wire N545,N1581,N1901,N2223,N2548,N2877,N3211,N3552,N3895,N4241,
       N4591,N4946,N5308,N5672,N5971,N6123,N6150,N6160,N6170,N6180,
       N6190,N6200,N6210,N6220,N6230,N6240,N6250,N6260,N6270,N6280,
       N6287,N6288;

	 
c6288 c0(N1,N18,N35,N52,N69,N86,N103,N120,N137,N154,
              N171,N188,N205,N222,N239,N256,N273,N290,N307,N324,
              N341,N358,N375,N392,N409,N426,N443,N460,N477,N494,
              N511,N528,N545,N1581,N1901,N2223,N2548,N2877,N3211,N3552,
              N3895,N4241,N4591,N4946,N5308,N5672,N5971,N6123,N6150,N6160,
              N6170,N6180,N6190,N6200,N6210,N6220,N6230,N6240,N6250,N6260,
              N6270,N6280,N6287,N6288);
		 
	 
reg [31:0] test_vectors[0:25];	
reg [31:0] out_vectors[0:25];

reg [31:0] tmp_test_vectors;	
reg [31:0] tmp_out_vectors;	

integer ii;
integer k;



			 
initial
	begin

	$readmemb("c6288_input_data.txt", test_vectors);
	
	end
	
initial
	begin
	
	for(k = 0; k < 10; k = k + 1)
		begin	
		
		/*for (ii = 31; ii >= 0; ii = ii - 1)
		  tmp_test_vectors[31-ii] = test_vectors[k][ii];
				  
		test_vectors[k] = tmp_test_vectors;*/
	
		#10
		{N1,N18,N35,N52,N69,N86,N103,N120,N137,N154,
      N171,N188,N205,N222,N239,N256,N273,N290,N307,N324,
      N341,N358,N375,N392,N409,N426,N443,N460,N477,N494,
      N511,N528} = test_vectors[k];
		
			
		#1
		out_vectors[k] = {N545,N1581,N1901,N2223,N2548,N2877,N3211,N3552,N3895,N4241,
       N4591,N4946,N5308,N5672,N5971,N6123,N6150,N6160,N6170,N6180,
       N6190,N6200,N6210,N6220,N6230,N6240,N6250,N6260,N6270,N6280,
       N6287,N6288};
		 
		
		/*for (ii = 31; ii >= 0; ii = ii - 1)
		  tmp_out_vectors[31-ii] = out_vectors[k][ii];
		  
		out_vectors[k] = tmp_out_vectors; */
		 
		$display("output%d: %h\n", k, out_vectors[k]);
		
		end
		
		$writememb("c6288_in_data.txt", test_vectors);
		$writememb("c6288_out_data.txt", out_vectors);
	
	end



endmodule
