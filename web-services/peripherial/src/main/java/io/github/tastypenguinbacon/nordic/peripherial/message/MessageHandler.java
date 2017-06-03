package io.github.tastypenguinbacon.nordic.peripherial.message;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.JsonNodeFactory;
import io.github.tastypenguinbacon.nordic.peripherial.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.nordic.peripherial.rest.TargetService;
import javaslang.control.Option;
import javaslang.control.Try;

import javax.inject.Inject;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.client.Entity;
import javax.ws.rs.client.WebTarget;
import javax.ws.rs.core.Response;
import java.util.function.Function;

/**
 * Created by pingwin on 28.05.17.
 */
public class MessageHandler {
    @Inject
    @TargetService(name = "error-message-processor")
    CommunicatorProvider error;

    @Inject
    @TargetService(name = "request-message-processor")
    CommunicatorProvider request;

    @Inject
    @TargetService(name = "status-message-processor")
    CommunicatorProvider status;

    public JsonNode consume(Message message) {
        Option<Response> response;
        Function<WebTarget, Response> perform = wt -> wt.request().post(Entity.json(message.getMessage()));
        switch (message.getType()) {
            case ERROR:
                response = error.sendMessage(perform);
                break;
            case STATUS:
                response = status.sendMessage(perform);
                break;
            case REQUEST:
                response = request.sendMessage(perform);
                break;
            default:
                throw new NotFoundException();
        }
        return response.filter(Response::hasEntity)
                .map(r -> Try.of(() -> r)
                        .map(i -> i.readEntity(JsonNode.class))
                        .getOrElse((JsonNode) null))
                .flatMap(Option::of)
                .getOrElse(() -> null);
    }
}
