module DSN_FSM(
  input                    clock,
  input                    reset,
  input       [7:0]         vpre,
  input       [7:0]         leak, // new
  input      [12:0]          vth,
  output reg               spike,
  output reg [12:0]        vfire,
  output reg            fullflag,
  output reg  [7:0] cyclecounter
);

	reg[1:0] state, next_state;
	reg init = 1'b1; // hmmmm
	reg[7:0] counter = 8'd0;
  reg[12:0] vleak;
	reg dnfflag;
	
	localparam dflt = 2'b00, compare = 2'b01, DNF = 2'b10, DF = 2'b11;
	
	
	
	//state update block
	always @(posedge clock) begin // added posedge reset
		state <= next_state;
		
		if(init) begin // terrible but it works
		vleak <= {5'b00000, vpre[7:0]};
		init <= 1'b0;
		end
		
		if(dnfflag)
			vleak <= vleak + vpre;
		else if (vleak > 0)
			vleak <= vleak - leak;
			
		if(spike | reset) // set vleak to 0 not to vpre on reset
			vleak <= 8'b00000000;
		
	end
	
	
	// next state logic
	always @(state or vpre or vleak or vth or reset) begin // added or reset
		case(state)
			dflt : 	 if(vpre & ~reset) begin // might work
							next_state <= compare; end
						 else begin
							next_state <= dflt; end
			
			compare : if(vleak < vth) begin
							next_state <= DNF; end
						else if(reset) begin       // new
							next_state <= dflt; end // new
						 else begin
							next_state <= DF; end
				
			DNF : 		next_state <= dflt;
			DF : 			next_state <= dflt;
			default :   next_state <= dflt;
		endcase
	end
	
	
	// output logic
	always @(state) begin
		case(state)
			dflt : begin
					fullflag <= 1'b0;
					spike <= 1'b0;
					dnfflag <= 0;
					cyclecounter <= 8'b00000000;
					vfire <= 13'b0000000000000;
				end
			compare : 
				begin
					counter <= counter + 1; // C = C + 1
					fullflag <= 1'b1;
					dnfflag <= 0;
				end
			DNF :
				begin
					counter <= counter + 1; // C = C + 1
					dnfflag <= 1; 
					spike <= 1'b0;
				end
			DF : 
				begin
					cyclecounter <= counter + 1; // C = C + 1
					counter <= 8'b0;
					spike <= 1'b1;
					vfire <= vleak;
					dnfflag <= 0;
				end
			default: fullflag <= 1'b0;
		endcase
	end
	
	
	
	
	
endmodule
