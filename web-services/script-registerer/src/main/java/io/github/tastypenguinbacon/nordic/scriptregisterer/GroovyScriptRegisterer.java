package io.github.tastypenguinbacon.nordic.scriptregisterer;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import javaslang.control.Try;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import java.util.function.Function;

/**
 * Created by pingwin on 08.06.17.
 */
public class GroovyScriptRegisterer {
    private final Client client;
    private final String resourceLocator;

    public GroovyScriptRegisterer(Client client, String resourceLocator) {
        this.client = client;
        this.resourceLocator = resourceLocator;
    }

    @SuppressWarnings("unchecked")
    public void registerScript(String scriptName, String script) {
        JsonNode locations = Try.of(() -> client.target(resourceLocator).request().get())
                .map(response -> response.readEntity(String.class))
                .flatMap(entity -> Try.of(() -> new ObjectMapper().readValue(entity, JsonNode.class)))
                .getOrElseThrow((Function<Throwable, RuntimeException>) RuntimeException::new);
        String scriptLocation = Try.of(() -> locations.get("scripts").asText())
                .getOrElseThrow(() -> new RuntimeException("No scripts endpoint provided"));
        String messageProcessorLocation = Try.of(() -> locations.get("message-processor").asText())
                .getOrElseThrow(() -> new RuntimeException("No message processor endpoint"));
        String name = scriptName.substring(0, scriptName.indexOf('.'));
        Try.of(() -> client.target(scriptLocation).path(name)
                .request().post(Entity.entity(script, MediaType.TEXT_PLAIN))).
                getOrElseThrow((Function<Throwable, RuntimeException>) RuntimeException::new);
        String registerJson = "{\"" + name + "-message-processor\":\"" + messageProcessorLocation + "/" + name + "\"}";
        Try.of(() -> client.target(resourceLocator).request()
                .post(Entity.entity(registerJson, MediaType.APPLICATION_JSON)))
                .getOrElseThrow((Function<Throwable, RuntimeException>) RuntimeException::new);
    }
}
