package io.github.tastypenguinbacon.nordic.scriptregisterer;

import javax.ws.rs.client.Client;

/**
 * Created by pingwin on 08.06.17.
 */
public class GroovyScriptRegisterer {
    private final Client client;

    public GroovyScriptRegisterer(Client client) {
        this.client = client;
    }

    public void registerScript(String script, String name) {

    }
}
