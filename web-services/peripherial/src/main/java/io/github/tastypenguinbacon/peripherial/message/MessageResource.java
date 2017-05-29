package io.github.tastypenguinbacon.peripherial.message;

import org.slf4j.Logger;

import javax.inject.Inject;
import javax.validation.Valid;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

/**
 * Created by pingwin on 28.05.17.
 */
@Path("message")
public class MessageResource {
    @Inject
    private Logger logger;

    @Inject
    private MessageHandler handler;

    @POST
    @Produces(MediaType.APPLICATION_JSON)
    @Path("{station_id}")
    public String consumeMessage(@PathParam("station_id") String stationId, @Valid Message message) {
        logger.info("{} sent message {}", stationId, message);
        return handler.consume(message);
    }
}
