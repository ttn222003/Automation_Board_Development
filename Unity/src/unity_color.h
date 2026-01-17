#ifndef UNITY_COLOR_H
#define UNITY_COLOR_H

#ifdef UNITY_COLOR
    #define COL_RED    "\033[31m"
    #define COL_GREEN  "\033[32m"
    #define COL_YELLOW "\033[33m"
    #define COL_RESET  "\033[0m"
#else
    #define COL_RED    ""
    #define COL_GREEN  ""
    #define COL_YELLOW ""
    #define COL_RESET  ""
#endif

#endif
