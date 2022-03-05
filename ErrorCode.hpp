#pragma once

#ifdef DEBUG
  #define LOGE(...)                 \
    do {                            \
      fprintf(stderr, __VA_ARGS__); \
      exit(1);                      \
    } while (0)
  #define LOG_ERROR(code, msg)                                               \
    LOGE("\n<Kaboom!!> %s [%d]: ERROR %d -- %s\n", __FILE__, __LINE__, code, \
         msg)
  #define GET_MACRO_INFO(_1, _2, NAME, ...) NAME
  #define LOG_INFO_MSG(msg) \
    printf("%s [%d]: INFO -- %s\n", __FILE__, __LINE__, msg)
  #define LOG_INFO(fmt, ...)                           \
    do {                                               \
      printf("%s [%d]: INFO -- ", __FILE__, __LINE__); \
      printf(fmt, __VA_ARGS__);                        \
      printf("\n");                                    \
    } while (0)
#else
  #define LOGE
  #define LOG_ERROR(code, mdg)
  #define LOG_INFO(msg)
#endif
