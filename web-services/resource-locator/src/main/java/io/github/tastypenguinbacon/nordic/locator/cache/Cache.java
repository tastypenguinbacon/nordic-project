package io.github.tastypenguinbacon.nordic.locator.cache;

import java.util.Map;

/**
 * Created by pingwin on 26.05.17.
 */
public interface Cache {
    void update(String identifier, String location);

    Map<String,String> getAll();
}
