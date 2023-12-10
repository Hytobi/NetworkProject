package modele;

import java.util.*;

import api.*;
import dto.*;

import java.awt.Point;
import java.awt.Robot;

/**
 * Classe qui modélise la Carte de jeu (la map)
 * @author PLOUVIN Patrice
 */
public class Carte{
    //Argument de construction de la classe
    private String myName;
    private ResGameJoin resGameJoin;

    //Construction du tableau de jeu
    private Indeplacable[][] plateau;
    private int maxi;
    private int maxj;
    private List<String> maListe;
    private List<Player> robots;
    private List<Point> mesMAJ = new ArrayList<Point>();

    /**Constructeur de la Classe
     * @param niveau : le niveau à charger
     */
    public Carte(String myName, ResGameJoin resGameJoin) throws Exception{
        this.myName = myName;
        this.resGameJoin = resGameJoin;
        //Charger une map
        Lecture map = new Lecture("map" + resGameJoin.getMapId());
        maListe = map.getMaListe();
        maxi = map.getNbLigne();
        maxj = map.getTailleLigne();
        initMap();
    }

    /**Toute les methodes pour acceder aux arguments private */
    public List<Player> getRobots(){
        return robots;
    }
    public int getMaxi(){
        return maxi;
    }
    public int getMaxj(){
        return maxj;
    }
    public List<Point> getMesMAJ(){
        return mesMAJ;
    }
    public Indeplacable[][] getPlateau(){
        return plateau;
    }

    /**Méthode qui relance une partie*/
    public void initMap(){
        //On met tout à 0
        plateau = new Indeplacable[maxi][maxj];

        int i,j;
        String temp;
        char carac;

        //Construction du tableau de jeu
        for (i=0;i<maxi;i++){
            temp = maListe.get(i);
            for (j=0;j<maxj;j++){
                carac = temp.charAt(j);
                if (carac == '#') plateau[i][j] = new Mur();
                else if (carac == '/') plateau[i][j] = new Vide();
                //Sinon tout le reste est un sol avec des choses posées dessus
                else if (carac == ' ') new Sol(" ");
                else {
                    plateau[i][j] = new Sol(String.valueOf(carac)); // speed +
                    plateau[i][j].setAItem(true);
                    plateau[i][j].setItem(new Item(String.valueOf(carac)));
                }
            }
        }

        if (resGameJoin.getNbPlayer() > 1){
            robots = new ArrayList<>();
            for (Player player : resGameJoin.getPlayers()){
                robots.add(player);
                // player.getPos() = "x,y", on recupere x et y en on les transforment en integer
                int x = Integer.parseInt(player.getPos().split(",")[0]);
                int y = Integer.parseInt(player.getPos().split(",")[1]);
                plateau[x][y] = new Sol("$");
            }
        }
        int x = Integer.parseInt(resGameJoin.getStartPos().split(",")[0]);
        int y = Integer.parseInt(resGameJoin.getStartPos().split(",")[1]);
        plateau[x][y] = new Sol("@");
        plateau[x][y].setAJoueur(true);
        resGameJoin.getPlayer().setX(x);
        resGameJoin.getPlayer().setY(y);
        resGameJoin.getPlayer().setName(this.myName);
        robots.add(resGameJoin.getPlayer());
    }

    /**Méthode qui vide la liste des mises à jour*/
    public void ViderMesMAJ(){
        mesMAJ.clear();
    }

    private void askForMove(char c){
        String move;
        if (c == 'z'){
            move = "up";
        } else if (c == 'q'){
            move = "left";
        } else if (c == 's'){
            move = "down";
        } else if (c == 'd'){
            move = "right";
        }
        //Api.post("ip", JsonJouer.postPlayerMove(move));
    }

    private Player getPlayerByName(String name){
        for (Player p : robots){
            if (p.getName().equals(name)){
                return p;
            }
        }
        return null;
    }

    /**
     * Méthode qui teste si le robot peut se deplacer et le déplace
     * @param c : La direction du robot
     * @return : vrai si le robot a pu se deplacer, faux sinon
     */
    public void robotSeDeplace(Update ppu){
        Player p = getPlayerByName(ppu.getPlayer());
        String carac = plateau[p.getX()][p.getY()].getCarac();
        Indeplacable temp = plateau[p.getX()][p.getY()];
        if (temp.getAItem()){
            temp.setCarac(temp.getItem().getCarac());
        } else {
            plateau[p.getX()][p.getY()].setCarac(" ");
        }
        if (ppu.getDir().equals("up")){
            p.setX(-1);
            p.setDirection(0);
        } else if (ppu.getDir().equals("down")){
            p.setX(1);
            p.setDirection(2);
        } else if (ppu.getDir().equals("left")){
            p.setY(-1);
            p.setDirection(3);
        } else if (ppu.getDir().equals("right")){
            p.setY(1);
            p.setDirection(1);
        }
        mesMAJ.add(new Point(p.getX(),p.getY()));
        // met a jour la map
        plateau[p.getX()][p.getY()].setCarac(carac);
    }

    /**
     * Méthode qui teste si le joueur est mort
     * @return : vrai si c'est le cas, faux sinon
     */
    public boolean finDePartie(){
        return getPlayerByName(myName).getLife() <= 0;
    }

    /**
     * Méthode qui affiche la carte.
     * Pour la vueTexte
     */
    public String toString(){
        StringBuffer sb = new StringBuffer();
        for (int i=0;i<maxi;i++){
            for (int j=0;j<maxj;j++){
                sb.append(plateau[i][j].toString());
            }
            sb.append("\n");
        }
        return sb.toString();
    }
}
