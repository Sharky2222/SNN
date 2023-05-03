
module encoder_32_tb();

	reg [31:0] in;
	wire [4:0] out;
	
	encoder_32 dut(in, out);
	
	initial in = 0;
	
	always begin
	
		#100;
		in = $random;
	
	end
	
	initial begin
	
		#100000;
		$stop;
	
	end

endmodule