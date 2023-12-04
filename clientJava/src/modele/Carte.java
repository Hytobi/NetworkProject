package modele;

import java.util.*;
import java.awt.Point;

/**
 * Classe qui modélise la Carte de jeu (la map)
 * @author PLOUVIN Patrice
 */
public class Carte{
    //Argument de construction de la classe
    private int niveau;

    //Construction du tableau de jeu
    private Indeplacable[][] plateau;
    private int maxi;
    private int maxj;
    private List<String> maListe;
    private Robot robot;
    private List<Point> mesDestinations = new ArrayList<Point>();
    private List<Point> mesMAJ = new ArrayList<Point>();

    //Pour les actions retour et recommencer pour le Sokoban Graphique
    private int nbMouvement;
    private Character dernierMouvement;
    private boolean aDeplacerCaisse;
    private Point derniereCaisseBouge;
    private boolean isRetour=false;
    private int nbRestart;

    /**Constructeur de la Classe
     * @param niveau : le niveau à charger
     */
    public Carte(int niveau) throws Exception{
        //Charger une map
        Lecture map = new Lecture("map" + niveau);
        maListe = map.getMaListe();
        maxi = map.getNbLigne();
        maxj = map.getTailleLigne();
        nbRestart=-1;
        restart();
    }

    /**Toute les methodes pour acceder aux arguments private */
    public Robot getRobot(){
        return robot;
    }
    public int getMaxi(){
        return maxi;
    }
    public int getMaxj(){
        return maxj;
    }
    public List<Point> getMesMAJ(){
        return mesMAJ;
    }
    public int getNbMouvement(){
        return nbMouvement;
    }
    public Character getDernierMouvement(){
        return dernierMouvement;
    }
    public List<Point> getMesDestinations() {
        return mesDestinations;
    }
    public int getNbRestart(){
        return nbRestart;
    }
    public Indeplacable[][] getPlateau(){
        return plateau;
    }

    /**Méthode qui vide la liste des mises à jour*/
    public void ViderMesMAJ(){
        mesMAJ.clear();
    }

    /**Méthode qui permet dans le Sokoban graphique de faire le mouvement inverse
     * du dernier mouvement effectuer
     * */
    public void faireDernierMouvement(){
        if (dernierMouvement != null){
            isRetour=true;
            //On les garde dans un autre repère, car ils vont changer avec les nouvelles actions
            boolean b = aDeplacerCaisse;
            char d = dernierMouvement;
            Point p = derniereCaisseBouge;
            //Direction inverse et on garde la meme direction du robot
            robotSeDeplace(counteur(d));
            robot.setDirection((robot.getDirection()+2)%4);
            //Si le robot avait déplacé une caisse on la remet ou elle était
            if (b) {
                String c = plateau[(int)derniereCaisseBouge.getX()][(int)derniereCaisseBouge.getY()].getCarac();
                //Si c'était une caisse sur une destination on remet une destination
                if (c=="*")plateau[(int)derniereCaisseBouge.getX()][(int)derniereCaisseBouge.getY()].setCarac(".");
                //Sinon c'est simplement une caisse et on met un sol
                else if (c=="$") plateau[(int)derniereCaisseBouge.getX()][(int)derniereCaisseBouge.getY()].setCarac(" ");
                //Dans tout les cas la caisse n'est pu à cette position du plateau
                plateau[(int)derniereCaisseBouge.getX()][(int)derniereCaisseBouge.getY()].setACaisse(false);
                //Maintenant on la déplace
                deplaceCaisse(d);
                //Et on la met a jour
                mesMAJ.add(p);
            }
            dernierMouvement=null;
            isRetour=false;
        }
    }


    /**
     * Méthode qui met a jour le X ou le Y du robot
     * @param c : la direction du robot
     */
    public void direction(char c){
        //On incrémente le nombre de mouvements
        nbMouvement++;
        //Aller vers le haut
        if (c == 'z'){
            robot.setX(-1);
            robot.setDirection(0);
        }
        //Aller vers la gauche
        else if (c == 'q'){
            robot.setY(-1);
            robot.setDirection(1);
        }
        //Aller vers le bas
        else if (c == 's'){
            robot.setX(1);
            robot.setDirection(2);
        }
        //Aller vers la droite
        else if (c == 'd'){
            robot.setY(1);
            robot.setDirection(3);
        }
    }

