// vga.sv: VGA Display Controller Peripheral
// Version: 1.0
// Author: Atharva Lele <atharva@pdx.edu>
//
// Version Details: 1.0: B&W Static Image Display Support

module vga_controller (
    input              clock, reset,
    output reg [3:0]   VGA_R,
    output reg [3:0]   VGA_G,
    output reg [3:0]   VGA_B,
    output reg         VGA_HS, VGA_VS
);

    // Internal wire
    logic  [18:0] pixel_num_wire;
    logic [3:0]  vga_data;
    logic video_on;

    // Block RAM module instance
    blk_mem_gen_0 vga_image_mem (
        .clka(clock),    // input wire clka
        .wea(1'b0),      // input wire [0 : 0] wea
        .addra(19'b0),  // input wire [18 : 0] addra
        .dina(4'b0),    // input wire [3 : 0] dina
        .clkb(clock),    // input wire clkb
        .addrb(pixel_num_wire),  // input wire [18 : 0] addrb
        .doutb(vga_data)  // output wire [3 : 0] doutb
    );

    // Display Timing Generator Instance
    dtg vga_dtg (
        .clock(clock),
        .rst(1'b0),
        .horiz_sync(VGA_HS),
        .vert_sync(VGA_VS),
        .video_on(video_on),
        .pixel_row(),
        .pixel_column(),
        .pix_num(pixel_num_wire)
    );

    // Assign data to VGA
    always_ff @(posedge clock) begin : vga_data_assign
        if (video_on == 1'b1) begin
            VGA_R <= vga_data;
            VGA_G <= vga_data;
            VGA_B <= vga_data;
        end
        else begin
            VGA_R <= 4'b0;
            VGA_G <= 4'b0;
            VGA_B <= 4'b0;
        end
    end: vga_data_assign
    
endmodule
