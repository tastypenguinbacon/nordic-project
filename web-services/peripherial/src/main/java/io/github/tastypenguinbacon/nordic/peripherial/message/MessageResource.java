package io.github.tastypenguinbacon.nordic.peripherial.message;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.peripherial.logger.SLF4JLogger;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.validation.Valid;
import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;

/**
 * Created by pingwin on 28.05.17.
 */
@Path("message")
public class MessageResource {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @Inject
    private MessageHandler handler;

    @POST
    @Produces(MediaType.APPLICATION_JSON)
    @Consumes(MediaType.APPLICATION_JSON)
    @Path("{station_id}")
    public JsonNode consumeMessage(@PathParam("station_id") String stationId, @Valid Message message) {
        logger.info("{} sent message {}", stationId, message);
        return handler.consume(message);
    }
}
