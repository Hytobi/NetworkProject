package dto;

/**
 * ConfigTouch : Objet représentant la configuration des touches
 * 
 * @author : Hana DELCOURT, Patrice PLOUVIN
 */

import com.fasterxml.jackson.annotation.JsonProperty;

public class ConfigTouch {
    private static final String DEFAULT_DIR_RIGHT = "d";
    private static final String DEFAULT_DIR_LEFT = "q";
    private static final String DEFAULT_DIR_UP = "z";
    private static final String DEFAULT_DIR_DOWN = "s";
    private static final String DEFAULT_PUT_REMOTE_BOMB = "r";
    private static final String DEFAULT_PUT_MINE = "f";
    private static final String DEFAULT_PUT_CLASSIC_BOMB = "c";
    private static final String DEFAULT_EXPLODE_REMOTE_BOMB = "e";

    @JsonProperty("dirRight")
    private String dirRight;

    @JsonProperty("dirLeft")
    private String dirLeft;

    @JsonProperty("dirUp")
    private String dirUp;

    @JsonProperty("dirDown")
    private String dirDown;

    @JsonProperty("putRemoteBomb")
    private String putRemoteBomb;

    @JsonProperty("putMine")
    private String putMine;

    @JsonProperty("putClassicBomb")
    private String putClassicBomb;

    @JsonProperty("explodeRemoteBomb")
    private String explodeRemoteBomb;

    public ConfigTouch(String dirRight, String dirLeft, String dirUp, String dirDown, String putRemoteBomb,
            String putMine, String putClassicBomb, String explodeRemoteBomb) {
        this.dirRight = dirRight;
        this.dirLeft = dirLeft;
        this.dirUp = dirUp;
        this.dirDown = dirDown;
        this.putRemoteBomb = putRemoteBomb;
        this.putMine = putMine;
        this.putClassicBomb = putClassicBomb;
        this.explodeRemoteBomb = explodeRemoteBomb;
    }

    public ConfigTouch() {
        dirRight = DEFAULT_DIR_RIGHT;
        dirLeft = DEFAULT_DIR_LEFT;
        dirUp = DEFAULT_DIR_UP;
        dirDown = DEFAULT_DIR_DOWN;
        putRemoteBomb = DEFAULT_PUT_REMOTE_BOMB;
        putMine = DEFAULT_PUT_MINE;
        putClassicBomb = DEFAULT_PUT_CLASSIC_BOMB;
        explodeRemoteBomb = DEFAULT_EXPLODE_REMOTE_BOMB;
    }

    public ConfigTouch(ConfigTouch configTouch) {
        dirRight = configTouch.getDirRight();
        dirLeft = configTouch.getDirLeft();
        dirUp = configTouch.getDirUp();
        dirDown = configTouch.getDirDown();
        putRemoteBomb = configTouch.getPutRemoteBomb();
        putMine = configTouch.getPutMine();
        putClassicBomb = configTouch.getPutClassicBomb();
        explodeRemoteBomb = configTouch.getExplodeRemoteBomb();
    }

    public String getDirRight() {
        return dirRight;
    }

    public String getDirLeft() {
        return dirLeft;
    }

    public String getDirUp() {
        return dirUp;
    }

    public String getDirDown() {
        return dirDown;
    }

    public String getPutRemoteBomb() {
        return putRemoteBomb;
    }

    public String getPutMine() {
        return putMine;
    }

    public String getPutClassicBomb() {
        return putClassicBomb;
    }

    public String getExplodeRemoteBomb() {
        return explodeRemoteBomb;
    }

    public void setDirRight(String dirRight) {
        this.dirRight = dirRight;
    }

    public void setDirLeft(String dirLeft) {
        this.dirLeft = dirLeft;
    }

    public void setDirUp(String dirUp) {
        this.dirUp = dirUp;
    }

    public void setDirDown(String dirDown) {
        this.dirDown = dirDown;
    }

    public void setPutRemoteBomb(String putRemoteBomb) {
        this.putRemoteBomb = putRemoteBomb;
    }

    public void setPutMine(String putMine) {
        this.putMine = putMine;
    }

    public void setPutClassicBomb(String putClassicBomb) {
        this.putClassicBomb = putClassicBomb;
    }

    public void setExplodeRemoteBomb(String explodeRemoteBomb) {
        this.explodeRemoteBomb = explodeRemoteBomb;
    }

    public String toString() {
        return "Config : dirRight = " + dirRight + " | dirLeft = " + dirLeft + " | dirUp = " + dirUp + " | dirDown = "
                + dirDown + " | putRemoteBomb = " + putRemoteBomb + " | putMine = " + putMine + " | putClassicBomb = "
                + putClassicBomb + " | explodeRemoteBomb = " + explodeRemoteBomb;
    }
}
