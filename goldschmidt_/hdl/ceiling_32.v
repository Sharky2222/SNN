
/* Async 32 Bit Ceiling Function  

	Inputs:
	in = input (32)
	
	Outputs:
	out = output (32)
	
	Function:
	Returns the next highest bit 
	
	Examples: 
	0010 => 0100
	0101 => 1000
	
	Note: inputs higher than (2^31 - 1) overflow

*/

//Current design has loads of prop delay, look into better solution

module ceiling_32(in, out);

	input [31:0] in;
	output [31:0] out;
	
	wire [31:0] mid; //intermediary layer cascading OR gates 
	
	assign mid = 
	{
		 in[31],
		 in[31] | in[30],
		mid[30] | in[29],
		mid[29] | in[28],
		mid[28] | in[27],
		mid[27] | in[26],
		mid[26] | in[25],
		mid[25] | in[24],
		mid[24] | in[23],
		mid[23] | in[22],
		mid[22] | in[21],
		mid[21] | in[20],
		mid[20] | in[19],
		mid[19] | in[18],
		mid[18] | in[17],
		mid[17] | in[16],
		mid[16] | in[15],
		mid[15] | in[14],
		mid[14] | in[13],
		mid[13] | in[12],
		mid[12] | in[11],
		mid[11] | in[10],
		mid[10] |  in[9],
		 mid[9] |  in[8],
		 mid[8] |  in[7],
		 mid[7] |  in[6],
		 mid[6] |  in[5],
		 mid[5] |  in[4],
		 mid[4] |  in[3],
		 mid[3] |  in[2],
		 mid[2] |  in[1],
		 mid[1] |  in[0]
	};
	
	assign out = 
	{
		mid[31] ^ mid[30],
		mid[30] ^ mid[29],
		mid[29] ^ mid[28],
		mid[28] ^ mid[27],
		mid[27] ^ mid[26],
		mid[26] ^ mid[25],
		mid[25] ^ mid[24],
		mid[24] ^ mid[23],
		mid[23] ^ mid[22],
		mid[22] ^ mid[21],
		mid[21] ^ mid[20],
		mid[20] ^ mid[19],
		mid[19] ^ mid[18],
		mid[18] ^ mid[17],
		mid[17] ^ mid[16],
		mid[16] ^ mid[15],
		mid[15] ^ mid[14],
		mid[14] ^ mid[13],
		mid[13] ^ mid[12],
		mid[12] ^ mid[11],
		mid[11] ^ mid[10],
		mid[10] ^  mid[9],
		 mid[9] ^  mid[8],
		 mid[8] ^  mid[7],
		 mid[7] ^  mid[6],
		 mid[6] ^  mid[5],
		 mid[5] ^  mid[4],
		 mid[4] ^  mid[3],
		 mid[3] ^  mid[2],
		 mid[2] ^  mid[1],
		 mid[1] ^  mid[0],
		~mid[0]	
	};

endmodule