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
    output reg         VGA_HS, VGA_VS,
    input wire         wb_clk,
    input wire         wb_rst,
    input wire [5:0]   i_wb_adr,
    input wire [31:0]  i_wb_dat,
    input wire [3:0]   i_wb_sel,
    input wire 	       i_wb_we,
    input wire 	       i_wb_cyc,
    input wire 	       i_wb_stb,
    output reg [31:0]  o_wb_rdt,
    output reg 	       o_wb_ack
);

    // Internal wire
    logic  [18:0] pixel_num_wire;
    logic [3:0]  vga_data;
    logic video_on;

    logic wea;
    logic [3:0] dina;
    logic [18:0] addra;

    // Block RAM module instance
    blk_mem_gen_0 vga_image_mem (
        .clka(wb_clk),    // input wire clka
        .wea(wea),      // input wire [0 : 0] wea
        .addra(addra),  // input wire [18 : 0] addra
        .dina(dina),    // input wire [3 : 0] dina
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

    // Wishbone Bus Signals
    reg wb_vga_ack_ff;
    reg [31:0] wb_vga_reg;

    assign o_wb_ack = wb_vga_ack_ff;
    assign o_wb_rdt = wb_vga_reg;
    
    always @(posedge wb_clk, posedge wb_rst) begin
        if (wb_rst) begin
            wb_vga_reg <= 32'h00000000;
            wb_vga_ack_ff <= 0 ;
            wea <= 0;
            dina <= 0;
            addra <= 0;
        end
        else begin
            if (wb_vga_ack_ff && i_wb_we) begin
                case (i_wb_adr)
                // 0x80003000: mapped to BRAM Write Enable
                0: begin
                    if (i_wb_sel[0]) wea <= i_wb_dat[0];
                end
                // 0x80003004: mapped to BRAM Address A
                4: begin
                    if (i_wb_sel[0]) addra[7:0] <= i_wb_dat[7:0];
                    if (i_wb_sel[1]) addra[15:8] <= i_wb_dat[15:8];
                    if (i_wb_sel[2]) addra[18:16] <= i_wb_dat[18:16];
                end
                // 0x80003008: mapped to BRAM Data In A
                8: begin
                    if (i_wb_sel[0]) dina[3:0] <= i_wb_dat[3:0];
                end

                endcase

                // Not sure if this is needed here, please check and confirm
                wb_vga_reg <= i_wb_dat;
            end
            else begin
                wb_vga_reg <= wb_vga_reg;
            end

            // Wishbone ACK signal assignment
            wb_vga_ack_ff <= ! wb_vga_ack_ff & i_wb_stb & i_wb_cyc;
        end
    end
    
endmodule
