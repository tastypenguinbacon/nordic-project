package io.github.tastypenguinbacon.nordic.messageprocessor.logger;

import javax.inject.Qualifier;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * Created by pingwin on 28.05.17.
 */
@Qualifier
@Retention(RetentionPolicy.RUNTIME)
public @interface SLF4JLogger {
}