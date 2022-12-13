module DSN_FSM(
  input                    clock,
  input                    reset,
  input       [7:0]         vpre, // might need to change to larger bit length
  input       [7:0]         leak, // new
  input      [12:0]          vth,
  output reg               spike,
  output reg [12:0]        vfire,
  output reg            fullflag,
  output reg  [7:0] counter,
  output reg [12:0] vleak // ADDED for debugging
);

	reg[1:0] state, next_state;
	reg[7:0] cyclecounter;
	//reg[12:0] vleak;
	reg dnfflag;
	
	localparam dflt = 2'b00, compare = 2'b01, DNF = 2'b10, DF = 2'b11;
	
	always@(posedge clock) begin //: state_update
				
		if(reset) begin
			vleak <= {5'b00000, vpre[7:0]};
			state <= dflt;
			//next_state <= dflt;
		end else begin
		
			if(spike) begin // set vleak to 0 not to vpre on reset 
				vleak <= 8'b00000000;
			end
			
			//if(init) begin // terrible but it works
			//vleak <= {5'b00000, vpre[7:0]};
			//init <= 1'b0;
			//end
			
			if(dnfflag) begin
				vleak <= vleak + vpre;
			end else if (vleak > 0) begin
				vleak <= vleak - leak;
			end
			
			state <= next_state;
		end
		
	end
	
	
	// next state logic
	always @(state or vpre or vleak or vth) begin
		case(state)
			dflt : 	 if(vpre) begin // might work
							next_state <= compare; end
						 else begin
							next_state <= dflt; end
			
			compare : if(vleak < vth) begin
							next_state <= DNF; end
						 else begin
							next_state <= DF; end
				
			DNF : 		next_state <= dflt;
			DF : 			next_state <= dflt;
			default :   next_state <= dflt;
		endcase
	end
	
	
	// output logic
	always @(state) begin
		if(reset) begin
			fullflag <= 1'b0;
			spike <= 1'b0;
			dnfflag <= 1'b0;
			cyclecounter <= 8'b00000000;
			vfire <= 13'b0000000000000;
			counter <= 8'b00000000;
		end else begin
			case(state)
				dflt : begin
						fullflag <= 1'b0;
						spike <= 1'b0;
						dnfflag <= 1'b0;
						cyclecounter <= cyclecounter + 1'b1; // C = C + 1
						counter <= 8'b0;
						vfire <= 13'b0000000000000;
						//if(spike == 1'b0) begin
						//	cyclecounter <= 8'b00000000; 
						//end 
					end
				compare : 
					begin
						cyclecounter <= cyclecounter + 1'b1; // C = C + 1
						counter <= 8'b0;
						fullflag <= 1'b1;
						dnfflag <= 1'b0;
						spike <= 1'b0;
						vfire <= 13'b0000000000000;
					end
				DNF :
					begin
						cyclecounter <= cyclecounter + 1'b1; // C = C + 1
						counter <= 8'b0;
						dnfflag <= 1'b1; 
						spike <= 1'b0;
						fullflag <= 1'b1;
						vfire <= 13'b0000000000000;
					end
				DF : 
					begin
						counter <= cyclecounter; // C = C + 1
						cyclecounter <= 8'b0;
						spike <= 1'b1;
						vfire <= vleak;
						dnfflag <= 1'b0;
						fullflag <= 1'b1;
					end
			endcase
		end
	end
	
	
	
	
	
endmodule
