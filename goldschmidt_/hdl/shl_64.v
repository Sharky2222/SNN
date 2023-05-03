
/* 64 bit Asynchronous Left Shift

	Input: in (64)
	
	Output: out (64)
	
	Function: logical 1b left shift

*/

module shl_64(in, out);

	input [63:0] in;
	output reg [63:0] out;
	
	always @(*) begin
	
		out = in << 1; 
	
	end

endmodule