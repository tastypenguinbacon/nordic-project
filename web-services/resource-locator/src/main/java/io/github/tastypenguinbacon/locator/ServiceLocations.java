package io.github.tastypenguinbacon.locator;

import io.github.tastypenguinbacon.locator.availability.AvailabilityMonitor;

import javax.inject.Inject;
import javax.validation.constraints.NotNull;
import javax.ws.rs.*;
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
    @Path("{identifier}/{location}")
    public void registerService(@NotNull @PathParam("identifier") String identifier,
                                @NotNull @PathParam("location") String location) {
        availableServices.updateService(identifier, location);
    }
}
