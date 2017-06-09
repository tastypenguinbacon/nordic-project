package io.github.tastypenguinbacon.nordic.sample;

import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.common.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.nordic.common.rest.TargetService;
import javaslang.collection.TreeMap;
import javaslang.control.Try;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.ws.rs.*;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.MediaType;
import java.util.Collections;
import java.util.HashMap;

/**
 * Created by pingwin on 02.06.17.
 */
@Path("/")
public class ServiceLocations {
    @Inject
    @TargetService(name = "sample-rest-message-processor")
    private CommunicatorProvider sample;

    @Inject
    @SLF4JLogger
    private Logger logger;

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public String getSampleNames() {
        String resp =  sample.sendMessage(wt -> wt.request()
                .post(Entity.json(new HashMap<>())))
                .toTry()
                .flatMap(r -> Try.of(() -> r.readEntity(String.class)))
                .get();
        logger.info("Sample rest api: {}", resp);
        return resp;
    }

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    @Path("{subsystem_id}")
    public String getSample(@PathParam("subsystem_id") String id) {
        return sample.sendMessage(wt -> wt.request()
                .post(Entity.json(TreeMap.of("id", id).toJavaMap())))
                .toTry()
                .flatMap(r -> Try.of(() -> r.readEntity(String.class)))
                .get();
    }

    @POST
    @Path("{subsystem_id}")
    public void postSample(@PathParam("subsystem_id") String id, @QueryParam("code") int code) {
        sample.sendMessage(wt -> wt.request()
                .post(Entity.json(TreeMap.of("id", id + "-info")
                .put("code", String.valueOf(code)).toJavaMap())))
                .toTry()
                .flatMap(r -> Try.of(() -> r.readEntity(String.class)))
                .get();
    }
}
