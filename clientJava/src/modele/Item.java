package modele;

public class Item {
    private String carac;

    public Item(String carac){
        this.carac = carac;
    }

    public String getCarac(){
        return carac;
    }

    public void setCarac(String c){
        carac = c;
    }
}
