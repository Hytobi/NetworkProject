package dto;

public class ResponseApi {
    private String action;
    private String statut;
    private String message;

    public String getAction() {
        return action;
    }

    public String getStatut() {
        return statut;
    }

    public String getMessage() {
        return message;
    }

    public void setAction(String action) {
        this.action = action;
    }

    public void setStatut(String statut) {
        this.statut = statut;
    }

    public void setMessage(String message) {
        this.message = message;
    }


    public String toString(){
        StringBuffer sb = new StringBuffer();
        sb.append("{\"action\":\"");
        sb.append(action);
        sb.append("\",\"statut\":\"");
        sb.append(statut);
        sb.append("\",\"message\":\"");
        sb.append(message);
        sb.append("}");
        return sb.toString();
    }
}
