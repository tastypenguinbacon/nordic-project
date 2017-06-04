package io.github.tastypenguinbacon.nordic.db.resource;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.db.service.ScriptProvider;
import javaslang.control.Option;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.json.Json;
import javax.json.JsonArray;
import javax.json.JsonObject;
import javax.print.attribute.standard.Media;
import javax.validation.constraints.NotNull;
import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;

/**
 * Created by pingwin on 04.06.17.
 */
@Path("")
public class DataAccess {
    @Inject
    @SLF4JLogger
    private Logger logger;

    @Inject
    private ScriptProvider scriptProvider;

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    @Path("{subsystem}")
    public JsonArray getData(@PathParam("subsystem") @NotNull String subsystem,
                             @QueryParam("data-after") @DefaultValue("0") int dataAfter) {
        return Json.createArrayBuilder().build();
    }

    @POST
    @Consumes(MediaType.TEXT_PLAIN)
    @Path("groovy/{name}")
    public void postGroovyScript(@PathParam("name") @NotNull String scriptName, String script) {
        logger.info("Registering {} for script name {}", script, scriptName);
        scriptProvider.addScript(scriptName, script);
    }

    @GET
    @Produces(MediaType.TEXT_PLAIN)
    @Path("groovy/{name}")
    public String getGroovyScript(@PathParam("name") @NotNull String scriptName) {
        logger.info("Groovy script requested {}", scriptName);
        Option<String> script = scriptProvider.query(scriptName);
        if (script.isEmpty()) {
            logger.warn("No script found for name {}", scriptName);
            throw new NotFoundException();
        }
        return script.get();
    }

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public JsonArray getAvailableIdentifiers() {
        return Json.createArrayBuilder()
                .build();
    }

    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Path("{subsystem}")
    public void postData(@PathParam("subsystem") String subsystem, JsonNode payload) {

    }
}
