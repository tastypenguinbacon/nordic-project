package io.github.tastypenguinbacon.peripherial.rest;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * Created by pingwin on 01.06.17.
 */
@Retention(RetentionPolicy.RUNTIME)
public @interface TargetService {
    String name();
}
