package dto;

/**
 * ResGameList : Objet reponse de la liste des games (envoyé par le serveur)
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 */

import java.util.List;

public class ResGameList extends ResponseApi {
    private int nbGamesList;
    private List<Game> games;

    public int getNbGamesList() {
        return nbGamesList;
    }

    public List<Game> getGames() {
        return games;
    }

    public void setNbGamesList(int nbGamesList) {
        this.nbGamesList = nbGamesList;
    }

    public void setGames(List<Game> games) {
        this.games = games;
    }
}
