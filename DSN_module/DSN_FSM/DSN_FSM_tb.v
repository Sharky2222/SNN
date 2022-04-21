module DSN_FSM_tb;
  reg                    clock;
  reg                    reset; // maybe use
  reg       [7:0]         vpre;
  reg       [7:0]         leak; // new
  reg      [12:0]          vth;
  wire    		          spike;
  wire     [12:0]    	 vfire;
  wire   		       fullflag;
  wire  		[7:0] cyclecounter;

	
	
	DSN_FSM dut(.clock(clock), .reset(reset), .vpre(vpre), .leak(leak), .vth(vth), .spike(spike), .vfire(vfire), .fullflag(fullflag), .cyclecounter(cyclecounter));
		
		
	initial begin
		clock <=   0;
	   reset <=   0;
		vpre <= 8'd0;
		leak <= 8'd0;
		vth <= 13'd0;
	end

	initial begin 
		#1000 $stop;
	end

	// Clock rises and falls every 5 seconds
	initial begin
		forever begin
			#10 clock <= ~clock;
		end
	end
		
	assign state = dut.state; // idk if i need this
	assign next_state = dut.next_state; // idk if i need this
		
	always begin
		vth <= 13'b0000000100000; // 32
		leak <= 8'b00000010; // 2
		#180
		vpre <= 8'd32;
		#90
		reset <= 1;
		#180
		reset <= 0;
		vpre <= 8'd32;
		#80;
		/*
		vpre <= 8'd0;
		#80
		vpre <= 8'd64;
		#80
		vpre <= 8'd0;
		#80
		vpre <= 8'd128;
		#80
		vpre <= 8'd0;
		*/
	
	
	end
		
		
		
endmodule	