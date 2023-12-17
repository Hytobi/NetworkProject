package api;

/**
 * JsonJouer: Contient les messages à envoyer au serveur qui sont en rapport
 * avec une partie
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

public class JsonJouer {

    /**
     * Evoie le json pour se déplacer
     * 
     * @param move [up, down, left, right]
     * @return le message à envoyer au serveur
     */
    public static String postPlayerMove(String move) {
        return "POST player/move\n {\"move\":\"" + move + "\"}";
    }

    /**
     * Evoie le json pour poser une bombe
     * 
     * @param pos  de la forme "x,y" : la position de la bombe
     * @param type [classicBomb, remoteBomb, mine]
     * @return le message à envoyer au serveur
     */
    public static String postAttackBomb(String pos, String type) {
        return "POST player/attack\n {\"pos\":\"" + pos + "\",\"type\":\"" + type + "\"}";
    }

    /**
     * Evoie le json pour déclancher une remote bombe
     * 
     * @return le message à envoyer au serveur
     */
    public static String postAttackRemoteGo() {
        return "POST attack/remote/go\n";
    }

    /**
     * @param type le type de l’objet parmi les objets possibles : classicBomb,
     *             remoteBomb, mine,
     *             impactUp, impactDown, speedUp, speedDown, lifeMax et invincible
     * @return le message à envoyer au serveur
     */
    public static String postObjectNew(String type) {
        return "POST object/new\n {\"type\":\"" + type + "\"}";
    }
}
