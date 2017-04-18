using System;
using System.Collections;
using System.Linq;
using System.Text;
using UnityEngine;

public enum CellEntry:byte
{
	Empty,
	PlayerX,
	PlayerO
}

public class Game : MonoBehaviour
{
    public bool resign;          //��ֹ��Ϸ
    TicTacToe t;                 //��������ã����ڷ������������

	public Board Current
	{
		get;
		private set;
	}
	
	public void Start()
	{
        t = FindObjectOfType<TicTacToe>(); 
	}
    public void BeginGame()
    {
        resign = false;
        CellEntry[] values = Enumerable.Repeat(CellEntry.Empty, t.n * t.n).ToArray();
        Board.Initlines(t.n);
        Current = new Board(values, true, t.n);
        if (t.player[t.curPlayer] == "Computer")
            ComputerMove();
    }
    public void NextPlayer()
    {
        if (resign)
        {
            t.RefreshBoard();
            return;
        }
        if(Current.IsTerminalNode())
        {
            t.Over();
            return;
        }
        t.curPlayer = (t.curPlayer == 1) ? 2 : 1;
        if (t.player[t.curPlayer] == "Computer")
            ComputerMove();
    }

    public void ComputerMove()
    {
        StartCoroutine(DoMove());
    }

    private IEnumerator DoMove()
    {
        yield return new WaitForSeconds(t.wait);
        Board next = Current.FindNextMove(t.depth, t.isSmart[t.curPlayer]);
        for (int i = 0; i < t.n; i++)
        {
            for (int j = 0; j < t.n; j++)
            {
                CellEntry v = next.m_Values[i + j * t.n], u = Current.m_Values[i + j * t.n];
                if (u != v)
                {
                    if ((t.curPlayer == 1 && t.isX1) || (t.curPlayer == 2 && !t.isX1))	//��ǰ��Ϸ����player1,��"X" �� ��ǰ��Ϸ����player2,��"X"
                        t.img[j, i] = t.imgCellX;
                    else if ((t.curPlayer == 1 && !t.isX1) || (t.curPlayer == 2 && t.isX1))	//��ǰ��Ϸ����player1,��"O" �� ��ǰ��Ϸ����player2,��"O"
                        t.img[j, i] = t.imgCellO;
                }
            }
        }
        if (next != null)
            Current = next;
        NextPlayer();
    }
	
    public void UserMove(int x,int y)
    {
        Current = Current.GetChildAtPosition(x, y);
    }
}