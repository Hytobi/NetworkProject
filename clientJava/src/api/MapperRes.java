package api;

/**
 * MapperRes: classe utilitaire pour convertir des objets en json et inversement
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

import com.fasterxml.jackson.databind.ObjectMapper;

public class MapperRes {

    private static final ObjectMapper mapper = new ObjectMapper();

    /**
     * Convertit un json en objet
     * 
     * @param <T>   le type de l’objet
     * @param json  le json à convertir
     * @param clazz la classe de l’objet
     * @return l’objet
     */
    public static <T> T fromJson(String json, Class<T> clazz) {
        try {
            return mapper.readValue(json, clazz);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Convertit un objet en json
     * 
     * @param obj l’objet à convertir
     * @return le json
     */
    public static String toJson(Object obj) {
        try {
            return mapper.writeValueAsString(obj);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
