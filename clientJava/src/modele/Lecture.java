package modele;

import java.util.*;
import java.io.*;

/**
 * Classe permettant la lecture d'un fichier texte et le garde dans une liste
 * @author PLOUVIN Patrice
 */
public class Lecture{
    private String niveau;

    private List<String> maListe = new ArrayList<String>();
    private int nbLigne=0;
    private int tailleLigne=0;

    /**Constructeur de la classe Lecture */
    public Lecture(String niveau) throws Exception {
        //On récupère le lien vers la map
        File doc = new File("map/" + niveau + ".txt");
        //On lie le fichier texte
        Scanner tableau = new Scanner(doc);
        //Tant qu'il y a une ligne suivante
        while (tableau.hasNextLine()){
            //On recupère la ligne dans ma liste
            maListe.add(tableau.nextLine());
        }
        //Et on garde les tailles
        nbLigne = maListe.size();
        tailleLigne = maListe.get(0).length();
    }

    /**Les accesseurs */
    public String getNiveau(){
        return niveau;
    }
    public List<String> getMaListe() {
        return maListe;
    }
    public int getNbLigne(){
        return nbLigne;
    }
    public int getTailleLigne(){
        return tailleLigne;
    }

    /**L'affichage */
    public String toString(){
        StringBuffer sb = new StringBuffer();
        for (String a : maListe){
            sb.append(a);
            sb.append("\n");
        }
        return sb.toString();
    }
    
}