
/* 64 Bit 2:1 Multiplexor

	Inputs:
	sel = select bit
	i0 = input 0 (64)
	i1 = input 1 (64)
	
	Outputs:
	out = Output (64)
	
	Function:
	out = selected input

*/

module mux_2x1_64(sel, i0, i1, out);

	input sel;
	input [63:0] i0, i1;
	output reg [63:0] out;
	
	always @(*) begin
		
		out = sel? i1: i0;
		
	end

endmodule