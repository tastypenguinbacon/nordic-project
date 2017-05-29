package io.github.tastypenguinbacon.peripherial.endpoints;

import javax.json.Json;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import java.util.Arrays;
import java.util.List;

/**
 * Created by pingwin on 28.05.17.
 */
@Path("/")
public class Endpoints {
    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public List<Endpoint> getEndpoints() {
        return Arrays.asList(
                new Endpoint("methods", "/peripherial", "GET", "Returns the methods provided by the service."),
                new Endpoint("heartbeat", "/peripherial/heartbeat/{station_id}", "POST", "Used for availability checks," +
                        " requires station identifier as parameter."),
                new Endpoint("message", "/peripherial/message/{station_id}", "POST", "Used for messages, accepts a JSON of" +
                        " the following form:" + exampleJsonMessage() + "."),
                new Endpoint("tasks", "/peripherial/tasks/{station_id}/{after}", "GET", "Returns a JSON list of tasks scheduled" +
                        " after the specified period (UNIX Epoch timestamp).")
        );
    }

    private String exampleJsonMessage() {
        return Json.createObjectBuilder()
                .add("type", "[ERROR | STATUS | REQUEST]")
                .add("message", "[message body]")
                .build().toString();
    }
}
