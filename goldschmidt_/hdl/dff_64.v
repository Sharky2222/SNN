
/* 64 Bit D Flip Flop

	Inputs:
	clk = clock
	rst = asynchronous reset
	en = enable
	d = data in (64)
	
	Output:
	q = data out (64)

*/

module dff_64(clk, rst, en, d, q);

	input clk, rst, en;
	input [63:0] d;
	output reg [63:0] q;
	
	always @(posedge clk or posedge rst) begin
	
		if (rst) q <= 64'b0;
		else if (en) q <= d;
	
	end

endmodule 