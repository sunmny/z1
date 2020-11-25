#ifndef HARDWARE_KEY_H
#define HARDWARE_KEY_H

void hw_key_init(void);
void hw_key_next_irq(void);

void hw_key_sos_irq(void);


void hw_key_call_irq(void);

void hw_key_power_in_irq(void);

#endif
