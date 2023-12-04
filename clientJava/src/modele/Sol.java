package modele;

/**
 * Classe modélisant les Sol de la map, le Robot et les caisses se séplace dessus.
 * @author PLOUVIN Patrice
 */
public class Sol extends Indeplacable{
    private String carac;

    /**Constructeur de la classe Sol */
    public Sol(String carac){
        super(carac);
    }
}