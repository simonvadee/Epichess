#include "Board.hh"

Board::Board()
{
}

Board::~Board()
{
}

std::vector<Pos>		Board::findPossibleActors(int turn)
{
  std::vector<Pos>	pawns;
  Pos			tmp;

  _turn = turn;
  pawns.clear();
  for (int i = 0; i < 8; ++i)
    {
      for (int j = 0; j < 8; ++j)
	{
	  if ((_map[i][j] & 0xf0) == turn)
	    {
	      // std::cout << (_map[i][j] & turn) << "pos.x : " << j << " pos.y : " << i << std::endl;
	      tmp.y = i;
	      tmp.x = j;
	      pawns.push_back(tmp);
	    }
	}
    }
  return pawns;
}

void			Board::setMap(char **map)
{
  _map = map;
}
