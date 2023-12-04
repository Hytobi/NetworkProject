package modele;

public class Impact extends Indeplacable {
    private static final String IMPACT_UP = "I";
    private static final String IMPACT_DOWN = "i";

    public Impact(int nb_alea) {
        super((nb_alea%2 == 0) ? IMPACT_UP : IMPACT_DOWN);
    }
}
