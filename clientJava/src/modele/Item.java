package modele;

public class Item {
    //speedUp, speedDown
    private static final String[] ITEMS= {"classicBomb", "remoteBomb", "mine", "impactUp", "impactDown", "lifeMax", "invincible"};
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

    @Override
    public String toString() {
        return "Item{" +
                "carac='" + carac + '\'' +
                '}';
    }
}
