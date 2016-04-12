#include "Board.hh"

Board::Board()
{
}

Board::~Board()
{
}

std::vector<Pos>&		Board::findPossibleActors(int turn)
{
  Pos			tmp;

  _turn = turn;
  _pawns.clear();
  for (int i = 0; i < 8; ++i)
    {
      for (int j = 0; j < 8; ++j)
	{
	  if ((_map[i][j] & 0xf0) == turn)
	    {
	      tmp.y = i;
	      tmp.x = j;
	      _pawns.push_back(tmp);
	    }
	}
    }
  return _pawns;
}

void			Board::setMap(char **map)
{
  _map = map;
}
