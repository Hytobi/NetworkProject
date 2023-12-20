package vueGraphique;

import modele.*;
import java.util.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import api.*;
import dto.*;

/**
 * VueBomber : Gère les actions relatives au jeu
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class VueBomber extends JFrame implements ActionListener, KeyListener {
    private static final Integer MAX_RETRY = 3;

    // Les images necessaires
    public static final ImageIcon CLASSIC = new ImageIcon("img/Classic.png");
    public static final ImageIcon REMOTE = new ImageIcon("img/Remote.png");
    public static final ImageIcon MINE_CHAR = new ImageIcon("img/Mine.png");
    public static final ImageIcon ITEM_CHAR = new ImageIcon("img/Item.png");
    public static final ImageIcon MUR_INCA_CHAR = new ImageIcon("img/MurInca.png");
    public static final ImageIcon MUR_CHAR = new ImageIcon("img/mur.gif");
    public static final ImageIcon SOL_CHAR = new ImageIcon("img/sol.gif");
    public static final ImageIcon[] PERSO = new ImageIcon[] { new ImageIcon("img/Haut.gif"),
            new ImageIcon("img/Gauche.gif"),
            new ImageIcon("img/Bas.gif"),
            new ImageIcon("img/Droite.gif") };

    private Carte jeu;

    private JPanel cartePanel;
    private JLabel infoLabel;
    private boolean stop = false;
    private TcpClient tcp;

    /** Constructeur de la classe vue */
    public VueBomber(Carte jeu, TcpClient tcp) {
        // Donne le titre via JFrame
        super("Bomberstudent");
        this.jeu = jeu;
        this.tcp = tcp;

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // Les inits
        initCarte();
        initGameInfos();
        addKeyListener(this);

        // Les indispensables
        pack();
        Dimension dimEcran = getToolkit().getScreenSize();
        setLocation((dimEcran.width - getWidth()) / 2, (dimEcran.height - getHeight()) / 2);
        setResizable(false);
        setVisible(true);
    }

    /**
     * Méthode qui initialise le JPanel
     */
    private void initCarte() {
        cartePanel = new JPanel();
        // on met un GridLayout pour faire une matrice
        cartePanel.setLayout(new GridLayout(jeu.getMaxi(), jeu.getMaxj()));
        // On l'ajoute à la vue
        getContentPane().add(cartePanel, BorderLayout.CENTER);

        String c;
        // On met l'image associée au caractère
        for (int i = 0; i < jeu.getMaxi(); i++) {
            for (int j = 0; j < jeu.getMaxj(); j++) {
                c = jeu.getPlateau()[i][j].getCarac();
                if (c == "#")
                    cartePanel.add(new JLabel(MUR_CHAR));
                else if (c == "/")
                    cartePanel.add(new JLabel());
                else if (c == "B")
                    cartePanel.add(new JLabel(CLASSIC));
                else if (c == "M")
                    cartePanel.add(new JLabel(MINE_CHAR));
                else if (c == "R")
                    cartePanel.add(new JLabel(REMOTE));
                else if (c == "I")
                    cartePanel.add(new JLabel(ITEM_CHAR));
                else if (c == "X")
                    cartePanel.add(new JLabel(MUR_INCA_CHAR));
                // Sinon tout le reste est un sol avec des choses posé dessus
                else if (c == "@")
                    cartePanel.add(new JLabel(PERSO[2]));
                else if (c == "$")
                    cartePanel.add(new JLabel(PERSO[2]));
                else
                    cartePanel.add(new JLabel(SOL_CHAR));
            }
        }
    }

    /**
     * Méthode qui initialise les infos de la partie
     * On met un label pour le nombre de mouvements dans un panel en bas
     */
    private void initGameInfos() {
        JPanel gameInfos = new JPanel();
        infoLabel = new JLabel();
        infoLabel.setForeground(Color.WHITE);
        gameInfos.add(infoLabel);
        updateGameInfos();
        gameInfos.setBackground(new Color(28, 25, 71));
        getContentPane().add(gameInfos, BorderLayout.SOUTH);
    }

    /**
     * Méthode qui met à jour l'affichage du nombre de mouvements du robot
     */
    public void updateGameInfos() {
        Player p = jeu.getPlayerByName(this.jeu.getMyName());

        infoLabel.setText("pv :" + p.getLife() + " | speed :" + p.getSpeed() + " | BClassic :" + p.getNbClassicBomb()
                + " | Mine :" + p.getNbMine() + " | BRemote :" + p.getNbRemoteBomb() + " | impactDist :"
                + p.getImpactDist() + " | invincible :" + p.getInvincible());
    }

    /**
     * Méthode qui parcourt la liste des Mises a Jour pour afficher la bonne image
     */
    public void parcourDesMAJ(String playerName) {
        if (jeu.getMesMAJ().isEmpty())
            return;
        int index;
        String c;
        for (Point p : jeu.getMesMAJ()) {
            index = (int) p.getX() * jeu.getMaxj() + (int) p.getY();
            c = jeu.getPlateau()[(int) p.getX()][(int) p.getY()].getCarac();
            if (c == " ") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(SOL_CHAR), index);
            } else if (c == "B") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(CLASSIC), index); // bombe
            } else if (c == "M") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(MINE_CHAR), index); // mine
            } else if (c == "R") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(REMOTE), index); // remote
            } else if (c == "$") {
                cartePanel.remove(index);
                if (playerName != null) {
                    Player player = jeu.getPlayerByName(playerName);
                    cartePanel.add(new JLabel(PERSO[player.getDirection()]), index);
                } else {
                    cartePanel.add(new JLabel(PERSO[2]), index);
                }
            } else if (c == "@") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(PERSO[jeu.getMyPlayer().getDirection()]), index);
            } else if (c == "I") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(ITEM_CHAR), index);
            }
        }
        // On met à jour la vu et on vide la liste des mises à jour
        cartePanel.updateUI();
        jeu.ViderMesMAJ();
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }

    @Override

    /**
     * Méthode qui gère ce qui se passe quand une touche est pressée
     * 
     * @param e : un événement
     */
    public void keyPressed(KeyEvent e) {
        // Si le jeu est fini on fait rien
        if (stop)
            return;
        // Selon l'entrée on déplace le robot dans la direction voulue
        if ((e.getKeyCode() == KeyEvent.VK_RIGHT) || (e.getKeyChar() == 'd')) {
            System.out.println("[POST] deplacement a droite");
            tcp.post(JsonJouer.postPlayerMove("right"));
        } else if ((e.getKeyCode() == KeyEvent.VK_LEFT) || (e.getKeyChar() == 'q')) {
            System.out.println("[POST] deplacement a gauche");
            tcp.post(JsonJouer.postPlayerMove("left"));
        } else if ((e.getKeyCode() == KeyEvent.VK_DOWN) || (e.getKeyChar() == 's')) {
            System.out.println("[POST] deplacement en bas");
            tcp.post(JsonJouer.postPlayerMove("down"));
        } else if ((e.getKeyCode() == KeyEvent.VK_UP) || (e.getKeyChar() == 'z')) {
            System.out.println("[POST] deplacement en haut");
            tcp.post(JsonJouer.postPlayerMove("up"));
        } else if (e.getKeyChar() == 'r') {
            if (jeu.getMyPlayer().getNbRemoteBomb() == 0)
                return;
            System.out.println("[POST] posser une remote bomb");
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY();
            tcp.post(JsonJouer.postAttackBomb(pos, "remote"));
        } else if (e.getKeyChar() == 'f' || e.getKeyChar() == 'm') {
            if (jeu.getMyPlayer().getNbMine() == 0)
                return;
            System.out.println("[POST] posser une mine");
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY();
            tcp.post(JsonJouer.postAttackBomb(pos, "mine"));
        } else if (e.getKeyChar() == 'c') {
            if (jeu.getMyPlayer().getNbClassicBomb() == 0)
                return;
            System.out.println("[POST] posser une classic bomb");
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY();
            tcp.post(JsonJouer.postAttackBomb(pos, "classic"));
        } else if (e.getKeyChar() == 'e') {
            if (!jeu.getMyPlayer().getArmedRemoteBomb())
                return;
            System.out.println("[POST] explosion de remote bomb");
            tcp.post(JsonJouer.postAttackRemoteGo());
        }

        if (jeu.finDePartie()) {
            stop = true;
            messageFin();
            System.exit(0);
        }
    }

    /**
     * Méthode qui gère ce qui se passe quand le serveur envoie un message de fin
     */
    public void serveurRageQuit() {
        JOptionPane.showMessageDialog(this, "La connection au serveur a été intérompu");
        try {
            tcp.closeSocket();
        } catch (Exception e) {
            System.out.println("Erreur lors de la fermeture du socket");
        } finally {
            System.exit(0);
        }
    }

    /** Méthode utilisée dans le main pour le message de fin */
    public void messageFin() {
        JOptionPane.showMessageDialog(this, "Partie terminée !");
    }

    @Override
    public void actionPerformed(ActionEvent e) {
    }
}
