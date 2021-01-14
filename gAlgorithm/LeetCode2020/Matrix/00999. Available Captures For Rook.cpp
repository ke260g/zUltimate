/*
 * @lc app=leetcode id=999 lang=cpp
 *
 * [999] Available Captures for Rook
 *
 * https://leetcode.com/problems/available-captures-for-rook/description/
 *
 * algorithms
 * Easy (66.76%)
 * Total Accepted:    35.6K
 * Total Submissions: 53.3K
 * Testcase Example:  '[[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","R",".",".",".","p"],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]'
 *
 * On an 8 x 8 chessboard, there is one white rook.  There also may be empty
 * squares, white bishops, and black pawns.  These are given as characters 'R',
 * '.', 'B', and 'p' respectively. Uppercase characters represent white pieces,
 * and lowercase characters represent black pieces.
 * 
 * The rook moves as in the rules of Chess: it chooses one of four cardinal
 * directions (north, east, west, and south), then moves in that direction
 * until it chooses to stop, reaches the edge of the board, or captures an
 * opposite colored pawn by moving to the same square it occupies.  Also, rooks
 * cannot move into the same square as other friendly bishops.
 * 
 * Return the number of pawns the rook can capture in one move.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * 
 * 
 * Input:
 * [[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","R",".",".",".","p"],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
 * Output: 3
 * Explanation: 
 * In this example the rook is able to capture all the pawns.
 * 
 * 
 * Example 2:
 * 
 * 
 * 
 * 
 * Input:
 * [[".",".",".",".",".",".",".","."],[".","p","p","p","p","p",".","."],[".","p","p","B","p","p",".","."],[".","p","B","R","B","p",".","."],[".","p","p","B","p","p",".","."],[".","p","p","p","p","p",".","."],[".",".",".",".",".",".",".","."],[".",".",".",".",".",".",".","."]]
 * Output: 0
 * Explanation: 
 * Bishops are blocking the rook to capture any pawn.
 * 
 * 
 * Example 3:
 * 
 * 
 * 
 * 
 * Input:
 * [[".",".",".",".",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".","p",".",".",".","."],["p","p",".","R",".","p","B","."],[".",".",".",".",".",".",".","."],[".",".",".","B",".",".",".","."],[".",".",".","p",".",".",".","."],[".",".",".",".",".",".",".","."]]
 * Output: 3
 * Explanation: 
 * The rook can capture the pawns at positions b5, d6 and f5.
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * board.length == board[i].length == 8
 * board[i][j] is either 'R', '.', 'B', or 'p'
 * There is exactly one cell with board[i][j] == 'R'
 * 
 * 
 */

class Solution {
public:
    int numRookCaptures(vector<vector<char>>& board) {
        // 1. 找到起始点
        // 2. 分别往4个方向遍历;
        // 3. 当遇到第一个非空点 ( != '.')
        //    如果是 'p' 那么 capture;
        //    无论是否是p; 终止该方向的遍历
        int x, y, ncap = 0;
        for (int i = 0; i < board.size(); ++i) {
            for (int j = 0; j < board[0].size(); ++j) {
                if (board[i][j] == 'R') {
                    x = i; y = j; break;
                }
            }
        }

        static int dx[4] = { 1, 0, -1, 0 };
        static int dy[4] = { 0, 1, 0, -1 };
        int dm[4] = {
            (int)board.size() - x, // 上
            (int)board.size() - y, // 右
            x,                     // 下
            y,                     // 左
        };
        for (int i = 0; i < 4; i++) { // 4个方向
            for (int j = 1; j < dm[i]; j++) { // 每个方向的步进数
                int v = board[x+dx[i]*j][y+dy[i]*j];
                if (v != '.') {
                    ncap += v == 'p';
                    break;
                }
            }
        }
        return ncap;
        return ncap;
    }
};
