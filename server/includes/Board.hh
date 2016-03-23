#ifndef BOARD_HH_
# define BOARD_HH_

# include <vector>
# include <iostream>

# define HUMAN 0xf0
# define MYSELF 0xe0

typedef struct
{
  int	x;
  int	y;
}	Pos;

typedef struct
{
  Pos	source;
  Pos	dest;
  int	weight;
}	Move;

class Board
{
public:
  Board();
  ~Board();

protected:
  char**		_map;
  int			_turn;
  std::vector<Pos>	_pawns;

public:
  std::vector<Pos>&	findPossibleActors(int turn);
  void			setMap(char **map);
};

#endif
