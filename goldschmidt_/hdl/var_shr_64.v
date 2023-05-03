
/* 64 bit Async Variable Right Shift

	Inputs: 
	in (64)
	shift = number of bits shifted (5)
	
	Output: out (64)
	
	Function: 
	in is shifted right (divided by 2) 'shift' times
	(eg in = 1001010, shift = 10, out = 10010)

*/

module var_shr_64(in, shift, out);

	input [63:0] in;
	input [4:0] shift;
	output reg [63:0] out;
	
	always @(*) begin
	
		out = in >> shift; 
	
	end

endmodule