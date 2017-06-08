package io.github.tastypenguinbacon.nordic.common.rest;

import com.fasterxml.jackson.databind.JsonNode;
import javaslang.control.Option;

import javax.inject.Inject;
import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import java.net.URI;

/**
 * Created by pingwin on 02.06.17.
 */
public class WebResourceLocator implements ResourceLocator {
    private final Client client;

    @Inject
    public WebResourceLocator() {
        this.client = ClientBuilder.newClient();
    }

    @Override
    public Option<URI> locate(String serviceName) {
        return Option.of(client.target(System.getProperty("resource-locator")).request().get().readEntity(JsonNode.class))
                .map(json -> json.get(serviceName))
                .flatMap(Option::of).map(JsonNode::asText)
                .flatMap(Option::of).map(URI::create);
    }
}
