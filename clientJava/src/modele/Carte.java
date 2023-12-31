package modele;

import java.util.*;

import api.*;
import dto.*;

import java.awt.Point;
import java.awt.Robot;

/**
 * Carte : Objet notifiant que la mine a explosé (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class Carte {
    // Argument de construction de la classe
    private String myName;
    private ResGameJoin resGameJoin;

    // Construction du tableau de jeu
    private Indeplacable[][] plateau;
    private int maxi;
    private int maxj;
    private Player myPlayer;
    private List<String> maListe;
    private List<Player> robots = new ArrayList<>();
    private List<Point> mesMAJ = new ArrayList<Point>();

    /**
     * Constructeur de la Classe
     * 
     * @param niveau : le niveau à charger
     */
    public Carte(String myName, ResGameJoin resGameJoin) throws Exception {
        this.myName = myName;
        this.resGameJoin = resGameJoin;
        MapInfo map = resGameJoin.getStartingMap();
        maxi = map.getHeight();
        maxj = map.getWidth();
        initMap(map.getContent());
    }

    /** Toute les methodes pour acceder aux arguments private */
    public List<Player> getRobots() {
        return robots;
    }

    public int getMaxi() {
        return maxi;
    }

    public int getMaxj() {
        return maxj;
    }

    public List<Point> getMesMAJ() {
        return mesMAJ;
    }

    public Indeplacable[][] getPlateau() {
        return plateau;
    }

    public String getMyName() {
        return myName;
    }

    public Player getMyPlayer() {
        return myPlayer;
    }

    /**
     * Méthode qui initialise la carte
     * 
     * @param content : le contenu de la carte
     */
    public void initMap(String content) {
        // On met tout à 0
        plateau = new Indeplacable[maxi][maxj];

        int i, j;
        char carac;
        content = content.replaceAll("\n", "");
        // Si on rejoin on a les caractères @ qui sont des joueurs
        content = content.replaceAll("@", " ");

        // Construction du tableau de jeu
        for (i = 0; i < maxi; i++) {
            for (j = 0; j < maxj; j++) {
                carac = content.charAt(i * maxj + j);
                if (carac == 'X')
                    plateau[i][j] = new MurInca();
                else if (carac == '/')
                    plateau[i][j] = new Vide();
                // Sinon tout le reste est un sol avec des choses posées dessus
                else if (carac == '#')
                    plateau[i][j] = new Sol("#");
                else if (carac == 'I') {
                    plateau[i][j] = new Sol("I");
                    plateau[i][j].setAItem(true);
                    plateau[i][j].setItem(new Item("I"));
                } else if (carac == 'R') {
                    plateau[i][j] = new Sol("R");
                    plateau[i][j].setABombe(true);
                    plateau[i][j].setItem(new Item("R"));
                } else if (carac == 'B') {
                    plateau[i][j] = new Sol("B");
                    plateau[i][j].setABombe(true);
                    plateau[i][j].setItem(new Item("B"));
                } else if (carac == 'M') {
                    plateau[i][j] = new Sol("M");
                    plateau[i][j].setABombe(true);
                    plateau[i][j].setItem(new Item("M"));
                } else
                    plateau[i][j] = new Sol(" ");
            }
        }

        // Si c'est une connection a une game en cours, il faut ajouter les autres
        // joueurs
        if (resGameJoin.getPlayers() != null && !resGameJoin.getPlayers().isEmpty()) {
            for (Player player : resGameJoin.getPlayers()) {
                // player.getPos() = "x,y", on recupere x et y en on les transforment en integer
                int x = Integer.parseInt(player.getPos().split(",")[0]);
                int y = Integer.parseInt(player.getPos().split(",")[1]);
                player.setX(x);
                player.setY(y);
                player.setSpeed(1);
                player.setDirection(2);
                robots.add(player);
                plateau[x][y] = new Sol("$");
            }
        }

        // On ajoute le joueur
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

    /** Méthode qui vide la liste des mises à jour */
    public void ViderMesMAJ() {
        mesMAJ.clear();
    }

    /**
     * Méthode qui récupère l'objet Player à partir de son nom
     * 
     * @param name : le nom du joueur
     * @return : le joueur, null si il n'existe pas
     */
    public Player getPlayerByName(String name) {
        for (Player p : robots) {
            if (p.getName().equals(name)) {
                return p;
            }
        }
        return null;
    }

    /**
     * Méthode qui gère l'explosion de la mine
     * 
     * @param me : l'objet qui contient les informations de l'explosion
     */
    public void exploseMine(MineExplose me) {
        int x = Integer.parseInt(me.getPos().split(",")[0]);
        int y = Integer.parseInt(me.getPos().split(",")[1]);
        plateau[x][y].setABombe(false);
        plateau[x][y].setItem(null);
        plateau[x][y].setCarac("@");
        mesMAJ.add(new Point(x, y));
    }

    /**
     * Méthode qui gère le séplacement du joueur
     * 
     * @param ppu : l'objet qui contient les informations de déplacement
     * @return : le type d'item récupéré, null si il n'y en a pas
     */
    public String robotSeDeplace(Update ppu) {
        // On recupere le joueur
        Player p = getPlayerByName(ppu.getPlayer());
        // On mettra a jour la case ou il se trouve actuellement
        mesMAJ.add(new Point(p.getX(), p.getY()));
        String carac = plateau[p.getX()][p.getY()].getCarac();
        // set Le nouveau caractère
        Indeplacable temp = plateau[p.getX()][p.getY()];
        if (temp.getABombe()) {
            temp.setCarac(temp.getItem().getCarac());
        } else {
            plateau[p.getX()][p.getY()].setCarac(" ");
        }
        int speed = p.getSpeed();
        // On met a jour les coordonnées du joueur
        if (ppu.getDir().equals("up")) {
            if (p.getX() - speed < 0) {
                p.setX(0);
            } else {
                p.setX(-speed);
            }
            p.setDirection(0);
        } else if (ppu.getDir().equals("down")) {
            if (p.getX() + speed >= maxi) {
                p.setX(maxi - 1);
            } else {
                p.setX(speed);
            }
            p.setDirection(2);
        } else if (ppu.getDir().equals("left")) {
            if (p.getY() - speed < 0) {
                p.setY(0);
            } else {
                p.setY(-speed);
            }
            p.setDirection(1);
        } else if (ppu.getDir().equals("right")) {
            if (p.getY() + speed >= maxj) {
                p.setY(maxj - 1);
            } else {
                p.setY(speed);
            }
            p.setDirection(3);
        }
        // On met a jour la case ou il se trouve maintenant
        mesMAJ.add(new Point(p.getX(), p.getY()));
        // s'il y a un item il le ramasse
        String item = null;
        if (plateau[p.getX()][p.getY()].getAItem() && myPlayer.getX() == p.getX() && myPlayer.getY() == p.getY()) {
            item = plateau[p.getX()][p.getY()].getItem().randomItem();
            temp.setAItem(false);
            temp.setItem(null);
        } else {
            plateau[p.getX()][p.getY()].setCarac(carac);
        }
        // met a jour la map
        plateau[p.getX()][p.getY()].setCarac(carac);
        if (myPlayer.getInvincible()) {
            myPlayer.setNbMoveInvincible(myPlayer.getNbMoveInvincible() - 1);
            if (myPlayer.getNbMoveInvincible() == 0) {
                myPlayer.setInvincible(false);
            }
        }
        return item;
    }

    /**
     * Méthode qui gère l'arrivée d'un nouveau joueur
     * 
     * @param pn : l'objet qui contient les informations du nouveau joueur
     */
    public void setNewPlayer(PlayerNew pn) {
        Player p = new Player();
        p.setName(pn.getPlayer());

        int x = Integer.parseInt(pn.getPos().split(",")[0]);
        int y = Integer.parseInt(pn.getPos().split(",")[1]);
        p.setX(x);
        p.setY(y);
        p.setSpeed(1);
        p.setDirection(2);
        robots.add(p);
        plateau[x][y] = new Sol("$");
        mesMAJ.add(new Point(x, y));
    }

    /**
     * Méthode qui gère la mise à jour des informations du joueur
     * 
     * @param p : l'objet qui contient les informations du joueur
     */
    public void updateMyPlayer(Player p) {
        if (p.getLife() < myPlayer.getLife()) {
            if (!myPlayer.getInvincible()) {
                myPlayer.setLife(p.getLife());
            }
        } else {
            myPlayer.setLife(p.getLife());
        }
        myPlayer.setSpeed(p.getSpeed());
        myPlayer.setNbClassicBomb(p.getNbClassicBomb());
        myPlayer.setNbMine(p.getNbMine());
        if (myPlayer.getNbRemoteBomb() > p.getNbRemoteBomb()) {
            myPlayer.setNbRemoteBomb(p.getNbRemoteBomb());
            myPlayer.setArmedRemoteBomb(true);
        } else {
            myPlayer.setNbRemoteBomb(p.getNbRemoteBomb());
        }
        myPlayer.setImpactDist(p.getImpactDist());
        myPlayer.setInvincible(p.getInvincible());
        if (myPlayer.getInvincible()) {
            myPlayer.setNbMoveInvincible(p.getNbMoveInvincible());
        }
    }

    /**
     * Méthode qui gère la pose d'une bombe
     * 
     * @param ab : l'objet qui contient les informations de la bombe
     */
    public void setABomb(AttackNewBomb anb) {
        int x = Integer.parseInt(anb.getPos().split(",")[0]);
        int y = Integer.parseInt(anb.getPos().split(",")[1]);
        plateau[x][y].setABombe(true);
        switch (anb.getType()) {
            case "classic":
                plateau[x][y].setItem(new Item("B"));
                break;
            case "remote":
                plateau[x][y].setItem(new Item("R"));
                break;
            case "mine":
                plateau[x][y].setItem(new Item("M"));
                break;
            default:
                System.out.println("Erreur dans le type de bombe");
                break;
        }
        mesMAJ.add(new Point(x, y));
    }

    /*
     * Méthode qui gère l'explosion de la bombe
     * 
     * @param ae : l'objet qui contient les informations de l'explosion
     */
    public void explose(AttackExplose ae) {
        int x = Integer.parseInt(ae.getPos().split(",")[0]);
        int y = Integer.parseInt(ae.getPos().split(",")[1]);
        plateau[x][y].setABombe(false);
        plateau[x][y].setItem(null);
        if (plateau[x][y].getCarac().equals("@")) {
            plateau[x][y].setCarac("@");
        } else if (plateau[x][y].getCarac().equals("$")) {
            plateau[x][y].setCarac("$");
        } else {
            plateau[x][y].setCarac(" ");
        }
        mesMAJ.add(new Point(x, y));
        for (int i =0; i<ae.getCasesModifies().size(); i++) {
            x = ae.getCasesModifies().get(i).getX();
            y = ae.getCasesModifies().get(i).getY();
            if (ae.getCasesModifies().get(i).getCarac().equals(" ")) {
                plateau[x][y].setCarac(" ");
            } else {
                plateau[x][y].setCarac("I");
                plateau[x][y].setAItem(true);
                plateau[x][y].setItem(new Item("I"));
            }
            mesMAJ.add(new Point(x, y));
        }
    }

    /**
     * Méthode qui gère la mise à jour du nombre de bombes du joueur
     * 
     * @param nbBombe : le nombre de bombes
     */
    public void updateMyPlayerBomb(int nbBombe) {
        myPlayer.setNbClassicBomb(nbBombe);
    }

    /**
     * Méthode qui teste si le joueur est mort
     * 
     * @return : vrai si c'est le cas, faux sinon
     */
    public boolean finDePartie() {
        return getPlayerByName(myName).getLife() <= 0;
    }

    /**
     * Méthode qui affiche la carte.
     * Pour la vueTexte
     */
    public String toString() {
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < maxi; i++) {
            for (int j = 0; j < maxj; j++) {
                sb.append(plateau[i][j].toString());
            }
            sb.append("\n");
        }
        return sb.toString();
    }
}
