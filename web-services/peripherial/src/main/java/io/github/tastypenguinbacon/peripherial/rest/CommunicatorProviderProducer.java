package io.github.tastypenguinbacon.peripherial.rest;

import io.github.tastypenguinbacon.peripherial.logger.SLF4JLogger;
import javaslang.control.Option;
import org.slf4j.Logger;

import javax.enterprise.inject.Produces;
import javax.enterprise.inject.spi.InjectionPoint;
import javax.inject.Inject;

/**
 * Created by pingwin on 01.06.17.
 */
public class CommunicatorProviderProducer {
    private final Logger logger;

    private final ResourceLocator resourceLocator;

    @Inject
    public CommunicatorProviderProducer(@SLF4JLogger Logger logger, ResourceLocator resourceLocator) {
        this.logger = logger;
        this.resourceLocator = resourceLocator;
    }

    @Produces
    public RestCommunicatorProvider producer(InjectionPoint ip) {
        return Option.of(ip)
                .map(InjectionPoint::getAnnotated)
                .flatMap(a -> Option.of(a.getAnnotation(TargetService.class)))
                .map(TargetService::name)
                .map((String serviceName) -> new RestCommunicatorProvider(serviceName, resourceLocator))
                .flatMap(Option::of)
                .getOrElseThrow(() -> exceptionOf(ip));
    }

    private RuntimeException exceptionOf(InjectionPoint ip) {
        String message = "There is no TargetService annotation for: " + ip.getMember();
        logger.error(message);
        return new IllegalArgumentException(message);
    }
}