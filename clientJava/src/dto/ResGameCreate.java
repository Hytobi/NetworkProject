package dto;

import modele.Player;

public class ResGameCreate extends ResponseApi {
    private int nbPlayer;
    private String mapId;
    private String startPos;
    private Player player;

    public int getNbPlayer() {
        return nbPlayer;
    }

    public String getMapId() {
        return mapId;
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

    public void setMapId(String mapId) {
        this.mapId = mapId;
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
        sb.append(mapId);
        sb.append("\",\"startPos\":\"");
        sb.append(startPos);
        sb.append("\",\"player\":");
        sb.append(player.toString());
        sb.append("}");
        return sb.toString();
    }
}
