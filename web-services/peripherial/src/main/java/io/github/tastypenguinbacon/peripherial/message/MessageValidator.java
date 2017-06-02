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
        return message.getType() != null && !(message.getMessage() == null || message.getMessage().isEmpty());
    }
}
