
module var_shr_64_tb();

	reg [63:0] in;
	reg [4:0] shift;
	wire [63:0] out;
	
	var_shr_64 dut(in, shift, out);
	
	initial begin
	
		in = $random;
		shift = 0;
	
	end
	
	always begin
	
		#10
		in = $random;
		shift = shift + 1;
	
	end
	
	
	initial begin
	
		#320;
		$stop;
	
	end

endmodule