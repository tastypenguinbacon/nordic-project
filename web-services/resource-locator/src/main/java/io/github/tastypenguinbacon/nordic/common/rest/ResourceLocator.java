package io.github.tastypenguinbacon.nordic.common.rest;

import javaslang.control.Option;

import java.net.URI;

/**
 * Created by pingwin on 02.06.17.
 */
public interface ResourceLocator {
    Option<URI> locate(String serviceName);
}
