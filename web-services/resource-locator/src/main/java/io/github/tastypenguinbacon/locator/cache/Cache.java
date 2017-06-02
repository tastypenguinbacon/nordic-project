package io.github.tastypenguinbacon.locator.cache;

/**
 * Created by pingwin on 26.05.17.
 */
public interface Cache {
    boolean contains(String id);

    void update(String identifier, String location);
}
