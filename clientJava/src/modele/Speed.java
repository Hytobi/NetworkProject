package modele;

public class Speed extends Indeplacable{
    private static final String SPEED_UP = "S";
    private static final String SPEED_DOWN = "s";

    public Speed(int nb_alea) {
        super((nb_alea%2 == 0) ? SPEED_UP : SPEED_DOWN);
    }

}
