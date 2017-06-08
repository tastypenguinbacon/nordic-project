package io.github.tastypenguinbacon.nordic.peripherial.message;

import com.fasterxml.jackson.databind.JsonNode;
import io.github.tastypenguinbacon.nordic.common.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.nordic.common.rest.TargetService;
import javaslang.control.Option;
import javaslang.control.Try;

import javax.inject.Inject;
import javax.ws.rs.client.Entity;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

/**
 * Created by pingwin on 28.05.17.
 */
public class MessageHandler {
    @Inject
    @TargetService(name = "error-message-processor")
    private CommunicatorProvider error;

    @Inject
    @TargetService(name = "request-message-processor")
    private CommunicatorProvider request;

    @Inject
    @TargetService(name = "status-message-processor")
    private CommunicatorProvider status;

    @Inject
    @TargetService(name = "message-processor")
    private CommunicatorProvider custom;

    public JsonNode consume(String stationId, Message message) {
        Option<Response> response;
        Map<String, Object> entity = new HashMap<>();
        entity.put("stationId", stationId);
        entity.put("body", message.getMessage());
        Function<WebTarget, Response> perform = wt -> wt.request().post(Entity.json(entity));
        switch (message.getType()) {
            case "error":
                response = error.sendMessage(perform);
                break;
            case "status":
                response = status.sendMessage(perform);
                break;
            case "request":
                response = request.sendMessage(perform);
                break;
            default:
                response = custom.sendMessage(wt -> perform.apply(wt.path(message.getType())));
        }
        return response.filter(Response::hasEntity)
                .map(r -> Try.of(() -> r)
                        .map(i -> i.readEntity(JsonNode.class))
                        .getOrElse((JsonNode) null))
                .flatMap(Option::of)
                .getOrElse(() -> null);
    }
}
