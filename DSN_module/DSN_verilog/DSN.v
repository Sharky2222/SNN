module DSN(
  input             clock,
  input             reset,
  input       [8:0] io_vpre,
  input      [12:0] io_vleak_in,
  input      [12:0] io_vth,
  output     [12:0] io_vsumout,
  output reg [12:0] io_vleak_out,
  output reg        io_spike,
  output reg [12:0] io_vfire,
  output reg        flag_full,
  output reg  [7:0] cyclecounter
);


	reg[12:0] vsumout_wl;
	reg[7:0] counter;
	wire wl;

	assign io_vsumout = {4'b0000, io_vpre};
	assign wl = vsumout_wl[12];
	
	
	
	// sequential logic
	always @ (posedge clock) begin
		if (io_vsumout[8] == 1'b1)
			vsumout_wl <= io_vsumout ^ 13'b1000100000000;
		else
			vsumout_wl <= io_vsumout;
	end
		
		
	always @ (posedge clock) begin
		if (wl == 1'b0) begin
			io_vleak_out <= vsumout_wl[11:0] + io_vleak_in;
			io_vfire <= 13'b0;
		end
		else if ( wl == 1'b1) begin
				io_vleak_out <= 13'b0;
				io_vfire <= vsumout_wl[11:0] + io_vleak_in;
			end
	end
		
	always @ (posedge clock) begin			
		if (io_vfire >= io_vth)
			io_spike <= 1'b1;
			counter <= 8'd0; // resets cycle counter
		else
			io_spike <= 1'b0;
			// implement leak here?
	end
	
	
	// full flag
	always @ (posedge clock) begin
		counter <= counter + 1;
		cyclecounter <= counter;
		if(io_vpre[7:0] !==  8'b00000000)
			flag_full = 1'b1;
		else
			flag_full = 1'b0;
	end
	
	
endmodule
