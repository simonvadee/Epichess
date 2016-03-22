#include "BrainCore.hh"

#include <iostream>
#include <string>

BrainCore::BrainCore()
  : _board(new ChessBoard())
{
  _map = new char*[8];

  for (int j = 0; j < 8; ++j)
    _map[j] = new char[8];

  _decode.assign(200, 200);
  _encode.assign(200, std::string("aa"));
  _decode['r'] = ROOK;
  _decode['n'] = KNIGHT;
  _decode['b'] = BISHOP;
  _decode['p'] = PAWN;
  _decode['k'] = KING;
  _decode['q'] = QUEEN;

  _encode[ROOK] = std::string("r");
  _encode[KNIGHT] = std::string("n");
  _encode[BISHOP] = std::string("b");
  _encode[PAWN] = std::string("p");
  _encode[KING] = std::string("k");
  _encode[QUEEN] = std::string("q");
}

BrainCore::~BrainCore()
{
}

void			BrainCore::generate(char *coded)
{
  int			i = 0;

  _actual.x = 0;
  _actual.y = 0;
  while (coded[i])
    {
      if (coded[i] != '/')
	{
	  std::cout << coded[i] - 48 << std::endl;
	  if (coded[i] >= '1' && coded[i] <= '9')
	    for (int j = 0; j < coded[i] - 48; ++j)
	      fillMap(0);
	  else
	    fillMap((coded[i] >= 'a' ? (_decode[coded[i]]) + 0xe0 : (_decode[coded[i] + 32]) + 0xf0));
	}
      ++i;
    }
}

void			BrainCore::displayMap()
{
  for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; ++j)
	{
	  std::cout << (int)(_map[i][j] & 0x0f);
	}
      std::cout << std::endl;
    }
}

void			BrainCore::fillMap(char pawn)
{
  _map[_actual.y][_actual.x] = pawn;
  if (++_actual.x == 8)
    {
      _actual.x = 0;
      ++_actual.y;
    }
}

void			BrainCore::fenMap()
{
  int			count = 0;
  std::string		str;

  for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; ++j)
	{
	  if (_map[i][j] == EMPTY)
	    ++count;
	  else
	    {
	      if (count != 0)
		{
		  str += std::to_string(count);
		  count = 0;
		}
	      if ((_map[i][j] & 0xf0) == MYSELF)
		str += _encode[(int)(_map[i][j] & 0x0f)];
	      else
		str += _encode[(int)(_map[i][j] & 0x0f)][0] - 32;
	    }
	  if (_map[i][j] == EMPTY && j == 7)
	    {
	      str += std::to_string(count);
	      count = 0;
	    }
	}
      if (i != 8)
	str += "/";
    }
  std::cout << str << std::endl;
}

void			BrainCore::fightBack(char *coded)
{
  generate(coded);
  minMax(0, _map, true);
  std::cout << "move : x " << static_cast<int>(_origin.x) << " , y " << static_cast<int>(_origin.y) << std::endl;
  std::cout << "move : x " << static_cast<int>(_move.x) << " , y " << static_cast<int>(_move.y) << std::endl;
}

int			BrainCore::minMax(int depth, char **map, bool turn)
{
  Pos			move, origin;
  std::vector<Pos>	possibleMoves;
  std::vector<Pos>	pawns;
  int			ret, best = (turn ? -MAXINT : MAXINT);

  _board->setMap(map);

  if (depth == MAXDEPTH)
    return _board->evaluate();

  pawns = _board->findPossibleActors(turn ? MYSELF : HUMAN);
  // std::cout << "DEPTH : " << depth << "  , possible pawn : " << pawns.size() << std::endl;
  for (int i = 0; i < pawns.size(); ++i)
    {
      possibleMoves = _board->findPossibleMoves(pawns[i]);
      // std::cout << "possible Moves : " << possibleMoves.size() << std::endl;
      for (int j = 0; j < possibleMoves.size(); ++j)
	{
	  map[possibleMoves[j].y][possibleMoves[j].x] = map[pawns[i].y][pawns[i].x];
	  map[pawns[i].y][pawns[i].x] = 0;
	  ret = minMax(depth + 1, map, !turn);
	  if ((turn && ret > best) || (!turn && ret < best))
	    {
	      best = ret;
	      move = possibleMoves[j];
	      origin = pawns[i];
	    }
	  map[pawns[i].y][pawns[i].x] = map[possibleMoves[j].y][possibleMoves[j].x];
	  map[possibleMoves[j].y][possibleMoves[j].x] = 0;
	}
    }
  if (depth == 0)
    {
      displayMap();
      _origin = origin;
      _move = move;
      _map[_move.y][_move.x] = map[_origin.y][_origin.x];
      _map[_origin.y][_origin.x] = 0;
  fenMap();
    }
  return (best);
}