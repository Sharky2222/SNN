
/* 32 Bit Synchronous Goldschmidt Divider

	Inputs:
		clk = clock (1)
		rst = reset (1)
		A = Dividend (32)
		B = Divisor (32)
	
	Ouputs:
		Q = Quotient (32)
	
	Function:
		Q = A/B
	
	Notes:
	
*/


module goldschmidt(clk, rst, en, A, B, Q);

	input clk, rst, en;
	input [31:0] A, B;
	output [31:0] Q;
	
//Computing R and r
	
	wire [31:0] Bminus1, R;
	wire [4:0] r;
	
	decrement_32 minus1(B, Bminus1);
	
	ceiling_32 ceiling(Bminus1, R);
	
	encoder_32 encoder(R, r);
	
// MAIN LOOP //	
	
//Mux

	wire [63:0] Aconcat, Bconcat, Ad, Bd, Aq, Bq, Anext, Bnext;
	
	mux_2x1_64 mux_A(rst, Anext, {32'h00000000, A}, Ad); 
	mux_2x1_64 mux_B(rst, Bnext, {32'h00000000, B}, Bd); 
	
	
//Initialize Iteration -- reset is manually set to 0 on flippers
	
	dff_64 A_reg(clk, 1'b0, en, Ad, Aq);
	dff_64 B_reg(clk, 1'b0, en, Bd, Bq);
	
	wire [63:0] Rsh, Y;
	
	shl_64 shl_R({32'h00000000, R}, Rsh);
	
	subtract_64 subtract(Rsh, Bq, Y); 
	
	
//Multiply

	wire [63:0] Aq_times_Y, Bq_times_Y;

	multiply_64 multiply_A(Aq, Y, Aq_times_Y);
	multiply_64 multiply_B(Bq, Y, Bq_times_Y);
	
//Shift
	
	var_shr_64 shr_A(Aq_times_Y, r, Anext);
	var_shr_64 shr_B(Bq_times_Y, r, Bnext);
	
//Quotient

	wire [63:0] quotient;
	
	var_shr_64 shr_Q(Anext, r, quotient);
	assign Q = quotient[31:0];

endmodule