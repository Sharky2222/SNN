import chisel3._
import chisel3.iotesters.PeekPokeTester
import org.scalatest._


class DSNTest extends FlatSpec with Matchers {

  "DSN" should "pass" in {
    chisel3.iotesters.Driver(() => new DSN()) { dut =>
      new PeekPokeTester(dut) {
      //vleak_in_tmp is the inital vleak_in value
      var vleak_in_tmp = 0	
      //vth is the v threshold value that causes a spike
      var vth = 128
      
      //set initial values in the inputs
      poke(dut.io.vleak_in, vleak_in_tmp)  //vleak_in=0
      poke(dut.io.vth, vth)  //vth = 128
	
	//-------------vpre > vth condition---------------------//
	
	//First cycle, set vpre to 127, wl is set to 0, 127+0 
	poke(dut.io.vpre, "b001111111".U) 	
	
	step(1) //updates the clock after writing
	
	println("\n\nVpre is set to 127 and wl is 0") 
	//vleak_in is vleak_out of previous clock cycle 
	println("vleak in is (0): " + peek(dut.io.vleak_in).toString)
	//vleak_out is vpre added to vleak_in
	println("vleakout output is (127): " + peek(dut.io.vleak_out).toString) 
	//spike is 0, spike directly correlates to wl bit and vth
	println("spike output is (0): " + peek(dut.io.spike).toString) 
	//vsumout is vpre with 13 bits, last 4 are 0's padded, this is before wl swap
	println("vsumout output is (127): " + peek(dut.io.vsumout).toString) 
	//vfire is the output voltage, 0 if wl is 0
	println("vfire output is: " + peek(dut.io.vfire).toString) 
	expect(dut.io.spike, 0)
	
	step(1) //update clock after reading
	
	//Second cycle vpre is 2, wl is set to 1, 2 + 127
	//127 sent to vleak_in from previous vleak_out cycle
	//update input vleak for next input signal
	vleak_in_tmp = peek(dut.io.vleak_out).toInt 
	//Sets the vleak_in value after updating input vleak
	poke(dut.io.vleak_in, vleak_in_tmp) //vleak_in = 127
	poke(dut.io.vpre, "b100000010".U) 
	
	step(1) //updates clock after writing
	
	println("\n\n Vpre is set to 2 and wl 1") 
	//vleak_in is 127 from previous
	println("vleak in is (127): " + peek(dut.io.vleak_in).toString) 
	//wl is 1 so vleak_out is 0
	println("vleakout output is (0): " + peek(dut.io.vleak_out).toString) 
	//wl is 1 and 129 > 128 so spike
	println("spike output is (1): " + peek(dut.io.spike).toString) 
	//this is vpre before wl swap
	println("vsumout output is (258) : " + peek(dut.io.vsumout).toString)
	//fires the value because 129 > 128 and wl is 1
	println("vfire output is (129): " + peek(dut.io.vfire).toString) 
	expect(dut.io.spike, 1)
		
	step(1) //updates clock after reading

	//-------------vpre < vth condition---------------------//
	
	//Update input vleak, 127 is vleak_out of previous cycle
	vleak_in_tmp = peek(dut.io.vleak_out).toInt 
	// set third cycle vpre to 126, wl is set to 0, 126+0 
	poke(dut.io.vpre, "b001111110".U) 	
	//127 sent to vleak_in, sets the vleak_in value after updating input vleak
	poke(dut.io.vleak_in, vleak_in_tmp) 
	
	step(1) //updates clock after writing
	
	println("\n\nVpre is set to 126 and wl is 0") 
	println("vleak in is (0): " + peek(dut.io.vleak_in).toString) 
	println("spike output is (0): " + peek(dut.io.spike).toString) 
	println("vsumout output is (126): " + peek(dut.io.vsumout).toString) 
	println("vfire output is (0): " + peek(dut.io.vfire).toString) 
	//future input for the next case, this is not the same as the input, just a reminder
	println("vleakout output is (126): " + peek(dut.io.vleak_out).toString) 
	expect(dut.io.spike, 0)
	
	step(1) //update clock after reading
	
	//update input vleak for next input signal
	vleak_in_tmp = peek(dut.io.vleak_out).toInt 
	//Sets the vleak_in value after updating input vleak_out with 126
	poke(dut.io.vleak_in, vleak_in_tmp) 
	//vpre is 1, wl is 1, 1 + 126
	poke(dut.io.vpre, "b100000001".U) 
	
	step(1) //update clock after writing
	
	println("\n\n Vpre is set to 1 and wl 1")
	println("vleak in is (126): " + peek(dut.io.vleak_in).toString) 
	//wl is 1, so vleak_out is set to 0
	println("vleakout output is (0): " + peek(dut.io.vleak_out).toString) 
	//no spike due to 127 < 128
	println("spike output is (0): " + peek(dut.io.spike).toString) 
	println("vsumout output is (257) : " + peek(dut.io.vsumout).toString)
	//Fires because wl is 1, no spike though because 127 < 128
	println("vfire output is (127): " + peek(dut.io.vfire).toString) 
	expect(dut.io.spike, 0)
	
	step(1) //update clock after reading
		
	//-------------vpre = vth condition---------------------//
	
	//Update input vleak and set vpre
	vleak_in_tmp = peek(dut.io.vleak_out).toInt 
	//vpre to 128, wl is set to 0, 128 + 0 
	poke(dut.io.vpre, "b010000000".U) 	
	//0 sent to vleak_in, sets the vleak_in value after updating input vleak
	poke(dut.io.vleak_in, vleak_in_tmp) 
	
	step(1) //update clock after writing
	
	println("\n\nVpre is set to 128 and wl is 0") 
	println("vleak in is (0): " + peek(dut.io.vleak_in).toString) 
	println("spike output is (0): " + peek(dut.io.spike).toString) 
	println("vsumout output is (128): " + peek(dut.io.vsumout).toString)
	println("vfire output is (0): " + peek(dut.io.vfire).toString) 
	println("vleakout output is (128): " + peek(dut.io.vleak_out).toString) 
	expect(dut.io.spike, 0)
	
	step(1) //update clock after reading
	
	//Update input vleak and set vpre
	vleak_in_tmp = peek(dut.io.vleak_out).toInt 
	//set first cycle vpre to 0, wl is set to 1, 128+0 
	poke(dut.io.vpre, "b100000000".U) 	
	//128 sent to vleak_in
	poke(dut.io.vleak_in, vleak_in_tmp) //Sets the vleak_in value after updating input vleak
	
	step(1) //update clock after writing
	
	println("\n\nVpre is set to 128 and wl is 1") 
	println("vleak in is (128): " + peek(dut.io.vleak_in).toString) 
	println("spike output is (1): " + peek(dut.io.spike).toString) 
	println("vsumout output is (256): " + peek(dut.io.vsumout).toString) 
	println("vfire output is (128): " + peek(dut.io.vfire).toString) 
	println("vleakout output is (0): " + peek(dut.io.vleak_out).toString) 
	expect(dut.io.spike, 1)
	}
     } should be (true)
  }

}
