#include<iostream>
#include<fstream>
#include <string>
#include<windows.h>
#include<conio.h>
using namespace std;

#pragma pack(1)
struct header
{
    int16_t header;
    int32_t filesize;
    int16_t reser;
    int16_t reser1;
    int32_t dataoffset;
};

struct infoheader
{
    int32_t headersize;
    int32_t width;
    int32_t height;
    int16_t plans;
    int16_t bpp;
    int32_t compression;
    int32_t datasize;
    int32_t re;
    int32_t ve;
    int32_t color;
    int32_t importantcolor;
};


struct  PIxel
{
    unsigned char G;
    unsigned char B;
    unsigned char R;
    unsigned char A;
};
struct Picture
{
    PIxel** A;
    int countheight;
    int countwidth;
};
PIxel Avg(PIxel in[9], int n)
{
    PIxel result = {0};
    int suma = 0, sumb = 0, sumc = 0, sumd = 0;
    for (int i = 0; i < n; i++)
    {
        suma += in[i].A;
        sumb += in[i].R;
        sumc += in[i].G;
        sumd += in[i].B; 
    }
    result.A = suma / n;
    result.B = sumd / n;
    result.G = sumc / n;
    result.R = sumb/n;
    return result;
}
Picture Zoomout(infoheader& info, PIxel** &A,HDC &hdc)
{
    Picture newpic;
     newpic.A = new PIxel * [(info.height) / 3 + 1];
    for (int i = 0; i < info.height / 3 + 1; i++)
    {
        newpic.A[i] = new PIxel[info.width / 3 + 1];
    }
    newpic.countheight = 0;
    newpic.countwidth = 0;
    for (int i = 0; i < info.height; i += 3)
    {
        for (int j = 0; j < info.width; j += 3)
        {
            int counting = 0;
            PIxel temp[9];
            for (int k = 0; k < 3; k++)
            {
                for (int o = 0; o < 3; o++)
                {
                    if ((i + k) < info.height && (j + o) < info.width)
                    {
                        temp[counting].A = A[i + k][j + o].A;
                        temp[counting].B = A[i + k][j + o].B;
                        temp[counting].G = A[i + k][j + o].G;
                        temp[counting].R = A[i + k][j + o].R;
                        counting++;
                    }
                }
            }
            newpic.A[newpic.countheight][newpic.countwidth] = Avg(temp, counting);
            newpic.countwidth++;
        }
        newpic.countheight++;
        newpic.countwidth = 0;
    }
    for (int i = 0; i < info.height / 3 + 1; i++)
    {
        for (int j = 0; j < info.width / 3 + 1; j++)
        {
            PIxel n = newpic.A[i][j];
            SetPixel(hdc, j + 20+ info.width + 60, i + 20, RGB(n.R, n.G, n.B));
        }
    }
    newpic.countheight = info.height / 3 + 1;
    newpic.countwidth = info.width / 3 + 1;
    return newpic;
}
int main()
{
    cout << 1 << " ";
    header h;
    infoheader info;
    PIxel* p;
    PIxel** A;
    //for (int i = 0; i <= 2; i++)
    {
        string name = "Damn2.bmp";
       /* if (i == 2) name = name + "2.bmp";
        else
        {
            name += ".bmp";
        }*/
        //getline(cin, name);

        ifstream file(name, ios::binary);
        if (file.is_open())
        {

            cout << "true" << endl;
            file.read((char*)&h, sizeof(h));
            if (h.header == 0x4D42)
            {
                cout << 1 << endl;
                file.read((char*)&info, sizeof(info));
                cout << info.width << " " << info.height << " " << h.filesize << " " << info.bpp << endl;
                // int pa = 3;
                cout << info.bpp << endl;
                int size = info.width * info.height * info.bpp / 8;
                char* arr = new char[size];
                file.read(arr, size);
                char* temp = arr;
                int sizep = info.height * info.width;
                p = new PIxel[sizep];
                for (int i = info.height - 1; i >= 0; i--)
                {
                    for (int j = 0; j < info.width; j++)
                    {
                        //temp++;
                        int index = i * info.width + j;
                        p[index].B = *(temp++);
                        p[index].G = *(temp++);
                        p[index].R = *(temp++);
                        p[index].A = *(temp++);
                    }
                    // temp += 1;
                }

                for (int i = 0; i < info.height; i++)
                {
                    cout << endl;
                }
                HWND consoleWindow = GetConsoleWindow();
                HDC hdc = GetDC(consoleWindow);
                int Move = 20;
                A = new PIxel*[info.height];
                for (int i = 0; i < info.height; i++)
                {
                    A[i] = new PIxel[info.width];
                }
                for (int i = 0; i < info.height; i++)
                {
                    for (int j = 0; j < info.width; j++)
                    {
                        int index = i * info.width + j;
                        //int index2 = i * info.width + j+ info.width/2;
                        
                        PIxel m = p[index];
                       // PIxel n = p[index2];
                       
                        if (j > Move)
                        {
                            SetPixel(hdc, j, i, RGB(m.R, m.G, m.B));
                            A[i][j - Move ] = m;
                        }
                        else
                        {
                            SetPixel(hdc, j + info.width, i, RGB(m.R, m.G, m.B));
                            A[i][j - Move +info.width] = m;
                        }
                        /*if (j > Move)   SetPixel(hdc, j +  info.width/2, i, RGB(n.R, n.G, n.B));
                        else
                        {
                            SetPixel(hdc, j + info.width/2  , i, RGB(n.R, n.G, n.B));
                        }*/
                       
                    }
                    //cout << endl;
                }
              /*  for (int i = 0; i < info.height; i++)
                {
                    for (int j = 0; j < info.width; j++)
                    {
                        PIxel n = A[i][j];
                        SetPixel(hdc, j+Move, i+info.height+Move, RGB(n.R, n.G, n.B));
                    }
                }*/
                Picture newpic= Zoomout(info, A, hdc);
                for (int i = 0; i < newpic.countheight; i++)
                {
                    for (int j = 0; j < newpic.countwidth; j++)
                    {
                        PIxel n = newpic.A[i][j];
                        SetPixel(hdc, j + 20 + info.width + 60, i + newpic.countheight +20 + 20,RGB( (n.R+n.B+n.G)/3, (n.R + n.B + n.G) / 3, (n.R + n.B + n.G) / 3));
                    }
                }
                ReleaseDC(consoleWindow, hdc);
                // system("pause");
            }
            else
            {
                cout << 0;
            }
        }
        /*for (int i = 0; i < info.height; i++)
        {
            cout << endl;
       }*/
    }
    while(!_kbhit())
        Sleep(1000);
 
    return 0;
}