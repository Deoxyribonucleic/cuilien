#ifndef _DEBUG_H_
#define _DEBUG_H_

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

#endif // _DEBUG_H_
