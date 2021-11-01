
module s1423_tb();
  
reg CK,G0,G1,G2,G3,G4,G5,G6,G7,G8,G9,G10,G11,G12,G13,G14,G15,G16;
wire G726,G729,G702,G727,G701BF;  



s1423 c0(CK,G0,G1,G2,G3,G4,G5,G6,G7,G8,G9,G10,G11,G12,G13,G14,G15,G16,G726,G729,G702,G727,G701BF);
    
	 
reg [16:0] test_vectors[0:49];	
reg [4:0] out_vectors[0:49];	

integer k;
			 
initial
	begin
	$readmemb("s1423_input_data.txt", test_vectors);
	CK = 1'b1;	
	end
	
initial
	begin
	
	for(k = 0; k < 50; k = k + 1)
		begin		
		{G0,G1,G2,G3,G4,G5,G6,G7,G8,G9,G10,G11,G12,G13,G14,G15,G16} = test_vectors[k];
		#8
		out_vectors[k] = {G726,G729,G702,G727,G701BF};
		#2
		$display("output%d: %h\n", k, out_vectors[k]);
		
		end
		
		$writememb("s1423_in_data.txt", test_vectors);
		$writememb("s1423_out_data.txt", out_vectors);
	
	end
	
always
	begin
	#5
	CK = ~CK;
	end

endmodule
