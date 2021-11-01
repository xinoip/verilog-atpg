
module c1908_tb();

	 
reg N1,N4,N7,N10,N13,N16,N19,N22,N25,N28,
      N31,N34,N37,N40,N43,N46,N49,N53,N56,N60,
      N63,N66,N69,N72,N76,N79,N82,N85,N88,N91,
      N94,N99,N104;

wire N2753,N2754,N2755,N2756,N2762,N2767,N2768,N2779,N2780,N2781,
       N2782,N2783,N2784,N2785,N2786,N2787,N2811,N2886,N2887,N2888,
       N2889,N2890,N2891,N2892,N2899;

c1908 c0(N1,N4,N7,N10,N13,N16,N19,N22,N25,N28,
		  N31,N34,N37,N40,N43,N46,N49,N53,N56,N60,
		  N63,N66,N69,N72,N76,N79,N82,N85,N88,N91,
		  N94,N99,N104,N2753,N2754,N2755,N2756,N2762,N2767,N2768,
		  N2779,N2780,N2781,N2782,N2783,N2784,N2785,N2786,N2787,N2811,
		  N2886,N2887,N2888,N2889,N2890,N2891,N2892,N2899);
  
		 
	 
reg [32:0] test_vectors[0:9];	
reg [24:0] out_vectors[0:9];	

integer k;
			 
initial
	begin

	$readmemb("c1908_input_data.txt", test_vectors);
	
	end
	
initial
	begin
	
	for(k = 0; k < 10; k = k + 1)
		begin		
		#10
		{N1,N4,N7,N10,N13,N16,N19,N22,N25,N28,
      N31,N34,N37,N40,N43,N46,N49,N53,N56,N60,
      N63,N66,N69,N72,N76,N79,N82,N85,N88,N91,
      N94,N99,N104} = test_vectors[k];
			
		#1
		out_vectors[k] = {N2753,N2754,N2755,N2756,N2762,N2767,N2768,N2779,N2780,N2781,
       N2782,N2783,N2784,N2785,N2786,N2787,N2811,N2886,N2887,N2888,
       N2889,N2890,N2891,N2892,N2899};
		 
		$display("output%d: %h\n", k, out_vectors[k]);
		
		end
		
		$writememb("c1908_in_data.txt", test_vectors);
		$writememb("c1908_out_data.txt", out_vectors);
	
	end



endmodule
