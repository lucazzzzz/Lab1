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
	IGraphBuilder *pGraph = NULL;
	IMediaControl *pControl = NULL;
	IMediaEvent   *pEvent = NULL;
	IMediaSeeking	*pSeek = NULL;
	PlaybackState State;

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
	hr = pGraph->RenderFile(L"Example.avi", NULL);

	if (SUCCEEDED(hr))
	{
		// Faire une fonction Commande qui retourne le caractère de commande (soit p, a, r ou q) à l'aide de getch qui prends le bon caractère en mémoire
		// Ne pas oublier de faire un printf pour afficher le caractère à l'écran

		hr = pControl->Run();
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
						hr = pControl->Pause();
						State = STATE_PAUSED;
					}
					else
					{
						printf("Play\n");
						hr = pControl->Run();
						State = STATE_RUNNING;
					}
				}
				if (Commande(buffer) == 'a')
				{
					printf("Avance Rapide\n");
					hr = pSeek->SetRate(2.0);

				}
				if (Commande(buffer) == 'r')
				{
					printf("Retour\n");
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
				if (Commande(buffer) == 'q')
				{
					printf("Quitter\n");
					break;
				}

			}
		}
		if (SUCCEEDED(hr))
		{
			// Wait for completion.
			long evCode;
			pEvent->WaitForCompletion(INFINITE, &evCode);

			// Note: Do not use INFINITE in a real application, because it
			// can block indefinitely.
		}
	}
	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();

}