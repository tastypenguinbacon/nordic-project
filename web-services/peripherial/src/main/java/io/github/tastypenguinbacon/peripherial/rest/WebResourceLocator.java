package io.github.tastypenguinbacon.peripherial.rest;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.peripherial.logger.SLF4JLogger;

import javax.inject.Inject;
import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import java.net.URI;
import org.slf4j.Logger;

/**
 * Created by pingwin on 02.06.17.
 */
public class WebResourceLocator implements ResourceLocator {
    private final Client client;
    private final Logger logger;

    @Inject
    public WebResourceLocator(@SLF4JLogger Logger logger) {
        this.client = ClientBuilder.newClient();
        this.logger = logger;
    }

    @Override
    public URI locate(String serviceName) {
        JsonNode locations = client.target(System.getProperty("resource-locator")).request().get().readEntity(JsonNode.class);
        return URI.create(locations.get(serviceName).asText());
    }
}
