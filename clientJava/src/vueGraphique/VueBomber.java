package vueGraphique;

import modele.*;
import java.util.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import api.*;
import dto.*;

/**
 * Classe qui s'occupe de la vue du Sokoban
 * @author PLOUVIN Patrice
 */
public class VueBomber extends JFrame implements ActionListener,KeyListener{
    //Les images necessaires
    public static final ImageIcon CLASSIC = new ImageIcon("img/Classic.png");
    public static final ImageIcon REMOTE = new ImageIcon("img/Remote.png");
    public static final ImageIcon MINE = new ImageIcon("img/Mine.png");
    public static final ImageIcon ITEM = new ImageIcon("img/Item.png");
    public static final ImageIcon MUR_INCA = new ImageIcon("img/MurInca.png");
    public static final ImageIcon MUR = new ImageIcon("img/mur.gif");
    public static final ImageIcon SOL = new ImageIcon("img/sol.gif");
    public static final ImageIcon[] PERSO = new ImageIcon[] { new ImageIcon("img/Haut.gif"),
                                                              new ImageIcon("img/Gauche.gif"),
                                                              new ImageIcon("img/Bas.gif"),
                                                              new ImageIcon("img/Droite.gif")};

    private Carte jeu;

    private JPanel cartePanel;
    private JLabel infoLabel;
    private Action retourAction;
    private Action restartAction;
    private boolean stop = false;
    private TcpClient tcp;

    /**Constructeur de la classe vue*/
    public VueBomber(Carte jeu, TcpClient tcp){
        //Donne le titre via JFrame
        super("Bomberstudent");
        this.jeu = jeu;
        this.tcp = tcp;

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        //Les inits
        initCarte();
        initGameInfos();
        addKeyListener(this);

        //Les indispensables
        pack();
        Dimension dimEcran = getToolkit().getScreenSize();
        setLocation((dimEcran.width - getWidth())/2, (dimEcran.height- getHeight())/2);
        setResizable(false);
        setVisible(true);
    }

    /**Méthode qui initialise le JPanel
     */
    private void initCarte(){
        cartePanel = new JPanel();
        //on met un GridLayout pour faire une matrice
        cartePanel.setLayout(new GridLayout(jeu.getMaxi(), jeu.getMaxj()));
        //On l'ajoute à la vue
        getContentPane().add(cartePanel, BorderLayout.CENTER);

        String c;
        //On met l'image associée au caractère
        for (int i=0;i<jeu.getMaxi();i++) {
            for (int j=0;j<jeu.getMaxj();j++) {
                c = jeu.getPlateau()[i][j].getCarac();
                if (c == "#") cartePanel.add(new JLabel(MUR));
                else if (c == "/") cartePanel.add(new JLabel());
                else if (c == "B") cartePanel.add(new JLabel(CLASSIC));
                else if (c == "M") cartePanel.add(new JLabel(MINE));
                else if (c == "R") cartePanel.add(new JLabel(REMOTE));
                else if (c == "I") cartePanel.add(new JLabel(ITEM));
                else if (c == "X") cartePanel.add(new JLabel(MUR_INCA));
                //Sinon tout le reste est un sol avec des choses posé dessus
                else if (c == "@") cartePanel.add(new JLabel(PERSO[2]));
                else if (c == "$") cartePanel.add(new JLabel(PERSO[2]));
                else cartePanel.add(new JLabel(SOL));
            }
        }
    }

    /**Méthode qui initialise les infos de la partie
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


    /**Méthode qui met à jour l'affichage du nombre de mouvements du robot
     */
    private void updateGameInfos() {
        Player p = jeu.getPlayerByName(this.jeu.getMyName());

        infoLabel.setText("pv :" + p.getLife() + " | speed :" + p.getSpeed() + " | BClassic :" + p.getNbClassicBomb() + " | Mine :" + p.getNbMine() + " | BRemote :" + p.getNbRemoteBomb() + " | impactDist :" + p.getImpactDist() + " | invincible :" + p.getInvincible() );
    }

