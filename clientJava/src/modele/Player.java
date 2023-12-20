package modele;

/**
 * Player : Objet representant un joueur du plateau
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class Player {
    public static final String carac = "@";
    public static final int MAX_PV = 100;

    private String name;
    private String pos;
    private int life;
    private int speed;
    private int nbClassicBomb;
    private int nbRemoteBomb;
    private int nbMine;
    private int impactDist;
    private boolean invincible;
    private int nbMoveInvincible;
    private boolean armedRemoteBomb;

    private int x;
    private int y;
    private int direction;

    public Player() {
    }

    public Player(String name, String pos) {
        this.name = name;
        this.pos = pos;
    }

    public Player(int life, int speed, int nbClassicBomb, int nbRemoteBomb, int nbMine, int impactDist,
            boolean invincible) {
        this.life = life;
        this.speed = speed;
        this.nbClassicBomb = nbClassicBomb;
        this.nbRemoteBomb = nbRemoteBomb;
        this.nbMine = nbMine;
        this.impactDist = impactDist;
        this.invincible = invincible;
    }

    /** Constructeur de la classe Robot */
    public Player(int x, int y) {
        this.x = x;
        this.y = y;
    }

    /** Les accesseurs */
    public int getDirection() {
        return direction;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public String getName() {
        return name;
    }

    public String getPos() {
        return pos;
    }

    public int getLife() {
        return life;
    }

    public int getSpeed() {
        return speed;
    }

    public int getNbClassicBomb() {
        return nbClassicBomb;
    }

    public int getNbRemoteBomb() {
        return nbRemoteBomb;
    }

    public int getNbMine() {
        return nbMine;
    }

    public int getImpactDist() {
        return impactDist;
    }

    public boolean getInvincible() {
        return invincible;
    }

    public int getNbMoveInvincible() {
        return nbMoveInvincible;
    }

    /**
     * Méthodes permettant de changer les valeur de direction,x et y
     */
    public void setDirection(int d) {
        direction = d;
    }

    public void setX(int i) {
        x += i;
    }

    public void setY(int i) {
        y += i;
    }

    public void setName(String n) {
        name = n;
    }

    public void setPos(String p) {
        pos = p;
    }

    public void setLife(int l) {
        life = l;
    }

    public void setSpeed(int s) {
        speed = s;
    }

    public void setNbClassicBomb(int n) {
        nbClassicBomb = n;
    }

    public void setNbRemoteBomb(int n) {
        nbRemoteBomb = n;
    }

    public void setNbMine(int n) {
        nbMine = n;
    }

    public void setImpactDist(int i) {
        impactDist = i;
    }

    public void setInvincible(boolean b) {
        invincible = b;
    }

    public void setArmedRemoteBomb(boolean b) {
        armedRemoteBomb = b;
    }

    public void setNbMoveInvincible(int i) {
        nbMoveInvincible = i;
    }

    public boolean getArmedRemoteBomb() {
        return armedRemoteBomb;
    }

    /** L'affichage */
    public String toString() {
        return carac;
    }

}