    /**
     * Méthode qui fait le mouvement inverse que celui entré par le joueur
     * @param c : la direction du robot
     * @return : La direction opposée
     */
    public char counteur(char c){
        //Le counteur utilise direction pour faire le mouvement inverse, donc on décrémente de 2
        nbMouvement-=2;
        if (c=='z') return 's';
        if (c=='q') return 'd';
        if (c=='s') return 'z';
        return 'q';
    }

    /**Méthode qui deplace la caisse pour éviter la duplication de code
     * @param x : action sur la ligne
     * @param y : action sur la colonne
     * @return faux si ce n'est pas possible, vrai sinon
     */
    public boolean bougerLaCaisseEn(int x,int y){
        String carac = plateau[robot.getX()+x][robot.getY()+y].getCarac();
        if (!((carac !="$") && (carac !="#") && (carac !="*"))) return false ;
        //Si on la pousse sur une destination
        if (carac == ".") plateau[robot.getX()+x][robot.getY()+y].setCarac("*");
        //Sinon elle est sur un sol
        else plateau[robot.getX()+x][robot.getY()+y].setCarac("$");
        //La caisse a été bougée et on met à jour
        plateau[robot.getX()+x][robot.getY()+y].setACaisse(true);
        mesMAJ.add(new Point(robot.getX()+x,robot.getY()+y));
        derniereCaisseBouge = new Point(robot.getX()+x,robot.getY()+y);
        return true;
    }

    /**Méthode qui déplace la caisse si le robot la pousse
     * @param c: un character associé à une direction
     * @retourn vrai si le mouvement a été effectué, faux sinon
     * */
    public boolean deplaceCaisse(char c){
        if (c=='z') return bougerLaCaisseEn(-1,0);
        if (c=='q') return bougerLaCaisseEn(0,-1);
        if (c=='s') return bougerLaCaisseEn(1,0);
        if (c=='d') return bougerLaCaisseEn(0,1);
        return false;
    }

    /**
     * Méthode qui teste si le robot peut se deplacer et le déplace
     * @param c : La direction du robot
     * @return : vrai si le robot a pu se deplacer, faux sinon
     */
    public boolean robotSeDeplace(char c){
        //Avant de déplacer le robot
        aDeplacerCaisse=false;
        boolean estSurDestination = false;
        //Action sur là où est le robot avant le déplacement
        plateau[robot.getX()][robot.getY()].setAJoueur(false);
        String inter = plateau[robot.getX()][robot.getY()].getCarac();
        //Si le robot est sur un sol
        if (inter =="@") plateau[robot.getX()][robot.getY()].setCarac(" ");
        //Sinon il est sur une destination
        else if (inter == "+") {
            plateau[robot.getX()][robot.getY()].setCarac(".");
            estSurDestination = true;
        }
        //On met a jour
        mesMAJ.add(new Point(robot.getX(),robot.getY()));

        //On déplace le robot et mise à jour
        direction(c);
        mesMAJ.add(new Point(robot.getX(),robot.getY()));

        //On regarde ce qu'il y a au meme endroit que le robot
        inter = plateau[robot.getX()][robot.getY()].getCarac();
        //Si c'est un mur ou un vide (jamais trop prudent)
        if (inter == "#" || inter=="/" ){
            //Alors on remet le joueur à sa place en faisant attention à si c'était une destination
            direction(counteur(c));
            plateau[robot.getX()][robot.getY()].setAJoueur(true);
            if (estSurDestination) plateau[robot.getX()][robot.getY()].setCarac("+");
            else plateau[robot.getX()][robot.getY()].setCarac("@");
            //Rien se s'est passé alors on ne met rien à jour
            mesMAJ.clear();
            return false;
        }
        //Si c'est une destination @ devient +
        else if ( inter == "."){
            plateau[robot.getX()][robot.getY()].setAJoueur(true);
            plateau[robot.getX()][robot.getY()].setCarac("+");
        }
        //Sinon on est sur une caisse
        else {
            if (inter == "$" || inter=="*"){
                //On deplace la caisse dans la meme direction que le robot
                aDeplacerCaisse = deplaceCaisse(c);
                //Si la caisse a pu être déplacé
                if (aDeplacerCaisse){
                    //La ou il y a le robot il ne peut y avoir de caisse
                    plateau[robot.getX()][robot.getY()].setACaisse(false);
                    //Suite à une action retour la caisse est sur une destination
                    //Action similaire à tirer la caisse
                    if ((inter =="*")&&(isRetour)){
                        plateau[robot.getX()][robot.getY()].setAJoueur(false);
                        plateau[robot.getX()][robot.getY()].setCarac(".");
                        return true;
                    } else if ((inter =="*")){
                        //Sinon le robot arrive sur la destination
                        plateau[robot.getX()][robot.getY()].setAJoueur(true);
                        plateau[robot.getX()][robot.getY()].setCarac("+");
                        return true;
                    }//Si on n'a pas pu la déplacer
                } else {
                    //On remet tout en place
                    direction(counteur(c));
                    plateau[robot.getX()][robot.getY()].setAJoueur(true);
                    if (estSurDestination) plateau[robot.getX()][robot.getY()].setCarac("+");
                    else plateau[robot.getX()][robot.getY()].setCarac("@");
                    //Et pas de mises à jour à faire
                    mesMAJ.clear();
                    return false;
                }
            }
            plateau[robot.getX()][robot.getY()].setCarac("@");
            plateau[robot.getX()][robot.getY()].setAJoueur(true);
        }
        dernierMouvement = c ;
        return true;
    }

