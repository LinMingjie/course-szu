package ChineseChess;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class ChineseChess {
    static ChineseChess app;
    JFrame mainFrame;
    ChineseChessCtrl control;

    private ChineseChess() {
        InitGlobalFont(new Font("Microsoft Yahei UI", Font.PLAIN, 14));
        mainFrame = new JFrame("中国象棋");
        control = new ChineseChessCtrl(mainFrame);
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("文件");
        JMenuItem item = new JMenuItem("新游戏");
        item.setActionCommand("NEW");
        item.addActionListener(control);
        menu.add(item);

        menu.addSeparator();
        
        item = new JMenuItem("退出");
        item.setActionCommand("EXIT");
        item.addActionListener(control);
        menu.add(item);

        menuBar.add(menu);
        mainFrame.setJMenuBar(menuBar);

        Game game = new Game();
        mainFrame.getContentPane().setLayout(new BorderLayout());   //GridBagLayout
        BoardView.defaultBoardView().setGame(game);

        //GridBagConstraints constrs = new GridBagConstraints();

        //mainFrame.add(BoardView.defaultBoardView(), constrs);
        mainFrame.add(BoardView.defaultBoardView(),BorderLayout.NORTH);
        JPanel panelLeft=new JPanel(new FlowLayout(FlowLayout.LEFT)); 
        JPanel panelRight=new JPanel(new FlowLayout(FlowLayout.RIGHT)); 
        JButton buttonHome=new JButton("<<");
        JButton buttonEnd=new JButton(">>");
        JButton buttonBack=new JButton("<");
        JButton buttonFront=new JButton(">");
        panelLeft.add(buttonHome);
        panelLeft.add(buttonBack);
        panelRight.add(buttonFront);
        panelRight.add(buttonEnd);
        mainFrame.add(panelLeft,BorderLayout.WEST);
        mainFrame.add(panelRight,BorderLayout.EAST);
        buttonHome.addActionListener(control);
        buttonBack.addActionListener(control);
        buttonEnd.addActionListener(control);
        buttonFront.addActionListener(control);
        
        /*
        constrs.fill = GridBagConstraints.BOTH;
        constrs.gridwidth = constrs.gridheight = GridBagConstraints.REMAINDER;
        constrs.weighty = 1.0;
        */
        mainFrame.setSize(303, 424);
        mainFrame.setLocationRelativeTo(mainFrame.getOwner());
        mainFrame.setResizable(false);

        mainFrame.addWindowListener(control);

        mainFrame.setAlwaysOnTop(true);
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    

    static public ChineseChess runningApplication() {
        if (app == null) {
            app = new ChineseChess();
        }
        return app;
    }

    private void start() {
        mainFrame.setVisible(true);
    }

    /**
     * 参考代码http://blog.csdn.net/chenxuejiakaren/article/details/7637731
     * 统一设置字体，父界面设置之后，所有由父界面进入的子界面都不需要再次设置字体
     */
    private static void InitGlobalFont(Font font) {
        javax.swing.plaf.FontUIResource fontRes = new javax.swing.plaf.FontUIResource(font);
        for (java.util.Enumeration<Object> keys = UIManager.getDefaults().keys(); keys.hasMoreElements();) {
            Object key = keys.nextElement();
            Object value = UIManager.get(key);
            if (value instanceof javax.swing.plaf.FontUIResource) {
                UIManager.put(key, fontRes);
            }
        }
    }

    public static void main(String[] args) {
        ChineseChess.runningApplication().start();
    }
}

class ChineseChessCtrl extends WindowAdapter implements ActionListener {
    JFrame mainFrame;
    public ChineseChessCtrl(JFrame frame) {
        mainFrame = frame;
    }

    public void actionPerformed(ActionEvent event) {
        if (event.getActionCommand().compareTo("NEW") == 0) {
            Game game = new Game();
            BoardView.defaultBoardView().setGame(game);
            BoardView.defaultBoardView().setColor('r');
        }
        if (event.getActionCommand().compareTo("EXIT") == 0) {
            System.exit(0);
        }
        if(event.getActionCommand().compareTo("<<")==0){
//            System.out.println("<<");
            while(!BoardView.defaultBoardView().getGame().back.isEmpty()){
                BoardView.defaultBoardView().getGame().backward();
            }
        }
        if(event.getActionCommand().compareTo("<")==0){
//            System.out.println("<");
            BoardView.defaultBoardView().getGame().backward();
        }
        if(event.getActionCommand().compareTo(">>")==0){
//            System.out.println(">>");
            while(!BoardView.defaultBoardView().getGame().front.isEmpty()){
                BoardView.defaultBoardView().getGame().forward();
            }
        }
        if(event.getActionCommand().compareTo(">")==0){
//            System.out.println(">");
            BoardView.defaultBoardView().getGame().forward();
        }
    }
}
