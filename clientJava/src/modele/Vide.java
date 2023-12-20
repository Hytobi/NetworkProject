package modele;

/**
 * Vide : Objet representant un vide sur la map
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

public class Vide extends Indeplacable {
    public static final String carac = "/";

    /** Constructeur de la classe Vide */
    public Vide() {
        super(carac);
    }

}