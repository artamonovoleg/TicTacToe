#include <iostream>
#include <array>
#include <vector>
#include <limits>

static constexpr int BOARD_SIDE = 3;
using Board = std::array<char, BOARD_SIDE * BOARD_SIDE>;
char AI_PLAYER = 'x';
char HU_PLAYER = 'o';
const char EMPTY_CELL = ' ';

bool CheckRows(const Board& board)
{
    for (int col = 0; col < BOARD_SIDE; ++col)
    {
        if (board[0 + col * BOARD_SIDE] == board[1 + col * BOARD_SIDE] &&
            board[1 + col * BOARD_SIDE] == board[2 + col * BOARD_SIDE] &&
            board[col * BOARD_SIDE] != EMPTY_CELL)
        {
            return true;
        }
    }

    return false;
}

bool CheckCols(const Board& board)
{
    for (int row = 0; row < BOARD_SIDE; ++row)
    {
        if (board[row + 0 * BOARD_SIDE] == board[row + 1 * BOARD_SIDE] &&
            board[row + 1 * BOARD_SIDE] == board[row + 2 * BOARD_SIDE] &&
            board[row + BOARD_SIDE] != EMPTY_CELL)
        {
            return true;
        }
    }

    return false;
}

bool CheckDiagonals(const Board& board)
{
    if (board[0] == board[4] && board[4] == board[8] && board[0] != EMPTY_CELL)
        return true;
    if (board[2] == board[4] && board[4] == board[6] && board[2] != EMPTY_CELL)
        return true;
    return false;
}

bool GameOver(const Board& board)
{
    return CheckRows(board) || CheckCols(board) || CheckDiagonals(board);
}

std::vector<int> GetMoves(const Board& board)
{
    std::vector<int> result;
    for (int i = 0; i < BOARD_SIDE * BOARD_SIDE; ++i)
    {
        if (board[i] == EMPTY_CELL)
            result.push_back(i);
    }
    return result;
}

int Minimax(Board& board, int depth, bool maximize)
{
    if (GameOver(board))
    {
        if (maximize)
            return -1;
        else
            return 1;
    }
    else
    {
        if (depth < 9)
        {
            int bestScore;
            if (maximize)
            {
                bestScore = std::numeric_limits<int>::min();
                auto moves = GetMoves(board);

                for (auto move: moves)
                {
                    board[move] = AI_PLAYER;
                    bestScore = std::max(bestScore, Minimax(board, depth + 1, false));
                    board[move] = EMPTY_CELL;
                }
            }
            else
            {
                bestScore = std::numeric_limits<int>::max();
                auto moves = GetMoves(board);

                for (auto move: moves)
                {
                    board[move] = HU_PLAYER;
                    bestScore = std::min(bestScore, Minimax(board, depth + 1, true));
                    board[move] = EMPTY_CELL;
                }
            }
            return bestScore;
        }
        else
        {
            return 0;
        }
    }
}

int FindBestMove(Board& board, int movesCount)
{
    auto moves = GetMoves(board);
    int bestScore = std::numeric_limits<int>::min();
    int bestMove = 0;

    for (auto move : moves)
    {
        board[move] = AI_PLAYER;
        int score = Minimax(board, movesCount + 1, false);
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
        board[move] = EMPTY_CELL;
    }
    return bestMove;
}

std::ostream& operator<<(std::ostream& os, const Board& board)
{
    for (int i = 0; i < BOARD_SIDE; ++i)
    {
        os  << " " << board[0 + i * BOARD_SIDE]
            << " | " << board[1 + i * BOARD_SIDE]
            << " | " << board[2 + i * BOARD_SIDE] << " \n";

        if (i < BOARD_SIDE - 1)
            os << "-----------\n";
    }
    return os;
}

int main()
{
    Board board;
    board.fill(EMPTY_CELL);

    bool aiTurn;

    std::cout << "Choose player x or o (x moves first): ";
    std::cin >> HU_PLAYER;

    if (HU_PLAYER == 'o')
    {
        AI_PLAYER = 'x';
        aiTurn = true;
    }
    else
    if (HU_PLAYER == 'x')
    {
        AI_PLAYER = 'o';
        aiTurn = false;
    }

    std::cout << board << std::endl;

    int movesCount = 0;

    std::cout << "(0, 0) - left up (2, 2) - right down" << std::endl;

    while (!GameOver(board) && movesCount != BOARD_SIDE * BOARD_SIDE)
    {
        if (aiTurn)
        {
            board[FindBestMove(board, movesCount)] = AI_PLAYER;
            aiTurn = !aiTurn;
            movesCount++;
        }
        else
        {
            int x, y;
            std::cout << "Enter x y: ";
            std::cin >> x >> y;
            int move = x + y * BOARD_SIDE;
            if (board[move] != EMPTY_CELL)
            {
                std::cout << "This cell not empty" << std::endl;
            }
            else
            {
                board[x + y * BOARD_SIDE] = HU_PLAYER;
                aiTurn = !aiTurn;
                movesCount++;
            }
        }
        std::cout << board << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
