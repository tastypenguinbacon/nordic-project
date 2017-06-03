package io.github.tastypenguinbacon.nordic.messageprocessor.message.error;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.messageprocessor.logger.SLF4JLogger;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.xml.ws.Response;

/**
 * Created by pingwin on 03.06.17.
 */
@Path("error")
public class ErrorHandler {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @POST
    public Response handleError(JsonNode message) {
        logger.info("Error {} is being handled", message);
        return null;
    }
}
