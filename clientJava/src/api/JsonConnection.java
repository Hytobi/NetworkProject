package api;

import dto.Game;

/**
 * JsonConnection : Contient les messages à envoyer au serveur qui ne sont pas
 * en rapport avec une partie
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class JsonConnection {
    public static final String RES_ATTENDU = "hello i'm a bomberstudent server";

    /**
     * Message envoyé par le client pour rechercher les serveurs
     * 
     * @return le message à envoyer au serveur
     */
    public static String msgSearch() {
        return "looking for bomberstudent servers";
    }

    /**
     * Message envoyé par le client pour se reconnecter au serveur si une
     * déconnection est survenue
     * 
     * @return le message à envoyer au serveur
     */
    public static String msgConnect() {
        return "I was connect to you";
    }

    /**
     * Demmande la liste des maps disponibles
     * 
     * @return le message à envoyer au serveur
     */
    public static String getMapList() {
        return "GET Maps/list";
    }

    /**
     * Demmande la liste des parties disponibles
     * 
     * @return le message à envoyer au serveur
     */
    public static String getGameList() {
        return "GET game/list";
    }

    /**
     * Demmande de création de partie
     * 
     * @param game information necessaire pour créer une partie (nomGame, mapId)
     * @return le message à envoyer au serveur
     */
    public static String postGameCreate(Game game) {
        return "POST game/create\n" + game.toString();
    }

    /**
     * Demmande de rejoindre une partie
     * 
     * @param game information necessaire pour rejoindre une partie (nomGame)
     * @return le message à envoyer au serveur
     */
    public static String postGameJoin(Game game) {
        return "POST game/join\n" + game.toString();
    }

}
