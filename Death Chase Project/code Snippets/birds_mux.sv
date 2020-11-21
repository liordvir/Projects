

module	birds_mux	(	
//		--------	Clock Input	 	
		input		logic	clk,
		input		logic	resetN,
		input	logic	[3:0] birdsBusRequest,
		input	logic	[3:0] [7:0] birdsBusRGB, 
				
		output	logic birdsDrawingRequest,
		output	logic	[7:0] birdsRGB 
					
);

logic [7:0] tmpRGB;



assign birdsRGB	  = tmpRGB; //--  extend LSB to create 10 bits per color  
assign birdsDrawingRequest  = (birdsBusRequest[0] || birdsBusRequest [1] //should anyone be displayed
						|| birdsBusRequest[2] || birdsBusRequest[3]);



always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN) begin
			tmpRGB	<= 8'b0;
	end
	else begin
		if (birdsBusRequest[0] == 1'b1 )   
			tmpRGB <= birdsBusRGB[0];  //first priority 

		else if (birdsBusRequest[1] == 1'b1 )   
			tmpRGB <= birdsBusRGB[1];  //second priority 
			
		else if (birdsBusRequest[2] == 1'b1 )   
			tmpRGB <= birdsBusRGB[2];  //third priority 
		
		else if (birdsBusRequest[3] == 1'b1 )   
			tmpRGB <= birdsBusRGB[3];  //forth priority 
		
		else
			tmpRGB <= 8'hff ; // last priority 
		end ; 
	end

endmodule


