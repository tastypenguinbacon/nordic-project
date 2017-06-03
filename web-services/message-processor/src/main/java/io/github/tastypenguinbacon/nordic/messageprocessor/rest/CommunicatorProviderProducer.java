package io.github.tastypenguinbacon.nordic.messageprocessor.rest;

import io.github.tastypenguinbacon.nordic.messageprocessor.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.messageprocessor.logger.SLF4JProducer;
import javaslang.control.Option;
import org.slf4j.Logger;

import javax.enterprise.inject.Produces;
import javax.enterprise.inject.spi.InjectionPoint;
import javax.inject.Inject;

/**
 * Created by pingwin on 01.06.17.
 */
public class CommunicatorProviderProducer {

    private final ResourceLocator resourceLocator;

    private final SLF4JProducer loggerProducer;

    @Inject
    public CommunicatorProviderProducer(@SLF4JLogger Logger logger, ResourceLocator resourceLocator, SLF4JProducer loggerProducer) {
        this.resourceLocator = resourceLocator;
        this.loggerProducer = loggerProducer;
    }

    @Produces
    public CommunicatorProvider producer(InjectionPoint ip) {
        Logger restLogger = loggerProducer.producer(ip);
        return Option.of(ip)
                .map(InjectionPoint::getAnnotated)
                .flatMap(a -> Option.of(a.getAnnotation(TargetService.class)))
                .map(TargetService::name)
                .filter(name -> !name.equals("resource-locator"))
                .map((String serviceName) -> new RestCommunicatorProvider(serviceName, resourceLocator, restLogger))
                .flatMap(Option::of)
                .map(el -> (CommunicatorProvider) el)
                .getOrElse(new ResourceLocatorCommunicatorProvider(restLogger));
    }
}