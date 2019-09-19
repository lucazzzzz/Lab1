#include <dshow.h>
#include <conio.h>
#include <string>

#define SECOND 10000000
REFERENCE_TIME rtNow = 0 * SECOND;


class Videoplayer
{
	IGraphBuilder *pGraph = NULL;
	IMediaControl *pControl = NULL;
	IMediaEvent   *pEvent = NULL;
	IMediaSeeking	*pSeek = NULL;
	HRESULT hr;
public:
	Videoplayer();
	void Run();
	void End();
	void Pause();
	void FF();
	void RetourArriere();

};

Videoplayer::Videoplayer()
{
	// Initialize the COM library.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		printf("ERROR - Could not initialize COM library");
		return;
	}

	// Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void **)&pGraph);
	if (FAILED(hr))
	{
		printf("ERROR - Could not create the Filter Graph Manager.");
		return;
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeek);

	// Build the graph. IMPORTANT: Change this string to a file on your system.
	hr = pGraph->RenderFile(L"C:\\Example.avi", NULL);

	if (FAILED(hr))
	{
		printf("ERROR - Could not render file");

	}
}

void Videoplayer::Run()
{
	hr = pControl->Run();
}

void Videoplayer::End()
{
	if (SUCCEEDED(hr))
	{
		// Wait for completion.
		long evCode;
		pEvent->WaitForCompletion(INFINITE, &evCode);

		// Note: Do not use INFINITE in a real application, because it
		// can block indefinitely.
	}

	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();
}

void Videoplayer::Pause()
{
	hr = pControl->Pause();
}

void Videoplayer::FF()
{
	hr = pSeek->SetRate(2.0);
}

void Videoplayer::RetourArriere()
{
	hr = pSeek->IsFormatSupported(&TIME_FORMAT_FRAME);
	if (hr == S_OK)
	{
		hr = pSeek->SetTimeFormat(&TIME_FORMAT_FRAME);
		if (SUCCEEDED(hr))
		{
			// Seek to frame number 0.
			LONGLONG rtNow = 0;
			hr = pSeek->SetPositions(
				&rtNow, AM_SEEKING_AbsolutePositioning,
				0, AM_SEEKING_NoPositioning);
		}
	}
}

enum PlaybackState
{
	STATE_NO_GRAPH,
	STATE_RUNNING,
	STATE_PAUSED,
	STATE_STOPPED,
};

char Commande(int caractere)
{
	char value;
	switch (caractere)
	{
	case 'p':
		value = 'p';
		break;
	case 'a':
		value = 'a';
		break;
	case 'r':
		value = 'r';
		break;
	case 'q':
		value = 'q';
		break;

	default:
		break;
	}

	return value;
}

void main(void)
{
	PlaybackState State;

	 
	Videoplayer VP = Videoplayer();

	VP.Run();
	State = STATE_RUNNING;

	while (true)
	{
		int buffer;
		buffer = _getch();

		if (buffer)
		{
			if (Commande(buffer) == 'p')
			{

				if (State == STATE_RUNNING)
				{
					printf("Pause\n");
					VP.Pause();
					State = STATE_PAUSED;
				}
				else
				{
					printf("Play\n");
					VP.Run();
					State = STATE_RUNNING;
				}
			}
			if (Commande(buffer) == 'a')
			{
				printf("Avance Rapide\n");
				VP.FF();

			}
			if (Commande(buffer) == 'r')
			{
				printf("Retour\n");
				VP.RetourArriere();
				
			}
			if (Commande(buffer) == 'q')
			{
				printf("Quitter\n");
				break;
				State = STATE_STOPPED;
			}

		}
	}

	VP.End();
}