    /**
     * Méthode qui teste si tout les caisses sont sur une destination
     * @return : vrai si c'est le cas, faux sinon
     */
    public boolean finDePartie(){
        //À voir si je laisse, mais pas tres utile (ajouté pour le bug de passage de niveau)
        if (mesDestinations.isEmpty()) return false;

        for (Point destination : mesDestinations){
            //Pour chaque destination on regarde s'il y a une caisse
            if (!plateau[(int) destination.getX()][(int) destination.getY()].aCaisse()){
                //Si y'en a pas on return faux
                return false;
            }
        }
        //Si on sort, c'est que toutes les caisses sont sur des destinations
        return true;
    }

    /**Méthode qui relance une partie*/
    public void restart(){
        //On met tout à 0
        mesDestinations.clear();
        dernierMouvement = null;
        nbMouvement =0;
        plateau = new Indeplacable[maxi][maxj];
        nbRestart++;

        int i,j;
        String temp;
        char carac;

        //Construction du tableau de jeu
        for (i=0;i<maxi;i++){
            temp = maListe.get(i);
            for (j=0;j<maxj;j++){
                carac = temp.charAt(j);
                if (carac == '#') plateau[i][j] = new Mur();
                else if (carac == '/') plateau[i][j] = new Vide();
                else if (carac == '.') {
                    plateau[i][j] = new Destination();
                    mesDestinations.add(new Point(i,j));
                }
                //Sinon tout le reste est un sol avec des choses posées dessus
                else if (carac == '@'){
                    plateau[i][j] = new Sol("@");
                    plateau[i][j].setAJoueur(true);
                    robot = new Robot(i,j);
                }
                else if (carac == '$'){
                    plateau[i][j] = new Sol("$");
                    plateau[i][j].setACaisse(true);
                }
                else if (carac == '*'){
                    plateau[i][j] = new Sol("*");
                    plateau[i][j].setACaisse(true);
                    mesDestinations.add(new Point(i,j));
                }
                else if (carac == '+'){
                    plateau[i][j] = new Sol("+");
                    plateau[i][j].setAJoueur(true);
                    robot = new Robot(i,j);
                    mesDestinations.add(new Point(i,j));
                }
                else plateau[i][j] = new Sol(" ");
            }
        }
    }

    /**
     * Méthode qui affiche la carte.
     * Pour la vueTexte
     */
    public String toString(){
        StringBuffer sb = new StringBuffer();
        for (int i=0;i<maxi;i++){
            for (int j=0;j<maxj;j++){
                sb.append(plateau[i][j].toString());
            }
            sb.append("\n");
        }
        return sb.toString();
    }
}
