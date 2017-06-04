package io.github.tastypenguinbacon.nordic.db.resource;

import com.fasterxml.jackson.databind.JsonNode;

import javax.ws.rs.*;
import javax.ws.rs.core.MediaType;

/**
 * Created by pingwin on 04.06.17.
 */
@Path("")
public class DataAccess {
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public JsonNode getData() {
        return null;
    }

    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    public void postData(JsonNode payload) {

    }
}
