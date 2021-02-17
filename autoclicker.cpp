//credits to Adrian McCarthy and Newbie, capturescreen functions sourced from stackoverflow 'How to read screen pixels'
//record: 44.736 tiles/s
//config: bluestacks4 with file explorer, vs 2019 and powershell running
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <wingdi.h>
#include <fstream>
using namespace std;

//functions
void CaptureScreen(HDC hCaptureDC, HDC hDesktopDC, HBITMAP hCaptureBitmap);
void click(int x, int y);

//list of x and y values to check on screen
int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

int y[] = { 550, 500, 450, 400, 350, 300, 250, 200, 150, 100, 50 };
int len = sizeof(y) / sizeof(int);

int x[] = { 139, 143, 419, 423 };

//acceleration compensation, values depends on which level in the game, adjust accordingly
//beginner challlenge : speedgradient 0.037, initial 3.33
//single tile challenge : speedgradient 0.0875, intial 5
double clickspeed = 0.0168;     //time taken to click
int tileheight = 249;           
double speedgradient = 0.0875;  //rate at which tile/s increases
double initialspeed = 5;        //initial tile/s
int extradis = 200;             //additional compensation distance to click below tile

int main()
{
    //log file 
    ofstream log;
    log.open("log.txt");

    //create capture device context and bitmap, and desktop device context
    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
    SelectObject(hCaptureDC, hCaptureBitmap);

    //loop the clicking algorithm
    while (GetAsyncKeyState(0x51) == 0)
    {
        clock_t t = clock();

        CaptureScreen(hCaptureDC, hDesktopDC, hCaptureBitmap);

        //logging time taken per cycle of screenshot and clicks
        clock_t time_passed = clock() - t;
        double seconds = (double)time_passed / CLOCKS_PER_SEC;
        log << seconds << endl;
    }

    //release device context
    ReleaseDC(hDesktopWnd, hDesktopDC);
    DeleteDC(hCaptureDC);
    DeleteObject(hCaptureBitmap);

    log.close();
}

void CaptureScreen(HDC hCaptureDC, HDC hDesktopDC, HBITMAP hCaptureBitmap)
{
    //take screenshot of target area
    BitBlt(hCaptureDC, x[0], y[len - 1], x[3] - x[0] + 1, y[0] - y[len - 1] + 1, hDesktopDC, x[0], y[len - 1], SRCCOPY | CAPTUREBLT);

    //define bitmap information
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = nScreenWidth;
    bmi.bmiHeader.biHeight = nScreenHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    //create buffer in memory
    RGBQUAD* pPixels = new RGBQUAD[nScreenWidth * nScreenHeight];

    //load screenshot into buffer
    GetDIBits(
        hCaptureDC,
        hCaptureBitmap,
        0,
        nScreenHeight,
        pPixels,
        &bmi,
        DIB_RGB_COLORS
    );

    //acceleration compensation
    double timeelapsed = (double)clock() / CLOCKS_PER_SEC;
    double tilepersec = speedgradient * timeelapsed + initialspeed;
    double pixpersec = tileheight * tilepersec;
    int dis = int(clickspeed * pixpersec) + extradis;

    //clicking the target pixels if blue value < 30
    for (int j : y)
    {
        for (int i : x)
        {
            if (pPixels[(1079 - j) * 1980 + i].rgbBlue < 30)
            {
                click(i, j + dis);
            }
        }
    }

    delete[] pPixels;
}

void click(int x, int y)
{
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}