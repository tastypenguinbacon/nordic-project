package io.github.tastypenguinbacon.nordic.db.resource;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.db.service.DataBase;
import io.github.tastypenguinbacon.nordic.db.service.ScriptProvider;
import javaslang.control.Option;
import org.slf4j.Logger;

import javax.inject.Inject;
import javax.validation.constraints.NotNull;
import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;
import java.util.List;

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

    @Inject
    private DataBase dataBase;

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    @Path("{station}")
    public List<JsonNode> getData(@PathParam("station") @NotNull String station,
                                @QueryParam("data-after") @DefaultValue("0") int dataAfter) {
        return dataBase.getAfter(station, dataAfter).toJavaList();
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
    public List<String> getAvailableIdentifiers() {
        return dataBase.availableIdentifiers().toJavaList();
    }

    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Path("{station}")
    public void postData(@PathParam("station") String station, JsonNode payload) {
        dataBase.put(station, payload);
    }
}
