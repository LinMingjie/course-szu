package ChineseChess;

import java.util.Vector;

public class Xiang extends Piece // 鐮�
{

    static Xiang xiang ;

    private Xiang() {
    }

    public static Vector<Destination> possibleDestinations(char[][] board, SquareCoord origin, boolean checkCheck) {
        if (xiang == null) {
            xiang = new Xiang();
        }
        return xiang._possibleDestinations(board, origin, checkCheck);
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

        if (x+2 <5&&y+2<9&&tempBoard[x+1][y+1]==' ') checkMove(moves, x+2,y+2,'E','r', tempBoard, board, checkCheck);
    // 西北
        if (x-2 >-1&&y+2<9&&tempBoard[x-1][y+1]==' ') checkMove(moves, x-2,y+2,'E','r', tempBoard, board, checkCheck);
    // 西南
        if (x+2<5&&y-2 >-1&&tempBoard[x+1][y-1]==' ') checkMove(moves, x+2, y-2, 'E', 'r', tempBoard, board, checkCheck);
    // 东北
        if (x-2>-1&&y-2> -1&&tempBoard[x-1][y-1]==' ') checkMove(moves, x-2, y-2, 'E', 'r', tempBoard, board, checkCheck);

        return moves;
    }

    private Vector<Destination> blue(char[][] board, SquareCoord origin, boolean checkCheck) {
        Vector<Destination> moves = new Vector<Destination>();
        int x = origin.getRow();
        int y = origin.getFile();
        char[][] tempBoard = fillTempBoard(board);
        tempBoard[x][y] = ' ';

        if (x+2 <10&&y+2<9&&tempBoard[x+1][y+1]==' ') checkMove(moves, x+2,y+2,'e','b', tempBoard, board, checkCheck);
    // 西北
        if (x-2 >4&&y+2<9&&tempBoard[x-1][y+1]==' ') checkMove(moves, x-2,y+2,'e','b', tempBoard, board, checkCheck);
    // 西南
        if (x+2<10&&y-2>-1&&tempBoard[x+1][y-1]==' ') checkMove(moves, x+2, y-2, 'e', 'b', tempBoard, board, checkCheck);
    // 东北
        if (x-2>4&&y-2> -1&&tempBoard[x-1][y-1]==' ') checkMove(moves, x-2, y-2, 'e', 'b', tempBoard, board, checkCheck);
    // 东南
        return moves;
    }
}

