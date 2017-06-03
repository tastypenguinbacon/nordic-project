package io.github.tastypenguinbacon.peripherial.rest;

import javaslang.control.Option;
import org.slf4j.Logger;

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
    private final Logger logger;

    public RestCommunicatorProvider(String serviceName, ResourceLocator locator, Logger logger) {
        this.client = ClientBuilder.newClient();
        this.locator = locator;
        this.serviceName = serviceName;
        this.logger = logger;
    }

    @Override
    public Option<Response> sendMessage(Function<WebTarget, Response> perform) {
        URI location = locator.locate(serviceName);
        WebTarget target = client.target(location);
        try {
            Response response = perform.apply(target);
            return Option.some(response);
        } catch (Exception e) {
            logger.warn("Exception occurred while performing request:", e);
            return Option.none();
        }
    }
}
