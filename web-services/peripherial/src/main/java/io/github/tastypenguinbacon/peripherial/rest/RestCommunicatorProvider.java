package io.github.tastypenguinbacon.peripherial.rest;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.net.URI;
import java.util.function.Function;

/**
 * Created by pingwin on 01.06.17.
 */
public class RestCommunicatorProvider implements CommunicatorProvider {
    private final String serviceName;
    private final Client client;
    private final ResourceLocator locator;

    public RestCommunicatorProvider(String serviceName, ResourceLocator locator) {
        this.client = ClientBuilder.newClient();
        this.locator = locator;
        this.serviceName = serviceName;
    }

    @Override
    public Response sendMessage(Function<WebTarget, Response> perform) {
        URI location = locator.locate(serviceName);
        WebTarget target = client.target(location);
        return perform.apply(target);
    }
}
