using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using UnityEngine;

public class TicTacToe : MonoBehaviour
{
    public Texture imgCell, imgCellO, imgCellX;     //资源：棋盘格子的图片
    public Texture[] imgHelp = new Texture[5];
    public Texture[,] img;	        //控制棋盘的显示
    const int cell = 80;                       //格子的边长

    public int n;                   //游戏规模n
	public Game game;			//游戏实例
	public int depth;				//递归树深度
    public float wait;              //电脑下棋前的等待时间
    public string txt_wait;
	public string txt_btnPlay;
	public string [] player=new string[3];	//当前下棋者 1 - player 1, 2 - player 2
	public int curPlayer;
	public bool isX1;						//player1是否是X
    public bool[] isSmart = new bool[3];         //AI智力程度
    public string txt_gameState;            //游戏结果：赢或和
    bool showWindow;                        //显示Help窗口
    int cur;                                //Help窗口的当前图片

    // Use this for initialization
    void Start()    //构造函数
    {
        n = 3;
		img = new Texture[20, 20];
		txt_btnPlay = "Play";
		player[1] = "Human"; player[2] = "Computer";
        isX1 = true; isSmart[1] = isSmart[2] = false;
        game = FindObjectOfType<Game>();
        showWindow = false;
        wait = 1.5f;
        txt_wait = "";
        Init();     //初始化游戏场景
    }

    void Init()     //开始游戏前的初始化
    {
        RefreshBoard();
		curPlayer = 0;
        txt_gameState = "";
    }

	void Play()     //开始游戏
	{
        //初始化UI
        RefreshBoard();
        txt_gameState = "";
		//初始化游戏
        depth = 1;
        curPlayer = isX1 ? 1 : 2;			//下"X"的人先手
        game.BeginGame();
	}

    public void Over(bool resign = false)
    {
        curPlayer = 0;
        int result = game.Current.GetResult();
        if (result==1 && isX1 || result==2 && !isX1)
            txt_gameState = "Player 1 wins.";
        else if (result==1 && !isX1 || result==2 && isX1)
            txt_gameState = "Player 2 wins.";
        else
            txt_gameState = "Draw";
        txt_btnPlay = "Play";
    }

