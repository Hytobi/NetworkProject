package api;

public class JsonJouer {
    
    public String postPlayerMove(String move){
        return "POST player/move\n {\"move\":\"" + move + "\"}";
    }

    public String postAttackBomb(String pos, String type){
        return "POST player/attack\n {\"pos\":\"" + pos + "\",\"type\":\"" + type + "\"}";
    }

    public String postAttackRemoteGo(){
        return "POST attack/remote/go\n";
    }

    /**
     * @param type le type de l’objet parmi les objets possibles : classicBomb, remoteBomb, mine,
impactUp, impactDown, speedUp, speedDown, lifeMax et invincible
     * @return
     */
    public String postObjectNew(String type){
        return "POST object/new\n {\"type\":\"" + type + "\"}";
    }
}
