package io.github.tastypenguinbacon.nordic.messageprocessor.message;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import groovy.lang.Binding;
import groovy.lang.GroovyClassLoader;
import groovy.lang.Script;
import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.common.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.nordic.common.rest.TargetService;
import javaslang.collection.HashMap;
import javaslang.control.Try;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

/**
 * Created by pingwin on 03.06.17.
 */
@Path("/")
public class MessageHandler {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @Inject
    @TargetService(name = "scripts")
    private CommunicatorProvider scripts;

    @Inject
    @TargetService(name = "database")
    private CommunicatorProvider db;

    @POST
    @Path("{type}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response handleError(@PathParam("type") String type, JsonNode message) throws IllegalAccessException, InstantiationException, JsonProcessingException {
        logger.info("Message of type {} given as {} is being handled", type, message);
        String script = scripts.sendMessage(wt -> wt.path(type).request().get())
                .filter(Response::hasEntity)
                .map(response -> Try.of(() -> response.readEntity(String.class))
                        .getOrElseThrow(() -> new IllegalStateException("Could not deserialize String")))
                .getOrElseThrow(() -> new NotFoundException("No script for type: " + type));
        Script groovyScript = ((Script) new GroovyClassLoader().parseClass(script).newInstance());
        groovyScript.setBinding(new Binding(HashMap.of("message", message, "db", db, "log", logger).toJavaMap()));
        String json = new ObjectMapper().writeValueAsString(groovyScript.run());
        return Response.ok().entity(json).build();
    }
}
