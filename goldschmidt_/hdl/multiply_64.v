  
/* 64 Bit Asynchronous Multiplier *NOT OPTIMIZED* *WALLACE TREE*

	Inputs:
	A = multiplier
	B = multiplicand
	
	Outputs:
	P = Product

*/

module multiply_64(A, B, P);

	input [63:0] A, B;
	output reg [63:0] P;
	
	always @(*) begin
		P = A * B;
	end

endmodule

