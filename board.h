#pragma once
#include "renderers.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int GRID_WIDTH = 30;
const unsigned int GRID_HEIGHT = 30;

const unsigned int BOARD_SIZE = 19;

const unsigned int STONE_TEXTURE_WIDTH = 100;

enum class TileType
{
    EMPTY,
    BLACK,
    WHITE
};

class Tile
{
    public:
        Tile(TileType type, glm::vec2 position, float rotate, glm::vec3 color)
            : _type(type), _isCaptured(false), _isSelected(false)
        {
            glm::vec2 size = glm::vec2(GRID_WIDTH, GRID_HEIGHT);
            switch(_type)
            {
                case TileType::EMPTY:
                    _sprite = Sprite(position, size, rotate, color, glm::vec2(1433.0f, 1333.0f), glm::vec2(STONE_TEXTURE_WIDTH, STONE_TEXTURE_WIDTH));
                    break;
                case TileType::BLACK:
                    _sprite = Sprite(position, size, rotate, color, glm::vec2(1333.0f, 1333.0f), glm::vec2(STONE_TEXTURE_WIDTH, STONE_TEXTURE_WIDTH));
                    break;
                case TileType::WHITE:
                    _sprite = Sprite(position, size, rotate, color, glm::vec2(1233.0f, 1333.0f), glm::vec2(STONE_TEXTURE_WIDTH, STONE_TEXTURE_WIDTH));
                    break;
            }
        }
        
        Tile()
        {
            // Default constructor
            _type = TileType::EMPTY;
            _isCaptured = false;
            _isSelected = false;
            _isChecked = false;
            _sprite = Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(GRID_WIDTH, GRID_HEIGHT), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1433.0f, 1333.0f), glm::vec2(STONE_TEXTURE_WIDTH, STONE_TEXTURE_WIDTH));
        }


        Tile& operator=(const Tile& other) // assignment operator
        {
            if (this == &other) return *this; // self-assignment check
            // Copy the values from the other object
            _type = other._type;
            _sprite = other._sprite;
            _isCaptured = other._isCaptured;
            _isChecked = other._isChecked;
            _isSelected = other._isSelected;
            return *this;
        }
        ~Tile()
        {
            // Destructor
        }

        

        void draw(SpriteRenderer& spriteRenderer)
        {
            spriteRenderer.DrawSprite(_sprite);
        }
        void setPosition(glm::vec2 position)
        {
            _sprite.position = position;
        }
        void setSize(glm::vec2 size)
        {
            _sprite.size = size;
        }
        void setRotate(float rotate)
        {
            _sprite.rotate = rotate;
        }
        void setColor(glm::vec3 color)
        {
            _sprite.color = color;
        }
        void setTexCoords(glm::vec2 texCoords)
        {
            _sprite.texCoords = texCoords;
        }
        void setTexSize(glm::vec2 texSize)
        {
            _sprite.texSize = texSize;
        }
        void setType(TileType type)
        {
            _type = type;
            switch(_type)
            {
                case TileType::EMPTY:
                    _sprite.texCoords = glm::vec2(1433.0f, 1333.0f);
                    break;
                case TileType::BLACK:
                    _sprite.texCoords = glm::vec2(1333.0f, 1333.0f);
                    break;
                case TileType::WHITE:
                    _sprite.texCoords = glm::vec2(1233.0f, 1333.0f);
                    break;
            }
        }
        void setPosition(float x, float y)
        {
            _sprite.position = glm::vec2(x, y);
        }
        void setCaptured(bool captured)
        {
            _isCaptured = captured;
        }
        void setSelected(bool selected)
        {
            _isSelected = selected;
        }
        TileType getType() const
        {
            return _type;
        }
        Sprite getSprite() const
        {
            return _sprite;
        }
        bool isCaptured() const
        {
            return _isCaptured;
        }
        bool isSelected() const
        {
            return _isSelected;
        }
        bool isChecked() const
        {
            return _isChecked;
        }
        void setChecked(bool checked)
        {
            _isChecked = checked;
        }
    private:
        TileType _type;
        Sprite _sprite;

        bool _isCaptured;
        bool _isSelected;     
        bool _isChecked; // used for checking if the tile has been checked for capture 
};


