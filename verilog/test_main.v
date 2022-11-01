`timescale 1ps/1ps
`include "main.v"

module register_test;

reg clock = 0;
reg en = 0;
reg reset = 0;
reg [7:0]in;
wire [7:0]out;

always #1 clock = !clock;

initial $dumpfile("register.vcd");
initial $dumpvars(0, register_test);

register r(clock, reset, en, in, out);

initial begin
    #1 reset = 1;
    #1 reset = 0;
    #1 en = 1; in = 8'b10011101;
    #1 en = 0;
    #1 $finish;
end
endmodule