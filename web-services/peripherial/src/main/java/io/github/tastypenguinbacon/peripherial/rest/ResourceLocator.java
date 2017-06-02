package io.github.tastypenguinbacon.peripherial.rest;

import java.net.URI;

/**
 * Created by pingwin on 02.06.17.
 */
public interface ResourceLocator {
    URI locate(String serviceName);
}
