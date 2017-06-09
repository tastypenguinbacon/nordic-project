package io.github.tastypenguinbacon.nordic.common.rest;

import javaslang.control.Option;
import org.slf4j.Logger;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.util.function.Function;

/**
 * Created by pingwin on 03.06.17.
 */
public class ResourceLocatorCommunicatorProvider implements CommunicatorProvider {
    private final Logger logger;
    private final Client client;

    public ResourceLocatorCommunicatorProvider(Logger restLogger) {
        this.logger = restLogger;
        this.client = ClientBuilder.newClient();
    }

    @Override
    public Option<Response> sendMessage(Function<WebTarget, Response> perform) {
        WebTarget target = client.target(System.getProperty("resource-locator"));
        try {
            Response response = perform.apply(target);
            return Option.some(response);
        } catch (Exception e) {
            logger.warn("Exception occurred while asking for communication provider:", e);
            return Option.none();
        }
    }
}