    /**Méthode qui parcourt la liste des Mises a Jour pour afficher la bonne image
     */
    public void parcourDesMAJ(){
        int index;
        String c;
        //Le premier point de la liste est l'endroit où était le bot avant le mouvement
        //Le perso peut être sur un Sol ou une Destination
        for (Point p : jeu.getMesMAJ()) {

            index = (int)p.getX() * jeu.getMaxj() + (int)p.getY();
            c = jeu.getPlateau()[(int)p.getX()][(int)p.getY()].getCarac();
            if (c == " ") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(SOL), index);
            } else if (c == "B"){
                cartePanel.remove(index);
                cartePanel.add(new JLabel(CLASSIC), index); // bombe
            } else if (c == "M"){
                cartePanel.remove(index);
                cartePanel.add(new JLabel(MINE), index); // mine
            } else if (c == "R"){
                cartePanel.remove(index);
                cartePanel.add(new JLabel(REMOTE), index); //remote
            } else if (c == "$") { //pas encore fait
                cartePanel.remove(index);
                cartePanel.add(new JLabel(PERSO[1]), index);
            } else if (c == "@") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(PERSO[jeu.getMyPlayer().getDirection()]), index); // jeu.getRobot().getDirection()
            }
        }
        //On met à jour la vu et on vide la liste des mises à jour
        cartePanel.updateUI();
        jeu.ViderMesMAJ();
    }

    @Override
    public void keyTyped(KeyEvent e) {}
    @Override
    public void keyReleased(KeyEvent e) {}
    @Override

    /**Méthode qui gère ce qui se passe quand une touche est pressée
     * @param e : un événement
     */
    public void keyPressed(KeyEvent e) {
        //Si le jeu est fini on fait rien
        if (stop) return;
        //Selon l'entrée on déplace le robot dans la direction voulue
        String move = null;
        String attack = null;
        String type = null; // a retirer
        if ((e.getKeyCode() == KeyEvent.VK_RIGHT) || (e.getKeyChar() == 'd')) {
            System.out.println("Envoie demande deplacement a droite");
            tcp.post(JsonJouer.postPlayerMove("right"));
            move = "{\"player\":\"player3\",\"dir\":\"right\"}";  //tcp.get();
        } 
        else if ((e.getKeyCode() == KeyEvent.VK_LEFT) || (e.getKeyChar() == 'q')){
            System.out.println("Envoie demande deplacement a gauche");
            tcp.post(JsonJouer.postPlayerMove("left"));
            move = "{\"player\":\"player3\",\"dir\":\"left\"}";  //tcp.get();
        }
        else if ((e.getKeyCode() == KeyEvent.VK_DOWN) || (e.getKeyChar() == 's')){
            System.out.println("Envoie demande deplacement en bas");
            tcp.post(JsonJouer.postPlayerMove("down"));
            move = "{\"player\":\"player3\",\"dir\":\"down\"}";  //tcp.get();
        }
        else if ((e.getKeyCode() == KeyEvent.VK_UP) || (e.getKeyChar() == 'z')) {
            System.out.println("Envoie demande deplacement en haut");
            tcp.post(JsonJouer.postPlayerMove("up"));
            move = "{\"player\":\"player3\",\"dir\":\"up\"}";  //tcp.get();
        }
        else if (e.getKeyChar() == 'r'){
            if (jeu.getMyPlayer().getNbRemoteBomb() == 0) return;
            System.out.println("Envoie demande posser une remote bomb");
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY();
            type = "remote";
            tcp.post(JsonJouer.postAttackBomb(pos, "remote"));
            attack = getDebugAttack(true, false, false);  //tcp.get();
        }
        else if (e.getKeyChar() == 'f' || e.getKeyChar() == 'm'){
            if (jeu.getMyPlayer().getNbMine() == 0) return;
            System.out.println("Envoie demande posser une mine");
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY();
            type = "mine";
            tcp.post(JsonJouer.postAttackBomb(pos, "mine"));
            attack = getDebugAttack(false, true, false); //tcp.get();
        }
        else if (e.getKeyChar() == 'c'){
            if (jeu.getMyPlayer().getNbClassicBomb() == 0) return;
            System.out.println("Envoie demande posser une classic bomb");
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY();
            type = "classic";
            tcp.post(JsonJouer.postAttackBomb(pos, "classic"));
            attack = getDebugAttack(false, false, true); //tcp.get();
        }
        if (move != null) {
            Update res = MapperRes.fromJson(move, Update.class);
            if (res != null){
                jeu.robotSeDeplace(res);
            } else {
                System.out.println("Erreur lors de la récupération du move");
            }
        } else if (attack != null) {
            AttackBomb res = MapperRes.fromJson(attack, AttackBomb.class);
            if (res != null){
                jeu.updateMyPlayer(res.getPlayer());
            } else {
                System.out.println("Erreur lors de la récupération du move");
            }
            String pos = jeu.getMyPlayer().getX() + "," + jeu.getMyPlayer().getY(); // a retirer
            String ares = "POST attack/newbomb {\"pos\":\""+ pos+"\",\"type\":\""+type+"\"}";            //tcp.get();
            AttackNewBomb anb = MapperRes.fromJson("{" + ares.split("\\{")[1], AttackNewBomb.class);
            if (anb != null){
                jeu.setABomb(anb);
            } else {
                System.out.println("Erreur lors de la récupération du move");
            }
        }

        //On met à jour les infos de la partie (le nombre de mouvements)
        updateGameInfos();
        //S'il n'y a pas de mises à jour on sort, sinon on met à jour
        if (jeu.getMesMAJ().isEmpty()) return;
        else parcourDesMAJ();
        //Si le jeu est fini on arrête et on sort (jamais trop prudent)
        if (jeu.finDePartie()){
            stop = true;
            return;
        }
    }

    /**Méthode utilisée dans le main pour le message de fin
     * @param i : le nombre de niveau
     * @param nb : le nombre de pas total
     */
    public void messageFin(int i, int nb){
        JOptionPane.showMessageDialog(this,"Vous avez fini les " + i +" niveaux en "+ nb +" mouvements");
    }

    @Override
    public void actionPerformed(ActionEvent e) {}

    private String getDebugAttack(boolean remote, boolean mine, boolean classic){
        return "{\"action\":\"attack/bomb\", \"statut\":\"201\", \"message\":\"bomb is armed at pos 5,3\",\"player\":{\"life\":100,\"speed\":1,\"nbClassicBomb\":1,\"nbMine\":1,\"nbRemoteBomb\":1,\"impactDist\":2,\"invincible\":false}}";
    }
}