    public void RefreshBoard()
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                img[i, j] = imgCell;
            }
        }
    }

    void OnGUI()
    {
        if (showWindow)
            Screen.SetResolution(1105, 700, false);
        else
            Screen.SetResolution((n + 3) * cell, (n + 3) * cell, false);                  	//窗口大小
        GUI.Label(new Rect(20, 10, 80, 50), "Tic-Tac-Toe");     	//标题
        if (GUI.Button(new Rect(20, 40, 60, 30), txt_btnPlay))		//play或resign按钮
        {
			if(txt_btnPlay=="Play")
			{
				txt_btnPlay="Resign";
				Play();
			}
			else
			{
                txt_btnPlay = "Play";
                game.resign = true;
                Init();
			}
        }
		if(curPlayer!=2)
		{
			GUI.Label(new Rect(20, 85, 80, 50), "Player1:");
            if(txt_btnPlay=="Play") //判断游戏状态
            {   //还没开始游戏
                if (GUI.Button(new Rect(70, 85, 25, 25), isX1 ? "X" : "O"))	//选择下"X"或"O"
                {
                    if (curPlayer == 0)		//下棋时不能更改
                        isX1 = !isX1;
                }
                if (GUI.Button(new Rect(20, 115, 70, 25), player[1]))			//选择人或AI
			    {
				    if(curPlayer==0)
				    	player[1] = (player[1]=="Human") ? "Computer" : "Human";
			    }
                if (player[1] == "Computer")
                {
                    isSmart[1] = !GUI.Toggle(new Rect(20, 140, 70, 25), !isSmart[1], "Normal");
                    isSmart[1] = GUI.Toggle(new Rect(20, 160, 70, 25), isSmart[1], "Smart");
                }
            }
            else
            {   //游戏中
                GUI.Label(new Rect(70, 85, 25, 25), isX1 ? "X" : "O");
                GUI.Label(new Rect(20, 115, 80, 30), player[1]);
                if(player[1]=="Computer")
                    GUI.Label(new Rect(20, 140, 70, 25), !isSmart[1] ? "Normal" : "Smart");
            }
		}
        if(curPlayer!=1)
		{
			GUI.Label(new Rect(20, 190, 80, 50), "Player2:");
            if (txt_btnPlay == "Play") //判断游戏状态
            {   //还没开始游戏
                if (GUI.Button(new Rect(70, 190, 25, 25), isX1 ? "O" : "X"))	//选择下"X"或"O"
                {
                    if (curPlayer == 0)		//下棋时不能更改
                        isX1 = !isX1;
                }
                if (GUI.Button(new Rect(20, 220, 70, 25), player[2]))			//选择人或AI
                {
                    if (curPlayer == 0)
                        player[2] = (player[2] == "Human") ? "Computer" : "Human";
                }
                if (player[2] == "Computer")
                {
                    isSmart[2] = !GUI.Toggle(new Rect(20, 245, 70, 25), !isSmart[2], "Normal");
                    isSmart[2] = GUI.Toggle(new Rect(20, 265, 70, 25), isSmart[2], "Smart");
                }
            }
            else
            {   //游戏中
                GUI.Label(new Rect(70, 190, 25, 25), isX1 ? "O" : "X");
                GUI.Label(new Rect(20, 220, 80, 30), player[2]);
                if(player[2]=="Computer")
                    GUI.Label(new Rect(20, 245, 70, 25), !isSmart[2] ? "Normal" : "Smart");
            }
		}
        if (curPlayer == 0) 
        {
            if(GUI.Button(new Rect(20, 300, 40, 25), "n++"))
            {
                if (n < 20)
                {
                    n++;
                    Init();
                }

            }
            if (GUI.Button(new Rect(65, 300, 40, 25), "n--"))
            {
                if (n > 3)
                {
                    n--;
                    Init();
                }
            }
        }
        GUI.Label(new Rect(20, 335, 120, 80), txt_gameState);
        if (GUI.Button(new Rect(20, 370, 60, 25), "Help"))		//Help按钮
        {
            showWindow = true; cur = 0;
        }
        if(showWindow)
            GUI.Window(0, new Rect(20, 20, 1060, 655), HelpWindow, "Help");
        if(txt_btnPlay=="Play")
        {
            GUI.Label(new Rect(20, 400, 120, 25), "Thinking Time:");
            txt_wait = GUI.TextField(new Rect(20, 430, 80, 25), txt_wait, 5);
            float x = -1.0f;
            try{
                x = float.Parse(txt_wait);
            }
            catch (System.FormatException e){
                x = -1.0f;
            }
            if (x > 0 && x < 100)
                wait = x;
        }
        
        //绘制棋盘
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (GUI.Button(new Rect(140 + j * cell, 40 + i * cell, cell, cell), img[i, j])) 
                {

                    if (img[i, j] == imgCell && player[curPlayer] == "Human" && !game.Current.IsTerminalNode())
					{
						if((curPlayer==1 && isX1) || (curPlayer==2 && !isX1))	//当前游戏者是player1,下"X" 或 当前游戏者是player2,下"X"
						{
							img[i,j]=imgCellX;
						}
						else if((curPlayer==1 && !isX1) || (curPlayer==2 && isX1))	//当前游戏者是player1,下"O" 或 当前游戏者是player2,下"O"
						{
							img[i,j]=imgCellO;
						}
                        game.UserMove(j, i);    //i行j列，与xy坐标相反
						game.NextPlayer();
					}
                }
            }
        }
    }
    void HelpWindow(int id)
    {
        GUI.Label(new Rect(20, 20, 1029, 580),imgHelp[cur]);
        if(GUI.Button(new Rect(200,605,80,30),"Pre"))
        {
            if (cur > 0)
                cur--;
        }
        if (GUI.Button(new Rect(829, 605, 80, 30), "Next"))
        {
            if (cur < 4)
                cur++;
        }
        if (GUI.Button(new Rect(515, 605, 80, 30), "Close"))
        {
            showWindow = false;
        }
    }
}
    