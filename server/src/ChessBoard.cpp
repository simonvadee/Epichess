#include "ChessBoard.hh"

Pos _dir[8] =
  {
    {0, -1},
    {1, -1},
    {1, 0},
    {1, 1},
    {0, 1},
    {-1, 1},
    {-1, 0},
    {-1, -1}
  };

Pos _kdir[8] =
  {
    {1, -2},
    {2, -1},
    {2, 1},
    {1, 2},
    {-1, 2},
    {-2, 1},
    {-2, -1},
    {-1, -2}
  };

ChessBoard::ChessBoard()
{
}

ChessBoard::~ChessBoard()
{
}

std::vector<Pos>&	ChessBoard::findPossibleMoves(Pos& pos, int turn)
{
  char			tmp = (_map[pos.y][pos.x] & 0x0f);

  _turn = turn;
  _possibleMoves.clear();
  switch (tmp)
    {
    case KING:
      findKingMoves(pos);
      break;
    case QUEEN:
      findQueenMoves(pos);
      break;
    case ROOK:
      findRookMoves(pos);
      break;
    case BISHOP:
      findBishopMoves(pos);
      break;
    case KNIGHT:
      findKnightMoves(pos);
      break;
    case PAWN:
      findPawnMoves(pos);
      break;
    }
  return _possibleMoves;
}

int			ChessBoard::evaluate()
{
  int			score = 0;
  PAWNS			pawn;
  Pos			pos;

  for (int i = 0; i < 8; ++i)
    {
      for (int j = 0; j < 8; ++j)
	{
	  if (_map[i][j] != EMPTY)
	    {
	      pawn = static_cast<PAWNS>(_map[i][j] & 0x0f);
	      if (pawn == PAWN)
		{
		  pos.x = i;
		  pos.y = j;
		  score += isIsolatedBlockedDouble(pos, ((_map[i][j] & 0xf0) == MYSELF ? 1 : -1));
		}
	      else
		score += evalMainPawns(pawn, ((_map[i][j] & 0xf0) == MYSELF ? 1 : -1));
	    }
	}
    }
  return score + (getMobility(MYSELF) - getMobility(HUMAN)) / 10;
}

int			ChessBoard::getMobility(int turn)
{
  int			ret = 0;
  std::vector<Pos>	pawns;
  std::vector<Pos>	moves;
  
  pawns = findPossibleActors(turn);
  for (int i = 0; i < pawns.size(); ++i)
    ret += findPossibleMoves(pawns[i], turn).size();
  return ret;
}

int			ChessBoard::evalMainPawns(PAWNS pawn, int fact)
{
  switch (pawn)
    {
    case KING:
      return 200 * fact;
      break;
    case QUEEN:
      return 9 * fact;
      break;
    case ROOK:
      return 5 * fact;
      break;
    case BISHOP:
      return 3 * fact;
      break;
    case KNIGHT:
      return 3 * fact;
      break;
    }
}

int			ChessBoard::isIsolatedBlockedDouble(Pos& pos, int fact)
{
  int			res = 1;
  Pos			move;

  move.x = pos.x;
  move.y = pos.y + ((_turn & 0xf0) == MYSELF ? 1 : -1);
  if (isInMap(move) && _map[move.y][move.x] != EMPTY)
    res -= 0.5;
  if (isInMap(move) && (_map[pos.y][pos.x] & 0xf0) == HUMAN && pos.y == 7 || (_map[pos.y][pos.x] & 0xf0) == MYSELF && pos.y == 1)
    res -= 0.5;
  for (int i = 0; i < 8; ++i)
    {
      move.x = pos.x + _dir[i].x;
      move.y = pos.y + _dir[i].y;
      if (isInMap(move) && (_map[pos.y][pos.x] & 0xf0) == MYSELF)
	return res * fact;
    }
  res -= 0.5;
  return res * fact;
}

void			ChessBoard::findKingMoves(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 8; ++i)
    {
      move.x = pos.x + _dir[i].x;
      move.y = pos.y + _dir[i].y;
      if (isInMap(move) && ((_map[move.y][move.x] & 0xf0) != _turn || _map[move.y][move.x] == EMPTY) && !isChessMate(move))
	_possibleMoves.push_back(move);
    }
}

