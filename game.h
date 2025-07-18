#pragma once
#include "game_resources.h"




   void screen_coordinates_to_board_coordinates(GameResources & game_resources, double xpos, double ypos, unsigned int & x, unsigned int & y)
    {
        /*
        Convert screen coordinates to board coordinates
        */
        int width, height;
    
        game_resources.get_window_size(width, height);
        float grid_width = float(GRID_WIDTH) * float(width) / float(SCR_WIDTH);
        float grid_height = float(GRID_HEIGHT) * float(height) / float(SCR_HEIGHT);
        x = (unsigned int)(xpos / grid_width - 0.5);
        y = (unsigned int)(ypos / grid_height - 0.5);

    }
class Player
{
    public:
     
        Player()
        {
            name = "Player";
            score = 0;
            captured_stones = 0;
            color = TileType::BLACK;
            is_turn = false;
            is_human = true;
            last_move_x = -1;
            last_move_y = -1;
        }
        Player(std::string name, unsigned int score, unsigned int captured_stones, TileType color, bool is_turn, bool is_human)
        {
            this->name = name;
            this->score = score;
            this->captured_stones = captured_stones;
            this->color = color;
            this->is_turn = is_turn;
            this->is_human = is_human;
            last_move_x = -1;
            last_move_y = -1;
        }
        Player(const Player& other)
        {
            name = other.name;
            score = other.score;
            captured_stones = other.captured_stones;
            color = other.color;
            is_turn = other.is_turn;
            is_human = other.is_human;
            last_move_x = other.last_move_x;
            last_move_y = other.last_move_y;
        }
        Player& operator=(const Player& other)
        {
            if (this != &other)
            {
                name = other.name;
                score = other.score;
                captured_stones = other.captured_stones;
                color = other.color;
                is_turn = other.is_turn;
                is_human = other.is_human;
                last_move_x = other.last_move_x;
                last_move_y = other.last_move_y;
            }
            return *this;
        }
     
        int move(GameResources & game_resources, double xpos, double ypos)
        {
            // Handle player move
            unsigned int num_captures = 0;
            if(is_turn && is_human && game_resources._mouse_clicked)
            {
                unsigned int x, y;
                screen_coordinates_to_board_coordinates(game_resources, xpos, ypos, x, y);
                if(x < game_resources.board.getSize() && y < game_resources.board.getSize())
                {
                    Tile tile = game_resources.board.getTile(x, y);
                    if(tile.getType() == TileType::EMPTY)
                    {
                        tile.setType(color);
                        game_resources.board.setTile(x, y, tile);
                        TileType opponent_color = (color == TileType::BLACK) ? TileType::WHITE : TileType::BLACK;
                        num_captures = game_resources.board.checkCapturedGroups(opponent_color);
                        setLastMove(x, y);

                    }
                }
            }
            else if(!is_human && is_turn)
            {
                // AI move logic goes here
                // For now, just simulate a move
                //readAIInputFromPythonScriptSomehow();
            }
            return num_captures;

        }
        bool isValidMove(GameResources & game_resources)
        {
            // Check if the move is valid
            if(game_resources.board.checkCapturedGroups(color) > 0)  // Check if move is a self-capture
                return false;
            return true;
        }

        void setLastMove(unsigned int x, unsigned int y)
        {
            last_move_x = x;
            last_move_y = y;
        }
        void getLastMove(unsigned int & x, unsigned int & y)
        {
            x = last_move_x;
            y = last_move_y;
        }
        void setName(std::string name)
        {
            this->name = name;
        }
        std::string getName()
        {
            return name;
        }
        void setScore(unsigned int score)
        {
            this->score = score;
        }
        unsigned int getScore()
        {
            return score;
        }
        void setCapturedStones(unsigned int captured_stones)
        {
            this->captured_stones = captured_stones;
        }
        unsigned int getCapturedStones()
        {
            return captured_stones;
        }
        void setColor(TileType color)
        {
            this->color = color;
        }
        TileType getColor()
        {
            return color;
        }
        void setIsTurn(bool is_turn)
        {
            this->is_turn = is_turn;
        }
        bool getIsTurn()
        {
            return is_turn;
        }
        void setIsHuman(bool is_human)
        {
            this->is_human = is_human;
        }
        bool getIsHuman()
        {
            return is_human;
        }
     
    private:
        std::string name;
        unsigned int score;
        unsigned int captured_stones;
        TileType color;
        bool is_turn;
        bool is_human;
        unsigned int last_move_x;
        unsigned int last_move_y;
       
};



