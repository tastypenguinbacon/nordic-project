package io.github.tastypenguinbacon.locator.cache;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * Created by pingwin on 28.05.17.
 */
@Retention(RetentionPolicy.RUNTIME)
public @interface CacheTimeout {
    int value();
}