bool			ChessBoard::isChessMate(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 8; ++i)
    {
      for (int j = 1; j < 8; ++j)
	{
	  move.x = pos.x + _dir[i].x * j;
	  move.y = pos.y + _dir[i].y * j;
	  if (!isInMap(move) || ((_map[move.y][move.x] & 0xf0) == _turn))
	    break;
	  if (_map[move.y][move.x] != EMPTY && isThreatning(static_cast<PAWNS>(_map[move.y][move.x] & 0x0f), i, j))
	    return true;
	}
    }
  for (int i = 0; i < 8; ++i)
    {
      move.x = pos.x + _kdir[i].x;
      move.y = pos.y + _kdir[i].y;
      if (isInMap(move) && _map[move.y][move.x] != EMPTY && (_map[move.y][move.x] & 0xf0) == _turn && _map[move.y][move.x] == KNIGHT)
	return true;
    }
  return false;
}

bool			ChessBoard::isThreatning(PAWNS type, int dir, int dist)
{
  switch (type)
    {
    case KING:
      return (dist == 1 ? true : false);
      break;
    case QUEEN:
      return true;
      break;
    case ROOK:
      return (dir % 2 == 0 ? true : false);
      break;
    case BISHOP:
      return (dir % 2 == 1 ? true : false);
      break;
    case PAWN:
      return (dist == 1 && dir % 2 == 1 ? true : false);
      break;
    default:
      return false;
      break;
    }
}

void			ChessBoard::findQueenMoves(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 8; ++i)
    {
      for (int j = 1; j < 8; ++j)
	{
	  move.x = pos.x + _dir[i].x * j;
	  move.y = pos.y + _dir[i].y * j;

	  if (!isInMap(move) || (_map[move.y][move.x] & 0xf0) == _turn)
	    break;
	  _possibleMoves.push_back(move);
	  if (_map[move.y][move.x] != EMPTY)
	    break;
	}
    }
}

void			ChessBoard::findRookMoves(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 8; ++i)
    {
      if (i % 2 == 0)
	{
	  for (int j = 1; j < 8; ++j)
	    {
	      move.x = pos.x + _dir[i].x * j;
	      move.y = pos.y + _dir[i].y * j;
	      if (!isInMap(move) || (_map[move.y][move.x] & 0xf0) == _turn)
		break;
	      _possibleMoves.push_back(move);
	      if (_map[move.y][move.x] != EMPTY)
		break;
	    }
	}
    }
}

void			ChessBoard::findBishopMoves(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 8; ++i)
    {
      if (i % 2 == 1)
	{
	  for (int j = 1; j < 8; ++j)
	    {
	      move.x = pos.x + _dir[i].x * j;
	      move.y = pos.y + _dir[i].y * j;
	      if (!isInMap(move) || (_map[move.y][move.x] & 0xf0) == _turn)
		break;
	      _possibleMoves.push_back(move);
	      if (_map[move.y][move.x] != EMPTY)
		break;
	    }
	}
    }
}

void			ChessBoard::findKnightMoves(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 8; ++i)
    {
      move.x = pos.x + _kdir[i].x;
      move.y = pos.y + _kdir[i].y;
      if (!isInMap(move))
	continue;
      if ((_map[move.y][move.x] & 0xf0) != _turn || _map[move.y][move.x] == EMPTY)
	_possibleMoves.push_back(move);
    }
}

void			ChessBoard::findPawnMoves(Pos& pos)
{
  Pos			move;

  for (int i = 0; i < 3; ++i)
    {
      move.x = pos.x + i - 1;
      move.y = pos.y + ((_turn & 0xf0) == MYSELF ? 1 : -1);
      if (isInMap(move) && ((_map[move.y][move.x] == EMPTY && i == 1) || (i != 1 && _map[move.y][move.x] != EMPTY && (_map[move.y][move.x] & 0xf0) != _turn)))
	_possibleMoves.push_back(move);
    }
}

bool			ChessBoard::isInMap(Pos& pos)
{
  return (pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8);
}
