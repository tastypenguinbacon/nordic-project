package io.github.tastypenguinbacon.nordic.messageprocessor.message.status;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.messageprocessor.logger.SLF4JLogger;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.core.Response;

/**
 * Created by pingwin on 03.06.17.
 */
@Path("status")
public class StatusHandler {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @POST
    public Response handleStatus(JsonNode message) {
        logger.info("Status update", message);
        return null;
    }
}
