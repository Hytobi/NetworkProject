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
    private Player myPlayer;
    private List<String> maListe;
    private List<Player> robots = new ArrayList<>();
    private List<Point> mesMAJ = new ArrayList<Point>();

    /**Constructeur de la Classe
     * @param niveau : le niveau à charger
     */
    public Carte(String myName, ResGameJoin resGameJoin) throws Exception{
        this.myName = myName;
        this.resGameJoin = resGameJoin;
        /*
        Lecture map = new Lecture("map" + resGameJoin.getMapId());
        maListe = map.getMaListe();
        maxi = map.getNbLigne();
        maxj = map.getTailleLigne();*/
        MapInfo map = resGameJoin.getStartingMap();
        maxi = map.getHeight();
        maxj = map.getWidth();
        initMap(map.getContent());
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
    public String getMyName(){
        return myName;
    }
    public Player getMyPlayer(){
        return myPlayer;
    }

    /**Méthode qui relance une partie*/
    public void initMap(String content){
        //On met tout à 0
        plateau = new Indeplacable[maxi][maxj];

        int i,j;
        char carac;
        content = content.replaceAll("\n", "");

        //Construction du tableau de jeu
        for (i=0;i<maxi;i++){
            for (j=0;j<maxj;j++){
                carac = content.charAt(i*maxj+j);
                if (carac == 'X') plateau[i][j] = new MurInca();
                else if (carac == '/') plateau[i][j] = new Vide();
                //Sinon tout le reste est un sol avec des choses posées dessus
                else if (carac == '#') plateau[i][j] = new Sol("#");
                else if (carac == 'I') {
                    plateau[i][j] = new Sol("I");
                    plateau[i][j].setAItem(true);
                    plateau[i][j].setItem(new Item("I"));
                }
                else if (carac == 'R') {
                    plateau[i][j] = new Sol("R");
                    plateau[i][j].setABombe(true);
                    plateau[i][j].setItem(new Item("R"));
                }
                else if (carac == 'B') {
                    plateau[i][j] = new Sol("B");
                    plateau[i][j].setABombe(true);
                    plateau[i][j].setItem(new Item("B"));
                }
                else if (carac == 'M') {
                    plateau[i][j] = new Sol("M");
                    plateau[i][j].setABombe(true);
                    plateau[i][j].setItem(new Item("M"));
                }
                else plateau[i][j] = new Sol(" ");
            }
        }

        if (resGameJoin.getNbPlayers() > 1){
            for (Player player : resGameJoin.getPlayers()){
                // player.getPos() = "x,y", on recupere x et y en on les transforment en integer
                int x = Integer.parseInt(player.getPos().split(",")[0]);
                int y = Integer.parseInt(player.getPos().split(",")[1]);
                player.setX(x);
                player.setY(y);
                robots.add(player);
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
        myPlayer = resGameJoin.getPlayer();
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

    public Player getPlayerByName(String name){
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
    public String robotSeDeplace(Update ppu){
        Player p = getPlayerByName(ppu.getPlayer());
        mesMAJ.add(new Point(p.getX(),p.getY()));
        String carac = plateau[p.getX()][p.getY()].getCarac();
        Indeplacable temp = plateau[p.getX()][p.getY()];
        if (temp.getABombe()){
            temp.setCarac(temp.getItem().getCarac());
        } else {
            plateau[p.getX()][p.getY()].setCarac(" ");
        }
        int speed = p.getSpeed();
        if (ppu.getDir().equals("up")){
            if (p.getX() - speed < 0){
                p.setX(0);
            } else {
                p.setX(-speed);
            }
            p.setDirection(0);
        } else if (ppu.getDir().equals("down")){
            if (p.getX() + speed >= maxi){
                p.setX(maxi-1);
            } else {
                p.setX(speed);
            }
            p.setDirection(2);
        } else if (ppu.getDir().equals("left")){
            if (p.getY() - speed < 0){
                p.setY(0);
            } else {
                p.setY(-speed);
            }
            p.setDirection(1);
        } else if (ppu.getDir().equals("right")){
            if (p.getY() + speed >= maxj){
                p.setY(maxj-1);
            } else {
                p.setY(speed);
            }
            p.setDirection(3);
        }
        mesMAJ.add(new Point(p.getX(),p.getY()));
        String item = null;
        if (plateau[p.getX()][p.getY()].getAItem()){
            item = plateau[p.getX()][p.getY()].getItem().randomItem();
            temp.setAItem(false);
            temp.setItem(null);
        }
        else {
            plateau[p.getX()][p.getY()].setCarac(carac);
        }
        // met a jour la map
        plateau[p.getX()][p.getY()].setCarac(carac);
        return item;
    }

    private void recupItem(String carac){ //a faire
        switch (carac){
            case "B":
                myPlayer.setNbClassicBomb(myPlayer.getNbClassicBomb()+1);
                break;
            case "R":
                myPlayer.setNbRemoteBomb(myPlayer.getNbRemoteBomb()+1);
                break;
            case "M":
                myPlayer.setNbMine(myPlayer.getNbMine()+1);
                break;
            case "I":
                myPlayer.setInvincible(true);
                break;
            case "S":
                myPlayer.setSpeed(myPlayer.getSpeed()+1);
                break;
            case "L":
                myPlayer.setLife(myPlayer.getLife()+1);
                break;
            case "D":
                myPlayer.setImpactDist(myPlayer.getImpactDist()+1);
                break;
            default:
                System.out.println("Erreur dans le type d'item");
                break;
        }
    }

    public void updateMyPlayer(Player p){
        if (p.getLife() < myPlayer.getLife()){
            if (!myPlayer.getInvincible()){
                myPlayer.setLife(myPlayer.getLife());
            }
        } else {
            myPlayer.setLife(p.getLife());
        }
        myPlayer.setSpeed(p.getSpeed());
        myPlayer.setNbClassicBomb(p.getNbClassicBomb());
        myPlayer.setNbMine(p.getNbMine());
        if (myPlayer.getNbRemoteBomb() > p.getNbRemoteBomb()){
            myPlayer.setNbRemoteBomb(p.getNbRemoteBomb());
            myPlayer.setArmedRemoteBomb(true);
        }
        myPlayer.setImpactDist(p.getImpactDist());
        myPlayer.setInvincible(p.getInvincible());
    }

    public void setABomb(AttackNewBomb anb){
        int x = Integer.parseInt(anb.getPos().split(",")[0]);
        int y = Integer.parseInt(anb.getPos().split(",")[1]);
        plateau[x][y].setABombe(true);
        switch (anb.getType()){
            case "classic":
                plateau[x][y].setItem(new Item("B"));
                break;
            case "remote":
                plateau[x][y].setItem(new Item("R"));
                break;
            case "mine":
                plateau[x][y].setItem(new Item("M"));
                break;
            default :
                System.out.println("Erreur dans le type de bombe");
                break;
        }
        mesMAJ.add(new Point(x,y));
    }

    public void explose(AttackExplose ae){
        int x = Integer.parseInt(ae.getPos().split(",")[0]);
        int y = Integer.parseInt(ae.getPos().split(",")[1]);
        plateau[x][y].setABombe(false);
        plateau[x][y].setItem(null);
        plateau[x][y].setCarac(" ");
        mesMAJ.add(new Point(x,y));
        String str;
        for (int i=1;i<=ae.getImpactDist();i++){
            if (x-i > 0){
                str = ae.getMap().charAt((x-i)*maxj+y) + ""; //String.valueOf(ae.getMap().charAt((x-i)*maxj+y));
                plateau[x-i][y].setCarac(str);
                mesMAJ.add(new Point(x-i,y));
            }
            if (x+i < maxi-1){
                str = ae.getMap().charAt((x+i)*maxj+y) + ""; //String.valueOf(ae.getMap().charAt((x+i)*maxj+y));
                plateau[x+i][y].setCarac(str);
                mesMAJ.add(new Point(x+i,y));
            }
            if (y-i > 0){
                str = ae.getMap().charAt(x*maxj+y-i) + ""; //String.valueOf(ae.getMap().charAt(x*maxj+y-i));
                plateau[x][y-i].setCarac(str);
                mesMAJ.add(new Point(x,y-i));
            }
            if (y+i < maxj-1){
                str = ae.getMap().charAt(x*maxj+y+i) + ""; //String.valueOf(ae.getMap().charAt(x*maxj+y+i));
                plateau[x][y+i].setCarac(str);
                mesMAJ.add(new Point(x,y+i));
            }
        }
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