class Board
{
    public:
        Board()
        {
            _is_initialized = false;
        }
        Board(unsigned int size)
            : _size(size)
        {
            _tiles = new Tile*[size];
            for(unsigned int i = 0; i < size; i++)
            {
                _tiles[i] = new Tile[size];
                for(unsigned int j = 0; j < size; j++)
                {
                    _tiles[i][j].setPosition(glm::vec2(i * GRID_WIDTH + GRID_WIDTH/2.0f, j * GRID_HEIGHT + GRID_HEIGHT/2.0f));
                }
            }
            float scr_width = float(SCR_WIDTH);
            float scr_height = float(SCR_HEIGHT);
            _board_sprite = Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(scr_width, scr_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1333.0f, 1333.0f));
            _is_initialized = true;
        }
        void init(unsigned int size)
        {
            _tiles = new Tile*[size];
            for(unsigned int i = 0; i < size; i++)
            {
                _tiles[i] = new Tile[size];
                for(unsigned int j = 0; j < size; j++)
                {
                    _tiles[i][j].setPosition(glm::vec2(i * GRID_WIDTH + GRID_WIDTH/2.0f, j * GRID_HEIGHT + GRID_HEIGHT/2.0f));
                }
            }
            float scr_width = float(SCR_WIDTH);
            float scr_height = float(SCR_HEIGHT);
            _board_sprite = Sprite(glm::vec2(0.0f, 0.0f), glm::vec2(scr_width, scr_height), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1333.0f, 1333.0f));
            _size = size;
            _is_initialized = true;
        }
        ~Board()
        {
            if(_is_initialized)
            {
                for(unsigned int i = 0; i < _size; i++)
                {
                    delete[] _tiles[i];
                }
                delete[] _tiles;
            }
        }
        void draw(SpriteRenderer& spriteRenderer)
        {
           
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    _tiles[i][j].draw(spriteRenderer);
                }
            }
        }
        void drawGrid(GeometryRenderer& geometryRenderer)
        {
            float board_size = float(_size);
            float grid_width = float(GRID_WIDTH);
           
            for(float x = 1.0; x <= board_size; x++)
            {
               geometryRenderer.drawLine(glm::vec2(grid_width*x, grid_width), glm::vec2(grid_width*x, grid_width*board_size), 2.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            }
            for(float y = 1.0; y <= board_size; y++)
            {
               geometryRenderer.drawLine(glm::vec2(grid_width, grid_width*y), glm::vec2(grid_width*board_size, grid_width*y), 2.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            }
            
        }

        void drawDots(SpriteRenderer & spriteRenderer)
        {
            float board_size = float(_size);
            float grid_width = float(GRID_WIDTH);
            float dot_width = 0.2f * grid_width;
            Sprite dotSprite(glm::vec2(grid_width*4.0f-dot_width/2.0f, grid_width*4.0f-dot_width/2.0f), glm::vec2(dot_width, dot_width), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1333.0f, 1333.0f), glm::vec2(STONE_TEXTURE_WIDTH, STONE_TEXTURE_WIDTH));
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*4.0f-dot_width/2.0f, grid_width*(board_size-3.0f)-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*(board_size-3.0f)-dot_width/2.0f, grid_width*4.0f-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*(board_size-3.0f)-dot_width/2.0f, grid_width*(board_size-3.0f)-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*(board_size+1.0f)/2.0f-dot_width/2.0f, grid_width*(board_size+1.0f)/2.0f-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*(board_size+1.0f)/2.0f-dot_width/2.0f, grid_width*(board_size-3.0f)-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*(board_size-3.0f)-dot_width/2.0f, grid_width*(board_size+1.0f)/2.0f-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*(board_size+1.0f)/2.0f-dot_width/2.0f, grid_width*4.0f-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            dotSprite.position = glm::vec2(grid_width*4.0f-dot_width/2.0f, grid_width*(board_size+1.0f)/2.0f-dot_width/2.0f);
            spriteRenderer.DrawSprite(dotSprite);
            
        }

        void drawBoard(SpriteRenderer & spriteRenderer)
        {
            spriteRenderer.DrawSprite(_board_sprite);
        }

        void setTile(unsigned int x, unsigned int y, TileType type)
        {
            if(x < _size && y < _size && x >= 0 && y >= 0)
            {
                _tiles[x][y].setType(type);
            }
            else
            {
                std::cout << "Error: Tile out of bounds" << std::endl;
            }
        }

        void setTile(unsigned int x, unsigned int y, Tile tile)
        {
            if(x < _size && y < _size && x >= 0 && y >= 0)
            {
                _tiles[x][y] = tile;
            }
            else
            {
                std::cout << "Error: Tile out of bounds" << std::endl;
            }
        }
        Tile getTile(unsigned int x, unsigned int y)
        {
            if(x < _size && y < _size && x >= 0 && y >= 0)
            {
                return _tiles[x][y];
            }
            else
            {
                std::cout << "Error: Tile out of bounds" << std::endl;
                 // Return a default tile if out of bounds
            }
            return _tiles[0][0]; // Return a default tile if out of bounds
        }
        void setChecked(unsigned int x, unsigned int y, bool checked)
        {
            if(x < _size && y < _size && x >= 0 && y >= 0)
            {
                _tiles[x][y].setChecked(checked);
            }
        }
        void setCaptured(unsigned int x, unsigned int y, bool captured)
        {
            if(x < _size && y < _size && x >= 0 && y >= 0)
            {
                _tiles[x][y].setCaptured(captured);
            }
        }
        bool isWhiteGroupCaptured(unsigned int x, unsigned int y)
        // This function checks if a group of stones is captured
        {
          if(x < 0 || x >= _size || y < 0 || y >= _size)
            return true;
          Tile &current_tile = _tiles[x][y];
          if(current_tile.getType() == TileType::EMPTY)
            return false;
          if(current_tile.getType() == TileType::BLACK)
            return true;
          if(current_tile.isChecked())
            return true;
          current_tile.setChecked(true);
          bool captured = true;
          
          captured = captured & isWhiteGroupCaptured(x, y-1);
          captured = captured & isWhiteGroupCaptured(x, y+1);
          captured = captured & isWhiteGroupCaptured(x-1, y);
          captured = captured & isWhiteGroupCaptured(x+1, y);
          //bool captured = captured_up & captured_down & captured_left & captured_right;
          //current_tile.setChecked(true);
          current_tile.setCaptured(captured);
          return captured;
        }
        bool isBlackGroupCaptured(unsigned int x, unsigned int y)
        // This function checks if a group of stones is captured
        {
          if(x < 0 || x >= _size || y < 0 || y >= _size)
            return true;
          Tile &current_tile = _tiles[x][y];
          //std::cout << "Checking tile (" << x << ", " << y << ")" << std::endl;
          //std::cout << "Tile type: " << int(current_tile.getType()) << std::endl;
          if(current_tile.getType() == TileType::EMPTY)
            return false;
          if(current_tile.getType() == TileType::WHITE)
            return true;
          if(current_tile.isChecked())
            return true;
          current_tile.setChecked(true);
          bool captured = true;
          captured = captured & isBlackGroupCaptured(x, y-1);
          captured = captured & isBlackGroupCaptured(x, y+1);
          captured = captured & isBlackGroupCaptured(x-1, y);
          captured = captured & isBlackGroupCaptured(x+1, y);
          // bool captured = captured_up & captured_down & captured_left & captured_right;
          current_tile.setCaptured(captured);
          //std::cout << "Captured: " << captured << std::endl;
          return captured;
        }
        int checkCapturedGroups(TileType color)
        {
            int captured_stones = 0;
            bool captured = false;
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    switch(color)
                    {
                       case TileType::WHITE:
                            if(_tiles[i][j].getType() == TileType::WHITE && isWhiteGroupCaptured(i, j))
                            {
                                captured_stones += clearCaptured();
                               
                            }
                            break;
                        case TileType::BLACK:
                            if(_tiles[i][j].getType() == TileType::BLACK && isBlackGroupCaptured(i, j))
                            {
                                captured_stones +=clearCaptured();
                              
                            }
                            break;
                        default:
                            std::cout << "Error: Invalid color" << std::endl;
                            break;
                    }
                    resetCaptured();
                    resetChecked();
                   
                }
               
            }
            
            return captured_stones;
        }
        unsigned int getSize() const
        {
            return _size;
        }
        void resetChecked()
        {
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    _tiles[i][j].setChecked(false);
                }
            }
        }
        void resetCaptured()
        {
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    _tiles[i][j].setCaptured(false);
                }
            }
        }
        int clearCaptured()
        {
            int captured_stones = 0;
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    if(_tiles[i][j].isCaptured())
                    {
                        _tiles[i][j].setType(TileType::EMPTY);
                        _tiles[i][j].setCaptured(false);
                        captured_stones++;
                    }
                }
            }
            return captured_stones;
        }
        Tile ** getBoardState()
        {
            return _tiles;
        }
        void setBoardState(Tile ** tiles)
        {
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    _tiles[i][j] = tiles[i][j];
                }
            }
        }
        bool operator==(const Board& other) const
        {
            if(_size != other._size)
                return false;
            for(unsigned int i = 0; i < _size; i++)
            {
                for(unsigned int j = 0; j < _size; j++)
                {
                    if(_tiles[i][j].getType() != other._tiles[i][j].getType())
                        return false;
                }
            }
            return true;
        }
        bool operator!=(const Board& other) const
        {
            return !(*this == other);
        }
    private:
        unsigned int _size;
        Tile **_tiles;
        Sprite _board_sprite;
        bool _is_initialized;

};

