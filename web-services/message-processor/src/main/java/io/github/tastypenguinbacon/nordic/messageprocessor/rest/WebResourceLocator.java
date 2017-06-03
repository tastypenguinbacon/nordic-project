package io.github.tastypenguinbacon.nordic.messageprocessor.rest;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.messageprocessor.logger.SLF4JLogger;
import javaslang.control.Option;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import java.net.URI;

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
    public Option<URI> locate(String serviceName) {
        return Option.of(client.target(System.getProperty("resource-locator")).request().get().readEntity(JsonNode.class))
                .map(json -> json.get(serviceName))
                .flatMap(Option::of).map(JsonNode::asText)
                .flatMap(Option::of).map(URI::create);
    }
}
