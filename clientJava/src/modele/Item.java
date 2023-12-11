package modele;

public class Item {
    private static final String[] ITEMS= {"classicBomb", "remoteBomb", "mine", "impactUp", "speedUp", "impactDown", "speedDown", "lifeMax", "invincible"};
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

    public String randomItem(){
        int i = (int)(Math.random() * ITEMS.length);
        return ITEMS[i];
    }
}