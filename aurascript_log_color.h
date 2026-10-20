#ifndef AURASCRIPT_LOG_COLOR_H
#define AURASCRIPT_LOG_COLOR_H

void aurascript_log_color_init(void);
int aurascript_register_log_curve(const char* standard, float black_clip, float white_clip);

#endif
