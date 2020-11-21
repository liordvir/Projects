


module	game_controller	(	
			input	logic	clk,
			input	logic	resetN,
			input	logic	startOfFrame,  // short pulse every start of frame 30Hz 
			input logic shoot,
			input logic god_mode, //invincible cheat
			input logic rapid_fire,
			input logic [10:0] playerCoordinates,
			input logic [7:0] random,
			input logic [NUM_OF_BIRDS - 1:0] bird_alive,
			input logic collision, // active in case of collision between player and tree
			input logic SingleHitPulse, // critical code, generating A single pulse in a frame 
			input logic pickup_hit,
			input logic out_of_time,	
			
			output logic [MAX_TREE_SPEED - 1:0] tree_speed,
			output logic [MAX_BIRD_SPEED - 1:0] bird_speed,
			output logic [NUM_OF_SHOTS - 1:0] deploy_shot,
			output logic [NUM_OF_TREES - 1:0] deploy_tree,
			output logic [NUM_OF_BIRDS - 1:0] deploy_bird,
			output logic deploy_pickup,
			output logic [3:0] bird_life,
			output logic player_red,
			output logic player_active,
			output logic add_time,
			output logic [1:0] [3:0] time_to_add,
			output logic more_damage,
			output logic shield,
			output logic [1:0] num_of_hearts,
			output logic [1:0] [NUM_OF_LEVELS - 1:0] level_num,
			output logic lvl_up
);


parameter NUM_OF_TREES = 16;
parameter NUM_OF_POWERUPS = 4;
parameter NUM_OF_BIRDS = 4;
parameter NUM_OF_SHOTS = 8;

localparam int NUM_OF_LEVELS = 8;
localparam int INITIAL_RED_COUNTER = 64;
localparam int INITIAL_LEVEL_COUNTER = 100;
localparam int INITIAL_POWERUP_TIME = 1000;
localparam int INITIAL_SHOT_COOLDOWN = 50;
localparam int RAPID_SHOT_COOLDOWN = 20;
localparam int MAX_TREE_SPEED = 3;
localparam int MAX_BIRD_SPEED = 2;

localparam logic [1:0] MAX_PLAYER_LIFE = 2'h3;
localparam logic [3:0]  MAX_DIGIT = 4'h9;
localparam logic [3:0]  ZERO_DIGIT = 4'h0;
localparam logic [3:0]  ONE_DIGIT = 4'h1;
localparam logic [NUM_OF_BIRDS - 1:0] ZERO_BIRDS = 4'b0;
localparam logic [NUM_OF_POWERUPS - 1:0] ZERO_POWERUPS = 4'b0;
localparam logic [NUM_OF_SHOTS - 1:0] ZERO_SHOTS = 8'b0;
localparam logic [1:0] ZERO_LIVES = 2'h0;
localparam logic [1:0] ONE_LIFE = 2'h1;
localparam logic [1:0] TWO_LIVES = 2'h2;
localparam logic [1:0] THREE_LIVES = 2'h3;

int cooldown_time;
int cooldown = 0; // cooldown between shots
int current_shot = 0; // index of shot
int current_tree = 0; // index of tree
int last_birds;
int power_up_counter = 0; //time for active powerups
int red_counter; // counter for red color
int level_counter; // cooldown between levels

logic [1:0] player_life = MAX_PLAYER_LIFE; //player life
logic [1:0] num_of_birds; // how many birds per level
logic [NUM_OF_TREES - 1:0] trees_to_add; // trees per level
logic start_level; //indicate start of level
logic level_up; // indicate passing to next level
logic [NUM_OF_POWERUPS - 1:0] powerups; // invinciblity, rapid fire, damage,life.
logic power_up_collision;
logic invincible; // god mode
logic bird_hit_flag; 



levelFsm level_fsm( //FSM for game levels
	.clk(clk),
	.resetN(resetN),
	.level_up(level_up),
	.startOfFrame(startOfFrame),
	.bird_life(bird_life),
	.trees_to_add(deploy_tree),
	.tree_speed(tree_speed),
	.bird_speed(bird_speed),
	.number_of_birds(num_of_birds),
	.level_num(level_num)
);

