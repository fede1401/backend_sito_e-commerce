#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <string>
#include <cctype> // Per isupper() e isdigit()
#include <sstream>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <array>
#include <string.h>
#include <fstream>
#include <filesystem>

#include "con2redis.h"

int micro_sleep(long usec);

#endif
