package dto;

/**
 * Case : Objet représentant une case de la map
 * 
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

public class Case {
    private int x;
    private int y;
    private String carac;

    public Case(int x, int y, String carac) {
        this.x = x;
        this.y = y;
        this.carac = carac;
    }

    public int getX() {
        return x;
    }

    public int getY() {
        return y;
    }

    public String getCarac() {
        return carac;
    }

    public void setX(int x) {
        this.x = x;
    }

    public void setY(int y) {
        this.y = y;
    }

    public void setType(String carac) {
        this.carac = carac;
    }

    public String toString() {
        return "Case : x = " + x + " | y = " + y + " | carac = " + carac;
    }

}
