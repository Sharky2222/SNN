
module goldschmidt_tb ();
	/*
	reg [8*45:1] str; //string 
	integer test_read, test_write; //read/write file
	integer i; //iterator

	reg [31:0] q_est; //quotient estimated by goldschmidt
	
	initial begin
		
		test_read = $fopen("test_read.txt", "r"); //open read file
		test_write = $fopen("test_write.txt", "w"); //open write file
		
		i = 0;
		
		while (! $feof(test_read)) begin //while not at end of file
		
			$fgets(str, test_read); //read string and store in str
		
			$display("%0s", str); //display currently-held string
			$fdisplay(test_write, "%0s", str);
			$fdisplay(test_write, str);
			
			i = i + 1;
			
		end
		
		$fclose(test_read);
		
		$fclose(test_write);
	
	end
	*/

	reg clk, rst, en;
	reg [31:0] A, B; 		//dividend (A) and divisor (B)
	wire [31:0] Q_est; 	//quotient estimated by goldschmidt
	integer test_write;  //CSV file to be imported to excel
	integer Q_exp; 		//expected value for quotient
	real  err [4:0]; 		//error b/t Q_exp and Q_est
	integer i, j, k; 		//loop iterators
	
	goldschmidt dut(clk, rst, en, A, B, Q_est);
	
	initial begin
	
		en = 1'b1;
		clk = 1'b0;
	
		test_write = $fopen("test_write.txt", "w");
		
		$fdisplay(test_write, "Dividend, Divisor, True Value, est1, est2, est3, est4, est5, err1, err2, err3, err4, err5");
		
		for (i = 1; i < 30; i = i + 1) begin //loop for A
			
			for (j = 1; j < 30; j = j + 1) begin //loop B values
			
				rst = 1'b1;
				A = 2**i + ({$random} %(2**(i-1)));
				B = 2**j + ({$random} %(2**(j-1)));
				
				Q_exp = A / B;
				
				#10;
				rst = 1'b0;
				
				$fwrite(test_write, "%0d , %0d , %0d ,", A, B, Q_exp);
				
				#10;
				
				for (k = 0; k < 5; k = k + 1) begin //loop for one iteration of the architecture
					
					err[k] = Q_exp > Q_est ? 100 * (Q_exp - Q_est) / Q_exp: 100 * (Q_est - Q_exp) / Q_exp;
					$fwrite(test_write, " %0d ,", Q_est);
					#20;
					
				end
				
				for (k = 0; k < 5; k = k + 1) $fwrite(test_write, " %0d ,", err[k]); //error measures
				
				$fdisplay(test_write);
				
			end
			
		end
		
		$fclose(test_write);
		$stop;
		
	end
	
	always begin
	
		#10;
		clk = ~clk;
		
	end


endmodule
