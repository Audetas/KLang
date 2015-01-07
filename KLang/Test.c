#include <windows.h>
#include <stdio.h>
//#include <math.h>

byte *data = (byte *)malloc(120);
byte *asmcode = (byte*)malloc(120);

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[] = "WindowsApp";

typedef void (WINAPI *asm_message)(HWND, LPCTSTR, LPCTSTR, UINT);

void(*sub_addr)();


void myfunc(void *num)
{
	char msg[10];
	sprintf(msg, "%d", num);
	MessageBox(0, msg, "Debug", 0);
}

void myfunc2(int num)
{
	char msg[10];
	sprintf(msg, "%d", num);
	MessageBox(0, msg, "Debug", 0);
}


void myfunc3()
{
	MessageBox(0, "testing asm calls", "Debug", 0);
}


int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nFunsterStil)

{
	HWND hwnd;               /* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */

	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	/* Use Windows's default color as the background of the window */
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		"Windows App",       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		544,                 /* The programs width */
		375,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
		);



	//dll
	//FARPROC dlluser_messagebox;
	asm_message dlluser_messagebox;
	HINSTANCE dlluser;


	dlluser = LoadLibrary("user32.dll");
	if (!dlluser)
	{
		MessageBox(0, "Missing DLL:", "Debug", 0);
		return FALSE;
	}

	dlluser_messagebox = (asm_message)GetProcAddress(dlluser, "MessageBoxA");

	if (!dlluser_messagebox)
	{
		MessageBox(0, "Missing DLL function:", "Debug", 0);
		return FALSE;
	}

	//this is great
	//dlluser_messagebox(0,"123","456",0);
	//
	int temp;

	//void (*my_routine)(int num);
	//void (*my_routine)(void *num);
	void(*my_routine)();

	temp = 1234;

	my_routine = myfunc3;


	myfunc2((int)my_routine);

	//asmcode[2]=((byte) ((int)my_routine>>24));
	//asmcode[3]=((byte) ((int)my_routine>>16));
	//asmcode[4]=((byte) ((int)my_routine>>8));
	//asmcode[5]=( (byte) my_routine);

	int myaddress = 0;


	myaddress = ((byte)((int)my_routine >> 24));
	//myaddress=((byte) ((int)my_routine>>16));
	//myaddress=((byte) ((int)my_routine>>8));
	//myaddress=( (byte) my_routine);

	// myfunc2(myaddress);




	//myfunc3();

	int i, a, b;
	a = 3000;
	b = -100000;
	a = a + b;

	b = 0;

	//byte badd;

	for (i = 0; i<100; i += 1)
	{
		asmcode[i] = 0x90;//nop
	}

	//asmcode[0]=0x60;//pusha

	//asmcode[0]=0xC3;//ret

	asmcode[1] = 0xC7;//mov
	asmcode[2] = 0xC0;//eax

	asmcode[3] = 0x20;//((byte) ((int)my_routine>>24));
	asmcode[4] = 0x13;//((byte) ((int)my_routine>>16));
	asmcode[5] = 0x40;//((byte) ((int)my_routine>>8));
	asmcode[6] = 0x00;//((byte) my_routine);

	asmcode[7] = 0xFF;//call
	asmcode[8] = 0xD0;//eax

	//asmcode[15]=0xE8;//call address
	//asmcode[16]=0x20;//((byte) ((int)my_routine>>24));
	//asmcode[17]=0x13;//((byte) ((int)my_routine>>16));
	//asmcode[18]=0x40;//((byte) ((int)my_routine>>8));
	//asmcode[19]=0x00;//((byte) my_routine);


	//asmcode[9]=0x61;//popa
	asmcode[20] = 0xC3;//ret

	sub_addr = (void(*)()) (asmcode);

	(*sub_addr)();


	/* Make the window visible on the screen */
	ShowWindow(hwnd, nFunsterStil);

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Translate virtual-key messages into character messages */
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		DispatchMessage(&messages);
	}

	/* The program return-value is 0 - The value that PostQuitMessage() gave */
	return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
	{
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}