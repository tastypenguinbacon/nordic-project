package io.github.tastypenguinbacon.nordic.messageprocessor.message.heartbeatloss;

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
@Path("heartbeat-loss")
public class HeartBeatLossHandler {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @POST
    public Response handleHeartBeatLoss(JsonNode node) {
        logger.info("Heartbeat loss is being handled for: {}", node);
        return null;
    }
}
