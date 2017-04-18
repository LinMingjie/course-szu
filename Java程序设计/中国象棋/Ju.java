package ChineseChess;

import java.util.Vector;

public class Ju extends Piece // 車
{

    static Ju ju ;

    private Ju() {
    }

    public static Vector<Destination> possibleDestinations(char[][] board, SquareCoord origin, boolean checkCheck) {
        if (ju == null) {
            ju = new Ju();
        }
        return ju._possibleDestinations(board, origin, checkCheck);
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

        int i;
        // 上�
        for(i = x + 1; i < 10 && board[i][y] == ' '; i++){
            checkMove(moves, i, y, 'R', 'r', tempBoard, board, checkCheck);
        }
        if(i < 10){checkMove(moves, i, y, 'R', 'r', tempBoard, board, checkCheck);}
        // 下�
        for(i = x - 1; i > -1 && board[i][y] == ' '; i--){
            checkMove(moves, i, y, 'R', 'r', tempBoard, board, checkCheck);
        }
        if(i>-1){checkMove(moves, i, y, 'R', 'r', tempBoard, board, checkCheck);}
        //�
        for(i = y - 1; i > -1 && board[x][i] == ' '; i--){
            checkMove(moves, x, i, 'R', 'r', tempBoard, board, checkCheck);
        }
        if(i>-1){checkMove(moves, x, i, 'R', 'r', tempBoard, board, checkCheck);}
        // �
        for(i = y + 1; i < 9 && board[x][i] == ' '; i++){
            checkMove(moves, x, i, 'R', 'r', tempBoard, board, checkCheck);
        }
        if(i<9){checkMove(moves, x, i, 'R', 'r', tempBoard, board, checkCheck);}
        return moves;
    }

    private Vector<Destination> blue(char[][] board, SquareCoord origin, boolean checkCheck) {
        Vector<Destination> moves = new Vector<Destination>();
        int x = origin.getRow();
        int y = origin.getFile();
        char[][] tempBoard = fillTempBoard(board);
        tempBoard[x][y] = ' ';

                int i;
        // 上�
        for(i = x + 1; i < 10 && board[i][y] == ' '; i++){
            checkMove(moves, i, y, 'r', 'b', tempBoard, board, checkCheck);
        }
        if(i < 10){checkMove(moves, i, y, 'r', 'b', tempBoard, board, checkCheck);}
        // 下�
        for(i = x - 1; i > -1 && board[i][y] == ' '; i--){
            checkMove(moves, i, y, 'r', 'b', tempBoard, board, checkCheck);
        }
        if(i>-1){checkMove(moves, i, y, 'r', 'b', tempBoard, board, checkCheck);}
        //�
        for(i = y - 1; i > -1 && board[x][i] == ' '; i--){
            checkMove(moves, x, i, 'r', 'b', tempBoard, board, checkCheck);
        }
        if(i>-1){checkMove(moves, x, i, 'r', 'b', tempBoard, board, checkCheck);}
        // �
        for(i = y + 1; i < 9 && board[x][i] == ' '; i++){
            checkMove(moves, x, i, 'r', 'b', tempBoard, board, checkCheck);
        }
        if(i<9){checkMove(moves, x, i, 'r', 'b', tempBoard, board, checkCheck);}
        return moves;
    }
}
