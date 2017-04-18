using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class Board
{
    public CellEntry[] m_Values;    //棋子的摆放
    int m_Score;                    //对当前棋局的评分
    public int m_n;                 //棋盘的规模
	public bool m_isXTurn ;         //当前是否轮到x下棋
    public static int[,] lines;     //常量数组，表示行，列，对角线，方便评分
	public bool GameOver            //游戏是否结束
	{
		get;
		private set;
	}
    public Board(CellEntry[] values, bool isXTurn, int n)   //构造函数，赋初值
	{
        m_isXTurn = isXTurn;
		m_Values = values;
        m_n = n;
		ComputeScore();
	}
	
    public static void Initlines(int m_n)           //初始化常量数组，表示行
    {
        lines = new int[m_n * 2 + 2, m_n];
        int k = 0;
        for (int i = 0; i < m_n; i++)
        {
            for (int j = 0; j < m_n; j++)
                lines[k, j] = j + i * m_n;
            k++;
        }
        for (int i = 0; i < m_n; i++)
        {
            for (int j = 0; j < m_n; j++)
                lines[k, j] = i + (j) * m_n;
            k++;
        }
        for (int j = 0; j < m_n; j++)
            lines[k, j] = j * m_n + j;
        k++;
        for (int j = m_n; j > 0; j--)
            lines[k, m_n - j] = (m_n - j) * m_n + j - 1;
    }
	
	public Board GetChildAtPosition(int x, int y)   //在x,y上放置棋子
	{
        int i = x + y * m_n;
		CellEntry[] newValues = (CellEntry[])m_Values.Clone();
		
		if (m_Values[i] != CellEntry.Empty) 
			throw new Exception(string.Format("invalid index [{0},{1}] is taken by {2}",x, y, m_Values[i]));
		
		newValues[i] = m_isXTurn?CellEntry.PlayerX:CellEntry.PlayerO;
        return new Board(newValues, !m_isXTurn, m_n);
	}
	
	public bool IsTerminalNode()
	{
		if (GameOver)
			return true;
		//if all entries are set, then it is a leaf node
		foreach (CellEntry v in m_Values)
		{
			if (v == CellEntry.Empty)
				return false;
		}
		return true;
	}
	
	public IEnumerable<Board> GetChildren()     //返回所有的孩子节点
	{
		for (int i = 0; i < m_Values.Length; i++)
		{
			if (m_Values[i] == CellEntry.Empty)
			{
				CellEntry[] newValues = (CellEntry[])m_Values.Clone();
				newValues[i] = m_isXTurn ? CellEntry.PlayerX : CellEntry.PlayerO;
                yield return new Board(newValues, !m_isXTurn, m_n);
			}
		}
	}
	
	//http://en.wikipedia.org/wiki/Alpha-beta_pruning
	public int MiniMax(int depth, int alpha, int beta, out Board maxChild)
	{
		maxChild = null;
		if (depth == 0 || IsTerminalNode())
		{
			//When it is turn for PlayO, we need to find the minimum score.
			return m_isXTurn ? m_Score : -m_Score;
		}
		foreach (Board cur in GetChildren())
		{
			Board temp;
			int score = -cur.MiniMax(depth - 1, -beta, -alpha, out temp);
			if (alpha < score)
			{
				alpha = score;
				maxChild = cur;
				if (alpha >= beta)
					break;
			}
		}
		return alpha;
	}
	
	public Board FindNextMove(int depth, bool isSmart)
	{
		Board ret = null;
        int emptyCell = 0;
        foreach (CellEntry v in m_Values)
        {
            if (v == CellEntry.Empty)
                emptyCell++;
        }
        if (!isSmart && emptyCell >= m_n * m_n - (m_n - 2) * (m_n - 2))         //一般水平的AI，前几步随机下
            RandomMove(out ret);
        else
            MiniMax(depth, int.MinValue + 1, int.MaxValue - 1, out ret);
		return ret;
	}
    void RandomMove(out Board ret)
    {
        int i;
        do
        {
            i = new System.Random().Next(m_n * m_n - 1);
        } while (m_Values[i] != CellEntry.Empty);
        ret = GetChildAtPosition(i % m_n, i / m_n);
    }
	void ComputeScore()                             //对叶子节点的评估函数
	{
		int ret = 0;
        for (int i = lines.GetLowerBound(0); i <= lines.GetUpperBound(0); i++)
        {
            CellEntry[] temp = new CellEntry[m_n];
            for (int j = 0; j < m_n; j++)
            {
                temp[j] = m_Values[lines[i, j]];
            }
            ret += GetScoreForOneLine(temp);
        }
		m_Score = ret;
	}
    int GetScoreForOneLine(CellEntry[] values)      //对某一行评估
    {
        int countX = 0, countO = 0;
        foreach (CellEntry v in values)
        {
            if (v == CellEntry.PlayerX)
                countX++;
            else if (v == CellEntry.PlayerO)
                countO++;
        }

        if (countO == m_n || countX == m_n)
        {
            GameOver = true;
        }
        /*
        下X的一方最大利益为+∞
        •己方在同一条线上有三子，记+100分;
        •己方在同一条线上有二子，记+10分;
        •己方在同一条线上有一子，记+1分;
        •同理，对方在同一条线上有三子，二子，一子，分别记-100, -10, -1分;
        •其余情况记0分。（在线上没有任一子或者同时有两方的子数）
        */
        int advantage = 1;
        if (countO == 0)
        {
            if (m_isXTurn)
                advantage = m_n;
            return (int)System.Math.Pow(10, countX) * advantage;
        }
        else if (countX == 0)
        {
            if (!m_isXTurn)
                advantage = m_n;
            return -(int)System.Math.Pow(10, countO) * advantage;
        }
        return 0;
    }
    public int GetResult()          //获取最后结果，0-平，1-X赢，2-O赢
    {
        int countX, countO; int ret = 0;
        for (int i = lines.GetLowerBound(0); i <= lines.GetUpperBound(0); i++)
        {
            countX = countO = 0;
            CellEntry[] temp = new CellEntry[m_n];
            for (int j = 0; j < m_n; j++)
            {
                temp[j] = m_Values[lines[i, j]];
            }
            foreach (CellEntry v in temp)
            {
                if (v == CellEntry.PlayerX)
                    countX++;
                else if (v == CellEntry.PlayerO)
                    countO++;
            }
            if (countX == m_n)
            {
                ret = 1;
                break;
            }
            if (countO == m_n)
            {
                ret = 2;
                break;
            }
        }
        return ret;
    }
}

