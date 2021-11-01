
module c499_tb();


reg N1,N5,N9,N13,N17,N21,N25,N29,N33,N37,
      N41,N45,N49,N53,N57,N61,N65,N69,N73,N77,
      N81,N85,N89,N93,N97,N101,N105,N109,N113,N117,
      N121,N125,N129,N130,N131,N132,N133,N134,N135,N136,
      N137;

wire N724,N725,N726,N727,N728,N729,N730,N731,N732,N733,
       N734,N735,N736,N737,N738,N739,N740,N741,N742,N743,
       N744,N745,N746,N747,N748,N749,N750,N751,N752,N753,
       N754,N755;
		 
		 
	 
c499 c0(N1,N5,N9,N13,N17,N21,N25,N29,N33,N37,
		 N41,N45,N49,N53,N57,N61,N65,N69,N73,N77,
		 N81,N85,N89,N93,N97,N101,N105,N109,N113,N117,
		 N121,N125,N129,N130,N131,N132,N133,N134,N135,N136,
		 N137,N724,N725,N726,N727,N728,N729,N730,N731,N732,
		 N733,N734,N735,N736,N737,N738,N739,N740,N741,N742,
		 N743,N744,N745,N746,N747,N748,N749,N750,N751,N752,
		 N753,N754,N755);
		 
	 
reg [40:0] test_vectors[0:9];	
reg [31:0] out_vectors[0:9];	

integer k;
			 
initial
	begin

	$readmemb("c499_input_data.txt", test_vectors);
	
	end
	
initial
	begin
	
	for(k = 0; k < 10; k = k + 1)
		begin		
		#10
		{N1,N5,N9,N13,N17,N21,N25,N29,N33,N37,
      N41,N45,N49,N53,N57,N61,N65,N69,N73,N77,
      N81,N85,N89,N93,N97,N101,N105,N109,N113,N117,
      N121,N125,N129,N130,N131,N132,N133,N134,N135,N136,
      N137} = test_vectors[k];
			
		#1
		out_vectors[k] = {N724,N725,N726,N727,N728,N729,N730,N731,N732,N733,
       N734,N735,N736,N737,N738,N739,N740,N741,N742,N743,
       N744,N745,N746,N747,N748,N749,N750,N751,N752,N753,
       N754,N755};
		 
		$display("output%d: %h\n", k, out_vectors[k]);
		
		end
		
		$writememb("c499_in_data.txt", test_vectors);
		$writememb("c499_out_data.txt", out_vectors);
	
	end



endmodule
