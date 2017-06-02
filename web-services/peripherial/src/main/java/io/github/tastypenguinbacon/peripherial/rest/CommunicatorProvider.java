package io.github.tastypenguinbacon.peripherial.rest;

import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.util.function.Function;

/**
 * Created by pingwin on 01.06.17.
 */
public interface CommunicatorProvider {
    Response sendMessage(Function<WebTarget, Response> perform);
}
