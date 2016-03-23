#ifndef BRAINCORE_HH_
# define BRAINCORE_HH_

# include <vector>
# include <cstring>

# include "ChessBoard.hh"

# define MAXINT 2147483647
# define MAXDEPTH 4

class BrainCore
{
public:
  BrainCore();
  ~BrainCore();

private:
  ChessBoard*			_board;
  char**			_map;
  std::vector<Pos>		_pawns;
  Pos				_origin;
  Pos				_move;
  std::vector<int>		_decode;
  std::vector<std::string>	_encode;
  Pos				_actual;

public:
  void				fillMap(char pawn);
  char*				genFenMap();
  char*				genMoveCode();
  void				generate(std::string const&);
  char*				fightBack(std::string const& fen);
  int				minMax(int depth, char **map, bool turn);
  void				displayMap();
};

#endif
