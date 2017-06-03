package io.github.tastypenguinbacon.nordic.peripherial.cache;

import io.github.tastypenguinbacon.nordic.peripherial.logger.SLF4JLogger;
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
    public GuavaCacheProducer(@SLF4JLogger Logger logger) {
        this.logger = logger;
    }

    @Produces
    public GuavaBasedCache produceCache(InjectionPoint injectionPoint) {
        return Option.of(injectionPoint.getAnnotated().getAnnotation(CacheTimeout.class))
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
