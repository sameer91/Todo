#pragma once

#ifdef DEBUG
  #define LOGE(...)                 \
    do {                            \
      fprintf(stderr, __VA_ARGS__); \
      exit(-1);                     \
    } while (0)
  #define LOG_ERROR(code, fmt, ...)                                         \
    LOGE("\n<Kaboom!!> %s [%d]: ERROR %d -- " fmt "\n", __FILE__, __LINE__, \
         code, ##__VA_ARGS__)
  // #define GET_MACRO_INFO(_1, _2, NAME, ...) NAME
  // #define LOG_INFO_MSG(msg) \
  //  printf("%s [%d]: INFO -- %s\n", __FILE__, __LINE__, msg)
  #define LOG_INFO(fmt, ...) \
    printf("%s [%d]: INFO -- " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#else
  #define LOGE(...)
  #define LOG_ERROR(code, msg)
  #define LOG_INFO_MSG(msg)
  #define LOG_INFO(fmt, ...)
#endif
