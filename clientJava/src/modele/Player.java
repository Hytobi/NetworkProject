package modele;

public class Player{
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

    private int x;
    private int y;
    private int direction;

    /**Constructeur de la classe Robot */
    public Player(int x, int y){
        this.x = x;
        this.y = y;
    }

    /**Les accesseurs */
    public int getDirection(){
        return direction;
    }
    public int getX(){
        return x;
    }
    public int getY(){
        return y;
    }

    public String getName(){
        return name;
    }

    public String getPos(){
        return pos;
    }

    public int getLife(){
        return life;
    }

    public int getSpeed(){
        return speed;
    }

    public int getNbClassicBomb(){
        return nbClassicBomb;
    }

    public int getNbRemoteBomb(){
        return nbRemoteBomb;
    }

    public int getNbMine(){
        return nbMine;
    }

    public int getImpactDist(){
        return impactDist;
    }

    public boolean getInvincible(){
        return invincible;
    }

    /**Méthodes permettant de changer les valeur de direction,x et y
     */
    public void setDirection(int d){
        direction = d;
    }
    public void setX(int i){
        x+=i;
    }
    public void setY(int i){
        y+=i;
    }

    public void setName(String n){
        name = n;
    }

    public void setPos(String p){
        pos = p;
    }

    public void setLife(int l){
        life = l;
    }

    public void setSpeed(int s){
        speed = s;
    }

    public void setNbClassicBomb(int n){
        nbClassicBomb = n;
    }

    public void setNbRemoteBomb(int n){
        nbRemoteBomb = n;
    }

    public void setNbMine(int n){
        nbMine = n;
    }

    public void setImpactDist(int i){
        impactDist = i;
    }

    public void setInvincible(boolean b){
        invincible = b;
    }

    /**L'affichage */
    public String toString(){
        return carac;
    }

}