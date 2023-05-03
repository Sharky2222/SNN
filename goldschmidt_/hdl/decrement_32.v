
// Asynchronous 32 Bit Decrementor


module decrement_32(in, out);

	input [31:0] in;
	output reg [31:0] out;
	
	always @(*) begin
	
		out <= in - 1'b1;
	
	end

endmodule