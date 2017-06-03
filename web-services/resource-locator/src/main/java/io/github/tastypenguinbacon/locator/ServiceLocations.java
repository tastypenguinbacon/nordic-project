package io.github.tastypenguinbacon.locator;

import io.github.tastypenguinbacon.locator.availability.AvailabilityMonitor;

import javax.inject.Inject;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import java.util.Map;

/**
 * Created by pingwin on 02.06.17.
 */
@Path("/")
public class ServiceLocations {
    @Inject
    private AvailabilityMonitor availableServices;

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Map<String, String> getServices() {
        return availableServices.availableServices();
    }

    @POST
    public void registerService(Map<String, String> locations) {
        locations.entrySet().stream()
                .filter(e -> e.getKey() != null && e.getValue() != null)
                .forEach(e -> availableServices.updateService(e.getKey(), e.getValue()));
    }
}
