package ChineseChess;

public interface GameObserver {

    public void newPosition(String fen);

    public void newMove(String move);
    
    public void reMove(String move,char type);

    public void mate(short which, char color);

    public void setColor(char color);
}
