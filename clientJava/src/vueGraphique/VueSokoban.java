package vueGraphique;

import modele.*;
import java.util.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

/**
 * Classe qui s'occupe de la vue du Sokoban
 * @author PLOUVIN Patrice
 */
public class VueSokoban extends JFrame implements ActionListener,KeyListener{
    //Les images necessaires
    public static final ImageIcon DESTINATION = new ImageIcon("img/but.gif");
    public static final ImageIcon CAISSE = new ImageIcon("img/caisse1.gif");
    public static final ImageIcon CAISSE_SUR_DESTINATION = new ImageIcon("img/caisse2.gif");
    public static final ImageIcon MUR = new ImageIcon("img/mur.gif");
    public static final ImageIcon SOL = new ImageIcon("img/sol.gif");
    public static final ImageIcon[] PERSO = new ImageIcon[] { new ImageIcon("img/Haut.gif"),
                                                              new ImageIcon("img/Gauche.gif"),
                                                              new ImageIcon("img/Bas.gif"),
                                                              new ImageIcon("img/Droite.gif")};

    private Carte jeu;

    private JPanel cartePanel;
    private JLabel nbMouvementLabel;
    private Action retourAction;
    private Action restartAction;
    private boolean stop = false;

    /**Constructeur de la classe vue*/
    public VueSokoban(Carte jeu){
        //Donne le titre via JFrame
        super("Sokoban");
        this.jeu = jeu;

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        //Les inits
        initActions();
        initToolBar();
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
                else if (c == ".") cartePanel.add(new JLabel(DESTINATION));
                //Sinon tout le reste est un sol avec des choses posé dessus
                else if (c == "@") cartePanel.add(new JLabel(PERSO[2]));
                else if (c == "$") cartePanel.add(new JLabel(CAISSE));
                else if (c == "*") cartePanel.add(new JLabel(CAISSE_SUR_DESTINATION));
                else cartePanel.add(new JLabel(SOL));
            }
        }
    }

    /**Méthode qui initialise les actions des boutons */
    private void initActions() {
        retourAction = new AbstractAction("Retour") {
            public void actionPerformed(ActionEvent e) {
                if ((jeu.getNbMouvement()>0)); {
                    jeu.faireDernierMouvement();
                    if (!jeu.getMesMAJ().isEmpty()) parcourDesMAJ();
                    updateGameInfos();
                }
            }
        };
        restartAction = new AbstractAction("Restart") {
            public void actionPerformed(ActionEvent e) {
                jeu.restart();
                initCarte();
                initGameInfos();
                cartePanel.updateUI();
            }
        };
    }

    /**Méthode qui desactive les boutons lorsque le dernier niveau est fini
     */
    public void desactiveAction(){
        retourAction.setEnabled(false);
        restartAction.setEnabled(false);
    }

    /**Méthode qui initialise la bare d'outil et les boutons */
    private void initToolBar() {
        JToolBar toolBar = new JToolBar();
        JButton retourBtn = new JButton(retourAction);
        JButton restartBtn = new JButton(restartAction);
        retourBtn.setFocusable(false);
        restartBtn.setFocusable(false);
        //Pour que les boutons soient beaux
        retourBtn.setBorderPainted(false);
        restartBtn.setBorderPainted(false);
        retourBtn.setFocusPainted(false);
        restartBtn.setFocusPainted(false);
        retourBtn.setContentAreaFilled(false);
        restartBtn.setContentAreaFilled(false);
        retourBtn.setForeground(Color.WHITE);
        restartBtn.setForeground(Color.WHITE);
        //Ajout des boutons a la ToolBar
        toolBar.add(retourBtn);
        toolBar.addSeparator();
        toolBar.add(restartBtn);
        //Pour que la ToolBar soit belle
        toolBar.setBackground(new Color(28, 25, 71));
        toolBar.setFloatable(false);
        toolBar.setRollover(true);
        toolBar.setBorderPainted(false);
        //On l'ajoute à la vue
        getContentPane().add(toolBar, BorderLayout.NORTH);
    }

    /**Méthode qui initialise les infos de la partie
     * On met un label pour le nombre de mouvements dans un panel en bas
     */
    private void initGameInfos() {
        JPanel gameInfos = new JPanel();
        nbMouvementLabel = new JLabel();
        nbMouvementLabel.setForeground(Color.WHITE);
        gameInfos.add(nbMouvementLabel);
        updateGameInfos();
        gameInfos.setBackground(new Color(28, 25, 71));
        getContentPane().add(gameInfos, BorderLayout.SOUTH);
    }


    /**Méthode qui met à jour l'affichage du nombre de mouvements du robot
     */
    private void updateGameInfos() {
        int nbMouvement = jeu.getNbMouvement();
        if (nbMouvement < 2)
            nbMouvementLabel.setText(nbMouvement + " Mouvement");
        else
            nbMouvementLabel.setText(nbMouvement + " Mouvements");
    }

    /**Méthode qui parcourt la liste des Mises a Jour pour afficher la bonne image
     */
    public void parcourDesMAJ(){
        int index;
        Point p;
        String c;
        //Le premier point de la liste est l'endroit où était le bot avant le mouvement
        //Le perso peut être sur un Sol ou une Destination
        p = jeu.getMesMAJ().get(0);
        index = (int)p.getX() * jeu.getMaxj() + (int)p.getY();
        c = jeu.getPlateau()[(int)p.getX()][(int)p.getY()].getCarac();
        if (c == " ") {
            cartePanel.remove(index);
            cartePanel.add(new JLabel(SOL), index);
        } else if (c == "."){
            cartePanel.remove(index);
            cartePanel.add(new JLabel(DESTINATION), index);
        } else if (c == "$") { //Pour le cas retour
            cartePanel.remove(index);
            cartePanel.add(new JLabel(CAISSE), index);
        }

        //Le deuxième est toujours le perso à sa nouvelle place
        p = jeu.getMesMAJ().get(1);
        index = (int)p.getX() * jeu.getMaxj() + (int)p.getY();
        c = jeu.getPlateau()[(int)p.getX()][(int)p.getY()].getCarac();
        //On fait un test, car jamais trop prudent
        if ((c == "@") || (c == "+")) {
            cartePanel.remove(index);
            cartePanel.add(new JLabel(PERSO[jeu.getRobot().getDirection()]), index);
        }

        //S'il existe le 3eme est la ou se trouve la caisse apres déplacement
        if (jeu.getMesMAJ().size()>2) {
            p = jeu.getMesMAJ().get(2);
            index = (int) p.getX() * jeu.getMaxj() + (int) p.getY();
            c = jeu.getPlateau()[(int)p.getX()][(int)p.getY()].getCarac();
            //Si la caisse est sur une Destination ou sur un Sol ce n'est pas la même image
            if (c=="*"){
                cartePanel.remove(index);
                cartePanel.add(new JLabel(CAISSE_SUR_DESTINATION), index);
            } else if (c == "$") {
                cartePanel.remove(index);
                cartePanel.add(new JLabel(CAISSE), index);
            }
        }

        //Si le 4eme existe, c'est que le joueur a fait un retour
        //On met à jour la ou se trouvait la caisse avant le retour
        if (jeu.getMesMAJ().size()==4) {
            p = jeu.getMesMAJ().get(3);
            index = (int) p.getX() * jeu.getMaxj() + (int) p.getY() ;
            c = jeu.getPlateau()[(int)p.getX()][(int)p.getY()].getCarac();
            //La caisse pouvait être sur une destination ou un Sol
            if (c=="."){
                cartePanel.remove(index);
                cartePanel.add(new JLabel(DESTINATION), index);
            } else if (c==" "){
                cartePanel.remove(index);
                cartePanel.add(new JLabel(SOL), index);
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
        if ((e.getKeyCode() == KeyEvent.VK_RIGHT) || (e.getKeyChar() == 'd')) jeu.robotSeDeplace('d');
        if ((e.getKeyCode() == KeyEvent.VK_LEFT) || (e.getKeyChar() == 'q')) jeu.robotSeDeplace('q');
        if ((e.getKeyCode() == KeyEvent.VK_DOWN) || (e.getKeyChar() == 's')) jeu.robotSeDeplace('s');
        if ((e.getKeyCode() == KeyEvent.VK_UP) || (e.getKeyChar() == 'z')) jeu.robotSeDeplace('z');
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
}
