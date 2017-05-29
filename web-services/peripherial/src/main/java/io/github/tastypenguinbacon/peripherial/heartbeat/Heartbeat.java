package io.github.tastypenguinbacon.peripherial.heartbeat;

import io.github.tastypenguinbacon.peripherial.logger.SLF4JLogger;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;

/**
 * Created by pingwin on 26.05.17.
 */
@Path("heartbeat")
public class Heartbeat {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @Inject
    private AvailabilityService heartbeatMonitor;

    @POST
    @Path("{station_id}")
    public void clientIsActive(@PathParam("station_id") String stationId) {
        logger.info("Received heartbeat from: " + stationId);
        heartbeatMonitor.updateAvailability(stationId);
    }
}
