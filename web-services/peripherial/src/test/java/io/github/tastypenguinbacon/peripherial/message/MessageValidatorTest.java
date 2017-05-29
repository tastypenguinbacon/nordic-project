package io.github.tastypenguinbacon.peripherial.message;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import javax.validation.ConstraintValidatorContext;

import static org.mockito.Matchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.testng.Assert.*;

/**
 * Created by pingwin on 28.05.17.
 */
public class MessageValidatorTest {
    @Test(dataProvider = "valid_messages")
    public void validMessages(Message message) {
        MessageValidator validator = new MessageValidator();
        ConstraintValidatorContext mock = mock(ConstraintValidatorContext.class);
        assertTrue(validator.isValid(message, mock));
    }

    @Test(dataProvider = "invalid_messages")
    public void invalidMessages(Message message) {
        MessageValidator validator = new MessageValidator();
        ConstraintValidatorContext mock = mock(ConstraintValidatorContext.class);
        assertFalse(validator.isValid(message, mock));
    }

    @DataProvider(name = "valid_messages")
    public Object[][] validMessageProvider() {
        return new Object[][]{
                {new Message("ERROR", "TEST")},
                {new Message("STATUS", "HEJO")},
                {new Message("REQUEST", "HEJO")}
        };
    }

    @DataProvider(name = "invalid_messages")
    public Object[][] invalidMessageProvider() {
        return new Object[][] {
                {new Message("eRRor", "TEST")},
                {new Message("ERROR", "")},
                {new Message(null, "TEST")},
                {new Message("ERROR", null)},
                {new Message(null, null)}
        };
    }

}