class Game
{
    public:
        Game(unsigned int size)
            : board_size(size), grid_width(float(GRID_WIDTH)), grid_height(float(GRID_HEIGHT))
        {
            // Initialize game resources
          
            //game_resources = GameResources();
           // game_resources.init(size, "textures/shinkaya3.png", "shaders/sprite.vs", "shaders/sprite.fs",
                            //    "shaders/geometry.vs", "shaders/geometry.fs");
           game_resources.init(size, "textures/shinkaya3.png", "shaders/sprite.vs", "shaders/sprite.fs",
                            "shaders/geometry.vs", "shaders/geometry.fs");
            init();
            
        }
        ~Game()
        {
            game_resources.deallocate();
            for(unsigned int i = 0; i < board_state.size(); i++)
            {
                delete board_state[i];
            }
        }
        void init()
        {
            // Initialize game state
            player1 = Player();
            player2 = Player();
            player1.setColor(TileType::BLACK);
            player2.setColor(TileType::WHITE);
            player1.setName("Player 1");
            player2.setName("Player 2");
            player1.setIsTurn(true);
            player2.setIsTurn(false);
            player1.setIsHuman(true);
            player2.setIsHuman(true);
            player1.setCapturedStones(0);
            player2.setCapturedStones(0);
            player1.setScore(0);
            player2.setScore(0);
            current_player = player1;
            push_current_state();
         
          
        }
        bool check_ko_violation()
        {
            bool _is_valid_move = true;
            if(board_state.size() > 2)
                {
                    Board * old_state = board_state.at(board_state.size() - 2);
                    Board & current_state = game_resources.board;
                    if(current_state == *old_state)
                    {
                        _is_valid_move = false;
                        
                    }
                    else
                    {
                        _is_valid_move = true;
                    }
                }
                else
                    _is_valid_move = true;
            return _is_valid_move;
        }
     
        void restore_last_board_state()
        {
            // Restore the last board state from the stack
            if(board_state.size() > 0)
            {
                Board * last_state = board_state.back();
                game_resources.board.setBoardState(last_state->getBoardState());
            }
        }

        void switch_turns()
        {
            current_player.setIsTurn(false);
           
            if(current_player.getColor() == TileType::BLACK)
            {
                player1 = current_player;
                
                player2.setIsTurn(true);
                current_player = player2;
            }
            else
            {
                player2 = current_player;
                
                player1.setIsTurn(true);
                current_player = player1;
            }
        }
        void game_loop()
        {
            // Main game loop
            bool _is_valid_move = true;
            unsigned int last_move_x = -1;
            unsigned int last_move_y = -1;
            while(!game_resources.window_should_close())
            {
                
                game_resources.clear_screen();
                game_resources.poll_events();
                game_resources.processInput();
                int num_captures = current_player.move(game_resources, game_resources.xpos, game_resources.ypos);
              
                if(game_resources._mouse_clicked)
                {
                    _is_valid_move = check_ko_violation();
                    _is_valid_move = _is_valid_move & current_player.isValidMove(game_resources);
                
                    if(_is_valid_move)
                    {
                        if(num_captures > 0)
                        {
                            std::cout << "Captured " << num_captures << " stones" << std::endl;
                            current_player.setScore(current_player.getScore() + num_captures);
                        }
                        current_player.getLastMove(last_move_x, last_move_y);
                        switch_turns();
                        push_current_state();
                        std::cout << "Player 1 score: " << player1.getScore() << std::endl;
                        std::cout << "Player 2 score: " << player2.getScore() << std::endl;
                    }
                    else
                    {
                        std::cout << "Invalid move" << std::endl;
                        restore_last_board_state();
                    }
                }
                game_resources.draw();
                if(last_move_x != -1 && last_move_y != -1)
                {
                    game_resources.drawLastMove(last_move_x, last_move_y);
                }
                game_resources.swap_buffers();
               
            }
        }


    private:
        void push_current_state()
        // Push the current state of the board to the stack
        // This function is called at the beginning of the game and after each move
        // It creates a new Tile array and copies the current state of the board into it
        // This is done to keep track of the history of the game
        {
            Board * current_state = new Board(board_size);
            current_state->init(board_size);

            current_state->setBoardState(game_resources.board.getBoardState());
            board_state.push_back(current_state);
        }
        Player player1;
        Player player2;
        Player current_player;
        std::vector<Board*> board_state;
        unsigned int size;
        unsigned int grid_width;
        unsigned int grid_height;
        unsigned int board_size;
        unsigned int stone_texture_width;
        unsigned int stone_texture_height;
        GameResources game_resources;

  

};


