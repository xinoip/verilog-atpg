
module c432_tb();

reg N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,
      N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,
      N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,
      N99,N102,N105,N108,N112,N115;

wire N223,N329,N370,N421,N430,N431,N432;

c432 c0(N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,
		 N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,
		 N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,
		 N99,N102,N105,N108,N112,N115,N223,N329,N370,N421,
		 N430,N431,N432);
		 
	 
reg [35:0] test_vectors[0:9];	
reg [6:0] out_vectors[0:9];	

integer k;
			 
initial
	begin

	$readmemb("c432_input_data.txt", test_vectors);
	
	end
	
initial
	begin
	
	for(k = 0; k < 10; k = k + 1)
		begin		
		#10
		{N1,N4,N8,N11,N14,N17,N21,N24,N27,N30,
			N34,N37,N40,N43,N47,N50,N53,N56,N60,N63,
			N66,N69,N73,N76,N79,N82,N86,N89,N92,N95,
			N99,N102,N105,N108,N112,N115} = test_vectors[k];
			
		#1
		out_vectors[k] = {N223,N329,N370,N421,N430,N431,N432};
		$display("output%d: %h\n", k, out_vectors[k]);
		end
		$writememb("c432_in_data.txt", test_vectors);
		$writememb("c432_out_data.txt", out_vectors);
	
	end



endmodule
