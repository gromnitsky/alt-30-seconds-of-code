#include "itoa.c"

#include <time.h>
#include <unistd.h>

typedef struct {
  int min;
  int max;
  int cur;
  long update_delay;
  // private
  long last_tick;
  char buf[1024];
} Progress;

long epoch_ns() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

void _progress_render(Progress *p) { /* 123/456  26.97% */
  if (p->last_tick && (epoch_ns() - p->last_tick < p->update_delay)) return;

  double percent = (p->cur/(p->max+0.0))*100;
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  int max_len = itoa_len(p->max);
  snprintf(p->buf, sizeof(p->buf), "%*d/%d %6.2f%%",
           max_len,  p->cur, p->max, percent);

  if (isatty(2)) {
    if (p->last_tick) { // erase prev string
      int len = strlen(p->buf);
      char eraser[len+1]; memset(eraser, '\b', len); eraser[len] = '\0';
      fprintf(stderr, "%s", eraser);
    }
    fprintf(stderr, "%s", p->buf);
  } else {
    fprintf(stderr, "%s\n", p->buf);
  }

  p->last_tick = epoch_ns();
}

Progress* progress_init(int min, int max) {
  Progress *p = (Progress*)malloc(sizeof(Progress));
  p->min = min; p->max = max;
  p->update_delay = 100000000; // 100 ms
  p->last_tick = 0;
  return p;
}

void progress_update(Progress *p, int cur) {
  p->cur = cur;
  _progress_render(p);
}

void progress_end(Progress **p) {
  if ( !(p && *p)) return;
  (*p)->update_delay = 0;
  _progress_render(*p); // presumably draw "100%"
  free(*p);
  *p = NULL;
}


void progress() {
  Progress *prg = progress_init(0, 50000);
  fprintf(stderr, "%s", "progress so far: "); // note the absence of \n

  for (int n = 0; n <= prg->max; n++) {
    progress_update(prg, n);
    usleep(1);
  }
  progress_end(&prg);
  fprintf(stderr, "\n");

  assert(NULL == prg);
}
