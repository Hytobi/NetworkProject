package modele;

import java.util.*;

import api.Api;
import api.JsonJouer;
import api.MapperRes;
import dto.Update;

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
        Lecture map = new Lecture("map" + resGameJoin.getMap());
        maListe = map.getMaListe();
        maxi = map.getNbLigne();
        maxj = map.getTailleLigne();
        initMap();
    }

    /**Toute les methodes pour acceder aux arguments private */
    public Player getRobot(){
        return robot;
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

    /**Méthode qui vide la liste des mises à jour*/
    public void ViderMesMAJ(){
        mesMAJ.clear();
    }

    /**
     * Méthode qui met a jour le X ou le Y du robot
     * @param c : la direction du robot
     */
    public void direction(char c){
        //On incrémente le nombre de mouvements
        //Aller vers le haut
        if (c == 'z'){
            robot.setX(-1);
            robot.setDirection(0);
        }
        //Aller vers la gauche
        else if (c == 'q'){
            robot.setY(-1);
            robot.setDirection(1);
        }
        //Aller vers le bas
        else if (c == 's'){
            robot.setX(1);
            robot.setDirection(2);
        }
        //Aller vers la droite
        else if (c == 'd'){
            robot.setY(1);
            robot.setDirection(3);
        }
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
        Api.post("ip", JsonJouer.postPlayerMove(move));
    }

    /**
     * Méthode qui fait le mouvement inverse que celui entré par le joueur
     * @param c : la direction du robot
     * @return : La direction opposée
     */
    public char counteur(char c){
        //Le counteur utilise direction pour faire le mouvement inverse, donc on décrémente de 2
        if (c=='z') return 's';
        if (c=='q') return 'd';
        if (c=='s') return 'z';
        return 'q';
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
    public void robotSeDeplace(char c){
        askForMove(c);
        // reception de la réponse todo
        String resServeur = "POST player/position/update {\"player\":\"player2\",\"dir\":\"up\" }";
        Update ppu = MapperRes.fromJson(resServeur.substring(resServeur.indexOf("{")), Update.class);
        Player p = getPlayerByName(ppu.getPlayer());
        if (ppu.getDir().equals("up")){
            p.setX(-1);
        } else if (ppu.getDir().equals("down")){
            p.setX(1);
        } else if (ppu.getDir().equals("left")){
            p.setY(-1);
        } else if (ppu.getDir().equals("right")){
            p.setY(1);
        }
        mesMAJ.add(new Point(p.getX(),p.getY()));
    }

    /**
     * Méthode qui teste si tout les caisses sont sur une destination
     * @return : vrai si c'est le cas, faux sinon
     */
    public boolean finDePartie(){
        /*
        //À voir si je laisse, mais pas tres utile (ajouté pour le bug de passage de niveau)
        if (mesDestinations.isEmpty()) return false;

        for (Point destination : mesDestinations){
            //Pour chaque destination on regarde s'il y a une caisse
            if (!plateau[(int) destination.getX()][(int) destination.getY()].aCaisse()){
                //Si y'en a pas on return faux
                return false;
            }
        }
        //Si on sort, c'est que toutes les caisses sont sur des destinations*/
        return false;
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
                else if (carac == 'S'){
                    plateau[i][j] = new Sol("S"); // speed +
                } else if (carac == 's'){
                    plateau[i][j] = new Sol("s"); // speed -
                } else if (carac == 'R'){
                    plateau[i][j] = new Sol("R"); // remote bomb +
                } else if (carac == 'M'){
                    plateau[i][j] = new Sol("M"); // mine + 
                } else if (carac == 'I'){
                    plateau[i][j] = new Sol("I"); // impact +
                } else if (carac == 'i'){
                    plateau[i][j] = new Sol("i"); // impact -
                } else if (carac == 'B'){
                    plateau[i][j] = new Sol("B"); // classic bomb +
                } else if (carac == '*'){
                    plateau[i][j] = new Sol("*"); // mur cassable
                } else plateau[i][j] = new Sol(" "); // sol
            }
        }

        if (resGameJoin.getNbPlayer() > 1){
            robots = new ArrayList<>();
            for (Player player : resGameJoin.getPlayers()){
                robots.add(new Player(player));
                // player.getPos() = "x,y", on recupere x et y en on les transforment en integer
                int x = player.getPos().substring(',')[0];
                int y = player.getPos().substring(',')[1];
                plateau[x][y] = new Sol('$');
            }
        }
        int x = resGameJoin.getStartPos().substring(',')[0];
        int y = resGameJoin.getStartPos().substring(',')[1];
        plateau[x][y] = new Sol("@");
        plateau[x][y].setAJoueur(true);
        resGameJoin.getPlayer().setX(x);
        resGameJoin.getPlayer().setY(y);
        resGameJoin.getPlayer().setName(this.myName);
        robots.add(resGameJoin.getPlayer());
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
