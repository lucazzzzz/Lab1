#pragma comment(lib, "Strmiids.lib")

#include <dshow.h>
#include <conio.h>

#define SECOND 10000000
REFERENCE_TIME rtNow = 0 * SECOND;

enum PlaybackState
{
	STATE_NO_GRAPH,
	STATE_RUNNING,
	STATE_PAUSED,
	STATE_STOPPED,
};

