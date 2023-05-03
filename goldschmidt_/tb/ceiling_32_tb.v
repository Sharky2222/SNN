
module ceiling_32_tb();

	reg [31:0] in;
	wire [31:0] out;
	
	ceiling_32 dut(in, out);
	
	initial in = 2147483647; // 2^31 - 1
	
	always begin
	
		#10;
		in = {$random} %(2147483647);
	
	end
	
	initial begin
	
		#1000;
		$stop;
	
	end

endmodule