always_ff@(posedge clk or negedge resetN)
begin
	if(!resetN)
	begin 
	//reset values
		time_to_add <= {MAX_DIGIT, MAX_DIGIT};
		add_time <= 1'b1;
		current_shot <= 0;
		current_tree <= 0;
		cooldown <= 0;
		start_level <= 1'b1;
		level_up <= 1'b0;
		deploy_shot <= ZERO_SHOTS;
		deploy_bird <= ZERO_BIRDS;
		deploy_pickup <= 1'b0;
		player_active <= 1'b1;
		player_life <= MAX_PLAYER_LIFE;
		red_counter <= 0;
		level_counter <= 0;
		bird_hit_flag <= 1'b1;
		last_birds <= 0;
		power_up_counter <= 0;
		powerups <= ZERO_POWERUPS;
		
	end 
	else begin 
		player_active <= player_active;
		powerups <= powerups;
		power_up_counter <= power_up_counter;
		time_to_add <= {ZERO_DIGIT, ZERO_DIGIT};
		add_time <= 1'b0;
		level_up <= 1'b0;
		bird_hit_flag <= bird_hit_flag;
		current_tree <= current_tree;
		
		if (startOfFrame) begin // every frame
			// defaults
			red_counter <= (red_counter > 0) ? red_counter - 1 : 0; 
			level_counter <= (level_counter > 0) ? level_counter - 1 : 0; 
			deploy_shot <= ZERO_SHOTS;
			cooldown <= (cooldown == 0) ? 0 : cooldown - 1; 
			current_shot <= current_shot;
			start_level <= 1'b0;

			deploy_bird <= ZERO_BIRDS;
			deploy_pickup <= 1'b0;
			last_birds <= bird_alive;
			power_up_counter <= (power_up_counter > 0) ? power_up_counter - 1 : 0;	
			if (shoot && (cooldown == 0)) begin // trying to shoot
				deploy_shot[current_shot] <= 1'b1;
				current_shot <= (current_shot == NUM_OF_SHOTS - 1) ? 0 : current_shot + 1;
				cooldown <= cooldown_time; // cooldown between shots
			end
			
			
			if (start_level == 1'b1) begin // at beginning of level
				for (int i = 0; i <= num_of_birds; i++) begin // deploy birds
					deploy_bird[i] <= 1'b1;
				end
				deploy_pickup <= 1'b1; // deploy pickup
			end
			
			if (bird_alive != ZERO_BIRDS) begin //a bird is still alive
				bird_hit_flag <= 1'b0;
			end
			
			if (SingleHitPulse && !invincible) begin // when hit
				if (player_life > ONE_LIFE) begin
					player_life <= player_life - ONE_LIFE;
					red_counter <= INITIAL_RED_COUNTER; // short term invinciblity
				end
				else begin
					// game over (life ran out)
					player_active <= 1'b0;
					player_life <= player_life - ONE_LIFE;
				end
			end
			
			if (out_of_time) begin
				// game over (time ran out)
				player_active <= 1'b0;
			end
			
			if ((bird_hit_flag == 1'b0) && (bird_alive == ZERO_BIRDS)) begin // finished level
				level_counter <= INITIAL_LEVEL_COUNTER; //cooldown until next level
				level_up <= 1'b1;
				bird_hit_flag <= 1'b1;
			end
			
			if ((level_counter == 1) && (bird_alive == ZERO_BIRDS)) begin // starting level when cooldown ends
				start_level <= 1'b1;
			end
			
			if ((bird_alive < last_birds) && player_active) begin // adds time to timer when bird is defeated
				time_to_add <= {ONE_DIGIT, ZERO_DIGIT};
				add_time <= 1'b1;
			end
			
			if (power_up_counter == 0) begin // ends powerups when counter is 0
				powerups <= ZERO_POWERUPS;
			end
			
			if (pickup_hit) begin // pickup taken
				powerups[random[5:4]] <= 1'b1; // randomly choose pickup
				power_up_counter <= INITIAL_POWERUP_TIME; // counter for powerup
			end
			
			if (powerups[3] == 1'b1) begin // life +1 powerup
				player_life <= (player_life > TWO_LIVES) ? player_life : player_life + ONE_LIFE;
				powerups[3] <= 1'b0;
			end
		end
	end 
end

assign player_red = red_counter > 0; //cosmetic red color for player when hit
assign num_of_hearts = player_life; //cosmetic bitmap for hearts
assign lvl_up = level_up;
assign cooldown_time = (rapid_fire || powerups[1] ) ? RAPID_SHOT_COOLDOWN : INITIAL_SHOT_COOLDOWN; //cooldown value between shots
assign more_damage = powerups[2]; //tripple damage powerup
assign invincible = (((red_counter > 0) ? 1'b1 : 1'b0) || god_mode || powerups[0]); //player cannot be harmed
assign shield = god_mode || powerups[0]; //cosmetic change to player bitmap when invincible

endmodule
