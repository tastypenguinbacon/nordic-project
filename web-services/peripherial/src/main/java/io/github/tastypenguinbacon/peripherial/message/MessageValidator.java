package io.github.tastypenguinbacon.peripherial.message;

import javax.validation.ConstraintValidator;
import javax.validation.ConstraintValidatorContext;

/**
 * Created by pingwin on 28.05.17.
 */
public class MessageValidator implements ConstraintValidator<ValidMessage, Message> {
    public void initialize(ValidMessage constraint) {
    }

    @Override
    public boolean isValid(Message message, ConstraintValidatorContext context) {
        if (message.getType() == null) {
            return false;
        }
        if (message.getMessage() == null || message.getMessage().isEmpty()) {
            return false;
        }
        return true;
    }
}
