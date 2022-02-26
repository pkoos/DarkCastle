#ifndef PULSES_H_
#define PULSES_H_

#define PASSES_PER_SEC          100

#define PULSE_TIMER	(1 * PASSES_PER_SEC)
#define PULSE_MOBILE    (4 * PASSES_PER_SEC)
#define PULSE_OBJECT    (4 * PASSES_PER_SEC)
#define PULSE_VIOLENCE  (2 * PASSES_PER_SEC)
#define PULSE_BARD      (1 * PASSES_PER_SEC)
#define PULSE_TENSEC	(10 * PASSES_PER_SEC)
#define PULSE_WEATHER   (45 * PASSES_PER_SEC)
#define PULSE_TIME      (60 * PASSES_PER_SEC)
#define PULSE_REGEN     (15 * PASSES_PER_SEC)
#define PULSE_SHORT     1 // Pulses all the time. 

enum pulse_type { TIMER, MOBILE, OBJECT, VIOLENCE, BARD, TENSEC, WEATHER, TIME, REGEN, SHORT };

#endif