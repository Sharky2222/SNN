
/* 64 Bit Asynchronous Subtractor

	Inputs:
	min = minuend (64)
	sub = subtrahend (64)
	
	Outputs:
	diff = difference (64)

	Function:	
	min - sub = diff

*/

module subtract_64(min, sub, diff);

	input [63:0] min, sub;
	reg [63:0] sub_tc;
	output reg [63:0] diff;

	
	always @(*) begin
		sub_tc = ~sub + 1'b1;	//twos-compliment of subtrahend
		diff = min + sub_tc;
	end

endmodule
