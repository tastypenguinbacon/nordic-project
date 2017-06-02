package io.github.tastypenguinbacon.locator.cache;

import com.google.common.cache.CacheBuilder;
import io.github.tastypenguinbacon.locator.availability.ServiceLocation;

import java.util.concurrent.TimeUnit;

/**
 * Created by pingwin on 26.05.17.
 */
public class GuavaBasedCache implements Cache {
    private final com.google.common.cache.Cache<String, String> cache;

    public GuavaBasedCache(int timeout) {
        cache = CacheBuilder.<String, String>newBuilder()
                .expireAfterWrite(timeout, TimeUnit.MILLISECONDS)
                .build();
    }

    @Override
    public boolean contains(String id) {
        return cache.getIfPresent(id) != null;
    }

    @Override
    public void update(String identifier, String location) {
        cache.put(identifier, location);
    }
}
