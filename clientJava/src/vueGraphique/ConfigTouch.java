package vueGraphique;

public class ConfigTouch {
    private Character dirRight;
    private Character dirLeft;
    private Character dirUp;
    private Character dirDown;
    private Character putRemoteBomb;
    private Character putMine;
    private Character putClassicBomb;
    private Character explodeRemoteBomb;

    public ConfigTouch() {
        dirRight = 'd';
        dirLeft = 'q';
        dirUp = 'z';
        dirDown = 's';
        putRemoteBomb = 'r';
        putMine = 'f';
        putClassicBomb = 'c';
        explodeRemoteBomb = 'e';
    }

    public Character getDirRight() {
        return dirRight;
    }

    public Character getDirLeft() {
        return dirLeft;
    }

    public Character getDirUp() {
        return dirUp;
    }

    public Character getDirDown() {
        return dirDown;
    }

    public Character getPutRemoteBomb() {
        return putRemoteBomb;
    }

    public Character getPutMine() {
        return putMine;
    }

    public Character getPutClassicBomb() {
        return putClassicBomb;
    }

    public Character getExplodeRemoteBomb() {
        return explodeRemoteBomb;
    }

    public void setDirRight(Character dirRight) {
        this.dirRight = dirRight;
    }

    public void setDirLeft(Character dirLeft) {
        this.dirLeft = dirLeft;
    }

    public void setDirUp(Character dirUp) {
        this.dirUp = dirUp;
    }

    public void setDirDown(Character dirDown) {
        this.dirDown = dirDown;
    }

    public void setPutRemoteBomb(Character putRemoteBomb) {
        this.putRemoteBomb = putRemoteBomb;
    }

    public void setPutMine(Character putMine) {
        this.putMine = putMine;
    }

    public void setPutClassicBomb(Character putClassicBomb) {
        this.putClassicBomb = putClassicBomb;
    }

    public void setExplodeRemoteBomb(Character explodeRemoteBomb) {
        this.explodeRemoteBomb = explodeRemoteBomb;
    }

    public String toString() {
        return "Config : dirRight = " + dirRight + " | dirLeft = " + dirLeft + " | dirUp = " + dirUp + " | dirDown = "
                + dirDown + " | putRemoteBomb = " + putRemoteBomb + " | putMine = " + putMine + " | putClassicBomb = "
                + putClassicBomb + " | explodeRemoteBomb = " + explodeRemoteBomb;
    }
}
