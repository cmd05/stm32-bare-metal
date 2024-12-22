#define ADC_SR_EOC      (1U << 1)

void pa1_adc_interrupt_init(void);
void start_conversion(void);
uint32_t adc_read(void);