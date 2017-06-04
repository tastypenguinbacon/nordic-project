package io.github.tastypenguinbacon.nordic.messageprocessor.message.request;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.core.Response;

/**
 * Created by pingwin on 03.06.17.
 */
@Path("request")
public class RequestHandler {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @POST
    public Response handleRequest(JsonNode message) {
        logger.info("Request is being processed: {}", message);
        return null;
    }
}
