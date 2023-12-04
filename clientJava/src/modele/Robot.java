package modele;

public class Robot{
    public static final String carac = "@";

    private int x;
    private int y;
    private int direction;

    /**Constructeur de la classe Robot */
    public Robot(int x, int y){
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

    /**L'affichage */
    public String toString(){
        return carac;
    }

}