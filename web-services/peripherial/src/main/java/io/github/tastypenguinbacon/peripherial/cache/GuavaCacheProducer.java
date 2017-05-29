package io.github.tastypenguinbacon.peripherial.cache;

import javaslang.control.Option;
import org.slf4j.Logger;

import javax.enterprise.inject.Produces;
import javax.enterprise.inject.spi.Annotated;
import javax.enterprise.inject.spi.InjectionPoint;
import javax.inject.Inject;

/**
 * Created by pingwin on 28.05.17.
 */
public class GuavaCacheProducer {
    private final Logger logger;

    @Inject
    public GuavaCacheProducer(Logger logger) {
        this.logger = logger;
    }

    @Produces
    public GuavaBasedCache produceCache(InjectionPoint injectionPoint) {
        Option<CacheTimeout> timeout = Option.of(injectionPoint.getAnnotated().getAnnotation(CacheTimeout.class));
        return timeout
                .map(CacheTimeout::value)
                .map(GuavaBasedCache::new)
                .getOrElseThrow(() -> exception(injectionPoint));
    }

    private RuntimeException exception(InjectionPoint injectionPoint) {
        Annotated whereViolationOccurred = injectionPoint.getAnnotated();
        String message = "Tried to create cache without specifying an timeout at: " + whereViolationOccurred;
        logger.error(message);
        return new IllegalArgumentException(message);
    }
}
