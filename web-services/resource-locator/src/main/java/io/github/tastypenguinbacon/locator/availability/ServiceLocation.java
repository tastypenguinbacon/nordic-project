package io.github.tastypenguinbacon.locator.availability;

import java.net.URI;

/**
 * Created by pingwin on 02.06.17.
 */
public class ServiceLocation {
    private URI uri;
    private String name;

    public ServiceLocation() {
    }

    public void setUri(String uri) {
        this.uri = URI.create(uri);
    }

    public void setName(String name) {
        this.name = name;
    }

    public URI getUri() {
        return uri;
    }

    public String getName() {
        return name;
    }
}
