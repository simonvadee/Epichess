#ifndef BRAINCORE_HH_
# define BRAINCORE_HH_

# include <vector>
# include "ChessBoard.hh"

# define MAXINT 2147483647
# define MAXDEPTH 1

class BrainCore
{
public:
  BrainCore();
  ~BrainCore();

private:
  ChessBoard*		_board;
  char**		_map;
  std::vector<Pos>	_pawns;
  Pos			_origin;
  Pos			_move;
  std::vector<int>	_decode;
  std::vector<std::string>	_encode;
  Pos			_actual;

public:
  void			fillMap(char pawn);
  void			fenMap();
  void			generate(char *coded);
  void			fightBack(char *map);
  int			minMax(int depth, char **map, bool turn);
  void			displayMap();
};

#endif
