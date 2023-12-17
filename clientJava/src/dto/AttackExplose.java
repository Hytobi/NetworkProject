package dto;

/**
 * AttackExplose : Objet notifiant qu'une bombe a explosé (envoyé par le
 * serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

public class AttackExplose {
    private String pos;
    private String type;
    private int impactDist;
    private String map;

    public String getPos() {
        return pos;
    }

    public String getType() {
        return type;
    }

    public int getImpactDist() {
        return impactDist;
    }

    public String getMap() {
        return map;
    }

    public void setPos(String pos) {
        this.pos = pos;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setImpactDist(int impactDist) {
        this.impactDist = impactDist;
    }

    public void setMap(String map) {
        this.map = map;
    }

    public String toString() {
        return "AttackExplose : pos = " + pos + " | type = " + type + " | impactDist = " + impactDist + " | map = "
                + map;
    }
}
