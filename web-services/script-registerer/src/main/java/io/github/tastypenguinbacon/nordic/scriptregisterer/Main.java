package io.github.tastypenguinbacon.nordic.scriptregisterer;

import javax.ws.rs.client.ClientBuilder;
import java.io.IOException;

/**
 * Created by pingwin on 08.06.17.
 */
public class Main {
    public static void main(String[] args) throws IOException {
        GroovyScriptRegisterer registerer = new GroovyScriptRegisterer(ClientBuilder.newClient());
        ClassPathScanner classPathScanner = new ClassPathScanner();
        System.out.println(classPathScanner.getResourceFiles("./"));
    }
}
