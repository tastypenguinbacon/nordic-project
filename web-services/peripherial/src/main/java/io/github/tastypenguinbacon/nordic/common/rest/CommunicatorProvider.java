package io.github.tastypenguinbacon.nordic.common.rest;

import javaslang.control.Option;

import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.util.function.Function;

/**
 * Created by pingwin on 01.06.17.
 */
public interface CommunicatorProvider {
    Option<Response> sendMessage(Function<WebTarget, Response> perform);
}
