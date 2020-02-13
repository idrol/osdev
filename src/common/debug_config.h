#pragma once

#ifndef DEBUG_VERBOSE
#define DEBUG_VERBOSE 1
#endif

#ifndef DEBUG_HLT_PAGE_FAULT
#define DEBUG_HLT_PAGE_FAULT 1 // If set to 1 kernel compiles with a hlt instruction in pagefault handler
#endif