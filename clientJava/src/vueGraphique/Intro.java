package vueGraphique;

import modele.Carte;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class Intro extends JFrame implements ActionListener {

    public static final ImageIcon SOKOBAN = new ImageIcon("img/Sokoban.gif");

    private boolean commencer = false;

    private JPanel infoPanel;
    private JLabel sokoban;
    private JLabel infoJeu;
    private Action quitterAction;
    private Action commencerAction;

    /**Constructeur de la classe vue*/
    public Intro(){
        //Donne le titre via JFrame
        super("Sokoban");

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        //Les inits
        initActions();
        initArrierePlan();

        //Les indispensables
        pack();
        Dimension dimEcran = getToolkit().getScreenSize();
        setLocation((dimEcran.width - getWidth())/2, (dimEcran.height- getHeight())/2);
        setResizable(false);
        setVisible(true);
    }


    /**Méthode qui initialise les actions des boutons */
    private void initActions() {
        quitterAction = new AbstractAction("Quitter") {
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        };
        commencerAction = new AbstractAction("Commencer") {
            public void actionPerformed(ActionEvent e){
                commencer=true;
            }
        };
    }

    private void initArrierePlan(){
        infoPanel = new JPanel();
        infoPanel.setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        infoPanel.setBackground(Color.BLACK);

        sokoban = new JLabel(SOKOBAN, JLabel.CENTER);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 5;
        c.gridx = 0;
        c.gridy = 0;
        infoPanel.add(sokoban, c);

        infoJeu = new JLabel("Programmé en Java par Patrice Plouvin", JLabel.CENTER);
        infoJeu.setForeground(Color.WHITE);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 5;
        c.gridx = 0;
        c.gridy = 1;
        infoPanel.add(infoJeu, c);

        JButton quitterBtn = new JButton(quitterAction);
        quitterBtn.setBorderPainted(false);
        quitterBtn.setFocusPainted(false);
        quitterBtn.setContentAreaFilled(false);
        quitterBtn.setForeground(Color.WHITE);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 1;
        c.ipady = 60;
        //c.weighty = 1.0;
        c.weightx = 0.5;
        c.gridx = 1;
        c.gridy = 2;
        infoPanel.add(quitterBtn, c);

        JButton commencerBtn = new JButton(commencerAction);
        commencerBtn.setBorderPainted(false);
        commencerBtn.setFocusPainted(false);
        commencerBtn.setContentAreaFilled(false);
        commencerBtn.setForeground(Color.WHITE);
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridwidth = 1;
        c.ipady = 60;
        //c.weighty = 1.0;
        c.weightx = 0.5;
        c.gridx = 3;
        c.gridy = 2;
        infoPanel.add(commencerBtn, c);

        getContentPane().add(infoPanel);
    }

    public boolean getCommencer(){
        return commencer;
    }

    @Override
    public void actionPerformed(ActionEvent e) {}

}