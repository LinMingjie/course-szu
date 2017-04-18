package ChineseChess;

import java.util.Vector;

public class Ma extends Piece // 马
{

    static Ma ma ;

    private Ma() {
    }

    public static Vector<Destination> possibleDestinations(char[][] board, SquareCoord origin, boolean checkCheck) {
        if (ma == null) {
            ma = new Ma();
        }
        return ma._possibleDestinations(board, origin, checkCheck);
    }

    private Vector<Destination> _possibleDestinations(char[][] board, SquareCoord origin, boolean checkCheck) {
        if (Character.isUpperCase(board[origin.getRow()][origin.getFile()])) {
            return red(board, origin, checkCheck);
        } else {
            return blue(board, origin, checkCheck);
        }
    }

    private Vector<Destination> red(char[][] board, SquareCoord origin, boolean checkCheck) {
        Vector<Destination> moves = new Vector<Destination>();
        int x = origin.getRow();
        int y = origin.getFile();
        char[][] tempBoard = fillTempBoard(board);
        tempBoard[x][y] = ' ';

        // 上
        if(x + 2 < 10 && board[x+1][y] == ' '){
            if(y - 1 > -1){
                checkMove(moves, x + 2, y - 1, 'H', 'r', tempBoard, board, checkCheck);
            }
            if(y + 1 < 9){
                checkMove(moves, x + 2, y + 1, 'H', 'r', tempBoard, board, checkCheck);
            }
        }
        // 下
        if(x - 2 > -1 && board[x-1][y] == ' '){
            if(y - 1 > -1){
                checkMove(moves, x - 2, y - 1, 'H', 'r', tempBoard, board, checkCheck);
            }
            if(y + 1 < 9){
                checkMove(moves, x - 2, y + 1, 'H', 'r', tempBoard, board, checkCheck);
            }
        }
        // 左
        if(y - 2 > -1 && board[x][y-1] == ' '){
            if(x - 1 > -1){
                checkMove(moves, x - 1, y - 2, 'H', 'r', tempBoard, board, checkCheck);
            }
            if(x + 1 < 10){
                checkMove(moves, x + 1, y - 2, 'H', 'r', tempBoard, board, checkCheck);
            }
        }
        // 右
        if(y + 2 < 9 && board[x][y+1] == ' '){
            if(x - 1 > -1){
                checkMove(moves, x - 1, y + 2, 'H', 'r', tempBoard, board, checkCheck);
            }
            if(x + 1 < 10){
                checkMove(moves, x + 1, y + 2, 'H', 'r', tempBoard, board, checkCheck);
            }
        }

        return moves;
    }

    private Vector<Destination> blue(char[][] board, SquareCoord origin, boolean checkCheck) {
        Vector<Destination> moves = new Vector<Destination>();
        int x = origin.getRow();
        int y = origin.getFile();
        char[][] tempBoard = fillTempBoard(board);
        tempBoard[x][y] = ' ';

        // 上
        if(x + 2 < 10 && board[x+1][y] == ' '){
            if(y - 1 > -1){
                checkMove(moves, x + 2, y - 1, 'h', 'b', tempBoard, board, checkCheck);
            }
            if(y + 1 < 9){
                checkMove(moves, x + 2, y + 1, 'h', 'b', tempBoard, board, checkCheck);
            }
        }
        // 下
        if(x - 2 > -1 && board[x-1][y] == ' '){
            if(y - 1 > -1){
                checkMove(moves, x - 2, y - 1, 'h', 'b', tempBoard, board, checkCheck);
            }
            if(y + 1 < 9){
                checkMove(moves, x - 2, y + 1, 'h', 'b', tempBoard, board, checkCheck);
            }
        }
        // 左
        if(y - 2 > -1 && board[x][y-1] == ' '){
            if(x - 1 > -1){
                checkMove(moves, x - 1, y - 2, 'h', 'b', tempBoard, board, checkCheck);
            }
            if(x + 1 < 10){
                checkMove(moves, x + 1, y - 2, 'h', 'b', tempBoard, board, checkCheck);
            }
        }
        // 右
        if(y + 2 < 9 && board[x][y+1] == ' '){
            if(x - 1 > -1){
                checkMove(moves, x - 1, y + 2, 'h', 'b', tempBoard, board, checkCheck);
            }
            if(x + 1 < 10){
                checkMove(moves, x + 1, y + 2, 'h', 'b', tempBoard, board, checkCheck);
            }
        }

        return moves;
    }
}
