package io.github.tastypenguinbacon.nordic.common.rest;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import javaslang.control.Option;
import org.slf4j.Logger;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.Entity;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
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
        Option<WebTarget> target = locator.locate(serviceName).map(client::target);
        try {
            return target.map(perform);
        } catch (Exception e) {
            logger.warn("Exception occurred while performing request:", e);
            return Option.none();
        }
    }

    public String getMessage(String target, long after) throws Exception {
        return locator.locate(serviceName).map(client::target).get().path(target)
                .queryParam("data-after", after)
                .request().get()
                .readEntity(String.class);
    }

    public String getMessage(String target) throws Exception {
        return locator.locate(serviceName).map(client::target).get().path(target).request().get()
                .readEntity(String.class);
    }

    public void postMessage(String target, String body) throws Exception{
        locator.locate(serviceName).map(client::target).get().path(target).request()
                .post(Entity.json(new ObjectMapper().readValue(body, JsonNode.class)));
    }
}
