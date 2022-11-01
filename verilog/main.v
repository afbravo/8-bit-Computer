module register(clock, reset, en, D, Q);
    input clock;
    input reset;
    input en;
    input [7:0] D;
    output reg [7:0] Q;

    always @(posedge clock or posedge reset)
        if (reset)
            Q <= 0;
        else if (en)
            Q <= D;
endmodule // Register


