package dto;

/**
 * AttackExplose : Objet notifiant qu'une bombe a explosé (envoyé par le
 * serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

import java.util.List;

public class AttackExplose {
    private String pos;
    private String type;
    private int impactDist;
    private List<Case> casesModifies;

    public String getPos() {
        return pos;
    }

    public String getType() {
        return type;
    }

    public int getImpactDist() {
        return impactDist;
    }

    public List<Case> getCasesModifies() {
        return casesModifies;
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

    public void setMap(List<Case> casesModifies) {
        this.casesModifies = casesModifies;
    }

    public String toString() {
        return "AttackExplose : pos = " + pos + " | type = " + type + " | impactDist = " + impactDist
                + " | casesModifies = " + casesModifies.toString();
    }
}
