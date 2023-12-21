package api;

import java.io.File;

/**
 * MapperRes: classe utilitaire pour convertir des objets en json et inversement
 * 
 * @author Hana DELCOURT, Patrice PLOUVIN
 * 
 */

import com.fasterxml.jackson.databind.ObjectMapper;
import dto.ConfigTouch;

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

    /**
     * Ecrit un json dans un fichier local-config.json
     * 
     * @param json le json à écrire
     */
    public static void writeJson(ConfigTouch ct) {
        try {
            mapper.writeValue(new File("local-config.json"), ct);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Lit un json dans un fichier local-config.json
     * 
     * @return le json lu
     */
    public static <T> T readJson(Class<T> clazz) {
        if (!new File("local-config.json").exists()) {
            return null;
        }
        try {
            return mapper.readValue(new File("local-config.json"), clazz);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
