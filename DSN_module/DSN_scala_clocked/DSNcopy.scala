import chisel3._
import chisel3.util._

class DSN extends Module {
val io = IO(new Bundle {
    val rclk = Input(Bool()) // i think this is right

    val vpre = Input(UInt(9.W))
    val vleak_in = Input(UInt(13.W))
    val vth = Input(UInt(13.W))
    val vsumout = Output(UInt(13.W))
    
    val vleak_out = Output(UInt(13.W)) // changed UInt to reg from lines 13-15
    val spike = Output(UInt(1.W))
    val vfire = Output(UInt(13.W))
    
    //val cycles = Output(UInt(4.W))
    })
    
    io.vleak_out := RegNext(io.vleak_out)
    io.spike := RegNext(io.spike)
    io.vfire := RegNext(io.vfire)
    
   // clock sandbox area
   def risingedge(x: Bool) = x && !RegNext(x)
   def fallingedge(x: Bool) = !x && RegNext(x)
   val posedge = Wire(Bool())
   val negedge = Wire(Bool())
   posedge := risingedge(io.rclk) 
   negedge := fallingedge(io.rclk)
   
   //io.cycles := Reg("b0000".U)
   //when(posedge) {
   //	io.cycles := io.cycles + 1.U
   //}
   
    // end clock sandbox area
  
  io.vsumout := Cat(0.U(4.W), io.vpre)

  val vsumout_wl = Reg(UInt()) //replaced wire with reg
  vsumout_wl := "b0000000000000".U
  
  
  when(io.vsumout(8) === "b1".U){
     vsumout_wl := io.vsumout ^ "b1000100000000".U  //bit 9 is a 1, or will change only this bit to 0
  }.otherwise{
     vsumout_wl := io.vsumout
  }
  val wl = Wire(UInt(1.W))
  wl := vsumout_wl(12)

  io.spike := "b0".U // added reg
  io.vleak_out := "b0".U // added reg
  io.vfire := "b0000000000000".U // added reg
  
  
  when(wl === "b0".U){
    io.vleak_out := vsumout_wl(11, 0) + io.vleak_in  	
    io.vfire := "b0000000000000".U
  }.elsewhen(wl === "b1".U){
    io.vleak_out := "b0000000000000".U
    io.vfire := vsumout_wl(11, 0) + io.vleak_in  
  }
  

  when(io.vfire >= io.vth) {
    io.spike := 1.U
    //io.cycles := "b0000".U
    // reset cycle counter here? check chisel counter documentation 
  } .otherwise {
    io.spike := 0.U
  }
}
// only output cycles when io.spike is 1
object DSN extends App {
  (new chisel3.stage.ChiselStage).emitVerilog(new DSN())
}
