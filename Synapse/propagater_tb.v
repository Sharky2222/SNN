module propagater_tb;
	reg  [13:0]     info;
	reg  [12:0]  divisor;
	reg            valid;
	reg              clk;
	reg            reset;
	wire [13:0]      avg;
	wire           ready;

	propagater prop(.info(info), .divisor(divisor), .valid(valid), .clk(clk), .reset(reset), .avg(avg), .ready(ready));
		
	initial begin
		clk <=   0;
	   reset <=   1;
	end

	initial begin 
		#1000 $stop;
	end

	// Clock rises and falls every 5 seconds
	initial begin
		forever begin
			#10 clk <= ~clk;
		end
	end
		
	always begin
		#40
		reset <= 0;
		info <= 14'b00000000011011;
		divisor <= 14'b000000001001;
		valid <= 1'b1;
		#180 $stop;
		//avg should = 27
		
	end
		
		
		
endmodule	