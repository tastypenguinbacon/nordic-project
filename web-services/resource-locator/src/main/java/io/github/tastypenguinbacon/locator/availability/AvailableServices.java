package io.github.tastypenguinbacon.locator.availability;

import io.github.tastypenguinbacon.locator.cache.Cache;

import javax.ejb.ConcurrencyManagement;
import javax.ejb.ConcurrencyManagementType;
import javax.ejb.Singleton;
import javax.ejb.Startup;

/**
 * Created by pingwin on 02.06.17.
 */
@Singleton
@Startup
@ConcurrencyManagement(ConcurrencyManagementType.CONTAINER)
public class AvailableServices {

    public AvailableServices(Cache cache) {

    }
}
