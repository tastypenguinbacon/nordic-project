package io.github.tastypenguinbacon.nordic.locator.availability;

import java.util.Map;

/**
 * Created by pingwin on 02.06.17.
 */
public interface AvailabilityMonitor {
    Map<String, String> availableServices();

    void updateService(String name, String location);
}
