package io.github.tastypenguinbacon.nordic.scriptregisterer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import java.io.IOException;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by pingwin on 08.06.17.
 */
public class Main extends TimerTask {
    private final GroovyScriptRegisterer registerer;
    private final ClassPathScanner scanner;
    private final Logger logger = LoggerFactory.getLogger(Main.class);

    public Main() {
        scanner = new ClassPathScanner();
        Client client = ClientBuilder.newClient();
        String resourceLocator = System.getProperty("resourceLocator");
        Objects.requireNonNull(resourceLocator);
        registerer = new GroovyScriptRegisterer(client, resourceLocator);
    }

    public static void main(String[] args) throws IOException {
        Timer timer = new Timer();
        System.getProperty("resourceLocator");
        long timeout = Integer.parseInt(System.getProperty("locatorTimeout"));
        timer.schedule(new Main(), 0, timeout);
    }

    @Override
    public void run() {

        try {
            logger.info("Registering scripts");
            scanner.getResourceFiles("./")
                    .stream()
                    .filter(fileName -> fileName.endsWith(".groovy"))
                    .forEach(filename -> registerer.registerScript(filename, scanner.getResourceAsString(filename)));
        } catch (Exception e) {
            logger.warn("Exception occured", e);
        }
    }
}
