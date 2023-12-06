package dto;

import modele.Player;

public class ResGameCreate extends ResponseApi {
    private int nbPlayer;
    private String gameId;
    private String startPos;
    private Player player;

    public int getNbPlayer() {
        return nbPlayer;
    }

    public String getGameId() {
        return gameId;
    }

    public String getStartPos() {
        return startPos;
    }

    public Player getPlayer() {
        return player;
    }

    public void setNbPlayer(int nbPlayer) {
        this.nbPlayer = nbPlayer;
    }

    public void setGameId(String gameId) {
        this.gameId = gameId;
    }

    public void setStartPos(String startPos) {
        this.startPos = startPos;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public String toString(){
        StringBuffer sb = new StringBuffer();
        sb.append("{\"nbPlayer\":\"");
        sb.append(nbPlayer);
        sb.append("\",\"gameId\":\"");
        sb.append(gameId);
        sb.append("\",\"startPos\":\"");
        sb.append(startPos);
        sb.append("\",\"player\":");
        sb.append(player.toString());
        sb.append("}");
        return sb.toString();
    }
}
