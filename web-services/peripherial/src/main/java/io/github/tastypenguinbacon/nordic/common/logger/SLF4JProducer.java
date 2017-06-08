package io.github.tastypenguinbacon.nordic.common.logger;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.enterprise.inject.Produces;
import javax.enterprise.inject.spi.InjectionPoint;

/**
 * Created by pingwin on 27.05.17.
 */
public class SLF4JProducer {
    @Produces
    @SLF4JLogger
    public Logger producer(InjectionPoint ip) {
        String className = ip.getMember().getDeclaringClass().getName();
        return LoggerFactory.getLogger(className);
    }
}
