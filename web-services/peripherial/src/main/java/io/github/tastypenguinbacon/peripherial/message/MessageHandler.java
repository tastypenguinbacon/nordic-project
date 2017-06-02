package io.github.tastypenguinbacon.peripherial.message;

import io.github.tastypenguinbacon.peripherial.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.peripherial.rest.TargetService;

import javax.inject.Inject;

/**
 * Created by pingwin on 28.05.17.
 */
public class MessageHandler {
    @Inject
    @TargetService(name = "hello world")
    CommunicatorProvider communicator;

    public String consume(Message message) {
        return null;
    }
}
