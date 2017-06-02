package io.github.tastypenguinbacon.locator.availability;

import io.github.tastypenguinbacon.locator.cache.Cache;
import io.github.tastypenguinbacon.locator.cache.CacheTimeout;
import io.github.tastypenguinbacon.locator.logger.SLF4JLogger;
import org.slf4j.Logger;

import javax.ejb.ConcurrencyManagement;
import javax.ejb.ConcurrencyManagementType;
import javax.ejb.Singleton;
import javax.ejb.Startup;
import javax.inject.Inject;
import java.util.Map;

/**
 * Created by pingwin on 02.06.17.
 */
@Singleton
@Startup
@ConcurrencyManagement(ConcurrencyManagementType.CONTAINER)
public class AvailableServices implements AvailabilityMonitor {
    private final static int CACHE_TIMEOUT = 10_000;

    private final Cache cache;
    private final Logger logger;

    @Inject
    public AvailableServices(@CacheTimeout(CACHE_TIMEOUT) Cache cache,
                             @SLF4JLogger Logger logger) {
        this.cache = cache;
        this.logger = logger;
    }

    @Override
    public Map<String, String> availableServices() {
        Map<String, String> available = cache.getAll();
        logger.info("Requested available services, returned: {}", available);
        return available;
    }

    @Override
    public void updateService(String name, String location) {
        logger.info("Updated availability for service: {}, at {}", name, location);
        cache.update(name, location);
    }
}
