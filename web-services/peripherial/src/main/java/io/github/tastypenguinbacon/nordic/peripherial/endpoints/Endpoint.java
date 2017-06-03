package io.github.tastypenguinbacon.nordic.peripherial.endpoints;

/**
 * Created by pingwin on 28.05.17.
 */
public class Endpoint {
    private final String name;
    private final String url;
    private final String description;
    private final String method;

    public Endpoint(String name, String url, String method, String description) {
        this.name = name;
        this.url = url;
        this.description = description;
        this.method = method;
    }

    public String getName() {
        return name;
    }

    public String getUrl() {
        return url;
    }

    public String getDescription() {
        return description;
    }

    public String getMethod() {
        return method;
    }
}
