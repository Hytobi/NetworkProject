package modele;
/**
 * Classe mère de Mur,Sol,Vide et Destination qui reste fixe le long du programme
 * @author PLOUVIN Patrice
 */
public class Indeplacable{
    private String carac;
    private Item item;
    private boolean aJoueur = false;
    private boolean aItem = false;
    private boolean aBombe = false;

    /**Constructeur de la classe indéplaçable */
    public Indeplacable(String carac){
        this.carac = carac;
    }

    /**Méthode permettant l'accès au carac
     * @return carac
     */
    public String getCarac(){
        return carac;
    }

    /**Méthode qui return vrai si le joueur est sur cette casse, faux sinon
     * @return aJoueur
     */
    //Pas utilisé dans le programme
    public boolean aJoueur(){
        return aJoueur;
    }

    /**Méthode qui return vrai si la caisse est sur cette casse, faux sinon
     * @return aCaisse
     */
    public boolean getAItem(){
        return aItem;
    }

    /**Méthode qui return vrai si la bombe est sur cette casse, faux sinon
     * @return aBombe
     */
    public boolean getABombe(){
        return aBombe;
    }

    /**Méthode qui permet de changer la valeur du carac
     * @param c : la nouvelle valeur de carac
     */
    public void setCarac(String c){
        carac = c;
    }

    /**Méthode qui permet de changer la valeur de aJoueur
     * @param b : boolean
     */
    public void setAJoueur(boolean b){
        aJoueur = b;
    }

    /**Méthode qui permet de changer la valeur de aCaisse
     * @param b : boolean
     */
    public void setAItem(boolean b){
        aItem = b;
    }

    /**Méthode qui permet de changer la valeur de aBombe
     * @param b : boolean
     */
    public void setABombe(boolean b){
        aBombe = b;
    }

    public Item getItem(){
        return item;
    }

    public void setItem(Item i){
        item = i;
    }

    /**Méthode qui affiche le carac selon aCaisse et aJoueur
     */
    public String toString(){
        if ((carac == ".") && aItem) return "*";
        if ((carac == ".") && aJoueur) return "+";
        return carac;
    }

}